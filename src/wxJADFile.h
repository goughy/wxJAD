
#ifndef WXJADFILE_H
#define WXJADFILE_H

#include <wx/treectrl.h>

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

class wxJADFile : public wxTreeItemData
{
public:

	enum EType { NONE, CLASS, ZIP, PLAIN, DIRECTORY };

public:

	wxJADFile() :
		m_parent( 0 ),
		m_type( NONE ), 
		m_data( NULL )  {}

	wxJADFile( const wxFileName & fname, wxJADFile * parent = 0 ) : 
		m_parent( parent ),
		m_filename( fname ), 
		m_type( NONE ), 
		m_data( NULL )  {}

	virtual ~wxJADFile() {}

	wxJADFile *  Parent()   { return m_parent; }
	wxFileName & Filename() { return m_filename; }
	EType      & Type()  { return m_type;   }
	void      *& Data()  { return m_data;   }

private:

	wxJADFile * m_parent;
	wxFileName  m_filename;
	EType  m_type;
	void * m_data;
};

WX_DEFINE_ARRAY_PTR(wxJADFile *, wxJADFileArray);

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

#endif // WXJADFILE_H
