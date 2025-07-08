/**
 * @file PostLog.h
 * @brief Main header for the PostLog library. Defines logging interfaces, sinks, and data structures.
 *
 * This file contains the main API for the PostLog logging library, including:
 *   - Log data structure
 *   - Logging sink interfaces (console, database)
 *   - Logger class for multi-sink logging
 *   - Utility functions for database connection and table creation
 *
 * Dependencies: libpqxx (PostgreSQL C++ client)
 */

#ifndef POSTLOG_H
#define POSTLOG_H
#include <string>
#include <pqxx/pqxx>

void hello();

bool isConnected();

void createTable(const std::string &tableName, const std::string &sql);

namespace PostLog {
    /**
     * @enum LogLevel
     * @brief Enumerates the standard syslog log levels.
     *
     * - Emergency (emerg): System is unusable and requires immediate attention.
     * - Alert (alert): Immediate action is necessary to resolve a critical issue.
     * - Critical (crit): Critical conditions demanding intervention to prevent system failure.
     * - Fatal (fatal): Severe errors that cause premature termination.
     * - Error (error): Error conditions that impair some operation but are less severe than critical situations.
     * - Warning (warn): Potential issues that may lead to errors or unexpected behavior in the future.
     * - Notice (notice): Normal but significant conditions that may require monitoring.
     * - Informational (info): Messages that provide a record of the normal operation of the system.
     * - Debug (debug): Detailed information for debugging purposes.
     */
    enum class LogLevel {
        Emergency,
        Alert,
        Critical,
        Fatal,      // Added fatal
        Error,
        Warning,
        Notice,
        Info,
        Debug
    };

    std::string logLevelToString(LogLevel level);
    LogLevel logLevelFromString(const std::string& str);

    /**
     * @struct Log
     * @brief Represents a single log entry.
     * @var message Log message text
     * @var level Log level (e.g., INFO, ERROR)
     * @var timestamp Log timestamp (as string or epoch)
     */
    struct Log {
        std::string message;
        LogLevel level;
        std::string timestamp;

        Log() = default;
        Log(const std::string &string, LogLevel level, time_t time);
    };

    /**
     * @class ILogSink
     * @brief Abstract interface for log sinks (output targets).
     */
    class ILogSink {
    public:
        virtual ~ILogSink() = default;
        virtual void log(const Log &entry) = 0;
    };

    /**
     * @class ConsoleSink
     * @brief Log sink that outputs to the console (stdout).
     */
    class ConsoleSink final : public ILogSink {
    public:
        void log(const Log &entry) override;
    };

    /**
     * @class DatabaseSink
     * @brief Log sink that writes logs to a PostgreSQL database table.
     *
     * Usage:
     *   - Construct with a pqxx::connection and table name
     *   - Call setup() to create the table and insert an initial message
     *   - Use log() to insert log entries
     */
    class DatabaseSink final : public ILogSink {
        pqxx::connection *conn = nullptr;
        std::string table;

    public:
        DatabaseSink(pqxx::connection *conn, const std::string &table);
        void log(const Log &entry) override;
        void setup();
    };

    /**
     * @class Logger
     * @brief Main logger class supporting multiple sinks.
     */
    class Logger {
        std::vector<ILogSink *> sinks;
    public:
        void addSink(ILogSink *sink);
        void removeSink(ILogSink *sink);
        void log(const std::string &message, const std::string &level = "INFO") const;
    };
}
#endif //POSTLOG_H
