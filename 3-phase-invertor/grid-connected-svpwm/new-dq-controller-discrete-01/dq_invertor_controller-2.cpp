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
#undef Ic
#undef Ib
#undef Ia
#undef p
#undef q
#undef Vdc
#undef Valph
#undef Vbeta
#undef Vq
#undef Vd
#undef theta
#undef Vdcf
#undef Id
#undef Iq
#undef Ioa
#undef Iob
#undef Ioc

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
   double P;
   double Q;

   double Valph;
   double Vbeta;

   double Valph_k[4];
   double Vbeta_k[4];

   double Ialph;
   double Ibeta;

   double Ialph_k[4];
   double Ibeta_k[4];

   double Io_alph;
   double Io_beta;

   double Io_alph_k[4];
   double Io_beta_k[4];

   double Var;
   double Vbr;

   double Id;
   double Iq;

   double theta;

   DSOGI dsogi;
   DQController dq;
};

void dq_controller(struct sDQ_INVERTOR_CONTROLLER *inst){
   dq_type in;

   double Valph = inst->Valph;
   double Vbeta = inst->Vbeta;

   double Ialph = inst->Ialph;
   double Ibeta = inst->Ibeta;

   double Io_alph = inst->Io_alph;
   double Io_beta = inst->Io_beta;

   double sinValue = sin(inst->theta);
   double cosValue = cos(inst->theta);

   in.Vdc = inst->Vdc;
   in.P = inst->P;
   in.Q = inst->Q;
   in.Vod =  Valph * cosValue + Vbeta * sinValue;
   in.Voq = -Valph * sinValue + Vbeta * cosValue;
   in.ILd =  Ialph * cosValue + Ibeta * sinValue;
   in.ILq = -Ialph * sinValue + Ibeta * cosValue;
   in.Iod =  Io_alph * cosValue + Io_beta * sinValue;
   in.Ioq = -Io_alph * sinValue + Io_beta * cosValue;

   inst->dq(&in);

   inst->Id = Ialph * cosValue + Ibeta * sinValue;;
   inst->Iq = -Ialph * sinValue + Ibeta * cosValue;
   inst->Var = cosValue * inst->dq.Vid - sinValue * inst->dq.Viq;
   inst->Vbr = sinValue * inst->dq.Vid + cosValue * inst->dq.Viq;
};

extern "C" __declspec(dllexport) void dq_invertor_controller(struct sDQ_INVERTOR_CONTROLLER **opaque, double t, union uData *data)
{
   double  Va    = data[ 0].d; // input
   double  Vb    = data[ 1].d; // input
   double  Vc    = data[ 2].d; // input
   double  Ic    = data[ 3].d; // input
   double  Ib    = data[ 4].d; // input
   double  Ia    = data[ 5].d; // input
   double  p     = data[ 6].d; // input
   double  q     = data[ 7].d; // input
   double  Vdc   = data[ 8].d; // input
   double  Ioa   = data[ 9].d; // input
   double  Iob   = data[10].d; // input
   double  Ioc   = data[11].d; // input
   double  Fsw   = data[12].d; // input parameter
   double  F     = data[13].d; // input parameter
   double  L     = data[14].d; // input parameter
   double  Kp    = data[15].d; // input parameter
   double  Ki    = data[16].d; // input parameter
   double  Fclk  = data[17].d; // input parameter
   double &Valph = data[18].d; // output
   double &Vbeta = data[19].d; // output
   double &Vq    = data[20].d; // output
   double &Vd    = data[21].d; // output
   double &theta = data[22].d; // output
   double &Vdcf  = data[23].d; // output
   double &Id    = data[24].d; // output
   double &Iq    = data[25].d; // output


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

      inst->Valph_k[0] = 2.0 * (Va - 0.5 * (Vb + Vc)) / 3.0;
      inst->Vbeta_k[0]  = sqrt(3.0) * (Vc - Vb) / 3.0;

      inst->Ialph_k[0] = 2.0 * (Ia - 0.5 * (Ib + Ic)) / 3.0;;
      inst->Ibeta_k[0] = sqrt(3.0) * (Ic - Ib) / 3.0;

      inst->Io_alph_k[0] = 2.0 * (Ioa - 0.5 * (Iob + Ioc)) / 3.0;;
      inst->Io_beta_k[0] = sqrt(3.0) * (Ioc - Iob) / 3.0;

      inst->theta = inst->dsogi(inst->Valph, inst->Vbeta);

      double quarter = inst->xpeak / 2.0;
      inst->trg_m   = inst->trg_e + inst->xpeak / inst->mcu_clk;
      inst->trg_q_r = inst->trg_e + quarter / inst->mcu_clk;
      inst->trg_q_f = inst->trg_e + (2 * inst->xpeak - quarter) / inst->mcu_clk;
      inst->trg_e   = inst->trg_e + 2 * inst->xpeak /  inst->mcu_clk;
   }

   if((inst->t_prev <= inst->trg_q_r)&&(t >= inst->trg_q_r)){
      inst->xcntr++;

      // current sample 1 at 1/4 period
      inst->Valph_k[1] = 2.0 * (Va - 0.5 * (Vb + Vc)) / 3.0;
      inst->Vbeta_k[1]  = sqrt(3.0) * (Vc - Vb) / 3.0;

      inst->Ialph_k[1] = 2.0 * (Ia - 0.5 * (Ib + Ic)) / 3.0;;
      inst->Ibeta_k[1] = sqrt(3.0) * (Ic - Ib) / 3.0;

      inst->Io_alph_k[1] = 2.0 * (Ioa - 0.5 * (Iob + Ioc)) / 3.0;;
      inst->Io_beta_k[1] = sqrt(3.0) * (Ioc - Iob) / 3.0;
   }

   if((inst->t_prev <= inst->trg_m)&&(t >= inst->trg_m)){
      inst->xcntr++;

      inst->Vdc = Vdc;
      inst->P = p;
      inst->Q = q;

      inst->Valph_k[2] = 2.0 * (Va - 0.5 * (Vb + Vc)) / 3.0;
      inst->Vbeta_k[2]  = sqrt(3.0) * (Vc - Vb) / 3.0;

      inst->Ialph_k[2] = 2.0 * (Ia - 0.5 * (Ib + Ic)) / 3.0;;
      inst->Ibeta_k[2] = sqrt(3.0) * (Ic - Ib) / 3.0;

      inst->Io_alph_k[2] = 2.0 * (Ioa - 0.5 * (Iob + Ioc)) / 3.0;;
      inst->Io_beta_k[2] = sqrt(3.0) * (Ioc - Iob) / 3.0;
   }

   if((inst->t_prev <= inst->trg_q_f)&&(t >= inst->trg_q_f)){
      inst->xcntr++;

      // current sample 3 at 3/4 period
      inst->Valph_k[3] = 2.0 * (Va - 0.5 * (Vb + Vc)) / 3.0;
      inst->Vbeta_k[3]  = sqrt(3.0) * (Vc - Vb) / 3.0;

      inst->Ialph_k[3] = 2.0 * (Ia - 0.5 * (Ib + Ic)) / 3.0;;
      inst->Ibeta_k[3] = sqrt(3.0) * (Ic - Ib) / 3.0;

      inst->Io_alph_k[3] = 2.0 * (Ioa - 0.5 * (Iob + Ioc)) / 3.0;;
      inst->Io_beta_k[3] = sqrt(3.0) * (Ioc - Iob) / 3.0;

      inst->Valph = (inst->Valph_k[0] + inst->Valph_k[1] + inst->Valph_k[2] + inst->Valph_k[3]) / 4.0;
      inst->Vbeta = (inst->Vbeta_k[0] + inst->Vbeta_k[1] + inst->Vbeta_k[2] + inst->Vbeta_k[3]) / 4.0;

      inst->Ialph = (inst->Ialph_k[0] + inst->Ialph_k[1] + inst->Ialph_k[2] + inst->Ialph_k[3]) / 4.0;
      inst->Ibeta = (inst->Ibeta_k[0] + inst->Ibeta_k[1] + inst->Ibeta_k[2] + inst->Ibeta_k[3]) / 4.0;

      inst->Io_alph = (inst->Io_alph_k[0] + inst->Io_alph_k[1] + inst->Io_alph_k[2] + inst->Io_alph_k[3]) / 4.0;
      inst->Io_beta = (inst->Io_beta_k[0] + inst->Io_beta_k[1] + inst->Io_beta_k[2] + inst->Io_beta_k[3]) / 4.0;

      dq_controller(inst);
   }

   Valph = inst->Var;
   Vbeta = inst->Vbr;

   Vd = inst->dq.Vid;
   Vq = inst->dq.Viq;

   Id = inst->Id;
   Iq = inst->Iq;

   Vdcf  = inst->dq.Vdcf;
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
