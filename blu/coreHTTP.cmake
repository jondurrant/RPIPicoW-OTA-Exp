# Add library cpp files

if (NOT DEFINED COREHTTP_PATH)
    set(COREHTTP_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/coreHTTP")
endif()

include(${COREHTTP_PATH}/httpFilePaths.cmake)



add_library(coreHTTP STATIC)

target_sources(coreHTTP PUBLIC
	${HTTP_SOURCES}
)


# Add include directory
target_include_directories(coreHTTP PUBLIC 
    ${HTTP_INCLUDE_PUBLIC_DIRS}
    ${COREHTTP_PORT}
)

target_link_libraries(coreHTTP PUBLIC pico_stdlib)