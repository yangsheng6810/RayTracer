project(RayTracer)

set(CMAKE_BUILD_TYPE Release)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
cmake_minimum_required(VERSION 2.8)
#aux_source_directory(. SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR} SRC_LIST)

message("PROJECT_SOURCE_DIR points to " ${PROJECT_SOURCE_DIR})
message("SRC_LIST points to " "${SRC_LIST}")

find_package(PNG REQUIRED)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.54.0 COMPONENTS thread filesystem system unit_test_framework REQUIRED)
message(status "** Boost Include: ${Boost_INCLUDE_DIR}")
message(status "** Boost Libraries: ${Boost_LIBRARY_DIRS}")
message(status "** Boost Libraries: ${Boost_LIBRARIES}")
set(Boost_USE_STATIC_LIBS  ON)
find_package(Boost REQUIRED thread system)

add_definitions(-DWITH_SAMPLE_NUMBER=20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME}
            ${CMAKE_THREAD_LIBS_INIT}
            ${PNG_LIBRARIES}
            ${ZLIB_LIBRARIES}
            ${Boost_LIBRARIES}
    )

