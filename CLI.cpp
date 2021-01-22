// Maor Malekan 321202962

#include "CLI.h"

using namespace std;

CLI::CLI(DefaultIO* dio): dio(dio)  {
    DetectionInfo* info = new DetectionInfo();
    numCommands = 6;
    addCommand(new UploadCommand(dio, info));
    addCommand(new AlgoSettingsCommand(dio, info));
    addCommand(new AnomalyDetectionCommand(dio, info));
    addCommand(new DisplayResultsCommand(dio, info));
    addCommand(new UploadAndAnalyzeCommand(dio, info));
    addCommand(new ExitCommand(dio, info));
}

CLI::~CLI() {
    deleteCommands();
}

void CLI::start(){
    int choose = 0;
    string stringChoose;
    do {
        dio->write("Welcome to the Anomaly Detection Server.\nPlease choose an option:\n");
        for (size_t i = 0; i < numCommands; i++) {
            string commandDesc = to_string(i+1) + "." + commands[i]->getDescription();
            dio->write(commandDesc);
        }
        stringChoose = dio->read();
        choose = stoi(stringChoose);
        if (choose >= 1 && choose <= numCommands - 1) {
            commands[choose-1]->execute();
        }
    } while (choose != 6);
}


void CLI::addCommand(Command* c) {
    commands.push_back(c);
}

void CLI:: deleteCommands() {
    for(const auto c: commands) {
        delete(c);
    }
}

