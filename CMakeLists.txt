##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3+
##---------------------------------------------------------------------------
 
# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(rtlsdr_pi)
SET(PACKAGE_NAME rtlsdr_pi)
SET(VERBOSE_NAME RTLSDR)
SET(TITLE_NAME RTLSDR)
SET(CPACK_PACKAGE_CONTACT "Sean D'Epagnier ")

MESSAGE (STATUS "*** Building ${PACKAGE_NAME} ***")

SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "0")

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_RTLSDR
            src/rtlsdr_pi.cpp
            src/rtlsdrDialog.cpp
            src/rtlsdrPrefs.cpp
            src/rtlsdrUI.cpp
            src/icons.cpp
	    )

IF(UNIX AND NOT APPLE)
  find_library(RTLSDR_LIBRARY NAMES rtlsdr
    DOC "The rtlsdr library"
    )
  IF(RTLSDR_LIBRARY)

    ADD_DEFINITIONS( "-DBUILTIN_RTLAIS" )
    SET(SRC_AISDECODER
      src/rtl-sdr-misc/rtl_ais.c
      src/rtl-sdr-misc/convenience.c
      src/rtl-sdr-misc/aisdecoder/sounddecoder.c
      src/rtl-sdr-misc/aisdecoder/aisdecoder.c
      src/rtl-sdr-misc/aisdecoder/lib/protodec.c
      src/rtl-sdr-misc/aisdecoder/lib/filter.c
      src/rtl-sdr-misc/aisdecoder/lib/hmalloc.c
      src/rtl-sdr-misc/aisdecoder/lib/receiver.c)

    SET(EXTRA_LIBS ${EXTRA_LIBS} ${RTLSDR_LIBRARY})
  ELSE()
    MESSAGE (STATUS "")
    MESSAGE (STATUS "================ WARNING ===================")
    MESSAGE (STATUS "rtlsdr library not found, no builtin decoder")
    MESSAGE (STATUS "You may want to install librtlsdr-dev and run cmake again")
    MESSAGE (STATUS "============================================")
    MESSAGE (STATUS "")
  ENDIF()
ENDIF(UNIX AND NOT APPLE)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_RTLSDR} ${SRC_AISDECODER})

INCLUDE_DIRECTORIES(${PLUGIN_SOURCE_DIR}/src/libaudiofile)

INCLUDE("cmake/PluginInstall.cmake")

# note, we don't need to include some dll files if they are already included
# with a mingw opencpn, for now, copy them anyway (makes installer large)
IF(WIN32)
  IF(EXISTS ${CMAKE_SOURCE_DIR}/buildwin/bin)
    INSTALL(DIRECTORY buildwin/bin DESTINATION "${INSTALL_DIRECTORY}")
  ENDIF(EXISTS ${CMAKE_SOURCE_DIR}/buildwin/bin)
ENDIF(WIN32)

INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
