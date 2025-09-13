// Automatically generated C++ file on Fri Sep 12 19:33:32 2025
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
#undef cnt

struct sPFC_CONTROLLER
{
  // declare the structure here
  //unsigned short lastV;
  unsigned short counter;
  double pwm;


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
   double          Vrec = data[0].d ; // input
   double          FREQ = data[1].d ; // input parameter
   double          TTOL = data[2].d ; // input parameter
   double         &PWM  = data[3].d ; // output
   double         &SW   = data[4].d ; // output
   unsigned short &cnt  = data[5].us; // output

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
      inst->pwm   = 0;
      inst->counter = 0;
   }
   struct sPFC_CONTROLLER *inst = *opaque;

   inst->incr_t = inst->next_t - t;

   // clock state change triggered
   if (t >= inst->next_t) {
      inst->tickCnt++;

      inst->counter++;
      if(inst->counter > 65535)
         inst->counter = 0;

      inst->next_t = calcTickTime(inst);
      inst->incr_t = inst->ttol;

      unsigned short v = (unsigned short) Vrec * 4096 ;

      inst->pwm = v > inst->counter ? 15.0 : 0.0;
   }

   PWM = inst->pwm;
   cnt = inst->counter;
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
