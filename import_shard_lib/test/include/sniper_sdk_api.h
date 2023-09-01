#ifndef SNIPER_SDK_API_H
#define SNIPER_SDK_API_H

#if defined(__WIN32) || defined(__WIN64)
    #ifdef SNIPER_SDK_API_EXPORT
        #define SNIPER_SDK_API_EXPORT __declspec(dllexport)
    #else
        #define SNIPER_SDK_API_EXPORT __declspec(dllimport)
    #endif
#else
    #define SNIPER_SDK_API_EXPORT
#endif

extern "C"
{
    SNIPER_SDK_API_EXPORT void start_client_export();
}

#endif