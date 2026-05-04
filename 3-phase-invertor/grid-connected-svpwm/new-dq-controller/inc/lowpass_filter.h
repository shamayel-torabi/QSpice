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
    void init(double time_constant){
        Tf = time_constant;
    }

    double operator() (double x, double t){
        double dt = (t - t_prev);

        // calculate the first order filer
        double alpha = Tf/(Tf + dt);
        double y = alpha * y_prev + (1.0 - alpha) * x;

        // save the variables for the future steps
        y_prev = y;
        t_prev = t;
        return y;
    }

    double Tf; //!< Low pass filter time constant

protected:
    double t_prev;  //!< Last execution timestamp
    double y_prev; //!< filtered value in previous execution step 
};

#endif // LOWPASS_FILTER_H