####### EDIT HERE #######
# Define your local Arma 3 server directory - If desired, you can define both Linux and Windows directories for cross-platform development.
set(ARMA3_SERVER_DIR_LINUX "/home/steam/arma3")
set(ARMA3_SERVER_DIR_WINDOWS "X:/Storage/Steam/steamapps/common/Arma 3")
# Python executable
set(PYTHON_WINDOWS "python")
set(PYTHON_LINUX "python3")
####### END EDIT #######



cmake_minimum_required(VERSION 3.8)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

project(arma3-discord-feed)

# Add source to this project's library.
add_library(arma3-discord-feed MODULE "arma3-discord-feed.cpp" "headers/RVExtensionUtil.h"  "classes/Functions.cpp"  "classes/Functions.h"  "classes/Logging.cpp"  "classes/Logging.h")

if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
	message(FATAL_ERROR "Only 64-bit builds are supported")
endif()

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET arma3-discord-feed PROPERTY CXX_STANDARD 20)
endif()

set_target_properties(arma3-discord-feed PROPERTIES OUTPUT_NAME "arma3-discord-feed_x64")
set_target_properties(arma3-discord-feed PROPERTIES CXX_STANDARD 17)

# LibCPR
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8)
FetchContent_MakeAvailable(cpr)
target_link_libraries(arma3-discord-feed PRIVATE cpr::cpr)

# Nlohmann JSON
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)
target_link_libraries(arma3-discord-feed PRIVATE nlohmann_json::nlohmann_json)

# dump all the variables for debugging
get_cmake_property(_variableNames VARIABLES)
list (SORT _variableNames)
foreach (_variableName ${_variableNames})
	message(STATUS "[VARS] ${_variableName}=${${_variableName}}")
endforeach()

# Set the python executable based on the OS
if (WIN32)
  set(PYTHON ${PYTHON_WINDOWS})
else()
  set(PYTHON ${PYTHON_LINUX})
endif()

# if on linux, remove the prefix
if (UNIX)
	set_target_properties(arma3-discord-feed PROPERTIES PREFIX "")
endif()

add_custom_command(TARGET arma3-discord-feed POST_BUILD COMMAND ${PYTHON} ${CMAKE_SOURCE_DIR}/scripts/build.py ${ARMA3_SERVER_DIR_LINUX} ${ARMA3_SERVER_DIR_WINDOWS} ${arma3-discord-feed_BINARY_DIR} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
