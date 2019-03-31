set (INTERFACES
	  "IRsolverUtils.h"
	  "IRsolverVision.h")

set (SOURCES
      "RsolverUtils.cpp"
	  "RsolverHelpers.cpp"
	  "RsolverVision.cpp"
	  "ThistlethwaiteSolver.cpp")

set (HEADERS
      "RsolverUtils.h"
	  "RsolverTypes.h"
	  "RsolverHelpers.h"
	  "RsolverVision.h"
	  "ThistlethwaiteSolver.h"
	  )

# Create named folders for the sources within the .vcproj
# Empty name lists them directly under the .vcproj
source_group("interfaces" FILES ${INTERFACES})
source_group("sources" FILES ${SOURCES})
source_group("headers" FILES ${HEADERS})

# Properties->C/C++->General->Additional Include Directories
include_directories (.)

# The following step has already been performed for UNIX from top level Cmakelists
if (MSVC)
	set(OpenCV_INCLUDE_DIRS 
	"D:/Program Files/opencv310/opencv/build/include")
	set(OpenCV_LIBS 
	"D:/Program Files/opencv310/opencv/build/x64/vc14/lib/opencv_world310d.lib")
	message(STATUS "    libraries: ${OpenCV_LIBS}")
	message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
	include_directories(${OpenCV_INCLUDE_DIRS})
endif (MSVC)

# Also adds sources to the Solution Explorer
add_library(rsolverutils STATIC ${INTERFACES} ${SOURCES} ${HEADERS})

# Creates a folder "libraries" and adds target project (math.vcproj) under it
set_property(TARGET rsolverutils PROPERTY FOLDER "libraries")

# Properties->General->Output Directory
set_target_properties(rsolverutils PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

target_link_libraries(rsolverutils ${OpenCV_LIBS})
# Adds logic to INSTALL.vcproj to copy math.dll to the destination directory
# install (TARGETS utils
#         RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/_install
#         LIBRARY DESTINATION ${PROJECT_SOURCE_DIR}/_install)
add_subdirectory (test)
