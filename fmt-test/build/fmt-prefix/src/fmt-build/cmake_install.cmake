# Install script for directory: /work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/cvmfs/software.eessi.io/versions/2023.06/compat/linux/x86_64/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64" TYPE STATIC_LIBRARY FILES "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-build/libfmt.a")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/fmt" TYPE FILE FILES
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/args.h"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/chrono.h"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/color.h"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/compile.h"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/core.h"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/format.h"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/format-inl.h"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/os.h"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/ostream.h"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/printf.h"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/ranges.h"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/std.h"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt/include/fmt/xchar.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/fmt" TYPE FILE FILES
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-build/fmt-config.cmake"
    "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-build/fmt-config-version.cmake"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/fmt/fmt-targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/fmt/fmt-targets.cmake"
         "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-build/CMakeFiles/Export/52567436e6aeb216c623c7cea7a08046/fmt-targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/fmt/fmt-targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib64/cmake/fmt/fmt-targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/fmt" TYPE FILE FILES "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-build/CMakeFiles/Export/52567436e6aeb216c623c7cea7a08046/fmt-targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/cmake/fmt" TYPE FILE FILES "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-build/CMakeFiles/Export/52567436e6aeb216c623c7cea7a08046/fmt-targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib64/pkgconfig" TYPE FILE FILES "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-build/fmt.pc")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
