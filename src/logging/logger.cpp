#include "logger.h"
#include <QDateTime>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>

Logger::Logger() 
    : currentLevel(INFO)
    , logFile(nullptr)
    , logStream(nullptr)
{
    // Set default log file in user's documents
    QString logDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QDir().mkpath(logDir);
    setLogFile(logDir + "/drone_simulator.log");
}

Logger::~Logger() {
    if (logStream) {
        logStream->flush();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::log(LogLevel level, const QString& message) {
    QMutexLocker locker(&mutex);

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString logMessage = QString("[%1] %2: %3")
                         .arg(timestamp)
                         .arg(levelToString(level))
                         .arg(message);

    // Output to console
    qDebug() << logMessage;

    // Output to file if available
    if (logStream) {
        *logStream << logMessage << Qt::endl;
        logStream->flush();
    }
}

void Logger::setLogFile(const QString& filename) {
    QMutexLocker locker(&mutex);

    logFile = std::make_unique<QFile>(filename);
    if (logFile->open(QIODevice::WriteOnly | QIODevice::Append)) {
        logStream = std::make_unique<QTextStream>(logFile.get());
    }
}

QString Logger::levelToString(LogLevel level) const {
    switch (level) {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARNING: return "WARNING";
        case ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}
