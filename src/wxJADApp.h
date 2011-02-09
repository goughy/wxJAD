/***************************************************************
 * Name:      wxJADApp.h
 * Purpose:   Defines Application Class
 * Author:    Andrew Gough (andrew@goughy.org)
 * Created:   2009-07-21
 * Copyright: Andrew Gough (http://www.goughy.org)
 * License:
 **************************************************************/

#ifndef WXJADAPP_H
#define WXJADAPP_H

#include <wx/app.h>
#include <wx/aui/aui.h>
#include "wxJADFileMgr.h"

extern wxAuiManager * g_aui;

class wxJADApp : public wxApp
{
public:
	virtual bool OnInit();

	wxJADFileMgr * GetFileMgr() { return & fileMgr; }

private:

	wxJADFileMgr fileMgr;
};

DECLARE_APP( wxJADApp )

#endif // WXJADAPP_H
