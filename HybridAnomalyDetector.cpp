#include "HybridAnomalyDetector.h"
#include "anomaly_detection_util.h"
#include <vector>

HybridAnomalyDetector::HybridAnomalyDetector() {}

HybridAnomalyDetector::~HybridAnomalyDetector() {}

void HybridAnomalyDetector:: buildCorrelatedFeatures(const TimeSeries& ts,float maxCorrelation, string f1, string f2, vector<float>& xVec, size_t valuesSize) {
    if (maxCorrelation < threshold && maxCorrelation > 0.5) {
        vector<float> correlatedVal = ts.getValFeature(f2);
        Point* points[valuesSize];
        buildArrayPoints(points, xVec.data(), correlatedVal.data(), valuesSize);
        Circle circle = findMinCircle(points, valuesSize);
        deletePoints(points, valuesSize);
        float cfThreshold = 1.1 * circle.radius;
        correlatedFeatures c;
        c.corrlation = maxCorrelation;
        c.feature1 = f1;
        c.feature2 = f2;
        c.threshold = cfThreshold;
        c.center = circle.center;
        cf.push_back(c);
    }
    else {
        SimpleAnomalyDetector:: buildCorrelatedFeatures(ts, maxCorrelation, f1, f2, xVec, valuesSize);
    }
}

float HybridAnomalyDetector:: findDev(Point p, const correlatedFeatures& features) {
    if (features.lin_reg.a == 0 && features.lin_reg.b == 0) {
        return p.distance(features.center);
    }
    else
        return SimpleAnomalyDetector:: findDev(p, features);
}

