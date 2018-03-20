cmake_minimum_required(VERSION 3.5)
project(Titler)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_CXX_STANDARD 11)

find_package(wxWidgets 3.1 REQUIRED core base media)
include(${wxWidgets_USE_FILE})

set(SOURCES
	src/Project.cpp
	src/property.cpp
	src/titBlackPanel.cpp
	src/titHandle.cpp
	src/Titler.cpp
	src/titMediaCtrl.cpp
	src/titMenuBar.cpp
	src/titSubtitle.cpp
	src/titTextEntryDialog.cpp
	src/titTimeline.cpp
	src/utils.cpp
)
set(HEADERS
	include/Project.h
	include/property.h
	include/srtparser.h
	include/titBlackPanel.h
	include/titHandle.h
	include/Titler.h
	include/titMediaCtrl.h
	include/titMenuBar.h
	include/titSubtitle.h
	include/titTextEntryDialog.h
	include/titTimeline.h
	include/utils.h
)
set(STUFF ${HEADERS} ${SOURCES} rc/titler.rc)

include_directories(${wxWidgets_INCLUDE_DIRS} "${PROJECT_SOURCE_DIR}/include")

add_executable(TitlerRelease ${STUFF})
target_link_libraries(TitlerRelease ${wxWidgets_LIBRARIES})

add_executable(TitlerDebug ${STUFF})
target_link_libraries(TitlerDebug ${wxWidgets_LIBRARIES})
if(MINGW)
	target_compile_options(TitlerDebug PUBLIC -gdwarf-2)
else()
	target_compile_options(TitlerDebug PUBLIC -g)
endif(MINGW)
