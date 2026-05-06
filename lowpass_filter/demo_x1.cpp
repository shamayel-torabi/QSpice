// Automatically generated C++ file on Wed May  6 08:11:24 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD demo_x1.cpp kernel32.lib

#include <malloc.h>
#include <inttypes.h>
#include "lowpass_filter.h"

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
#undef in
#undef out

struct sDEMO_X1
{
   uint64_t xcntr;
   double maxstep;
   double mcu_clk;
   double xpeak;
   double t_prev;

   double trg_e;   // trigger at start period

   double out;
   double Fsw;
   LowPassFilter lp;
};

extern "C" __declspec(dllexport) void demo_x1(struct sDEMO_X1 **opaque, double t, union uData *data)
{
   double  in   = data[0].d; // input
   double  Fclk = data[1].d; // input parameter
   double  Fs   = data[2].d; // input parameter
   double  Tf   = data[3].d; // input parameter
   double &out  = data[4].d; // output

   if(!*opaque)
   {
      *opaque = (struct sDEMO_X1 *) malloc(sizeof(struct sDEMO_X1));
      bzero(*opaque, sizeof(struct sDEMO_X1));

      struct sDEMO_X1 *inst = *opaque;

      inst->Fsw = Fs;
      inst->mcu_clk = Fclk;
      inst->xpeak = Fclk / (2 * Fs);
      inst->trg_e = 0.0;
      inst->maxstep = 10e-12;

      inst->lp.init(Tf, 1.0/Fs);
   }
   struct sDEMO_X1 *inst = *opaque;

// Implement module evaluation code here:
   if((inst->t_prev <= inst->trg_e)&&(t >= inst->trg_e)){
      inst->xcntr++;
      inst->maxstep = inst->xpeak / inst->mcu_clk;

      inst->out = inst->lp(in);

      inst->trg_e   = inst->trg_e + 2 * inst->xpeak /  inst->mcu_clk;
   }

   out = inst->out;
   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sDEMO_X1 *inst, double t)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sSVPWM
}

extern "C" __declspec(dllexport) void Trunc(struct sDEMO_X1 *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sDEMO_X1
   const double ttol = 1e-9; // 1ns default tolerance
   if(*timestep > ttol)
   {
      struct sDEMO_X1 tmp = *inst;
      demo_x1(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sDEMO_X1 *inst)
{
   free(inst);
}
