#ifndef WXJADTREE_H
#define WXJADTREE_H

#include <wx/treectrl.h>
#include <wx/filename.h>
#include <map>

#include "wxJADDropTarget.h"
#include "wxJADEntry.h"

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

class wxJADTree : public wxTreeCtrl
{
public:
    wxJADTree( wxWindow * parent );
    virtual ~wxJADTree();

    wxTreeItemId AddEntry( JadEntry * entry, wxTreeItemId parent = 0 );
    wxTreeItemId AddClassEntry( const wxFileName & fname, wxTreeItemId parent = 0, void * data = 0 );

    bool SelectEntry( const wxFileName & name ); //returns true if page found, false if it doesn't exist

    void OnItemSelected( wxTreeEvent & event );
    void OnItemRClick( wxTreeEvent & event );
    void OnPropertiesMenu( wxCommandEvent & event );
    void OnCloseItem( wxCommandEvent & event );
	void OnItemExpanded( wxTreeEvent & event );

private:

	void AddChildren( wxTreeItemId item, JadEntry * pEntry );
    wxTreeItemId FindItem( wxTreeItemId root, const wxFileName & fname );

    wxTreeItemId orphanItems;
    wxJADDropTarget * dropTarget;

    DECLARE_EVENT_TABLE()
};

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

#endif // WXJADTREE_H
