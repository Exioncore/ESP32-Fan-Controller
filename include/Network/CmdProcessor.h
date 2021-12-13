#ifndef CmdProcessor_h
#define CmdProcessor_h

#include <Arduino.h>
#include "Util/StringProcessor.h"

/**
 * Singleton Class
 * This class is used to process commands
 */

class CmdProcessor {
    public:
        static CmdProcessor* getInstance();

        char* process(char* cmd);

    private:
        // Store the instance
        static CmdProcessor* instance;
        // Constructor
        CmdProcessor();

        StringProcessor stringProcessor;

        void processFan(char* cmd, char* response, char* buff, StringProcessor stringProcessor);
        void processSensor(char* cmd, char* response, char* buff, StringProcessor stringProcessor);
};

#endif