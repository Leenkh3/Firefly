# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-stamp/fmt-gitclone-lastrun.txt" AND EXISTS "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-stamp/fmt-gitinfo.txt" AND
  "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-stamp/fmt-gitclone-lastrun.txt" IS_NEWER_THAN "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-stamp/fmt-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-stamp/fmt-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/cvmfs/software.eessi.io/versions/2023.06/compat/linux/x86_64/usr/bin/git"
            clone --no-checkout --config "advice.detachedHead=false" "https://github.com/fmtlib/fmt.git" "fmt"
    WORKING_DIRECTORY "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/fmtlib/fmt.git'")
endif()

execute_process(
  COMMAND "/cvmfs/software.eessi.io/versions/2023.06/compat/linux/x86_64/usr/bin/git"
          checkout "10.1.1" --
  WORKING_DIRECTORY "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: '10.1.1'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/cvmfs/software.eessi.io/versions/2023.06/compat/linux/x86_64/usr/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-stamp/fmt-gitinfo.txt" "/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-stamp/fmt-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/work/students/hpc-bvrhib/firefly/fmt-test/build/fmt-prefix/src/fmt-stamp/fmt-gitclone-lastrun.txt'")
endif()
