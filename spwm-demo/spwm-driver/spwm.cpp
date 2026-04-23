// Automatically generated C++ file on Wed Apr 15 16:05:11 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD spwm.cpp kernel32.lib

#include <malloc.h>
#include <time.h>
#include <math.h>

#define PI 3.1415926535897932384626

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
#undef g1
#undef g2
#undef cnt
#undef duty
#undef g3
#undef g4

struct sSPWM
{
  // declare the structure here
  long long int xcntr;
  double maxstep;
  double t_prev;

  double trg_r;   // trigger cmp at rising
  double trg_m;   // trigger at half period
  double trg_f;   // trigger cmp at falling
  double trg_e;   // trigger at start period

  double ttol;
  double mcu_clk;
  double xpeak;
  double xcmp;

  double theta;

  bool sector;

  double g1;
  double g2;
  double g3;
  double g4;
};

extern "C" __declspec(dllexport) void spwm(struct sSPWM **opaque, double t, union uData *data)
{
   double          Fclk = data[0].d ; // input parameter
   double          Fsw  = data[1].d ; // input parameter
   double         &g1   = data[2].d ; // output
   double         &g2   = data[3].d ; // output
   unsigned short &cnt  = data[4].us; // output
   double         &duty = data[5].d ; // output
   double         &g3   = data[6].d ; // output
   double         &g4   = data[7].d ; // output

   double dtheta = PI * 100.0 / Fsw;

   if(!*opaque)
   {
      *opaque = (struct sSPWM *) malloc(sizeof(struct sSPWM));
      bzero(*opaque, sizeof(struct sSPWM));

      struct sSPWM *inst = *opaque;

      inst->mcu_clk = Fclk;
      inst->xpeak= Fclk / (2 * Fsw);
      inst->xcmp = 0.0;

      inst->trg_e = 0.0;
      inst->trg_m = 0.0;
      inst->trg_r = 0.0;
      inst->trg_f = 0.0;

      inst->maxstep = 1e-9;
      inst->t_prev = 0.0;
      inst->theta = 0.0;
      inst->sector = true;
   }
   struct sSPWM *inst = *opaque;

// Implement module evaluation code here:
   if((inst->t_prev <= inst->trg_e)&&(t >= inst->trg_e))
   {
      inst->xcntr++;
      inst->maxstep = inst->xpeak / inst->mcu_clk;

      //===================================================================
      // control algorithm interrupt - START ==============================
      //===================================================================
      //inst->xcmp = inst->xpeak * (1.0 - 0.9 *sin(inst->theta));
      inst->xcmp = inst->xpeak * 0.9 * sin(inst->theta);
      //===================================================================
      // control algorithm interrupt - END   ==============================
      //===================================================================

      inst->trg_m = inst->trg_e + inst->xpeak / inst->mcu_clk;;
      inst->trg_r = inst->trg_e + inst->xcmp / inst->mcu_clk;
      inst->trg_f = inst->trg_e + (2.0 * inst->xpeak - inst->xcmp) / inst->mcu_clk;
      inst->trg_e = inst->trg_e + 2.0 * inst->xpeak / inst->mcu_clk;

      inst->theta += dtheta;

      if(inst->theta >= PI){
         inst->theta = dtheta;
         inst->sector = !inst->sector;
      }
   }


   if((inst->t_prev <= inst->trg_m)&&(t >= inst->trg_m))
   {
      inst->xcntr++;
   }

   if(t <= inst->trg_m){
      if((inst->t_prev <= inst->trg_r)&&(t >= inst->trg_r))
      {
         inst->xcntr++;
         inst->g1 = -7.0;
         inst->g2 = 15.0;

         inst->g3 = -7.0;
         inst->g4 = 15.0;
      }
   }
   else{
      if((inst->t_prev <= inst->trg_f)&&(t >= inst->trg_f))
      {
         inst->xcntr++;
         if(inst->sector)
         {
            inst->g1 = 15.0;
            inst->g2 = -7.0;

            inst->g3 = -7.0;
            inst->g4 = 15.0;
         }
         else
         {
            inst->g1 = -7.0;
            inst->g2 = 15.0;

            inst->g3 = 15.0;
            inst->g4 = -7.0;
         }
      }
   }

   cnt = (unsigned short) (inst->theta * 180.0 / PI);
   duty = inst->xcmp;

   g1 = inst->g1;
   g2 = inst->g2;
   g3 = inst->g3;
   g4 = inst->g4;

   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sSPWM *inst, double t)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sEPWM
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
