cmake_minimum_required(VERSION 2.8.11)
# Name of the project
set(PROJECT_NAME MassSpring)
project(${PROJECT_NAME})

#Bring the headers into the project (local ones)
include_directories(include $ENV{HOME}/NGL/include ui)

#the file(GLOB...) allows for wildcard additions of our src dir
set(SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp  
			${PROJECT_SOURCE_DIR}/src/MainWindow.cpp  
			${PROJECT_SOURCE_DIR}/src/NGLScene.cpp 
			${PROJECT_SOURCE_DIR}/src/AbstractRK4Integrator.cpp  
			${PROJECT_SOURCE_DIR}/src/RK4Spring.cpp
			${PROJECT_SOURCE_DIR}/include/NGLScene.h 
			${PROJECT_SOURCE_DIR}/include/MainWindow.h 
			${PROJECT_SOURCE_DIR}/include/AbstractRK4Integrator.h 
			${PROJECT_SOURCE_DIR}/include/RK4Spring.h
	)

# NGL also needs the OpenGL framework from Qt so add it Also as this demo uses 
# a UI these modules need to be imported before we can use the qt5_wrap_ui module.
find_package(Qt5OpenGL)
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Core)
qt5_wrap_ui(UIS_HDRS ui/MainWindow.ui)
#This demo uses a UI so we need to add this to run UIC
set(CMAKE_AUTOUIC ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# see what platform we are on and set platform defines
# see what platform we are on and set platform defines
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	add_definitions(-DDARWIN)
	find_library(MACGL OpenGL)
	set ( PROJECT_LINK_LIBS -lNGL ${MACGL})

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	add_definitions(-DLINUX)
	set ( PROJECT_LINK_LIBS -lNGL -lGL)

endif()

#As were using Qt we need to run moc
#disable -rdynamic
SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")


add_definitions(-O2 -D_FILE_OFFSET_BITS=64 -fPIC) 


# now add NGL specific values
link_directories( ~/NGL/lib )
include_directories(~/NGL/include/)
# as NGL uses Qt we need to define this flag
add_definitions(-DQT5BUILD)


# add exe and link libs this must be after the other defines
add_executable(${PROJECT_NAME} ${SOURCES} ${UIS_HDRS})
target_link_libraries(${PROJECT_NAME} ${PROJECT_LINK_LIBS} Qt5::OpenGL Qt5::Core Qt5::Gui Qt5::Widgets )

