#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
    #ifdef starisk_core_EXPORTS
        //starisk_core.dll -> Export the symbols
        #define STARISK_API __declspec(dllexport)
    #else
        //starisk_core.dll -> Import the symbols
        #define STARISK_API __declspec(dllimport)
    #endif
#else
    // Linux / macOS: mark symbols visible
   
    #define STARISK_API __attribute__((visibility("default")))
#endif