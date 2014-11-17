set(test_xunzip_SOURCES
	test_xunzip.cpp
)

add_executable(test_xunzip
	${test_xunzip_SOURCES}
)

target_link_libraries(test_xunzip
	xcore
	Qt5::Core
	Qt5::Test
)

