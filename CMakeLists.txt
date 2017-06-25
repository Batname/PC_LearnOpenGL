cmake_minimum_required (VERSION 3.4)
cmake_policy(VERSION 3.4)

project (PC_LearnOpenGL)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

link_directories(${CMAKE_SOURCE_DIR}/lib)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")

# Build only for UNIX now
set(CMAKE_CXXFLAGSDEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
find_package(OpenGL REQUIRED)
add_definitions(${OPENGL_DEFINITIONS})
find_package(X11 REQUIRED)
# https://stackoverflow.com/a/33768963/3191141
set(LIBS glfw X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread glfw assimp)

set(CHAPTERS
    1.getting_started
    4.advanced_opengl
)

set(1.getting_started
    1.1.hello_window
)

set(4.advanced_opengl
    6.1.cubemaps_skybox
    6.2.cubemaps_enviroment_mapping
)

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

# first create relevant static libraries requried for other projects
add_library(STB_IMAGE "src/stb_image.cpp")
set_target_properties(STB_IMAGE PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "src/glad.c")
set_target_properties(GLAD PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(LIBS ${LIBS} GLAD)

# then create a project file per tutorial
foreach(CHAPTER ${CHAPTERS})
    foreach(DEMO ${${CHAPTER}})
        file(GLOB SOURCE
            "src/${CHAPTER}/${DEMO}/*.h"
            "src/${CHAPTER}/${DEMO}/*.cpp"
        )
        set(NAME "${CHAPTER}__${DEMO}")
        add_executable(${NAME} ${SOURCE})
        target_link_libraries(${NAME} ${LIBS})
		set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin/${CHAPTER}")
        # copy shader files to build directory
        file(GLOB SHADERS 
                 "src/${CHAPTER}/${DEMO}/*.vs"
                 # "src/${CHAPTER}/${DEMO}/*.frag"
                 "src/${CHAPTER}/${DEMO}/*.fs"
                 "src/${CHAPTER}/${DEMO}/*.gs"
        )
        foreach(SHADER ${SHADERS})
		file(COPY ${SHADER} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/${CHAPTER})			
        endforeach(SHADER)
      # if compiling for visual studio, also use configure file for each project (specifically to set up working directory)
      if(MSVC)
          configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.vcxproj.user @ONLY)
      endif(MSVC)
    endforeach(DEMO)
endforeach(CHAPTER)

include_directories(${CMAKE_SOURCE_DIR}/include)
