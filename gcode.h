#ifndef GCODE_H
#define GCODE_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

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

// stab at representing all of the layers in an object
class layerMap {
private:
    // vector containing all of the layer heights we know about
    vector<float> heights;
public:
    // Record that we've seen a specific z height. If it's already in the list, it is ignored, otherwise it is added.
    void recordHeight(float height);

    // Get the height corresponding to a given layer
    float getLayerHeight(int layer);

    // Test if a height is in a given layer
    bool heightInLayer(int layer, float height);

    // Test if a height is higher than a given layer
    bool heightGreaterThanLayer(int layer, float height);

    // Test if a height is lower than a given later
    bool heightLessThanLayer(int layer, float height);

    // Return the number of layers that we know about
    int size();

    void clear();
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
    layerMap map;

public:
    gcodeModel();

    void loadGCode(string filename);

    float getModelZCenter();

private:
    float minZ;
    float maxZ;
};


#endif // GCODE_H
