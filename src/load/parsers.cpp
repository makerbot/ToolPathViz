#include "parsers.h"

#include <QFile>
#include <QTextStream>

ParserFactory::ParserFactory()
{
    // v Add parsers to this list v
    m_parsers.append(new ExampleGCodeParser());
    // ^ Add parsers to this list ^
}

bool ParserFactory::loadToolpath(const QFileInfo &file,
                                 const QString parserFilter,
                                 Toolpath &path)
{
    foreach(const Parser *p, m_parsers)
    {
        if(parserFilter.compare(p->filter()) == 0)
        {
            return p->loadToolpath(file, path);
        }
    }
    qDebug() << "No parsers match filter " << parserFilter;
    return false;
}

QStringList ParserFactory::parsers()
{
    QStringList filters;
    foreach(const Parser *p, m_parsers)
    {
        filters << p->filter();
    }
    return filters;
}
