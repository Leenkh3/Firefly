if(EXODUS_INCLUDE_DIRS AND EXODUS_LIBRARIES)
  set(EXODUS_FIND_QUIETLY TRUE)
endif()

find_path(EXODUS_INCLUDE_DIR exodusII.h
          PATHS ${EXODUS_INSTALL_DIR}
                $ENV{EXODUS_DIR}
                ${CMAKE_BINARY_DIR}/exodus/install
                /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/exodus/mpi
                /usr/local/include
                /usr/local
          PATH_SUFFIXES include)

set(EXODUS_INCLUDE_DIRS ${EXODUS_INCLUDE_DIR})

find_library(EXODUS_LIBRARY NAMES exodus
             PATHS ${EXODUS_INSTALL_DIR}
                   $ENV{EXODUS_DIR}
                   ${CMAKE_BINARY_DIR}/exodus/install
                   /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/exodus/mpi
                   /usr/local/lib
                   /usr/local
             PATH_SUFFIXES lib)

set(EXODUS_LIBRARIES ${EXODUS_LIBRARY})

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args(Exodus DEFAULT_MSG EXODUS_INCLUDE_DIRS EXODUS_LIBRARIES)

mark_as_advanced(EXODUS_INCLUDE_DIRS EXODUS_LIBRARIES)