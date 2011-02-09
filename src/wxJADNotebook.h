#ifndef WXJADNOTEBOOK_H
#define WXJADNOTEBOOK_H

#include <wx/aui/auibook.h>
#include <wx/notebook.h>
#include <wx/stc/stc.h>
#include <wx/filename.h>
#include <map>

#include "wxJADEntry.h"

class wxJADNotebook : public wxAuiNotebook
{
public:

	typedef std::map< wxString, wxStyledTextCtrl * > PageMap;

public:
	wxJADNotebook( wxWindow * parent );
	virtual ~wxJADNotebook();

	void AddPage( JadEntry * pEntry, const wxString & data );
	bool SelectPage( const wxFileName & name ); //returns true if page found, false if it doesn't exist

	void OnPageSelected( wxAuiNotebookEvent & event );

private:

	wxStyledTextCtrl * AddPage( const wxString & name );

	DECLARE_EVENT_TABLE()
};

#endif // WXJADNOTEBOOK_H
