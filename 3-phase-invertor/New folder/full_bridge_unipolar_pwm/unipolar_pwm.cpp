// Automatically generated C++ file on Sun Nov 24 17:40:04 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD unipolar_pwm.cpp kernel32.lib

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
#undef m
#undef c
#undef g1
#undef g2
#undef g3
#undef g4

extern "C" __declspec(dllexport) void unipolar_pwm(void **opaque, double t, union uData *data)
{
   double  m  = data[0].d; // input
   double  c  = data[1].d; // input
   double &g1 = data[2].d; // output
   double &g2 = data[3].d; // output
   double &g3 = data[4].d; // output
   double &g4 = data[5].d; // output

   int m_sign;
   double m_abs;

// Implement module evaluation code here:

   if (m > 0) {
      m_sign = 1;
      m_abs = m;
   } else {
      m_sign = -1;
      m_abs = -m;
   }

   if (m_sign > 0) {
      // Positive half cycle
      // S1-S4 = +Vdc, S1-S3 = 0

      if (m_abs > c) {
         g1 = 15.0;
         g2 = -10.0;
         g3 = -10.0;
         g4 = 15.0;
      } else {
         g1 = 15.0;
         g2 = -10.0;
         g3 = 15.0;
         g4 = -10.0;
      }

   } else {
      // Negative half cycle
      // S2-S3 = -Vdc, S2-S4 = 0

      if (m_abs > c) {
         g1 = -10.0;
         g2 = 15.0;
         g3 = 15.0;
         g4 = -10.0;
      } else {
         g1 = -10.0;
         g2 = 15.0;
         g3 = -10.0;
         g4 = 15.0;
      }
   }

}
