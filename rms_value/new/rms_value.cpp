// Automatically generated C++ file on Thu Apr 30 17:03:44 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD rms_value.cpp kernel32.lib

#include <malloc.h>
#include <math.h>
#include <inttypes.h>

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
#undef in
#undef rms

struct sRMS_VALUE
{
   uint64_t xcntr;
   double maxstep;
   double mcu_clk;
   double xpeak;
   double t_prev;
   double Fs;

   double trg_e;   // trigger at start period
   double trg_m;   // trigger at half period

   size_t n;
   size_t k;

   double in;

   double v_sq[200];
   double v_mn[200];

   double sq_sum;
   double mn_sum;
   double v_mean;
};

void calculate_rms(struct sRMS_VALUE *inst){
   inst->mn_sum -= inst->v_mn[inst->k];
   inst->mn_sum += inst->in;

   inst->sq_sum -= inst->v_sq[inst->k];
   double in_square = (inst->in - inst->v_mean) * (inst->in - inst->v_mean);

   inst->sq_sum += in_square;

   inst->v_mn[inst->k] = inst->in;
   inst->v_sq[inst->k] = in_square;
   
   inst->v_mean = inst->mn_sum / inst->n;

   inst->k++;
   if(inst->k >= inst->n){
      inst->k = 0;
   }
};

extern "C" __declspec(dllexport) void rms_value(struct sRMS_VALUE **opaque, double t, union uData *data)
{
   double  in   = data[0].d; // input
   const double  Fclk = data[1].d; // input parameter
   const double  Fs   = data[2].d; // input parameter
   double &rms  = data[3].d; // output

   if(!*opaque)
   {
      *opaque = (struct sRMS_VALUE *) malloc(sizeof(struct sRMS_VALUE));
      bzero(*opaque, sizeof(struct sRMS_VALUE));

      struct sRMS_VALUE *inst = *opaque;

      inst->Fs = Fs;
      inst->mcu_clk = Fclk;
      inst->xpeak = Fclk / (2 * Fs);
      inst->trg_e = 0.0;
      inst->trg_m = inst->xpeak / inst->mcu_clk;
      inst->maxstep = 10e-12;

      inst->n = 200;
      inst->sq_sum = 0.0;
      inst->mn_sum = 0.0;
      inst->v_mean = 0.0;
      inst->k = 0;
   }
   struct sRMS_VALUE *inst = *opaque;

// Implement module evaluation code here:
   if((inst->t_prev <= inst->trg_e)&&(t >= inst->trg_e)){
      inst->xcntr++;
      inst->maxstep = inst->xpeak / inst->mcu_clk;

      inst->in = in;
      calculate_rms(inst);

      inst->trg_m   = inst->trg_e + inst->xpeak / inst->mcu_clk;
      inst->trg_e   = inst->trg_e + 2 * inst->xpeak /  inst->mcu_clk;
   }

   if((inst->t_prev <= inst->trg_m)&&(t >= inst->trg_m))
   {
      inst->xcntr++;
   }

   rms = sqrt(inst->sq_sum / inst->n);

   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sRMS_VALUE *inst, double t)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sSVPWM
}

extern "C" __declspec(dllexport) void Trunc(struct sRMS_VALUE *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sRMS_VALUE
   const double ttol = 1e-9; // 1ns default tolerance
   if(*timestep > ttol)
   {
      struct sRMS_VALUE tmp = *inst;
      rms_value(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sRMS_VALUE *inst)
{
   free(inst);
}
