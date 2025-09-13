// Automatically generated C++ file on Fri Sep 12 11:46:10 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD clock.cpp kernel32.lib

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
#undef ck

struct sCLOCK
{
  // input parameters
  double ttol;   // time tolerance
  double freq;   // clock frequency (Hz)

  // clock management
  double       next_t;    // next clock tick simulation clock time
  double       incr_t;    // simulation time step increment to next clock tick
  unsigned int tickCnt;   // tick counter
};


// calculate simulation clock time for a clock tick with minimal rounding error.
inline double calcTickTime(sCLOCK *inst) {
  return inst->tickCnt / (2 * inst->freq);
}




extern "C" __declspec(dllexport) void clock(struct sCLOCK **opaque, double t, union uData *data)
{
   double  FREQ = data[0].d; // input parameter
   double  TTOL = data[1].d; // input parameter
   double &ck   = data[2].d; // output

   if(!*opaque)
   {
      *opaque = (struct sCLOCK *) malloc(sizeof(struct sCLOCK));
      bzero(*opaque, sizeof(struct sCLOCK));

      struct sCLOCK *inst = *opaque;
      // initialize instance data fields
      inst->ttol    = TTOL;   // note: no error checking...
      inst->freq    = FREQ;   // note: no error checking...
      inst->tickCnt = 0;
      inst->incr_t = inst->next_t = calcTickTime(inst);

      // initialize output port(s)
      ck = 0;   // high puts first cycle low
   }
   struct sCLOCK *inst = *opaque;

// Implement module evaluation code here:

  // default next step increment...
  inst->incr_t = inst->next_t - t;

  // clock state change triggered
  if (t >= inst->next_t) {
    ck = !ck;
    inst->tickCnt++;
    inst->next_t = calcTickTime(inst);
    inst->incr_t = inst->ttol;
  }
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sCLOCK *inst, double t)
{
    const double forever  = 1e308;    // return forever to not limit timestep size

   if (inst->incr_t <= 0) return forever;
   return inst->incr_t;
}

extern "C" __declspec(dllexport) void Destroy(struct sCLOCK *inst)
{
   free(inst);
}
