// Automatically generated C++ file on Tue Apr 14 07:47:38 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD abc2ab.cpp kernel32.lib

#include <math.h>

union uData
{
   bool b;
   char c;
   unsigned char uc;
   short s;
   unsigned short us;
   int i;
   unsigned int ui;
   float f;
   double d;
   long long int i64;
   unsigned long long int ui64;
   char *str;
   unsigned char *bytes;
};

// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef a
#undef b
#undef c
#undef alpha
#undef beta

extern "C" __declspec(dllexport) void abc2ab(void **opaque, double t, union uData *data)
{
   double  a     = data[0].d; // input
   double  b     = data[1].d; // input
   double  c     = data[2].d; // input
   double &alpha = data[3].d; // output
   double &beta  = data[4].d; // output

// Implement module evaluation code here:
   alpha = 2.0 * (a - 0.5 * (b + c)) / 3.0;
   beta  = sqrt(3.0) * (c - b) / 3.0;
}
