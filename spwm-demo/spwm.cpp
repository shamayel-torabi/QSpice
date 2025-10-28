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

  bool pwm_trigger;

  double ttol;
  double mcu_clk;

  unsigned short xpeak;
  unsigned short xcmp;

  unsigned short carrier_f;
  unsigned short duty;
  unsigned short counter;
  bool up_counte;
};

const unsigned int sine_table[] ={
0,330,659,987,1314,1640,1964,2285,2603,2918,
3230,3538,3841,4140,4434,4722,5005,5281,5551,5814,
6070,6319,6560,6793,7018,7235,7442,7640,7829,8009,
8179,8339,8488,8628,8756,8874,8982,9078,9163,9238,
9301,9352,9392,9421,9438,9444,9438,9421,9392,9352,
9301,9238,9163,9078,8982,8874,8756,8628,8488,8339,
8179,8009,7829,7640,7442,7235,7018,6793,6560,6319,
6070,5814,5551,5281,5005,4722,4434,4140,3841,3538,
3230,2918,2603,2285,1964,1640,1314,987,659,330,0
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

      inst->xpeak= (unsigned short)PER;
      inst->xcmp = 0;

      // inst->trg1 = 2 * inst->xpeak / inst->mcu_clk;
      // inst->trg2 = 2 * inst->xpeak / inst->mcu_clk;
      // inst->trg3 = 2 * inst->xpeak / inst->mcu_clk;
      // inst->trg4 = 2 * inst->xpeak / inst->mcu_clk;

      inst->trg1 = 0;
      inst->trg2 = 1 * inst->xpeak / inst->mcu_clk;
      inst->trg3 = 0;
      inst->trg4 = 2 * inst->xpeak / inst->mcu_clk;


      inst->carrier_f = (unsigned short) floor(FREQ / (200 * PER));
      inst->duty = 0;
      inst->counter = 1;
      inst->up_counte = true;

      g1 = 0.0;
      g2 = 0.0;
      Counter = 0;

      inst->maxstep = 1e-6;
      inst->t_prev = 0.0;
   }
   struct sSPWM *inst = *opaque;

// Implement module evaluation code here:
   inst->pwm_trigger = false;

   if((inst->t_prev <= inst->trg4)&&(t >= inst->trg4))
   {
      inst->xcntr++;
      inst->xpeak= PER;

      //===================================================================
      // control algorithm interrupt - START ==============================
      //===================================================================

      inst->duty = (unsigned short) (0.9 * sine_table[inst->counter]);
      //===================================================================
      // control algorithm interrupt - END   ==============================
      //===================================================================

      if(inst->up_counte){
         inst->counter++;

         if(inst->counter > inst->carrier_f){
            inst->counter = inst->carrier_f;
            inst->up_counte = false;
         }
      }
      else{
         inst->counter--;
         if(inst->counter <= 0)
         {
            inst->counter = 0;
            inst->up_counte = true;
         }
      }

      inst->xcmp = inst->duty;

      inst->trg1 = inst->trg4 + inst->xcmp / inst->mcu_clk;
      inst->trg2 = inst->trg4 + inst->xpeak / inst->mcu_clk;
      inst->trg3 = inst->trg4 + (2*inst->xpeak - inst->xcmp) / inst->mcu_clk;
      inst->trg4 = inst->trg4 + 2*inst->xpeak / inst->mcu_clk;

      inst->maxstep = PER / inst->mcu_clk;
   }


   if((inst->t_prev <= inst->trg2)&&(t >= inst->trg2))
   {
      inst->xcntr++;
   }

   // if(inst->counter == 0 || inst->counter == inst->carrier_f / 2)
   // {
   //    g1 = 0;
   //    g2 = 0;
   // }

   if(inst->up_counte){
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
   else{

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
