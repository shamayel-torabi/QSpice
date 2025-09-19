// Automatically generated C++ file on Thu Sep 18 19:05:00 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD digital_pwm.cpp kernel32.lib

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
#undef pwm
#undef Vin

struct sDIGITAL_PWM
{
  // declare the structure here
  long long int xcntr;
  double maxstep;

  double ttol;
  double mcu_clk;
  double per;

  double t_prev;
  double startTrg;
  unsigned short counter;
};

extern "C" __declspec(dllexport) void digital_pwm(struct sDIGITAL_PWM **opaque, double t, union uData *data)
{
   double  Vin  = data[0].d; // input
   double  FREQ = data[1].d; // input parameter
   double  TTOL = data[2].d; // input parameter
   double  PER  = data[3].d; // input parameter
   double &pwm  = data[4].d; // output

if(!*opaque)
   {
      *opaque = (struct sDIGITAL_PWM *) malloc(sizeof(struct sDIGITAL_PWM));
      bzero(*opaque, sizeof(struct sDIGITAL_PWM));

      struct sDIGITAL_PWM *inst = *opaque;

      inst->startTrg = 0;
      inst->maxstep = 1e-9;
      inst->ttol = TTOL;
      inst->mcu_clk = FREQ;
      inst->per = PER;
      inst->counter = 0;
   }
   struct sDIGITAL_PWM *inst = *opaque;

// Implement module evaluation code here:


   if((inst->t_prev <= inst->startTrg)&&(t >= inst->startTrg)){
      inst->startTrg += 1.0/inst->mcu_clk;
      inst->maxstep = 1.0/inst->mcu_clk;

      inst->counter++;
      if(inst->counter >= inst->per)
         inst->counter = 0;

      double Vabs = fabs(Vin);

      if(Vabs > 3.3)
         Vabs = 3.3;

      if(Vabs < 0.0)
         Vabs = 0.0;

      unsigned short v = unsigned short (Vabs * inst->per / 3.3);

      if(v > inst->counter)
         pwm = 15.0;
      else
         pwm = 0.0;
   }

   //pwm = inst->pwm;
   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sDIGITAL_PWM *inst, double t)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sEPWM
}

extern "C" __declspec(dllexport) void Trunc(struct sDIGITAL_PWM *inst, double t, union uData *data, double *timestep)
{
   // limit the timestep to a tolerance if the circuit causes a change in struct sDIGITAL_CLOCK
   if(*timestep > inst->ttol)
   {
      struct sDIGITAL_PWM tmp = *inst;
      digital_pwm(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = inst->ttol;
   }

}

extern "C" __declspec(dllexport) void Destroy(struct sDIGITAL_PWM *inst)
{
   free(inst);
}
