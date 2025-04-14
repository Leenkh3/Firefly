# تحديد الحد الأدنى لإصدار CMake المطلوب
cmake_minimum_required(VERSION 3.15)

# اسم المشروع والإصدار واللغات المستخدمة
project(Firefly VERSION 1.0 LANGUAGES CXX)

# تعيين مسار الوحدات الإضافية
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# إضافة دليل المصدر للمشروع الرئيسي
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# المكتبة للتعامل مع المصفوفات
add_library(MatrixLib
    src/matrix/Dense.cpp
    src/matrix/Matrix.h
    src/matrix/Dense.h
    src/matrix/SparseCSR.h
    src/matrix/SparseCSR.cpp
)

# مكتبة CSR
add_library(CSR src/laplacian/CSR.cpp)

# مكتبة التدرج (Laplacian)
add_library(Laplacian src/laplacian/Laplacian.cpp)

# برامج الاختبار
add_executable(LaplacianTests src/laplacian/testLaplacian.cpp)
target_link_libraries(LaplacianTests PRIVATE CSR Laplacian)

add_executable(MatrixTests src/matrix/test_matrix.cpp)
target_link_libraries(MatrixTests PRIVATE MatrixLib)

# الملف التنفيذي الرئيسي
add_executable(firefly firefly.cpp)

# تمكين بيئة الاختبار
enable_testing()

# إضافة اختبارات إلى المشروع
add_test(NAME firefly COMMAND firefly)
add_test(NAME MatrixTests COMMAND MatrixTests)
add_test(NAME laplacian COMMAND LaplacianTests)

# إضافة وظيفة لاختبارات الانحدار
include(add_regression_test)

# إعداد اختبارات الانحدار مع نتائج النجاح/الفشل المتوقعة
add_regression_test(test_example_pass_regexp firefly
                    EXTRA_PASS_REGEXP "Firefly Project"
                    LABELS "basic")
add_regression_test(test_example_fail_regexp firefly
                    EXTRA_FAIL_REGEXP "Firefly Project"
                    EXTRA_PROPERTIES "WILL_FAIL;1"
                    LABELS "basic")

# إعداد وثائق Doxygen
find_package(Doxygen REQUIRED)

if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/docs)

    add_custom_command(
        OUTPUT ${DOXYGEN_OUT}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_IN}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )

    # تعريف الهدف لتوليد الوثائق
    add_custom_target(doc ALL DEPENDS ${DOXYGEN_OUT})

    # إضافة تبعية الوثائق إلى الهدف الرئيسي (على سبيل المثال firefly)
    add_dependencies(firefly doc)
endif()
