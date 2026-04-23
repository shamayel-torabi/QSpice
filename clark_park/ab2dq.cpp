// Automatically generated C++ file on Mon Apr 13 08:01:45 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD ab2dq.cpp kernel32.lib

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
#undef Valpha
#undef Vbeta
#undef Vd
#undef Vq
#undef theta

extern "C" __declspec(dllexport) void ab2dq(void **opaque, double t, union uData *data)
{
   double  Valpha = data[0].d; // input
   double  Vbeta  = data[1].d; // input
   double  theta  = data[2].d; // input
   double &Vd     = data[3].d; // output
   double &Vq     = data[4].d; // output

// Implement module evaluation code here:
   double sinValue = sin(theta);
   double cosValue = cos(theta);

   Vd =  Valpha * cosValue + Vbeta * sinValue;
   Vq = -Valpha * sinValue + Vbeta * cosValue;
}
