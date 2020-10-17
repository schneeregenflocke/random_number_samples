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

#target_compile_definitions(random_samples PRIVATE IMGUI_USER_CONFIG=<ImGui_User_Config.h>)

target_include_directories(random_samples PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)


file(GLOB SRC_FILES CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.h 
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

target_sources(random_samples PUBLIC ${SRC_FILES})

### boost ###

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

### glad ###

set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/submodules/glad")
target_link_libraries(random_samples PUBLIC glad)


### glm ###

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/submodules/glm")
target_link_libraries(random_samples PUBLIC glm)


### glfw ###

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/submodules/glfw")
target_link_libraries(random_samples PUBLIC glfw)
#target_include_directories(random_samples PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)


### imgui ###

target_sources(random_samples PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/imgui.h 
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/imgui.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/imgui_demo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/imgui_draw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/imgui_internal.h
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/imgui_widgets.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/imstb_rectpack.h
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/imstb_textedit.h
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/imstb_truetype.h

	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/backends/imgui_impl_glfw.h
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/backends/imgui_impl_glfw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/backends/imgui_impl_opengl3.h
	${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/backends/imgui_impl_opengl3.cpp
	)

target_include_directories(random_samples PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui)
target_include_directories(random_samples PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/backends)


### licenses ###

target_include_directories(random_samples PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/submodules/embed-resource")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/submodules/embed-resource")

list(APPEND LICENSE_FILES_PATHS
	"${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.txt" random_samples
	"${CMAKE_CURRENT_SOURCE_DIR}/submodules/glad/LICENSE" glad
	"${CMAKE_CURRENT_SOURCE_DIR}/submodules/glfw/LICENSE.md" glfw
	"${CMAKE_CURRENT_SOURCE_DIR}/submodules/imgui/LICENSE.txt" imgui
	"${CMAKE_CURRENT_SOURCE_DIR}/submodules/glm/copying.txt" glm
	"${CMAKE_CURRENT_SOURCE_DIR}/submodules/embed-resource/LICENSE.txt" "embed-resource"
)

if(${CMAKE_SYSTEM_NAME} STREQUAL Windows)
	list(APPEND LICENSE_FILES_PATHS "${Boost_INCLUDE_DIRS}/LICENSE_1_0.txt" boost)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
	list(APPEND LICENSE_FILES_PATHS "/usr/share/licenses/boost/LICENSE_1_0.txt" boost)
endif()

embed_resources(RESOURCES ${LICENSE_FILES_PATHS})

target_sources(random_samples PRIVATE ${RESOURCES})

