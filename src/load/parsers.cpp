#include "parsers.h"

#include <QFile>
#include <QTextStream>

ParserFactory::ParserFactory()
{
    // v Add parsers to this list v
    parsers.append(new ExampleGCodeParser());
    // ^ Add parsers to this list ^
}

bool ParserFactory::loadToolpath(const QFileInfo &file,
                                 const QString parserFilter,
                                 Toolpath &path)
{
    foreach(const Parser *p, parsers)
    {
        if(parserFilter.compare(p->filter()) == 0)
        {
            return p->loadToolpath(file, path);
        }
    }
    qDebug() << "No parsers match filter " << parserFilter;
    return false;
}

QString ParserFactory::openDialogFilter()
{
    QString filters("");
    foreach(const Parser *p, parsers)
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

    return false;
}
