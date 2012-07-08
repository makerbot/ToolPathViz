# ToolpathViz

This is yet another toolpath visualizer.

The goals of this particular toolpath visualizer are:
* To be easily extensible
* To be embeddable in other Qt projects

## Requirements

Qt~4.8 (I haven't tested it with anything < 4.8.0, but it should work with at least 4.7)

## Layout

The important structural bits of ToolpathViz are laid out as follows

- /README.md				This document
- /ToolpathViewer.pro		The QMake project file for building the example
- /src/main.cpp				main will run the example toolpath viewer
- /src/example.h			some example classes showing how one might use the toolpath scene embedded in a project
- src/load/				code pertaining to loading data from one model to another. File -> toolpath is handled by Parsers, toolpath -> visual is handled by Visualizers.
- src/load/gcode			some utilities for loading gcode files
- src/model/				data models of toolpaths and visual information
- src/view/				contains the ToolpathScene - a QGraphicsScene which can be included in other Qt projects and viewed through a QGraphicsView

## Hacking

One of the goals of this project is to make it easy to get the visual you want. (once there are more parsers, and more complete parsers) You should only need to write a small amount of code mapping `Step`s in the `Toolpath` to `VisualItem`s like `Line`s, `Arrow`s, or `Triangle`s to read any supported toolpath and see what you want to see about it.
Likewise, adding support for a new kind of toolpath description file is little more than writing a function to walk the file and find discrete events, recording them as `Step`s in a `Toolpath`.

If you do add a parser or visualizer, let me know with a pull request!

## In Progress

This is a list of specific things I'd like to do that I either haven't gotten around to yet, or am unsure of and would appreciate feedback on:
* An example of a visual with floating controls associated with it. Probably the next thing I'm going to work on.
* A better way to add new `Step`s [1]
* Extensive documentation of how to create new parsers/visualizers/steps
* A proper, full, Marlin-or-other-major-reprap-style-gcode compliant gcode parser
* A proper, full, Makerbot-style-gcode compliant gcode parser (obviously not that different from above)
* More visualizers, looking at specific aspects of a toolpath [2]
* Unit tests! So people can hack on this without breaking it.
* Improvements to the opengl code to speed up rendering. I'm not doing anything to speed rendering, yet. I need to figure out how those VBOs work.
* A Makerbot s3g parser

This is in approximate order of how important I think these things are/what order I'm planning on working on them.

[1] The current method requires adding a check with a dynamic_cast to visualizers.cpp and a function on `Visualizer` to pass the converted `Step` to. 
* I've looked at using variants of some sort, but you lose the meaning of the data, and data types are limited by implementation.
* I've looked at using some template trick to automatically build a list of things that subclass `Step`, maybe like:

```c++
template<type derived>
class Step
{
// something clever
}

class MoveStep : private Step<MoveStep>
{
// something clever
}
```

but I'm not sure quite how to do it.
* I've thought about an enumeration of type IDs and a virtual function, but that means anyone creating a new visualizer has to write lots of boilerplate `dynamic_cast`s and I'd rather optimise the (hopefully) common case of adding a visualizer than the uncommon case of adding a new kind of step.
* I could use `type_info` to generate some sort of list. If the list were in step.h I'd consider that a big improvement.

[2] I may start with a feedrate visualizer and then try an intersection visualizer, which would higlight spots in the toolpath where the toolhead goes over the same point twice.