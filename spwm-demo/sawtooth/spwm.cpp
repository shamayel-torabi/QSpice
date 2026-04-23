// Automatically generated C++ file on Wed Apr 15 15:15:03 2026
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD spwm.cpp kernel32.lib


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
#undef carrier
#undef m
#undef g1
#undef g2
#undef g3
#undef g4

extern "C" __declspec(dllexport) void spwm(void **opaque, double t, union uData *data)
{
   double carrier = data[0].d; // input
   double m       = data[1].d; // input
   bool  &g1      = data[2].b; // output
   bool  &g2      = data[3].b; // output
   bool  &g3      = data[4].b; // output
   bool  &g4      = data[5].b; // output

// Implement module evaluation code here:

   if(carrier < m)
   {
      g1 = false;
      g2 = true;
   }
   else
   {
      g1 = true;
      g2 = false;
   }

   if(-m < carrier){
      g3 = true;
      g4 = false;
   }
   else{
      g3 = false;
      g4 = true;
   }
}
