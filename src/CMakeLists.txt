#add executable

set ( HLSYN_SOURCES
    main.cpp
    scheduling.hpp
    scheduling.cpp
    graph.hpp
    graph.cpp
    )

add_executable(hlsyn ${HLSYN_SOURCES})
set_property(TARGET hlsyn PROPERTY CXX_STANDARD 11)

