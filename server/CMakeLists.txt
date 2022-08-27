include_directories(
    ${PROJECT_SOURCE_DIR}/common 
    ${PROJECT_SOURCE_DIR}/package 
    ${PROJECT_SOURCE_DIR}/sql
    ${PROJECT_SOURCE_DIR}/sql/executor
	${PROJECT_SOURCE_DIR}/sql/resolver
)
aux_source_directory(. DIR_SERVER_SRCS)
add_subdirectory(packet)
add_library(server ${DIR_SERVER_SRCS})
target_link_libraries(server sql common package packet)