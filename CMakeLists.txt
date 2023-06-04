cmake_minimum_required(VERSION 3.10)
project(data_splitter VERSION 0.1)
set(CMAKE_CXX_STANDARD 20)
find_package(Boost
	REQUIRED
	COMPONENTS program_options)
add_executable(data_splitter
	main.cc)

target_link_libraries(data_splitter Boost::program_options)

install(TARGETS data_splitter DESTINATION bin)
