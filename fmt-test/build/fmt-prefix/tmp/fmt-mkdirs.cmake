# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt"
  "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-build"
  "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix"
  "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/tmp"
  "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-stamp"
  "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src"
  "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-stamp${cfgdir}") # cfgdir has leading slash
endif()
