
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

find_package(wxWidgets COMPONENTS base aui core stc REQUIRED)
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

include( ${wxWidgets_USE_FILE} )
#include_directories(${wxWidgets_INCLUDE_DIRS} ${WX_stc_INCLUDE_DIR})

#add_definitions(${wxWidgets_DEFINITIONS})

set(SRCS
	src/wxJADApp.cpp
	src/wxJADApp.h
	src/wxJADEvent.h
	src/wxJADFileMgr.cpp
	src/wxJADFileMgr.h
	src/wxJADFrame.cpp
	src/wxJADFrame.h
	src/wxJADNotebook.cpp
	src/wxJADNotebook.h
	src/wxJADTree.cpp
	src/wxJADTree.h
	src/PropertiesDlg.cpp
	src/PropertiesDlg.h
	src/wxJADDropTarget.cpp
	src/wxJADDropTarget.h
	src/wxJADFile.h
)

if(UNIX)
	add_definitions(-g -fPIC -DENABLE_BINRELOC)
	set(SRCS
		${SRCS}
		src/binreloc.c
		src/binreloc.h
	)
endif(UNIX)

add_executable(wxJAD WIN32 ${SRCS})

target_link_libraries(wxJAD ${wxWidgets_LIBRARIES} ${WX_stc})


message(STATUS "Finished " ${CMAKE_CURRENT_LIST_FILE})
