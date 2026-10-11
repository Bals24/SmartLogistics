#include <QApplication>

#include "MainWindow.hpp"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Smart Logistics");
    app.setApplicationDisplayName("Smart Logistics");

    MainWindow window;
    window.show();

    return app.exec();
}