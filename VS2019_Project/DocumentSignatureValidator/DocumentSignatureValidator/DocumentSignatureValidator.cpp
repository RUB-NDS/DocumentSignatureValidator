#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <windows.h>
#include <iterator>
#include <filesystem>
#include <tlhelp32.h>
#include <codecvt>
#include <atlimage.h>
#include "..\rapidxml-1.13\rapidxml.hpp"
#include "..\rapidxml-1.13\rapidxml_utils.hpp"

using namespace std;
namespace fs = std::filesystem;

void WINAPI SaveScreenshot(const char* path)
{
    //Source: https://stackoverflow.com/a/53421510 and https://stackoverflow.com/a/36545132
    BITMAPFILEHEADER bfHeader;
    BITMAPINFOHEADER biHeader;
    BITMAPINFO bInfo;
    HGDIOBJ hTempBitmap;
    HBITMAP hBitmap;
    BITMAP bAllDesktops;
    HDC hDC, hMemDC;
    LONG lWidth, lHeight;
    BYTE* bBits = NULL;
    HANDLE hHeap = GetProcessHeap();
    DWORD cbBits, dwWritten = 0;
    INT x = GetSystemMetrics(SM_XVIRTUALSCREEN);
    INT y = GetSystemMetrics(SM_YVIRTUALSCREEN);

    ZeroMemory(&bfHeader, sizeof(BITMAPFILEHEADER));
    ZeroMemory(&biHeader, sizeof(BITMAPINFOHEADER));
    ZeroMemory(&bInfo, sizeof(BITMAPINFO));
    ZeroMemory(&bAllDesktops, sizeof(BITMAP));

    hDC = GetDC(NULL);
    hTempBitmap = GetCurrentObject(hDC, OBJ_BITMAP);
    GetObjectW(hTempBitmap, sizeof(BITMAP), &bAllDesktops);

    lWidth = bAllDesktops.bmWidth;
    lHeight = bAllDesktops.bmHeight;

    DeleteObject(hTempBitmap);

    bfHeader.bfType = (WORD)('B' | ('M' << 8));
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biBitCount = 24;
    biHeader.biCompression = BI_RGB;
    biHeader.biPlanes = 1;
    biHeader.biWidth = lWidth;
    biHeader.biHeight = lHeight;

    bInfo.bmiHeader = biHeader;

    cbBits = (((24 * lWidth + 31) & ~31) / 8) * lHeight;

    hMemDC = CreateCompatibleDC(hDC);
    hBitmap = CreateDIBSection(hDC, &bInfo, DIB_RGB_COLORS, (VOID**)&bBits, NULL, 0);
    SelectObject(hMemDC, hBitmap);
    BitBlt(hMemDC, 0, 0, lWidth, lHeight, hDC, x, y, SRCCOPY);

    vector<BYTE> buf;
    IStream* stream = NULL;
    HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
    CImage image;
    ULARGE_INTEGER liSize;

    image.Attach(hBitmap);
    image.Save(stream, Gdiplus::ImageFormatJPEG);
    IStream_Size(stream, &liSize);
    DWORD len = liSize.LowPart;
    IStream_Reset(stream);
    buf.resize(len);
    IStream_Read(stream, &buf[0], len);
    stream->Release();

    fstream fi;
    fi.open(path, fstream::binary | fstream::out);
    fi.write(reinterpret_cast<const char*>(&buf[0]), buf.size() * sizeof(BYTE));
    fi.close();

    DeleteDC(hMemDC);
    ReleaseDC(NULL, hDC);
    DeleteObject(hBitmap);
}

//UTF-16
int findStringU16(HANDLE process, string const& testStr, u16string const& searchStrA, u16string const& searchStrA2, u16string const& searchStrB, u16string const& searchStrB2, u16string const& searchStrC, u16string const& searchStrC2, int minFounds) {

    unsigned char* p = NULL;
    int foundsA = 0;
    int foundsA2 = 0;
    int foundsB = 0;
    int foundsB2 = 0;
    int foundsC = 0;
    int foundsC2 = 0;
    MEMORY_BASIC_INFORMATION info;

    int result = 0; //0 = nothing found;
    try
    {
        for (p = NULL; VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info); p += info.RegionSize)
        {
            vector<wchar_t> buffer;
            vector<char>::iterator pos;

            try
            {
                if (info.State == MEM_COMMIT)
                {
                    SIZE_T bytes_read;
                    buffer.resize(info.RegionSize);
                    //Start reading process memory
                    ReadProcessMemory(process, p, &buffer[0], info.RegionSize, &bytes_read);

                    buffer.resize(bytes_read);

                    vector<wchar_t>::iterator found;

                    //Search for test string
                    found = search(buffer.begin(), buffer.end(), testStr.begin(), testStr.end());
                    if (testStr.empty() || found != buffer.end())
                    {
                        //Search for string A
                        if (!searchStrA.empty())
                        {
                            found = search(buffer.begin(), buffer.end(), searchStrA.begin(), searchStrA.end());
                            if (found != buffer.end())
                            {
                                foundsA++;
                                if (foundsA >= minFounds)
                                {
                                    return 1; //1 = string Afound 
                                }
                            }
                        }

                        //Search for string A2
                        if (!searchStrA2.empty())
                        {
                            found = search(buffer.begin(), buffer.end(), searchStrA2.begin(), searchStrA2.end());
                            if (found != buffer.end())
                            {
                                foundsA2++;
                                if (foundsA2 >= minFounds)
                                {
                                    return 2; //2 = string A2 found 
                                }
                            }
                        }

                        //Search for string B
                        if (!searchStrB.empty())
                        {
                            found = search(buffer.begin(), buffer.end(), searchStrB.begin(), searchStrB.end());
                            if (found != buffer.end())
                            {
                                foundsB++;
                                if (foundsB >= minFounds)
                                {
                                    return 3; //3 = string B found 
                                }
                            }
                        }

                        //Search for string B2
                        if (!searchStrB2.empty())
                        {
                            found = search(buffer.begin(), buffer.end(), searchStrB2.begin(), searchStrB2.end());
                            if (found != buffer.end())
                            {
                                foundsB2++;
                                if (foundsB2 >= minFounds)
                                {
                                    return 4; //4 = string B2 found 
                                }
                            }
                        }

                        //Search for string C
                        if (!searchStrC.empty())
                        {
                            found = search(buffer.begin(), buffer.end(), searchStrC.begin(), searchStrC.end());
                            if (found != buffer.end())
                            {
                                foundsC++;
                                if (foundsC >= minFounds)
                                {
                                    return 5; //5 = string C found 
                                }
                            }
                        }

                        //Search for string C2
                        if (!searchStrC2.empty())
                        {
                            found = search(buffer.begin(), buffer.end(), searchStrC2.begin(), searchStrC2.end());
                            if (found != buffer.end())
                            {
                                foundsC2++;
                                if (foundsC2 >= minFounds)
                                {
                                    return 6; //6 = string C2 found 
                                }
                            }
                        }
                    }
                    else
                    {
                        result = 7; //7 = no test string found
                    }
                }
            }
            catch (exception& e)
            {
                std::cout << "Error in ReadProcessMemory:" << '\n' << e.what() << endl;
                return -1;
            }
        }
    }
    catch (exception& e)
    {
        std::cout << "Error in VirtualQueryEx:" << '\n' << e.what() << endl;
        return -1;
    }

    return result;
}

//UTF-8
int findStringU8(HANDLE process, string const& testStr, string const& searchStrA, string const& searchStrA2, string const& searchStrB, string const& searchStrB2, string const& searchStrC, string const& searchStrC2, int minFounds) {

    unsigned char* p = NULL;
    int foundsA = 0;
    int foundsA2 = 0;
    int foundsB = 0;
    int foundsB2 = 0;
    int foundsC = 0;
    int foundsC2 = 0;
    MEMORY_BASIC_INFORMATION info;

    int result = 0; //nothing found
    try
    {
        for (p = NULL; VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info); p += info.RegionSize)
        {
            vector<char> buffer;
            vector<char>::iterator pos;

            try
            {
                if (info.State == MEM_COMMIT)
                {
                    SIZE_T bytes_read;
                    buffer.resize(info.RegionSize);
                    //Start reading process memory
                    ReadProcessMemory(process, p, &buffer[0], info.RegionSize, &bytes_read);

                    buffer.resize(bytes_read);

                    vector<char>::iterator found;

                    //Search for test string
                    found = search(buffer.begin(), buffer.end(), testStr.begin(), testStr.end());
                    if (testStr.empty() || found != buffer.end())
                    {
                        //Search for string A
                        if (!searchStrA.empty())
                        {
                            found = search(buffer.begin(), buffer.end(), searchStrA.begin(), searchStrA.end());
                            if (found != buffer.end())
                            {
                                foundsA++;
                                if (foundsA >= minFounds)
                                {
                                    return 1; //1 = string A found 
                                }
                            }
                        }

                        //Search for string A2
                        if (!searchStrA2.empty())
                        {
                            found = search(buffer.begin(), buffer.end(), searchStrA2.begin(), searchStrA2.end());
                            if (found != buffer.end())
                            {
                                foundsA2++;
                                if (foundsA2 >= minFounds)
                                {
                                    return 2; //2 = string A2 found 
                                }
                            }
                        }

                        //Search for string B
                        if (!searchStrB.empty())
                        {
                            found = search(buffer.begin(), buffer.end(), searchStrB.begin(), searchStrB.end());
                            if (found != buffer.end())
                            {
                                foundsB++;
                                if (foundsB >= minFounds)
                                {
                                    return 3; //3 = string B found 
                                }
                            }
                        }

                        //Search for string B2
                        if (!searchStrB2.empty())
                        {
                            found = search(buffer.begin(), buffer.end(), searchStrB2.begin(), searchStrB2.end());
                            if (found != buffer.end())
                            {
                                foundsB2++;
                                if (foundsB2 >= minFounds)
                                {
                                    return 4; //4 = string B2 found 
                                }
                            }
                        }

                        //Search for string C
                        if (!searchStrC.empty())
                        {
                            found = search(buffer.begin(), buffer.end(), searchStrC.begin(), searchStrC.end());
                            if (found != buffer.end())
                            {
                                foundsC++;
                                if (foundsC >= minFounds)
                                {
                                    return 5; //5 = string C found 
                                }
                            }
                        }

                        //Search for string C2
                        if (!searchStrC2.empty())
                        {
                            found = search(buffer.begin(), buffer.end(), searchStrC2.begin(), searchStrC2.end());
                            if (found != buffer.end())
                            {
                                foundsC2++;
                                if (foundsC2 >= minFounds)
                                {
                                    return 6; //6 = string C2 found 
                                }
                            }
                        }
                    }
                    else
                    {
                        result = 7; //7 = no test string found
                    }
                }
            }
            catch (exception& e)
            {
                std::cout << "Error in ReadProcessMemory:" << '\n' << e.what() << endl;
                return -1;
            }
        }
    }
    catch (exception& e)
    {
        std::cout << "Error in VirtualQueryEx:" << '\n' << e.what() << endl;
        return -1;
    }

    return result;
}

int startProcess(char process[], char argument[], int sleeptime, bool sleep)
{
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    int pid;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(process, argument, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        std::cout << ("CreateProcessA() failed (%d)\n", GetLastError()) << endl;
        return -1;
    }
    else
    {
        //Wait until process is loaded or until sleep time is over
        if (sleep == true)
        {
            Sleep(sleeptime * 1000);
        }
        else
        {
            WaitForInputIdle(pi.hProcess, sleeptime * 1000);
        }
        
        pid = pi.dwProcessId;
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return pid;
    }
}

void killProcess(int processID)
{
    HANDLE handler;
    handler = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, TRUE, processID);
    TerminateProcess(handler, 0);
}

int* GetProcessId(const wstring& processName, size_t size)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);
    int i = 0;
    int* ids = new int[size];

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        ids[0] = -1;
        return ids;
    }

    while (Process32Next(processesSnapshot, &processInfo))
    {
        if (!processName.compare(processInfo.szExeFile))
        {
            if (processInfo.th32ProcessID > 0 && i < int(size))
            {
                ids[i] = processInfo.th32ProcessID;
                i++;
            }
        }
    }

    CloseHandle(processesSnapshot);
    return ids;
}

int main(int argc, char** argv) {
    std::cout << "Start" << endl;

    //default values
    int sleeptime = 1;
    int maxProcesses = 5;
    int minFounds = 1;
    int found = -1;
    int result = -1;
    int i = 0;
    int countApps = 0;
    int countFiles = 0;
    bool sleep = false;
    bool u16 = true;
    string xmlConfig;
    string name;
    string pathFil;
    string arg;
    string ext;
    string pre;
    string str_process;
    size_t sub;
    string processName;
    wstring processNameW;
    HANDLE processH;
    ofstream csvfile;
    string filename;
    string dirname;
    string shotname;
    string csvString;
    string outputStr;
    time_t sec = std::time(&sec);
    stringstream ss;
    ss << sec;

    //Check if XML config was passed
    if (argc < 2)
    {
        std::cout << "Please pass the XML config file as argument." << endl;
        std::cout << "End" << endl;
        return -1;
    }
    else
    {
        xmlConfig = argv[1];
        if (xmlConfig.substr(xmlConfig.size() - 3) != "xml")
        {
            std::cout << "Please pass the XML config file as argument." << endl;
            std::cout << "End" << endl;
            return -1;
        }
    }

    try
    {
        rapidxml::file<> configFile(xmlConfig.c_str());
        rapidxml::xml_document<> doc;
        doc.parse<0>(configFile.data());
        rapidxml::xml_node<>* config = doc.first_node("config");
        if (config == nullptr)
        {
            std::cout << "No root element in config file found." << endl;
            return -1;
        }
        else
        {
            try
            {
                //Get files
                rapidxml::xml_node<>* files = config->first_node("files");
                name = files->name();
                if (name != "files")
                {
                    std::cout << "Error get files element from XML config" << endl;
                    return -1;
                }
                rapidxml::xml_node<>* pathFiles = files->first_node("path");
                rapidxml::xml_node<>* arguments = files->first_node("arguments");
                rapidxml::xml_node<>* extension = files->first_node("extension");

                pathFil = pathFiles->value();
                arg = arguments->value();
                ext = extension->value();

                //Get output directory
                rapidxml::xml_node<>* output = config->first_node("output");
                name = output->name();
                if (name != "output")
                {
                    std::cout << "Error get output element from XML config" << endl;
                    return -1;
                }

                rapidxml::xml_node<>* pathOutput = output->first_node("path");
                string pathOut = pathOutput->value();
                if (empty(pathOut))
                {
                    std::cout << "Error no output path in XML config." << endl;
                    return -1;
                }

                //Create output file
                try
                {
                    filename += pathOut;
                    if (filename.back() != '\\')
                    {
                        filename += "\\";
                    }
                    filename += "output_";
                    filename += ss.str();
                    filename += ".csv";
                    csvfile.open(filename);
                    csvfile << "Application;Filename;Signature Status;Signature String\n";

                }
                catch (exception& e)
                {
                    std::cout << "Error create CSV file." << '\n' << e.what() << endl;
                    return -1;
                }

                //Create screenshot folder
                dirname += pathOut;
                if (dirname.back() != '\\')
                {
                    dirname += "\\";
                }
                dirname += "screenshots_";
                dirname += ss.str();
                fs::create_directory(dirname);

            }
            catch (exception& e)
            {
                std::cout << "Error get files value from XML config." << '\n' << e.what() << endl;
                return -1;
            }

            rapidxml::xml_node<>* pathApp;
            rapidxml::xml_node<>* overwriteprocessname;
            rapidxml::xml_node<>* encoding;
            rapidxml::xml_node<>* teststring;
            rapidxml::xml_node<>* searchStringA;
            rapidxml::xml_node<>* searchStringA2;
            rapidxml::xml_node<>* searchStringB;
            rapidxml::xml_node<>* searchStringB2;
            rapidxml::xml_node<>* searchStringC;
            rapidxml::xml_node<>* searchStringC2;
            rapidxml::xml_node<>* sleepvalue;
            rapidxml::xml_node<>* minfoundvalues;

            char* process;
            string overwriteprocessnameStr;
            string encodingStr;
            string testStr;
            string searchStrA;
            u16string searchStrAu16;
            string searchStrA2;
            u16string searchStrA2u16;
            string searchStrB;
            u16string searchStrBu16;
            string searchStrB2;
            u16string searchStrB2u16;
            string searchStrC;
            u16string searchStrCu16;
            string searchStrC2;
            u16string searchStrC2u16;
            string sleep_str;
            int sleep_int;
            string minFounds_str;
            int minFounds_int;
            string file;


            for (rapidxml::xml_node<>* application = config->first_node("application"); application; application = application->next_sibling())
            {
                name = application->name();
                if (name != "application")
                {
                    csvfile.close();
                    //Remove lock files
                    for (const auto& entry : fs::recursive_directory_iterator(pathFil))
                    {
                        file = entry.path().u8string();
                        filesystem::path p(file);
                        if (p.stem().u8string().substr(0, 6) == ".~lock" || p.stem().u8string().substr(0, 2) == "~$")
                        {
                            fs::remove(file);
                        }
                    }
                    std::cout << endl;
                    std::cout << "Total Applications: " << countApps << " | Total Files each Application: " << countFiles << endl;
                    std::cout << "Results: " << filename << endl;
                    std::cout << "End" << endl;
                    return 0;
                }
                countApps++;

                try
                {
                    pathApp = application->first_node("path");
                    overwriteprocessname = application->first_node("overwriteprocessname");
                    encoding = application->first_node("encoding");
                    teststring = application->first_node("teststring");
                    searchStringA = application->first_node("sigvalidstring");
                    searchStringA2 = application->first_node("sigvalidstring2");
                    searchStringB = application->first_node("siginvalidstring");
                    searchStringB2 = application->first_node("siginvalidstring2");
                    searchStringC = application->first_node("sigproblem");
                    searchStringC2 = application->first_node("sigproblem2");
                    sleepvalue = application->first_node("wait");
                    minfoundvalues = application->first_node("minfoundvalues");

                    process = pathApp->value();
                    overwriteprocessnameStr = overwriteprocessname->value();
                    encodingStr = encoding->value();
                    testStr = teststring->value();
                    searchStrA = searchStringA->value();
                    searchStrA2 = searchStringA2->value();
                    searchStrB = searchStringB->value();
                    searchStrB2 = searchStringB2->value();
                    searchStrC = searchStringC->value();
                    searchStrC2 = searchStringC2->value();
                    sleep_str = sleepvalue->value();
                    minFounds_str = minfoundvalues->value();

                    std::wstring_convert<std::codecvt<char16_t, char, std::mbstate_t>, char16_t> convert;
                    searchStrAu16 = convert.from_bytes(searchStrA);
                    searchStrA2u16 = convert.from_bytes(searchStrA2);
                    searchStrBu16 = convert.from_bytes(searchStrB);
                    searchStrB2u16 = convert.from_bytes(searchStrB2);
                    searchStrCu16 = convert.from_bytes(searchStrC);
                    searchStrC2u16 = convert.from_bytes(searchStrC2);

                    if (encodingStr == "utf8" || encodingStr == "utf-8" || encodingStr == "UTF-8")
                    {
                        u16 = false;
                    }
                    else
                    {
                        u16 = true;
                    }
                }
                catch (exception& e)
                {
                    std::cout << "Error get application values from XML config." << '\n' << e.what() << endl;
                    return -1;
                }

                try
                {
                    sleep_int = atoi(sleep_str.c_str());
                    if (sleep_int > 0)
                    {
                        sleep = true;
                        sleeptime = sleep_int;
                    }
                    else
                    {
                        sleep = false;
                    }

                    minFounds_int = atoi(minFounds_str.c_str());
                    if (minFounds_int > 0)
                    {
                        minFounds = minFounds_int;
                    }
                }
                catch (...) {}

                //Get process name with extension and convert to wstring
                str_process = process;
                sub = str_process.find_last_of("/\\");
                processName = str_process.substr(sub + 1);

                //Overwrite process name
                if (!overwriteprocessnameStr.empty())
                {
                    processName = overwriteprocessnameStr;
                }

                //convert process name with extension to wstring
                wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
                processNameW = converter.from_bytes(processName);

                //Get process name without extension
                if (!processName.empty())
                {
                    processName.resize(processName.size() - 4);
                }

                countFiles = 0;
                //Get files in directory
                for (const auto& entry : fs::recursive_directory_iterator(pathFil))
                {
                    file = entry.path().u8string();

                    //Remove lock files
                    filesystem::path p(file);
                    if (p.stem().u8string().substr(0, 6) == ".~lock" || p.stem().u8string().substr(0, 2) == "~$")
                    {
                        fs::remove(file);
                    }
                    else
                    {
                        if (entry.path().extension() == ext)
                        {
                            //Build file path
                            countFiles++;
                            pre = "\\C ";
                            pre += " ";
                            pre += arg;
                            pre += " ";
                            pre += file;
                            char* currentfile = new char[pre.size() + 1];
                            std::copy(pre.begin(), pre.end(), currentfile);
                            currentfile[pre.size()] = '\0';

                            //Start main process
                            result = startProcess(process, currentfile, sleeptime, sleep);
                            if (result > 0)
                            {
                                //Get array of process IDs with the same process name
                                int* processId = GetProcessId(processNameW, maxProcesses);

                                //Check for errors in GetProcessId. Casting maxProcesses to long long prevents the "Warning C26451"
                                if (sizeof(processId) >= ((long long)maxProcesses - 1) && processId[0] > 0)
                                {
                                    //Start findString() for all processes
                                    for (i = 0; i < sizeof(processId) + 1; i++)
                                    {
                                        if (processId[i] > 0)
                                        {
                                            //Open process with read rights
                                            processH = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, false, processId[i]);

                                            //Find string in process memory
                                            if (u16 == true)
                                            {
                                                found = findStringU16(processH, testStr, searchStrAu16, searchStrA2u16, searchStrBu16, searchStrB2u16, searchStrCu16, searchStrC2u16, minFounds);
                                            }
                                            else
                                            {
                                                found = findStringU8(processH, testStr, searchStrA, searchStrA2, searchStrB, searchStrB2, searchStrC, searchStrC2, minFounds);
                                            }

                                            CloseHandle(processH);
                                            if (found > 0 && found < 7)
                                            {
                                                result = found;
                                                break;
                                            }
                                            else if (found == 0 || found == 7)
                                            {
                                                result = found;
                                            }
                                        }
                                    }

                                    //Save screenshot
                                    shotname += dirname;
                                    shotname += "\\";
                                    shotname += processName;
                                    shotname += "_";
                                    filesystem::path p(file);
                                    shotname += p.stem().u8string();
                                    shotname += "_";
                                    shotname += to_string(countFiles);
                                    shotname += ".jpg";
                                    SaveScreenshot(shotname.c_str());

                                    //Kill processes
                                    for (i = 0; i < sizeof(processId) + 1; i++)
                                    {
                                        if (processId[i] > 0)
                                        {
                                            killProcess(processId[i]);
                                        }
                                    }

                                    //Write results
                                    outputStr += "Application: ";
                                    outputStr += processName;
                                    outputStr += " | File: ";
                                    outputStr += file;
                                    csvString += processName;
                                    csvString += ";";
                                    csvString += file;
                                    if (result == 0)
                                    {
                                        outputStr += " | No signature value found";
                                        csvString += ";No signature value found in memory";
                                    }
                                    else if (result == 1)
                                    {
                                        outputStr += " | Signature is valid";
                                        csvString += ";Signature is valid;";
                                        csvString += searchStrA;
                                    }
                                    else if (result == 2)
                                    {
                                        outputStr += " | Signature is valid";
                                        csvString += ";Signature is valid;";
                                        csvString += searchStrA2;
                                    }
                                    else if (result == 3)
                                    {
                                        outputStr += " | Signature is invalid";
                                        csvString += ";Signature is invalid;";
                                        csvString += searchStrB;
                                    }
                                    else if (result == 4)
                                    {
                                        outputStr += " | Signature is invalid";
                                        csvString += ";Signature is invalid;";
                                        csvString += searchStrB2;
                                    }
                                    else if (result == 5)
                                    {
                                        outputStr += " | Signature has problems";
                                        csvString += ";Signature has problems;";
                                        csvString += searchStrC;
                                    }
                                    else if (result == 6)
                                    {
                                        outputStr += " | Signature has problems";
                                        csvString += ";Signature has problems;";
                                        csvString += searchStrC2;
                                    }
                                    else if (result == 7)
                                    {
                                        outputStr += " | No test string found";
                                        csvString += ";No test string found: error while opening the file or sleep time to low";
                                    }
                                    else
                                    {
                                        outputStr += " | Error in findString()";
                                        csvString += ";Error in findString()";
                                    }
                                    csvString += "\n";
                                    csvfile << csvString;
                                    cout << outputStr << endl;
                                }
                                else
                                {
                                    std::cout << "\nError in GetProcessId()" << endl;
                                }

                                //Free arrays and strings
                                delete[] processId;
                                delete[] currentfile;
                                found = -1;
                                result = 0;
                                csvString = "";
                                shotname = "";
                                outputStr = "";
                            }
                        }
                    }
                }
            }
        }
    }
    catch (const exception& e)
    {
        std::cout << "Error." << '\n' << e.what() << endl;
        return -1;
    }

    csvfile.close();
    std::cout << "\nEnd" << endl;
    return 0;
}