#include "Logging.h"
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>

void Logging::baseLog(const char* message, const char* file)
{
    // Get the current time as a time_point
    auto now = std::chrono::system_clock::now();
    // Convert it to a time_t object
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    // Optionally, convert time_t to tm as local time
    std::tm* ptm = std::localtime(&now_time);

    // Format the time into a readable string
    char buffer[32];
    // Example format: Year-Month-Day Hour:Minute:Second
    std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", ptm);

    // Write the formatted time and the message to the log file
    std::ofstream logFile(file, std::ios_base::app);
    logFile << "[" << buffer << "] " << message << std::endl;
    logFile.close();
}

void Logging::logDebug(const char* message)
{
    baseLog(message, "arma3-discord-feed/debug.log");
}

void Logging::logError(const char* message)
{
    baseLog(message, "arma3-discord-feed/error.log");
}
