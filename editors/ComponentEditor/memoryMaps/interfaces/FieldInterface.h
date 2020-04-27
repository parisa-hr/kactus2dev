//-----------------------------------------------------------------------------
// File: FieldInterface.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 07.04.2020
//
// Description:
// Interface for editing fields.
//-----------------------------------------------------------------------------

#ifndef FIELDINTERFACE_H
#define FIELDINTERFACE_H

#include <editors/ComponentEditor/common/interfaces/ParameterizableInterface.h>
#include <editors/ComponentEditor/common/interfaces/NameGroupInterface.h>

class Field;
class FieldValidator;
class ResetInterface;

#include <QVector>
#include <QMap>

//-----------------------------------------------------------------------------
//! Interface for editing fields.
//-----------------------------------------------------------------------------
class FieldInterface : public ParameterizableInterface, public NameGroupInterface
{

public:

	/*!
	 *  The constructor.
	 */
	FieldInterface();
	
	/*!
     *  The destructor.
     */
    virtual ~FieldInterface() = default;

    /*!
     *  Set available fields.
     *
     *      @param [in] newFields   The new fields.
     */
    void setFields(QSharedPointer<QList<QSharedPointer<Field> > > newFields);

    /*!
     *  Set field validator.
     *
     *      @param [in] validator   Validator for fields.
     */
    void setValidator(QSharedPointer<FieldValidator> validator);

    /*!
     *  Get index of the selected item.
     *
     *      @param [in] itemName    Name of the selected item.
     *
     *      @return Index of the selected item.
     */
    virtual int getItemIndex(std::string const& itemName) const override final;

    /*!
     *  Get name of the indexed item.
     *
     *      @param [in] itemIndex   Index of the selected item.
     *
     *      @return Name of the selected item.
     */
    virtual std::string getIndexedItemName(int const& itemIndex) const override final;

    /*!
     *  Get the number of available items.
     *
     *      @return Number of available items.
     */
    virtual int itemCount() const override final;

    /*!
     *  Get the names of the available items.
     *
     *      @return Names of the available items.
     */
    virtual std::vector<std::string> getItemNames() const override final;

    /*!
     *  Set a new name for the selected item.
     *
     *      @param [in] currentName     Name of the selected item.
     *      @param [in] newName         New name for the item.
     *
     *      @return True, if successful, false otherwise.
     */
    virtual bool setName(std::string const& currentName, std::string const& newName) override final;

    /*!
     *  Get the description of the selected item.
     *
     *      @param [in] itemName    Name of the selected item.
     *
     *      @return Description of the selected item.
     */
    virtual std::string getDescription(std::string const& itemName) const override final;

    /*!
     *  Set a new description for the selected item.
     *
     *      @param [in] itemName        Name of the selected item.
     *      @param [in] newDescription  New description.
     *
     *      @return True, if successful, false otherwise.
     */
    virtual bool setDescription(std::string const& itemName, std::string const& newDescription) override final;

    /*!
     *  Get the calculated offset value of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *      @param [in] baseNumber  Base for displaying the value.
     *
     *      @return Calculated offset value of the selected field.
     */
    std::string getOffsetValue(std::string const& fieldName, int const& baseNumber = 0) const;

    /*!
     *  Get the formatted offset expression of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Formatted offset expression of the selected field.
     */
    std::string getOffsetFormattedExpression(std::string const& fieldName) const;

    /*!
     *  Get the offset expression of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Offset expression of the selected field.
     */
    std::string getOffsetExpression(std::string const& fieldName) const;

    /*!
     *  Set a new offset value for the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *      @param [in] newOffset   New offset value.
     *
     *      @return True, if successful, false otherwise.
     */
    bool setOffset(std::string const& fieldName, std::string const& newOffset);

    /*!
     *  Get the calculated bit width value of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *      @param [in] baseNumber  Base for displaying the value.
     *
     *      @return Calculated bit width value of the selected parameter.
     */
    std::string getWidthValue(std::string const& fieldName, int const& baseNumber = 0) const;
    
    /*!
     *  Get the formatted bit width expression of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Formatted bit width expression of the selected field.
     */
    std::string getWidthFormattedExpression(std::string const& fieldName) const;

    /*!
     *  Get the bit width expression of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Bit width expression of the selected field.
     */
    std::string getWidthExpression(std::string const& fieldName) const;

    /*!
     *  Set a new bit width value for the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *      @param [in] newWidth    New bit width value.
     *
     *      @return True, if successful, false otherwise.
     */
    bool setWidth(std::string const& fieldName, std::string const& newWidth);

    /*!
     *  Get the reset values of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Reset values of the selected field.
     */
    std::string getResets(std::string const& fieldName) const;

    /*!
     *  Get the tooltip of the reset values of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Tooltip constructed from the reset values of the selected field.
     */
    std::string getResetsToolTip(std::string const& fieldName) const;

    /*!
     *  Get the volatile status of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Volatile status of the selected field.
     */
    std::string getVolatile(std::string const& fieldName) const;

    /*!
     *  Set the volatile status of the selected field.
     *
     *      @param [in] fieldName       Name of the selected field.
     *      @param [in] newVolatile     The new volatile status of the selected field.
     *
     *      @return True, if successful, false otherwise.
     */
    bool setVolatile(std::string const& fieldName, std::string const& newVolatile);

    /*!
     *  Get the access of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Access of the selected field.
     */
    std::string getAccess(std::string const& fieldName) const;

    /*!
     *  Set the access of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *      @param [in] newAccess   The new access of the selected field.
     *
     *      @return True, if successful, false otherwise.
     */
    bool setAccess(std::string const& fieldName, std::string const& newAccess);

    /*!
     *  Get the modified write value of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Modified write value of the selected field.
     */
    std::string getModifiedWrite(std::string const& fieldName) const;

    /*!
     *  Set the modified write value of the selected field.
     *
     *      @param [in] fieldName           Name of the selected field.
     *      @param [in] newModifiedWrite    The new modified write value of the selected field.
     *
     *      @return True, if successful, false otherwise.
     */
    bool setModifiedWrite(std::string const& fieldName, std::string const& newModifiedWrite);

    /*!
     *  Get the read action of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Read action of the selected field.
     */
    std::string getReadAction(std::string const& fieldName) const;

    /*!
     *  Set the read action of the selected field.
     *
     *      @param [in] fieldName       Name of the selected field.
     *      @param [in] newReadAction   The new read action of the selected field.
     *
     *      @return True, if successful, false otherwise.
     */
    bool setReadAction(std::string const& fieldName, std::string const& newReadAction);

    /*!
     *  Get the testable string of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Testable string of the selected field.
     */
    std::string getTestableValue(std::string const& fieldName) const;

    /*!
     *  Get the testable bool of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Testable bool of the selected field.
     */
    bool getTestableBool(std::string const& fieldName) const;

    /*!
     *  Set the testable value of the selected field.
     *
     *      @param [in] fieldName       Name of the selected field.
     *      @param [in] newTestable     The new testable value of the selected field.
     *
     *      @return True, if successful, false otherwise.
     */
    bool setTestable(std::string const& fieldName, std::string const& newTestable);

    /*!
     *  Get the test constraint of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Test constraint of the selected field.
     */
    std::string getTestConstraint(std::string const& fieldName) const;

    /*!
     *  Set the test constraint of the selected field.
     *
     *      @param [in] fieldName           Name of the selected field.
     *      @param [in] newTestConstraint   The new test constraint of the selected field.
     *
     *      @return True, if successful, false otherwise.
     */
    bool setTestConstraint(std::string const& fieldName, std::string const& newTestConstraint);

    /*!
     *  Get the calculated is present value of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *      @param [in] baseNumber  Base for displaying the value.
     *
     *      @return Calculated is present value of the selected field.
     */
    std::string getIsPresentValue(std::string const& fieldName, int const& baseNumber = 0) const;

    /*!
     *  Get the formatted is present expression of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Formatted is present expression of the selected field.
     */
    std::string getIsPresentFormattedExpression(std::string const& fieldName) const;

    /*!
     *  Get the is present expression of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Is present expression of the selected parameter.
     */
    std::string getIsPresentExpression(std::string const& fieldName) const;

    /*!
     *  Set a new is present value for the selected field.
     *
     *      @param [in] parameterName   Name of the selected parameter.
     *      @param [in] newIsPresent    New is present value.
     *
     *      @return True, if successful, false otherwise.
     */
    bool setIsPresent(std::string const& fieldName, std::string const& newIsPresent);

    /*!
     *  Calculate all the references to the selected ID in the selected item.
     *
     *      @param [in] itemName    Name of the selected item.
     *      @param [in] valueID     The selected ID.
     *
     *      @return Number of references to the selected ID in the selected item.
     */
    virtual int getAllReferencesToIdInItem(const std::string& itemName, std::string const&  valueID) const override final;

    /*!
     *  Validates the contained items.
     *
     *      @return True, if all the ports are valid, false otherwise.
     */
    virtual bool validateItems() const override final;

    /*!
     *  Check if the selected item has a valid name.
     *
     *      @param [in] itemName    Name of the selected item.
     *
     *      @return True, if the name is valid, false otherwise.
     */
    virtual bool itemHasValidName(std::string const& itemName) const override final;

    /*!
     *  Check if the selected field has a valid offset.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return True, if the offset is valid, false otherwise.
     */
    bool hasValidOffset(std::string const& fieldName) const;

    /*!
     *  Check if the selected field has a valid bit width.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return True, if the bit width is valid, false otherwise.
     */
    bool hasValidWidth(std::string const& fieldName) const;

    /*!
     *  Check if the selected field has valid reset values.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return True, if the reset values are valid, false otherwise.
     */
    bool hasValidResets(std::string const& fieldName) const;

    /*!
     *  Check if the selected field has a valid is present.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return True, if the is present is valid, false otherwise.
     */
    bool hasValidIsPresent(std::string const& fieldName) const;

    /*!
     *  Check if the selected field has a valid access.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return True, if the access is valid, false otherwise.
     */
    bool hasValidAccess(std::string const& fieldName) const;

    /*!
     *  Add a new field.
     *
     *      @param [in] row             Row of the new field.
     *      @param [in] newFieldName    Name of the new field.
     */
    void addField(int const& row, std::string const& newFieldName = std::string(""));

    /*!
     *  Remove the selected field.
     *
     *      @param [in] FieldName   Name of the selected field.
     *
     *      @return True, if successful, false otherwise.
     */
    bool removeField(std::string const& fieldName);

    /*!
     *  Copy the selected fields.
     *
     *      @param [in] selectedRows    Indexes of the selected fields.
     */
    void copyRows(std::vector<int> selectedRows);

    /*!
     *  Paste the selected fields.
     *
     *      @return Names of the pasted fields.
     */
    std::vector<std::string> pasteRows();

    /*!
     *  Get the number of pasted fields.
     *
     *      @return Number of the pasted fields.
     */
    int getPasteRowCount() const;

    /*!
     *  Get the expressions in the selected fields.
     *
     *      @param [in] fieldNames  Names of the selected fields.
     *
     *      @return The expressions of the selected fields.
     */
    std::vector<std::string> getExpressionsInSelectedFields(std::vector<std::string> fieldNames) const;

    /*!
     *  Add a reset interface.
     *
     *      @param [in] fieldName           Name of the field containing the resets.
     *      @param [in] newResetInterface   The new reset interface.
     */
    void addResetInterface(std::string const& fieldName, ResetInterface* newResetInterface);

    /*!
     *  Remove the selected reset interface.
     *
     *      @param [in] removedInterface    The selected reset interface.
     *
     *      @return True, if successful, false otherwise.
     */
    bool removeResetInterface(ResetInterface* removedInterface);

    /*!
     *  Get the list of reset interfaces.
     */
    std::vector<ResetInterface*> getResetInterfaces() const;

    /*!
     *  Get the reset interface of the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return Reset interface of the selected field.
     */
    ResetInterface* getResetInterface(std::string const& fieldName) const;

private:

    /*!
     *  Get the selected field.
     *
     *      @param [in] fieldName   Name of the selected field.
     *
     *      @return The selected field.
     */
    QSharedPointer<Field> getField(std::string const& fieldName) const;

    /*!
     *  Change the key in the reset interfaces.
     *
     *      @param [in] currentName     The current key.
     *      @param [in] newName         The new key.
     */
    void changeKeyInSubInterfaces(QString const& currentName, QString const& newName);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! List of the contained fields.
    QSharedPointer<QList<QSharedPointer<Field> > > fields_;

    //! Validator for fields.
    QSharedPointer<FieldValidator> validator_;

    //! Map for the contained reset interfaces.
    QMap<QString, ResetInterface*> resetInterfaces_;
};

#endif // REGISTERTABLEMODEL_H
