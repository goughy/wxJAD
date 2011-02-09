
#ifndef _JAD_ENTRY_H
#define _JAD_ENTRY_H

#include <wx/treectrl.h>

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

class JadEntry
{
public:

    typedef std::map<wxString, JadEntry> JadMap;
    enum EType { UNKNOWN, ZIP, JAR, DIR, ZIPCLASS, ZIPPLAIN, CLASS, PLAIN };

public:

    JadEntry( const wxFileName & p, EType t = UNKNOWN ) :
		parent( NULL ),
		path( p ),
		type( t ),
		notebookPage( -1 ) {}

	JadEntry *		   Parent() const			{ return parent; }
	void 			   Parent( JadEntry * p ) 	{ parent = p; }

	const wxFileName & Path() const   		{ return path; }
	EType 			   Type() const			{ return type; }
	wxTreeItemId & 	   TreeItem()			{ return treeItem; }
	int & 			   NotebookPage()		{ return notebookPage; }
	const JadMap & 	   GetChildren() const 	{ return children; }

	wxString   Name() const;
	JadEntry * AddChild( const wxFileName & fname, EType t );

private:

	JadEntry * 		parent;
	wxFileName		path;
	EType			type;
	wxTreeItemId 	treeItem;
	int				notebookPage;
    JadMap 			children;
};

typedef JadEntry::JadMap JadMap;

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

inline JadEntry::EType jad_type( const wxFileName & fn, bool is_zip = false )
{
    wxString ext = fn.GetExt().Lower();
    if ( ext == wxT( "class" ) )
        return is_zip ? JadEntry::ZIPCLASS : JadEntry::CLASS;

    if ( ext == wxT( "jar" ) )
        return JadEntry::JAR;

	if( ext == wxT( "zip" ) )
        return JadEntry::ZIP;

     return is_zip ? JadEntry::ZIPPLAIN: JadEntry::PLAIN;
}

inline wxString jad_str( JadEntry::EType t )
{
	switch( t )
	{
		case JadEntry::ZIP:
			return wxT( "ZIP" );

		case JadEntry::JAR:
			return wxT( "JAR" );

		case JadEntry::DIR:
			return wxT( "DIR" );

		case JadEntry::CLASS:
			return wxT( "CLASS" );

		case JadEntry::PLAIN:
			return wxT( "PLAIN" );

		case JadEntry::ZIPCLASS:
			return wxT( "ZIP/CLASS" );

		case JadEntry::ZIPPLAIN:
			return wxT( "ZIP/PLAIN" );

		case JadEntry::UNKNOWN:
		default:
			break;
	}
	return wxT( "UNKNOWN" );
}


#endif
