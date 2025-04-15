if(EXODUS_INCLUDE_DIRS AND EXODUS_LIBRARIES)
  set(EXODUS_FIND_QUIETLY TRUE)
endif()

find_path(EXODUS_INCLUDE_DIR NAMES exodusII.h
        PATHS ${EXODUS_INSTALL_DIR}
            $ENV{EXODUS_DIR}
            ${CMAKE_BINARY_DIR}/exodus/install
            /usr/include
            /usr/local/include
        PATH_SUFFIXES include)

set(EXODUS_INCLUDE_DIRS ${EXODUS_INCLUDE_DIR})

find_library(EXODUS_LIBRARY NAMES exodus_for exodus
            PATHS ${EXODUS_INSTALL_DIR}
                   $ENV{EXODUS_DIR}
                   ${CMAKE_BINARY_DIR}/exodus/install
                   /usr/lib
                   /usr/local/lib
             PATH_SUFFIXES lib)

set(EXODUS_LIBRARIES ${EXODUS_LIBRARY})

include(FindPackageHandleStandardArgs)

# handle the QUIETLY and REQUIRED arguments and set EXODUS_FOUND to TRUE if 
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Exodus DEFAULT_MSG EXODUS_LIBRARIES EXODUS_INCLUDE_DIR)

MARK_AS_ADVANCED(EXODUS_INCLUDE_DIRS EXODUS_LIBRARIES)
