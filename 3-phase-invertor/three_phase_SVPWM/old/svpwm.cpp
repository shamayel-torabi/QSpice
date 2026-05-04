// Automatically generated C++ file on Fri Apr 10 08:34:47 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD svpwm.cpp kernel32.lib

#include <malloc.h>
#include <math.h>


/* Maximum Voltage applying */
#define _U_DC 560
/* Square Root of 3 */
#define _SQRT3 1.73205081
/* Pi divided by 3 */
#define _PIdiv3 1.04719755

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
#undef clk
#undef g1
#undef g2
#undef g3
#undef g4
#undef g5
#undef g6
#undef Vb
#undef cnt
#undef out

struct sSVPWM
{
  // declare the structure here
   long long int xcntr;
   double maxstep;
   double t_prev;

   double trg_m;     // trigger at half period
   double trg_e;     // trigger at full period
   double trg_a_r;   // trigger cmp at rising phase a
   double trg_a_f;   // trigger cmp at falling phase a
   double trg_b_r;   // trigger cmp at rising phase b
   double trg_b_f;   // trigger cmp at falling phase b
   double trg_c_r;   // trigger cmp at rising phase c
   double trg_c_f;   // trigger cmp at falling phase c

   double mcu_clk; 
   double xpeak;

   double g1;
   double g2;
   double g3;
   double g4;
   double g5;
   double g6;

   unsigned short counter;
   unsigned short switchtime[3];

   double theta;
};

void svpwm_clculate(sSVPWM *inst, double U_alpha, double U_beta) {
   double  U_max = (1/_SQRT3)*_U_DC;
   double T_halfsample = inst->xpeak / inst->mcu_clk;

   unsigned char sector = inst->theta/_PIdiv3;
   double U_ref = hypotf(U_alpha, U_beta);

   if (U_ref > U_max) {
      U_ref = U_max;
   }

   double angle = inst->theta - (sector*_PIdiv3);
   double U_ref_percent = (_SQRT3)*(U_ref/_U_DC); // previous: (2/_SQRT3)

   double t_1 = U_ref_percent * sin(_PIdiv3-angle) * T_halfsample;
   double t_2 = U_ref_percent * sin(angle) * T_halfsample;
   double t_0 = T_halfsample - t_1 - t_2;
   double t_0_half = t_0/2.0;


   /* Switching counter values for Timer Interrupts */

   /* Upper switches */
   unsigned short ontime_t_0_half = (t_0_half) * inst->mcu_clk;
   unsigned short ontime_value_1 = (t_0_half + t_1) *  inst->mcu_clk;
   unsigned short ontime_value_2 = (t_0_half + t_2) *  inst->mcu_clk;
   unsigned short ontime_value_3 = (t_0_half + t_1 + t_2) *  inst->mcu_clk;

   switch (sector) {

   /*     Upper switches   */

   /* Sector 1 */
   case 0:
      inst->switchtime[0] = ontime_t_0_half;
      inst->switchtime[1] = ontime_value_1;
      inst->switchtime[2] = ontime_value_3;
      break;

   /* Sector 2 */
   case 1:
      inst->switchtime[0] = ontime_value_2;
      inst->switchtime[1] = ontime_t_0_half;
      inst->switchtime[2] = ontime_value_3;
      break;

   /* Sector 3 */
   case 2:
      inst->switchtime[0] = ontime_value_3;
      inst->switchtime[1] = ontime_t_0_half;
      inst->switchtime[2] = ontime_value_1;
      break;

   /* Sector 4 */
   case 3:
      inst->switchtime[0] = ontime_value_3;
      inst->switchtime[1] = ontime_value_2;
      inst->switchtime[2] = ontime_t_0_half;
      break;

   /* Sector 5 */
   case 4:
      inst->switchtime[0] = ontime_value_1;
      inst->switchtime[1] = ontime_value_3;
      inst->switchtime[2] = ontime_t_0_half;
      break;

   /* Sector 6 */
   case 5:
      inst->switchtime[0] = ontime_t_0_half;
      inst->switchtime[1] = ontime_value_3;
      inst->switchtime[2] = ontime_value_2;
      break;
   }
}

extern "C" __declspec(dllexport) void svpwm(struct sSVPWM **opaque, double t, union uData *data)
{
   double          Va  = data[ 0].d ; // input
   bool            clk = data[ 1].b ; // input
   double          Vb  = data[ 2].d ; // input
   const double    Per = data[ 3].d ; // input parameter
   double         &g1  = data[ 4].d ; // output
   double         &g2  = data[ 5].d ; // output
   double         &g3  = data[ 6].d ; // output
   double         &g4  = data[ 7].d ; // output
   double         &g5  = data[ 8].d ; // output
   double         &g6  = data[ 9].d ; // output
   unsigned short &cnt = data[10].us; // output
   double         &out = data[11].d ; // output

   const double mcu_clk = 170E6;
   const double peak    = 8500;

   if(!*opaque)
   {
      *opaque = (struct sSVPWM *) malloc(sizeof(struct sSVPWM));
      bzero(*opaque, sizeof(struct sSVPWM));

      struct sSVPWM *inst = *opaque;

      inst->mcu_clk = mcu_clk;
      inst->xpeak= peak;

      inst>switchtime[0] = 0;
      inst>switchtime[1] = 0;
      inst>switchtime[2] = 0;

      inst->trg_m =   inst->xpeak/mcu_clk;
      inst->trg_e = 2*inst->xpeak/mcu_clk;
      
      inst->trg_a_r = 2*inst->xpeak/mcu_clk;
      inst->trg_a_f = 2*inst->xpeak/mcu_clk;

      inst->trg_b_r = 2*inst->xpeak/mcu_clk;
      inst->trg_b_f = 2*inst->xpeak/mcu_clk;

      inst->trg_c_r = 2*inst->xpeak/mcu_clk;
      inst->trg_c_f = 2*inst->xpeak/mcu_clk;

      inst->maxstep = 1e-9;

      inst->up_counter = true;
      inst->counter = 0;
      inst->theta = 0.0;
   }
   struct sSVPWM *inst = *opaque;


// Implement module evaluation code here:

   if((inst->t_prev <= inst->trg_e)&&(t >= inst->trg_e)){
      inst->xcntr++;
      //inst->xpeak= peak;
      inst->theta +=  PI / 100.0;

      svpwm_clculate(inst, Va, Vb);

      inst->trg_m = inst->trg_e + inst->xpeak/mcu_clk;

      inst->trg_a_r = inst->trg_e + inst->switchtime[0];
      inst->trg_a_f = inst->trg_e + 2*inst->xpeak - inst->switchtime[0];

      inst->trg_b_r = inst->trg_e + inst->switchtime[1];
      inst->trg_b_f = inst->trg_e + 2*inst->xpeak - inst->switchtime[1];

      inst->trg_c_r = inst->trg_e + inst->switchtime[2];
      inst->trg_c_f = inst->trg_e + 2*inst->xpeak - inst->switchtime[2];

      inst->trg_e = inst->trg_e + 2*inst->xpeak/mcu_clk;

      inst->maxstep = peak/mcu_clk;
   }

   if((inst->t_prev <= inst->trg_m)&&(t >= inst->trg_m))
   {
      inst->xcntr++;
   }

   if(t < inst->trg_m){
      inst->counter++;

      if((inst->t_prev <= inst->trg_a_r)&&(t >= inst->trg_a_r))
      {
         inst->xcntr++;
         inst->g1 =  15.0;
         inst->g2 = -15.0;
      }
      if((inst->t_prev <= inst->trg_b_r)&&(t >= inst->trg_b_r))
      {
         inst->xcntr++;
         inst->g3 =  15.0;
         inst->g4 = -15.0;
      }
      if((inst->t_prev <= inst->trg_c_r)&&(t >= inst->trg_c_r))
      {
         inst->xcntr++;
         inst->g5 = 15.0;
         inst->g6 = -15.0;
      }
   }
   else{
      inst->counter--;

      if((inst->t_prev <= inst->trg_a_f)&&(t >= inst->trg_a_f))
      {
         inst->xcntr++;
         inst->g1 = -15.0;
         inst->g2 =  15.0;
      }
      if((inst->t_prev <= inst->trg_b_f)&&(t >= inst->trg_b_f))
      {
         inst->xcntr++;
         inst->g3 = -15.0;
         inst->g4 =  15.0;
      }
      if((inst->t_prev <= inst->trg_c_f)&&(t >= inst->trg_c_f))
      {
         inst->xcntr++;
         inst->g5 = -15.0;
         inst->g6 =  15.0;
      }
   }

   inst->t_prev = t;
   cnt = inst->counter;

   g1 = inst->g1;
   g2 = inst->g2;
   g3 = inst->g3;
   g4 = inst->g4;
   g5 = inst->g5;
   g6 = inst->g6;
}

extern "C" __declspec(dllexport) double MaxExtStepSize(struct sSVPWM *inst, double t)
{
   return inst->maxstep; // implement a good choice of max timestep size that depends on struct sSVPWM
}

extern "C" __declspec(dllexport) void Trunc(struct sSVPWM *inst, double t, union uData *data, double *timestep)
{ // limit the timestep to a tolerance if the circuit causes a change in struct sSVPWM
   const double ttol = 10e-12; // 10ps default tolerance
   if(*timestep > ttol)
   {
      struct sSVPWM tmp = *inst;
      svpwm(&(&tmp), t, data);
      if(tmp.xcntr != inst->xcntr) // implement a meaningful way to detect if the state has changed
         *timestep = ttol;      
   }
}

extern "C" __declspec(dllexport) void Destroy(struct sSVPWM *inst)
{
   free(inst);
}


