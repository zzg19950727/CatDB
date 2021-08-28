include_directories(
    ${PROJECT_SOURCE_DIR}/common 
    ${PROJECT_SOURCE_DIR}/sql
	${PROJECT_SOURCE_DIR}/sql/resolver
)
aux_source_directory(. DIR_RESOVER_SRCS)
add_library(resolver ${DIR_RESOVER_SRCS})
target_link_libraries(resolver parser common)