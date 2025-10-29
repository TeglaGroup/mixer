cmake_minimum_required(VERSION 3.16)
project(TeglaMixer C)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SNDFILE REQUIRED sndfile)
pkg_check_modules(LIBXMP REQUIRED libxmp)
pkg_check_modules(AO REQUIRED ao)

include_directories(
    include
    ${SNDFILE_INCLUDE_DIRS}
    ${LIBXMP_INCLUDE_DIRS}
    ${AO_INCLUDE_DIRS}
)

set(SRC
    src/mixer.c
    src/file.c
    src/mutex.c
    src/thread.c
    src/ao-backend.c
)

add_library(TeglaMixer SHARED ${SRC})

target_link_directories(TeglaMixer
    PRIVATE
    ${SNDFILE_LIBRARY_DIRS}
    ${LIBXMP_LIBRARY_DIRS}
    ${AO_LIBRARY_DIRS}
)

target_link_libraries(TeglaMixer
    PRIVATE
    ${SNDFILE_LIBRARIES}
    ${LIBXMP_LIBRARIES}
    ${AO_LIBRARIES}

    pthread
)

install(TARGETS TeglaMixer DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)

option(BUILD_EXAMPLES "Build example programs" ON)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
