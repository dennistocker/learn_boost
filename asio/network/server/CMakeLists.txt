# ########## Project setup ##########
PROJECT(server)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)
set(CMAKE_CXX_STANDARD 17)

FIND_PACKAGE(Boost 1.67.0 REQUIRED COMPONENTS system)

if(Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

    MESSAGE(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}")
    MESSAGE(STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}")
    MESSAGE(STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}")
endif()

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## server executable ##########
# Sources:
SET(server_executable_SRCS
    server.cpp
)

# Headers:
SET(server_executable_HDRS
)



# actual target:
ADD_EXECUTABLE(server ${server_executable_SRCS})
target_link_libraries(server ${Boost_LIBRARIES})

# add install target:
INSTALL(TARGETS server DESTINATION bin)
