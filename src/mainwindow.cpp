#include "mainwindow.h"
#include "dronesimulator.h"
#include "simulationfactory.h"
#include "movementstrategy.h"
#include "logger.h"
#include <QApplication>
#include <QMessageBox>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget(nullptr)
    , currentlyHovering(true)
{
    setupUI();
    connectSignals();

    // Create simulator using factory pattern
    simulator = SimulationFactory::createSimulator(SimulationFactory::BASIC_SIMULATOR);

    // Set initial movement strategy
    auto hoverStrategy = SimulationFactory::createMovementStrategy(SimulationFactory::HOVER_MOVEMENT);
    simulator->setMovementStrategy(std::move(hoverStrategy));

    // Attach this window as an observer
    simulator->attach(this);

    // Connect simulator signals
    connect(simulator.get(), &DroneSimulator::telemetryUpdated,
            this, &MainWindow::onTelemetryUpdated);

    Logger::getInstance().log(Logger::INFO, "MainWindow initialized.");
}

MainWindow::~MainWindow() {
    if (simulator) {
        simulator->detach(this);
        simulator->stopSimulation();
    }
    Logger::getInstance().log(Logger::INFO, "MainWindow destroyed");
}

void MainWindow::setupUI() {
    setWindowTitle("Real-Time Drone Telemetry Simulator");
    setMinimumSize(800, 625);

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 10, 20, 10);

    // Setup all sections
    setupTelemetrySection();
    setupControlsSection();
    setupToggleSection();
    setupStatusBarSection();

    statusBar()->showMessage("Drone Telemetry Simulator v1.0 - Ready");
    statusBar()->setStyleSheet("color: black;");

    // Apply styling
    applyStyles();
}

void MainWindow::setupTelemetrySection() {
    telemetryGroup = new QGroupBox("Live Telemetry Data", this);
    telemetryGroup->setObjectName("telemetryGroup");

    telemetryLayout = new QGridLayout(telemetryGroup);
    telemetryLayout->setSpacing(5);
    telemetryLayout->setContentsMargins(20, 5, 20, 5);

    // Top center - Drone ID
    droneIdLabel = new QLabel("DRONE-001", this);
    droneIdLabel->setObjectName("droneIdLabel");
    droneIdLabel->setMaximumHeight(90);
    droneIdLabel->setAlignment(Qt::AlignCenter);
    telemetryLayout->addWidget(droneIdLabel, 0, 0, 1, 4);  // Row 0, spans 4 columns

    // Left side - Position data
    QLabel* positionHeader = new QLabel("POSITION", this);
    positionHeader->setObjectName("sectionHeader");
    telemetryLayout->addWidget(positionHeader, 1, 0, 1, 2);

    latitudeLabel = new QLabel("Latitude: --", this);
    latitudeLabel->setObjectName("telemetryLabel");
    telemetryLayout->addWidget(latitudeLabel, 2, 0, 1, 2);

    longitudeLabel = new QLabel("Longitude: --", this);
    longitudeLabel->setObjectName("telemetryLabel");
    telemetryLayout->addWidget(longitudeLabel, 3, 0, 1, 2);

    altitudeLabel = new QLabel("Altitude: -- m", this);
    altitudeLabel->setObjectName("telemetryLabel");
    telemetryLayout->addWidget(altitudeLabel, 4, 0, 1, 2);

    // Right side - Motion data
    QLabel* motionHeader = new QLabel("MOTION", this);
    motionHeader->setObjectName("sectionHeader");
    telemetryLayout->addWidget(motionHeader, 1, 2, 1, 2);

    headingLabel = new QLabel("Heading: -- °", this);
    headingLabel->setObjectName("telemetryLabel");
    telemetryLayout->addWidget(headingLabel, 2, 2, 1, 2);

    speedLabel = new QLabel("Speed: -- m/s", this);
    speedLabel->setObjectName("telemetryLabel");
    telemetryLayout->addWidget(speedLabel, 3, 2, 1, 2);

    // Left bottom - GPS Status
    gpsStatusLabel = new QLabel("GPS: --", this);
    gpsStatusLabel->setObjectName("gpsStatusLabel");
    telemetryLayout->addWidget(gpsStatusLabel, 5, 0, 1, 2);

    // Right bottom - Battery with progress bar
    batteryFrame = new QFrame(this);
    batteryFrame->setObjectName("batteryFrame");
    batteryLayout = new QHBoxLayout(batteryFrame);
    batteryLayout->setContentsMargins(5, 5, 5, 5);

    // Battery icon (using Unicode)
    batteryIconLabel = new QLabel("🔋", this);
    batteryIconLabel->setObjectName("batteryIcon");

    // Battery progress bar
    batteryProgressBar = new QProgressBar(this);
    batteryProgressBar->setObjectName("batteryProgressBar");
    batteryProgressBar->setRange(0, 100);
    batteryProgressBar->setValue(100);
    batteryProgressBar->setTextVisible(false);
    batteryProgressBar->setMaximumHeight(20);

    // Battery percentage text
    batteryPercentLabel = new QLabel("100%", this);
    batteryPercentLabel->setObjectName("batteryPercentLabel");

    batteryLayout->addWidget(batteryIconLabel);
    batteryLayout->addWidget(batteryProgressBar);
    batteryLayout->addWidget(batteryPercentLabel);

    telemetryLayout->addWidget(batteryFrame, 5, 2, 1, 2);

    mainLayout->addWidget(telemetryGroup);
}

void MainWindow::setupControlsSection() {
    controlsGroup = new QGroupBox("Simulation Controls", this);
    controlsGroup->setObjectName("controlsGroup");

    auto* controlsLayout = new QHBoxLayout(controlsGroup);
    controlsLayout->setSpacing(15);
    controlsLayout->setContentsMargins(20, 20, 20, 20);

    startStopButton = new QPushButton("▶️ Start Simulation", this);
    startStopButton->setObjectName("startStopButton");
    startStopButton->setMinimumHeight(50);

    failureModeButton = new QPushButton("⚠️ Enable Failure Mode", this);
    failureModeButton->setObjectName("failureModeButton");
    failureModeButton->setMinimumHeight(50);

    movementStrategyButton = new QPushButton("🔄 Switch to Random Walk", this);
    movementStrategyButton->setObjectName("movementStrategyButton");
    movementStrategyButton->setMinimumHeight(50);

    controlsLayout->addWidget(startStopButton);
    controlsLayout->addWidget(failureModeButton);
    controlsLayout->addWidget(movementStrategyButton);

    mainLayout->addWidget(controlsGroup);
}

void MainWindow::setupToggleSection() {
    toggleFrame = new QFrame(this);
    toggleFrame->setObjectName("toggleFrame");
    toggleFrame->setMaximumHeight(80);

    toggleLayout = new QHBoxLayout(toggleFrame);

    // Add spacer to center the toggles
    toggleLayout->addStretch();

    // Toggle 1 - Mode indicator
    toggleIcon1 = new QLabel("🚁", this);
    toggleIcon1->setObjectName("toggleIcon");
    toggleIcon1->setAlignment(Qt::AlignCenter);
    toggleIcon1->setFixedSize(60, 60);

    // Toggle 2 - Status indicator
    toggleIcon2 = new QLabel("📡", this);
    toggleIcon2->setObjectName("toggleIcon");
    toggleIcon2->setAlignment(Qt::AlignCenter);
    toggleIcon2->setFixedSize(60, 60);

    toggleLayout->addWidget(toggleIcon1);
    toggleLayout->addSpacing(30);
    toggleLayout->addWidget(toggleIcon2);
    toggleLayout->addStretch();

    mainLayout->addWidget(toggleFrame);
}

void MainWindow::setupStatusBarSection() {
    statusMessage = new QGroupBox("", this);
    statusMessage->setObjectName("controlsGroup");

    auto* statusMessageLayout = new QHBoxLayout(statusMessage);
    statusMessageLayout->setSpacing(2);
    statusMessageLayout->setContentsMargins(20, 5, 20, 5);

    // Status bar
    statusLabel = new QLabel("Ready to start simulation", this);
    statusLabel->setStyleSheet("color: black;");

    // Copyright Label
    copyrightLabel = new QLabel("©2025 Brijesh Markandey", this);
    copyrightLabel->setStyleSheet("color: black; font-weight: bold;");
    copyrightLabel->setAlignment(Qt::AlignRight);

    statusMessageLayout->addWidget(statusLabel);
    statusMessageLayout->addWidget(copyrightLabel);

    mainLayout->addWidget(statusMessage);
}

void MainWindow::applyStyles() {
    QString styleSheet = R"(
        /* Main window background */
        QMainWindow {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #f8fafc, stop:1 #e2e8f0);
        }

        /* Telemetry group */
        #telemetryGroup {
            font-size: 16px;
            font-weight: bold;
            color: #1a202c;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 rgba(255,255,255,0.9),
                stop:0.5 rgba(240,248,255,0.8),
                stop:1 rgba(255,255,255,0.9));
            border: 2px solid #cbd5e0;
            border-radius: 15px;
            margin-top: 10px;
            padding-top: 15px;
        }

        /* Drone ID styling */
        #droneIdLabel {
            font-size: 24px;
            font-weight: bold;
            color: #2d3748;
            background: #667eea;
            border: 2px solid #e2e8f0;
            border-radius: 8px;
            padding: 10px;
            margin-bottom: 15px;
        }

        /* Section headers */
        #sectionHeader {
            font-size: 14px;
            font-weight: bold;
            color: #4a5568;
            padding: 5px 0px;
            border-bottom: 1px solid #e2e8f0;
            margin-bottom: 5px;
        }

        /* Telemetry labels */
        #telemetryLabel {
            font-family: 'Consolas', 'Monaco', monospace;
            font-size: 14px;
            color: #2d3748;
            padding: 8px 12px;
            background: rgba(247, 250, 252, 0.6);
            border-left: 3px solid #4299e1;
            border-radius: 4px;
            margin: 2px 0px;
        }

        /* GPS Status */
        #gpsStatusLabel {
            font-family: 'Consolas', 'Monaco', monospace;
            font-size: 14px;
            font-weight: bold;
            color: #2d3748;
            padding: 8px 12px;
            border-radius: 6px;
            background: #f0fff4;
            border: 1px solid #9ae6b4;
        }

        /* Battery frame */
        #batteryFrame {
            background: rgba(247, 250, 252, 0.8);
            border: 1px solid #e2e8f0;
            border-radius: 8px;
            padding: 5px;
        }

        /* Battery icon */
        #batteryIcon {
            font-size: 20px;
            padding: 2px;
        }

        /* Battery progress bar */
        #batteryProgressBar {
            border: 1px solid #cbd5e0;
            border-radius: 6px;
            background: #f7fafc;
        }

        #batteryProgressBar::chunk {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #48bb78, stop:0.7 #68d391, stop:1 #38a169);
            border-radius: 5px;
        }

        /* Battery percentage */
        #batteryPercentLabel {
            font-family: 'Consolas', 'Monaco', monospace;
            font-size: 12px;
            font-weight: bold;
            color: #2d3748;
            min-width: 40px;
        }

        /* Controls group */
        #controlsGroup {
            font-size: 16px;
            font-weight: bold;
            color: #1a202c;
            background: rgba(255,255,255,0.7);
            border: 1px solid #e2e8f0;
            border-radius: 12px;
            margin-top: 10px;
        }

        /* Modern buttons */
        #startStopButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #48bb78, stop:1 #38a169);
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 14px;
            font-weight: bold;
            padding: 12px 20px;
        }

        #startStopButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #68d391, stop:1 #48bb78);
        }

        #startStopButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #38a169, stop:1 #2f855a);
        }

        #failureModeButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #f56565, stop:1 #e53e3e);
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 14px;
            font-weight: bold;
            padding: 12px 20px;
        }

        #failureModeButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #fc8181, stop:1 #f56565);
        }

        #movementStrategyButton {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #4299e1, stop:1 #3182ce);
            color: white;
            border: none;
            border-radius: 8px;
            font-size: 14px;
            font-weight: bold;
            padding: 12px 20px;
        }

        #movementStrategyButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #63b3ed, stop:1 #4299e1);
        }

        /* Toggle frame */
        #toggleFrame {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 rgba(255,255,255,0.3),
                stop:0.5 rgba(240,248,255,0.5),
                stop:1 rgba(255,255,255,0.3));
            border: 1px solid #e2e8f0;
            border-radius: 10px;
        }

        /* Toggle icons */
        #toggleIcon {
            font-size: 28px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #667eea, stop:1 #764ba2);
            border: 2px solid #cbd5e0;
            border-radius: 30px;
            padding: 15px;
        }

        #toggleIcon:hover {
            border: 2px solid #4299e1;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #4299e1, stop:1 #667eea);
        }
    )";

    setStyleSheet(styleSheet);
}

void MainWindow::connectSignals() {
    connect(startStopButton, &QPushButton::clicked, this, &MainWindow::onStartStopClicked);
    connect(failureModeButton, &QPushButton::clicked, this, &MainWindow::onFailureModeToggled);
    connect(movementStrategyButton, &QPushButton::clicked, this, &MainWindow::onMovementStrategyChanged);
}

void MainWindow::update(const DroneData& data) {
    updateDisplayLabels(data);
}

void MainWindow::updateDisplayLabels(const DroneData& data) {
    // Update drone ID
    droneIdLabel->setText(data.getId());

    // Update position data (left side)
    latitudeLabel->setText(QString("Latitude: %1°").arg(data.getLatitude(), 0, 'f', 6));
    longitudeLabel->setText(QString("Longitude: %1°").arg(data.getLongitude(), 0, 'f', 6));
    altitudeLabel->setText(QString("Altitude: %1 m").arg(data.getAltitude(), 0, 'f', 1));

    // Update motion data (right side)
    headingLabel->setText(QString("Heading: %1°").arg(data.getHeading(), 0, 'f', 1));
    speedLabel->setText(QString("Speed: %1 m/s").arg(data.getSpeed(), 0, 'f', 2));

    // Update GPS status (left bottom) with color coding
    QString gpsText = QString("GPS: %1").arg(data.gpsStatusString());
    gpsStatusLabel->setText(gpsText);

    if (data.getGPSStatus() == GPSFixStatus::FIX_3D) {
        gpsStatusLabel->setStyleSheet("#gpsStatusLabel { background: #f0fff4; border: 1px solid #9ae6b4; color: #22543d; }");
    } else {
        gpsStatusLabel->setStyleSheet("#gpsStatusLabel { background: #fed7d7; border: 1px solid #fc8181; color: #742a2a; }");
    }

    // Update battery (right bottom) with progress bar and color coding
    int batteryValue = static_cast<int>(data.getBattery());
    batteryProgressBar->setValue(batteryValue);
    batteryPercentLabel->setText(QString("%1%").arg(batteryValue));

    // Color-code battery progress bar based on level
    QString batteryStyle;
    if (batteryValue > 50) {
        batteryStyle = "#batteryProgressBar::chunk { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #48bb78, stop:1 #38a169); }";
    } else if (batteryValue > 20) {
        batteryStyle = "#batteryProgressBar::chunk { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ed8936, stop:1 #dd6b20); }";
    } else if (batteryValue == 0) {
        batteryStyle = "#batteryProgressBar::chunk { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f56565, stop:1 #e53e3e); }";
        simulator->stopSimulation();
        startStopButton->setText("▶️ Start Simulation");
        statusLabel->setText("Simulation stopped due to low battery");
        statusBar()->showMessage("Simulation stopped due to low battery");
        toggleIcon1->setText("⏸️");  // Paused
    } else {
        batteryStyle = "#batteryProgressBar::chunk { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f56565, stop:1 #e53e3e); }";
    }
    batteryProgressBar->setStyleSheet(batteryStyle);

    // Update toggle icons based on status
    if (data.getGPSStatus() == GPSFixStatus::FIX_3D) {
        toggleIcon2->setText("📡");  // Good signal
    } else {
        toggleIcon2->setText("📵");  // No signal
    }
}

void MainWindow::onStartStopClicked() {
    if (!simulator) return;

    if (simulator->isRunning()) {
        simulator->stopSimulation();
        startStopButton->setText("▶️ Start Simulation");
        statusLabel->setText("Simulation stopped");
        statusBar()->showMessage("Simulation stopped");
        toggleIcon1->setText("⏸️");  // Paused
    } else {
        simulator->startSimulation();
        startStopButton->setText("⏹️ Stop Simulation");
        statusLabel->setText("Simulation running");
        statusBar()->showMessage("Simulation running - Updates every 500ms");
        toggleIcon1->setText("🚁");  // Active
    }

    Logger::getInstance().log(Logger::INFO,
                              QString("Simulation %1").arg(simulator->isRunning() ? "started" : "stopped"));
}

void MainWindow::onFailureModeToggled() {
    if (!simulator) return;

    static bool failureEnabled = false;
    failureEnabled = !failureEnabled;

    simulator->setFailureMode(failureEnabled);

    if (failureEnabled) {
        failureModeButton->setText("✅ Disable Failure Mode");
        statusLabel->setText("⚠️ FAILURE MODE ACTIVE");
        statusBar()->showMessage("FAILURE MODE: GPS lost, rapid battery drain");
    } else {
        failureModeButton->setText("⚠️ Enable Failure Mode");
        statusLabel->setText("Simulation running normally");
        statusBar()->showMessage("Failure mode disabled - Normal operation");
    }
}

void MainWindow::onMovementStrategyChanged() {
    if (!simulator) return;

    if (currentlyHovering) {
        auto randomWalkStrategy = SimulationFactory::createMovementStrategy(SimulationFactory::RANDOM_WALK_MOVEMENT);
        simulator->setMovementStrategy(std::move(randomWalkStrategy));
        movementStrategyButton->setText("🎯 Switch to Hover Mode");
        statusLabel->setText("Movement: Random Walk");
        currentlyHovering = false;
    } else {
        auto hoverStrategy = SimulationFactory::createMovementStrategy(SimulationFactory::HOVER_MOVEMENT);
        simulator->setMovementStrategy(std::move(hoverStrategy));
        movementStrategyButton->setText("🔄 Switch to Random Walk");
        statusLabel->setText("Movement: Hover Mode");
        currentlyHovering = true;
    }
}

void MainWindow::onTelemetryUpdated(const DroneData& data) {
    updateDisplayLabels(data);
}
