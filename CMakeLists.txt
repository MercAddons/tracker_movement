cmake_minimum_required(VERSION 2.8.3)
project(tracker_movement)
find_package(catkin REQUIRED COMPONENTS
          geometry_msgs
					roscpp
					roslib
					tf
                    OpenCV REQUIRED)
find_package(orocos_kdl REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenGL REQUIRED)

#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# Find OpenNI2
#find_package(PkgConfig)
#pkg_check_modules(OpenNI2 REQUIRED libopenni2)
find_path(OpenNI2_INCLUDEDIR
	  NAMES OpenNI.h
	  HINTS /usr/include/openni2)
find_library(OpenNI2_LIBRARIES
	     NAMES OpenNI2 DummyDevice OniFile PS1090
	     HINTS /usr/lib/ /usr/lib/OpenNI2/Drivers
	     PATH_SUFFIXES lib)
message(STATUS ${OpenNI2_LIBRARIES})

# Find Nite2
message(status $ENV{NITE2_INCLUDE})
message(status $ENV{NITE2_REDIST64})
find_path(Nite2_INCLUDEDIR
	  NAMES NiTE.h
	  HINTS $ENV{NITE2_INCLUDE})
find_library(Nite2_LIBRARY
	     NAMES NiTE2
	     HINTS $ENV{NITE2_REDIST64}
	     PATH_SUFFIXES lib)

catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES openni_prueba
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

include_directories(include
        ${catkin_INCLUDEDIR}
		    ${OpenNI2_INCLUDEDIR}
		    ${Nite2_INCLUDEDIR}
            ${OpenCV_INCLUDE_DIRS}
            ${OpenGL_INCLUDE_DIRS}
            ${GLUT_INCLUDE_DIRS})

add_definitions(-DK2_CALIB_PATH="${PROJECT_SOURCE_DIR}/data/")
add_executable(tracker_movement src/tracker_movement.cpp src/Viewer.cpp src/ReferencePoints.cpp src/FuzzySystem.cpp)
target_link_libraries(tracker_movement
  ${catkin_LIBRARIES}
  ${OpenNI2_LIBRARIES}
  ${Nite2_LIBRARY}
  ${OpenCV_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARY}
  fuzzylite
)

install(TARGETS tracker_movement RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
install(FILES tracker_movement.xml DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})
