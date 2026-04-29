4// Automatically generated C++ file on Tue Mar 31 15:34:06 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD srf_pll.cpp kernel32.lib

#include <malloc.h>
#include <math.h>
#include "ing.h"
#include "pi.h"

#define INVSQRT3  0.5773502691896257645091
#define PI        3.14159265358979
#define SQRT2     1.4142135623731

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
#undef Vd
#undef Omega

struct sSRF_PLL
{
  // declare the structure here
   double theta;
   double omega;
   double vm;

   double last_t;
   bool   last_clk;

   Integrator integ;
   PIController pi;
};

double max(double a, double b){
   if(a > b)
      return a;
   else
      return b;
};

extern "C" __declspec(dllexport) void srf_pll(struct sSRF_PLL **opaque, double t, union uData *data)
{
   double  Va    = data[0].d; // input
   double  Vb    = data[1].d; // input
   double  Vc    = data[2].d; // input
   bool    clk   = data[3].b; // input
   double  Kp    = data[4].d; // input parameter
   double  Ki    = data[5].d; // input parameter
   double  Freq  = data[6].d; // input parameter
   double &theta = data[7].d; // output
   double &Vd    = data[8].d; // output
   double &Omega = data[9].d; // output

   if(!*opaque)
   {
      *opaque = (struct sSRF_PLL *) malloc(sizeof(struct sSRF_PLL));
      bzero(*opaque, sizeof(struct sSRF_PLL));

      struct sSRF_PLL *inst = *opaque;
      inst->pi.init(Kp, Ki);
   }
   struct sSRF_PLL *inst = *opaque;


// Implement module evaluation code here:
   if (clk && !inst->last_clk)
   {
      double va = Va * SQRT2 * 220.0;
      double vb = Vb * SQRT2 * 220.0;
      double vc = Vc * SQRT2 * 220.0;

      // Clark Transformation
      double Valpha = 2 * (va - 0.5 * (vb + vc)) / 3;
      double Vbeta  = INVSQRT3 * (vc - vb);

      // Park Transformation
      double Vd = Valpha * sin(inst->theta) + Vbeta * cos(inst->theta);
      double Vq = Valpha * cos(inst->theta) - Vbeta * sin(inst->theta);

      double Vm = sqrt(Vd * Vd + Vq * Vq);
      double Vin = Vq / max(Vm, 1e-6);

      double omega = inst->pi(Vin, t);
      omega += 2 * PI * Freq;

      double thta = inst->integ(omega, t);

       // store [n-1] sampling
      inst->vm = Vm;
      inst->omega = omega;
      inst->theta = thta;
      inst->last_t = t;
   }

   inst->last_clk = clk;
   theta = inst->theta;
   Vd = inst->vm;
   Omega = inst->omega;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sSRF_PLL *inst, double t)
{
   return 1e308; // implement a good choice of max timestep size that depends on struct sSRF_PLL
}

extern "C" __declspec(dllexport) void Trunc(struct sSRF_PLL *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sSRF_PLL
   const double ttol = 10e-12; // 10ps default tolerance
   if(*timestep > ttol)
   {
      struct sSRF_PLL tmp = *inst;
      srf_pll(&(&tmp), t, data);
      if(tmp.last_clk != inst->last_clk & !inst->last_clk)  // clock rising edge reduce timestep to TTOL
         *timestep = ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sSRF_PLL *inst)
{
   free(inst);
}
