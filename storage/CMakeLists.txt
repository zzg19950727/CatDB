include_directories(. ${PROJECT_SOURCE_DIR}/common)
aux_source_directory(. DIR_STORAGE_SRCS)
add_library(storage ${DIR_STORAGE_SRCS})