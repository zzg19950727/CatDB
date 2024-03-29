cmake_minimum_required(VERSION 2.8.0)
project(CatDB CXX)

#set(CMAKE_CXX_COMPILER "g++")
set (CMAKE_CXX_FLAGS  "-std=c++11 -Wno-deprecated-register")
if (WIN32)
    set (PLATFORM_LIBS Ws2_32 mswsock iphlpapi ntdll)
else (WIN32)
    set (PLATFORM_LIBS tcmalloc pthread ${CMAKE_DL_LIBS})
endif (WIN32)

include_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/object
    ${PROJECT_SOURCE_DIR}/sql
    ${PROJECT_SOURCE_DIR}/sql/parser
    ${PROJECT_SOURCE_DIR}/sql/resolver
	${PROJECT_SOURCE_DIR}/sql/transformer
	${PROJECT_SOURCE_DIR}/sql/optimizer
    ${PROJECT_SOURCE_DIR}/sql/executor
	${PROJECT_SOURCE_DIR}/server
	${PROJECT_SOURCE_DIR}/server/packet
    ${PROJECT_SOURCE_DIR}/storage
    ${PROJECT_SOURCE_DIR}/package
    )

aux_source_directory(. SrcFiles)

add_subdirectory(common)
add_subdirectory(object)
add_subdirectory(package)
add_subdirectory(storage)
add_subdirectory(sql)
add_subdirectory(server)
add_subdirectory(tools/random_test)

add_executable(CatDB ${SrcFiles})
target_link_libraries(CatDB common object package storage sql server ${PLATFORM_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
