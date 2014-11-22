PKG_CHECK_MODULES(ZLIB REQUIRED "zlib")

set(core_SOURCES
	xglobal.h
	xunzip.h
	xunzip.cpp
)

add_definitions(-DCORE_LIB)

add_library(xcore SHARED
	${core_SOURCES}
)

target_link_libraries(xcore
	Qt5::Core
	minizip
	${ZLIB_LIBRARIES}
)

install(TARGETS xcore LIBRARY DESTINATION ${libdir})

xmr_add_tests()

