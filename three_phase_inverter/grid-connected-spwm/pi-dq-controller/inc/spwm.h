//spwm.h
#ifndef SPWM_H
#define SPWM_H

#include <math.h>

class SPWM {
public:
    SPWM();

    void init(double vdc, double xp){
        xpeak = xp;
        Vdc = vdc;
    }

    void operator()(double valpha, double vbeta, double theta){        
        double Vref = hypotf(valpha, vbeta) / Vdc;
        //double theta = atan2(vbeta, valpha);

        if(Vref > 1.0){
            Vref = 1.0;
        }            
        
        double Valpha = Vref * sin(theta);
        double Vbeta  = Vref * cos(theta);

        ma =  valpha;
        mb =  (sqrt(3.0) * vbeta -  valpha) / 2.0;
        mc = -(ma + mb);

        switchtime_a = xpeak * (1.0 - ma) / 2.0;
        switchtime_b = xpeak * (1.0 - mb) / 2.0;
        switchtime_c = xpeak * (1.0 - mc) / 2.0;
    }

    double switchtime_a;
    double switchtime_b;
    double switchtime_c;

    double ma;
    double mb;
    double mc;

private:
    double xpeak;
    double Vdc;
};

#endif  //SPWM_H