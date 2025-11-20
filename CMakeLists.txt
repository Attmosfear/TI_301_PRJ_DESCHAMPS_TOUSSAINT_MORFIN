cmake_minimum_required(VERSION 3.10)
project(MarkovGraphs C)

# Configuration du projet
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Options de compilation avec warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Définir les fichiers source
set(SOURCES
    main.c
    utils.c
    hasse.c
    matrix.c
)

set(HEADERS
    utils.h
    hasse.h
    matrix.h
)

# Créer l'exécutable
add_executable(markov ${SOURCES} ${HEADERS})

# Lier la bibliothèque mathématique (nécessaire pour fabsf, etc.)
if(UNIX)
    target_link_libraries(markov m)
endif()

# Configuration pour le mode Debug
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        target_compile_options(markov PRIVATE /DEBUG)
    else()
        target_compile_options(markov PRIVATE -g)
    endif()
endif()

# Afficher les informations de compilation
message(STATUS "Projet: ${PROJECT_NAME}")
message(STATUS "Compilateur C: ${CMAKE_C_COMPILER}")
message(STATUS "Type de build: ${CMAKE_BUILD_TYPE}")
