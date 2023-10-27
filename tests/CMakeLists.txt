# unit tests for cloxpp c++ classes and modules

include_directories(../bytecode_vm)
add_executable(HASH_TABLE_TEST hash_table/hash_table_test.cpp)

add_executable(STATIC_ARRAY_TEST static_array_test.cpp)

add_executable(OBJECT_MEMORY_TEST
        memory/object_tests.cpp
        ../bytecode_vm/object.cpp
        ../bytecode_vm/memory/memory.cpp)

add_executable(object_info
        memory/object_info.cpp
        ../bytecode_vm/object.cpp
        ../bytecode_vm/memory/memory.cpp)

add_executable(ALLOCATOR_TEST memory/allocator_tests.cpp)