#ifndef WXJADFILEMGR_H
#define WXJADFILEMGR_H

#include <wx/treectrl.h>
#include <wx/filename.h>
#include <map>

#include "wxJADEntry.h"

#define UNPARENTED wxT( "_unparented_" )

// All files are added as follows:
//		- 	for a plain file or class file, a JadEntry for the directory
//			contains a child for the filename
//		- 	for a jar/zip file, a JadEntry parent contains a set of
//			directories, each containing a set of children entries
//

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

class wxJADFileMgr
{
public:

    static wxFileName JADExe();

public:

    wxJADFileMgr();
    virtual ~wxJADFileMgr();

    void Open( const wxString & filename );
    wxString GetFileData( JadEntry * pEntry );

private:

    static JadEntry * FindPathRecursive( const JadEntry::JadMap & files,
										 const wxString & entryName );

    JadEntry * OpenDir( const wxFileName & filename, JadEntry * parent = NULL );
    JadEntry * OpenClass( const wxFileName & filename, JadEntry * parent = NULL );
    JadEntry * OpenPlain( const wxFileName & filename, JadEntry * parent = NULL );
    JadEntry * OpenJar( const wxFileName & filename, JadEntry * parent = NULL );

    JadEntry * AddDir( const wxString & dirName, const JadMap * collection = NULL );

	wxString ReadPlainFile( JadEntry * pEntry );
	wxString ReadPlainZip( JadEntry * pEntry );
	wxString JADPlainClass( JadEntry * pEntry );
	wxString JADZipClass( JadEntry * pEntry );

    wxString JADClass( const wxFileName & filename );

	char * GetZipEntry( const wxString & zipPath, const wxString & entryPath, size_t & len );

private:

    JadEntry root;
};

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

#endif // WXJADFILEMGR_H
