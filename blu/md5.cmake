# Add library cpp files

if (NOT DEFINED MD5_PATH)
    set(MD5_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/md5")
endif()



add_library(md5 STATIC)

target_sources(md5 PUBLIC
    ${MD5_PATH}/md5.cpp
)


# Add the standard library to the build
target_link_libraries(md5 PUBLIC pico_stdlib)


# Add include directory
target_include_directories(md5 INTERFACE 
    ${MD5_PATH}
)
