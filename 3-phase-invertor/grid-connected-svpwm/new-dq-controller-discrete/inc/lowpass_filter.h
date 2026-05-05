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
        b0 = -(tf * ts - 2.0) / (tf * ts + 2.0);
        a0 = tf * ts / (tf * ts + 2.0);
    }

    double operator() (double x){
        // calculate the first order filer
        double y = b0 * y_prev + a0 * (x + x_prev);

        // save the variables for the future steps
        y_prev = y;
        x_prev = x;
        return y;
    }


protected:
    double b0;
    double a0;
    double y_prev;
    double x_prev;
};

#endif // LOWPASS_FILTER_H