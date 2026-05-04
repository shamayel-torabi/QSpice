#ifndef LOWPASS_FILTER_H
#define LOWPASS_FILTER_H

class LowPassFilter
{
public:
    LowPassFilter();

    void init(double tf){
        Tf = tf;
    }

    double operator() (double x, double t)
    {
        double dt = (t - t_prev);
        
        double alpha = Tf / (Tf + dt);
        double y = alpha * y_prev + (1.0 - alpha) * x;

        t_prev = t;
        y_prev = y;
        return y;
    }

protected:
    double Tf; //!< Low pass filter time constant
    double t_prev;  //!< Last execution timestamp
    double y_prev;  //!< filtered value in previous execution step 
};

#endif // LOWPASS_FILTER_H