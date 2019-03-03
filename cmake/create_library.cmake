add_library( 
    ${PROJECT_NAME} 
    SHARED
    ${SHAKE_ALL_SOURCE_FILES}
)

add_executable( shake ${SHAKE_ALL_SOURCE_FILES} )

include(GenerateExportHeader)
generate_export_header( ${PROJECT_NAME} )

set_target_properties(
    ${PROJECT_NAME}  
    PROPERTIES VERSION 
    ${PROJECT_VERSION})