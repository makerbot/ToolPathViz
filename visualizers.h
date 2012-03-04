#ifndef VISUALIZERS_H
#define VISUALIZERS_H

#include <vector>

#include <commands.h>
#include <geometry.h>

struct visualModel {
public:
    std::vector<segment> segments;

};

// visualizers take a command model and translate it into a visual model
// the visual model is then ready to be rendered
namespace visualizers {

    // an example visualizer
    void simpleVisualizer(const std::vector<commandLine*>& cmodel, visualModel& vmodel);

}

#endif // VISUALIZERS_H
