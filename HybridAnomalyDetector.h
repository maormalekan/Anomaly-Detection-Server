#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {

public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector();
    
protected:
	virtual void buildCorrelatedFeatures(const TimeSeries& ts,float maxCorrelation, string f1, string f2, vector<float>& xVec, size_t valuesSize) override;
	virtual float findDev(Point p, const correlatedFeatures& features) override;

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
