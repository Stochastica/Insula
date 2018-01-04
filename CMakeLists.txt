project(Insula)

cmake_minimum_required(VERSION 3.5)

set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(CMAKE_BINARY_DIR   ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH    ${CMAKE_BINARY_DIR})

find_package(Boost 1.58.0 COMPONENTS
	filesystem
	program_options
	system
	thread
	timer
	REQUIRED)
if (${Boost_FOUND})
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIR})
endif()
find_package(Threads REQUIRED)
find_package(PNG REQUIRED)
if (${PNG_FOUND})
	include_directories(${PNG_INCLUDE_DIRS})
endif()
set(FileSource
	src/main.cpp
	src/fractals.cpp
	src/integration.cpp
	)

add_executable(Insula ${FileSource})
target_link_libraries(Insula ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Insula ${Boost_LIBRARIES})
target_link_libraries(Insula ${PNG_LIBRARIES})
target_compile_features(Insula PRIVATE
	cxx_range_for
	cxx_noexcept
	cxx_rvalue_references
	cxx_override
	cxx_final
	cxx_deleted_functions
	cxx_delegating_constructors
	cxx_auto_type
	cxx_constexpr
	cxx_lambdas
	cxx_relaxed_constexpr
	)

