project(buddhabrot-generator C)

set(CMAKE_C_FLAGS "-Wall -Wextra -O3 -std=gnu99 -fopenmp")
#set(CMAKE_C_FLAGS "-Wall -Wextra -O0 -std=gnu99 -g")

add_executable(buddhabrot-generator
    main.c
    pixmap.h pixmap.c
    generator.h generator.c
    )
