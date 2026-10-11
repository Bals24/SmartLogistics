cmake_minimum_required(VERSION 3.16)

project(SmartLogistics CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ============================================================
# Dossiers du projet
# ============================================================

include_directories(${CMAKE_SOURCE_DIR}/include)

# ============================================================
# Threads
# ============================================================

find_package(Threads REQUIRED)

# ============================================================
# Qt 6
# ============================================================

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

# ============================================================
# PostgreSQL 18
# ============================================================

# Installation standard de PostgreSQL sous Windows
set(POSTGRESQL_ROOT "C:/Program Files/PostgreSQL/18")

if(NOT EXISTS "${POSTGRESQL_ROOT}")
    message(FATAL_ERROR
        "PostgreSQL 18 introuvable dans : ${POSTGRESQL_ROOT}"
    )
endif()

# Dossier des headers PostgreSQL
set(POSTGRESQL_INCLUDE_DIR
    "${POSTGRESQL_ROOT}/include"
)

# Dossier contenant libpq.dll
set(POSTGRESQL_BIN_DIR
    "${POSTGRESQL_ROOT}/bin"
)

# ============================================================
# Import library compatible avec le MinGW de Qt
# ============================================================

# IMPORTANT :
# On n'utilise PAS :
#     ${POSTGRESQL_ROOT}/lib/libpq.a
#
# car cette bibliothèque statique provoque des erreurs de
# linkage avec le MinGW utilisé par Qt.
#
# On utilise à la place une import library MinGW générée
# depuis libpq.dll :
#
#     third_party/postgresql/libpq.dll.a

set(POSTGRESQL_MINGW_LIBRARY
    "${CMAKE_SOURCE_DIR}/third_party/postgresql/libpq.dll.a"
)

if(NOT EXISTS "${POSTGRESQL_MINGW_LIBRARY}")
    message(FATAL_ERROR
        "Import library PostgreSQL compatible MinGW introuvable : "
        "${POSTGRESQL_MINGW_LIBRARY}"
    )
endif()

# Vérification des chemins
message(STATUS "PostgreSQL include : ${POSTGRESQL_INCLUDE_DIR}")
message(STATUS "PostgreSQL bin     : ${POSTGRESQL_BIN_DIR}")
message(STATUS "PostgreSQL MinGW   : ${POSTGRESQL_MINGW_LIBRARY}")

# ============================================================
# Sources C++ du projet
# ============================================================

file(GLOB_RECURSE SL_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)

# ============================================================
# Application console
# ============================================================

add_executable(smart_logistics
    main.cpp
    ${SL_SOURCES}
)

target_include_directories(smart_logistics
    PRIVATE
    ${POSTGRESQL_INCLUDE_DIR}
)

target_link_libraries(smart_logistics
    PRIVATE
    Threads::Threads
    ${POSTGRESQL_MINGW_LIBRARY}
)

# ============================================================
# Application GUI Qt
# ============================================================

add_executable(smart_logistics_gui
    gui/main_gui.cpp
    gui/MainWindow.cpp
    gui/MainWindow.hpp
    gui/Style.hpp
    ${SL_SOURCES}
)

target_include_directories(smart_logistics_gui
    PRIVATE
    ${POSTGRESQL_INCLUDE_DIR}
)

target_link_libraries(smart_logistics_gui
    PRIVATE
    Threads::Threads
    Qt6::Widgets
    ${POSTGRESQL_MINGW_LIBRARY}
)

# ============================================================
# Test PostgreSQL
# ============================================================

add_executable(postgresql_test
    tests/postgresql_test.cpp
    src/database/Database.cpp
)

target_include_directories(postgresql_test
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${POSTGRESQL_INCLUDE_DIR}
)

target_link_libraries(postgresql_test
    PRIVATE
    ${POSTGRESQL_MINGW_LIBRARY}
)

# ============================================================
# Tests existants
# ============================================================

if(EXISTS "${CMAKE_SOURCE_DIR}/tests/test_main.cpp")

    add_executable(smart_logistics_tests
        tests/test_main.cpp
        ${SL_SOURCES}
    )

    target_include_directories(smart_logistics_tests
        PRIVATE
        ${POSTGRESQL_INCLUDE_DIR}
    )

    target_link_libraries(smart_logistics_tests
        PRIVATE
        Threads::Threads
        ${POSTGRESQL_MINGW_LIBRARY}
    )

endif()

# ============================================================
# Copie automatique de libpq.dll
# ============================================================

# Le fichier libpq.dll doit être disponible à côté des
# exécutables au moment de leur lancement.

add_custom_command(
    TARGET smart_logistics
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${POSTGRESQL_BIN_DIR}/libpq.dll"
        "$<TARGET_FILE_DIR:smart_logistics>/libpq.dll"
)

add_custom_command(
    TARGET smart_logistics_gui
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${POSTGRESQL_BIN_DIR}/libpq.dll"
        "$<TARGET_FILE_DIR:smart_logistics_gui>/libpq.dll"
)

add_custom_command(
    TARGET postgresql_test
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${POSTGRESQL_BIN_DIR}/libpq.dll"
        "$<TARGET_FILE_DIR:postgresql_test>/libpq.dll"
)

if(TARGET smart_logistics_tests)

    add_custom_command(
        TARGET smart_logistics_tests
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${POSTGRESQL_BIN_DIR}/libpq.dll"
            "$<TARGET_FILE_DIR:smart_logistics_tests>/libpq.dll"
    )

endif()