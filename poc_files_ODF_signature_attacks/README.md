# Exploits
The exploits are divided into 5 attacks and are operating system independent:

01: Macro Manipulation with Certificate Doubling<br>
02: Content Manipulation with Certificate Doubling<br>
03: Content Manipulation with Certificate Validation Bypass<br>
04: Content Manipulation with Signature Upgrade<br>
05: Timestamp Manipulation with Signature Wrapping<br>

## Impact
Signatures are valid and the signer is trusted.<br>
<br>
01: The attacker chooses the content + macros of the document and changes the signer of the macros to "Trusted Person". More examples of macro exploits, see below.<br>
02: The attacker chooses the content of the document and changes the signer of the document to "Trusted Person".<br>
03: The attacker chooses the content of the document and changes the signer of the document to any person.<br>
04: The attacker chooses the content of the document. Signer remains unchanged.<br>
05: The attacker changes the timestamp of the signed document. Signer remains unchanged.<br>

## Trustness
Except for exploit 03, the attacks require that the included certificate from "Trusted Person" on the victim's system is trusted.

## ODF Applications
Exploits for LibreOffice, Collabora, Microsoft Office are based on XAdES signatures. Since OpenOffice, NeoOffice, Lotus Symphony and AO Office do not support XAdES signatures, the exploits are not executable for these applications.<br>
Exploits for OpenOffice, NeoOffice, Lotus Symphony, and AO Office are built based on normal XML signatures. These exploits are also executable for LibreOffice and Collabora.

## Further Macro Exploit Examples
These exploits are of attack class "01: Macro Manipulation with Certificate Doubling" and are designed for Windows operating systems.

### exe_download_execute:
The included macro downloads an .exe file from https://github.com/attodf/odf-test when the document is opened and saves it to C:\Users\%USERNAME%\AppData\Local\Temp, then automatically executes the program. The program is harmless and does not contain any malicious code. It just outputs a text on the console.

### ransomware:
The included macro creates the file "example_ransomware.py" under C:\Users\\%USERNAME%\AppData\Local\Temp. Then, this Python script is executed, using the Python environment of the respective office application, which can be found under C:\Program Files\%ODF-Application%\program\python.exe. This ransomware simulation serves as a PoC and is not supposed to do any damage, so it only creates a hashed file with .hashed extension from each file under C:\Users\%USERNAME%\Desktop. The function to delete the original files is not active in the Python code.

## CVEs
2021-25633, 2021-25634, 2021-25635, 2021-41830, 2021-41831, 2021-41832

### Affected Applications
<table>
  <thead>
    <tr>
      <th>Applikation</th>
      <th>Version</th>
	  <th>OS</th>
	  <th>Attack Class</th>
    </tr>
  </thead>
  <tbody>
	<tr>
      <td>AO Office</td>
      <td>4.1.6</td>
	  <td>iOS</td>
	  <td>2, 5</td>
    </tr>
    <tr>
      <td>Apache OpenOffice</td>
      <td>4.1.8</td>
	  <td>Win, macOS, Linux</td>
	  <td>1, 2, 3 (Win), 5</td>
    </tr>
	<tr>
      <td>Collabora Office</td>
      <td>6.2-20210530</td>
	  <td>Win, macOS, Linux</td>
	  <td>1, 2, 3 (Win), 5</td>
    </tr>
	<tr>
      <td>Collabora Office</td>
      <td>6.4.11-2</td>
	  <td>iOS</td>
	  <td>2, 5</td>
    </tr>
	<tr>
      <td>Collabora Office</td>
      <td>6.4.3</td>
	  <td>Android</td>
	  <td>2, 5</td>
    </tr>
	<tr>
      <td>Collabora Online (CODE)</td>
      <td>6.0-18</td>
	  <td>Online</td>
	  <td>2, 5</td>
    </tr>
	<tr>
      <td>IBM Lotus Symphony</td>
      <td>3.0.1 fp2</td>
	  <td>Win, Linux</td>
	  <td>1, 2, 5</td>
    </tr>
	<tr>
      <td>LibreOffice</td>
      <td>7.0.4.2</td>
	  <td>Win, macOS, Linux</td>
	  <td>1, 2, 3 (Win), 5</td>
    </tr>
	<tr>
      <td>Microsoft Office 2019</td>
      <td>16.0.10374.20040</td>
	  <td>Win</td>
	  <td>4</td>
    </tr>
	<tr>
      <td>NeoOffice</td>
      <td>2017.27</td>
	  <td>macOS</td>
	  <td>1, 2, 5</td>
    </tr>
  </tbody>
</table>

### Known Fixed Applications

<table>
  <thead>
    <tr>
      <th>Applikation</th>
      <th>Version</th>
    </tr>
  </thead>
  <tbody>
	<tr>
      <td>Apache OpenOffice</td>
      <td>4.1.11</td>
    </tr>
	<tr>
      <td>Collabora</td>
      <td>6.2-33, 6.4.14</td>
    </tr>
	<tr>
      <td>LibreOffice</td>
      <td>7.0.2, 7.1.2</td>
    </tr>
	<tr>
      <td>NeoOffice</td>
      <td>2017.31</td>
    </tr>
  </tbody>
</table>
