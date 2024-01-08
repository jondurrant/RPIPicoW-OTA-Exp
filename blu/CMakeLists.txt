cmake_minimum_required(VERSION 3.12)

# Change your executable name to something creative!
set(NAME blu) # <-- Name your project/executable here!

set(PICO_BOARD pico_w)

include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_CXX_ENABLE_EXCEPTIONS 1)

# Initialize the SDK
pico_sdk_init()

include(lwip_import.cmake)

SET(FREERTOS_KERNEL_PATH "${CMAKE_CURRENT_LIST_DIR}/../lib/FreeRTOS-Kernel" CACHE STRING "Course Common Lib")
SET(FREERTOS_CONFIG_FILE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/port/FreeRTOS-Kernel" CACHE STRING "Local Config")
include(${CMAKE_CURRENT_LIST_DIR}/port/FreeRTOS-Kernel/freertos_config.cmake)
include_directories("${FREERTOS_CONFIG_FILE_DIRECTORY}") 
include(FreeRTOS_Kernel_import.cmake)

set(COREHTTP_PATH "${CMAKE_CURRENT_LIST_DIR}/../lib/coreHTTP" CACHE STRING "Course Common Lib")
set(COREHTTP_PORT "${CMAKE_CURRENT_LIST_DIR}/port/coreHTTP/" CACHE STRING "Course Common Lib")
include(coreHTTP.cmake)

SET(URILIB_PATH "${CMAKE_CURRENT_LIST_DIR}/../lib/uri-library" CACHE STRING "Common Lib")
include(urilib.cmake)

SET(JSON_MAKER_DIR "${CMAKE_CURRENT_LIST_DIR}/../lib/json-maker" CACHE STRING "Common Lib")
include(json-maker.cmake)

SET(TINY_JSON_DIR "${CMAKE_CURRENT_LIST_DIR}/../lib/tiny-json" CACHE STRING "Common Lib")
include(tiny-json.cmake)

set(MD5_PATH "${CMAKE_CURRENT_LIST_DIR}/../lib/md5")
include(md5.cmake)


add_subdirectory(src)

#Set up files for the release packages
install(CODE "execute_process(COMMAND $ENV{HOME}/bin/picoDeploy.sh ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.elf)")

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/src/${NAME}.uf2
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
