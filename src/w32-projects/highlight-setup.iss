; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
AppName=Highlight Code Converter

AppVerName=Highlight
OutputBaseFilename=highlight-setup-4.1

AppPublisher=Andr� Simon
AppPublisherURL=http://www.andre-simon.de
AppUpdatesURL=http://www.andre-simon.de
DefaultDirName={commonpf}\Highlight
DefaultGroupName=Highlight Code Converter
AllowNoIcons=yes
LicenseFile=..\..\COPYING
InfoAfterFile=..\..\README.adoc
Compression=lzma2/ultra 
SolidCompression=yes
PrivilegesRequired=none
WizardStyle=modern
SetupIconFile=hl_icon_exe.ico
UninstallDisplayIcon={app}\highlight-gui.exe

[Tasks]
Name: "desktopicon"; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"

[Files]
Source: "..\..\highlight-gui.exe"; DestDir: "{app}";  Flags: ignoreversion
Source: "hl_icon_exe.ico"; DestDir: "{app}";  Flags: ignoreversion
Source: "..\..\langDefs\*.lang"; DestDir: "{app}\langDefs\";  Flags: ignoreversion sortfilesbyextension
Source: "..\..\themes\*.theme"; DestDir: "{app}\themes\";  Flags: ignoreversion sortfilesbyextension
Source: "..\..\themes\base16\*.theme"; DestDir: "{app}\themes\base16\";  Flags: ignoreversion sortfilesbyextension
Source: "..\..\README*"; DestDir: "{app}";  Flags: ignoreversion
Source: "..\..\AUTHORS"; DestDir: "{app}";  Flags: ignoreversion
Source: "..\..\ChangeLog.adoc"; DestDir: "{app}";  Flags: ignoreversion
Source: "..\..\COPYING"; DestDir: "{app}";  Flags: ignoreversion
Source: "..\..\*.conf"; DestDir: "{app}";  Flags: ignoreversion
Source: "..\..\gui_files\l10n\*.qm"; DestDir: "{app}\gui_files\l10n\";  Flags: ignoreversion sortfilesbyextension
Source: "..\..\gui_files\ext\*.*"; DestDir: "{app}\gui_files\ext\";  Flags: ignoreversion
Source: "..\..\plugins\*.lua";  DestDir: "{app}\plugins\";  Flags: ignoreversion;

Source: "..\..\extras\*.py";  DestDir: "{app}\extras\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\..\extras\*.pm";  DestDir: "{app}\extras\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\..\extras\*.php";  DestDir: "{app}\extras\";  Flags: ignoreversion;Components: sourcecode;
Source: "..\..\extras\highlight_pipe.*";  DestDir: "{app}\extras\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\..\extras\swig\*.*";  DestDir: "{app}\extras\swig";  Flags: ignoreversion; Components: sourcecode;
Source: "..\..\extras\tcl\*.*";  DestDir: "{app}\extras\tcl";  Flags: ignoreversion; Components: sourcecode;
Source: "..\..\extras\pandoc\*.*";  DestDir: "{app}\extras\pandoc";  Flags: ignoreversion;  Components: sourcecode;
Source: "..\..\extras\langDefs-resources\*.*";  DestDir: "{app}\extras\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\..\extras\themes-resources\base16\*.*";  DestDir: "{app}\extras\themes-resources\";  Flags: ignoreversion;Components: sourcecode;
Source: "..\..\extras\themes-resources\css-themes\*.*";  DestDir: "{app}\extras\themes-resources\";  Flags: ignoreversion;Components: sourcecode;

Source: "..\..\highlight.exe";   DestDir: "{app}";  Flags: ignoreversion; Components: cli;
Source: "..\..\man\*.*";  DestDir: "{app}\man\";  Flags: ignoreversion; Components: cli;

Source: "..\include\*.*";  DestDir: "{app}\src\include\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\include\astyle\*.*";  DestDir: "{app}\src\include\astyle\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\include\Diluculum\*.*";  DestDir: "{app}\src\include\Diluculum\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\include\picojson\*.*";  DestDir: "{app}\src\include\picojson\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\core\*.*";  Excludes: "*.o,*.depend"; DestDir: "{app}\src\core\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\core\astyle\*.*";  Excludes: "*.o"; DestDir: "{app}\src\core\astyle\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\core\Diluculum\*.*";  Excludes: "*.o"; DestDir: "{app}\src\core\Diluculum\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\gui-qt\*.*";  Excludes: "*.o,*.Release,*.Debug,ui_*,*.user,*.qm,Makefile"; DestDir: "{app}\src\gui-qt\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\cli\*.*";  Excludes: "*.o"; DestDir: "{app}\src\cli\";  Flags: ignoreversion; Components: sourcecode;

Source: "..\w32-projects\*.iss";   DestDir: "{app}\src\projectfiles\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\w32-projects\highlight_cli\*.pro";   DestDir: "{app}\src\projectfiles\highlight_cli\";  Flags: ignoreversion; Components: sourcecode;
Source: "..\w32-projects\highlight_lib\*.pro";   DestDir: "{app}\src\projectfiles\highlight_lib\";  Flags: ignoreversion; Components: sourcecode;

[Dirs]
Name: "{userappdata}\Highlight"; Attribs: hidden

[Components]
Name: "main"; Description: "Highlight GUI"; Types: full compact custom; Flags: fixed
Name: "cli"; Description: "Highlight command line interface"; Types: full
Name: "sourcecode"; Description: "Source code"; Types: full
Name: "portable"; Description: "Portable starter icon"; Types: full


[INI]
Filename: "{app}\Highlight.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.andre-simon.de"

[Icons]
Name: "{group}\Highlight"; Filename: "{app}\highlight-gui.exe";  IconFilename: "{app}\hl_icon_exe.ico";WorkingDir: "{app}"
Name: "{group}\Highlight Portable"; Filename: "{app}\highlight-gui.exe";  IconFilename: "{app}\hl_icon_exe.ico";WorkingDir: "{app}";Parameters: "--portable";  Components: portable;
Name: "{group}\Highlight on the Web"; Filename: "{app}\Highlight.url"
Name: "{group}\Uninstall Highlight"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Highlight"; Filename: "{app}\highlight-gui.exe"; Tasks: desktopicon; IconFilename: "{app}\hl_icon_exe.ico";WorkingDir: "{app}"
Name: "{userdesktop}\Highlight Portable"; Filename: "{app}\highlight-gui.exe"; Tasks: desktopicon; IconFilename: "{app}\hl_icon_exe.ico";WorkingDir: "{app}";Parameters: "--portable";  Components: portable;

[Run]
Filename: "{app}\highlight-gui.exe"; Description: "Launch Highlight Code Converter"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\Highlight.url"
