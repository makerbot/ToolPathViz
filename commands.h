#ifndef COMMANDS_H
#define COMMANDS_H

#include <string>
#include <vector>
#include <map>
#include <QString>
#include <typeinfo>

#include <gcode.h>
#include <geometry.h>


/*

  it would be nice if parsers didn't have to implement their own way of figuring out if
  the type of command they want is in a specific line.

can I set it up so that there's a function getX(const command&, X&)?
that way all the casting & looping would be hidden. also, no more enum.

what if I didn't even use polymorphism? maybe I'm just doing that because of Java.
Is there a good way to do this with templates?

I could have something like: const X& get<X extends commandLine>(const commandLine&);
that would encapsulate the lookup and cast

What if there are multiple of the same kind if command on a line?
void get<X>(const commandLine&, int& count, X* commands);

can I call static methods on instances?
if so, I can have my addCommand() handle (hide) the enum (do I even need the enum?
I'm sure I can drop it. Maybe I should focus on a way to do this without the enum)

also, let's assume you can only have one of any kind of command. What use case NEEDS
multiple commands of the same type in the same line? (extruder on for both heads?)


void addCommand(command* c) {
    map.insert(pair<string, command*>(typeid(*c), c);
}

const X* getCommand<X> () {
    command *c = map.get(typeid(X));
    return static_cast<X*>(c);
}

*/

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
