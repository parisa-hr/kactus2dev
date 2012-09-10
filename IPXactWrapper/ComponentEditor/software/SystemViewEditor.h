//-----------------------------------------------------------------------------
// File: SystemViewEditor.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Joni-Matti M��tt�
// Date: 22.5.2012
//
// Description:
// System view editor.
//-----------------------------------------------------------------------------

#ifndef SYSTEMVIEWEDITOR_H
#define SYSTEMVIEWEDITOR_H

#include <IPXactWrapper/ComponentEditor/itemeditor.h>

#include <common/widgets/nameGroupEditor/namegroupeditor.h>
#include <common/widgets/vlnvEditor/vlnveditor.h>

class SystemView;

//-----------------------------------------------------------------------------
//! System view editor.
//-----------------------------------------------------------------------------
class SystemViewEditor : public ItemEditor
{
    Q_OBJECT

public:
    /*!
     *  Constructor.
     *
     *      @param [in] component    The component being edited.
     *      @param [in] systemView		 The system view being edited.
     *      @param [in] libHandler   The library handler.
     *      @param [in] parent       The parent widget.
     */
	SystemViewEditor(QSharedPointer<Component> component,
		QSharedPointer<SystemView> systemView,
		LibraryInterface* libHandler, 
		QWidget *parent);

    /*!
     *  Destructor.
     */
    virtual ~SystemViewEditor();

    /*!
     *  Checks for the validity of the edited API interface.
     *
	 *      @return True if the API interface is in valid state.
	 */
	bool isValid() const;

	/*! \brief Reload the information from the model to the editor.
	*/
	virtual void refresh();

protected:

	//! \brief Handler for widget's show event
	virtual void showEvent(QShowEvent* event);

private slots:

	//! \brief Handler for changes in hierarchy reference.
	void onHierRefChange();

private:
    // Disable copying.
    SystemViewEditor(SystemViewEditor const& rhs);
    SystemViewEditor& operator=(SystemViewEditor const& rhs);

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! \brief Pointer to the instance that manages the library.
    LibraryInterface* libHandler_;

    //! \brief Pointer to the view being edited.
    SystemView* view_;

    //! \brief Editor to set the name, display name and description of the view.
    NameGroupEditor nameEditor_;

    //! VLNV editor for the hierarchy reference.
    VLNVEditor hierRefEditor_;
};

//-----------------------------------------------------------------------------

#endif // SYSTEMVIEWEDITOR_H
