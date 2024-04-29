#include "Functions.h"
#include <fstream>
#include <filesystem>
#include <cpr/cpr.h>
#include <iostream>
#include "Logging.h"

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

void Functions::bootstrap()
{
	std::string status = "Bootstrap: ";

	// check if directory exists
	if (std::filesystem::exists("arma3-discord-feed"))
	{
		status += "Directory already exists\r\n";
		Logging::logDebug(status.c_str());
		return;
	} else {
		std::filesystem::create_directory("arma3-discord-feed");
	}

	// check if config file exists
	if (std::filesystem::exists("arma3-discord-feed/config.json"))
	{
		status += "Config file already exists\r\n";
		Logging::logDebug(status.c_str());
		return;
	} else {
		// create config file if it doesn't exist
		std::ofstream configFile("arma3-discord-feed/config.json");
		json config = {
			{"name", "Discord Feed"},
			{"avatar", "<changeme>"},
			{"webhook_url", "<changeme>"},
			{"embed_color", 0},	// https://gist.github.com/thomasbnt/b6f455e2c7d743b796917fa3c205f812
			{"auto_start_feed", true}
		};
		json featureFlags = {
			{"EntityKilled", true},
			{"HandleChatMessage", true},
			{"PlayerConnected", true},
			{"OnUserKicked", false} // experimental and VERY MUCH untested
		};
		config["feature_flags"] = featureFlags;
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
			status += "Created log file: " + std::string(logFile) + ".log\r\n";
		}
	}

	status += "Environment created!";
	Logging::logDebug(status.c_str());
}

std::string Functions::armaParsableConfig(json config)
{
    if (config.is_null() || config.empty()) {
        return "[]";
    }

    // Using vector of pairs to store string and boolean values
    std::vector<std::pair<std::string, bool>> featureFlags;

    // Check if "auto_start_feed" is present and a boolean before adding it
    if (config.contains("auto_start_feed") && config["auto_start_feed"].is_boolean()) {
        featureFlags.push_back({"auto_start_feed", config["auto_start_feed"].get<bool>()});
    }

    // Safely access "feature_flags" and iterate over them if it's an object
    if (config.contains("feature_flags") && config["feature_flags"].is_object()) {
        for (auto& it : config["feature_flags"].items()) {
            // Ensure each item is a boolean before adding
            if (it.value().is_boolean()) {
                featureFlags.push_back({it.key(), it.value().get<bool>()});
            }
        }
    }

    // Building the output string
    std::string out = "[";
    for (auto& featureFlag : featureFlags) {
        out += "[\"" + featureFlag.first + "\", " + (featureFlag.second ? "true" : "false") + "], ";
    }

    // Remove the last comma and space if the vector is not empty
    if (!featureFlags.empty()) {
        out.pop_back(); // Remove the last space
        out.pop_back(); // Remove the last comma
    }
    out += "]";

    return out;
}

std::string Functions::simpleFeedEmbed(json args, const char* type)
{

	json config;
	try { 
		config = Functions::getConfig(); 
		Logging::logDebug(("Config: " + config.dump()).c_str());
	} 
	catch (json::parse_error& e) {
		std::string out = "RVExtension Error: " + std::string(e.what());
		Logging::logError(("Failed to load config: " + out).c_str());
		return "[ [\"success\", false], [\"error\", \"" + out + "\"] ]";
	}


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

std::map<std::string, Functions::Function> Functions::functionMap = {
    {"bootstrap", Functions::Function::bootstrap},
    {"getconfig", Functions::Function::getconfig},
    {"simpleFeedEmbed", Functions::Function::simpleFeedEmbed}
};

Functions::Function Functions::getFunctionEnum(const std::string& functionName) {
    auto it = functionMap.find(functionName);
    if (it != functionMap.end()) {
        return it->second;
    }
    throw std::invalid_argument("Unknown function name");
}