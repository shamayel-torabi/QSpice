// Automatically generated C++ file on Mon Apr 13 17:10:43 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD dsogi_pll.cpp kernel32.lib

#include <malloc.h>
#include <math.h>
#include "inc/dsogi.h"

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
#undef Valpha
#undef Vbeta
#undef theta
#undef clk

struct sDSOGI_PLL
{
  // declare the structure here
   bool   last_clk;

   double theta;
   DSOGI_PLL dsogi;
};

extern "C" __declspec(dllexport) void dsogi_pll(struct sDSOGI_PLL **opaque, double t, union uData *data)
{
   double  Valpha = data[0].d; // input
   double  Vbeta  = data[1].d; // input
   bool    clk    = data[2].b; // input
   double  Kp     = data[3].d; // input parameter
   double  Ki     = data[4].d; // input parameter
   double  Freq   = data[5].d; // input parameter
   double &theta  = data[6].d; // output

   if(!*opaque)
   {
      *opaque = (struct sDSOGI_PLL *) malloc(sizeof(struct sDSOGI_PLL));
      bzero(*opaque, sizeof(struct sDSOGI_PLL));

      struct sDSOGI_PLL *inst = *opaque;
      inst->dsogi.init(Kp, Ki, Freq);
   }
   struct sDSOGI_PLL *inst = *opaque;

// Implement module evaluation code here:
   if (clk && !inst->last_clk)
   {
      double thta = inst->dsogi(Valpha, Vbeta, t);
      inst->theta = thta;
   }

   inst->last_clk = clk;
   theta = inst->theta;
}

extern "C" __declspec(dllexport) void Trunc(struct sDSOGI_PLL *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sDSOGI_PLL
   const double ttol = 1e-9; // 1ns default tolerance
   if(*timestep > ttol)
   {
      struct sDSOGI_PLL tmp = *inst;
      dsogi_pll(&(&tmp), t, data);
      if(tmp.last_clk != inst->last_clk & !inst->last_clk)  // clock rising edge reduce timestep to TTOL
         *timestep = ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sDSOGI_PLL *inst)
{
   free(inst);
}
