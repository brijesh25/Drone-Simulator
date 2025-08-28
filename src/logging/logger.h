#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QMutex>
#include <QTextStream>
#include <QFile>
#include <memory>

// Singleton Pattern Implementation
class Logger {
public:
    enum LogLevel {
        DEBUG = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3
    };

    static Logger& getInstance();
    void log(LogLevel level, const QString& message);
    void setLogFile(const QString& filename);

    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger();
    ~Logger();

    LogLevel currentLevel;
    QMutex mutex;
    std::unique_ptr<QFile> logFile;
    std::unique_ptr<QTextStream> logStream;
    QString levelToString(LogLevel level) const;
};

#endif // LOGGER_H
