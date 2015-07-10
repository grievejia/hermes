include_directories (${PROJECT_SOURCE_DIR}/include)

set (HermesLibSourceCode
	Parser/TypeBuilder.cc
	Printer/ClassPrinter.cc
	Printer/SpecPrinter.cc
	Printer/UnionPrinter.cc
)

add_library (Hermes STATIC ${HermesLibSourceCode})


set (UtilSourceCode
	Util/CommandLineParser.cpp
	Util/TypedCommandLineParser.cpp
)

add_library (Util ${UtilSourceCode})
