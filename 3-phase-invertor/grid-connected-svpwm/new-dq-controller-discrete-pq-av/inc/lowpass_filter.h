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
		double alpha = Tf/(Tf + Ts);
		double y = alpha * y_prev + (1.0 - alpha) * x;
		y_prev = y;
		return y;
	}
	
    double Tf; //!< Low pass filter time constant
	double Ts;

protected:
    double y_prev; //!< filtered value in previous execution step 
};

#endif // LOWPASS_FILTER_H