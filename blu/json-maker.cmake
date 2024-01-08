# Add library cpp files

if (NOT DEFINED JSON_MAKER_DIR)
    set(JSON_MAKER_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/json-maker")
endif()

add_library(json_maker STATIC)
target_sources(json_maker PUBLIC
    ${JSON_MAKER_DIR}/src/json-maker.c
)

# Add include directory
target_include_directories(json_maker PUBLIC 
    ${JSON_MAKER_DIR}/src/include
)

# Add the standard library to the build
target_link_libraries(json_maker PUBLIC pico_stdlib)