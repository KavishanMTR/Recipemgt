#include <QApplication>
#include <QFont>

#include "authentication.h"
#include "filehandler.h"
#include "loginwindow.h"
#include "mainwindow.h"
#include "recipemanager.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Recipe Management System");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("University Project");
    app.setStyle("Fusion");
    app.setFont(QFont("Segoe UI", 10));

    FileHandler::initialize();

    Authentication auth;
    RecipeManager manager;

    LoginWindow login(&auth);
    if (login.exec() != QDialog::Accepted || !auth.isLoggedIn()) {
        return 0;
    }

    MainWindow window(&manager, &auth, auth.getCurrentUser()->getUsername());
    QObject::connect(&window, &MainWindow::logoutRequested, &app, &QApplication::quit);
    window.show();

    return app.exec();
}
