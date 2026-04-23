//spwm.h
#ifndef SPWM_H
#define SPWM_H

#ifdef __cplusplus
extern "C" {
#endif

class SPWM {
public:
    SPWM();

    void init(double vdc, double fsw, double mcuclk){
        Vdc = vdc;
        mcu_clk = mcuclk;
        t_half = 1.0 / (2.0 * fsw);
        xpeak = mcuclk / (2 * fsw);
    }

    void operator()(double va, double vb, double vc, double theta){
        ma = va * 2 / Vdc;
        mb = vb * 2 / Vdc;
        mc = vc * 2 / Vdc;

        if(ma < 0.0)
            ma = -ma;
        if(mb < 0.0)
            mb = -mb;
        if(mc < 0.0)
            mc = -mc;

        if(ma > 1.0)
            ma = 1.0;
        if(mb > 1.0)
            mb = 1.0;
        if(mc > 1.0)
            mc = 1.0;

        switchtime_a = xpeak * (1.0 - ma);
        switchtime_b = xpeak * (1.0 - mb);
        switchtime_c = xpeak * (1.0 - mc);
    }

    double switchtime_a;
    double switchtime_b;
    double switchtime_c;

    double ma;
    double mb;
    double mc;

private:
    double Vdc;
    double t_half;
    double xpeak;
    double mcu_clk;
};


#ifdef __cplusplus
}
#endif
#endif  //SPWM_H