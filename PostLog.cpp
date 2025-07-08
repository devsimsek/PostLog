/**
 * @file PostLog.cpp
 * @brief Implementation of the PostLog logging library.
 *
 * Contains the implementation for all logging sinks, the Logger class, and utility functions.
 *
 * Dependencies: libpqxx (PostgreSQL C++ client)
 */

#include "PostLog.h"
#include <pqxx/pqxx>
#include <iostream>
#include <ctime>

using namespace PostLog;

/**
 * @brief Add a log sink to the logger.
 * @param sink Pointer to a log sink (ConsoleSink, DatabaseSink, etc.)
 */
void PostLog::Logger::addSink(ILogSink *sink) {
    sinks.push_back(sink);
}

/**
 * @brief Remove a log sink from the logger.
 * @param sink Pointer to the log sink to remove.
 */
void PostLog::Logger::removeSink(ILogSink *sink) {
    std::erase(sinks, sink);
}

/**
 * @brief Log a message to all registered sinks.
 * @param message The log message.
 * @param level The log level (default: INFO).
 */
void PostLog::Logger::log(const std::string &message, const std::string &level) const {
    const time_t timestamp = time(nullptr);
    const Log entry{message, logLevelFromString(level), timestamp};
    for (ILogSink *sink: sinks) {
        sink->log(entry);
    }
}

namespace PostLog {
    std::string logLevelToString(LogLevel level) {
        switch (level) {
            case LogLevel::Emergency: return "emerg";
            case LogLevel::Alert: return "alert";
            case LogLevel::Critical: return "crit";
            case LogLevel::Fatal: return "fatal";
            case LogLevel::Error: return "error";
            case LogLevel::Warning: return "warn";
            case LogLevel::Notice: return "notice";
            case LogLevel::Info: return "info";
            case LogLevel::Debug: return "debug";
            default: return "unknown";
        }
    }

    LogLevel logLevelFromString(const std::string &str) {
        if (str == "emerg") return LogLevel::Emergency;
        if (str == "alert") return LogLevel::Alert;
        if (str == "crit") return LogLevel::Critical;
        if (str == "fatal") return LogLevel::Fatal;
        if (str == "error") return LogLevel::Error;
        if (str == "warn") return LogLevel::Warning;
        if (str == "notice") return LogLevel::Notice;
        if (str == "info") return LogLevel::Info;
        if (str == "debug") return LogLevel::Debug;
        return LogLevel::Info;
    }

    Log::Log(const std::string &msg, LogLevel lvl, time_t time)
        : message(msg), level(lvl), timestamp(std::to_string(time)) {
    }
}

/**
 * @brief Output a log entry to the console.
 * @param entry The log entry to output.
 */
void PostLog::ConsoleSink::log(const Log &entry) {
    std::cout << "[" << PostLog::logLevelToString(entry.level) << "] " << entry.timestamp << ": " << entry.message <<
            std::endl;
}

/**
 * @brief Insert a log entry into the database table.
 * @param entry The log entry to insert.
 */
void PostLog::DatabaseSink::log(const Log &entry) {
    pqxx::work txn(*conn);
    txn.exec("INSERT INTO " + table + " (level, timestamp, message) VALUES (" +
             txn.quote(PostLog::logLevelToString(entry.level)) + ", " +
             txn.quote(entry.timestamp) + ", " +
             txn.quote(entry.message) + ")");
    txn.commit();
}

/**
 * @brief Create the log table if it does not exist and insert an initial setup message.
 */
void PostLog::DatabaseSink::setup() {
    pqxx::work txn(*conn);
    txn.exec("CREATE TABLE IF NOT EXISTS " + table + " ("
             "id SERIAL PRIMARY KEY, "
             "level TEXT, "
             "timestamp BIGINT, "
             "message TEXT)");
    // Only insert the setup message if the table was just created (i.e., is empty)
    pqxx::result r = txn.exec("SELECT COUNT(*) FROM " + table);
    if (!r.empty() && r[0][0].as<long>() == 0) {
        txn.exec("INSERT INTO " + table + " (level, timestamp, message) VALUES (" +
                 txn.quote(PostLog::logLevelToString(LogLevel::Notice)) + ", " +
                 txn.quote(time(nullptr)) + ", " +
                 txn.quote("Initial setup complete.") + ")");
    }
    txn.commit();
}

/**
 * @brief Construct a DatabaseSink with a pqxx connection and table name.
 */
PostLog::DatabaseSink::DatabaseSink(pqxx::connection *conn_, const std::string &table_)
    : conn(conn_), table(table_) {
}
