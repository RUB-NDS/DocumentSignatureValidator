# Exploits
The exploits are divided into 5 attacks and are operating system independent:

01: Macro Manipulation with Certificate Doubling
02: Content Manipulation with Certificate Doubling
03: Content Manipulation with Certificate Validation Bypass
04: Content Manipulation with Signature Upgrade
05: Timestamp Manipulation with Signature Wrapping

## Impact
Signatures are valid and the signer is trusted.

01: The attacker chooses the content + macros of the document and changes the signer of the macros to "Trusted Person". More examples of macro exploits, see below. 
02: The attacker chooses the content of the document and changes the signer of the document to "Trusted Person".
03: The attacker chooses the content of the document and changes the signer of the document to any person.
04: The attacker chooses the content of the document. Signer remains unchanged.
05: The attacker changes the timestamp of the signed document. Signer remains unchanged.

## Trustness
Except for exploit 03, the attacks require that the included certificate from "Trusted Person" on the victim's system is trusted.

## ODF Applications
Exploits for LibreOffice, Collabora, Microsoft Office are based on XAdES signatures. Since OpenOffice, NeoOffice, Lotus Symphony and AO Office do not support XAdES signatures, the exploits are not executable for these applications.

Exploits for OpenOffice, NeoOffice, Lotus Symphony, and AO Office are built based on normal XML signatures. These exploits are also executable for LibreOffice and Collabora.


## Further Macro Exploit Examples
These exploits are of attack class "01: Macro Manipulation with Certificate Doubling" and are designed for Windows operating systems.

### exe_download_execute:
The included macro downloads an .exe file from https://github.com/attodf/odf-test when the document is opened and saves it to C:\Users\%USERNAME%\AppData\Local\Temp, then automatically executes the program. The program is harmless and does not contain any malicious code. It just outputs a text on the console.

### ransomware:
The included macro creates the file "example_ransomware.py" under C:\Users\%USERNAME%\AppData\Local\Temp. Then, this Python script is executed, using the Python environment of the respective office application, which can be found under C:\Program Files\%ODF-Application%\program\python.exe. This ransomware simulation serves as a PoC and is not supposed to do any damage, so it only creates a hashed file with .hashed extension from each file under C:\Users\%USERNAME%\Desktop. The function to delete the original files is not active in the Python code.

