#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "04";
	static const char MONTH[] = "05";
	static const char YEAR[] = "2010";
	static const char UBUNTU_VERSION_STYLE[] = "10.05";
	
	//Software Status
	static const char STATUS[] = "Alpha";
	static const char STATUS_SHORT[] = "a";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 1;
	static const long BUILD = 163;
	static const long REVISION = 932;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 196;
	#define RC_FILEVERSION 1,1,163,932
	#define RC_FILEVERSION_STRING "1, 1, 163, 932\0"
	static const char FULLVERSION_STRING[] = "1.1.163.932";
	
	//SVN Version
	static const char SVN_REVISION[] = "709";
	static const char SVN_DATE[] = "2009-11-25T10:43:18.841456Z";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 63;
	

}
#endif //VERSION_H
