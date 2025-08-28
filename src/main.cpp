#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include "mainwindow.h"
#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set application properties
    app.setApplicationName("Drone Telemetry Simulator");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("DroneSimulation");

    // Initialize logger
    Logger::getInstance().log(Logger::INFO, "Application starting...");

    // Create and show main window
    MainWindow window;
    window.show();

    Logger::getInstance().log(Logger::INFO, "Main window displayed");

    int result = app.exec();

    Logger::getInstance().log(Logger::INFO, "Application exiting...");
    return result;
}