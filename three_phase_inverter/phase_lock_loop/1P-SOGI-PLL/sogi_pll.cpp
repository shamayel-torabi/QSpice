// Automatically generated C++ file on Thu Apr  2 11:14:04 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD sogi_pll.cpp kernel32.lib

#include <malloc.h>
#include <math.h>
#include "ing.h"
#include "sogi.h"
#include "pi.h"

#define PI     3.14159265358979

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
#undef theta
#undef clk
#undef Vm
#undef W

struct sSOGI_PLL
{
  // declare the structure here
    bool   last_clk;

   double theta;
   double omega;
   double vm;

   SOGIIntegrator sogi;
   PIController pi;
   Integrator omeg_int;
};

double max(double a, double b){
   if(a > b)
      return a;
   else
      return b;
};

extern "C" __declspec(dllexport) void sogi_pll(struct sSOGI_PLL **opaque, double t, union uData *data)
{
   double  Va    = data[0].d; // input
   bool    clk   = data[1].b; // input
   double  Kp    = data[2].d; // input parameter
   double  Ki    = data[3].d; // input parameter
   double  Freq  = data[4].d; // input parameter
   double &theta = data[5].d; // output
   double &Vm    = data[6].d; // output
   double &W     = data[7].d; // output

   if(!*opaque)
   {
      *opaque = (struct sSOGI_PLL *) malloc(sizeof(struct sSOGI_PLL));
      bzero(*opaque, sizeof(struct sSOGI_PLL));

      struct sSOGI_PLL *inst = *opaque;
      inst->pi.init(Kp, Ki);
      inst->omega = 2 * PI * Freq;
   }
   struct sSOGI_PLL *inst = *opaque;

// Implement module evaluation code here:
   if (clk && !inst->last_clk){
      double va, vb;
      inst->sogi(Va, inst->omega, t, &va, &vb);

      double Vd = va * sin(inst->theta) + vb * cos(inst->theta);
      double Vq = va * cos(inst->theta) - vb * sin(inst->theta);

      double Vm = sqrt(Vd * Vd + Vq * Vq);
      double Vin = Vq / max(Vm, 1e-6);

      double omega = inst->pi(Vin, t);
      omega += 2 * PI * Freq;

      double tt = inst->omeg_int(omega, t);

      inst->omega = omega;
      inst->theta = tt;
      inst->vm = Vm;
   }

   theta = inst->theta;
   W = inst->omega;
   Vm = inst->vm;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sSOGI_PLL *inst, double t)
{
   return 1e308; // implement a good choice of max timestep size that depends on struct sSOGI_PLL
}

extern "C" __declspec(dllexport) void Trunc(struct sSOGI_PLL *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sSOGI_PLL
   const double ttol = 10e-12; // 10ps default tolerance
   if(*timestep > ttol)
   {
      struct sSOGI_PLL tmp = *inst;
      sogi_pll(&(&tmp), t, data);
      if(tmp.last_clk != inst->last_clk & !inst->last_clk)  // clock rising edge reduce timestep to TTOL
         *timestep = ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sSOGI_PLL *inst)
{
   free(inst);
}
