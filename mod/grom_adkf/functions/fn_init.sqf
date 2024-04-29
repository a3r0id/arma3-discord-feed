systemChat "Arma3-Discord-Feed: Loading extension...";


// Globals
GROM_ADKF_CONFIG = nil;
GROM_ADKF_CALLBACKS = createHashMap;

// Define callbacks - these will be called when arma receives a message from the extension

GROM_ADKF_CALLBACKS set ["getconfig", {

    systemChat "Arma3-Discord-Feed: getConfig: in callback...";
    params ["_data"];

    // Load the config
    GROM_ADKF_CONFIG = createHashMapFromArray (parseSimpleArray _data);

    // Config:
    /*
    "auto_start_feed" - bool - Whether the server feed should start automatically - use case for NOT starting is if you want to start it manually for a custom reason
    "feature_flags" - array - An array of feature flags to enable [["key", bool], ...]
    */

    // Feature Flags:
    /*
    EntityKilled
    HandleChatMessage
    PlayerConnected
    OnUserKicked

    // Experimental/Not yet implemented
    BuildingChanged
    PlayerDisconnected
    */

    // Helper function to get a boolean value from the config - saves a lot of characters...
    private _getBool = {
        params ["_key"];
        GROM_ADKF_CONFIG getOrDefault [_key, false]
    };

    // Load the feed EHs - this doesn't start the feed, just sets up the EHs and all other prerequisites
    call grom_adkf_modules_feed_fnc_initFeed;
    systemChat "Arma3-Discord-Feed: Feed initialized";

    // If the server feed is set to auto-start, start it now
    if (["auto_start_feed"] call _getBool) then {
        systemChat "Arma3-Discord-Feed: Starting server feed... (auto_start_feed is true)";
        call grom_adkf_modules_fnc_startServerFeed;
    };
}];

GROM_ADKF_CALLBACKS set ["bootstrap", {
    systemChat "Arma3-Discord-Feed: Extension bootstrap complete - calling getconfig...";
    "arma3-discord-feed" callExtension "getconfig";
}];

// Set the EXT callback EH
addMissionEventHandler ["ExtensionCallback", grom_adkf_api_fnc_handleExtensionCallback];

// Call the bootstrap function on the extension
"arma3-discord-feed" callExtension "bootstrap";
