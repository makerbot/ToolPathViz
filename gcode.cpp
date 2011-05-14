#include "gcode.h"

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


gcodeModel::gcodeModel() {

}

void gcodeModel::loadGCode(string filename) {
    points.clear();

    ifstream file;

    // TODO: error checking!
    file.open(filename.c_str());

    while (file.good()) {
        string line;

        std::getline(file, line);
        gcode code = gcode(line);

        //		cout << " hascodeG:" << code.hasCode('G') << std::endl;

        // If the code contains a line, let's add it to our list.
        if (code.hasCode('G') && (int)code.getCodeValue('G') == 1) {
            points.push_back(point(code.getCodeValue('X'), code.getCodeValue('Y'),code.getCodeValue('Z')));
        }
    }
}
