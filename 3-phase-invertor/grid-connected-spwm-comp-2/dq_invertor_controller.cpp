// Automatically generated C++ file on Fri Apr 17 07:27:10 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD dq_invertor_controller.cpp kernel32.lib

#include <malloc.h>
#include <math.h>
#include <inttypes.h>
#include "inc/dsogi.h"
#include "inc/spwm.h"
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
#undef g1
#undef g2
#undef g3
#undef g4
#undef g5
#undef g6
#undef Valpha
#undef Vbeta
#undef Vq
#undef Vd

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

   double trg_a_r; // trigger cmp at rising phase a
   double trg_a_f; // trigger cmp at falling phase a

   double trg_b_r; // trigger cmp at rising phase b
   double trg_b_f; // trigger cmp at falling phase b

   double trg_c_r; // trigger cmp at rising phase c
   double trg_c_f; // trigger cmp at falling phase c

   double F;
   double Fsw;
   double L;
   double Vdc;

   double Valpha;
   double Vbeta;

   double Id;
   double Iq;

   double Vd;
   double Vq;

   double Vas;
   double Vbs;

   double Ids;
   double Iqs;

   double theta;
   double sinValue;
   double cosValue;

   double g1;
   double g2;
   double g3;
   double g4;
   double g5;
   double g6;

   DSOGI_PLL dsogi;
   SPWM pwm;
   DQController dq;
};

void calculate_theta(struct sDQ_INVERTOR_CONTROLLER *inst, double t){
   double theta =  inst->dsogi(inst->Valpha, inst->Vbeta, t);

   double sinValue = sin(theta);
   double cosValue = cos(theta);

   inst->theta = theta;
   inst->sinValue = sinValue;
   inst->cosValue = cosValue;
   inst->Vd =  inst->Valpha * cosValue + inst->Vbeta * sinValue;
   inst->Vq = -inst->Valpha * sinValue + inst->Vbeta * cosValue;
};

void dq_controller(struct sDQ_INVERTOR_CONTROLLER *inst, double t){

   inst->dq(inst->Ids, inst->Iqs, inst->Id, inst->Iq, inst->Vd, inst->Vq, t);

   inst->Vas = inst->cosValue * inst->dq.Vds - inst->sinValue * inst->dq.Vqs;
   inst->Vbs = inst->sinValue * inst->dq.Vds + inst->cosValue * inst->dq.Vqs;
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
   double &g1     = data[15].d; // output
   double &g2     = data[16].d; // output
   double &g3     = data[17].d; // output
   double &g4     = data[18].d; // output
   double &g5     = data[19].d; // output
   double &g6     = data[20].d; // output
   double &Valpha = data[21].d; // output
   double &Vbeta  = data[22].d; // output
   double &Vq     = data[23].d; // output
   double &Vd     = data[24].d; // output


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

      inst->g1 = -7.0;
      inst->g2 = -7.0;
      inst->g3 = -7.0;
      inst->g4 = -7.0;
      inst->g5 = -7.0;
      inst->g6 = -7.0;

      inst->trg_e = 0.0;
      inst->trg_m = inst->xpeak / inst->mcu_clk;

      inst->maxstep = 10e-12;
      double Ts = 1.0 / (2.0 * Fsw);

      inst->dsogi.init(KP_PLL, KI_PLL, F);
      //inst->pi_d.init(Kp, Ki);
      //inst->pi_q.init(Kp, Ki);
      inst->pwm.init(Vdc, inst->xpeak);
      inst->dq.init(Kp, Ki, L, F);
   }
   struct sDQ_INVERTOR_CONTROLLER *inst = *opaque;

// Implement module evaluation code here:
   if((inst->t_prev <= inst->trg_e)&&(t >= inst->trg_e)){
      inst->xcntr++;
      inst->maxstep = inst->xpeak / inst->mcu_clk;

      // sample voltage
      inst->Valpha = 2.0 * (Va - 0.5 * (Vb + Vc)) / 3.0;
      inst->Vbeta  = sqrt(3.0) * (Vc - Vb) / 3.0;
      inst->Vdc = Vdc * 2.0;

      calculate_theta(inst, t);
      inst->pwm(inst->Valpha, inst->Vbeta);

      double t_mid = inst->xpeak / inst->mcu_clk;

      inst->trg_m   = inst->trg_e + t_mid;

      inst->trg_a_r = inst->trg_e + inst->pwm.switchtime_a / inst->mcu_clk ;
      inst->trg_a_f = inst->trg_e + (2 * t_mid - inst->pwm.switchtime_a) / inst->mcu_clk;

      inst->trg_b_r = inst->trg_e + inst->pwm.switchtime_b /  inst->mcu_clk ;
      inst->trg_b_f = inst->trg_e + (2 * t_mid - inst->pwm.switchtime_b) / inst->mcu_clk;

      inst->trg_c_r = inst->trg_e + inst->pwm.switchtime_c / inst->mcu_clk ;
      inst->trg_c_f = inst->trg_e + (2 * t_mid - inst->pwm.switchtime_c) / inst->mcu_clk;

      inst->trg_e   = inst->trg_e + 2 * t_mid;
   }

   if((inst->t_prev <= inst->trg_m)&&(t >= inst->trg_m))
   {
      inst->xcntr++;

      // sample current
      double Ialpha = 2.0 * (Ia - 0.5 * (Ib + Ic)) / 3.0;
      double Ibeta  = sqrt(3.0) * (Ic - Ib) / 3.0;

      inst->Ids = Ids;
      inst->Iqs = Iqs;

      inst->Id =  Ialpha * inst->cosValue + Ibeta * inst->sinValue;
      inst->Iq = -Ialpha * inst->sinValue + Ibeta * inst->cosValue;

      dq_controller(inst, t);
   }

   if(t <= inst->trg_m){
      if((inst->t_prev <= inst->trg_a_r)&&(t >= inst->trg_a_r))
      {
         inst->xcntr++;
         inst->g1 =  15.0;
         inst->g2 = -7.0;
      }
      if((inst->t_prev <= inst->trg_b_r)&&(t >= inst->trg_b_r))
      {
         inst->xcntr++;
         inst->g3 =  15.0;
         inst->g4 = -7.0;
      }
      if((inst->t_prev <= inst->trg_c_r)&&(t >= inst->trg_c_r))
      {
         inst->xcntr++;
         inst->g5 =  15.0;
         inst->g6 = -7.0;
      }
   }
   else{
      if((inst->t_prev <= inst->trg_a_f)&&(t >= inst->trg_a_f))
      {
         inst->xcntr++;
         inst->g1 = -7.0;
         inst->g2 =  15.0;
      }
      if((inst->t_prev <= inst->trg_b_f)&&(t >= inst->trg_b_f))
      {
         inst->xcntr++;
         inst->g3 = -7.0;
         inst->g4 =  15.0;
      }
      if((inst->t_prev <= inst->trg_c_f)&&(t >= inst->trg_c_f))
      {
         inst->xcntr++;
         inst->g5 = -7.0;
         inst->g6 =  15.0;
      }
   }

   g1 = inst->g1;
   g2 = inst->g2;
   g3 = inst->g3;
   g4 = inst->g4;
   g5 = inst->g5;
   g6 = inst->g6;

   Valpha = inst->Vas;
   Vbeta  = inst->Vbs;

   Vd = inst->dq.Vds;
   Vq = inst->dq.Vqs;

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
