include_directories(
    . 
    ${PROJECT_SOURCE_DIR}/common 
    ${PROJECT_SOURCE_DIR}/plan
	${PROJECT_SOURCE_DIR}/parser
    )
aux_source_directory(. DIR_SERVER_SRCS)
add_subdirectory(packet)
add_library(server ${DIR_SERVER_SRCS})
target_link_libraries(server plan common packet)