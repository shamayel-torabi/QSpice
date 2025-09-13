// Automatically generated C++ file on Sat Sep 13 09:30:36 2025
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD adc.cpp kernel32.lib

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

struct sADC
{
  unsigned short  adc_value;
  double          last_clk;
};

extern "C" __declspec(dllexport) void adc(struct sADC **opaque, double t, union uData *data)
{
   double          In  = data[0].d ; // input
   double          clk = data[1].d ; // input
   unsigned short &Out = data[2].us; // output

   if(!*opaque)
   {
      *opaque = (struct sADC *) malloc(sizeof(struct sADC));
      bzero(*opaque, sizeof(struct sADC));
   }
   struct sADC *inst = *opaque;

   if(In > 3.2)
      In = 3.2;
   if(In < 0.0)
      In = 0.0;

   if(clk > 0.9 && inst->last_clk < 0.1)
   {
      inst->adc_value = unsigned short(In * 4096.0 / 3.2);
   }


   inst->last_clk = clk;
   Out = inst->adc_value;
}

extern "C" __declspec(dllexport) void Destroy(struct sADC *inst)
{
   free(inst);
}
