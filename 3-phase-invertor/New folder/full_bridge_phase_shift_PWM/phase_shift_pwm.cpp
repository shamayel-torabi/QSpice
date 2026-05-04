// Automatically generated C++ file on Sun Nov 24 20:33:23 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD phase_shift_pwm.cpp kernel32.lib

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
#undef ph
#undef g1
#undef g2
#undef g3
#undef g4


// Global variable

double carr_wave1=0.0, carr_wave2=0.0;
double prev_t = 0.0;
int pwm_cycle_start = 0;
double prev_ph = 0;


extern "C" __declspec(dllexport) void phase_shift_pwm(void **opaque, double t, union uData *data)
{
   double  ph = data[0].d; // input
   double &g1 = data[1].d; // output
   double &g2 = data[2].d; // output
   double &g3 = data[3].d; // output
   double &g4 = data[4].d; // output

// Implement module evaluation code here:

   double Ts = 100.0e-6;  // Time period of the carrier waveform
   double carr_slope;
   double ph_change;

   // Change in ph (phase shift)
   ph_change = ph - prev_ph;


   // Generate sawtooth waveforms - carr_wave1 and carr_wave2
   // 10kHz frequency - 0 and 1

   // Slope of carrier waveform = (1 - 0) / Ts

   // f(n) = f(n-1) + Slope*(tn - tn-1)

   // If previous time instant - 0.1s, next time instant = 0.1 + 1e-9 ?

   carr_wave1 += (1/Ts) * (t - prev_t);
   carr_wave2 += (1/Ts) * (t - prev_t);

   if (carr_wave1 >= 1) {
      carr_wave1 = 0.0;
   }

   if (carr_wave2 >= 1) {
      carr_wave2 = 0.0;
   }


   // Implementation of control in hardware
   // Control algorithm - phase shift angle
   // At the beginning of PWM cycle, control will initiate
   // - read all the measurements, currents, voltages
   // - fed to the ADC, will convert them to digital form for the controller
   // - receive these digital values, and will perform computations to produce the control action
   // the control action - phase shift
   // the controller will complete its tasks within a switching cycle
   // by the end of the cycle, there is a new phase shift for the next switching cycle
   // In the next switching cycle, the new phase shift is applied


   // Beginning of the PWM cycle
   // Microcontroller will have interrupts that can be configured
   // Interrupt at the beginning of every switching cycle

   if (carr_wave1 < 0.005) {
      pwm_cycle_start = 1;
   } else {
      pwm_cycle_start = 0;
   }


   if (pwm_cycle_start) {
      // Implement our controller - the steps above
      // Introduce a phase shift
      // carr_wave1 - reference waveform - we do not change it
      // carr_wave2 - will be shifted by ph (input)

      carr_wave2 += ph_change;

      prev_ph = ph;

   }


   if (0.5 > carr_wave1) {
      // Leg1
      g1 = 15.0;
      g2 = -10.0;
   } else {
      g1 = -10.0;
      g2 = 15.0;
   }

   if (0.5 > carr_wave2) {
      g3 = 15.0;
      g4 = -10.0;
   } else {
      g3 = -10.0;
      g4 = 15.0;
   }

   prev_t = t;

}
