# ########## Project setup ##########
PROJECT(network)
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.5)

# ######### General setup ##########
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})

# ########## network executable ##########
# Sources:
SET(network_executable_SRCS
    client.cpp
)

# Headers:
SET(network_executable_HDRS
)



# actual target:
ADD_EXECUTABLE(network ${network_executable_SRCS})

# add install target:
INSTALL(TARGETS network DESTINATION bin)
