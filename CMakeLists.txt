# --------------------------- IMPORTANT ----------------------------
# This file is not used to build anything, it just gets HAL and CAL
# file paths and hand them to the extra_build_script.py.
# 
# This file expects LT_CAL CMake variable.
# ------------------------------------------------------------------

cmake_minimum_required(VERSION 3.21.0)
project(HALCALFilesExtractor NONE)

# Select HAL based on LT_PLATFORM (default to arduino).
# Normalize LT_PLATFORM to match on-disk folder names (e.g. rpi_pico -> rpi-pico).
if(NOT LT_PLATFORM)
    set(LT_PLATFORM "arduino")
endif()
string(REPLACE "_" "-" LT_PLATFORM_DIR "${LT_PLATFORM}")

# Function to convert a CMake list into a JSON array string
function(list_to_json_array list_var out_var)
    set(json "[")
    list(LENGTH ${list_var} list_len)
    math(EXPR last_index "${list_len} - 1")

    foreach(index RANGE ${last_index})
        list(GET ${list_var} ${index} item)
        string(APPEND json "\"${item}\"")
        if(NOT index EQUAL last_index)
            string(APPEND json ",")
        endif()
    endforeach()

    string(APPEND json "]")
    set(${out_var} "${json}" PARENT_SCOPE)
endfunction()


# Add the CAL directory
add_subdirectory("libtropic/cal/${LT_CAL}")

# Add the HAL directory
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libtropic/hal/${LT_PLATFORM_DIR}/CMakeLists.txt")
    add_subdirectory("libtropic/hal/${LT_PLATFORM_DIR}")
else()
    message(WARNING "LT_PLATFORM='${LT_PLATFORM}' not found; falling back to HAL 'arduino'")
    add_subdirectory("libtropic/hal/arduino")
endif()

# Convert lists from the CMake variables into JSON lists
list_to_json_array(LT_CAL_SRCS LT_CAL_SRCS_JSON)
list_to_json_array(LT_CAL_INC_DIRS LT_CAL_INC_DIRS_JSON)
list_to_json_array(LT_HAL_SRCS LT_HAL_SRCS_JSON)
list_to_json_array(LT_HAL_INC_DIRS LT_HAL_INC_DIRS_JSON)

# Write JSON file
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/hal_cal_vars.json"
    "{\n"
    "  \"LT_CAL_SRCS\": ${LT_CAL_SRCS_JSON},\n"
    "  \"LT_CAL_INC_DIRS\": ${LT_CAL_INC_DIRS_JSON},\n"
    "  \"LT_HAL_SRCS\": ${LT_HAL_SRCS_JSON},\n"
    "  \"LT_HAL_INC_DIRS\": ${LT_HAL_INC_DIRS_JSON}\n"
    "}\n"
)
