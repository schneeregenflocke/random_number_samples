cmake_minimum_required (VERSION 3.8)

project(random_samples)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "CMAKE_EXPORT_COMPILE_COMMANDS" FORCE)


message("CMAKE_VERSION: ${CMAKE_VERSION}")
message("CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
message("CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")

add_executable(random_samples)


if(${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	target_compile_options(random_samples PRIVATE "-mrdrnd")
endif()

target_compile_definitions(random_samples PRIVATE IMGUI_USER_CONFIG=<ImGui_User_Config.h>)

target_include_directories(random_samples PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)


file(GLOB SRC_FILES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

target_sources(random_samples PUBLIC ${SRC_FILES})


set(Boost_USE_STATIC_LIBS ON)
find_package(Boost)
message("Boost_FOUND: ${Boost_FOUND}")
if(NOT ${Boost_FOUND})
	message(FATAL_ERROR "Set directory path variable Boost_DIR to the directory containing the file BoostConfig.cmake (boost/stage/lib/cmake/Boost-1.74.0). See https://cmake.org/cmake/help/latest/module/FindBoost.html")
endif()

message("Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}") 
message("Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
message("Boost_LIBRARIES: ${Boost_LIBRARIES}")
target_include_directories(random_samples PUBLIC ${Boost_INCLUDE_DIRS})
target_link_directories(random_samples PUBLIC ${Boost_LIBRARY_DIRS})

#find_package(OpenGL)
#target_link_libraries(random_samples PUBLIC ${OPENGL_LIBRARIES})
#target_include_directories(random_samples PUBLIC ${OPENGL_INCLUDE_DIRS})


set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/submodules/glad")
target_link_libraries(random_samples PUBLIC glad)


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/submodules/glfw")
target_link_libraries(random_samples PUBLIC glfw)
#target_include_directories(random_samples PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)


file(GLOB IM_GUI_SRC CONFIGURE_DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/*.h 
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/*.cpp
	)
target_sources(random_samples PUBLIC 
	${IM_GUI_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/examples/imgui_impl_glfw.h
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/examples/imgui_impl_glfw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/examples/imgui_impl_opengl3.h
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/examples/imgui_impl_opengl3.cpp)

target_include_directories(random_samples PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui)
target_include_directories(random_samples PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/examples)




target_include_directories(random_samples PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/submodules/embed-resource")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/submodules/embed-resource")

list(APPEND LICENSE_FILES_PATHS
	"${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt" random_samples
	"${CMAKE_CURRENT_SOURCE_DIR}/submodules/embed-resource/LICENSE.txt" "embed-resource"		 
	"${CMAKE_CURRENT_SOURCE_DIR}/submodules/glad/LICENSE" glad
	"${CMAKE_CURRENT_SOURCE_DIR}/submodules/glfw/LICENSE.md" glfw
	"${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/LICENSE.txt" imgui
)

if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
	list(APPEND LICENSE_FILES_PATHS "${Boost_INCLUDE_DIRS}/LICENSE_1_0.txt" boost)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
	list(APPEND LICENSE_FILES_PATHS "/usr/share/licenses/boost/LICENSE_1_0.txt" boost)
endif()

embed_resources(RESOURCES ${LICENSE_FILES_PATHS})

target_sources(random_samples PRIVATE ${RESOURCES})

