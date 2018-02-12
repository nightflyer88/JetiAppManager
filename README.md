# Jeti App Manager

The Jeti App Manager will list all online available LUA apps, and then you can
easy and fast to be installed on the Jeti DC/DS transmitter. Simply and conveniently select the app, in the upper left select the SD card of the transmitter,
and press on install - Done!

![preview](https://github.com/nightflyer88/JetiAppManager/blob/master/images/JetiAppManager.png?raw=true)

### Functionality

The Jeti App Manager is installed on the local computer (Mac or Windows), the LUA apps are integrated by a source file, so the user can decide for themselves which apps from which sources he would like to have.
However, the apps are not stored locally, but are only downloaded from the Internet during installation on the transmitter. This guarantees that the user is always up to date with the latest software.

Under *Settings*, app sources can still be defined. All you have to do is insert a link to this * .json source file (one link per line). This link to the * .json file must be provided by the app developer. Since the source file is also not stored locally, the latest app versions are always loaded, and new apps are automatically displayed by the app developer, without having to update anything.

### installation

Download the latest version of [Jeti App Manager](https://github.com/nightflyer88/JetiAppManager/releases) for Mac or Windows.

#### Mac

Download and open JetiAppManager.dmg, drag and drop the JetiAppManager.app into the program folder.

#### Windows

Download JetiAppManager_setup.exe, run it and follow the instructions. If the Jeti App Manager does not start, it may be necessary to install Visual Studio 2015 Express.

### source file

Any app developer who wants to offer his apps must create a JSON source file. The JSON source file will then contain all the necessary information about the app, including: required files, destination paths on the transmitter, minimum transmitter hardware and software requirements, app description, preview image, etc.

The source file can eg. in the app repository on GIT or anywhere else on the web. The user then only needs a link to the JSON file. This link can now be easily inserted under *File -> Settings* on a new line. If an app developer only wants to use his apps for himself and does not publish them, the app can also be saved on the local computer. For this, the JSON file must also be available on the local computer. The URL for local files must be specified as shown in the example below.

Example links:
```
https://raw.githubusercontent.com/nightflyer88/JetiAppManager/master/default.json	<- File is loaded from the web
file:///Users/path_to_file/test.json							<- File is loaded from the local computer
file:///c:/path_to_file/test.json							<- Path for a local file on a Windows machine
```

The source file must be in JSON format. With this online [tool](http://jsoneditoronline.org) the source file is created easily and quickly, and is also checked for syntax errors.

The source file can contain multiple apps and must be structured as in the following examples:

Example of multiple apps per source file:
```
{
  "GPS to QR-Code": {
    "author": "M. Lehmann",
    "version": "V1.1",
    "previewImg": "https://raw.githubusercontent.com/nightflyer88/Lua_gpsQRcode/master/img/qpsQRcode.jpg",
    "description": "https://raw.githubusercontent.com/nightflyer88/Lua_gpsQRcode/master/README.md",
    "sourceFile": [
      "https://raw.githubusercontent.com/nightflyer88/Lua_gpsQRcode/master/gpsQRcode.lc",
      "https://raw.githubusercontent.com/nightflyer88/Lua_gpsQRcode/master/gpsQRcode/gpsQRcode.jsn"
    ],
    "destinationPath": [
      "/Apps",
      "/Apps/gpsQRcode"
    ]
  },
  "Artificial Horizon": {
    "author": "JETI model",
    "version": "V1.0",
    "previewImg": "https://raw.githubusercontent.com/JETImodel/Lua-Apps/master/Img/Artificial%20Horizon/Horizon6.png",
    "description": "https://raw.githubusercontent.com/JETImodel/Lua-Apps/master/Artificial%20Horizon/README.md",
    "sourceFile": [
      "https://raw.githubusercontent.com/JETImodel/Lua-Apps/master/Artificial%20Horizon/Horizon.lua"
    ],
    "destinationPath": [
      "/Apps"
    ]
  }
}
```
The individual apps are defined in the JSON file as Object{}. The object name corresponds to the app name. In a JSON file, there is no limit on the maximum number of apps.


Example of an app running on every transmitter:
```
{
  "GPS to QR-Code": {                                   <-- Name of the app
    "author": "M. Lehmann",                             <-- Name of the author
    "version": "V1.1",                                  <-- current version
    "previewImg": "https://raw.git.../qpsQRcode.jpg",   <-- Link to the preview Image of the app, can be in *.jpeg, *.png or *.bmp format
    "description": "https://raw.git.../README.md",      <-- Link to the description must be in MarkDown format (*.md file, eg on git the README.md)
    "sourceFile": [                                     <-- List of files copied to the transmitter
      "https://raw.git.../gpsQRcode.lc",                <-- first file
      "https://raw.git.../gpsQRcode/gpsQRcode.jsn"      <-- second file
    ],
    "destinationPath": [                                <-- List of destination paths (absolute) on the transmitter
      "/Apps",                                          <-- first file is copied to this folder
      "/Apps/gpsQRcode"                                 <-- second file is copied to this folder
    ]
  }
}
```
If the app can be installed on any type of transmitter, the list of app files can be defined with the key word _**sourceFile**_. If the destination paths on the transmitter are not left over, they will be created. When uninstalling, empty folders in the /apps directory are automatically deleted.

Example of an app that requires the transmitter firmware 4.23 or higher, and contains different app versions for DC/DS14-16 or DC/DS24.
```
{
  "Battery Percentage": {                               <-- Name of the app
    "author": "RC-Thoughts",                            <-- Name of the author
    "version": "V2.4",                                  <-- current app-version
    "previewImg": "https://.../Jeti-Tools_small.png",   <-- Link to the preview Image of the app, can be in *.jpeg, *.png or *.bmp format
    "description": "https://.../README.md",             <-- Link to the description must be in MarkDown format (*.md file, eg on git the README.md)
    "requiredFirmware": 4.23,                           <-- required minimum transmitter firmware
    "sourceFile14_16": [                                <-- List of files copied to the transmitter when the transmitter type is DC/DS14-16
      "https://raw.git.../DCDS-1416/RCT-Batt.lc",
      "https://raw.git.../DCDS-1416/Lang/RCT-Batt.jsn"
    ],
    "sourceFile24": [                                   <-- List of files copied to the transmitter if the transmitter type is DC/DS24
      "https://raw.git.../DCDS-24/RCT-Batt.lc",
      "https://raw.git.../DCDS-24/Lang/RCT-Batt.jsn"
    ],
    "destinationPath": [                                <-- List of destination paths (absolute) on the transmitter
      "/Apps",
      "/Apps/Lang"
    ]
  }
}
```
If the app can only be installed on a DC/DS24, the file list is defined with the key word _**sourceFile24**_. For transmitter types DC/DS14-16, _**sourceFile14_16**_ is defined. The minimum required transmitter firmware is defined with the key word _**requiredFirmware**_ and as a double number.

Example of a multilingual app:
```
{
  "LUA App deutsch": {                                  <-- default name of the app (here in german)
    "author": "M. Lehmann",                             # default information of the app
    "version": "V1.1",                                  # If no other language is available this information will be used
    "previewImg": "https://raw.git.../qpsQRcode.jpg",
    "description": "https://raw.git.../README.md",
    "requiredFirmware": 4.23,
    "sourceFile24": [
      "https://raw.git.../gpsQRcode.lc"
    ],
    "destinationPath": [
      "/Apps"
    ],
    "en": {                                             # If this language (English) is set the new information is used
      "appName": "LUA App english",                     <-- new name of the app in English
      "description": "https://raw.git.../README_en.md"  <-- other file for the description
    },
    "cz": {
      "appName": "LUA App český",
      "description": "https://raw.git.../README_cz.md",
      "previewImg": "https://raw.git.../cz.jpg"         <-- another preview image for Czech
    },
    "it": {
      "description": "https://raw.git.../README_it.md"  <-- here only other description is defined, App name remains the same
    }
  }
}
```

Through the language objects, the standard app information can be overwritten by others. It can eg. a different app name, a different (translated) description, etc. can be defined. It is possible to override all standard information, including the _**sourceFile**_ and _**destinationPath**_ objects.

Currently the following languages are supported:

| Language     | Key	|
|--------------|--------|
| Deutsch      | de     |
| Englisch     | en     |
| Tschechisch  | cz     |
| Französisch  | fr     |
| Italienisch  | it     |
| Spanisch     | es     |
| Portugisisch | pt     |


All links to the files must be direct. Also the pictures which are linked in the description (* .md file) must
directly to the file and may not be redirected.

Example links:

This link is accepted: https://raw.githubusercontent.com/JETImodel/Lua-Apps/master/Img/Artificial%20Horizon/Horizon6.png

This link does not work: https://github.com/nightflyer88/JetiAppManager/blob/master/images/JetiAppManager.png?raw=true

You recognize the redirected link: **?raw=true**
