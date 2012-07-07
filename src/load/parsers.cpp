#include "parsers.h"

#include <QFile>
#include <QTextStream>

ParserFactory::ParserFactory()
{
    // \/ Add parsers to this list \/
    parsers.append(new ExampleGCodeParser());
    // /\ Add parsers to this list /\
}

static bool ParserFactory::loadToolpath(const QFileInfo &file,
                                 const QString parserFilter,
                                 Toolpath &path)
{
    foreach(Parser* p, parsers)
    {
        if(parserFilter.compare(p->filter()) == 0)
        {
            return p->loadToolpath(file, path);
        }
    }
}

static QString ParserFactory::openDialogFilter()
{
    QString filters("");
    foreach(Parser* p, parsers)
    {
        filters.append(p->filter());
        filters.append(";;");
    }
    return filters;
}

bool ExampleGCodeParser::loadToolpath(const QFileInfo &info,
                                      Toolpath &path) const
{
    QFile file(info.canonicalFilePath());
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);

//    while(not in.atEnd())
//    {
//        QTextStream line(in.readLine());
//        QString code
//    }

}
