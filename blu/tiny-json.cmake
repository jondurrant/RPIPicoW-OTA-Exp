# Add library cpp files

if (NOT DEFINED TINY_JSON_DIR)
    set(TINY_JSON_DIR "${CMAKE_CURRENT_LIST_DIR}/lib/tiny-json")
endif()

add_library(tiny_json STATIC)
target_sources(tiny_json PUBLIC
    ${TINY_JSON_DIR}/tiny-json.c
)

# Add include directory
target_include_directories(tiny_json PUBLIC 
   ${TINY_JSON_DIR}/
)

# Add the standard library to the build
target_link_libraries(tiny_json PUBLIC pico_stdlib)