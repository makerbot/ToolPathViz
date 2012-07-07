#ifndef PARSERS_H
#define PARSERS_H

#include <QFileInfo>
#include <QList>
#include <QString>

#include "src/model/toolpath.h"

struct ParserFactory
{
    /** This will load a file into the passed Toolpath.
        Returns false if the file cannot be read.
        \param file The file to be loaded
        \param path The Toolpath to load the file into */
    bool loadToolpath(const QFileInfo& file,
                      const QString parserFilter,
                      Toolpath& path);

    /** Returns a QString filter displaying all the supported
        Toolpath file formats. Intended for use with QFileDialog. */
    QString openDialogFilter();

    ParserFactory();

private:
    QList<Parser const *const> parsers;
}

/*!
  The base class for all file Parsers: takes a file and produces a Toolpath.

  To create a new Parser, simply override the loadToolpathInternal(),
  extension(), and filter(), then add your parser to the list in parsers.cpp
  */
struct Parser
{
    /** This is the method that must be implemented to load a file.
        Returns false if the file cannot be read.
        \param file The file to be loaded
        \param path The Toolpath to load the file into */
    virtual bool loadToolpath(const QFileInfo& file, Toolpath& path) const =0;

    /** Returns the type of file this parser can parse */
    const QString extension() const =0;
    /** Returns a QString filter displaying the extension this parser can parse.
        Intended for use with QFileDialog. */
    const QString filter() const =0;
};

struct ExampleGCodeParser : public Parser
{
    bool loadToolpath(const QFileInfo &file, Toolpath &path) const;

    const QString extension() const { "gcode"; }
    const QString filter() const { "Example GCode Parser ("+extension+")"; }
};

#endif // PARSERS_H
