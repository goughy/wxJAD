#include "PropertiesDlg.h"

//(*InternalHeaders(PropertiesDlg)
#include <wx/string.h>
#include <wx/intl.h>
//*)

//(*IdInit(PropertiesDlg)
const long PropertiesDlg::ID_STATICTEXT1 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT2 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT3 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT4 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT5 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT6 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT7 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT8 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT9 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT10 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT11 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT12 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT13 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT14 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT15 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT16 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT17 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT18 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT19 = wxNewId();
const long PropertiesDlg::ID_STATICTEXT20 = wxNewId();
//*)

BEGIN_EVENT_TABLE(PropertiesDlg,wxDialog)
	//(*EventTable(PropertiesDlg)
	//*)
END_EVENT_TABLE()

PropertiesDlg::PropertiesDlg(wxWindow* parent,wxWindowID id)
{
	//(*Initialize(PropertiesDlg)
	Create(parent, wxID_ANY, _("Properties"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
	SetClientSize(wxSize(499,230));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Name :"), wxPoint(16,16), wxSize(40,13), 0, _T("ID_STATICTEXT1"));
	nameLabel = new wxStaticText(this, ID_STATICTEXT2, _("Label"), wxPoint(80,16), wxSize(392,13), 0, _T("ID_STATICTEXT2"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT3, _("Source :"), wxPoint(16,40), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	sourceLabel = new wxStaticText(this, ID_STATICTEXT4, _("Label"), wxPoint(80,40), wxSize(400,13), 0, _T("ID_STATICTEXT4"));
	StaticText3 = new wxStaticText(this, ID_STATICTEXT5, _("Size :"), wxPoint(16,64), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	sizeLabel = new wxStaticText(this, ID_STATICTEXT6, _("Label"), wxPoint(80,64), wxSize(400,13), 0, _T("ID_STATICTEXT6"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT7, _("Compression Ratio :"), wxPoint(16,128), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT8, _("Comment :"), wxPoint(16,88), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT9, _("CRC :"), wxPoint(16,176), wxDefaultSize, 0, _T("ID_STATICTEXT9"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT10, _("System :"), wxPoint(16,200), wxDefaultSize, 0, _T("ID_STATICTEXT10"));
	StaticText8 = new wxStaticText(this, ID_STATICTEXT11, _("Compression Method :"), wxPoint(16,152), wxDefaultSize, 0, _T("ID_STATICTEXT11"));
	StaticText9 = new wxStaticText(this, ID_STATICTEXT12, _("8-bit text :"), wxPoint(248,200), wxDefaultSize, 0, _T("ID_STATICTEXT12"));
	StaticText10 = new wxStaticText(this, ID_STATICTEXT13, _("Extra Info :"), wxPoint(248,176), wxDefaultSize, 0, _T("ID_STATICTEXT13"));
	commentLabel = new wxStaticText(this, ID_STATICTEXT14, _("Label"), wxPoint(80,88), wxSize(392,13), 0, _T("ID_STATICTEXT14"));
	ratioLabel = new wxStaticText(this, ID_STATICTEXT15, _("Label"), wxPoint(152,128), wxSize(216,13), 0, _T("ID_STATICTEXT15"));
	compMethodLabel = new wxStaticText(this, ID_STATICTEXT16, _("Label"), wxPoint(152,152), wxSize(216,13), 0, _T("ID_STATICTEXT16"));
	crcLabel = new wxStaticText(this, ID_STATICTEXT17, _("Label"), wxPoint(80,176), wxSize(136,13), 0, _T("ID_STATICTEXT17"));
	systemLabel = new wxStaticText(this, ID_STATICTEXT18, _("Label"), wxPoint(80,200), wxSize(136,13), 0, _T("ID_STATICTEXT18"));
	isTextlabel = new wxStaticText(this, ID_STATICTEXT19, _("Label"), wxPoint(320,200), wxSize(136,13), 0, _T("ID_STATICTEXT19"));
	extraInfoLabel = new wxStaticText(this, ID_STATICTEXT20, _("Label"), wxPoint(320,176), wxSize(136,13), 0, _T("ID_STATICTEXT20"));
	//*)
}

PropertiesDlg::~PropertiesDlg()
{
	//(*Destroy(PropertiesDlg)
	//*)
}

