//-----------------------------------------------------------------------------
// File: BooleanValue.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Esko Pekkarinen
// Date: 05.08.2015
//
// Description:
// Kactus2 type for a boolean value, that can contain an unspecified value.
//-----------------------------------------------------------------------------

#ifndef BOOLEANVALUE_H
#define BOOLEANVALUE_H

#include <QString>

#include <IPXACTmodels/ipxactmodels_global.h>

//-----------------------------------------------------------------------------
//! Kactus2 type for a boolean value, that can contain an unspecified value.
//-----------------------------------------------------------------------------
class IPXACTMODELS_EXPORT BooleanValue 
{
public:

	//! The constructor.
	BooleanValue();

    //! Copy constructor;
    BooleanValue(BooleanValue const& other);
    
    //! Move constructor;
    BooleanValue(BooleanValue&& other) = default;

	//! The destructor.
	~BooleanValue() = default;

    //! Assignment operator.
    BooleanValue& operator=(BooleanValue const& other);
    
    //! Move assignment operator.
    BooleanValue& operator=(BooleanValue&& other) = default;

    /*!
     *  Set a boolean value.
     *
     *      @param [in] value   The new boolean value.
     */
    void setValue(bool value);

    /*!
     *  Get the contained value as a boolean value.
     *
     *      @return True for a BOOL_TRUE, otherwise false. 
     */
    bool toBool() const;

    /*!
     *  Gets the value as a string.
     *
     *      @return String of the given value.
     */
    QString toString() const;

    /*!
     *  Set the value to unspecified.
     */
    void setUnspecified();

private:

    // Possible boolean values including unspecified.
    enum Boolean {
        BOOL_TRUE,
        BOOL_FALSE,
        BOOL_UNSPECIFIED
    };

    //! The containing value.
    Boolean value_;
};

#endif // BOOLEANVALUE_H
