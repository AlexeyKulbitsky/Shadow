set(MINIZIP_SOURCES
	unzip.c
	ioapi.c)

add_definitions(-DSHADOW_USE_VULKAN_API)

add_library(minizip ${MINIZIP_SOURCES})