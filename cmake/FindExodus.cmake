# FindExodus.cmake
# Module to locate the ExodusII library (from SEACAS)

# The following variables will be set:
#   Exodus_FOUND         - True if Exodus was found
#   Exodus_INCLUDE_DIRS  - The include directories for Exodus
#   Exodus_LIBRARIES     - The Exodus library

find_path(Exodus_INCLUDE_DIR
    NAMES exodusII.h
    PATHS
        ${NETCDF_INSTALL_DIR}
        $ENV{NETCDF_DIR}
        ${EXODUS_ROOT}/include
        /usr/include
        /usr/local/include
        /opt/local/include
        ENV CPATH
)

find_library(Exodus_LIBRARY
    NAMES exodus exoIIv2c
    PATHS
        ${NETCDF_INSTALL_DIR}
        $ENV{NETCDF_DIR}
        ${EXODUS_ROOT}/lib
        /usr/lib
        /usr/local/lib
        /opt/local/lib
        ENV LIBRARY_PATH
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Exodus
    REQUIRED_VARS Exodus_LIBRARY Exodus_INCLUDE_DIR
    VERSION_VAR Exodus_VERSION
)

if(Exodus_FOUND)
    set(Exodus_INCLUDE_DIRS ${Exodus_INCLUDE_DIR})
    set(Exodus_LIBRARIES ${Exodus_LIBRARY})
endif()

mark_as_advanced(Exodus_INCLUDE_DIR Exodus_LIBRARY)
