// Automatically generated C++ file on Sat Oct 25 11:04:29 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD spwm.cpp kernel32.lib

#include <malloc.h>
#include <time.h>
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
#undef Vref
#undef g1
#undef g2
#undef g3
#undef g4
#define PI 3.14159265358979


struct sSPWM
{
  // declare the structure here
  long long int xcntr;
  double maxstep;
  double t_prev;

  double trg1; // trigger cmp at rising
  double trg2; // trigger at half period
  double trg3; // trigger cmp at falling
  double trg4; // trigger at full period

  double xpeak;
  double xcmp;

  bool pwm_trigger;

  double ttol;
  double mcu_clk;
  double duty;
  unsigned short counter;
};

extern "C" __declspec(dllexport) void spwm(struct sSPWM **opaque, double t, union uData *data)
{
   const double peak    = 4722;

   double  Vref = data[0].d; // input
   const double  TTOL = data[1].d; // input parameter
   const double  FREQ = data[2].d; // input parameter
   double &g1   = data[3].d; // output
   double &g2   = data[4].d; // output
   double &g3   = data[5].d; // output
   double &g4   = data[6].d; // output

   if(!*opaque)
   {
      *opaque = (struct sSPWM *) malloc(sizeof(struct sSPWM));
      bzero(*opaque, sizeof(struct sSPWM));

      struct sSPWM *inst = *opaque;

      inst->ttol = TTOL;
      inst->mcu_clk = FREQ;

      inst->xpeak= peak;
      inst->xcmp = 0;

      inst->trg1 = 2*inst->xpeak/inst->mcu_clk;
      inst->trg2 = 2*inst->xpeak/inst->mcu_clk;
      inst->trg3 = 2*inst->xpeak/inst->mcu_clk;
      inst->trg4 = 2*inst->xpeak/inst->mcu_clk;

      inst->counter = 0;

      g1 = 0.0;
      g2 = 0.0;
      g3 = 0.0;
      g4 = 0.0;

      inst->maxstep = 1e-9;
   }
   struct sSPWM *inst = *opaque;
// Implement module evaluation code here:
   inst->pwm_trigger = false;

   if((inst->t_prev <= inst->trg4)&&(t >= inst->trg4))
   {
      inst->xcntr++;
      inst->xpeak= peak;
      inst->xcmp = inst->duty;

      inst->trg1 = inst->trg4 + inst->xcmp/inst->mcu_clk;
      inst->trg2 = inst->trg4 + inst->xpeak/inst->mcu_clk;
      inst->trg3 = inst->trg4 + (2*inst->xpeak - inst->xcmp)/inst->mcu_clk;
      inst->trg4 = inst->trg4 + 2*inst->xpeak/inst->mcu_clk;

      inst->maxstep = peak/inst->mcu_clk;

      inst->counter++;
      if(inst->counter >= 2 * peak)
         inst->counter = 0;

      //===================================================================
      // control algorithm interrupt - START ==============================
      //===================================================================
      double theta = inst->counter * PI / peak;
      inst->duty = round(peak * sin(theta));
      //===================================================================
      // control algorithm interrupt - END   ==============================
      //===================================================================
   }

   if((inst->t_prev <= inst->trg2)&&(t >= inst->trg2))
   {
      inst->xcntr++;
   }

   if(t < inst->trg2)
   {
      if((inst->t_prev <= inst->trg1)&&(t >= inst->trg1))
      {
         inst->xcntr++;
         g1 = 0;
         g2 = 15.0;
         inst->pwm_trigger = true;
      }
   }
   else
   {
      if((inst->t_prev <= inst->trg3)&&(t >= inst->trg3))
      {
         inst->xcntr++;
         g1 = 15.0;
         g2 = 0.0;
         inst->pwm_trigger = true;
      }
   }

   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sSPWM *inst, double t)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sEPWM
}

extern "C" __declspec(dllexport) void Trunc(struct sSPWM *inst, double t, union uData *data, double *timestep)
{
   // limit the timestep to a tolerance if the circuit causes a change in struct sSPWM

   struct sSPWM tmp = *inst;
   if(inst->t_prev < inst->trg1){*timestep = (inst->trg1 - inst->t_prev);}
   if(inst->t_prev < inst->trg2){if(*timestep > (inst->trg2 - inst->t_prev)){*timestep = (inst->trg2 - inst->t_prev);}}
   if(inst->t_prev < inst->trg3){if(*timestep > (inst->trg3 - inst->t_prev)){*timestep = (inst->trg3 - inst->t_prev);}}
   if(inst->t_prev < inst->trg4){if(*timestep > (inst->trg4 - inst->t_prev)){*timestep = (inst->trg4 - inst->t_prev);}}
   if(inst->pwm_trigger)
   {
      if(*timestep > inst->ttol) *timestep = inst->ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sSPWM *inst)
{
   free(inst);
}
