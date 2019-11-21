include_directories(
    . 
    ${PROJECT_SOURCE_DIR}/common
    )
aux_source_directory(. DIR_PACKET_SRCS)
add_library(packet ${DIR_PACKET_SRCS})
target_link_libraries(packet common)