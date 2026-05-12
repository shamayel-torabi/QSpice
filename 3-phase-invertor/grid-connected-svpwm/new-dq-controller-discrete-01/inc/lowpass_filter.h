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
		double tstf = ts/tf;
		b0 = (2.0 - tstf)/(2.0 + tstf);
		a0 = tstf/(2.0 + tstf);
	}

    double operator() (double x){
		double y = b0 * y_prev + a0 * (x + x_prev);
		y_prev = y;
		x_prev = x;
		return y;
	}
	
protected:
	double a0;
	double b0;
    double y_prev;
	double x_prev;
};

#endif // LOWPASS_FILTER_H