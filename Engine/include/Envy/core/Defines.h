#pragma once

// TODO: ENVY_API
// API macro derived from Raylib
// Source code :https://github.com/raysan5/raylib/blob/master/src/raylib.h

#if defined(_WIN32)
    #if defined(__TINYC__)
        #define __declspec(x) __attribute__((x))
    #endif
    #if defined(ENVY_BUILD_SHARED)
        #define ENVY_API __declspec(dllexport)     // Building the library as a Win32 shared library (.dll)
    #elif defined(ENVY_BUILD_USE_SHARED)
        #define ENVY_API __declspec(dllimport)     // Using the library as a Win32 shared library (.dll)
    #endif
#else
    #if defined(ENVY_BUILD_SHARED)
        #define ENVY_API __attribute__((visibility("default"))) // Building as a Unix shared library (.so/.dylib)
    #endif
#endif

#ifndef ENVY_API
    #define ENVY_API       // Functions defined as 'extern' by default (implicit specifiers)
#endif

#define ENVY_NAMESPACE_START namespace Envy {
#define ENVY_NAMESPACE_END }
#if __cplusplus >= 201703L
    #define ENVY_NODISCARD [[nodiscard]]
#else
    #define ENVY_NODISCARD
#endif
