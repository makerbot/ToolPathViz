#include <iostream>
#include <parsers.h>
#include <geometry.h>

void parsers::simpleLocationParser(std::vector<commandLine*> &lines) {

    point5d currentPoint;

    for(std::vector<commandLine*>::iterator it = lines.begin(); it < lines.end(); it++) {

        // we assume that the programmer has made sure to pass us gcode commands
        gcodeCommandLine *line = ((gcodeCommandLine*)(*it));

        std::string c = line->getLine();
        std::cout << c << " changes ";
        if(line->hasCode('X')) {
            currentPoint.x = line->getCodeValue('X');
            std::cout << "x ";
        }
        if(line->hasCode('Y')) {
            currentPoint.y = line->getCodeValue('Y');
            std::cout << "y ";
        }
        if(line->hasCode('Z')) {
            currentPoint.z = line->getCodeValue('Z');
            std::cout << "z";
        }
        std::cout << std::endl;

        line->addCommand(newPoint(currentPoint));
    }
}
