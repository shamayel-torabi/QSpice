// Automatically generated C++ file on Mon Nov 25 15:02:47 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD tc_pwm.cpp kernel32.lib

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
#undef ma
#undef c
#undef g1
#undef g2
#undef g3
#undef g4
#undef g5
#undef g6
#undef mb
#undef mc

extern "C" __declspec(dllexport) void tc_pwm(void **opaque, double t, union uData *data)
{
   double  ma = data[ 0].d; // input
   double  c  = data[ 1].d; // input
   double  mb = data[ 2].d; // input
   double  mc = data[ 3].d; // input
   double &g1 = data[4].d; // output
   double &g2 = data[5].d; // output
   double &g3 = data[6].d; // output
   double &g4 = data[7].d; // output
   double &g5 = data[8].d; // output
   double &g6 = data[9].d; // output

// Implement module evaluation code here:


   if (ma > c) {
      g1 = 15.0;
      g2 = -10.0;
   } else {
      g1 = -10.0;
      g2 = 15.0;
   }

   if (mb > c) {
      g3 = 15.0;
      g4 = -10.0;
   } else {
      g3 = -10.0;
      g4 = 15.0;
   }

   if (mc > c) {
      g5 = 15.0;
      g6 = -10.0;
   } else {
      g5 = -10.0;
      g6 = 15.0;
   }

}
