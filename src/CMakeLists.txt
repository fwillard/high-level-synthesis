#add executable

set ( HLSYN_SOURCES
    main.cpp
    )

add_executable(hlsyn ${HLSYN_SOURCES})
set_property(TARGET hlsyn PROPERTY CXX_STANDARD 11)

