cmake_minimum_required(VERSION 3.0.0)
project(CatDB VERSION 0.1.0)

#set(CMAKE_CXX_COMPILER "g++")
set (CMAKE_CXX_FLAGS  "-std=c++11")

include_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/sql
    ${PROJECT_SOURCE_DIR}/storage
    ${PROJECT_SOURCE_DIR}/parser
    ${PROJECT_SOURCE_DIR}/parser/lex_yacc
	${PROJECT_SOURCE_DIR}/plan
	${PROJECT_SOURCE_DIR}/optimizer
    )

aux_source_directory(. SrcFiles)

add_subdirectory(common)
add_subdirectory(storage)
add_subdirectory(sql)
add_subdirectory(parser)
add_subdirectory(plan)
add_subdirectory(optimizer)

add_executable(CatDB ${SrcFiles})
target_link_libraries(CatDB common storage sql parser plan optimizer)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
