#ifndef GCODE_H
#define GCODE_H

#include <string>
#include <vector>
#include <fstream>

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

        string getCommand();
        string getComment();
        bool hasCode(char searchCode);
        double getCodeValue(char searchCode);
};

// TODO: Use whatever the equivalent class here should be.
struct point {
public:
    float x;
    float y;
    float z;

    point(float x, float y, float z) : x(x), y(y), z(z) {}
};

// Object that can open a file containing GCode and turn it into a series of lines
class gcodeModel {
public:
    // For now, we have a long list of points to string together, that is public!
    vector<point> points;

public:
    gcodeModel();

    void loadGCode(string filename);
};


#endif // GCODE_H
