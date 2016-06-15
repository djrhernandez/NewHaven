// Created by Junior on 6/11/2016.
#ifndef WINDOWCOMPAT_H
#define WINDOWCOMPAT_H
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef _WIN32
#include <windows.h>
    //Disable 'const double to float truncation' warning
    #pragma warning (disable:4305)
    //Disable 'double to float truncation
    #pragma warning (disable:4244)
    //Disable 'const int to char truncation' warning
    #pragma warning (disable:4309)
    //Disable 'int to bool' warning
    #pragma warning (disable:4800)
#endif

#endif //WINDOWCOMPAT_H
