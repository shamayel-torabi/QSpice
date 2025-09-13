// Automatically generated C++ file on Fri Sep 12 19:14:40 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD pfc_controller.cpp kernel32.lib

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
#undef PWM
#undef Vrec
#undef SW

struct sPFC_CONTROLLER
{
  // declare the structure here
  unsigned short lastV;


  // input parameters
  double ttol;   // time tolerance
  double freq;   // clock frequency (Hz)

  // clock management
  double       next_t;    // next clock tick simulation clock time
  double       incr_t;    // simulation time step increment to next clock tick
  unsigned int tickCnt;   // tick counter
};

// calculate simulation clock time for a clock tick with minimal rounding error.
inline double calcTickTime(sPFC_CONTROLLER *inst) {
  return inst->tickCnt / (2 * inst->freq);
}

extern "C" __declspec(dllexport) void pfc_controller(struct sPFC_CONTROLLER **opaque, double t, union uData *data)
{
   double  Vrec = data[0].d; // input
   const double  FREQ = data[1].d; // input parameter
   const double  TTOL = data[2].d; // input parameter
   double &PWM  = data[3].d; // output
   double &SW   = data[4].d; // output

   if(!*opaque)
   {
      *opaque = (struct sPFC_CONTROLLER *) malloc(sizeof(struct sPFC_CONTROLLER));
      bzero(*opaque, sizeof(struct sPFC_CONTROLLER));

      struct sPFC_CONTROLLER *inst = *opaque;
      // initialize instance data fields
      inst->ttol    = TTOL;   // note: no error checking...
      inst->freq    = FREQ;   // note: no error checking...
      inst->tickCnt = 0;
      inst->incr_t  = inst->next_t = calcTickTime(inst);
      inst->lastV   = 0;
   }
   struct sPFC_CONTROLLER *inst = *opaque;

   inst->incr_t = inst->next_t - t;

   // clock state change triggered
   if (t >= inst->next_t) {
      inst->tickCnt++;
      inst->next_t = calcTickTime(inst);
      inst->incr_t = inst->ttol;
      inst->lastV = (unsigned short) Vrec * 4096;
   }

   PWM = inst->lastV;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sPFC_CONTROLLER *inst, double t)
{
   const double forever  = 1e308;    // return forever to not limit timestep size

   if (inst->incr_t <= 0) return forever;
   return inst->incr_t;
}

extern "C" __declspec(dllexport) void Destroy(struct sPFC_CONTROLLER *inst)
{
   free(inst);
}
