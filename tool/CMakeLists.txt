include_directories (${PROJECT_SOURCE_DIR}/include)

set (HermesSourceFiles
	CommandLineOptions.cc
	hermes.cc
)

add_executable(hermes ${HermesSourceFiles}) 
target_link_libraries (hermes Hermes Util)