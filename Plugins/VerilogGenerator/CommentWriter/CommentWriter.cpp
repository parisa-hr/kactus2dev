//-----------------------------------------------------------------------------
// File: CommentWriter.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 08.08.2014
//
// Description:
// Class for writing C/Verilog comments.
//-----------------------------------------------------------------------------

#include "CommentWriter.h"

#include <QStringList>

//-----------------------------------------------------------------------------
// Function: CommentWriter::CommentWriter()
//-----------------------------------------------------------------------------
CommentWriter::CommentWriter(QString const& comment): comment_(comment), 
    lineLength_(DEFAULT_LINE_LENGHT), 
    indentSize_(0)
{

}

//-----------------------------------------------------------------------------
// Function: CommentWriter::~CommentWriter()
//-----------------------------------------------------------------------------
CommentWriter::~CommentWriter()
{

}

//-----------------------------------------------------------------------------
// Function: CommentWriter::write()
//-----------------------------------------------------------------------------
void CommentWriter::write(QTextStream& output) const
{
    if (nothingToWrite())
    {
        return;
    }

    QString indent = QString(" ").repeated(indentSize_);

    for (QString line : comment_.split("\n"))
    {
        while (line.length() > 0)
        {
            auto lineEnd = line.length();

            if (line.length() > DEFAULT_LINE_LENGHT && line.lastIndexOf(" ", DEFAULT_LINE_LENGHT) != -1)
            {
                lineEnd = line.lastIndexOf(" ", DEFAULT_LINE_LENGHT);
            }
            else if (line.length() > DEFAULT_LINE_LENGHT && line.indexOf(" ", DEFAULT_LINE_LENGHT) != -1)
            {
                lineEnd = line.indexOf(" ", DEFAULT_LINE_LENGHT);
            }
            
            output << indent << "// " << line.left(lineEnd) << Qt::endl;
            line = line.remove(0, lineEnd + 1);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: CommentWriter::nothingToWrite()
//-----------------------------------------------------------------------------
bool CommentWriter::nothingToWrite() const
{
    return comment_.isEmpty();
}

//-----------------------------------------------------------------------------
// Function: CommentWriter::setLineLength()
//-----------------------------------------------------------------------------
void CommentWriter::setLineLength(int maxLength)
{
    lineLength_ = maxLength;        
}

//-----------------------------------------------------------------------------
// Function: CommentWriter::setIndent()
//-----------------------------------------------------------------------------
void CommentWriter::setIndent(int size)
{
    indentSize_ = size;
}
