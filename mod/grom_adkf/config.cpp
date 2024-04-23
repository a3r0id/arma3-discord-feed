class CfgPatches
{
	class grom_adkf
	{
		name                ="Arma 3 Discord Feed";
		author              ="Grom";
		tooltip             ="A simple Arma 3 Discord Feed, inspired by Law's DayZ killfeed.";
		tooltipOwned        ="A simple Arma 3 Discord Feed, inspired by Law's DayZ killfeed.";
		url                 ="https://github.com/a3r0id";
		units[]             = {};
		requiredVersion     = 1.0;
		requiredAddons[]    = {};
	};
};

class CfgFunctions
{
	class grom_adkf_api
	{
		class Functions
		{
			tag 	= "grom_adkf_api";
			file 	= "grom_adkf\functions\api";
            class bootstrap    				{postInit = 1;}; // (postInit)
			class handleExtensionCallback 	{}; // grom_adkf_api_fnc_handleExtensionCallback
			class simpleFeedEmbed 			{}; // grom_adkf_api_fnc_simpleFeedEmbed	
		};
	};
	class grom_adkf_data
	{
		class Functions
		{
			tag 	= "grom_adkf_data";
			file 	= "grom_adkf\functions\data";
			class stringReplace {}; // grom_adkf_data_fnc_stringReplace
			class sanitize 		{}; // grom_adkf_data_fnc_sanitize
		};
	};
    class grom_adkf_modules
    {
        class Functions
        {
			tag 	= "grom_adkf_modules";
			file 	= "grom_adkf\functions\modules";
			class startServerFeed {}; // grom_adkf_modules_fnc_startServerFeed
			class stopServerFeed  {}; // grom_adkf_modules_fnc_stopServerFeed
        };
    };	
};