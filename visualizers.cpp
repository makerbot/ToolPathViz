#include <commands.h>
#include <visualizers.h>

void visualizers::simpleVisualizer(const std::vector<commandLine*>& cmodel, visualModel& vmodel) {
    point5d current, previous;
    std::cout << "simpleVisualizer" << std::endl;
    for(std::vector<commandLine*>::const_iterator line = cmodel.begin(); line < cmodel.end(); line++) {
        std::multimap<const commandType, const command*> commands = (*line)->getCommands();

        if(commands.count(NEW_POINT) > 0)
        {
            previous = current;
            current = ((newPoint*)(*commands.lower_bound(NEW_POINT)).second)->getLocation();
            current.print(std::cout);
            std::cout << std::endl;
            vmodel.segments.push_back(segment(previous, current));
        }
    }
}
