#ifndef GCODE_H
#define GCODE_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <QString>

using std::string;
using std::vector;
using std::ifstream;

// Object that represents a single parsed line of GCode.
class gcode {
private:
        class gCodeParameter {
        public:
                char code;
                double value;

                gCodeParameter(char code, double value) {
                        this->code = code;
                        this->value = value;
                }
        };

        // These are the letter codes that we understand
        static char codes[];

        // The actual GCode command string
        string command;

        // Parsed out comment
        string comment;

        // The set of parameters in this GCode
        vector<gCodeParameter> parameters;

        // Find any comments, store them, then remove them from the command
        void parseComments();

        // Find any codes, and store them
        void parseCodes();

public:
        gcode(string command);

        const string getCommand() const;
        const string getComment() const;
        bool hasCode(char searchCode) const;
        double getCodeValue(char searchCode) const;
};
#endif // GCODE_H
