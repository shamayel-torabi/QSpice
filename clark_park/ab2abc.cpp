// Automatically generated C++ file on Thu Apr  9 13:03:35 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD ab2abc.cpp kernel32.lib

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
#undef alpha
#undef beta
#undef b
#undef c
#undef a

extern "C" __declspec(dllexport) void ab2abc(void **opaque, double t, union uData *data)
{
   double  alpha = data[0].d; // input
   double  beta  = data[1].d; // input
   double &b     = data[2].d; // output
   double &c     = data[3].d; // output
   double &a     = data[4].d; // output

// Implement module evaluation code here:
   a = alpha;
   b =  (sqrt(3.0) * beta -  alpha) / 2.0;
   c = -(a + b);
}
