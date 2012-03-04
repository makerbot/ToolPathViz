#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>
#include <map>
#include <QString>

#include <gcode.h>
#include <geometry.h>

// To add a new type of command:
// 1) add it to this enumeration
// 2) create a subclass of command at the bottom of this file
//          that returns that commandType from its getType()
enum commandType {
    NEW_POINT
};

//base class of objects representing one instruction in the line
class command {
public:
    // this value allows us to know what to cast as
    // i.e. what values we can get from the subclass
    virtual const commandType getType() const = 0;

    //this lets us copy commands
    // should just call the copy constuctor
    // use covariant return types
    virtual const command* clone() const = 0;
};

//object that represents a single line of instruction
class commandLine {
private:
    std::multimap<const commandType, const command*> commands;
    std::string line;

public:
    commandLine(string line);
    void addCommand(const command& c);
    std::multimap<const commandType, const command*> getCommands() const;

    const std::string& getLine() const {
        return line;
    }

    virtual ~commandLine();
};

class gcodeCommandLine : public commandLine {
private:
    gcode theCodes;
public:

    gcodeCommandLine(string line);

    const string getComment() const;
    const bool hasCode(char searchCode) const;
    const double getCodeValue(char searchCode) const;
};

// This holds a model of the set of commands in a file
// vizualizers should build their visuals from one of these
class commandModel {
private:
public:
    vector<commandLine*> lines;
    commandModel();
    ~commandModel();
    bool loaded;
    // Figures out the type of file, loads it into 'lines'
    // and runs appropriate parsers on it.
    void loadFile(QString filename);
};

////////////////////////////////////////////////////////////
// the following are all the commands that parsers can create
//

class newPoint : public command {
private:
    const point5d point;

public:
    const commandType getType() const {
        return NEW_POINT;
    }

    newPoint(point5d p) : point(p) {};

    const point5d getLocation() const {
        return point;
    }

    newPoint* clone() const {
        return new newPoint(*this);
    }
};

#endif // COMMANDS_H
