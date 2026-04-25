// Automatically generated C++ file on Sat Apr 25 10:40:45 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD spwm.cpp kernel32.lib

#include <malloc.h>
#include <inttypes.h>
#include "inc/spwm.h"

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
#undef alpha
#undef g1
#undef g2
#undef g3
#undef g4
#undef g5
#undef g6
#undef beta

struct sSPWM
{
   uint64_t xcntr;
   double maxstep;
   double mcu_clk;
   double xpeak;
   double t_prev;

   double trg_e;   // trigger at start period
   double trg_m;   // trigger at half period

   double trg_a_r; // trigger cmp at rising phase a
   double trg_a_f; // trigger cmp at falling phase a

   double trg_b_r; // trigger cmp at rising phase b
   double trg_b_f; // trigger cmp at falling phase b

   double trg_c_r; // trigger cmp at rising phase c
   double trg_c_f; // trigger cmp at falling phase c

   double F;
   double Fsw;

   double g1;
   double g2;
   double g3;
   double g4;
   double g5;
   double g6;

   SPWM pwm;
};

extern "C" __declspec(dllexport) void spwm(struct sSPWM **opaque, double t, union uData *data)
{
   double  alpha = data[ 0].d; // input
   double  beta  = data[ 1].d; // input
   double  Fsw   = data[ 2].d; // input parameter
   double  Fclk  = data[ 3].d; // input parameter
   double  Vdc   = data[ 4].d; // input parameter
   double &g1    = data[ 5].d; // output
   double &g2    = data[ 6].d; // output
   double &g3    = data[ 7].d; // output
   double &g4    = data[ 8].d; // output
   double &g5    = data[ 9].d; // output
   double &g6    = data[10].d; // output

   if(!*opaque)
   {
      *opaque = (struct sSPWM *) malloc(sizeof(struct sSPWM));
      bzero(*opaque, sizeof(struct sSPWM));

      struct sSPWM *inst = *opaque;

      inst->Fsw = Fsw;
      inst->mcu_clk = Fclk;
      inst->xpeak = Fclk / (2 * Fsw);

      inst->pwm.init(Vdc, inst->xpeak, inst->mcu_clk);

      inst->g1 = -7.0;
      inst->g2 = -7.0;
      inst->g3 = -7.0;
      inst->g4 = -7.0;
      inst->g5 = -7.0;
      inst->g6 = -7.0;

      inst->trg_e = 0.0;
      inst->trg_m = inst->xpeak / inst->mcu_clk;
      inst->maxstep = 10e-12;
   }
   struct sSPWM *inst = *opaque;

// Implement module evaluation code here:
   if((inst->t_prev <= inst->trg_e)&&(t >= inst->trg_e)){
      inst->xcntr++;
      inst->maxstep = inst->xpeak / inst->mcu_clk;

      inst->pwm(alpha, beta);

      inst->trg_m   = inst->trg_e + inst->xpeak / inst->mcu_clk;

      inst->trg_a_r = inst->trg_e + inst->pwm.switchtime_a / inst->mcu_clk;
      inst->trg_a_f = inst->trg_e + (2 * inst->xpeak - inst->pwm.switchtime_a) / inst->mcu_clk;

      inst->trg_b_r = inst->trg_e + inst->pwm.switchtime_b / inst->mcu_clk;
      inst->trg_b_f = inst->trg_e + (2 * inst->xpeak - inst->pwm.switchtime_b) / inst->mcu_clk;

      inst->trg_c_r = inst->trg_e + inst->pwm.switchtime_c / inst->mcu_clk;
      inst->trg_c_f = inst->trg_e + (2 * inst->xpeak - inst->pwm.switchtime_c) / inst->mcu_clk;

      inst->trg_e   = inst->trg_e + 2 * inst->xpeak /  inst->mcu_clk;
   }

   if((inst->t_prev <= inst->trg_m)&&(t >= inst->trg_m))
   {
      inst->xcntr++;
   }

   if(t <= inst->trg_m){
      if((inst->t_prev <= inst->trg_a_r)&&(t >= inst->trg_a_r))
      {
         inst->xcntr++;
         inst->g1 =  15.0;
         inst->g2 = -7.0;
      }
      if((inst->t_prev <= inst->trg_b_r)&&(t >= inst->trg_b_r))
      {
         inst->xcntr++;
         inst->g3 =  15.0;
         inst->g4 = -7.0;
      }
      if((inst->t_prev <= inst->trg_c_r)&&(t >= inst->trg_c_r))
      {
         inst->xcntr++;
         inst->g5 =  15.0;
         inst->g6 = -7.0;
      }
   }
   else{
      if((inst->t_prev <= inst->trg_a_f)&&(t >= inst->trg_a_f))
      {
         inst->xcntr++;
         inst->g1 = -7.0;
         inst->g2 =  15.0;
      }
      if((inst->t_prev <= inst->trg_b_f)&&(t >= inst->trg_b_f))
      {
         inst->xcntr++;
         inst->g3 = -7.0;
         inst->g4 =  15.0;
      }
      if((inst->t_prev <= inst->trg_c_f)&&(t >= inst->trg_c_f))
      {
         inst->xcntr++;
         inst->g5 = -7.0;
         inst->g6 =  15.0;
      }
   }

   g1 = inst->g1;
   g2 = inst->g2;
   g3 = inst->g3;
   g4 = inst->g4;
   g5 = inst->g5;
   g6 = inst->g6;

   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sSPWM *inst, double t)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sSVPWM
}

extern "C" __declspec(dllexport) void Trunc(struct sSPWM *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sSPWM
   const double ttol = 10e-12; // 10ps default tolerance
   if(*timestep > ttol)
   {
      struct sSPWM tmp = *inst;
      spwm(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sSPWM *inst)
{
   free(inst);
}
