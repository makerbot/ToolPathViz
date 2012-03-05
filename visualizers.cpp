#include <commands.h>
#include <visualizers.h>

void visualizers::simpleVisualizer(const std::vector<commandLine*>& cmodel, visualModel& vmodel) {
    point5d current, previous;

    std::vector<commandLine*>::const_iterator line;
    for(line = cmodel.begin(); line < cmodel.end(); line++) {
        const newPoint *p = (*line)->getCommand<newPoint>();

        if(p != 0)
        {
            previous = current;
            current = p->getLocation();

            vmodel.segments.push_back(segment(previous, current));
        }
    }
}
