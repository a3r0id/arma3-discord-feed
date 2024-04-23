// A3API2.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <functional>
#include <sstream>
#include <thread>
#include <chrono>
#include "headers/RVExtensionUtil.h"
#include "classes/Logging.h"
#include "classes/Functions.h"
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

const char* EXTENSION_NAME = "arma3-discord-feed";
const char* EXTENSION_VERSION = "1.0.0";
const char* DELIMITER = "|";

void CALL_CONVENTION RVExtensionVersion(char* output, unsigned int outputSize) {
	std::string version = EXTENSION_NAME + std::string(" v") + EXTENSION_VERSION;
	strncpy(output, version.c_str(), outputSize - 1);
};

RVExtensionCallbackProc* callbackPtr = nullptr;
void CALL_CONVENTION RVExtensionRegisterCallback(RVExtensionCallbackProc* callbackProc)
{
	callbackPtr = callbackProc;
}

void CALL_CONVENTION RVExtension(char* output, unsigned int outputSize, const char* function)
{
	if (!callbackPtr)
		return;
	
	// Capture function in a local string to ensure its lifetime
	std::string fnc(function);
	// Start a new thread to handle the function
	std::thread([fnc]() {
		std::string functionName = "";
		std::string data = "";
		json dataParsed;
		// if delimiter is found, split the string
		if (fnc.find(DELIMITER) != std::string::npos)
		{
			functionName = fnc.substr(0, fnc.find(DELIMITER));
			// if any data is found after the delimiter, assign it to the data variable
			if (fnc.find(DELIMITER) != fnc.length() - 1)
			{
				data = fnc.substr(fnc.find(DELIMITER) + 1);
				try {
					Logging::logDebug(("Data: " + data).c_str());
					dataParsed = json::parse(data);
					// ensure data is a JSON object
					if (!dataParsed.is_object()) {
						std::string out = "RVExtension Error: Data must be a JSON object!";
						Logging::logError(out.c_str());
						callbackPtr(EXTENSION_NAME, functionName.c_str(), out.c_str());
						return;
					}
				}
				catch (json::parse_error& e) {
					std::string out = "RVExtension Error: " + std::string(e.what());
					Logging::logError(out.c_str());
					callbackPtr(EXTENSION_NAME, functionName.c_str(), out.c_str());
					return;
				}
			}
		}
		else
		{
			functionName = fnc;
		}

		// bootstrap environment if not already bootstrapped - assume that bootstrap was called then return if bootstrap is the function
		bool bBootstrapped = Functions::isBootstrapped();
		if (!bBootstrapped) {
			Functions::bootstrap(bBootstrapped);
			callbackPtr(EXTENSION_NAME, "bootstrap", "Environment Bootstrapped!");
		}

		if (functionName == "bootstrap") {
			return;
		}

		json config;
		try { 
			config = Functions::getConfig(); 
			Logging::logDebug(("Config: " + config.dump()).c_str());
		} 
		catch (json::parse_error& e) {
			std::string out = "RVExtension Error: " + std::string(e.what());
			Logging::logError(("Failed to load config: " + out).c_str());
			callbackPtr(EXTENSION_NAME, functionName.c_str(), out.c_str());
			return;
		}
		std::string returnValue = "";
		if (functionName == "simpleFeedEmbed") {
			returnValue = Functions::simpleFeedEmbed(dataParsed, "public_channel", config);
		} else {
			returnValue = "RVExtension Error: Function not found!";
		}
		callbackPtr(EXTENSION_NAME, functionName.c_str(), returnValue.c_str());
	}).detach();
}