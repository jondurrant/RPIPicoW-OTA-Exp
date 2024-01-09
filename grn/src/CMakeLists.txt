include(../../blu/src/files.cmake)
add_executable(${NAME}
        ${SRC_FILES}
)

# Pull in our pico_stdlib which pulls in commonly used features
target_link_libraries(${NAME} 
	 pico_stdlib
     FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
     freertos_config #FREERTOS_PORT
     LWIP_PORT
     pico_cyw43_arch_lwip_sys_freertos
     coreHTTP
     urilib
     json_maker
     tiny_json
     md5
     pico_fota_bootloader_lib
	)
	
target_include_directories(${NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
     )
     
if (NOT DEFINED ENV{WIFI_SSID})
	message(FATAL_ERROR "WIFI_SSID Environment variable not defined")
endif()
if (NOT DEFINED ENV{WIFI_PASSWORD})
	message(FATAL_ERROR "WIFI_PASSWORD Environment variable not defined")
endif()


target_compile_definitions(${NAME} PRIVATE
    WIFI_SSID=\"$ENV{WIFI_SSID}\"
    WIFI_PASSWORD=\"$ENV{WIFI_PASSWORD}\"
    CYW43_HOST_NAME="DrJonEA"
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

# enable usb output, disable uart output
pico_enable_stdio_usb(${NAME} 0)
pico_enable_stdio_uart(${NAME} 1)
target_compile_definitions(${NAME} PRIVATE
    PICO_DEFAULT_UART_RX_PIN=16
    PICO_DEFAULT_UART_TX_PIN=17
    RELEASE="GRN"
    OTA_URL="http://vmu22a.local.jondurrant.com:5000/blu"
    LED_GP=14
)

target_link_options(${NAME} PRIVATE 
    "-L${CMAKE_CURRENT_SOURCE_DIR}/../pico_fota_bootloader/linker_common"
    )
    
pico_set_linker_script(${NAME} 
    ${CMAKE_CURRENT_SOURCE_DIR}/../pico_fota_bootloader/linker_common/application.ld
    )
