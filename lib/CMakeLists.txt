include_directories (${PROJECT_SOURCE_DIR}/include)

set (HermesLibSourceCode
	Parser/ErrorReport.cc
	Parser/TypeBuilder.cc
)

add_library (HermesLib ${HermesLibSourceCode})