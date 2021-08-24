# Document Signature Validator
Automation tool for searching for strings in an application's process memory to check whether a document contains a valid or invalid digital signature.

## Supported file types
OpenDocument Format (ODF), Office Open XML (OOXML), Portable Document Format (PDF)

## Start
Start the DocumentSignatureValidator.exe from the console with an XML config file as argument.

## Structure of an XML config file
```xml
<config>
	<application>
		<path>Path to the application to be tested</path>
		<overwriteprocessname>(optional) If the application starts a process whose name does not match the name of the .exe file</overwriteprocessname>
		<encoding>(optional) Encoding of the search strings: UTF-8 or UTF-16. Default: UTF-16</encoding>
		<teststring>(optional) String to test if the file was correctly loaded</teststring>
		<sigvalidstring>(optional) First valid signature string</sigvalidstring>
		<sigvalidstring2>(optional) Second valid signature string</sigvalidstring2>
		<siginvalidstring>(optional) First invalid signature string</siginvalidstring>
		<siginvalidstring2>(optional) Second invalid signature string</siginvalidstring2>
		<sigproblem>(optional) First signature has problems string</sigproblem>
		<sigproblem2>(optional) Second signature has problems string</sigproblem2>
		<wait>(optional) Time in seconds to wait for the application to load completely</wait>
		<minfoundvalues>(optional) Minimum number of identical test strings found for the search to be considered successful</minfoundvalues>
	</application>
	<files>
		<path>Path to the files to be tested</path>
		<arguments>(optional) Argument with which the file will be opened</arguments>
		<extension>File extension of the files to be tested</extension>
	</files>
	<output>
		<path>Path for the result file and screenshot directory</path>
	</output>
</config>
``` 

## XML parser
This tool use the RapidXml parser http://rapidxml.sourceforge.net/license.txt

## Version
0.1
