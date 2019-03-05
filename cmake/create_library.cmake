add_library( 
    ${PROJECT_NAME} 
    SHARED
    ${SHAKE_ALL_SOURCE_FILES}
)

set_target_properties( ${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 )

add_executable( shake ${SHAKE_ALL_SOURCE_FILES} )
set_target_properties( shake PROPERTIES CXX_STANDARD 17 )

include(GenerateExportHeader)
generate_export_header( ${PROJECT_NAME} )

set_target_properties(
    ${PROJECT_NAME}  
    PROPERTIES VERSION 
    ${PROJECT_VERSION})