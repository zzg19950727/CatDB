include_directories(.)
aux_source_directory(. DIR_OBJECT_SRCS)
add_library(object ${DIR_OBJECT_SRCS})