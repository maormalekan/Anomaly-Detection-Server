/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Maor Malekan 321202962
 */

#include "anomaly_detection_util.h"
#include "SimpleAnomalyDetector.h"
#include "timeseries.h"
#include <math.h>
#include <vector>

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    threshold = 0.9;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {}

float SimpleAnomalyDetector:: findMaxThreshold(const vector<float>& xVec, const vector<float>& yVec, size_t size, const Line& line) {
    float max = 0;
    for (size_t i = 0; i < size; i++) {
        Point p(xVec[i], yVec[i]);
        max = fmax(max, fabs(dev(p, line)));
    }
    return max;
}

void SimpleAnomalyDetector:: buildArrayPoints(Point** points, float* x, float* y, int size) {
    for (size_t i = 0; i < size; i++) {
        points[i] = new Point(x[i], y[i]);
    }
}

void SimpleAnomalyDetector:: deletePoints(Point** points, size_t size) {
    for (size_t i = 0; i < size; i++) {
        delete(points[i]);
    }
}

void SimpleAnomalyDetector:: buildCorrelatedFeatures(const TimeSeries& ts,float maxCorrelation,
                                string f1, string f2, vector<float>& xVec, size_t valuesSize) {    
    if (maxCorrelation >= threshold) {
        vector<float> correlatedVal = ts.getValFeature(f2);
        Point* points[valuesSize];
        buildArrayPoints(points, xVec.data(), correlatedVal.data(), valuesSize);
        Line line = linear_reg(points, valuesSize);
        deletePoints(points, valuesSize);
        float cfThreshold = 1.2 * findMaxThreshold(xVec, correlatedVal, valuesSize, line);
        correlatedFeatures c = {f1, f2, maxCorrelation, line, cfThreshold};
        cf.push_back(c);
    }    
}

void SimpleAnomalyDetector:: learnNormal(const TimeSeries& ts) {
    vector<string> features = ts.getFeatures();
    size_t size = features.size();
    for (size_t i = 0; i < size - 1; i++) {
        float maxCorrelation = 0;
        int correlatedIndex = -1;
        float maxDistance = 0;
        vector<float> xVec = ts.getValFeature(features[i]);
        vector<float> yVec;
        size_t valuesSize = xVec.size();
        for (size_t j = i + 1; j < size; j++) {
            yVec = ts.getValFeature(features[j]);
            float pe = pearson(xVec.data(), yVec.data(), valuesSize);
            pe = fabs(pe);
            if (pe > maxCorrelation) {
                maxCorrelation = pe;
                correlatedIndex = j;
            }
        }
        buildCorrelatedFeatures(ts, maxCorrelation, features[i], features[correlatedIndex], xVec, valuesSize);
    }
}

float SimpleAnomalyDetector:: findDev(Point p, const correlatedFeatures& features) {
    return dev(p, features.lin_reg);
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    vector<AnomalyReport> ar;
    for(auto const& value: cf) {
        vector<float> xValues = ts.getValFeature(value.feature1);
        vector<float> yValues = ts.getValFeature(value.feature2);
        size_t vecSize = xValues.size();
        for (size_t i = 0; i < vecSize; i++) {
            Point p(xValues[i], yValues[i]);
            float d = findDev(p, value);
            if (d > value.threshold) {
                string desc = value.feature1 + "-" + value.feature2;
                AnomalyReport temp(desc, i + 1);
                ar.push_back(temp);
            }
        }
    }
    return ar;
}

