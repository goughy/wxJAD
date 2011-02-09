#ifndef PROPERTIESDLG_H
#define PROPERTIESDLG_H

//(*Headers(PropertiesDlg)
#include <wx/dialog.h>
#include <wx/stattext.h>
//*)

class PropertiesDlg: public wxDialog
{
	public:

		PropertiesDlg(wxWindow* parent,wxWindowID id=wxID_ANY);
		virtual ~PropertiesDlg();

		//(*Declarations(PropertiesDlg)
		wxStaticText* crcLabel;
		wxStaticText* ratioLabel;
		wxStaticText* isTextlabel;
		wxStaticText* StaticText1;
		wxStaticText* StaticText10;
		wxStaticText* StaticText3;
		wxStaticText* sizeLabel;
		wxStaticText* systemLabel;
		wxStaticText* StaticText8;
		wxStaticText* sourceLabel;
		wxStaticText* StaticText7;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText2;
		wxStaticText* compMethodLabel;
		wxStaticText* StaticText6;
		wxStaticText* nameLabel;
		wxStaticText* extraInfoLabel;
		wxStaticText* commentLabel;
		wxStaticText* StaticText9;
		//*)

	protected:

		//(*Identifiers(PropertiesDlg)
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT9;
		static const long ID_STATICTEXT10;
		static const long ID_STATICTEXT11;
		static const long ID_STATICTEXT12;
		static const long ID_STATICTEXT13;
		static const long ID_STATICTEXT14;
		static const long ID_STATICTEXT15;
		static const long ID_STATICTEXT16;
		static const long ID_STATICTEXT17;
		static const long ID_STATICTEXT18;
		static const long ID_STATICTEXT19;
		static const long ID_STATICTEXT20;
		//*)

	private:

		//(*Handlers(PropertiesDlg)
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
