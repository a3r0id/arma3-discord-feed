#include <string>
#include <filesystem>
#include <array>
#include <string>
#include "Functions.h"
#include "Logging.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

const char* logFiles[] = { "debug", "error" };
const char* requiredFieldsBasicEmbed[] = {"title", "description"};

json Functions::getConfig()
{
	std::ifstream configFile("arma3-discord-feed/config.json");
	json config;
	try {
		configFile >> config;
	} catch (json::parse_error& e) {
		Logging::logError(e.what());
		return json();
	}
	configFile.close();
	return config;
}

bool Functions::isBootstrapped()
{
	return std::filesystem::exists("arma3-discord-feed");
}

std::string Functions::bootstrap(bool bBootstrapped)
{
	std::string status = "Bootstrap: ";
	// check if directory exists and create if it doesn't
	if (!bBootstrapped)
	{
		std::filesystem::create_directory("arma3-discord-feed");
		status += "Environment created!";
	}
	else
	{
		status += "Environment already exists!";
		Logging::logDebug(status.c_str());
		return status;
	}
	// check if config file exists and create if it doesn't
	if (!std::filesystem::exists("arma3-discord-feed/config.json"))
	{
		std::ofstream configFile("arma3-discord-feed/config.json");
		configFile << "{\"private_channel\": {\"bot_token\": \"<your bot's token>\", \"name\": \"Admin Log\", \"avatar\": \"<changeme>\", \"webhook_url\": \"<changeme>\"}, \"public_channel\": {\"name\": \"Public Log\", \"avatar\": \"<changeme>\", \"webhook_url\": \"<changeme>\"}}";
		configFile.close();
	}
	// iterate over log files and create if they don't exist
	for (const char* logFile : logFiles)
	{
		std::string filePath = "arma3-discord-feed/" + std::string(logFile) + ".log"; // Create the full file path
		if (!std::filesystem::exists(filePath)) // Check if the file exists
		{
			std::ofstream logFileStream(filePath); // Open the file using the file path
			logFileStream.close(); // Immediately close the file
		}
	}
	Logging::logDebug(status.c_str());
	return status;
}

std::string Functions::simpleFeedEmbed(json args, const char* type, json config)
{

	// ensure that the args is an object
	if (!args.is_object()) {
		return "[ [\"success\", false], [\"error\", \"Invalid JSON object\"] ]";
	}

	// ensure that the config contains the required fields
	if (!config.contains(type)) {
		// Log error or handle the situation
		return "[ [\"success\", false], [\"error\", \"Missing configuration: channel type\"] ]";
	}

	// type can be either "private_channel" or "public_channel"
	json channel = config[type];

	// if channel is not an object, log error or handle the situation
	if (!channel.is_object()) {
		return "[ [\"success\", false], [\"error\", \"Invalid configuration: channel is not object\"] ]";
	}

	// Log the channel and args
	Logging::logDebug(("[Functions::simpleFeedEmbed::Config] " + channel.dump()).c_str());
	Logging::logDebug(("[Functions::simpleFeedEmbed::Args] " + args.dump()).c_str());

	// ensure that the channel contains the required fields
	if (!channel.contains("webhook_url") || !channel.contains("name") || !channel.contains("avatar")) {
		return "[ [\"success\", false], [\"error\", \"Missing configuration\"] ]";
	}

	// iterate over the required fields and ensure that they are not null
	for (const char* field : requiredFieldsBasicEmbed)
	{
		if (!args.contains(field) || args[field].is_null())
		{
			return "[ [\"success\", false], [\"error\", \"Missing JSON field: " + std::string(field) + "\"] ]";
		}
	}

	// create payload object
	json payload;
	payload["username"] = channel["name"];
	payload["avatar_url"] = channel["avatar"];

	// create embed object
	json embed;
	embed["title"] = args["title"];
	embed["description"] = args["description"];
	embed["color"] = 16711680; // Red
	std::vector embeds = { embed };
	payload["embeds"] = embeds;
	std::string payloadString = payload.dump();
	Logging::logDebug(("[Functions::simpleFeedEmbed::Payload] " + payloadString).c_str());
	std::string webhookURL = channel["webhook_url"];
	cpr::Response r = cpr::Post(
		cpr::Url(webhookURL),
		cpr::Body(payloadString),
		cpr::Header{ {"Content-Type", "application/json"} }
	);
	if (r.status_code != 204)
	{
		std::string error = "[ [\"success\", false], [\"status_code\", " + std::to_string(r.status_code) + "], [\"error\", \"" + r.text + "\"] ]";
		Logging::logError(("[Functions::simpleFeedEmbed::Error] " + error).c_str());
		return error;
	}
	return "[ [\"success\", true], [\"status_code\", " + std::to_string(r.status_code) + "], [\"message\", \"" + r.text + "\"] ]";
};