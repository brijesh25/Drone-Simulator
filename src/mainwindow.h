#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QTimer>
#include <QProgressBar>
#include <QFrame>
#include <QSizePolicy>
#include <memory>
#include "observer.h"
#include "dronedata.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QGroupBox;
class QProgressBar;
class QFrame;
QT_END_NAMESPACE

class DroneSimulator;
class MovementStrategy;

class MainWindow : public QMainWindow, public Observer {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Observer pattern implementation
    void update(const DroneData& data) override;

private slots:
    void onStartStopClicked();
    void onFailureModeToggled();
    void onMovementStrategyChanged();
    void onTelemetryUpdated(const DroneData& data);

private:
    void setupUI();
    void setupTelemetrySection();
    void setupControlsSection();
    void setupToggleSection();
    void setupStatusBarSection();
    void updateDisplayLabels(const DroneData& data);
    void connectSignals();
    void applyStyles();

    // Main layout components
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;

    // Telemetry section with custom layout
    QGroupBox* telemetryGroup;
    QGridLayout* telemetryLayout;

    // Top center - Drone ID
    QLabel* droneIdLabel;

    // Left side labels
    QLabel* latitudeLabel;
    QLabel* longitudeLabel;
    QLabel* altitudeLabel;
    QLabel* gpsStatusLabel;

    // Right side labels
    QLabel* headingLabel;
    QLabel* speedLabel;

    // Battery section (right bottom)
    QFrame* batteryFrame;
    QHBoxLayout* batteryLayout;
    QLabel* batteryIconLabel;
    QProgressBar* batteryProgressBar;
    QLabel* batteryPercentLabel;

    // Toggle section (center bottom)
    QFrame* toggleFrame;
    QHBoxLayout* toggleLayout;
    QLabel* toggleIcon1;
    QLabel* toggleIcon2;

    // Control buttons
    QGroupBox* controlsGroup;
    QGroupBox* statusMessage;
    QPushButton* startStopButton;
    QPushButton* failureModeButton;
    QPushButton* movementStrategyButton;

    QLabel* statusLabel;
    QLabel* copyrightLabel;

    // Simulation components
    std::unique_ptr<DroneSimulator> simulator;
    bool currentlyHovering;
};

#endif // MAINWINDOW_H
