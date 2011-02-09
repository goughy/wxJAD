#include "wxJADTree.h"
#include "wxJADApp.h"
#include <wx/imaglist.h>
#include <wx/artprov.h>
#include <wx/filename.h>
#include <wx/msgdlg.h>
#include <wx/menu.h>

#include "PropertiesDlg.h"
#include "wxJADEvent.h"

#define IMAGE_FOLDER  0
#define IMAGE_CLASS  1
#define IMAGE_FILE   2

#define TREE_ROOT  -1
#define CLASS_ROOT  0

#define JAD_TREEID  2001

#define JAD_ID_OPEN  3001
#define JAD_ID_CLOSE 3002
#define JAD_ID_PROPS 3003
#define JAD_ID_SAVE  3004

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

class TreeInfo : public wxTreeItemData
{
public:

	TreeInfo() {}

	TreeInfo( JadEntry * entry ) : m_entry( entry ) {}

	virtual ~TreeInfo() {}

	JadEntry * Entry()
	{
		return m_entry;
	}

private:

	JadEntry * m_entry;
};

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

BEGIN_EVENT_TABLE( wxJADTree, wxTreeCtrl )
	EVT_TREE_SEL_CHANGED( JAD_TREEID, wxJADTree::OnItemSelected )
	EVT_TREE_ITEM_RIGHT_CLICK( JAD_TREEID, wxJADTree::OnItemRClick )
	EVT_MENU( JAD_ID_PROPS, wxJADTree::OnPropertiesMenu )
	EVT_MENU( JAD_ID_CLOSE, wxJADTree::OnCloseItem )
	EVT_TREE_ITEM_EXPANDING( JAD_TREEID, wxJADTree::OnItemExpanded )
END_EVENT_TABLE()

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

wxJADTree::wxJADTree( wxWindow * parent ) :
	wxTreeCtrl( parent, JAD_TREEID, wxDefaultPosition, wxDefaultSize, wxTR_HIDE_ROOT | wxTR_HAS_BUTTONS )
{
	wxImageList * imgList = new wxImageList();
	imgList->Add( wxArtProvider::GetBitmap( wxART_FOLDER ) );
	imgList->Add( wxArtProvider::GetBitmap( wxART_EXECUTABLE_FILE ) );
	imgList->Add( wxArtProvider::GetBitmap( wxART_NORMAL_FILE ) );

	SetImageList( imgList );
	wxTreeItemId root = AddRoot( wxT( "_Hidden_" ) );   //should be hidden root
	orphanItems = AppendItem( root, wxT( "Open Files" ), IMAGE_FOLDER );

	dropTarget = new wxJADDropTarget;

	SetDropTarget( dropTarget );
}

// ------------------------------------------------------------------------- //

wxJADTree::~wxJADTree()
{
}

// ------------------------------------------------------------------------- //

void wxJADTree::OnItemSelected( wxTreeEvent & event )
{
	wxTreeItemId selItem = event.GetItem();
	if( !selItem.IsOk() )
		return;

	TreeInfo * pInfo = ( TreeInfo * ) GetItemData( selItem );
	if( !pInfo || pInfo->Entry() == NULL )
		return;

	wxCommandEvent ev( wxEVT_TREE_SELECT );
	ev.SetClientData( pInfo->Entry() );

	switch( pInfo->Entry()->Type() )
	{
		case JadEntry::CLASS:
		case JadEntry::PLAIN:
		case JadEntry::ZIPCLASS:
		case JadEntry::ZIPPLAIN:
			::wxPostEvent( wxGetApp().GetTopWindow(), ev );
			break;

		//nothing to do for these types...
		case JadEntry::JAR:
		case JadEntry::ZIP:
		case JadEntry::DIR:
		case JadEntry::UNKNOWN:
		default:
			break;
	}
}

// ------------------------------------------------------------------------- //

void wxJADTree::OnItemRClick( wxTreeEvent & event )
{
	wxTreeItemId selItem = event.GetItem();

	if( !selItem.IsOk() )
		return;

	TreeInfo * pInfo = ( TreeInfo * ) GetItemData( selItem );

	if( !pInfo )
		return;

	wxMenu menu;

	wxPoint point = event.GetPoint();

	// add stuff
	menu.Append( JAD_ID_OPEN, wxT( "Open" ) );

	menu.Append( JAD_ID_CLOSE, wxT( "Close" ) );

	switch( pInfo->Entry()->Type() )
	{
		case JadEntry::CLASS:
			menu.AppendSeparator();
			menu.Append( JAD_ID_SAVE, wxT( "Convert..." ) );
			break;

		case JadEntry::ZIP:
		case JadEntry::JAR:
			menu.AppendSeparator();
			menu.Append( JAD_ID_SAVE, wxT( "Convert All..." ) );
			break;

		case JadEntry::PLAIN:
			menu.AppendSeparator();
			menu.Append( JAD_ID_SAVE, wxT( "Extract..." ) );
			break;

		case JadEntry::UNKNOWN:
		case JadEntry::DIR:
		default:
			break;
	}

	menu.AppendSeparator();

	menu.Append( JAD_ID_PROPS, wxT( "Properties" ) );

	// and then display
	PopupMenu( &menu, point.x, point.y );
}

// ------------------------------------------------------------------------- //

void wxJADTree::OnPropertiesMenu( wxCommandEvent & event )
{
	wxTreeItemId selItem = GetSelection();
	if( !selItem.IsOk() )
		return;

	TreeInfo * pInfo = ( TreeInfo * ) GetItemData( selItem );

	if( !pInfo )
		return;

	PropertiesDlg dlg( this, wxID_ANY );
	wxFileName fn = pInfo->Entry()->Name();
	wxString fileName, sourceName;

	int bangPos = fn.GetFullPath().Find( wxT( '!' ) );
	if( bangPos != wxNOT_FOUND )
	{
		sourceName = fn.GetFullPath().Mid( 0, bangPos );
		fileName = fn.GetFullPath().Mid( bangPos + 1 );
	}
	else
	{
		fileName = fn.GetFullName();
		sourceName = fn.GetPath();
	}

	dlg.nameLabel->SetLabel( fileName );
	dlg.sourceLabel->SetLabel( sourceName );

	dlg.ShowModal();
}

// ------------------------------------------------------------------------- //

void wxJADTree::OnCloseItem( wxCommandEvent & event )
{
	wxTreeItemId selItem = GetSelection();
	if( !selItem.IsOk() )
		return;

	TreeInfo * pInfo = ( TreeInfo * ) GetItemData( selItem );

	if( !pInfo )
		return;


}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

void wxJADTree::OnItemExpanded( wxTreeEvent & event )
{
	TreeInfo * pInfo = ( TreeInfo * ) GetItemData( event.GetItem() );
	if( pInfo == NULL )
		return;

	//find the first child item and verify its NOT the "Loading..." placeholder...
	wxTreeItemIdValue cookie;
	wxTreeItemId childItem = GetFirstChild( event.GetItem(), cookie );
	if( GetItemData( childItem ) == NULL )
	{
		AddChildren( event.GetItem(), pInfo->Entry() );
	}
}

// ------------------------------------------------------------------------- //

void wxJADTree::AddChildren( wxTreeItemId item, JadEntry * pEntry )
{
	if( pEntry == NULL )
		return;

	//if there is no attached info, its a placeholder
	//so delete the children and replace with the JadEntry children (if any)
	DeleteChildren( item );
	for( JadMap::const_iterator it = pEntry->GetChildren().begin();
			it != pEntry->GetChildren().end(); it++ )
	{
		AddEntry( const_cast<JadEntry *>( & it->second ), item );
	}
}

// ------------------------------------------------------------------------- //

wxTreeItemId wxJADTree::AddEntry( JadEntry * pEntry, wxTreeItemId parent )
{
	wprintf( wxT( "Adding: %ls (%ls)\n" ), pEntry->Name().c_str(), jad_str( pEntry->Type() ).c_str() ) ;

	if( !parent.IsOk() )
	{
		if( pEntry->Parent() != NULL && pEntry->Parent()->TreeItem().IsOk() )
			parent = pEntry->Parent()->TreeItem();
		else
			parent = GetRootItem();
	}

	//we have this item already in the tree - update it if
	//its a directory...
	if( pEntry->TreeItem().IsOk() && pEntry->Type() == JadEntry::DIR )
	{
		AddChildren( pEntry->TreeItem(), pEntry );
		return pEntry->TreeItem();
	}

	wxTreeItemId item;
	switch( pEntry->Type() )
	{
		case JadEntry::ZIP:
		case JadEntry::JAR:
			item = AppendItem( parent, pEntry->Name(), IMAGE_CLASS );
			//add a loading child...
			AppendItem( item, wxT( "Loading..." ) );
			break;

		case JadEntry::DIR:
			item = AppendItem( parent, pEntry->Name(), IMAGE_FOLDER );
			//add a loading child...
			AppendItem( item, wxT( "Loading..." ) );
			break;

		case JadEntry::CLASS:
		case JadEntry::PLAIN:
		default:
			item = AppendItem( parent, pEntry->Name(), IMAGE_FILE );
			break;
	}

	SetItemData( item, new TreeInfo( pEntry ) );
	//and set the corresponding tree item id in the JadEntry...
	pEntry->TreeItem() = item;
	return item;
}

// ------------------------------------------------------------------------- //

bool wxJADTree::SelectEntry( const wxFileName & name )  //returns true if entry found, false if it doesn't exist
{
	TreeInfo * pInfo = 0;

	//check to see whether the requested item is already the current selection
	wxTreeItemId selItem = GetSelection();

	if( selItem.IsOk() )
	{
		pInfo = ( TreeInfo * ) GetItemData( selItem );

		if( !pInfo )
			return false;

		if( pInfo->Entry()->Name() == name.GetFullPath() )
			return true;
	}

	//recurse the tree looking for the filename in the wxTreeItemData of the respective entry...
	wxTreeItemId item = FindItem( GetRootItem(), name );

	if( item.IsOk() )
		SelectItem( item );

	return item.IsOk();
}

// ------------------------------------------------------------------------- //

wxTreeItemId wxJADTree::FindItem( wxTreeItemId root, const wxFileName & fname )
{
	wxTreeItemIdValue cookie;
	wxTreeItemId search;
	wxTreeItemId item = GetFirstChild( root, cookie );
	wxTreeItemId child;

	while( item.IsOk() )
	{
		TreeInfo * pInfo = ( TreeInfo * ) GetItemData( item );

		if( pInfo && pInfo->Entry()->Name() == fname.GetFullPath() )
			return item;

		if( ItemHasChildren( item ) )
		{
			wxTreeItemId search = FindItem( item, fname );

			if( search.IsOk() )
				return search;
		}

		item = GetNextChild( root, cookie );
	}

	/* Not found */
	wxTreeItemId dummy;

	return dummy;
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //


