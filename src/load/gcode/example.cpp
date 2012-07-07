#include "src/load/parsers.h"

#include <fstream>

#include "gcode.h"

bool ExampleGCodeParser::loadToolpath(const QFileInfo &info,
                                      Toolpath &path) const
{
    // this is mostly pulled from Matt Metts' original toolpathviz gcode parser

    if(not info.exists())
        return false;

    std::ifstream file;

    file.open(info.canonicalFilePath().toStdString().c_str());

    uint linenum = 0;
    qreal x, y, z;

    while (file.good()) {
        string line;

        std::getline(file, line);
        gcode code = gcode(line);

        if (code.hasCode('G') && (int)code.getCodeValue('G') == 1) {
            if (code.hasCode('X')) {
                x = code.getCodeValue('X');
            }
            if (code.hasCode('Y')) {
               y = code.getCodeValue('Y');
            }
            if (code.hasCode('Z')) {
                z = code.getCodeValue('Z');
            }
            path.append(new MoveAbsoluteStep(linenum, x, y, z, 0, 0));
        }

        linenum++;
    }
    return true;
}
