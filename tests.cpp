/**
 * @file tests.cpp
 * @brief Test file for the PostLog library.
 *
 * This file demonstrates and tests the usage of the PostLog logging library, including:
 *   - Connecting to a PostgreSQL database
 *   - Creating a log table and inserting an initial setup message
 *   - Logging a test message to the database
 *
 * Requirements:
 *   - PostgreSQL server running and accessible
 *   - Database 'PostLog' and user 'PostLog' must exist
 *   - libpqxx must be installed and linked
 */

#include <iostream>
#include <pqxx/pqxx>

#include "PostLog.h"

using namespace PostLog;

/**
 * Test for DatabaseSink::setup() and log().
 *
 * This test connects to the 'PostLog' database, creates a 'test_logs' table (if not exists),
 * inserts an initial setup message, and logs a test message. It prints the result to stdout.
 *
 * Requirements:
 * - PostgreSQL server running and accessible
 * - Database 'PostLog' and user 'PostLog' must exist
 * - libpqxx must be installed and linked
 */
int main() {
    try {
        // Connect to the PostgreSQL database
        pqxx::connection conn("dbname=PostLog user=PostLog");
        // Create a DatabaseSink for the 'test_logs' table
        DatabaseSink dbSink(&conn, "test_logs");
        // Run setup: create table and insert initial setup message
        dbSink.setup();
        std::cout << "Setup completed successfully." << std::endl;
        // Log a test message
        Log testLog{"Test message", LogLevel::Info, time(nullptr)};
        dbSink.log(testLog);
        std::cout << "Test log inserted." << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
