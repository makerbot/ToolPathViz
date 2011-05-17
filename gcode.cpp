#include "gcode.h"
#include <cmath>

char gcode::codes[] =  "ABDEFGHIJKLMPQRSTXYZ";


gcode::gcode(string command) :
    command(command) {

    //	cout << "parsing command: " << command << std::endl;

    // Parse (and strip) any comments out into a comment string
    parseComments();

    // Parse any codes out into the code tables
    parseCodes();
}


// Find any comments, store them, then remove them from the command
// TODO: Handle this correctly. For now, we just look for ( and bail.
void gcode::parseComments() {
    if (command.find_first_of(")") != string::npos) {
        comment = command.substr(command.find_first_of("(")+1);
        command = command.substr(0,command.find_first_of("("));
        //		cout << " comment=" << comment << std::endl;
    }

}


// Find any codes, and store them
void gcode::parseCodes() {
    // For each code letter we know about, scan for it and record it's value.
    int codeIndex = 0;

    while (codes[codeIndex]!=0) {
        // Search the command for an occurance of said code letter
        if (command.find_first_of(codes[codeIndex]) != string::npos) {
            double value = atof(command.substr(command.find_first_of(codes[codeIndex])+1).c_str());

            //			cout << " code=" << codes[codeIndex] << " value=" << value << std::endl;
            parameters.push_back(gCodeParameter(codes[codeIndex],value));
        }
        codeIndex++;
    }
}


string gcode::getCommand() {
    // TODO: Note that this is the command minus any comments.
    return string(command);
}


string gcode::getComment() {
    return string(comment);
}


bool gcode::hasCode(char searchCode) {
    for (int i = 0; i < parameters.size(); i++) {
        if(parameters[i].code == searchCode) {
            return true;
        }
    }

    return false;
}


double gcode::getCodeValue(char searchCode) {
    for (int i = 0; i < parameters.size(); i++) {
        if(parameters[i].code == searchCode) {
            return parameters[i].value;
        }
    }

    return -1;	// TODO: What do we return if there is no code?
}


bool layerMap::heightInLayer(int layer, float height) {
    return (std::fabs(heights[layer] - height) < .07);
}

bool layerMap::heightGreaterThanLayer(int layer, float height) {
    return (!heightInLayer(layer, height) && height > heights[layer]);
}

bool layerMap::heightLessThanLayer(int layer, float height) {
    return (!heightInLayer(layer, height) && height < heights[layer]);
}

// Record that we've seen a specific z height. If it's already in the list, it is ignored, otherwise it is added.
void layerMap::recordHeight(float height) {
    for (int i = 0; i < heights.size(); i++) {
        if (heightInLayer(i, height)) {
            return;
        }
    }
    heights.push_back(height);
}

// Get the height corresponding to a given layer
float layerMap::getLayerHeight(int layer) {
    return heights.at(layer);
}

// Return the number of layers that we know about
int layerMap::size() {
    return heights.size();
}

// Clear the map out.
void layerMap::clear() {
    heights.clear();
}



gcodeModel::gcodeModel() {

}

void gcodeModel::loadGCode(string filename) {
    points.clear();
    map.clear();

    ifstream file;

    // TODO: error checking!
    file.open(filename.c_str());

    while (file.good()) {
        string line;

        std::getline(file, line);
        gcode code = gcode(line);

        //		cout << " hascodeG:" << code.hasCode('G') << std::endl;

        // If the code contains a line
        if (code.hasCode('G') && (int)code.getCodeValue('G') == 1) {
            // let's add it to our list.
            points.push_back(point(code.getCodeValue('X'), code.getCodeValue('Y'),code.getCodeValue('Z')));

            // and also record its z-height so we can make a map of layers.
            map.recordHeight(code.getCodeValue('Z'));
        }
    }
}
