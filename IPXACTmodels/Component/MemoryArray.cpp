//-----------------------------------------------------------------------------
// File: MemoryArray.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Anton Hagqvist
// Date: 21.7.2023
//
// Description:
// Describes the ipxact:array element found in array block, register, and field.
//-----------------------------------------------------------------------------

#include "MemoryArray.h"

//-----------------------------------------------------------------------------
// Function: MemoryArray::MemoryArray()
//-----------------------------------------------------------------------------
MemoryArray::MemoryArray()
{

}

//-----------------------------------------------------------------------------
// Function: MemoryArray::MemoryArray()
//-----------------------------------------------------------------------------
MemoryArray::MemoryArray(MemoryArray const& other):
    stride_(other.stride_)
{
    Utilities::copyList(dimensions_, other.dimensions_);
}

//-----------------------------------------------------------------------------
// Function: MemoryArray::operator=()
//-----------------------------------------------------------------------------
MemoryArray& MemoryArray::operator=(MemoryArray const& other)
{
    if (this != &other)
    {
        stride_ = other.stride_;
        Utilities::copyList(dimensions_, other.dimensions_);
    }

    return *this;
}

//-----------------------------------------------------------------------------
// Function: MemoryArray::getDimensions()
//-----------------------------------------------------------------------------
QSharedPointer<QList<QSharedPointer<MemoryArray::Dimension> > > MemoryArray::getDimensions() const
{
    return dimensions_;
}

//-----------------------------------------------------------------------------
// Function: MemoryArray::setDimensions()
//-----------------------------------------------------------------------------
void MemoryArray::setDimensions(QSharedPointer<QList<QSharedPointer<Dimension> > > dimensions)
{
    dimensions_ = dimensions;
}

//-----------------------------------------------------------------------------
// Function: MemoryArray::getStride()
//-----------------------------------------------------------------------------
QString MemoryArray::getStride() const
{
    return stride_;
}

//-----------------------------------------------------------------------------
// Function: MemoryArray::setStride()
//-----------------------------------------------------------------------------
void MemoryArray::setStride(QString const& stride)
{
    stride_ = stride;
}
