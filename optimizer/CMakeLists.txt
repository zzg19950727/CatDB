include_directories(
    . 
	${PROJECT_SOURCE_DIR}/sql 
    ${PROJECT_SOURCE_DIR}/common
    )
aux_source_directory(. DIR_SQL_SRCS)
add_library(optimizer ${DIR_SQL_SRCS})
target_link_libraries(optimizer sql common)