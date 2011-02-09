/***************************************************************
 * Name:      wxJADMain.cpp
 * Purpose:   Code for Application Frame
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

#include "wxJADFrame.h"
#include "wxJADEvent.h"
#include "wxJADApp.h"
#include "wxJADTree.h"
#include "version.h"
#include <memory>

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

//helper functions
enum wxbuildinfoformat
{
	short_f, long_f
};

// ------------------------------------------------------------------------- //

wxString wxbuildinfo( wxbuildinfoformat format )
{
	wxString wxbuild( wxVERSION_STRING );

	if( format == long_f )
	{
#if defined(__WXMSW__)
		wxbuild << wxT( "-Windows" );
#elif defined(__WXMAC__)
		wxbuild << wxT( "-Mac" );
#elif defined(__UNIX__)
		wxbuild << wxT( "-Linux" );
#endif

#if wxUSE_UNICODE
		wxbuild << wxT( "-Unicode build" );
#else
		wxbuild << wxT( "-ANSI build" );
#endif // wxUSE_UNICODE
	}

	return wxbuild;
}

DEFINE_EVENT_TYPE( wxEVT_NEW_ENTRY )
DEFINE_EVENT_TYPE( wxEVT_TREE_SELECT )
DEFINE_EVENT_TYPE( wxEVT_NOTEBOOK_SELECT )

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

BEGIN_EVENT_TABLE( wxJADFrame, wxFrame )
	EVT_CLOSE( wxJADFrame::OnClose )
	EVT_MENU( idMenuOpen, wxJADFrame::OnFileOpen )
	EVT_MENU( idMenuDirOpen, wxJADFrame::OnDirOpen )
	EVT_MENU( idMenuQuit, wxJADFrame::OnQuit )
	EVT_MENU( idMenuAbout, wxJADFrame::OnAbout )
	EVT_MENU( idMenuLog, wxJADFrame::OnLog )
	EVT_COMMAND( wxID_ANY, wxEVT_NEW_ENTRY, wxJADFrame::OnNewEntry )
	EVT_COMMAND( wxID_ANY, wxEVT_TREE_SELECT, wxJADFrame::OnTreeSelect )
	EVT_COMMAND( wxID_ANY, wxEVT_NOTEBOOK_SELECT, wxJADFrame::OnNotebookSelect )
END_EVENT_TABLE()

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

wxJADFrame::wxJADFrame( wxFrame *frame, const wxString& title )
	: wxFrame( frame, wxID_ANY, title,  wxDefaultPosition, wxSize( 640, 480 ) )
{
#if wxUSE_MENUS
	// create a menu bar
	wxMenuBar* mbar = new wxMenuBar();
	wxMenu* fileMenu = new wxMenu( wxT( "" ) );
	fileMenu->Append( idMenuOpen, wxT( "&Open...\tCtrl-O" ), wxT( "Open a file" ) );
	fileMenu->Append( idMenuDirOpen, wxT( "&Open Directory...\tCtrl-D" ), wxT( "Open a directory" ) );
	fileMenu->Append( idMenuQuit, wxT( "&Quit\tAlt-F4" ), wxT( "Quit the application" ) );
	mbar->Append( fileMenu, wxT( "&File" ) );

	wxMenu* helpMenu = new wxMenu( wxT( "" ) );
	helpMenu->Append( idMenuAbout, wxT( "&About\tF1" ), wxT( "Show info about this application" ) );
	helpMenu->Append( idMenuLog, wxT( "&Log\tF1" ), wxT( "Show the wxWidgets log frame" ) );
	mbar->Append( helpMenu, wxT( "&Help" ) );

	SetMenuBar( mbar );
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
	// create a status bar with some information about the used wxWidgets version
	wxStatusBar * statusBar = CreateStatusBar( 2 );
	int widths[] = { -3, -1 };
	statusBar->SetStatusWidths( 2, widths );
	SetStatusText( wxT( "Started" ), 0 );
	SetStatusText( wxbuildinfo( short_f ), 1 );
#endif // wxUSE_STATUSBAR

	m_logWnd = 0;
}

// ------------------------------------------------------------------------- //

wxJADFrame::~wxJADFrame()
{
}

// ------------------------------------------------------------------------- //

void wxJADFrame::OnClose( wxCloseEvent &event )
{
	Destroy();
}

// ------------------------------------------------------------------------- //

void wxJADFrame::OnQuit( wxCommandEvent &event )
{
	Destroy();
}

// ------------------------------------------------------------------------- //

void wxJADFrame::OnAbout( wxCommandEvent &event )
{
	wxString msg = wxString::Format( wxT( "wxJAD %s %s (r%s)" ),
	                                 wxString( AutoVersion::FULLVERSION_STRING, wxConvUTF8 ).c_str(),
	                                 wxString( AutoVersion::STATUS, wxConvUTF8 ).c_str(),
	                                 wxString( AutoVersion::SVN_REVISION, wxConvUTF8 ).c_str() );
	wxMessageBox( msg, _( "Welcome to..." ) );
}

// ------------------------------------------------------------------------- //

void wxJADFrame::OnLog( wxCommandEvent & event )
{
	if( !m_logWnd )
		m_logWnd = new wxLogWindow( this, wxT( "wxJAD Log" ) );

// m_logWnd->Show();
}

// ------------------------------------------------------------------------- //

void wxJADFrame::OnNewEntry( wxCommandEvent & event )
{
	wprintf( wxT( "New entry received...\n" ) );
	JadEntry * pEntry = ( JadEntry * ) event.GetClientData();
	if( pEntry != NULL )
		tree->AddEntry( pEntry );
}

// ------------------------------------------------------------------------- //

void wxJADFrame::OnTreeSelect( wxCommandEvent & event )
{
	//item was selected in the tree view
	//make sure its synchronised with the notebook view
	JadEntry * pEntry = ( JadEntry * ) event.GetClientData();
	if( pEntry != NULL )
	{
		if( pEntry->NotebookPage() > -1 )
			notebook->SetSelection( pEntry->NotebookPage() );
		else
			notebook->AddPage( pEntry, wxGetApp().GetFileMgr()->GetFileData( pEntry ) );
	}
}

// ------------------------------------------------------------------------- //

void wxJADFrame::OnNotebookSelect( wxCommandEvent & event )
{
	//item was selected in the notebook view
	//make sure its synchronised with the tree view
	JadEntry * pEntry = ( JadEntry * ) event.GetClientData();
	if( pEntry != NULL && pEntry->TreeItem().IsOk() )
		tree->SelectItem( pEntry->TreeItem() );
}

// ------------------------------------------------------------------------- //

void wxJADFrame::OnFileOpen( wxCommandEvent& event )
{
	wxFileDialog fd( this, wxT( "Open a CLASS file" ), wxT( "" ), wxT( "" ),
	                 wxT( "Class files (*.class)|*.class|Java files (*.java)|*.java|JAR files (*.jar)|*.jar|ZIP files (*.zip)|*.zip|All Files (*.*)|*.*" ),
	                 wxFD_FILE_MUST_EXIST | wxFD_OPEN | wxFD_PREVIEW );
	int res = fd.ShowModal();

	if( res != wxID_OK )
		return;

	wxGetApp().GetFileMgr()->Open( fd.GetPath() );
}

// ------------------------------------------------------------------------- //

void wxJADFrame::OnDirOpen( wxCommandEvent& event )
{
	wxDirDialog dd( this, wxT( "Choose a directory" ), wxT( "" ),
	                 wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST );
	int res = dd.ShowModal();

	if( res != wxID_OK )
		return;

	wxGetApp().GetFileMgr()->Open( dd.GetPath() );
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

