#add executable

set ( DPGEN_SOURCES
    main.cpp
    graph.hpp
    graph.cpp
    Parser.cpp
    Parser.hpp
    Component.cpp
    Component.hpp
    )

#add_executable(dpgen main.cpp graph.hpp graph.cpp node.hpp node.cpp edge.hpp edge.cpp Parser.cpp Parser.hpp Component.cpp Component.hpp)
add_executable(dpgen ${DPGEN_SOURCES})
set_property(TARGET dpgen PROPERTY CXX_STANDARD 11)

