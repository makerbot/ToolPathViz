#ifndef PARSERS_H
#define PARSERS_H

#include "commands.h"

namespace parsers {

    //mostly for testing purposes, just looks for X, Y, & Z codes
    void simpleLocationParser(std::vector<commandLine*> &lines);
};

#endif // PARSERS_H
