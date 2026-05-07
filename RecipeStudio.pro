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
    addrecipedialog.cpp \
    authentication.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    recipedetailwindow.cpp \
    recipemanager.cpp \
    recipecard.cpp \
    splashscreen.cpp \
    src/filehandler.cpp \
    src/recipe.cpp \
    src/user.cpp
