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
const char* requiredFieldsConfig[] = {"name", "avatar", "webhook_url", "embed_color"};

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
		json config = {
			{"name", "Discord Feed"},
			{"avatar", "<changeme>"},
			{"webhook_url", "<changeme>"},
			{"embed_color", 0}	// https://gist.github.com/thomasbnt/b6f455e2c7d743b796917fa3c205f812
		};
		configFile << config.dump(4);
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

	// ensure that args is an object
	if (!args.is_object()) {
		return "[ [\"success\", false], [\"error\", \"Invalid JSON object\"] ]";
	}

	// if config is not an object, log error or handle the situation
	if (!config.is_object()) {
		return "[ [\"success\", false], [\"error\", \"Invalid configuration: config is not object\"] ]";
	}

	// validate the required fields in the config
	for (const char* field : requiredFieldsConfig)
	{
		if (!config.contains(field) || config[field].is_null())
		{
			return "[ [\"success\", false], [\"error\", \"Missing configuration field: " + std::string(field) + "\"] ]";
		}
	}

	// Log the config and args
	Logging::logDebug(("[Functions::simpleFeedEmbed::Config] " + config.dump()).c_str());
	Logging::logDebug(("[Functions::simpleFeedEmbed::Args] " + args.dump()).c_str());

	// ensure that the config contains the required fields
	if (!config.contains("webhook_url") || !config.contains("name") || !config.contains("avatar")) {
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
	payload["username"] = config["name"];
	payload["avatar_url"] = config["avatar"];

	// create embed object
	json embed;
	embed["title"] = args["title"];
	embed["description"] = args["description"];
	embed["color"] = config["embed_color"];
	std::vector embeds = { embed };
	payload["embeds"] = embeds;
	std::string payloadString = payload.dump();
	Logging::logDebug(("[Functions::simpleFeedEmbed::Payload] " + payloadString).c_str());
	std::string webhookUrl = config["webhook_url"];
	cpr::Response r = cpr::Post(
		cpr::Url(webhookUrl),
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