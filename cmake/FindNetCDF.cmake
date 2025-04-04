if(NETCDF_INCLUDE_DIRS AND NETCDF_LIBRARIES)
  set(NETCDF_FIND_QUIETLY TRUE)
endif()

find_path(NETCDF_INCLUDE_DIR netcdf.h
          PATHS ${NETCDF_INSTALL_DIR}
                $ENV{NETCDF_DIR}
                ${CMAKE_BINARY_DIR}/netcdf/install
                /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/netcdf/mpi
          PATH_SUFFIXES include)

set(NETCDF_INCLUDE_DIRS ${NETCDF_INCLUDE_DIR})

find_library(NETCDF_LIBRARY NAMES netcdf libnetcdf.a
             PATHS ${NETCDF_INSTALL_DIR}
                   $ENV{NETCDF_DIR}
                   ${CMAKE_BINARY_DIR}/netcdf/install
                   /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/netcdf/mpi
             PATH_SUFFIXES lib)

set(NETCDF_LIBRARIES ${NETCDF_LIBRARY})

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args(NetCDF DEFAULT_MSG NETCDF_INCLUDE_DIRS NETCDF_LIBRARIES)

mark_as_advanced(NETCDF_INCLUDE_DIRS NETCDF_LIBRARIES)