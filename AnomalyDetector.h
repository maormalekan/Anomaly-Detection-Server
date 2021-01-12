/*
 * AnomalyDetector.h
 *
 * Author: Maor Malekan 321202962
 */

#ifndef ANOMALYDETECTOR_H_
#define ANOMALYDETECTOR_H_

#include "timeseries.h"

class AnomalyReport{
public:
    const std::string description;
    const long timeStep;
    AnomalyReport(std::string description, long timeStep) :
    description(description),timeStep(timeStep){}
};

class TimeSeriesAnomalyDetector {
public:
    virtual void learnNormal(const TimeSeries& ts)=0;
    virtual std::vector<AnomalyReport> detect(const TimeSeries& ts)=0;
    virtual ~TimeSeriesAnomalyDetector(){}
};


#endif
