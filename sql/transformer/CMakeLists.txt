include_directories(
	${PROJECT_SOURCE_DIR}/sql/resolver
    ${PROJECT_SOURCE_DIR}/common
)
aux_source_directory(. DIR_TRANSFORMER_SRCS)
add_library(transformer ${DIR_TRANSFORMER_SRCS})
target_link_libraries(transformer resolver common)