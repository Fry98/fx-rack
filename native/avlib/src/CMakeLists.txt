SET (IIMA_LINK )
SET (IIMA_LIBS )
SET (IIMA_INCLUDE )

add_definitions(-DNAPI_VERSION=5)
add_definitions(-DNAPI_EXPERIMENTAL)

execute_process(COMMAND node -p "require('node-addon-api').include"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE NODE_ADDON_API_DIR
)
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

SET (IIMA_SRC Utils.cpp AudioTypes.cpp AudioFilter.cpp AudioSink.cpp
				WaveFile.cpp WaveSource.cpp WaveSink.cpp
				filters/SineMultiply.cpp filters/NullFilter.cpp
				filters/SimpleEchoFilter.cpp
				video_ops.cpp


				artnet/Socket.cpp
				artnet/DatagramSocket.cpp
				artnet/ARTNet.cpp



				../include/iimavlib.h ../include/iimavlib/Utils.h ../include/iimavlib/AudioTypes.h
				../include/iimavlib/AudioFilter.h ../include/iimavlib/AudioSink.h
				../include/iimavlib/WaveFile.h ../include/iimavlib/WaveSource.h ../include/iimavlib/WaveSink.h
				../include/iimavlib/filters/SineMultiply.h ../include/iimavlib/filters/NullFilter.h
				../include/iimavlib/filters/SimpleEchoFilter.h
				../include/iimavlib/video_types.h ../include/iimavlib/video_ops.h
				../include/iimavlib/artnet/ARTNet.h
				../include/iimavlib/artnet/DatagramSocket.h
				../include/iimavlib/artnet/Socket.h
				../include/iimavlib/ArrayTypes.h ../include/iimavlib/FFT.h ../include/iimavlib/AudioFFT.h
				)
IF (UNIX)
SET(IIMA_SRC ${IIMA_SRC} AlsaDevice.cpp AlsaSink.cpp AlsaSource.cpp
				../include/iimavlib/AlsaDevice.h ../include/iimavlib/AlsaSink.h ../include/iimavlib/AlsaSource.h)
SET(IIMA_LIBS ${IIMA_LIBS} asound)
ELSEIF(WIN32)
SET(IIMA_SRC ${IIMA_SRC} WinMMDevice.cpp WinMMSink.cpp WinMMSource.cpp
		../include/iimavlib/WinMMDevice.h ../include/iimavlib/WinMMSink.h ../include/iimavlib/WinMMSource.h)
ELSEIF(CYGWIN)
SET(IIMA_SRC ${IIMA_SRC} WinMMDevice.cpp WinMMSink.cpp WinMMSource.cpp
		../include/iimavlib/WinMMDevice.h ../include/iimavlib/WinMMSink.h ../include/iimavlib/WinMMSource.h)
ELSE()
MESSAGE("Unknown platform")
SET(IIMA_SRC ${IIMA_SRC} WinMMDevice.cpp WinMMSink.cpp WinMMSource.cpp
		../include/iimavlib/WinMMDevice.h ../include/iimavlib/WinMMSink.h ../include/iimavlib/WinMMSource.h)
ENDIF()

IF (SDL_FOUND)
	SET(IIMA_SRC ${IIMA_SRC} SDLDevice.cpp ../include/iimavlib/SDLDevice.h)
	SET(IIMA_LIBS ${IIMA_LIBS} ${SDL_LIBRARY})
	SET(IIMA_INCLUDE ${IIMA_INCLUDE} ${SDL_INCLUDE_DIR})
ENDIF()

include_directories(${CMAKE_JS_INC})
add_library(iimav ${IIMA_SRC} ${CMAKE_JS_SRC})
link_directories ( ${IIMA_LINK} )
include_directories ( ${IIMA_INCLUDE} )
target_include_directories(iimav PRIVATE ${NODE_ADDON_API_DIR})
target_link_libraries ( iimav ${IIMA_LIBS} ${CMAKE_JS_LIB})

SET_TARGET_PROPERTIES( iimav PROPERTIES SOVERSION 1 VERSION 1.1.0 )
install(TARGETS iimav 		RUNTIME DESTINATION bin
							LIBRARY DESTINATION lib
							ARCHIVE DESTINATION lib)
