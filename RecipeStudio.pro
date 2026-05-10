QT += widgets core gui

CONFIG += c++17
TEMPLATE = app
TARGET = RecipeStudio

INCLUDEPATH += include

HEADERS += \
    include/addrecipedialog.h \
    include/authentication.h \
    include/filehandler.h \
    include/loginwindow.h \
    include/mainwindow.h \
    include/recipe.h \
    include/recipecard.h \
    include/recipedetailwindow.h \
    include/recipemanager.h \
    include/splashscreen.h \
    include/styles.h \
    include/uihelpers.h \
    include/user.h

SOURCES += \
    src/addrecipedialog.cpp \
    src/authentication.cpp \
    src/loginwindow.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/recipedetailwindow.cpp \
    src/recipemanager.cpp \
    src/recipecard.cpp \
    src/splashscreen.cpp \
    src/filehandler.cpp \
    src/recipe.cpp \
    src/user.cpp
