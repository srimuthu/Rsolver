set (INTERFACES
	  "IRsolverUtils.h"
	  "IRsolverVision.h"
	  "IRsolverControl.h"
	  "IRsolverSerial.h")

set (MOCK_FILES
	"IRsolverSerialMock.h")

set (SOURCES
	  "RsolverUtils.cpp"
	  "RsolverHelpers.cpp"
	  "RsolverVision.cpp"
	  "ThistlethwaiteSolver.cpp"
	  "RsolverControl.cpp"
	  "RsolverSerial.cpp")

set (HEADERS
	  "RsolverUtils.h"
	  "RsolverTypes.h"
	  "RsolverHelpers.h"
	  "RsolverVision.h"
	  "ThistlethwaiteSolver.h"
	  "RsolverControl.h"
	  "RsolverSerial.h")

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("interfaces" FILES ${INTERFACES})
source_group("sources" FILES ${SOURCES})
source_group("headers" FILES ${HEADERS})
source_group("mocks" FILES ${MOCK_FILES})

# Properties->C/C++->General->Additional Include Directories
include_directories (.)

# The following step has already been performed for UNIX from top level Cmakelists
if (MSVC)
	set(OpenCV_INCLUDE_DIRS
	"D:/Program Files/opencv310/opencv/build/include")
	set(OpenCV_LIBS
	"D:/Program Files/opencv310/opencv/build/x64/vc14/lib/opencv_world310d.lib")
	set(BOOST_INCLUDE_DIRS
	"D:/sb/libraries/boost_1_69_0")
	set(BOOST_LIBS
	"D:/sb/libraries/boost_1_69_0/stage/lib/libboost_date_time-vc141-mt-gd-x64-1_69.lib"
	"D:/sb/libraries/boost_1_69_0/stage/lib/libboost_regex-vc141-mt-gd-x64-1_69.lib"
	)
	message(STATUS "    libraries: ${OpenCV_LIBS}")
	message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
	include_directories(${OpenCV_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS})
endif (MSVC)

# Also adds sources to the Solution Explorer
add_library(rsolverutils STATIC ${INTERFACES} ${SOURCES} ${HEADERS} ${MOCK_FILES})

# Creates a folder "libraries" and adds target project (math.vcproj) under it
set_property(TARGET rsolverutils PROPERTY FOLDER "libraries")

# Properties->General->Output Directory
set_target_properties(rsolverutils PROPERTIES
					  RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

target_link_libraries(rsolverutils ${OpenCV_LIBS} ${BOOST_LIBS})
# Adds logic to INSTALL.vcproj to copy math.dll to the destination directory
# install (TARGETS utils
#         RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/_install
#         LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/_install)
add_subdirectory (test)
