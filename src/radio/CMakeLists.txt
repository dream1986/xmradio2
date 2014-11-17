set(xradio_SOURCES
	xradio.cpp
	xradiofactory.cpp
)

set(xradio_HEADERS
	xradio.h
	xradioservice.h
	xradioserviceplugin.h
	xradiofactory.h
	xradiostyle.h
	xmusicinfo.h
)

add_definitions(-DRADIO_LIB)
add_library(xradio SHARED
	${xradio_SOURCES}
	${xradio_HEADERS}
)

target_link_libraries(xradio
	Qt5::Core
)

install(TARGETS xradio LIBRARY DESTINATION ${libdir})

######################################################################
MACRO(output_radio _target)
	set_target_properties(${_target} PROPERTIES
		LIBRARY_OUTPUT_DIRECTORY ${XMR_OUTPUT_PATH}/radio
	)
ENDMACRO()

set(radio_plugins
	xiami
)

foreach(plugin ${radio_plugins})
	add_subdirectory(${plugin})
	output_radio(${plugin})
endforeach()

xmr_add_tests()
