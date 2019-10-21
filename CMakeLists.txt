cmake_minimum_required(VERSION 3.2)
 
project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
# project(allocator VERSION 0.0.1)

find_package(GTest REQUIRED)

configure_file(version.h.in version.h)

add_executable(allocator main.cpp)
add_library(vers lib.cpp)
add_executable(test_factor test_factor.cpp)


set_target_properties(allocator vers test_factor PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(vers
    PRIVATE "${CMAKE_BINARY_DIR}"
)
target_link_libraries(
    test_factor ${GTEST_LIBRARIES} gtest_main Threads::Threads
)

target_link_libraries(allocator PRIVATE
    vers
)

if (MSVC)
    target_compile_options(allocator PRIVATE
        /W4
    )
    target_compile_options(vers PRIVATE
        /W4
    )
    target_compile_options(test_factor PRIVATE
        /W4
    )
else ()
    target_compile_options(allocator PRIVATE
        -Wall -Wextra -pedantic -g3 -Wpointer-arith
    )
    target_compile_options(vers PRIVATE
        -Wall -Wextra -pedantic -g3 -Wpointer-arith
    )
    target_compile_options(test_factor PRIVATE
        -Wall -Wextra -pedantic -g3 -Wpointer-arith
    )
endif()



install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_factor test_factor)


