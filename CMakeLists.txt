CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(VideoObjectExtractor)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include/)

#CONFIGURE_FILE(VOE.pc.in ${CMAKE_BINARY_DIR}/VOE.pc @ONLY)
#INSTALL(FILES ${CMAKE_BINARY_DIR}/VOE.pc DESTINATION /usr/lib/pkgconfig)

FILE(GLOB SRCS ${CMAKE_SOURCE_DIR}/src/*.hpp
               ${CMAKE_SOURCE_DIR}/src/*.cpp)

include(FindPkgConfig)
pkg_check_modules(VOE REQUIRED opencv)

FOREACH(flag ${VOE_CFLAGS})
  SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

# Set the compiler flags for compiling C sources
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS} -std=gnu99 -O1 -g")

# Set the compiler flags for compiling C++ sources
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CFLAGS} -std=c++0x -O1 -g")

SET(EXTRA_LIBS ${EXTRA_LIBS} ${VOE_LDFLAGS})

ADD_EXECUTABLE(VOE ${SRCS})

TARGET_LINK_LIBRARIES(VOE ${EXTRA_LIBS})

MESSAGE("${CMAKE_CXX_FLAGS} ${EXTRA_CFLAGS} ${EXTRA_LIBS}")
