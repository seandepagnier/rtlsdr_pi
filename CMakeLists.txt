##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3+
##---------------------------------------------------------------------------
 
# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(rtlsdr_pi)
SET(PACKAGE_NAME rtlsdr_pi)
SET(CPACK_PACKAGE_CONTACT "Sean D'Epagnier ")

MESSAGE (STATUS "*** Building ${PACKAGE_NAME} ***")

SET(VERSION_MAJOR "0")
SET(VERSION_MINOR "9")

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_RTLSDR
            src/rtlsdr_pi.cpp
            src/rtlsdrDialog.cpp
            src/rtlsdrPrefs.cpp
            src/rtlsdrUI.cpp
            src/icons.cpp
	)
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_RTLSDR})

INCLUDE_DIRECTORIES(${PLUGIN_SOURCE_DIR}/src/libaudiofile)

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
