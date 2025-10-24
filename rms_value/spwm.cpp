// Automatically generated C++ file on Fri Oct 24 17:14:22 2025
//
// To build with Digital Mars C++ Compiler: 
//
//    dmc -mn -WD spwm.cpp kernel32.lib

#include <malloc.h>

extern "C" __declspec(dllexport) void (*bzero)(void *ptr, unsigned int count)   = 0;

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
#undef Vref

struct sSPWM
{
  // declare the structure here
};

extern "C" __declspec(dllexport) void spwm(struct sSPWM **opaque, double t, union uData *data)
{
   double  Vref = data[0].d; // input
   double &Va   = data[1].d; // output

   if(!*opaque)
   {
      *opaque = (struct sSPWM *) malloc(sizeof(struct sSPWM));
      bzero(*opaque, sizeof(struct sSPWM));
   }
   struct sSPWM *inst = *opaque;

// Implement module evaluation code here:

}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sSPWM *inst, double t)
{
   return 1e308; // implement a good choice of max timestep size that depends on struct sSPWM
}

extern "C" __declspec(dllexport) void Destroy(struct sSPWM *inst)
{
   free(inst);
}
