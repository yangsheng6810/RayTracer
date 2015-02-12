cmake_minimum_required(VERSION 2.8)
project(RayTracer)

#set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)
set(MY_SOURCES
		point3.cpp
		vector3.cpp
		color.cpp
        bbox.cpp
        threadpool.h
        constants.h
        sahkdtree.cpp
        triangle.cpp
        light.cpp
        parallel.cpp
        pointlight.cpp
        spotlight.cpp
        rectlight.cpp
		baseobject.cpp
		sphere.cpp
		plane.cpp
		scene.cpp
		ray.cpp
		material.cpp
		shadepacket.cpp
		trianglemesh.cpp
        rectangle.cpp
		output.cpp
        camera.cpp
		tracer.cpp)

	#aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)

#message("PROJECT_SOURCE_DIR points to " ${PROJECT_SOURCE_DIR})
#message("SRC_LIST points to " "${SRC_LIST}")

###################
# add include

find_package(PNG REQUIRED)
FIND_PACKAGE(Boost 1.54.0)
IF(Boost_FOUND)

    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_LIBS  OFF)
    set(Boost_USE_STATIC_RUNTIME OFF)
    find_package(Boost 1.54.0 COMPONENTS thread filesystem system unit_test_framework REQUIRED thread system)

    #find_package(Boost REQUIRED thread system)

    add_definitions(-DWITH_SAMPLE)
	add_library(render SHARED render1.c ${MY_SOURCES})
	target_link_libraries(render
            ${CMAKE_THREAD_LIBS_INIT}
            ${PNG_LIBRARIES}
            ${ZLIB_LIBRARIES}
            ${Boost_LIBRARIES}
    )

    # add_executable(render render1.c) # ${MY_SOURCES})
    # target_link_libraries(render
            # my-library
            # #${CMAKE_THREAD_LIBS_INIT}
    # )

    configure_file(my_try.py my_try.py COPYONLY)
    configure_file(render_engine.py render_engine.py COPYONLY)

ELSEIF(NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR "Unable to find correct Boost version. Did you set BOOST_ROOT?")
ENDIF()



IF(CMAKE_COMPILER_IS_GNUCXX)
  # ADD_DEFINITIONS("-Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address -fno-omit-frame-pointer -ffast-math -std=c++03")
ELSE()
  MESSAGE(FATAL_ERROR "CMakeLists.txt has not been tested/written for your compiler.")
ENDIF()
