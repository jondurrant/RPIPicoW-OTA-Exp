# Add library cpp files

if (NOT DEFINED URILIB_PATH)
    set(URILIB_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/uri-library")
endif()



add_library(urilib INTERFACE)


# Add include directory
target_include_directories(urilib INTERFACE 
    ${URILIB_PATH}
)
