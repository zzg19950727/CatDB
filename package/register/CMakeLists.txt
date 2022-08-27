include_directories(.)
aux_source_directory(. DIR_PACKAGE_REGISTER_SRCS)
add_library(package_register ${DIR_PACKAGE_REGISTER_SRCS})