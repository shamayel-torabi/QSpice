// Automatically generated C++ file on Mon Mar 30 16:19:09 2026
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
#undef Va
#undef Vb
#undef Vc
#undef Valpha
#undef Vbeta

extern "C" __declspec(dllexport) void abc2ab(void **opaque, double t, union uData *data)
{
   double  Va     = data[0].d; // input
   double  Vb     = data[1].d; // input
   double  Vc     = data[2].d; // input
   double &Valpha = data[3].d; // output
   double &Vbeta  = data[4].d; // output

// Implement module evaluation code here:
   Valpha = 2.0 * (Va - 0.5 * (Vb + Vc)) / 3.0;
   Vbeta  = sqrt(3.0) * (Vc - Vb) / 3.0;
}
