#add executable

set ( HLSYN_SOURCES
    main.cpp
    scheduling.hpp
    scheduling.cpp
    graph.hpp
    graph.cpp
    schedule_exception.hpp
    schedule_exception.cpp
    Parser.cpp
    Parser.hpp
    ParserException.cpp
    ParserException.hpp
    Operation.cpp
    Operation.hpp
    IfOperation.cpp
    IfOperation.hpp   
    )

add_executable(hlsyn ${HLSYN_SOURCES})
set_property(TARGET hlsyn PROPERTY CXX_STANDARD 11)

