// Maor Malekan 321202962

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
 	vector<Command*> commands;
	size_t numCommands;

	virtual void deleteCommands();
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();

	void addCommand(Command* command);
};

#endif /* CLI_H_ */
