#ifndef DEFAULTS_H
#define DEFAULTS_H

#endif // DEFAULTS_H

#define DEFAULT_LANGUAGE "de"

#define DEFAULT_SHOW_DEBUGLOG   false

#define DEFAULT_SOURCE_URL      "https://raw.githubusercontent.com/nightflyer88/JetiAppManager/master/default.json"

#define TRANSMITTER_CONFIGFILE  "/Config/Config.jsn"
#define TRANSMITTER_APPFOLDER   "/Apps"


#ifdef __APPLE__
    #define FONTSIZE_APP_DESCRIPTION    16
    #define FONTSIZE_APP_VERSION        10
    #define FONTSIZE_APP_AUTHOR         10
    #define FONTSIZE_APP_TAG            9
#elif _WIN32
    #define FONTSIZE_APP_DESCRIPTION    14
    #define FONTSIZE_APP_VERSION        9
    #define FONTSIZE_APP_AUTHOR         9
    #define FONTSIZE_APP_TAG            7
#endif
