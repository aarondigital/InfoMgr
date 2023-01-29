# CMakeList.txt : CMake project for InfoMgr, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (InfoGUI "InfoGUI.cpp" "InfoGUI.h")

# GUI
find_package(imgui CONFIG REQUIRED)
target_link_libraries(InfoGUI PRIVATE imgui::imgui)

# Additional Include Directories
include_directories( ${imgui_INCLUDE_DIRS} )
# Additional Library Directories
link_directories( ${imgui_LIB_DIR} )
# Additional Dependencies
target_link_libraries( InfoMgr PRIVATE ${imgui_LIBS} )