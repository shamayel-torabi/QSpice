// Automatically generated C++ file on Thu Oct 23 11:46:10 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD rms.cpp kernel32.lib

#include <malloc.h>
#include <cmath>

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
#undef VIn
#undef Vrms
#undef Vmean
#undef Vs

#define FS 360

struct InstanceData
{
  // input parameters
  double ttol;   // time tolerance
  double freq;   // clock frequency (Hz)

  // clock management
  double       next_t;    // next clock tick simulation clock time
  double       incr_t;    // simulation time step increment to next clock tick
  unsigned int tickCnt;   // tick counter

  double v_sq_array[FS];
  double v_mn_array[FS];

  size_t index;
  double v_mn_sum;
  double v_sq_sum;
  double v_mean;
  double vs;
};

// calculate simulation clock time for a clock tick with minimal rounding error.
inline double calcTickTime(InstanceData *inst) {
  return inst->tickCnt / (2 * inst->freq);
}

extern "C" __declspec(dllexport) void rms(struct InstanceData **opaque, double t, union uData *data)
{
   double  VIn          = data[0].d;   // input
   const double  TTOL   = data[1].d;   // input parameter
   double &Vrms         = data[2].d;   // output
   double &Vmean        = data[3].d;   // output
   double &Vs           = data[4].d;   // output

double v_square;

   if(!*opaque)
   {
      *opaque = (struct InstanceData *) malloc(sizeof(struct InstanceData));
      bzero(*opaque, sizeof(struct InstanceData));

      struct InstanceData *inst = *opaque;
      // initialize instance data fields
      inst->ttol    = TTOL;      // note: no error checking...
      inst->freq    = FS * 25.0; // note: no error checking...
      inst->tickCnt = 0;
      inst->incr_t = inst->next_t = calcTickTime(inst);

      for(size_t i = 0; i < FS;i++){
         inst->v_mn_array[i] = 0.0;
         inst->v_sq_array[i] = 0.0;
      }

      inst->index = 0;
      inst->v_mn_sum = 0.0;
      inst->v_sq_sum = 0.0;
      inst->v_mean = 220 * sqrt(2);

      Vrms  = 0.0;
      Vmean = 0.0;
      Vs    = 0.0;
   }
   struct InstanceData *inst = *opaque;

// Implement module evaluation code here:
   inst->incr_t = inst->next_t - t;

  // clock state change triggered
   if (t >= inst->next_t) {
      inst->v_mn_sum -= inst->v_mn_array[inst->index];
      inst->v_mn_sum += VIn;

      inst->v_sq_sum -= inst->v_sq_array[inst->index];
      v_square = (VIn - inst->v_mean) * (VIn - inst->v_mean);

      inst->v_sq_sum += v_square ;

      inst->v_mn_array[inst->index] = VIn;
      inst->v_sq_array[inst->index] = v_square;

      inst->index++;
      if(inst->index >= FS){
         inst->index = 0;
         inst->v_mean = inst->v_mn_sum / FS;
      }

      inst->vs = VIn;
      inst->tickCnt++;
      inst->next_t = calcTickTime(inst);
      inst->incr_t = inst->ttol;
   }
   Vs = inst->vs;
   Vrms = sqrt(inst->v_sq_sum / FS);
   Vmean =  inst->v_mean;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct InstanceData *inst, double t)
{
   const double forever  = 1e308;    // return forever to not limit timestep size

   if (inst->incr_t <= 0)
      return forever;

   return inst->incr_t;
}

extern "C" __declspec(dllexport) void Destroy(struct sRMS *inst)
{
   free(inst);
}
