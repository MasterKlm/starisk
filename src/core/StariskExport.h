#ifndef STARISK_EXPORT_H
#define STARISK_EXPORT_H

#ifdef _WIN32
    #ifdef starisk_core_EXPORTS
        //starisk_core.dll -> Export the symbols
        #define STARISK_API __declspec(dllexport)
    #else
        //starisk_core.dll -> Import the symbols
        #define STARISK_API __declspec(dllimport)
    #endif
#else
    // Non-Windows systems don't hide symbols by default
    #define STARISK_API
#endif

#endif