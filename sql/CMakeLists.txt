aux_source_directory(. DIR_SQL_SRCS)
add_subdirectory(parser)
add_subdirectory(resolver)
add_subdirectory(transformer)
add_subdirectory(optimizer)
add_subdirectory(executor)
add_library(sql ${DIR_SQL_SRCS})
target_link_libraries(sql parser resolver transformer optimizer executor common)