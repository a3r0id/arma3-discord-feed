#pragma once
#include <string>
#include <map>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

class Functions {
public:
    enum class Function {
        bootstrap,
        getconfig,
        simpleFeedEmbed
    };

    static void bootstrap();
    static std::string simpleFeedEmbed(json args, const char* type);
    static bool isBootstrapped();
    static json getConfig();
    static Function getFunctionEnum(const std::string& functionName);
	static std::string armaParsableConfig(json config);

private:
    static std::map<std::string, Function> functionMap;
};