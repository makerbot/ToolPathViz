#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>
#include <map>
#include <QString>
#include <typeinfo>

#include <gcode.h>
#include <geometry.h>

//base class of objects representing one instruction in the line
class command {
public:
    //this lets us copy commands
    // should just call the copy constuctor
    // use covariant return types
    virtual const command* clone() const = 0;
};

//object that represents a single line of instruction
class commandLine {
private:
    // For now I'm assuming that there will only be
    // one of any type of command. If that's not the
    // case, switch this to a multimap and fix the
    // getCommand() function.
    std::map<const std::string, const command*> commands;
    std::string line;

public:
    commandLine(string line);

    // copies and stores the command
    void addCommand(const command& c);

    // returns a pointer to the command of the type
    // specified by C, or null if not present
    template<class C> const C* getCommand() const {
        std::map<const std::string, const command*>::const_iterator com;

        // lookup the command by type
        com = commands.lower_bound(typeid(C).name());
        if(com == commands.end())
            return 0; //null pointer

        return static_cast<const C*>((*com).second);
    };

    // get the string for this line
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
    newPoint(point5d p) : point(p) {};

    const point5d getLocation() const {
        return point;
    }

    newPoint* clone() const {
        return new newPoint(*this);
    }
};

#endif // COMMANDS_H
