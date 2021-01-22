/*
 * timeseries.h
 *
 * Author: Maor Malekan 321202962
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;


class TimeSeries{

    vector<string> features;
    map<string, vector<float>> featuresMap;
    size_t dataSize;
    
    public:
        TimeSeries() {}

        TimeSeries(string csv){
            ifstream my_file(csv);
            string line;
            bool first_line = true;
            if (my_file.is_open()) {
                while (getline (my_file, line)) {
                    if (!first_line) {
                        addData(line);
                    }
                    else {
                        initFeatures(line);
                    }
                    first_line = false;
                }
                my_file.close();
            }
        }

        void initFeatures(string line);

        void addData(string values);

        // The function returns a vector with it's features
        const vector<string> getFeatures() const;

        // The function gets feature's name and returns it's values
        const vector<float> getValFeature(string feature) const;

        // The function gets feature's name and returns it's values
        const map<string, vector<float>> getMap() const;

        // The function gets returns the size of data.
        const size_t getDataSize() const;
};

#endif /* TIMESERIES_H_ */

