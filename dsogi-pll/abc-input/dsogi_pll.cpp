// Automatically generated C++ file on Mon Apr 13 21:12:38 2026
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
#undef Va
#undef Vb
#undef Vc
#undef theta
#undef clk
#undef vd
#undef Vq

struct sDSOGI_PLL
{
  // declare the structure here
   double theta;
   bool   last_clk;

   DSOGI_PLL dsogi;
};

extern "C" __declspec(dllexport) void dsogi_pll(struct sDSOGI_PLL **opaque, double t, union uData *data)
{
   double  Va           = data[0].d; // input
   double  Vb           = data[1].d; // input
   double  Vc           = data[2].d; // input
   bool    clk          = data[3].b; // input
   const double  Kp     = data[4].d; // input parameter
   const double  Ki     = data[5].d; // input parameter
   const double  Freq   = data[6].d; // input parameter
   double &theta        = data[7].d; // output

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
      double thta = inst->dsogi(Va, Vb, Vc, t);
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
