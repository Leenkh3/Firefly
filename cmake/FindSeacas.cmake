if(SEACAS_INCLUDE_DIRS AND SEACAS_LIBRARIES)
  set(SEACAS_FIND_QUIETLY TRUE)
endif()

find_path(SEACAS_INCLUDE_DIR exodus_config.h
          PATHS ${SEACAS_INSTALL_DIR}
                $ENV{SEACAS_DIR}
                ${CMAKE_BINARY_DIR}/seacas/install
                /usr/include/seacas
                /usr/local/include/seacas
          PATH_SUFFIXES include)

set(SEACAS_INCLUDE_DIRS ${SEACAS_INCLUDE_DIR})

find_library(SEACAS_LIBRARY NAMES seacas libseacas
             PATHS ${SEACAS_INSTALL_DIR}
                   $ENV{SEACAS_DIR}
                   ${CMAKE_BINARY_DIR}/seacas/install
                   /usr/lib
                   /usr/local/lib
             PATH_SUFFIXES lib)

set(SEACAS_LIBRARIES ${SEACAS_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Seacas DEFAULT_MSG SEACAS_INCLUDE_DIRS SEACAS_LIBRARIES)

mark_as_advanced(SEACAS_INCLUDE_DIRS SEACAS_LIBRARIES)
