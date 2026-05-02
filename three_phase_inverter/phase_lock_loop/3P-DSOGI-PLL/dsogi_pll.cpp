// Automatically generated C++ file on Wed Apr 29 07:02:17 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD dsogi_pll.cpp kernel32.lib

#include <malloc.h>
#include <math.h>
#include <inttypes.h>
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
#undef werr
#undef Vd
#undef Vq

struct sDSOGI_PLL
{
   uint64_t xcntr;
   double maxstep;
   double mcu_clk;
   double xpeak;
   double t_prev;

   double trg_e;   // trigger at start period
   double trg_m;   // trigger at half period

   double F;
   double Fsw;

   double theta;
   DSOGI_PLL dsogi;
};

extern "C" __declspec(dllexport) void dsogi_pll(struct sDSOGI_PLL **opaque, double t, union uData *data)
{
   double  Valpha = data[ 0].d; // input
   double  Vbeta  = data[ 1].d; // input
   double  Kp     = data[ 2].d; // input parameter
   double  Ki     = data[ 3].d; // input parameter
   double  Freq   = data[ 4].d; // input parameter
   double  Fs     = data[ 5].d; // input parameter
   double  Fclk   = data[ 6].d; // input parameter
   double &theta  = data[ 7].d; // output
   double &werr   = data[ 8].d; // output
   double &Vd     = data[ 9].d; // output
   double &Vq     = data[10].d; // output

   if(!*opaque)
   {
      *opaque = (struct sDSOGI_PLL *) malloc(sizeof(struct sDSOGI_PLL));
      bzero(*opaque, sizeof(struct sDSOGI_PLL));

      struct sDSOGI_PLL *inst = *opaque;

      inst->Fsw = Fs;
      inst->F = Freq;

      inst->mcu_clk = Fclk;
      inst->xpeak = Fclk / (2 * Fs);

      inst->trg_e = 0.0;
      inst->trg_m = inst->xpeak / inst->mcu_clk;

      inst->maxstep = 10e-12;

      inst->dsogi.init(Kp, Ki, Freq);
   }
   struct sDSOGI_PLL *inst = *opaque;

// Implement module evaluation code here:
   if((inst->t_prev <= inst->trg_e)&&(t >= inst->trg_e)){
      inst->xcntr++;
      inst->maxstep = inst->xpeak / inst->mcu_clk;

      inst->dsogi(Valpha, Vbeta, t);

      inst->trg_m   = inst->trg_e + inst->xpeak / inst->mcu_clk;
      inst->trg_e   = inst->trg_e + 2 * inst->xpeak /  inst->mcu_clk;
   }

   if((inst->t_prev <= inst->trg_m)&&(t >= inst->trg_m))
   {
      inst->xcntr++;
   }

   theta = inst->dsogi.theta;
   werr  = inst->dsogi.omega_err;
   Vd    = inst->dsogi.Vd;
   Vq    = inst->dsogi.Vq;
   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sDSOGI_PLL *inst, double t)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sSVPWM
}

extern "C" __declspec(dllexport) void Trunc(struct sDSOGI_PLL *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sDSOGI_PLL
   const double ttol = 1e-9; // 1ns default tolerance
   if(*timestep > ttol)
   {
      struct sDSOGI_PLL tmp = *inst;
      dsogi_pll(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sDSOGI_PLL *inst)
{
   free(inst);
}
