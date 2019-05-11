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
