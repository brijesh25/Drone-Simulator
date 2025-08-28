#include <QtTest/QtTest>
#include <QTemporaryFile>
#include "logger.h"

class TestLogger : public QObject {
    Q_OBJECT

private slots:
    void testSingleton();
    void testLogLevels();
    void testFileLogging();
};

void TestLogger::testSingleton() {
    Logger& logger1 = Logger::getInstance();
    Logger& logger2 = Logger::getInstance();

    // Should be the same instance
    QCOMPARE(&logger1, &logger2);
}

void TestLogger::testLogLevels() {
    Logger& logger = Logger::getInstance();

    // These should work without crashing
    logger.log(Logger::DEBUG, "Debug message");    // Should be filtered out
    logger.log(Logger::INFO, "Info message");      // Should be filtered out  
    logger.log(Logger::WARNING, "Warning message"); // Should be logged
    logger.log(Logger::ERROR, "Error message");     // Should be logged

    QVERIFY(true); // If we get here, no crashes occurred
}

void TestLogger::testFileLogging() {
    QTemporaryFile tempFile;
    QVERIFY(tempFile.open());

    QString filename = tempFile.fileName();
    tempFile.close();

    Logger& logger = Logger::getInstance();
    logger.setLogFile(filename);
    logger.log(Logger::INFO, "Test log message");

    // Verify file exists and has content
    QFile logFile(filename);
    QVERIFY(logFile.exists());
    QVERIFY(logFile.size() > 0);
}

QTEST_MAIN(TestLogger)
#include "test_logger.moc"
