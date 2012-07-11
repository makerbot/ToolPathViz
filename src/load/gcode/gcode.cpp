#include "gcode.h"
#include <cmath>
#include <float.h>
#include <iostream>
#include <QFile>
#include <QTextStream>

char gcode::codes[] =  "ABDEFGHIJKLMPQRSTXYZ";


gcode::gcode(string command) :
    command(command) {

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
    }
}


// Find any codes, and store them
// TODO: write this correctly, handle upper/lower, spacing between code and numbers, error reporting, etc.
void gcode::parseCodes() {
    // For each code letter we know about, scan for it and record it's value.
    int codeIndex = 0;

    while (codes[codeIndex]!=0) {
        // Search the command for an occurance of said code letter
        if (command.find_first_of(codes[codeIndex]) != string::npos) {
            double value = atof(command.substr(command.find_first_of(codes[codeIndex])+1).c_str());

            parameters.push_back(gCodeParameter(codes[codeIndex],value));

        }
        codeIndex++;
    }
}


const string gcode::getCommand() const {
    // TODO: Note that this is the command minus any comments.
    return string(command);
}


const string gcode::getComment() const {
    return string(comment);
}

bool gcode::hasCode(char searchCode) const {

    for (unsigned int i = 0; i < parameters.size(); i++) {

        if(parameters[i].code == searchCode) {
            return true;
        }
    }
    return false;
}


double gcode::getCodeValue(char searchCode) const {
    for (unsigned int i = 0; i < parameters.size(); i++) {
        if(parameters[i].code == searchCode) {
            return parameters[i].value;
        }
    }

    return -1;	// TODO: What do we return if there is no code?
}
