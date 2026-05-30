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

#define KP_PLL    92
#define KI_PLL    4230


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
#undef Ia
#undef Ib
#undef Ic
#undef Vset
#undef Vdc
#undef Valph
#undef Vbeta
#undef Vq
#undef Vd
#undef Iq
#undef Id
#undef Vdcf
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
   double trg_q_r; // trigger at quarter rise period
   double trg_q_f; // trigger at quarter fall period

   double F;
   double Fsw;
   double L;

   double Vdc;
   double Vset;

   double Valph;
   double Valph_1;
   double Valph_2;

   double Vbeta;
   double Vbeta_1;
   double Vbeta_2;

   double Ialph;
   double Ialph_1;
   double Ialph_2;

   double Ibeta;
   double Ibeta_1;
   double Ibeta_2;

   double Ialph_k[4];
   double Ibeta_k[4];

   double Valph_k[4];
   double Vbeta_k[4];

   double Var;
   double Vbr;

   double Id;
   double Iq;

   double theta;

   DSOGI dsogi;
   DQController dq;
};


void dq_controller(struct sDQ_INVERTOR_CONTROLLER *inst){
   double theta = inst->dsogi(inst->Valph, inst->Vbeta);

   double sinValue = sin(theta);
   double cosValue = cos(theta);

   dq_type in;

   in.Vdc  = inst->Vdc;
   in.Vset = inst->Vset;

   in.Vod =  inst->Valph * cosValue + inst->Vbeta * sinValue;
   in.Voq = -inst->Valph * sinValue + inst->Vbeta * cosValue;

   in.ILd =  inst->Ialph * cosValue + inst->Ibeta * sinValue;
   in.ILq = -inst->Ialph * sinValue + inst->Ibeta * cosValue;

   inst->dq(&in);

   inst->Id = in.ILd;
   inst->Iq = in.ILq;
   inst->Var = cosValue * inst->dq.Vid - sinValue * inst->dq.Viq;
   inst->Vbr = sinValue * inst->dq.Vid + cosValue * inst->dq.Viq;
   inst->theta = theta;
};

extern "C" __declspec(dllexport) void dq_invertor_controller(struct sDQ_INVERTOR_CONTROLLER **opaque, double t, union uData *data)
{
   double  Va    = data[ 0].d; // input
   double  Vb    = data[ 1].d; // input
   double  Vc    = data[ 2].d; // input
   double  Ia    = data[ 3].d; // input
   double  Ib    = data[ 4].d; // input
   double  Ic    = data[ 5].d; // input
   double  Vset  = data[ 6].d; // input
   double  Vdc   = data[ 7].d; // input
   double  Fsw   = data[ 8].d; // input parameter
   double  F     = data[ 9].d; // input parameter
   double  L     = data[10].d; // input parameter
   double  Kp    = data[11].d; // input parameter
   double  Ki    = data[12].d; // input parameter
   double  Fclk  = data[13].d; // input parameter
   double &Valph = data[14].d; // output
   double &Vbeta = data[15].d; // output
   double &Vq    = data[16].d; // output
   double &Vd    = data[17].d; // output
   double &Iq    = data[18].d; // output
   double &Id    = data[19].d; // output
   double &Vdcf  = data[20].d; // output
   double &theta = data[21].d; // output


   if(!*opaque)
   {
      *opaque = (struct sDQ_INVERTOR_CONTROLLER *) malloc(sizeof(struct sDQ_INVERTOR_CONTROLLER));
      bzero(*opaque, sizeof(struct sDQ_INVERTOR_CONTROLLER));

      struct sDQ_INVERTOR_CONTROLLER *inst = *opaque;

      inst->Fsw = Fsw;
      inst->F = F;
      inst->L = L;
      double Ts = 1.0 / Fsw;
      double w  = 2.0 * PI * F;

      inst->mcu_clk = Fclk;
      inst->xpeak = Fclk / (2 * Fsw);

      inst->trg_e = 0.0;
      inst->trg_m = inst->xpeak / inst->mcu_clk;

      inst->maxstep = 10e-12;

      inst->dsogi.init(KP_PLL, KI_PLL, F, Ts);
      inst->dq.init(Kp, Ki, w, L, Ts);

   }
   struct sDQ_INVERTOR_CONTROLLER *inst = *opaque;

// Implement module evaluation code here:
   if((inst->t_prev <= inst->trg_e)&&(t >= inst->trg_e)){
      inst->xcntr++;
      inst->maxstep = inst->xpeak / inst->mcu_clk;

      dq_controller(inst);

      //current sample 0 at start of period
      inst->Ialph_k[0] = 2.0 * (Ia - 0.5 * (Ib + Ic)) / 3.0;;
      inst->Ibeta_k[0] = sqrt(3.0) * (Ic - Ib) / 3.0;

      inst->Valph_k[0] = 2.0 * (Va - 0.5 * (Vb + Vc)) / 3.0;
      inst->Vbeta_k[0]  = sqrt(3.0) * (Vc - Vb) / 3.0;

      double Iam = (inst->Ialph_k[0] + inst->Ialph_k[1] + inst->Ialph_k[2] + inst->Ialph_k[3]) / 4.0;
      double Ibm = (inst->Ibeta_k[0] + inst->Ibeta_k[1] + inst->Ibeta_k[2] + inst->Ibeta_k[3]) / 4.0;

      inst->Ialph = (Iam + 2.0 * inst->Ialph_1 + inst->Ialph_2) / 4.0;
      inst->Ibeta = (Ibm + 2.0 * inst->Ibeta_1 + inst->Ibeta_2) / 4.0;

      inst->Ialph_2 = inst->Ialph_1;
      inst->Ialph_1 = Iam;

      inst->Ibeta_2 = inst->Ibeta_1;
      inst->Ibeta_1 = Ibm;

      double Vam = (inst->Valph_k[0] + inst->Valph_k[1] + inst->Valph_k[2] + inst->Valph_k[3]) / 4.0;
      double Vbm = (inst->Vbeta_k[0] + inst->Vbeta_k[1] + inst->Vbeta_k[2] + inst->Vbeta_k[3]) / 4.0;

      inst->Valph = (Vam + 2.0 * inst->Valph_1 + inst->Valph_2) / 4.0;
      inst->Vbeta = (Vbm + 2.0 * inst->Vbeta_1 + inst->Vbeta_2) / 4.0;

      inst->Valph_2 = inst->Valph_1;
      inst->Valph_1 = Vam;

      inst->Vbeta_2 = inst->Vbeta_1;
      inst->Vbeta_1 = Vbm;
      
      double quarter = inst->xpeak / 2.0;
      inst->trg_m   = inst->trg_e + inst->xpeak / inst->mcu_clk;
      inst->trg_q_r = inst->trg_e + quarter / inst->mcu_clk;
      inst->trg_q_f = inst->trg_e + (2 * inst->xpeak - quarter) / inst->mcu_clk;
      inst->trg_e   = inst->trg_e + 2 * inst->xpeak /  inst->mcu_clk;
   }

   if((inst->t_prev <= inst->trg_q_r)&&(t >= inst->trg_q_r)){
      inst->xcntr++;

      // current sample 1 at 1/4 period
      inst->Ialph_k[1] = 2.0 * (Ia - 0.5 * (Ib + Ic)) / 3.0;;
      inst->Ibeta_k[1] = sqrt(3.0) * (Ic - Ib) / 3.0;

      inst->Valph_k[1] = 2.0 * (Va - 0.5 * (Vb + Vc)) / 3.0;
      inst->Vbeta_k[1]  = sqrt(3.0) * (Vc - Vb) / 3.0;
   }


   if((inst->t_prev <= inst->trg_m)&&(t >= inst->trg_m)){
      inst->xcntr++;

      // current sample 2 at 1/2 period
      inst->Ialph_k[2] = 2.0 * (Ia - 0.5 * (Ib + Ic)) / 3.0;
      inst->Ibeta_k[2] = sqrt(3.0) * (Ic - Ib) / 3.0;

      inst->Valph_k[2] = 2.0 * (Va - 0.5 * (Vb + Vc)) / 3.0;
      inst->Vbeta_k[2]  = sqrt(3.0) * (Vc - Vb) / 3.0;

      inst->Vdc  = Vdc;
      inst->Vset = Vset;

      double Iam = (inst->Ialph_k[0] + inst->Ialph_k[1] + inst->Ialph_k[2] + inst->Ialph_k[3]) / 4.0;
      double Ibm = (inst->Ibeta_k[0] + inst->Ibeta_k[1] + inst->Ibeta_k[2] + inst->Ibeta_k[3]) / 4.0;

      inst->Ialph = (Iam + 2.0 * inst->Ialph_1 + inst->Ialph_2) / 4.0;
      inst->Ibeta = (Ibm + 2.0 * inst->Ibeta_1 + inst->Ibeta_2) / 4.0;

      inst->Ialph_2 = inst->Ialph_1;
      inst->Ialph_1 = Iam;

      inst->Ibeta_2 = inst->Ibeta_1;
      inst->Ibeta_1 = Ibm;

      double Vam = (inst->Valph_k[0] + inst->Valph_k[1] + inst->Valph_k[2] + inst->Valph_k[3]) / 4.0;
      double Vbm = (inst->Vbeta_k[0] + inst->Vbeta_k[1] + inst->Vbeta_k[2] + inst->Vbeta_k[3]) / 4.0;

      inst->Valph = (Vam + 2.0 * inst->Valph_1 + inst->Valph_2) / 4.0;
      inst->Vbeta = (Vbm + 2.0 * inst->Vbeta_1 + inst->Vbeta_2) / 4.0;

      inst->Valph_2 = inst->Valph_1;
      inst->Valph_1 = Vam;

      inst->Vbeta_2 = inst->Vbeta_1;
      inst->Vbeta_1 = Vbm;
   }
   
   if((inst->t_prev <= inst->trg_q_f)&&(t >= inst->trg_q_f)){
      inst->xcntr++;

      // current sample 3 at 3/4 period
      inst->Ialph_k[3] = 2.0 * (Ia - 0.5 * (Ib + Ic)) / 3.0;;
      inst->Ibeta_k[3] = sqrt(3.0) * (Ic - Ib) / 3.0;

      inst->Valph_k[3] = 2.0 * (Va - 0.5 * (Vb + Vc)) / 3.0;
      inst->Vbeta_k[3]  = sqrt(3.0) * (Vc - Vb) / 3.0;
   }

   Valph = inst->Var;
   Vbeta = inst->Vbr;

   Vd = inst->dq.Vid;
   Vq = inst->dq.Viq;

   Id = inst->Id;
   Iq = inst->Iq;

   Vdcf = inst->dq.Vdcf;
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
