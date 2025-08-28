#include <QtTest/QtTest>
#include <QSignalSpy>
#include "dronesimulator.h"
#include "simulationfactory.h"
#include "movementstrategy.h"
#include "dronedata.h"

class TestSimulation : public QObject {
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testDroneDataCreation();
    void testSimulatorFactory();
    void testSimulationStartStop();
    void testFailureMode();
    void testObserverPattern();

private:
    std::unique_ptr<DroneSimulator> simulator;
};

void TestSimulation::initTestCase() {
    simulator = SimulationFactory::createSimulator(SimulationFactory::BASIC_SIMULATOR);
    QVERIFY(simulator != nullptr);
}

void TestSimulation::cleanupTestCase() {
    if (simulator) {
        simulator->stopSimulation();
    }
}

void TestSimulation::testDroneDataCreation() {
    DroneData data;
    QCOMPARE(data.getId(), QString("DRONE-001"));
    QCOMPARE(data.getBattery(), 100.0);
    QCOMPARE(data.getGPSStatus(), GPSFixStatus::FIX_3D);

    DroneData customData("TEST-DRONE", 40.0, -74.0, 150.0, 90.0, 5.0, 75.0, GPSFixStatus::FIX_2D);
    QCOMPARE(customData.getId(), QString("TEST-DRONE"));
    QCOMPARE(customData.getLatitude(), 40.0);
    QCOMPARE(customData.getLongitude(), -74.0);
    QCOMPARE(customData.getAltitude(), 150.0);
    QCOMPARE(customData.getBattery(), 75.0);
    QCOMPARE(customData.getGPSStatus(), GPSFixStatus::FIX_2D);
}

void TestSimulation::testSimulatorFactory() {
    auto sim1 = SimulationFactory::createSimulator(SimulationFactory::BASIC_SIMULATOR);
    auto sim2 = SimulationFactory::createSimulator(SimulationFactory::ADVANCED_SIMULATOR);

    QVERIFY(sim1 != nullptr);
    QVERIFY(sim2 != nullptr);

    auto hoverStrategy = SimulationFactory::createMovementStrategy(SimulationFactory::HOVER_MOVEMENT);
    auto randomStrategy = SimulationFactory::createMovementStrategy(SimulationFactory::RANDOM_WALK_MOVEMENT);

    QVERIFY(hoverStrategy != nullptr);
    QVERIFY(randomStrategy != nullptr);
}

void TestSimulation::testSimulationStartStop() {
    QVERIFY(!simulator->isRunning());

    simulator->startSimulation();
    QVERIFY(simulator->isRunning());

    simulator->stopSimulation();
    QVERIFY(!simulator->isRunning());
}

void TestSimulation::testFailureMode() {
    const DroneData& initialData = simulator->getDroneData();
    QCOMPARE(initialData.getGPSStatus(), GPSFixStatus::FIX_3D);

    simulator->setFailureMode(true);
    const DroneData& failureData = simulator->getDroneData();
    QCOMPARE(failureData.getGPSStatus(), GPSFixStatus::NO_FIX);

    simulator->setFailureMode(false);
    const DroneData& recoveredData = simulator->getDroneData();
    QCOMPARE(recoveredData.getGPSStatus(), GPSFixStatus::FIX_3D);
}

void TestSimulation::testObserverPattern() {
    QSignalSpy spy(simulator.get(), &DroneSimulator::telemetryUpdated);

    simulator->startSimulation();

    // Wait for at least one signal
    QVERIFY(spy.wait(1000));
    QVERIFY(spy.count() >= 1);

    simulator->stopSimulation();
}

QTEST_MAIN(TestSimulation)
#include "test_simulation.moc"
