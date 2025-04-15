find_path(SEACAS_INCLUDE_DIR
  NAMES
    SEACAS_Config.h
  PATHS
    ENV SEACAS_ROOT
    /usr/local/include
    /opt/seacas/include
    /usr/include
  PATH_SUFFIXES seacas)

find_library(SEACAS_LIBRARY
  NAMES seacas
  PATHS
    ENV SEACAS_ROOT
    /usr/local/lib
    /opt/seacas/lib
    /usr/lib
)

find_library(EXODUS_LIBRARY
  NAMES exodus
  PATHS
    ENV SEACAS_ROOT
    /usr/local/lib
    /opt/seacas/lib
    /usr/lib
)

find_library(NEMESIS_LIBRARY
  NAMES nemesis
  PATHS
    ENV SEACAS_ROOT
    /usr/local/lib
    /opt/seacas/lib
    /usr/lib
)

find_library(NF_LIBRARY
  NAMES netcdf
  PATHS
    ENV NETCDF_ROOT
    /usr/local/lib
    /opt/netcdf/lib
    /usr/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Seacas DEFAULT_MSG
  SEACAS_LIBRARY
  SEACAS_INCLUDE_DIR
)

