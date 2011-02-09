/***************************************************************
 * Name:      wxJADApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Andrew Gough (andrew@goughy.org)
 * Created:   2009-07-21
 * Copyright: Andrew Gough (http://www.goughy.org)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "wxJADApp.h"
#include "wxJADFrame.h"
#include "wxJADTree.h"
#include "wxJADNotebook.h"
#include "wxJADEvent.h"

#include <wx/toolbar.h>
#include <wx/artprov.h>

#include "binreloc.h"
#include "version.h"

#ifdef WIN32
	#define EXE_EXT	 wxT("exe")
#else
	#define EXE_EXT	 wxT("")
#endif

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

//(*AppHeaders
#include <wx/image.h>
//*)

IMPLEMENT_APP( wxJADApp );

wxAuiManager * g_aui;

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

bool wxJADApp::OnInit()
{
	//(*AppInitialize
//	bool wxsOK = true;
	wxInitAllImageHandlers();
	//*)
//	return wxsOK;

#ifdef ENABLE_BINRELOC

	BrInitError br_err;

	if( br_init( &br_err ) == 0 )
	{
		wprintf( wxT( "Failed to initialise BINRELOC code: %d\n" ), br_err );
		return false;
	}

	wprintf( wxT( "BINRELOC resolved exe directory: %s\n" ), br_find_exe_dir( NULL ) );
	if( !wxFileName::FileExists( wxJADFileMgr::JADExe().GetFullPath() ) )
	{
		wprintf( wxT( "Failed to locate required JAD executable: %ls\n" ), wxJADFileMgr::JADExe().GetFullPath().c_str() );
		return false;
	}

#endif

	wxJADFrame * frame = new wxJADFrame( 0L, wxT( "wxJAD" ) );
	g_aui = new wxAuiManager( frame );

	wxToolBar * tb = new wxToolBar( frame, wxID_ANY, wxDefaultPosition, wxDefaultSize,
	                                wxTB_FLAT | wxTB_TEXT | wxBORDER_SIMPLE );
	tb->SetToolBitmapSize( wxSize( 16, 16 ) );
	tb->AddTool( idMenuOpen, wxT( "Open" ), wxArtProvider::GetBitmap( wxART_FILE_OPEN ) );
//    tb->AddSeparator();
	tb->Realize();

	wxAuiPaneInfo tbi; //toolbar info
	g_aui->AddPane( tb,
	                tbi.Name( wxT( "toolbar" ) ).Caption( wxT( "toolbar" ) ).ToolbarPane().
	                Top().PaneBorder( false ).Floatable( false ).Gripper( false ).
	                LeftDockable( false ).RightDockable( false ) );

	wxAuiPaneInfo tci; //treectrl info
	wxJADTree * tree = new wxJADTree( frame );
	g_aui->AddPane( tree,
	                tci.Name( wxT( "tree" ) ).Caption( wxT( "Class files" ) ).Left().
	                Layer( 1 ).Position( 1 ).BestSize( 200, 200 ).CloseButton( false ).MaximizeButton( true ) );

	wxAuiPaneInfo nbi; //notebook info
	wxJADNotebook * nb = new wxJADNotebook( frame );
	g_aui->AddPane( nb,
	                tci.Name( wxT( "notebook" ) ).Caption( wxT( "Source" ) ).Center().
	                Layer( 1 ).Position( 0 ).CaptionVisible( false ).CloseButton( false ).MaximizeButton( true ) );

	this->SetTopWindow( frame );
	frame->SetTree( tree );
	frame->SetNotebook( nb );
	frame->Show();
	g_aui->Update();
	return true;
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

