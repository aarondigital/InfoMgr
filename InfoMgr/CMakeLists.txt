# CMakeList.txt : CMake project for InfoMgr, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (InfoMgr "InfoMgr.cpp" "InfoMgr.h" "ImageProc.cpp" "ImageProc.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET InfoMgr PROPERTY CXX_STANDARD 17)
  set_property(TARGET InfoMgr PROPERTY CXX_STANDARD_REQUIRED ON)
endif()


# TODO: Add tests and install targets if needed.

find_package(libpqxx CONFIG REQUIRED)
target_link_libraries( InfoMgr PRIVATE libpqxx::pqxx )

#set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv2")
find_package(OpenCV REQUIRED)

# Additional Include Directories
include_directories( ${OpenCV_INCLUDE_DIRS} )

# Additional Library Directories
link_directories( ${OpenCV_LIB_DIR} )

# Additional Dependencies
target_link_libraries( InfoMgr PRIVATE ${OpenCV_LIBS} )
