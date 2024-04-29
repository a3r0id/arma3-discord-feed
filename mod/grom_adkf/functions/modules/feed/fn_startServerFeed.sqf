// If feed is already running, exit
if (GROM_ADKF_SERVER_FEED_RUNNING) exitWith {
    diag_log "[arma3-discord-feed] Error: Feed already running, exiting";
    if (hasInterface) then {
        systemChat "[arma3-discord-feed] Error: Feed already running, exiting";
    };
};

// Refresh the ID's map
GROM_ADKF_FEED_EH_IDS = createHashMap; 

// Set Event handlers and capture the event handler ids for future removal
{
    GROM_ADKF_FEED_EH_IDS set [_x, addMissionEventHandler [_x#0, _x#1]];
} forEach GROM_ADKF_FEED_EHS;

// feed running flag
GROM_ADKF_SERVER_FEED_RUNNING = true;





