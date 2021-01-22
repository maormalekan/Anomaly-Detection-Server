// Maor Malekan 321202962

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <sstream>
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

class SocketIO:public DefaultIO{
	int clientID;
public:
	SocketIO(int clientID): clientID(clientID) {}

	virtual string read(){
		string clientInput="";
		char c=0;
		::read(clientID,&c,sizeof(char));
		while(c!='\n'){				
			clientInput+=c;
			::read(clientID,&c,sizeof(char));
		}
		return clientInput;
	}

	virtual void read(float* f){
		::read(clientID, f, sizeof(float));
	}

	virtual void write(string text){
		::send(clientID, text.data(), text.length(), 0);
	}

	virtual void write(float f){
		stringstream ss;
		ss << f;
		string newf = ss.str();
		::send(clientID,newf.data() ,newf.size(), 0);
	}

	~SocketIO(){
		close(clientID);
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
		float threshold = info->getDetector()->getCorrelationThreshold();
		threshold = floor(10 * threshold) / 10;
		ostringstream ostr;
		ostr << "The current correlation threshold is " << threshold;
		dio->write(ostr.str() + '\n');
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
			dio->write(to_string(ar.timeStep) + "\t " + ar.description + "\n");
		}
		dio->write("Done.\n");
	}
};

class UploadAndAnalyzeCommand: public Command {

	vector<reportWindow> buildReportWindows (vector<AnomalyReport> vecAr) {
		vector<reportWindow> windows;
		long firstTS, currentTs;
		string description;
		bool first = true;
		for(auto const& ar: info->getReport()) {
			if (first) {
				firstTS = ar.timeStep;
				currentTs = firstTS+1;
				description = ar.description;
				first = false;
			}
			else {
				if (ar.timeStep != currentTs || ar.description.compare(description) != 0) {
					reportWindow r = {description, firstTS, currentTs - 1};
					windows.push_back(r);
					firstTS = ar.timeStep;
					currentTs = firstTS + 1;
					description = ar.description;				}
				else {
					currentTs++;
				}	
			}
		}
		if (!info->getReport().empty()){
			reportWindow r = {description, firstTS, info->getReport().back().timeStep};
			windows.push_back(r);
		}
		return windows;
	}

	bool isExceptionInWindows(long firstLine, long lastLine, vector<reportWindow> windows) {
		for (auto const& w: windows) {
			if ((firstLine >= w.firstTimeStep && firstLine <= w.lastTimeStep) || (lastLine >= w.firstTimeStep && lastLine <= w.lastTimeStep) || 
				(firstLine <= w.firstTimeStep && lastLine >= w.lastTimeStep)) {
				return true;
			}
		}
		return false;
	}

public:
	UploadAndAnalyzeCommand(DefaultIO* dio, DetectionInfo* info): Command(dio, info) {
		description = "upload anomalies and analyze results\n";
	}
	virtual void execute() {
		vector<reportWindow> windows = buildReportWindows(info->getReport());
		dio->write("Please upload your local anomalies file.\n");
		string line = dio->read();
		size_t p = 0;
		float tp = 0, fp = 0;
		long n = info->getTestTs()->getDataSize();
		while (line.compare("done") != 0) {
			stringstream ss (line);
			string firstLine;
			getline(ss, firstLine, ',');
			string lastLine;
			getline(ss, lastLine, ',');
			p++;
			long first = stol(firstLine);
			long last = stol(lastLine);
			n -= (last - first + 1);
			if (isExceptionInWindows(first, last, windows)) {
				tp++;
			}
			else {
				fp++;
			}

			line = dio->read();
		}
		fp = (windows.size() - tp);
		dio->write("Upload complete.\n");
		ostringstream ostr1 , ostr2;
		ostr1 << "True Positive Rate: " << floor(1000 * (tp / p)) / 1000;
		ostr2 << "False Positive Rate: " << floor(1000 * (fp / n)) / 1000;
		dio->write(ostr1.str() +  '\n');
		dio->write(ostr2.str() +  '\n');
		// dio->write("False Positive Rate: " + to_string(floor(1000 * (fp / n)) / 1000) + '\n');
	}
};

class ExitCommand: public Command {
public:
	ExitCommand(DefaultIO* dio, DetectionInfo* info): Command(dio, info) {
		description = "exit\n";
	}
	virtual void execute() {
		// delete(dio);
	}
};

// implement here your command classes



#endif /* COMMANDS_H_ */
