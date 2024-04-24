GROM_ADKF_SERVER_FEED_RUNNING = false;
addMissionEventHandler ["ExtensionCallback", grom_adkf_api_fnc_handleExtensionCallback];
"arma3-discord-feed" callExtension "bootstrap";
call grom_adkf_modules_fnc_startServerFeed;
