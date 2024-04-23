#pragma once
#include <string>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Functions {
public:
	static std::string bootstrap(bool bBootstrapped);
	static std::string simpleFeedEmbed(json args, const char* type, json config);
	static bool isBootstrapped();
	static json getConfig();
};