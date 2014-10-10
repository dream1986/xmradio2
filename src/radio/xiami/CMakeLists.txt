set(xiami_SOURCES
	xxiamiradio.h
	xxiamiradio.cpp
	xxiamiradioserviceplugin.cpp
)

add_library(xiami SHARED
	${xiami_SOURCES}
)

target_link_libraries(xiami
	Qt5::Core
	Qt5::Network
	Qt5::Xml
)

install(TARGETS xiami LIBRARY DESTINATION ${radiodir})
