// Automatically generated C++ file on Tue Nov 26 20:20:29 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD tc_svpwm.cpp kernel32.lib

#include <cmath>

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
#undef vra
#undef g1
#undef g2
#undef g3
#undef g4
#undef g5
#undef g6
#undef vrb
#undef vrc


// Global variables
double t_control = 0.0, t_execute = 0.0;
double t_vectorx, t_vectory, t_vector0;
int sector;
int gating_vectors[8];
double gating_time_intervals[8];



extern "C" __declspec(dllexport) void tc_svpwm(void **opaque, double t, union uData *data)
{
   double  vra = data[0].d; // input
   double  vrb = data[1].d; // input
   double  vrc = data[2].d; // input
   double &g1  = data[3].d; // output
   double &g2  = data[4].d; // output
   double &g3  = data[5].d; // output
   double &g4  = data[6].d; // output
   double &g5  = data[7].d; // output
   double &g6  = data[8].d; // output

// Implement module evaluation code here:


   double vr_alpha, vr_beta;

   double Ts = 100.0e-6;

   double conv_vectors[8][2] = {
      {0.0, 0.0},
      {16.0008, 0.0},
      {-7.9992, 13.8552},
      {-7.9992, -13.8552},
      {-16.0008, 0.0},
      {7.9992, -13.8552},
      {7.9992, 13.8552},
      {0.0, 0.0}
   };

   int sector_vectors[6][2] = {
      {1, 6},
      {2, 6},
      {2, 4},
      {3, 4},
      {3, 5},
      {1, 5}
   };

   int vector_sequence[6][8] = {
      {0, 1, 6, 7, 7, 6, 1, 0},
      {0, 2, 6, 7, 7, 6, 2, 0},
      {0, 2, 4, 7, 7, 4, 2, 0},
      {0, 3, 4, 7, 7, 4, 3, 0},
      {0, 3, 5, 7, 7, 5, 3, 0},
      {0, 1, 5, 7, 7, 5, 1, 0}
   };

   int gating_logic[8][3] = {
      {0, 0 ,0},
      {1, 0, 0},
      {0, 1, 0},
      {0, 0, 1},
      {0, 1, 1},
      {1, 0, 1},
      {1, 1, 0},
      {1, 1, 1}
   };


   if (t >= t_control) {



      // Convert reference voltages to alpha-beta domain

      vr_alpha = (2.0/3.0) * ( vra - 0.5*vrb - 0.5*vrc );
      vr_beta = (2.0/3.0) * (sqrt(3)/2.0) * ( vrb - vrc );


      // Determining the position of the reference voltage vector

      if (vr_alpha > 0) {
         // V1-V6 (S1), V6-V2 (S2), V1-V5 (S6), V5-V3 (S5)

         if (vr_beta > 0) {
            // V1-V6 (S1), V6-V2 (S2)

            if (vr_beta < 1.73205 * vr_alpha) {
               // V1-V6 (S1)
               sector = 1;
            } else {
               // V6-V2 (S2)
               sector = 2;
            }

         } else {
            // V1-V5 (S6), V5-V3 (S5)

            if (vr_beta > -1.73205 * vr_alpha) {
               // V1-V5 (S6)
               sector = 6;
            } else {
               // V5-V3 (S5)
               sector = 5;
            }
         }

      } else {

         if (vr_beta > 0) {
            // V4-V2 (S3), V2-V6 (S2)

            if (vr_beta < -1.73205 * vr_alpha) {
               // V4-V2 (S3)
               sector = 3;
            } else {
               // V2-V6 (S2)
               sector = 2;
            }
         } else {
            // V4-V3 (S4), V3-V4 (S5)

            if (vr_beta > 1.73205 * vr_alpha) {
               // V4-V3 (S4)
               sector = 4;
            } else {
               // V3-V4 (S5)
               sector = 5;
            }
         }
      }

      // Calculating the time intervals of the non-zero vectors

      t_vectorx = Ts * ( vr_alpha * conv_vectors[sector_vectors[sector-1][1]][1] - vr_beta * conv_vectors[sector_vectors[sector-1][1]][0] ) / ( conv_vectors[sector_vectors[sector-1][0]][0] * conv_vectors[sector_vectors[sector-1][1]][1] - conv_vectors[sector_vectors[sector-1][0]][1] * conv_vectors[sector_vectors[sector-1][1]][0] );

      t_vectory = Ts * ( vr_beta * conv_vectors[sector_vectors[sector-1][0]][0] - vr_alpha * conv_vectors[sector_vectors[sector-1][0]][1]) / ( conv_vectors[sector_vectors[sector-1][0]][0] * conv_vectors[sector_vectors[sector-1][1]][1] - conv_vectors[sector_vectors[sector-1][0]][1] * conv_vectors[sector_vectors[sector-1][1]][0] );

      t_vector0 = Ts - (t_vectorx + t_vectory);


      // Writing the final switching logic

      // The 8 vectors that will form a switching cycle
      gating_vectors[0] = 0;
      gating_vectors[1] = sector_vectors[sector-1][0];
      gating_vectors[2] = sector_vectors[sector-1][1];
      gating_vectors[3] = 7;
      gating_vectors[4] = 7;
      gating_vectors[5] = sector_vectors[sector-1][1];
      gating_vectors[6] = sector_vectors[sector-1][0];
      gating_vectors[7] = 0;

      // Time intervals for the vectors
      gating_time_intervals[0] = t_vector0/4.0;
      gating_time_intervals[1] = t_vectorx/2.0;
      gating_time_intervals[2] = t_vectory/2.0;
      gating_time_intervals[3] = t_vector0/4.0;
      gating_time_intervals[4] = t_vector0/4.0;
      gating_time_intervals[5] = t_vectory/2.0;
      gating_time_intervals[6] = t_vectorx/2.0;
      gating_time_intervals[7] = t_vector0/4.0;



      t_control += Ts;


   }



   // Gating sequence generation

   if (t <= t_execute + gating_time_intervals[0]) {
      // Generate V0 vector
      g1 = -10.0;
      g3 = -10.0;
      g5 = -10.0;
      g2 = 15.0;
      g4 = 15.0;
      g6 = 15.0;
   } else if (t <= t_execute + gating_time_intervals[0] + gating_time_intervals[1]) {
      // Generate x vector
      if (gating_logic[gating_vectors[1]][0]) {
         g1 = 15.0;
         g2 = -10.0;
      } else {
         g1 = -10.0;
         g2 = 15.0;
      }
      if (gating_logic[gating_vectors[1]][1]) {
         g3 = 15.0;
         g4 = -10.0;
      } else {
         g3 = -10.0;
         g4 = 15.0;
      }
      if (gating_logic[gating_vectors[1]][2]) {
         g5 = 15.0;
         g6 = -10.0;
      } else {
         g5 = -10.0;
         g6 = 15.0;
      }

   } else if (t <= t_execute + gating_time_intervals[0] + gating_time_intervals[1] + gating_time_intervals[2]) {
      // Generate y vector
      if (gating_logic[gating_vectors[2]][0]) {
         g1 = 15.0;
         g2 = -10.0;
      } else {
         g1 = -10.0;
         g2 = 15.0;
      }
      if (gating_logic[gating_vectors[2]][1]) {
         g3 = 15.0;
         g4 = -10.0;
      } else {
         g3 = -10.0;
         g4 = 15.0;
      }
      if (gating_logic[gating_vectors[2]][2]) {
         g5 = 15.0;
         g6 = -10.0;
      } else {
         g5 = -10.0;
         g6 = 15.0;
      }

   } else if (t <= t_execute + gating_time_intervals[0] + gating_time_intervals[1] + gating_time_intervals[2] + gating_time_intervals[3]) {
      // Generate V7 vector
      g1 = 15.0;
      g3 = 15.0;
      g5 = 15.0;
      g2 = -10.0;
      g4 = -10.0;
      g6 = -10.0;

   } else if (t <= t_execute + gating_time_intervals[0] + gating_time_intervals[1] + gating_time_intervals[2] + gating_time_intervals[3] + gating_time_intervals[4]) {
      // Generate V7 vector
      g1 = 15.0;
      g3 = 15.0;
      g5 = 15.0;
      g2 = -10.0;
      g4 = -10.0;
      g6 = -10.0;

   } else if (t <= t_execute + gating_time_intervals[0] + gating_time_intervals[1] + gating_time_intervals[2] + gating_time_intervals[3] + gating_time_intervals[4] + gating_time_intervals[5]) {
      // Generate y vector
      if (gating_logic[gating_vectors[5]][0]) {
         g1 = 15.0;
         g2 = -10.0;
      } else {
         g1 = -10.0;
         g2 = 15.0;
      }
      if (gating_logic[gating_vectors[5]][1]) {
         g3 = 15.0;
         g4 = -10.0;
      } else {
         g3 = -10.0;
         g4 = 15.0;
      }
      if (gating_logic[gating_vectors[5]][2]) {
         g5 = 15.0;
         g6 = -10.0;
      } else {
         g5 = -10.0;
         g6 = 15.0;
      }

   }  else if (t <= t_execute + gating_time_intervals[0] + gating_time_intervals[1] + gating_time_intervals[2] + gating_time_intervals[3] + gating_time_intervals[4] + gating_time_intervals[5] + gating_time_intervals[6]) {
      // Generate x vector
      if (gating_logic[gating_vectors[6]][0]) {
         g1 = 15.0;
         g2 = -10.0;
      } else {
         g1 = -10.0;
         g2 = 15.0;
      }
      if (gating_logic[gating_vectors[6]][1]) {
         g3 = 15.0;
         g4 = -10.0;
      } else {
         g3 = -10.0;
         g4 = 15.0;
      }
      if (gating_logic[gating_vectors[6]][2]) {
         g5 = 15.0;
         g6 = -10.0;
      } else {
         g5 = -10.0;
         g6 = 15.0;
      }

   }  else if (t <= t_execute + gating_time_intervals[0] + gating_time_intervals[1] + gating_time_intervals[2] + gating_time_intervals[3] + gating_time_intervals[4] + gating_time_intervals[5] + gating_time_intervals[6] + gating_time_intervals[7]) {
      // Generate V0 vector
      g1 = -10.0;
      g3 = -10.0;
      g5 = -10.0;
      g2 = 15.0;
      g4 = 15.0;
      g6 = 15.0;
   }


   // Update t_execute

   if (t >= t_execute + Ts) {
      t_execute += Ts;
   }


}
