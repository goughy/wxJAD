#include "wxJADNotebook.h"

#include "wxJADApp.h"
#include "wxJADEvent.h"

#define JAD_NOTEBOOKID 2002

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

static wxChar* MyKeyWords =
    wxT( "new delete class import public protected private package throw throws static " )
    wxT( "if else while do switch case default for foreach break continue return catch implements " )
    wxT( "int char float string bool boolean data byte finally typeid dynamic interface abstract " )
    wxT( "const weakref private ref synchronized enum " )
    wxT( "hexdata ascii unicode " )
    wxT( "true false null void" );

BEGIN_EVENT_TABLE( wxJADNotebook, wxAuiNotebook )
	EVT_AUINOTEBOOK_PAGE_CHANGED( JAD_NOTEBOOKID, wxJADNotebook::OnPageSelected )
END_EVENT_TABLE()

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

wxJADNotebook::wxJADNotebook( wxWindow * parent ) :
	wxAuiNotebook( parent, JAD_NOTEBOOKID, wxDefaultPosition, wxDefaultSize,
				wxAUI_NB_TOP | wxAUI_NB_TAB_SPLIT | wxAUI_NB_TAB_MOVE |
				wxAUI_NB_SCROLL_BUTTONS | wxAUI_NB_WINDOWLIST_BUTTON |
				wxAUI_NB_CLOSE_ON_ALL_TABS )
{
	//add an empty page
// AddPage( , wxT( "" ) );

}

// ------------------------------------------------------------------------- //

wxJADNotebook::~wxJADNotebook()
{
	//dtor
}

// ------------------------------------------------------------------------- //

void wxJADNotebook::OnPageSelected( wxAuiNotebookEvent & event )
{
//	wprintf( wxT( "wxJADNotebook::OnPageSelected\n" ) );
	int selPage = event.GetSelection();
	wxWindow * selWindow = GetPage( selPage );

	if( !selWindow )
		return;

	JadEntry * pEntry = (JadEntry *) selWindow->GetClientData();
	if( pEntry != NULL )
	{
//		wprintf( wxT( "wxJADNotebook::OnPageSelected: posting notebook selection event\n" ) );

		wxCommandEvent ev( wxEVT_NOTEBOOK_SELECT );
		ev.SetClientData( pEntry );

		::wxPostEvent( wxGetApp().GetTopWindow(), ev );
	}
}

// ------------------------------------------------------------------------- //

void wxJADNotebook::AddPage( JadEntry * pEntry, const wxString & data )
{
	wxStyledTextCtrl * stcCtrl = new wxStyledTextCtrl( this, wxID_ANY, wxDefaultPosition,
	        wxDefaultSize, wxSIMPLE_BORDER );
	wxFont codeFont( 8, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL );

	stcCtrl->SetLexer( wxSTC_LEX_CPP );
// stcCtrl->SetMarginType( 0, wxSCI_MARGIN_NUMBER );
	stcCtrl->SetMarginLeft( 5 );
// stcCtrl->SetMarginRight( 3 );
	stcCtrl->SetMarginWidth( 0, 25 );

	for( int i = 0; i < wxSTC_STYLE_LASTPREDEFINED; i++ )
		stcCtrl->StyleSetFont( i, codeFont );

	stcCtrl->StyleSetForeground( wxSTC_C_COMMENT, wxColor( 0, 100, 0 ) );
	stcCtrl->StyleSetForeground( wxSTC_C_COMMENTLINE, wxColor( 0, 125, 0 ) );
	stcCtrl->StyleSetForeground( wxSTC_C_STRING, wxColor( 128, 0, 0 ) );
	stcCtrl->StyleSetForeground( wxSTC_C_WORD, wxColor( 0, 0, 200 ) );
	stcCtrl->SetKeyWords( 0, MyKeyWords );
	stcCtrl->SetTabWidth( 4 );
	stcCtrl->SetUseTabs( false );
	stcCtrl->SetTabIndents( true );
	stcCtrl->SetBackSpaceUnIndents( true );
	stcCtrl->SetIndent( 4 );
	stcCtrl->SetText( data );

	wxAuiNotebook::AddPage( stcCtrl, pEntry->Name(), true ); //true == select page
	stcCtrl->SetClientData( pEntry );
	pEntry->NotebookPage() = GetSelection();
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

