/*
 * anomaly_detection_util.cpp
 *
 * Author: Maor Malekan 321202962
 */

#include "anomaly_detection_util.h"

float avg(float* x, int size){
    float sum = 0;
    for (int i = 0; i < size; i++) {
        sum += x[i];
    }
    return sum / size;
}

// returns the variance of X and Y
float var(float* x, int size) {
    float average = avg(x, size);
    float temp = 0;
    for (int i = 0; i < size; i++) {
        temp += (x[i] * x[i]);
    }
    return (temp / size) - (average * average);
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    float avgX = avg(x, size);
    float avgY = avg(y, size);
    float temp = 0;
    for (int i = 0; i < size; i++) {
        temp += ((x[i] - avgX) * (y[i] -avgY));
    }
    return temp / size;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
    float devX = sqrt(var(x, size));
    float devY = sqrt(var(y, size));
    return cov(x, y, size) / (devX * devY);
}

// returns array of x values of the points

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
    float x[size];
    float y[size];
    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float a = cov(x, y, size) / var(x, size);
    float b = avg(y, size) - (a * avg(x, size));
	return Line(a, b);
}

// Line linear_reg(float* x, float* y, int size) {
//     float a = cov(x, y, size) / var(x, size);
//     float b = avg(y, size) - (a * avg(x, size));
// 	return Line(a, b);
// }


// returns the deviation between point p and the line equation of the points
float dev(Point p, Point** points, int size){
	Line l = linear_reg(points, size);
    return dev(p, l);
}

// returns the deviation between point p and the line
float dev(Point p, Line l){
	float excpectedY = l.f(p.x);
    return fabs(excpectedY - p.y);
}



