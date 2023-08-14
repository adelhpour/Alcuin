#define MyAppName "NetworkEditorGUITestApp"
#define MyAppExeName "NetworkEditorGUITestApp.exe"
#define MyAppVersion "0.1.2"
#define MyAppPublisher "Herbert Sauro's Sys-Bio Lab"
#define MyAppURL "https://github.com/adelhpour/NetworkEditorGUI"
#define MyAppID 00000000-0000-000-0000-000000000000

[Setup]
AppId={#MyAppID}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName="{autopf}\{#MyAppName}"
DefaultGroupName="{#MyAppName}"
OutputBaseFilename="{#MyAppName}-{#MyAppVersion}Setup"
Compression=lzma2
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
SolidCompression=yes
WizardStyle=modern
DisableWelcomePage=no
AlwaysShowDirOnReadyPage=yes
Uninstallable=yes


[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked

[Icons]
Name: "{group}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:ProgramOnTheWeb,{#MyAppName}}"; Filename: "{#MyAppURL}"
Name: "{userdesktop}\{#MyAppName}"; FileName: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Files]
Source: "C:\Users\adel\Documents\Projects\Made\5\install_app\bin\*"; DestDir: {app}; Flags: ignoreversion recursesubdirs createallsubdirs

[Run]
FileName: {app}\{#MyAppExeName}; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: nowait postinstall skipifsilent