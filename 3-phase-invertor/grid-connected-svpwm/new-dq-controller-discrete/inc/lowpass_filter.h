#ifndef LOWPASS_FILTER_H
#define LOWPASS_FILTER_H

/**
 *  Low pass filter class
 */
class LowPassFilter
{
public:
    /**
     * @param Tf - Low pass filter time constant
     */
    LowPassFilter();
    void init(double tf, double ts){
        Tf = tf;
        Ts = ts;
    }

    double operator() (double x){
        // calculate the first order filer
        double alpha = Tf/(Tf + Ts);
        double y = alpha * y_prev + (1.0 - alpha) * x;

        // save the variables for the future steps
        y_prev = y;
        return y;
    }


protected:
    double Tf;
    double Ts;
    double y_prev; 
};

#endif // LOWPASS_FILTER_H