# ########## Project setup ##########
PROJECT(client)
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

# ########## client executable ##########
# Sources:
SET(client_executable_SRCS
    client.cpp
)

# Headers:
SET(client_executable_HDRS
)



# actual target:
ADD_EXECUTABLE(client ${client_executable_SRCS})
target_link_libraries(client ${Boost_LIBRARIES})

# add install target:
INSTALL(TARGETS client DESTINATION bin)
