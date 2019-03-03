#----------------------------------------------------------------
# Gather all files for the project

# Gather external headers we want to include in our own source
file( GLOB
    SHAKE_EXTERNAL_SOURCES
    external/glad/src/glad.c
    external/json11/json11.cpp
)

# Gather files related to headers of the source
file( GLOB_RECURSE
    PROJECT_HEADERS
    src/*.hpp
    src/*.h
    src/*.inl
    src/*.txt
)

# Gather files related to source files of the source
file( GLOB_RECURSE
    SHAKE_PROJECT_SOURCES
    src/*.cpp
    src/*.cxx
    src/*.cc
    src/*.c
)

# Gather files related to configuration
file (GLOB
    SHAKE_PROJECT_CONFIGS
    CMakeLists.txt
    README.rst
    .gitignore
    .gitmodules
)

# List of all files to include in the project
set( SHAKE_ALL_SOURCE_FILES
    ${SHAKE_PROJECT_HEADERS}
    ${SHAKE_PROJECT_SOURCES}
    ${SHAKE_EXTERNAL_SOURCES}
    ${SHAKE_PROJECT_CONTENT}
    ${SHAKE_PROJECT_CONFIGS}
)

# For windows we need to specify the source structure explicitly
# We want it to be the same as the file structure 
if( WIN32 )
    source_group( TREE ${CMAKE_SOURCE_DIR} FILES ${SHAKE_ALL_SOURCE_FILES} )
endif()