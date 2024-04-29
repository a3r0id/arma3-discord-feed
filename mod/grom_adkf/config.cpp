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

	class grom_adkf
	{
		class Functions
		{
			tag 	= "grom_adkf";
			file 	= "grom_adkf\functions";
			class init {postInit = 1;}; // (postInit)
		};
	};
	class grom_adkf_api
	{
		class Functions
		{
			tag 	= "grom_adkf_api";
			file 	= "grom_adkf\functions\api";
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
			class sanitize 		{}; // grom_adkf_data_fnc_sanitize
		};
	};
    class grom_adkf_modules_feed
    {
        class Functions
        {
			tag 	= "grom_adkf_modules_feed";
			file 	= "grom_adkf\functions\modules\feed";
			class initFeed		  {}; // grom_adkf_modules_feed_fnc_initFeed - automatically called from getconfig callback
			class startServerFeed {}; // grom_adkf_modules_feed_fnc_startServerFeed
			class stopServerFeed  {}; // grom_adkf_modules_feed_fnc_stopServerFeed
        };
    };	
    class grom_adkf_spatial
    {
        class Functions
        {
			tag 	= "grom_adkf_spatial";
			file 	= "grom_adkf\functions\spatial";
			class nearestLocationName {}; // grom_adkf_spatial_fnc_nearestLocationName
        };
    };		
};