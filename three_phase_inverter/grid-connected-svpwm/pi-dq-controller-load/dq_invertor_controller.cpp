// Automatically generated C++ file on Fri Apr 17 07:27:10 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD dq_invertor_controller.cpp kernel32.lib

#include <malloc.h>
#include <math.h>
#include <inttypes.h>
#include "inc/dsogi.h"
#include "inc/dq_controller.h"

#define KP_PLL    9.2E-4
#define KI_PLL    42.3E-4
#define PI        3.1415926535897932384626

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
#undef Va
#undef Vb
#undef Vc
#undef Ic
#undef Ib
#undef Ia
#undef Ids
#undef Iqs
#undef Vdc
#undef Valpha
#undef Vbeta
#undef Vq
#undef Vd
#undef theta

struct sDQ_INVERTOR_CONTROLLER
{
  // declare the structure here
   uint64_t xcntr;
   double maxstep;
   double mcu_clk;
   double xpeak;
   double t_prev;

   double trg_e;   // trigger at start period
   double trg_m;   // trigger at half period

   double F;
   double Fsw;
   double L;
   double Vdc;

   double Valpha;
   double Vbeta;

   double Ialpha;
   double Ibeta;

   double Var;
   double Vbr;

   double Ids;
   double Iqs;

   double theta;
   double sinValue;
   double cosValue;

   DSOGI_PLL dsogi;
   DQController dq;
};

void calculate_theta(struct sDQ_INVERTOR_CONTROLLER *inst, double t){
   inst->theta = inst->dsogi(inst->Valpha, inst->Vbeta, t);;
   inst->sinValue = sin(inst->theta);
   inst->cosValue = cos(inst->theta);
};

void dq_controller(struct sDQ_INVERTOR_CONTROLLER *inst, double t){
   double sinValue = inst->sinValue;
   double cosValue = inst->cosValue;

   double Vd =  inst->Valpha * cosValue + inst->Vbeta * sinValue;
   double Vq = -inst->Valpha * sinValue + inst->Vbeta * cosValue;

   double Id =  inst->Ialpha * cosValue + inst->Ibeta * sinValue;
   double Iq = -inst->Ialpha * sinValue + inst->Ibeta * cosValue;

   inst->dq(inst->Ids, inst->Iqs, Id, Iq, Vd, Vq, t);

   inst->Var = cosValue * inst->dq.Vdr - sinValue * inst->dq.Vqr;
   inst->Vbr = sinValue * inst->dq.Vdr + cosValue * inst->dq.Vqr;
};

extern "C" __declspec(dllexport) void dq_invertor_controller(struct sDQ_INVERTOR_CONTROLLER **opaque, double t, union uData *data)
{
   double  Va     = data[ 0].d; // input
   double  Vb     = data[ 1].d; // input
   double  Vc     = data[ 2].d; // input
   double  Ic     = data[ 3].d; // input
   double  Ib     = data[ 4].d; // input
   double  Ia     = data[ 5].d; // input
   double  Ids    = data[ 6].d; // input
   double  Iqs    = data[ 7].d; // input
   double  Vdc    = data[ 8].d; // input
   double  Fsw    = data[ 9].d; // input parameter
   double  F      = data[10].d; // input parameter
   double  L      = data[11].d; // input parameter
   double  Kp     = data[12].d; // input parameter
   double  Ki     = data[13].d; // input parameter
   double  Fclk   = data[14].d; // input parameter
   double &Valpha = data[15].d; // output
   double &Vbeta  = data[16].d; // output
   double &Vq     = data[17].d; // output
   double &Vd     = data[18].d; // output
   double &theta  = data[19].d; // output


   if(!*opaque)
   {
      *opaque = (struct sDQ_INVERTOR_CONTROLLER *) malloc(sizeof(struct sDQ_INVERTOR_CONTROLLER));
      bzero(*opaque, sizeof(struct sDQ_INVERTOR_CONTROLLER));

      struct sDQ_INVERTOR_CONTROLLER *inst = *opaque;

      inst->Fsw = Fsw;
      inst->F = F;
      inst->L = L;

      inst->mcu_clk = Fclk;
      inst->xpeak = Fclk / (2 * Fsw);

      inst->trg_e = 0.0;
      inst->trg_m = inst->xpeak / inst->mcu_clk;

      inst->maxstep = 10e-12;

      inst->dsogi.init(KP_PLL, KI_PLL, F);
      inst->dq.init(Kp, Ki, L, F, Vdc, 100.0);
   }
   struct sDQ_INVERTOR_CONTROLLER *inst = *opaque;

// Implement module evaluation code here:
   if((inst->t_prev <= inst->trg_e)&&(t >= inst->trg_e)){
      inst->xcntr++;
      inst->maxstep = inst->xpeak / inst->mcu_clk;

      // sample voltage
      inst->Valpha = 2.0 * (Va - 0.5 * (Vb + Vc)) / 3.0;
      inst->Vbeta  = sqrt(3.0) * (Vc - Vb) / 3.0;
      inst->Vdc = Vdc;

      calculate_theta(inst, t);

      inst->trg_m   = inst->trg_e + inst->xpeak / inst->mcu_clk;
      inst->trg_e   = inst->trg_e + 2 * inst->xpeak /  inst->mcu_clk;
   }

   if((inst->t_prev <= inst->trg_m)&&(t >= inst->trg_m))
   {
      inst->xcntr++;

      // sample current
      inst->Ialpha = 2.0 * (Ia - 0.5 * (Ib + Ic)) / 3.0;
      inst->Ibeta  = sqrt(3.0) * (Ic - Ib) / 3.0;

      inst->Ids = Ids;
      inst->Iqs = Iqs;

      dq_controller(inst, t);
   }

   Valpha = inst->Var;
   Vbeta  = inst->Vbr;

   Vd = inst->dq.Vdr;
   Vq = inst->dq.Vqr;

   theta = inst->theta;

   inst->t_prev = t;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sDQ_INVERTOR_CONTROLLER *inst, double t)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sSVPWM
}

extern "C" __declspec(dllexport) void Trunc(struct sDQ_INVERTOR_CONTROLLER *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sDQ_INVERTOR_CONTROLLER
   const double ttol = 10e-12; // 1ns default tolerance
   if(*timestep > ttol)
   {
      struct sDQ_INVERTOR_CONTROLLER tmp = *inst;
      dq_invertor_controller(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = ttol;
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sDQ_INVERTOR_CONTROLLER *inst)
{
   free(inst);
}
