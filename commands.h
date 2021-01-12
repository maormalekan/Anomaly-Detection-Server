#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{

public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}

	virtual void uploadFile(string nameFile) {
		ofstream file;
		file.open(nameFile);
		string line = this->read();
		while (line.compare("done") != 0) {
			file << line << endl;
			line = this->read();
		}
		file.close();
	}
};

class DetectionInfo {
private:
	HybridAnomalyDetector* detector;
	TimeSeries* trainTs;
	TimeSeries* testTs;
	vector<AnomalyReport> report;

public:
	DetectionInfo() {
		detector = new HybridAnomalyDetector();
	}
	~DetectionInfo() {
		delete(detector);
	}

	HybridAnomalyDetector* getDetector() {
		return detector;
	}

	void setTrainTs(TimeSeries* ts) {
		trainTs = ts;
	}

	void setTestTs(TimeSeries* ts) {
		testTs = ts;
	}

	const TimeSeries* getTrainTs() const {
		return trainTs;
	}

	const TimeSeries* getTestTs() const {
		return testTs;
	}

	const vector<AnomalyReport> getReport() const {
		return report;
	}

	void setReport(vector<AnomalyReport> vecAr) {
    	for(auto const& ar: vecAr) {
			report.push_back(ar);
		}
	}
};

class Command{
protected:
	DefaultIO* dio;
	string description;
	DetectionInfo* info;

public:
	Command(DefaultIO* dio, DetectionInfo* info):dio(dio), info(info){}
	virtual void execute()=0;
	virtual ~Command(){}
	virtual const string getDescription() const {
		return description;
	}

};

class UploadCommand: public Command {
public:
	UploadCommand(DefaultIO* dio, DetectionInfo* info): Command(dio, info) {
		description = "upload a time series csv file\n";
	}
	~UploadCommand(){}

	virtual void execute() {
		dio->write("Please upload your local train CSV file.\n");
		dio->uploadFile("anomalyTrain.csv");
		dio->write("Upload complete.\n");
		dio->write("Please upload your local test CSV file.\n");
		dio->uploadFile("anomalyTest.csv");
		dio->write("Upload complete.\n");
	}
};

class AlgoSettingsCommand: public Command {
public:
	AlgoSettingsCommand(DefaultIO* dio, DetectionInfo* info): Command(dio, info) {
		description = "algorithm settings\n";
	}
	virtual void execute() {
		dio->write("The current correlation threshold is " + to_string(info->getDetector()->getCorrelationThreshold()) + '\n');
		bool isValid = false;
		do {
			float newThreshold = 0;
			dio->write("Type a new threshold\n");
			dio->read(&newThreshold);
			isValid = newThreshold >= 0 && newThreshold <= 1;
			if (isValid) {
				info->getDetector()->setCorrelationThreshold(newThreshold);
			}
			else {
				dio->write("please choose a value between 0 and 1.\n");
				dio->read();
			}
		} while (!isValid);
	}
};

class AnomalyDetectionCommand: public Command {
public:
	AnomalyDetectionCommand(DefaultIO* dio, DetectionInfo* info): Command(dio, info) {
		description = "detect anomalies\n";
	}
	virtual void execute() {
		info->setTrainTs(new TimeSeries("anomalyTrain.csv"));
		info->setTestTs(new TimeSeries("anomalyTest.csv"));
		info->getDetector()->learnNormal(*(info->getTrainTs()));
		info->setReport(info->getDetector()->detect(*(info->getTestTs())));
		dio->write("anomaly detection complete.\n");
	}
};

class DisplayResultsCommand: public Command {
public:
	DisplayResultsCommand(DefaultIO* dio, DetectionInfo* info): Command(dio, info) {
		description = "display results\n";
	}
	virtual void execute() {
		for(auto const& ar: info->getReport()) {
			dio->write(to_string(ar.timeStep) + "\t" + ar.description + "\n");
		}
		dio->write("Done.\n");
	}
};

class UploadAndAnalyzeCommand: public Command {
public:
	UploadAndAnalyzeCommand(DefaultIO* dio, DetectionInfo* info): Command(dio, info) {
		description = "upload anomalies and analyze results\n";
	}
	virtual void execute() {
		dio->write("Please upload your local anomalies file.\n");
		string line = dio->read();
		while (line.compare("done") != 0) {
			stringstream ss (line);
			string firstLine;
			getline(ss, firstLine, ',');
			string lastLine;
			getline(ss, lastLine, ',');
			
			line = dio->read();
		}
	}
};

class ExitCommand: public Command {
public:
	ExitCommand(DefaultIO* dio, DetectionInfo* info): Command(dio, info) {
		description = "exit\n";
	}
	virtual void execute() {
	}
};

// implement here your command classes



#endif /* COMMANDS_H_ */
