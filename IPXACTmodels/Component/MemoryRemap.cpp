//-----------------------------------------------------------------------------
// File: MemoryRemap.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 01.10.2015
//
// Description:
// Describes the ipxact:memoryRemap element.
//-----------------------------------------------------------------------------

#include "MemoryRemap.h"

//-----------------------------------------------------------------------------
// Function: MemoryRemap::MemoryRemap()
//-----------------------------------------------------------------------------
MemoryRemap::MemoryRemap(QString const& name, QString const& remapState) :
    MemoryMapBase(name),
    remapState_(remapState)
{

}

//-----------------------------------------------------------------------------
// Function: MemoryRemap::operator=()
//-----------------------------------------------------------------------------
MemoryRemap& MemoryRemap::operator=(const MemoryRemap& other)
{
    if (this != &other)
    {
        MemoryMapBase::operator=(other);
        remapState_ = other.remapState_;
    }

    return *this;
}

//-----------------------------------------------------------------------------
// Function: MemoryRemap::elementName()
//-----------------------------------------------------------------------------
QString MemoryRemap::elementName() const
{
    return QStringLiteral("memory remap");
}

//-----------------------------------------------------------------------------
// Function: MemoryRemap::getRemapState()
//-----------------------------------------------------------------------------
QString MemoryRemap::getRemapState() const
{
    return remapState_;
}

//-----------------------------------------------------------------------------
// Function: MemoryRemap::setRemapState()
//-----------------------------------------------------------------------------
void MemoryRemap::setRemapState(QString newRemapState)
{
    remapState_ = newRemapState;
}
