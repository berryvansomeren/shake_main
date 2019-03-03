# CMake configuration
set( DESTINATION_FOLDER "C:/Users/Berry/Documents/shake_build" )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DESTINATION_FOLDER} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DESTINATION_FOLDER} )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DESTINATION_FOLDER} )
set( CMAKE_EXECUTABLE_OUTPUT_PATH   ${DESTINATION_FOLDER} )

# this option is necessary to automatically export ALL symbols on Windows, 
# without needing to specify __declspec( export )
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)