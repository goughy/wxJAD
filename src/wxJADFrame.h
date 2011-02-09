/***************************************************************
 * Name:      wxJADMain.h
 * Purpose:   Defines Application Frame
 * Author:    Andrew Gough (andrew@goughy.org)
 * Created:   2009-07-21
 * Copyright: Andrew Gough (http://www.goughy.org)
 * License:
 **************************************************************/

#ifndef WXJADMAIN_H
#define WXJADMAIN_H

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "wxJADTree.h"
#include "wxJADNotebook.h"

class wxJADFrame: public wxFrame
{
public:

	wxJADFrame( wxFrame *frame, const wxString& title );
	~wxJADFrame();

	void SetTree( wxJADTree * pTree ) { tree = pTree; }
	void SetNotebook( wxJADNotebook * pNb ) { notebook = pNb; }

private:

	void OnClose( wxCloseEvent & event );
	void OnQuit( wxCommandEvent & event );
	void OnAbout( wxCommandEvent & event );
	void OnLog( wxCommandEvent & event );
	void OnNewEntry( wxCommandEvent & event );
	void OnTreeSelect( wxCommandEvent & event );
	void OnNotebookSelect( wxCommandEvent & event );

	void OnFileOpen( wxCommandEvent & event );
	void OnDirOpen( wxCommandEvent & event );

	wxJADTree * tree;
	wxJADNotebook * notebook;
	wxLogWindow * m_logWnd;

	DECLARE_EVENT_TABLE()
};


#endif // WXJADMAIN_H
