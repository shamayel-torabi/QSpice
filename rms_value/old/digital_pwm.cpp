// Automatically generated C++ file on Fri Sep 19 18:41:09 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD digital_pwm.cpp kernel32.lib

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
#undef pwm
#undef Vin
#undef Iin
#undef Vout
#undef duty
#undef Iref

#define PI(y_prev,x,x_prev,kp,kits) y_prev+kp*(x-x_prev)+kits*x

#define TWO_PI 6.28318530717959


struct InstanceData
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


  double verror;
  double verror_prev;
  double iref;
  double ierror;
  double ierror_prev;
  double out;

  double ek_1;
  double ek_2;
  double yk_1;
  double yk_2;
};

extern "C" __declspec(dllexport) void digital_pwm(struct InstanceData **opaque, double t, union uData *data)
{
   double  Vin  = data[ 0].d; // input
   double  Iin  = data[ 1].d; // input
   double  Vout = data[ 2].d; // input
   const double  FREQ = data[ 3].d; // input parameter
   const double  TTOL = data[ 4].d; // input parameter
   const double  PER  = data[ 5].d; // input parameter
   const double  Kp   = data[ 6].d; // input parameter
   const double  Ki   = data[ 7].d; // input parameter
   double &pwm  = data[ 8].d; // output
   double &duty = data[ 9].d; // output
   double &Iref = data[10].d; // output



   if(!*opaque)
   {
      *opaque = (struct InstanceData *) malloc(sizeof(struct InstanceData));
      bzero(*opaque, sizeof(struct InstanceData));

      struct InstanceData *inst = *opaque;

      inst->startTrg = 0;
      inst->maxstep = 1e-9;
      inst->ttol = TTOL;
      inst->mcu_clk = FREQ;
      inst->per = PER;
      inst->counter = 0;

      inst->ek_1 = 0;
      inst->ek_2 = 0;
      inst->yk_1 = 0;
      inst->yk_2 = 0;
   }
   struct InstanceData *inst = *opaque;

// Implement module evaluation code here:

   if((inst->t_prev <= inst->startTrg)&&(t >= inst->startTrg)){
      inst->startTrg += 1.0/inst->mcu_clk;
      inst->maxstep = 1.0/inst->mcu_clk;

      inst->counter++;
      if(inst->counter >= inst->per)
         inst->counter = 0;

      //inst->verror = 2.0 - Vout;
      //inst->iref = PI(inst->iref, inst->verror, inst->verror_prev, 1.0, 0.001623);
      //if(inst->iref > 500) inst->iref = 500;
      //if(inst->iref < 0) inst->iref = 0;
      //inst->verror_prev = inst->verror;

      double W0 = TWO_PI*50.0;
      double Wc = 10.0;
      double Ts = 1.0 / inst->per;

      double a1 = 4 * Ki * Ts * Wc;
      double b0 = Ts * Ts * W0 * W0 + 4 * Ts * Wc + 4;
      double b1 = 2 * Ts * Ts * W0 * W0 - 8;
      double b2 = Ts * Ts * W0 * W0 - 4 * Ts * Wc + 4;

      double e = Vin - Iin;
      double y = (a1 * e - a1 * inst->ek_2 - b1 * inst->yk_1 - b2 * inst->yk_2) / b0;

      inst->ek_2 = inst->ek_1;
      inst->ek_1 = e;
      inst->yk_2 = inst->yk_1;
      inst->yk_1 = y;

      inst->out = y * 1000;
      Iref = e;

      //inst->ierror = inst->iref - Iin;
      //inst->out = PI(inst->out, inst->ierror, inst->ierror_prev, Kp, Ki);
      //if(inst->out > inst->per) inst->out = inst->per;
      //if(inst->out < 0) inst->out = 0;
      //inst->ierror_prev = inst->ierror;


      duty = inst->out;

      unsigned short v = unsigned short (inst->out);

      if(v > inst->counter)
         pwm = 15.0;
      else
         pwm = 0.0;
   }

   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct InstanceData *inst, double t)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sEPWM
}

extern "C" __declspec(dllexport) void Trunc(struct InstanceData *inst, double t, union uData *data, double *timestep)
{
   // limit the timestep to a tolerance if the circuit causes a change in struct sDIGITAL_CLOCK
   if(*timestep > inst->ttol)
   {
      struct InstanceData tmp = *inst;
      digital_pwm(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = inst->ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct InstanceData *inst)
{
   free(inst);
}
