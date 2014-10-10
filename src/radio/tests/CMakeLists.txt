set(testradio_SOURCES
	testradio.cpp
)

add_executable(testradio
	${testradio_SOURCES}
)

target_link_libraries(testradio
	xradio
)
