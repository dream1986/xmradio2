set(minizip_SOURCES
	ioapi.c
	ioapi.h
	unzip.c
	unzip.h
)

add_library(minizip STATIC
	${minizip_SOURCES}
)

set_target_properties(minizip PROPERTIES
	COMPILE_FLAGS "-fPIC"
)
