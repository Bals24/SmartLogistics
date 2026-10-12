cmake_minimum_required(VERSION 3.16)

project(SmartLogistics CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# Options
# ============================================================
# Par defaut tout est active, mais si Qt ou PostgreSQL sont
# introuvables le projet compile quand meme la version console
# et les tests (au lieu de s'arreter avec une erreur fatale).

option(SL_WITH_POSTGRES "Activer le support PostgreSQL (libpq)" ON)
option(SL_WITH_GUI      "Compiler l'interface graphique Qt"      ON)

# ============================================================
# Threads
# ============================================================

find_package(Threads REQUIRED)

# ============================================================
# PostgreSQL (libpq)
# ============================================================
# Tout le code qui parle a PostgreSQL doit rester dans
#   include/database/  et  src/database/
# car ce dossier est exclu automatiquement de la compilation
# quand PostgreSQL n'est pas disponible.

set(SL_HAS_POSTGRES OFF)
set(SL_PG_INCLUDE_DIR "")
set(SL_PG_LIBRARY "")
set(SL_PG_DLL "")

if(SL_WITH_POSTGRES)
    if(WIN32)
        # Installation standard de PostgreSQL sous Windows.
        # Modifiable : cmake -DPOSTGRESQL_ROOT="D:/PostgreSQL/18" ...
        set(POSTGRESQL_ROOT "C:/Program Files/PostgreSQL/18"
            CACHE PATH "Dossier d'installation de PostgreSQL")

        if(EXISTS "${POSTGRESQL_ROOT}/include/libpq-fe.h")
            set(SL_PG_INCLUDE_DIR "${POSTGRESQL_ROOT}/include")
            set(SL_PG_DLL         "${POSTGRESQL_ROOT}/bin/libpq.dll")

            if(MINGW)
                # Import library compatible MinGW (la libpq.a officielle
                # provoque des erreurs de linkage avec le MinGW de Qt).
                # 1) un fichier deja fourni dans third_party/ est utilise tel quel
                # 2) sinon il est genere depuis libpq.def avec dlltool
                set(_sl_implib "${CMAKE_SOURCE_DIR}/third_party/postgresql/libpq.dll.a")

                if(NOT EXISTS "${_sl_implib}")
                    set(_sl_implib "${CMAKE_BINARY_DIR}/libpq.dll.a")

                    if(NOT EXISTS "${_sl_implib}")
                        get_filename_component(_sl_cxx_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
                        find_program(SL_DLLTOOL
                            NAMES dlltool x86_64-w64-mingw32-dlltool
                            HINTS "${_sl_cxx_dir}")

                        if(SL_DLLTOOL)
                            message(STATUS "Generation de libpq.dll.a avec dlltool")
                            execute_process(
                                COMMAND "${SL_DLLTOOL}"
                                        -d "${CMAKE_SOURCE_DIR}/third_party/postgresql/libpq.def"
                                        -l "${_sl_implib}"
                                RESULT_VARIABLE _sl_dlltool_result)
                            if(NOT _sl_dlltool_result EQUAL 0)
                                message(WARNING "dlltool a echoue : PostgreSQL desactive")
                                set(_sl_implib "")
                            endif()
                        else()
                            message(WARNING "dlltool introuvable : PostgreSQL desactive")
                            set(_sl_implib "")
                        endif()
                    endif()
                endif()
                set(SL_PG_LIBRARY "${_sl_implib}")
            else()
                # MSVC : la bibliotheque d'import officielle convient.
                set(SL_PG_LIBRARY "${POSTGRESQL_ROOT}/lib/libpq.lib")
            endif()
        else()
            message(WARNING "PostgreSQL introuvable dans ${POSTGRESQL_ROOT} : support PostgreSQL desactive")
        endif()

        if(SL_PG_LIBRARY AND SL_PG_INCLUDE_DIR)
            set(SL_HAS_POSTGRES ON)
        endif()
    else()
        # Linux / macOS : sudo apt install libpq-dev
        find_package(PostgreSQL)
        if(PostgreSQL_FOUND)
            set(SL_PG_INCLUDE_DIR "${PostgreSQL_INCLUDE_DIRS}")
            set(SL_PG_LIBRARY     "${PostgreSQL_LIBRARIES}")
            set(SL_HAS_POSTGRES ON)
        else()
            message(WARNING "libpq introuvable : support PostgreSQL desactive")
        endif()
    endif()
endif()

if(SL_HAS_POSTGRES)
    message(STATUS "PostgreSQL : ACTIVE  (include : ${SL_PG_INCLUDE_DIR})")
else()
    message(STATUS "PostgreSQL : desactive")
endif()

# ============================================================
# Noyau du projet (compile UNE seule fois, partage par toutes
# les applications : console, GUI, tests)
# ============================================================

file(GLOB_RECURSE SL_CORE_SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Le code PostgreSQL est traite a part.
list(FILTER SL_CORE_SOURCES EXCLUDE REGEX "/src/database/")

add_library(sl_core STATIC ${SL_CORE_SOURCES})

target_include_directories(sl_core PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(sl_core PUBLIC Threads::Threads)

if(SL_HAS_POSTGRES)
    file(GLOB_RECURSE SL_DB_SOURCES CONFIGURE_DEPENDS src/database/*.cpp)
    target_sources(sl_core PRIVATE ${SL_DB_SOURCES})
    target_include_directories(sl_core PUBLIC ${SL_PG_INCLUDE_DIR})
    target_link_libraries(sl_core PUBLIC ${SL_PG_LIBRARY})
    target_compile_definitions(sl_core PUBLIC SL_HAS_POSTGRES=1)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sl_core PRIVATE -Wall -Wextra)
endif()

# Copie libpq.dll a cote d'un executable (Windows uniquement).
function(sl_copy_libpq target)
    if(WIN32 AND SL_HAS_POSTGRES AND EXISTS "${SL_PG_DLL}")
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${SL_PG_DLL}" "$<TARGET_FILE_DIR:${target}>/libpq.dll")
    endif()
endfunction()

# ============================================================
# Application console
# ============================================================

add_executable(smart_logistics main.cpp)
target_link_libraries(smart_logistics PRIVATE sl_core)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(smart_logistics PRIVATE -Wall -Wextra)
endif()
sl_copy_libpq(smart_logistics)

# ============================================================
# Tests
# ============================================================

enable_testing()

if(EXISTS "${CMAKE_SOURCE_DIR}/tests/test_main.cpp")
    add_executable(smart_logistics_tests tests/test_main.cpp)
    target_link_libraries(smart_logistics_tests PRIVATE sl_core)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(smart_logistics_tests PRIVATE -Wall -Wextra)
    endif()
    sl_copy_libpq(smart_logistics_tests)

    add_test(NAME smart_logistics_tests COMMAND smart_logistics_tests)
endif()

# Test de connexion PostgreSQL (necessite un serveur lance,
# donc volontairement PAS enregistre dans ctest).
if(SL_HAS_POSTGRES AND EXISTS "${CMAKE_SOURCE_DIR}/tests/postgresql_test.cpp")
    add_executable(postgresql_test tests/postgresql_test.cpp)
    target_link_libraries(postgresql_test PRIVATE sl_core)
    sl_copy_libpq(postgresql_test)
endif()

# ============================================================
# Application graphique Qt 6
# ============================================================

if(SL_WITH_GUI)
    find_package(Qt6 COMPONENTS Widgets QUIET)

    if(Qt6_FOUND)
        add_executable(smart_logistics_gui
            gui/main_gui.cpp
            gui/MainWindow.cpp
            gui/MainWindow.hpp
            gui/GuiDatabase.cpp
            gui/GuiDatabase.hpp
            gui/FormulaireDialog.cpp
            gui/FormulaireDialog.hpp
            gui/Style.hpp
        )
        set_target_properties(smart_logistics_gui PROPERTIES
            AUTOMOC ON
            AUTOUIC ON
            AUTORCC ON
        )
        target_link_libraries(smart_logistics_gui PRIVATE sl_core Qt6::Widgets)
        sl_copy_libpq(smart_logistics_gui)
        message(STATUS "Interface Qt : ACTIVEE")
    else()
        message(STATUS "Interface Qt : desactivee (Qt6 Widgets introuvable)")
    endif()
endif()