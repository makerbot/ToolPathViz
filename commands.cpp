
#include "commands.h"
#include "parsers.h"

commandLine::commandLine(string line) {
    this->line = line;
}

void commandLine::addCommand(const command& c) {
    commands.insert(std::pair<const std::string, const command*>(typeid(c).name(), c.clone()));
}

commandLine::~commandLine() {
    std::map<const std::string, const command*>::iterator it;
    for(it = commands.begin(); it != commands.end(); it++) {
        delete (*it).second;
    }
}

gcodeCommandLine::gcodeCommandLine(string line) : commandLine(line), theCodes(line) {
}

const string gcodeCommandLine::getComment() const {
    return theCodes.getComment();
}

const bool gcodeCommandLine::hasCode(char searchCode) const {
    return theCodes.hasCode(searchCode);
}

const double gcodeCommandLine::getCodeValue(char searchCode) const {
    return theCodes.getCodeValue(searchCode);
}

//reads from a gcode file and breaks up the contents
// into individual lines
void loadGCodeFile(ifstream &file, vector<commandLine*> &lines) {

    while (file.good()) {
        string line;

        std::getline(file, line);
        lines.push_back(new gcodeCommandLine(line));
    }
}

commandModel::commandModel() {
    lines = vector<commandLine*>();
    loaded = false;
}

commandModel::~commandModel() {
    std::vector<commandLine*>::iterator it;
    for(it = lines.begin(); it != lines.end(); it++) {
        delete (*it);
    }
}

void commandModel::loadFile(QString filename) {

    ifstream file;
    file.open(filename.toStdString().c_str());

    if(filename.endsWith(".gcode", Qt::CaseInsensitive)) {
        loadGCodeFile(file, lines);
    }
    loaded = true;

}

