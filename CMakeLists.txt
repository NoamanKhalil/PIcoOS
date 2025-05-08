cmake_minimum_required(VERSION 3.13)

# Include the RP2350 SDK (path may need adjustment)
set(RP2350_SDK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/rp2350_sdk)
list(APPEND CMAKE_MODULE_PATH ${RP2350_SDK_PATH}/cmake)

# Project name
project(pico_os C CXX ASM)

# Set C and C++ standards
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Add library directories
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/freertos)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/fatfs)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/lvgl)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${RP2350_SDK_PATH}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/freertos/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/fatfs/source
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/lvgl
)

# Add source files
file(GLOB_RECURSE SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

# Compile options for optimization
add_compile_options(-O3 -fdata-sections -ffunction-sections)
add_link_options(-Wl,--gc-sections)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    freertos
    fatfs
    lvgl
    rp2350_sdk
)

# Additional compiler flags for RP2350-specific optimizations
target_compile_options(${PROJECT_NAME} PRIVATE
    -mcpu=cortex-m0plus
    -mthumb
    -Wall
    -Wextra
)

# Create firmware
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${RP2350_SDK_PATH}/tools/elf2uf2 ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.elf ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.uf2
    COMMENT "Creating UF2 file"
)
