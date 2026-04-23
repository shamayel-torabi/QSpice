// Automatically generated C++ file on Wed Apr  8 21:53:31 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD dq2ab.cpp kernel32.lib

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
#undef q
#undef d
#undef a
#undef b
#undef theta

extern "C" __declspec(dllexport) void dq2ab(void **opaque, double t, union uData *data)
{
   double  q     = data[0].d; // input
   double  d     = data[1].d; // input
   double  theta = data[2].d; // input
   double &a     = data[3].d; // output
   double &b     = data[4].d; // output


// Implement module evaluation code here:
   double sinValue = sin(theta);
   double cosValue = cos(theta);

   a = cosValue * d - sinValue * q;
   b = sinValue * d + cosValue * q;
}
