#include <bad_zip/logging/Logger.hpp>
#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>
using bad_zip::Logger, bad_zip::LogLevel;
using std::string, std::ostringstream;

namespace {
string expected_message(const LogLevel& level, const string& source, const string& message) {
    return "\033[1m[" + Logger::log_level_to_string(level) + "]\033[0m <" + source + "> " +
           message + "\n";
}
}  // namespace

TEST_CASE("logger correctly produces the output message", "[Logger]") {
    ostringstream out;
    const auto logger = Logger(false, &out);

    SECTION("using the DEBUG level") {
        logger.log(LogLevel::DEBUG, "main", "test message");

        REQUIRE(out.str() == expected_message(LogLevel::DEBUG, "main", "test message"));
    }

    SECTION("using the INFO level") {
        logger.log(LogLevel::INFO, "main", "test message again");

        REQUIRE(out.str() == expected_message(LogLevel::INFO, "main", "test message again"));
    }

    SECTION("using the WARN level") {
        logger.log(LogLevel::WARN, "main", "test message 3");

        REQUIRE(out.str() == expected_message(LogLevel::WARN, "main", "test message 3"));
    }

    SECTION("using the ERROR level") {
        logger.log(LogLevel::ERROR, "not main", "test_message");

        REQUIRE(out.str() == expected_message(LogLevel::ERROR, "not main", "test_message"));
    }

    SECTION("using the FATAL level") {
        logger.log(LogLevel::FATAL, "another main", "fatal message");

        REQUIRE(out.str() == expected_message(LogLevel::FATAL, "another main", "fatal message"));
    }
}

TEST_CASE("quiet mode suppresses DEBUG, INFO, and WARN messages", "[Logger]") {
    ostringstream out;
    const auto logger = Logger(true, &out);

    SECTION("DEBUG is suppressed") {
        logger.log(LogLevel::DEBUG, "main", "test message");

        REQUIRE(out.str().empty());
    }

    SECTION("INFO is suppressed") {
        logger.log(LogLevel::INFO, "main", "test message");

        REQUIRE(out.str().empty());
    }

    SECTION("WARN is suppressed") {
        logger.log(LogLevel::WARN, "main",
                   "this really doesn't matter cuz it shouldn't be printing anyways");

        REQUIRE(out.str().empty());
    }

    SECTION("ERROR is not suppressed") {
        logger.log(LogLevel::ERROR, "main", "test");

        REQUIRE(out.str() == expected_message(LogLevel::ERROR, "main", "test"));
    }

    SECTION("FATAL is not suppressed") {
        logger.log(LogLevel::FATAL, "main", "test");

        REQUIRE(out.str() == expected_message(LogLevel::FATAL, "main", "test"));
    }
}