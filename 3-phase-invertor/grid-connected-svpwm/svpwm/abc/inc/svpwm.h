//svpwm.h
#ifndef SpaceVectorPWM_H
#define SpaceVectorPWM_H

#include <math.h>
#define PI 3.1415926535897932384626

class SVPWM {
public:
    SVPWM();

    void init(double vdc, double x_peak, double mcuClk){
        SQRT3 = sqrt(3.0);
        PI_DIV_3 = PI / 3.0;
        Vdc = vdc;
        xpeak = x_peak;
        mcu_clk = mcuClk;
    }

    void operator()(double Va, double Vb, double Vc, double theta){
        double Valpha = 2.0 * (Va - 0.5 * (Vb + Vc)) / 3.0;
        double Vbeta  = sqrt(3.0) * (Vc - Vb) / 3.0;

        calculate_time(Valpha, Vbeta, theta);
    }

    double switchtime_a;
    double switchtime_b;
    double switchtime_c;

private:

    void calculate_time(double Valpha, double Vbeta, double theta){
        double  U_max = Vdc / SQRT3;
        double t_half = xpeak / mcu_clk;

        uint8_t sector = theta / PI_DIV_3;
        double U_ref = hypotf(Valpha, Vbeta);

        if (U_ref > U_max) {
            U_ref = U_max;
        }

        double angle = theta - (sector * PI_DIV_3);
        double U_ref_percent = SQRT3 * (U_ref / Vdc); // previous: (2/_SQRT3)

        double t_1 = U_ref_percent * sin(PI_DIV_3 - angle) * t_half;
        double t_2 = U_ref_percent * sin(angle) * t_half;
        double t_0 = t_half - t_1 - t_2;
        double t_0_half = t_0/2.0;


        /* Switching counter values for Timer Interrupts */

        /* Upper switches */
        uint16_t ontime_t_0_half = (t_0_half) * mcu_clk ;
        uint16_t ontime_value_1 = (t_0_half + t_1) * mcu_clk;
        uint16_t ontime_value_2 = (t_0_half + t_2) * mcu_clk;
        uint16_t ontime_value_3 = (t_0_half + t_1 + t_2) * mcu_clk;


        switch (sector) {

            /*     Upper switches   */

            /* Sector 1 */
            case 0:
                switchtime_a = ontime_t_0_half;
                switchtime_b = ontime_value_1;
                switchtime_c = ontime_value_3;
                break;

            /* Sector 2 */
            case 1:
                switchtime_a = ontime_value_2;
                switchtime_b = ontime_t_0_half;
                switchtime_c = ontime_value_3;
                break;

            /* Sector 3 */
            case 2:
                switchtime_a = ontime_value_3;
                switchtime_b = ontime_t_0_half;
                switchtime_c = ontime_value_1;
                break;

            /* Sector 4 */
            case 3:
                switchtime_a = ontime_value_3;
                switchtime_b = ontime_value_2;
                switchtime_c = ontime_t_0_half;
                break;

            /* Sector 5 */
            case 4:
                switchtime_a = ontime_value_1;
                switchtime_b = ontime_value_3;
                switchtime_c = ontime_t_0_half;
                break;

            /* Sector 6 */
            case 5:
                switchtime_a = ontime_t_0_half;
                switchtime_b = ontime_value_3;
                switchtime_c = ontime_value_2;
                break;
        }
    }
    
    double Vdc;
    double xpeak;
    double mcu_clk;
    double SQRT3;
    double PI_DIV_3;
};


#endif  //SpaceVectorPWM_H