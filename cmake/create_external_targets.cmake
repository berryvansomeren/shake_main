#----------------------------------------------------------------
# Create the external targets that our project relies on

# a list of all include directories
set( SHAKE_INCLUDE_DIRECTORIES 
	src/
    external/glm/
	external/glad/include/
	C:/Users/Berry/Documents/shake3/shake_content/src/
	C:/Users/Berry/Documents/shake3/shake_core/src/
	C:/Users/Berry/Documents/shake3/shake_ecs/src/
	C:/Users/Berry/Documents/shake3/shake_graphics/src/
	C:/Users/Berry/Documents/shake3/shake_hid/src/
	C:/Users/Berry/Documents/shake3/shake_io/src/
)

# a list of all external libraries
set( SHAKE_EXTERNAL_LIBRARIES 
	${EXECUTABLE_NAME}
	${GLAD_LIBRARIES}
	shake_content
	shake_core
	shake_ecs
	shake_graphics
	shake_hid
	shake_io
)