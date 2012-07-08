#ifndef PARSERS_H
#define PARSERS_H

#include <QFileInfo>
#include <QList>
#include <QString>

#include "src/model/toolpath.h"

/*!
  The base class for all file Parsers: takes a file and produces a Toolpath.

  To create a new Parser, simply subclass and reimplement
  loadToolpathInternal(), extension(), and name().

  To display your parser in the example (src/main.cpp),
  add your parser to the map in src/example.h
  */
struct Parser
{
    /** This is the method that must be implemented to load a file.
        Returns false if the file cannot be read.
        \param file The file to be loaded
        \param path The Toolpath to load the file into */
    virtual bool loadToolpath(const QFileInfo& file, Toolpath& path) const =0;

    /** Returns the type of file this parser can parse */
    virtual const QString extension() const =0;
    /** Returns the name of this parser, for display purposes. */
    virtual const QString name() const =0;
};

/*!
  A basic gcode parser, an example of how to implement a new parser.
  */
struct ExampleGCodeParser : public Parser
{
    bool loadToolpath(const QFileInfo &file, Toolpath &path) const;

    const QString extension() const {
        return "gcode";
    }
    const QString name() const {
        return "Example GCode Parser";
    }
};

/*!
  A more complex gcode parser than ExampleGCodeParser.
  */
struct FullExampleGCodeParser : public Parser
{
    bool loadToolpath(const QFileInfo &file, Toolpath &path) const;

    const QString extension() const {
        return "gcode";
    }
    const QString name() const {
        return "Full Example GCode Parser";
    }
};


#endif // PARSERS_H
