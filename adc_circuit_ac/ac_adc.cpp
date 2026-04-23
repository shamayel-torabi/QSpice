// Automatically generated C++ file on Mon Mar 30 15:53:25 2026
//
// To build with Digital Mars C++ Compiler: 
//
//    dmc -mn -WD ac_adc.cpp kernel32.lib

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
#undef In
#undef Out
#undef clk

struct sAC_ADC
{
  short  adc_value;
  double          last_clk;
};

extern "C" __declspec(dllexport) void ac_adc(struct sAC_ADC **opaque, double t, union uData *data)
{
   double  In   = data[0].d; // input
   double  clk  = data[1].d; // input
   double  Vref = data[2].d; // input parameter
   double &Out  = data[3].d; // output

   if(!*opaque)
   {
      *opaque = (struct sAC_ADC *) malloc(sizeof(struct sAC_ADC));
      bzero(*opaque, sizeof(struct sAC_ADC));
   }
   struct sAC_ADC *inst = *opaque;

// Implement module evaluation code here:

   if(In > Vref)
      In = Vref;
   if(In < - Vref)
      In = -Vref;

   if(clk > 0.99 && inst->last_clk < 0.01)
   {
      inst->adc_value = short(In * 4096.0 / Vref);
   }


   inst->last_clk = clk;
   Out = inst->adc_value;
}

extern "C" __declspec(dllexport) void Destroy(struct sAC_ADC *inst)
{
   free(inst);
}
