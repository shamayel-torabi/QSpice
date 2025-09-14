// Automatically generated C++ file on Sun Sep 14 17:03:52 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD digital_clock.cpp kernel32.lib

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
#undef clk
#undef cnt

struct sDIGITAL_CLOCK
{
  // declare the structure here
  long long int xcntr;
  double maxstep;
  double ttol;
  double t_prev;
  double startTrg;
};

extern "C" __declspec(dllexport) void digital_clock(struct sDIGITAL_CLOCK **opaque, double t, union uData *data)
{
   const double    peak = 170;
   double          mcu_clk = data[0].d ; // input parameter
   double          TTOL    = data[1].d ; // input parameter
   double         &clk     = data[2].d ; // output
   unsigned short &cnt     = data[3].us; // output

   if(!*opaque)
   {
      *opaque = (struct sDIGITAL_CLOCK *) malloc(sizeof(struct sDIGITAL_CLOCK));
      bzero(*opaque, sizeof(struct sDIGITAL_CLOCK));

      struct sDIGITAL_CLOCK *inst = *opaque;

      inst->startTrg = peak / mcu_clk;
      inst->maxstep = 1e-12;
      inst->ttol = TTOL;
   }
   struct sDIGITAL_CLOCK *inst = *opaque;

// Implement module evaluation code here:

   if((inst->t_prev <= inst->startTrg)&&(t >= inst->startTrg)){
      inst->xcntr++;

      inst->startTrg += peak/mcu_clk;
      inst->maxstep = peak/mcu_clk;

      cnt++;
      if(cnt > peak)
         cnt = 0;
   }

   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sDIGITAL_CLOCK *inst, double t)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sEPWM
}

extern "C" __declspec(dllexport) void Trunc(struct sDIGITAL_CLOCK *inst, double t, union uData *data, double *timestep)
{
   // limit the timestep to a tolerance if the circuit causes a change in struct sDIGITAL_CLOCK
   //const double ttol = 1e-9; // 1ns default tolerance

   if(*timestep > inst->ttol)
   {
      struct sDIGITAL_CLOCK tmp = *inst;
      digital_clock(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = inst->ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sDIGITAL_CLOCK *inst)
{
   free(inst);
}
