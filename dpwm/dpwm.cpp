// Automatically generated C++ file on Sun Sep 14 08:15:36 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD dpwm.cpp kernel32.lib

#include <malloc.h>
#include <math.h>

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
#undef vin
#undef pwm
#undef cmp

struct sDPWM
{
  long long xcntr;
  double maxstep;
  double t_prev;

  double cmpTrg; // trigger at cmp
  double endTrg; // trigger at end

  double xpeak;
  double xcmp;

  double pwm;
  //double vin;

  double out;
};

extern "C" __declspec(dllexport) void dpwm(struct sDPWM **opaque, double t, union uData *data)
{
   const double mcu_clk = 100E6;
   const double peak    = 10000;

   double  vin = data[0].d; // input
   double &pwm = data[1].d; // output
   double &cmp = data[2].d; // output

   if(!*opaque)
   {
      *opaque = (struct sDPWM *) malloc(sizeof(struct sDPWM));
      bzero(*opaque, sizeof(struct sDPWM));

      struct sDPWM *inst = *opaque;

      pwm = 0.0;

      inst->xpeak= peak;
      inst->xcmp = 0;

      inst->cmpTrg = inst->xpeak/mcu_clk;
      inst->endTrg = inst->xpeak/mcu_clk;

      inst->maxstep = 1e-9;

   }
   struct sDPWM *inst = *opaque;

   // Implement module evaluation code here:
   //end of period event
   if((inst->t_prev <= inst->endTrg)&&(t >= inst->endTrg)){
      inst->xcntr++;
      inst->xpeak= peak;
      inst->xcmp = round(5000 * abs(vin));

      inst->cmpTrg = inst->endTrg + inst->xcmp/mcu_clk;
      inst->endTrg = inst->endTrg + inst->xpeak/mcu_clk;

      inst->maxstep = peak/mcu_clk;
      inst->pwm = 15.0;
   }

   if((inst->t_prev <= inst->cmpTrg)&&(t >= inst->cmpTrg)){
      inst->xcntr++;
      inst->pwm = 0.0;
   }

   pwm = inst->pwm;
   cmp = inst->xcmp;
   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sDPWM *inst, double t)
{
   return inst->maxstep;
}

extern "C" __declspec(dllexport) void Trunc(struct sDPWM *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sDPWM
   const double ttol = 1e-9; // 1ns default tolerance
   if(*timestep > ttol)
   {
      struct sDPWM tmp = *inst;
      dpwm(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sDPWM *inst)
{
   free(inst);
}
