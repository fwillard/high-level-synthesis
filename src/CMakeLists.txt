#add executable

set ( DPGEN_SOURCES
    main.cpp
    )

#add_executable(dpgen main.cpp graph.hpp graph.cpp node.hpp node.cpp edge.hpp edge.cpp Parser.cpp Parser.hpp)
add_executable(test_parse ${DPGEN_SOURCES})

