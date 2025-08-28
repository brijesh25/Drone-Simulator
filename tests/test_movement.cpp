#include <QtTest/QtTest>
#include "hoverstrategy.h"
#include "randomwalkstrategy.h"
#include "dronedata.h"

class TestMovement : public QObject {
    Q_OBJECT

private slots:
    void testHoverStrategy();
    void testRandomWalkStrategy();
    void testStrategyNames();
};

void TestMovement::testHoverStrategy() {
    HoverStrategy hover;
    DroneData drone;

    double initialLat = drone.getLatitude();
    double initialLon = drone.getLongitude();

    hover.updatePosition(drone);

    // Position should change slightly for hovering
    QVERIFY(qAbs(drone.getLatitude() - initialLat) < 0.01);
    QVERIFY(qAbs(drone.getLongitude() - initialLon) < 0.01);
    QVERIFY(drone.getSpeed() >= 0.5 && drone.getSpeed() <= 2.0);
}

void TestMovement::testRandomWalkStrategy() {
    RandomWalkStrategy randomWalk;
    DroneData drone;

    double initialLat = drone.getLatitude();
    double initialLon = drone.getLongitude();

    randomWalk.updatePosition(drone);

    // Position should be within reasonable bounds
    QVERIFY(drone.getLatitude() >= 28.3 && drone.getLatitude() <= 29.2);
    QVERIFY(drone.getLongitude() >= 77.0 && drone.getLongitude() <= 78);
    QVERIFY(drone.getAltitude() >= 50.0 && drone.getAltitude() <= 200.0);
}

void TestMovement::testStrategyNames() {
    HoverStrategy hover;
    RandomWalkStrategy randomWalk;

    QCOMPARE(hover.getStrategyName(), QString("Hover Mode"));
    QCOMPARE(randomWalk.getStrategyName(), QString("Random Walk"));
}

QTEST_MAIN(TestMovement)
#include "test_movement.moc"
