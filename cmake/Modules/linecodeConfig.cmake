INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_LINECODE linecode)

FIND_PATH(
    LINECODE_INCLUDE_DIRS
    NAMES linecode/api.h
    HINTS $ENV{LINECODE_DIR}/include
        ${PC_LINECODE_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    LINECODE_LIBRARIES
    NAMES gnuradio-linecode
    HINTS $ENV{LINECODE_DIR}/lib
        ${PC_LINECODE_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(LINECODE DEFAULT_MSG LINECODE_LIBRARIES LINECODE_INCLUDE_DIRS)
MARK_AS_ADVANCED(LINECODE_LIBRARIES LINECODE_INCLUDE_DIRS)

