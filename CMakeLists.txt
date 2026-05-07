cmake_minimum_required(VERSION 3.16)

project(RecipeStudio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 CONFIG COMPONENTS Widgets Core Gui QUIET)
if(Qt6_FOUND)
    set(QT_PACKAGE_PREFIX Qt6)
else()
    find_package(Qt5 CONFIG REQUIRED COMPONENTS Widgets Core Gui)
    set(QT_PACKAGE_PREFIX Qt5)
endif()

set(PROJECT_SOURCES
    main.cpp
    splashscreen.cpp
    loginwindow.cpp
    mainwindow.cpp
    recipecard.cpp
    recipedetailwindow.cpp
    addrecipedialog.cpp
    authentication.cpp
    recipemanager.cpp
    src/recipe.cpp
    src/user.cpp
    src/filehandler.cpp
)

set(PROJECT_HEADERS
    include/addrecipedialog.h
    include/authentication.h
    include/filehandler.h
    include/loginwindow.h
    include/mainwindow.h
    include/recipe.h
    include/recipecard.h
    include/recipedetailwindow.h
    include/recipemanager.h
    include/splashscreen.h
    include/styles.h
    include/uihelpers.h
    include/user.h
)

add_executable(RecipeStudio
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
)

target_include_directories(RecipeStudio PRIVATE include)
target_link_libraries(RecipeStudio PRIVATE
    ${QT_PACKAGE_PREFIX}::Widgets
    ${QT_PACKAGE_PREFIX}::Core
    ${QT_PACKAGE_PREFIX}::Gui
)

if(WIN32)
    set_target_properties(RecipeStudio PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
