// Automatically generated C++ file on Sat Oct 25 11:04:29 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD spwm.cpp kernel32.lib

#include <malloc.h>

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

#define PER 65535

struct sSPWM
{
  // declare the structure here
  long long int xcntr;
  double maxstep;

  double ttol;
  double mcu_clk;

  double t_prev;
  double startTrg;
  unsigned short counter;
};

extern "C" __declspec(dllexport) void spwm(struct sSPWM **opaque, double t, union uData *data)
{
   double  Vref = data[0].d; // input
   double  TTOL = data[1].d; // input parameter
   double  FREQ = data[2].d; // input parameter
   double &g1   = data[3].d; // output
   double &g2   = data[4].d; // output
   double &g3   = data[5].d; // output
   double &g4   = data[6].d; // output

   if(!*opaque)
   {
      *opaque = (struct sSPWM *) malloc(sizeof(struct sSPWM));
      bzero(*opaque, sizeof(struct sSPWM));

      struct sSPWM *inst = *opaque;

      inst->startTrg = 0;
      inst->maxstep = 1e-9;
      inst->ttol = TTOL;
      inst->mcu_clk = FREQ;
      inst->counter = 0;
   }
   struct sSPWM *inst = *opaque;
// Implement module evaluation code here:
   if((inst->t_prev <= inst->startTrg)&&(t >= inst->startTrg)){
      
      inst->counter++;
      if(inst->counter >= PER)
         inst->counter = 0;
   }

   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sSPWM *inst, double t)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sEPWM
}

extern "C" __declspec(dllexport) void Trunc(struct sSPWM *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sSPWM
   const double ttol = 1e-9; // 1ns default tolerance
   if(*timestep > ttol)
   {
      struct sSPWM tmp = *inst;
      spwm(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = inst->ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sSPWM *inst)
{
   free(inst);
}
