// Automatically generated C++ file on Mon Oct 27 17:44:38 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD spwm.cpp kernel32.lib

#include <malloc.h>
#include <time.h>
#include <math.h>

#define PI 3.14159265358979


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
#undef Counter
#undef Duty

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
  double carrier_f;
  unsigned short counter;
};

extern "C" __declspec(dllexport) void spwm(struct sSPWM **opaque, double t, union uData *data)
{
   double                Vref    = data[0].d ; // input
   const double          TTOL    = data[1].d ; // input parameter
   const double          FREQ    = data[2].d ; // input parameter
   const double          PER     = data[3].d ; // input parameter
   double                &g1      = data[4].d ; // output
   double                &g2      = data[5].d ; // output
   unsigned short        &Counter = data[6].us; // output
   double                &Duty    = data[7].d ; // output



   if(!*opaque)
   {
      *opaque = (struct sSPWM *) malloc(sizeof(struct sSPWM));
      bzero(*opaque, sizeof(struct sSPWM));

      struct sSPWM *inst = *opaque;

      inst->ttol = TTOL;
      inst->mcu_clk = FREQ;

      inst->xpeak= PER;
      inst->xcmp = 0;

      inst->trg1 = 2 * inst->xpeak / inst->mcu_clk;
      inst->trg2 = 2 * inst->xpeak / inst->mcu_clk;
      inst->trg3 = 2 * inst->xpeak / inst->mcu_clk;
      inst->trg4 = 2 * inst->xpeak / inst->mcu_clk;


      inst->duty = 0.0;
      inst->carrier_f = (FREQ / (2 * PER)) / 50 ;
      inst->counter = 0;

      g1 = 0.0;
      g2 = 0.0;
      Counter = 0;

      inst->maxstep = 1e-9;
   }
   struct sSPWM *inst = *opaque;

// Implement module evaluation code here:
   inst->pwm_trigger = false;

   if((inst->t_prev <= inst->trg4)&&(t >= inst->trg4))
   {
      inst->xcntr++;
      inst->xpeak= PER;
      inst->xcmp = inst->duty;

      inst->trg1 = inst->trg4 + inst->xcmp / inst->mcu_clk;
      inst->trg2 = inst->trg4 + inst->xpeak / inst->mcu_clk;
      inst->trg3 = inst->trg4 + (2*inst->xpeak - inst->xcmp) / inst->mcu_clk;
      inst->trg4 = inst->trg4 + 2*inst->xpeak / inst->mcu_clk;

      inst->maxstep = PER / inst->mcu_clk;

      inst->counter++;
      if(inst->counter >= inst->carrier_f)
         inst->counter = 0;

      //===================================================================
      // control algorithm interrupt - START ==============================
      //===================================================================
      double theta = inst->counter * 2 * PI / inst->carrier_f;
      inst->duty = round(fabs(0.9 * PER * sin(theta)));
      //===================================================================
      // control algorithm interrupt - END   ==============================
      //===================================================================
   }


   if((inst->t_prev <= inst->trg2)&&(t >= inst->trg2))
   {
      inst->xcntr++;
   }

   if(inst->counter == 0 || inst->counter == inst->carrier_f / 2)
   {
      g1 = 0;
      g2 = 0;
   }

   if(inst->counter < inst->carrier_f / 2){
      g2 = 0;
      if(t < inst->trg2)
      {
         if((inst->t_prev <= inst->trg1)&&(t >= inst->trg1))
         {
            inst->xcntr++;
            g1 = 0;
            inst->pwm_trigger = true;
         }
      }
      else
      {
         if((inst->t_prev <= inst->trg3)&&(t >= inst->trg3))
         {
            inst->xcntr++;
            g1 = 15;
            inst->pwm_trigger = true;
         }
      }
   }
   else{
      g1 = 0;
      if(t < inst->trg2)
      {
         if((inst->t_prev <= inst->trg1)&&(t >= inst->trg1))
         {
            inst->xcntr++;
            g2 = 0;
            inst->pwm_trigger = true;
         }
      }
      else
      {
         if((inst->t_prev <= inst->trg3)&&(t >= inst->trg3))
         {
            inst->xcntr++;
            g2 = 15;
            inst->pwm_trigger = true;
         }
      }
   }

   Counter = inst->counter;
   Duty = inst->duty;
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
