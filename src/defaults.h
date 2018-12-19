#ifndef DEFAULTS_H
#define DEFAULTS_H

#endif // DEFAULTS_H

// Version
#define JETIAPPMANAGER_VERSION          "V0.7 beta"


// Default settings
#define DEFAULT_LANGUAGE                "de"
#define DEFAULT_SHOW_NEWAPPS            true
#define DEFAULT_SHOW_APPUPDATES         true
#define DEFAULT_SHOW_DEBUGLOG           false
#define DEFAULT_SOURCE_URL              "https://raw.githubusercontent.com/nightflyer88/JetiAppManager/master/default.json"


// Transmitter paths
#define TRANSMITTER_CONFIGFILE          "/Config/Config.jsn"
#define TRANSMITTER_APPFOLDER           "/Apps"


// Help file
#define HELP_FILE_URL                   "https://raw.githubusercontent.com/nightflyer88/JetiAppManager/master/README.md"
#define HELP_FILE_URL_DE                "https://raw.githubusercontent.com/nightflyer88/JetiAppManager/master/README_de.md"


// Font sizes
#if defined(__APPLE__) || defined(__linux__)
    #define FONTSIZE_APP_DESCRIPTION    16
    #define FONTSIZE_APP_VERSION        10
    #define FONTSIZE_APP_AUTHOR         10
    #define FONTSIZE_APP_TAG            9
    #define FONTSIZE_APP_INSTALLED      8
    #define FONTSIZE_APP_UPDATE         9
    #define FONTSIZE_APP_NEW            14
#elif _WIN32
    #define FONTSIZE_APP_DESCRIPTION    14
    #define FONTSIZE_APP_VERSION        9
    #define FONTSIZE_APP_AUTHOR         9
    #define FONTSIZE_APP_TAG            7
    #define FONTSIZE_APP_INSTALLED      7
    #define FONTSIZE_APP_UPDATE         8
    #define FONTSIZE_APP_NEW            12
#endif
