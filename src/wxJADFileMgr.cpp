#include "wxJADFileMgr.h"

#include <wx/zipstrm.h>
#include <wx/ffile.h>
#include <wx/wfstream.h>
#include <wx/stream.h>
#include <wx/process.h>
#include <wx/log.h>
#include <wx/dir.h>
#include <memory>

#include "binreloc.h"
#include "wxJADApp.h"
#include "wxJADEvent.h"

#define JAD_EXE_NAME	wxT( "jad" )

#ifdef WIN32
#define JAD_EXE_EXT	 wxT( "exe" )
#else
#define JAD_EXE_EXT	 wxT( "" )
#endif

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

void hex_dump( unsigned char * pData, int len, int modulus )
{
    int i = 0;

    for( ; i < len; ++i )
    {
        if( i % modulus == 0 ) //start of lne?
            wprintf( wxT( "\n%08x  " ), i );

        wprintf( wxT( "%02x " ), pData[ i ] );
    }

    while( (i++ % modulus) != 0 )
        wprintf( wxT( "   " ) );

    wprintf( wxT( "\n" ) );
}

// ------------------------------------------------------------------------- //

static int depth = 0;
void dump_recursive( const JadEntry * entry )
{
//    wprintf( wxT( "\nListing %ls\n" ), entry->GetName().c_str() );
    for( JadMap::const_iterator it = entry->GetChildren().begin(); it != entry->GetChildren().end(); it++ )
    {
        if( it->second.GetChildren().size() <= 0 )
			continue;

        for( int i = 0; i < depth; ++i )
            wprintf( wxT( "   " ) );

        wprintf( wxT( "-> %ls\n" ), it->first.c_str() );
        if( it->second.GetChildren().size() > 0 )
        {
			depth++;
			dump_recursive( & it->second );
			depth--;
        }
    }
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

wxFileName wxJADFileMgr::JADExe()
{
    static wxFileName _jadExe;
    if( !_jadExe.IsOk() )
    {
        _jadExe.Assign( wxString::FromUTF8( br_find_exe_dir( NULL ) ), JAD_EXE_NAME, JAD_EXE_EXT );
		wprintf( wxT( "Calculated JAD executable path: %ls\n" ), _jadExe.GetFullPath().c_str() );
    }
    return _jadExe;
}

// ------------------------------------------------------------------------- //

JadEntry * wxJADFileMgr::FindPathRecursive( const JadEntry::JadMap & files,
        const wxString & entryName )
{
    JadMap::const_iterator it = files.find( entryName );
    if( it != files.end() )
        return const_cast<JadEntry *>( & it->second );

    for( it = files.begin(); it != files.end(); it++ )
    {
        if( it->second.GetChildren().size() > 0 )
        {
            JadEntry * e = FindPathRecursive( it->second.GetChildren(), entryName );
            if( e != NULL )
                return e;
        }
    }

    return NULL;
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

JadEntry * JadEntry::AddChild( const wxFileName & fname, EType t )
{
	std::pair<JadMap::iterator, bool> result = children.insert(
													JadMap::value_type( fname.GetFullPath(), JadEntry( fname, t ) ) );
	JadMap::iterator itp = result.first;
	itp->second.Parent( this );
	return & itp->second;
}

// ------------------------------------------------------------------------- //

wxString JadEntry::Name() const
{
	switch( type )
	{
		case JadEntry::CLASS:
		case JadEntry::PLAIN:
		case JadEntry::ZIPCLASS:
		case JadEntry::ZIPPLAIN:
			return path.GetFullName();

		case JadEntry::JAR:
		case JadEntry::ZIP:
		case JadEntry::DIR:
		case JadEntry::UNKNOWN:
		default:
			break;
	}
	return path.GetFullPath();
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

wxJADFileMgr::wxJADFileMgr() : root( wxFileName( wxT( "/" ) ) )
{
    //ctor
}

// ------------------------------------------------------------------------- //

wxJADFileMgr::~wxJADFileMgr()
{
    //dtor
}

// ------------------------------------------------------------------------- //

wxString wxJADFileMgr::GetFileData( JadEntry * pEntry )
{
	switch( pEntry->Type() )
	{
		case JadEntry::PLAIN:
			return ReadPlainFile( pEntry );

		case JadEntry::CLASS:
			return JADPlainClass( pEntry );

		case JadEntry::ZIPCLASS:
			return JADZipClass( pEntry );

		case JadEntry::ZIPPLAIN:
			return ReadPlainZip( pEntry );

		//nothing to do for these types...
		case JadEntry::JAR:
		case JadEntry::ZIP:
		case JadEntry::DIR:
		case JadEntry::UNKNOWN:
		default:
			break;
	}
	return wxT( "" );
}

// ------------------------------------------------------------------------- //

void wxJADFileMgr::Open( const wxString & filename )
{
    wxFileName fname( filename );
//    if ( !fname.FileExists() )
//    {
//        wxLogError( wxT( "Specified file does not exist: %s\n" ), filename.c_str() );
//        return;
//    }

    wxString ext = fname.GetExt().Lower();
    JadEntry * newEntry = NULL;
    if( wxDirExists( fname.GetFullPath() ) )
		newEntry = OpenDir( fname, & root );
    else if ( ext == wxT( "class" ) )
        newEntry = OpenClass( fname, & root );
    else if ( ext == wxT( "jar" ) || ext == wxT( "zip" ) )
        newEntry = OpenJar( fname, & root );
    else
        newEntry = OpenPlain( fname, & root );

	if( newEntry != NULL )
	{
		wxCommandEvent ev( wxEVT_NEW_ENTRY );
		ev.SetClientData( (void *) newEntry );

		::wxPostEvent( wxGetApp().GetTopWindow(), ev );
	}
}

// ------------------------------------------------------------------------- //

JadEntry * wxJADFileMgr::OpenDir( const wxFileName & filename, JadEntry * parent )
{
    wprintf( wxT( "Opening directory: %ls\n" ), filename.GetFullPath().c_str() );
	wxDir dir( filename.GetFullPath() );
	if( !dir.IsOpened() )
		return NULL;

	JadEntry * pDir = AddDir( filename.GetFullPath() );

	wxString currFile;
	bool cont = dir.GetFirst( &currFile, wxEmptyString, wxDIR_FILES );
	while( cont )
	{
		wxFileName fn( filename.GetFullPath(), currFile );
		switch( jad_type( fn ) )
		{
			case JadEntry::ZIP:
			case JadEntry::JAR:
				OpenJar( fn, pDir );
				break;

			case JadEntry::PLAIN:
			case JadEntry::CLASS:
			case JadEntry::ZIPCLASS:
			case JadEntry::ZIPPLAIN:
			case JadEntry::DIR:
			case JadEntry::UNKNOWN:
			default:
				pDir->AddChild( fn.GetFullPath(), jad_type( fn ) );
				break;
		}
		cont = dir.GetNext( &currFile );
	}

	return pDir;
}

// ------------------------------------------------------------------------- //

JadEntry * wxJADFileMgr::OpenPlain( const wxFileName & filename, JadEntry * parent )
{
    wprintf( wxT( "Opening plain file: %ls\n" ), filename.GetFullPath().c_str() );

    JadEntry * pCurrent = AddDir( filename.GetPath() );
    pCurrent->AddChild( filename.GetFullPath(), jad_type( filename ) );

//    dump_recursive( & root );

	return pCurrent;
}

// ------------------------------------------------------------------------- //

JadEntry * wxJADFileMgr::OpenClass( const wxFileName & filename, JadEntry * parent )
{
    return OpenPlain( filename );
}

// ------------------------------------------------------------------------- //

JadEntry * wxJADFileMgr::OpenJar( const wxFileName & filename, JadEntry * parent )
{
    wxFFileInputStream in( filename.GetFullPath() );
    const wxArchiveClassFactory * acf = wxArchiveClassFactory::Find( wxT( "zip" ) );

    if ( acf )
    {
        wxZipInputStream zis( in );
        std::auto_ptr<wxZipEntry> entry;

        // list the contents of the archive
        int i = 0;
        wxFileName tmpFile;
        wxPathList paths;

        // add a JadEntry for the zip file itself...
        JadEntry * zipFile = parent->AddChild( filename, jad_type( filename ) );

        //Parse contents into correct tree structure
		JadEntry * pEntry = NULL;
        while (( entry.reset( zis.GetNextEntry() ) ), entry.get() != NULL )
        {
            //we only add leaf nodes (files) as they contain the dir structure in their name
            // empty directories in a ZIP/JAR cannot be decompiled anyway...
            if( !entry->IsDir() )
            {
                tmpFile.Assign( entry->GetName() );

                //locate an existing, or create a new, entry for the directory component
				JadMap::const_iterator it = zipFile->GetChildren().find( tmpFile.GetPath() );
				if( it != zipFile->GetChildren().end() )
					pEntry = const_cast<JadEntry *>( & it->second );
				else
					pEntry = zipFile->AddChild( tmpFile.GetPath(), JadEntry::DIR );

				//add the file as a leaf...
                pEntry->AddChild( tmpFile.GetFullPath(), jad_type( tmpFile, true ) );
            }
            i++;
        }

        wprintf( wxT("Finished processing %d entries from JAR name: %ls\n"), i, filename.GetFullName().c_str() );
        dump_recursive( & root );
		return zipFile;
    }
    else
    {
        acf = wxArchiveClassFactory::GetFirst();
        while ( acf )
        {
            wprintf( wxT("Supported type: %ls\n"), acf->GetProtocol().c_str() );
            acf = acf->GetNext();
        }

        wxLogError( wxT( "Unsupported file type '%s'\n" ), filename.GetFullName().c_str() );
    }

    return NULL;
}

// ------------------------------------------------------------------------- //
// ------------------------------------------------------------------------- //

wxString wxJADFileMgr::ReadPlainZip( JadEntry * pEntry )
{
	//walk the parent entries to find both full path and the original ZIP file
	JadEntry * zipEntry = pEntry->Parent();
	while( zipEntry != NULL && zipEntry->Type() != JadEntry::ZIP && zipEntry->Type() != JadEntry::JAR )
		zipEntry = zipEntry->Parent();

	if( zipEntry == NULL )
		return wxT( "[Error locating ZIP entry - could not locate parent ZIP file]" );

	size_t len = 0;
	std::auto_ptr<char> buf( GetZipEntry( zipEntry->Path().GetFullPath(),
										pEntry->Path().GetFullPath(), len ) );
	if( buf.get() == NULL )
		return wxT( "[Error reading ZIP entry]" );

	return wxString::FromUTF8( buf.get(), len );
}

// ------------------------------------------------------------------------- //

JadEntry * wxJADFileMgr::AddDir( const wxString & dirName, const JadMap * collection )
{
    JadEntry * pCurrent = FindPathRecursive( collection == NULL ? root.GetChildren() : *collection, dirName );
    if( pCurrent == NULL )
    {
        wprintf( wxT( "Adding directory entry: %ls\n" ), dirName.c_str() );
		pCurrent = root.AddChild( dirName, JadEntry::DIR );
    }

    return pCurrent;
}

// ------------------------------------------------------------------------- //

wxString wxJADFileMgr::ReadPlainFile( JadEntry * pEntry )
{
    wxFFile f( pEntry->Path().GetFullPath() );
    wxString buf;
    f.ReadAll( & buf );
    f.Close();

    return buf;
}

// ------------------------------------------------------------------------- //

wxString wxJADFileMgr::JADPlainClass( JadEntry * pEntry )
{
	return JADClass( pEntry->Path().GetFullPath() );
}

// ------------------------------------------------------------------------- //

wxString wxJADFileMgr::JADZipClass( JadEntry * pEntry )
{
	//walk the parent entries to find both full path and the original ZIP file
	JadEntry * zipEntry = pEntry->Parent();
	while( zipEntry != NULL && zipEntry->Type() != JadEntry::ZIP && zipEntry->Type() != JadEntry::JAR )
		zipEntry = zipEntry->Parent();

	if( zipEntry == NULL )
		return wxT( "[Error locating ZIP entry - could not locate parent ZIP file]" );

	size_t len = 0;
	std::auto_ptr<char> buf( GetZipEntry( zipEntry->Path().GetFullPath(),
										pEntry->Path().GetFullPath(), len ) );
	if( buf.get() == NULL )
		return wxT( "[Error reading ZIP entry]" );

	//write the contents to a temp file...
	wxString tmpName = wxFileName::CreateTempFileName( wxT( "wxJAD" ) );
	wxFile tmpFile( tmpName, wxFile::write );
	wprintf( wxT( "Created temporary file: %ls\n" ), tmpName.c_str() );
	tmpFile.Write( buf.get(), len );
	tmpFile.Close();

	return JADClass( tmpName );
}

// ------------------------------------------------------------------------- //

wxString wxJADFileMgr::JADClass( const wxFileName & filename )
{
    //TODO: JAD settings dialog with configurable options
    wxString cmd( JADExe().GetFullPath() );
    cmd += wxT( " -p -a -af -ff -i " );
    cmd += filename.GetFullPath();

    wxProcess * proc = wxProcess::Open( cmd );
    if( proc == NULL )
		return wxT( "[Error - unable to process class conversion - could not launch JAD]" );

    wxInputStream * pIn  = proc->GetInputStream();
    WXUNUSED( wxInputStream * pErr = proc->GetErrorStream() );

    wxString data;
    char buf[ 4096 ];

    while( !pIn->Eof() )
    {
        pIn->Read( buf, 4096 );
        buf[ pIn->LastRead() ] = '\0';
        data.Append( wxString::FromUTF8( buf ) );
    }

    wprintf( wxT( "PID %d: Read %d bytes from JAD processing of %ls\n" ),
				proc->GetPid(), data.length(), filename.GetFullPath().c_str() );
    return data;
}

// ------------------------------------------------------------------------- //

char * wxJADFileMgr::GetZipEntry( const wxString & zipPath,
								const wxString & entryPath, size_t & len )
{
    wxFFileInputStream in( zipPath );
    const wxArchiveClassFactory * acf = wxArchiveClassFactory::Find( wxT( "zip" ) );

    if( !acf )
		return NULL;

	wxZipInputStream zis( in );
	std::auto_ptr<wxZipEntry> entry;

	wprintf( wxT("Current JAR name: %ls\n"), zipPath.c_str() );

	// list the contents of the archive

	while (( entry.reset( zis.GetNextEntry() ) ), entry.get() != NULL )
	{
		if ( !entry->IsDir() && entry->GetName() == entryPath )
			break;
	}

	if( !entry.get() )
		return NULL;

	//read all data from the input stream 'zis' up to Eof() == true
	std::auto_ptr<char> buf( new char[ entry->GetSize() + 10 ] );
	zis.Read( buf.get(), entry->GetSize() + 10 );
	len = zis.LastRead();

//	wprintf( wxT( "Read %d bytes\n" ), len );
	*(buf.get() + len) = '\0';

	return buf.release();
}
// ------------------------------------------------------------------------- //
