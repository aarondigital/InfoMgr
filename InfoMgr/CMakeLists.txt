# CMakeList.txt : CMake project for InfoMgr, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (InfoMgr "InfoMgr.cpp" "InfoMgr.h" "ImageProc.cpp" "ImageProc.h" "DB.h" "DB.cpp")

add_executable (Tasker "TaskMgr.cpp" "TaskMgr.h" "DB.h" "DB.cpp")

add_executable (InfoGUI "InfoGUI.cpp" "InfoGUI.h" "DB.h" "DB.cpp")

set_property(TARGET InfoMgr PROPERTY CXX_STANDARD 17)
set_property(TARGET InfoMgr PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET Tasker PROPERTY CXX_STANDARD 17)
set_property(TARGET Tasker PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET InfoGUI PROPERTY CXX_STANDARD 17)
set_property(TARGET InfoGUI PROPERTY CXX_STANDARD_REQUIRED ON)

# Postgres libraries
find_package(libpqxx CONFIG REQUIRED)
target_link_libraries( InfoMgr PRIVATE libpqxx::pqxx )
target_link_libraries( Tasker PRIVATE libpqxx::pqxx )
target_link_libraries( InfoGUI PRIVATE libpqxx::pqxx )

# OpenCV
find_package(OpenCV REQUIRED)
# Additional Include Directories
include_directories( ${OpenCV_INCLUDE_DIRS} )
# Additional Library Directories
link_directories( ${OpenCV_LIB_DIR} )
# Additional Dependencies
target_link_libraries( InfoMgr PRIVATE ${OpenCV_LIBS} )

# GUI
find_package(imgui CONFIG REQUIRED)
find_package(SFML CONFIG REQUIRED COMPONENTS graphics system window)
find_package(ImGui-SFML CONFIG REQUIRED)
target_link_libraries(InfoGUI PRIVATE imgui::imgui)
target_link_libraries(main PRIVATE
        imgui::imgui
        sfml-graphics sfml-system sfml-window
        ImGui-SFML::ImGui-SFML
    )