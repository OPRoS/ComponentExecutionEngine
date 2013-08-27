set (MY_COMPONENT_NAME HelloMaker)
cmake_minimum_required(VERSION 2.6)
PROJECT(${MY_COMPONENT_NAME})

FILE(GLOB_RECURSE SRCS src/*.cpp src/*.h)


SET(OPROS_HOME "D:/workspace/OPRoS_I1/OPRoSEngine/ComponentEngine/PublicVersion/export_Abalone/opros_Abalone_1.01a")

SET(CMAKE_INSTALL_PREFIX "d:/out/install")

INCLUDE_DIRECTORIES(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        ${OPROS_HOME}/include
        ${MY_INCLUDE_DIRS}
)

LINK_DIRECTORIES(
${OPROS_HOME}/bin
)

ADD_LIBRARY(${MY_COMPONENT_NAME} SHARED ${SRCS})

ADD_DEFINITIONS(-DEXECUTOR_USE_ADAPTOR -DNO_VERIFY_COMPONENT -DNO_BOOST -DMAKE_COMPONENT_DLL)

TARGET_LINK_LIBRARIES(${MY_COMPONENT_NAME} OPRoSCDL)