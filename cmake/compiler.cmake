
# extra compile definitions after building external targets
add_compile_definitions(
    GLFW_INCLUDE_NONE
    PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}
	SHAKE_EXECUTABLE_NAME=${SHAKE_EXECUTABLE_NAME}
	SHAKE_PYTHON_LIBRARY_NAME=${SHAKE_PYTHON_LIBRARY_NAME}
)