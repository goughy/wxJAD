
cmake_minimum_required(VERSION 2.6)

if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

project(wxJAD)

message(STATUS "Processing " ${CMAKE_CURRENT_LIST_FILE})

if(WIN32)
	message(STATUS "Configuring windows search directories...")
	set(wxWidgets_ROOT_DIR "c:/wxWidgets-2.8.10")
	set(wxWidgets_LIB_DIR "${wxWidgets_ROOT_DIR}/lib/vc_lib")
	set(wxWidgets_CONFIGURATION "mswud")
endif(WIN32)

find_package(wxWidgets COMPONENTS base aui core REQUIRED)

if(WIN32)
	FIND_LIBRARY(WX_stc
        NAMES
        wxmsw${UNV}29${UCD}${DBG}_stc
        wxmsw${UNV}28${UCD}${DBG}_stc
        wxmsw${UNV}27${UCD}${DBG}_stc
        wxmsw${UNV}26${UCD}${DBG}_stc
        wxmsw${UNV}25${UCD}${DBG}_stc
        PATHS ${WX_LIB_DIR}
        NO_DEFAULT_PATH
        )
	FIND_PATH(WX_stc_INCLUDE_DIR 
		NAMES wx/stc/stc.h
		PATHS
		${wxWidgets_ROOT_DIR}/contrib/include
	)
	message(STATUS "WX_stc = ${WX_stc}")
	message(STATUS "WX_stc_INCLUDE_DIR = ${WX_stc_INCLUDE_DIR}")
endif(WIN32)

include_directories(${wxWidgets_INCLUDE_DIRS} ${WX_stc_INCLUDE_DIR})

add_definitions(${wxWidgets_DEFINITIONS})
if(UNIX)
	add_definitions(-g -fPIC)
endif(UNIX)

set(SRCS
	wxJADApp.cpp
	wxJADApp.h
	wxJADEvent.h
	wxJADFileMgr.cpp
	wxJADFileMgr.h
	wxJADFrame.cpp
	wxJADFrame.h
	wxJADNotebook.cpp
	wxJADNotebook.h
	wxJADTree.cpp
	wxJADTree.h
	PropertiesDlg.cpp
	PropertiesDlg.h
	wxJADDropTarget.cpp
	wxJADDropTarget.h
	wxJADFile.h
)

if(UNIX)
	add_definitions(-g -fPIC -DENABLE_BINRELOC)
	set(SRCS
		${SRCS}
		binreloc.c
		binreloc.h
	)
endif(UNIX)

add_definitions(${wxWidgets_DEFINITIONS})

add_executable(wxJAD WIN32 ${SRCS})

target_link_libraries(wxJAD ${wxWidgets_LIBRARIES} ${WX_stc})


message(STATUS "Finished " ${CMAKE_CURRENT_LIST_FILE})
