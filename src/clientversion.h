#ifndef CLIENTVERSION_H
#define CLIENTVERSION_H

//
// client versioning
//

#ifdef BUILD_DAEMON
    #define CLIENT_VERSION_BUILD        3
#else
    #if (defined (_WIN32) || defined (_WIN64))
        #define CLIENT_VERSION_BUILD    1
    #elif (defined (LINUX) || defined (__linux__))
        #define CLIENT_VERSION_BUILD    2
    #else
        #define CLIENT_VERSION_BUILD    4
    #endif
#endif

// These need to be macros, as version.cpp's and bitcoin-qt.rc's voodoo requires it
#define CLIENT_VERSION_MAJOR       1
#define CLIENT_VERSION_MINOR       4
#define CLIENT_VERSION_REVISION    4

// Converts the parameter X to a string after macro replacement on X has been performed.
// Don't merge these into one macro!
#define STRINGIZE(X) DO_STRINGIZE(X)
#define DO_STRINGIZE(X) #X

#endif // CLIENTVERSION_H
