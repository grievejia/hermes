include_directories (${PROJECT_SOURCE_DIR}/include)

set (HermesSourceFiles
	hermes.cc
)

add_executable(hermes ${HermesSourceFiles}) 
target_link_libraries (hermes HermesLib)