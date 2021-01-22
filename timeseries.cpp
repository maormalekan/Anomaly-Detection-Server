// Maor Malekan 321202962

#include "timeseries.h"

// The function returns a vector with it's features
const vector<string> TimeSeries:: getFeatures() const {
    return features;
}

// The function gets feature's name and returns it's values
const vector<float> TimeSeries:: getValFeature(string feature) const {
    return featuresMap.find(feature)->second;
}


// The function gets feature's name and returns it's values
const map<string, vector<float>> TimeSeries:: getMap() const {
    return featuresMap;
}

void TimeSeries:: initFeatures(string line) {            
    stringstream ss(line);
    string feature;
    size_t i = 0;
    while (getline(ss, feature, ',')) {
        features.push_back(feature);
        vector<float> v;
        featuresMap.insert({feature, v});
    }            
}

void TimeSeries:: addData(string values) {
    stringstream ss(values);
    string value;
    size_t i = 0;
    while (getline(ss, value, ',')) {
        featuresMap[features[i]].push_back(stof(value));
        i++;
    }
    dataSize++;
}

const size_t TimeSeries:: getDataSize() const {
    return dataSize;
}
