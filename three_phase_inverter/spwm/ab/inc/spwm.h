//spwm.h
#ifndef SPWM_H
#define SPWM_H

#include <math.h>

class SPWM {
public:
    SPWM();

    void init(double vdc, double xpeak){
        Vdc = vdc / 2.0;
        xPeak = xpeak;
    }

    void operator()(double valpha, double vbeta, double theta){        
        double Vref = hypotf(valpha, vbeta) / Vdc;
        //double theta = atan2(vbeta, valpha);

        if(Vref > 1.0){
            Vref = 1.0;
        }            
        
        double Valpha = Vref * sin(theta);
        double Vbeta  = Vref * cos(theta);

        ma =  Valpha;
        mb =  (sqrt(3.0) * Vbeta -  Valpha) / 2.0;
        mc = -(ma + mb);

        switchtime_a = xPeak * (1.0 + ma) / 2.0;
        switchtime_b = xPeak * (1.0 + mb) / 2.0;
        switchtime_c = xPeak * (1.0 + mc) / 2.0;
    }

    double switchtime_a;
    double switchtime_b;
    double switchtime_c;

    double ma;
    double mb;
    double mc;

private:
    double xPeak;
    double Vdc;
};

#endif  //SPWM_H