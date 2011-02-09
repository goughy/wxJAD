
#include "wxJADDropTarget.h"
#include "wxJADApp.h"

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

wxJADDropTarget::wxJADDropTarget()
{
}

// ------------------------------------------------------------------------- //

bool wxJADDropTarget::OnDropFiles( wxCoord x, wxCoord y, const wxArrayString& filenames )
{
	for( unsigned int i = 0; i < filenames.GetCount(); i++ )
	{
		wprintf( wxT( "Dropped %d: %ls\n" ), i, filenames[ i ].c_str() );
		wxGetApp().GetFileMgr()->Open( filenames[ i ] );
	}
	return true;
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //
