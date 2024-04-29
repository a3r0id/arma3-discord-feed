GROM_ADKF_SERVER_FEED_RUNNING = false;

// This maps eventhandlers to their respective callback function, by name
// In theory, you could add your own eventhandlers in another file, but i've not tested that
GROM_ADKF_FEED_EHS = createHashMap;

// [?] deaths & blue on blue kills
GROM_ADKF_FEED_EHS set ["EntityKilled",
    {
        params ["_unit", "_killer", "_instigator", "_useEffects"];
        if (isNull _instigator) then { _instigator = UAVControl vehicle _killer select 0 }; // UAV/UGV player operated road kill
        if (isNull _instigator) then { _instigator = _killer }; // player driven vehicle road kill
        if (isPlayer _unit) then {
            private _anyNearestLocation = call grom_adkf_spatial_fnc_nearestLocationName;
            if (count _anyNearestLocation > 0) then {
                _location = str(_anyNearestLocation#0);
            };
            if ("Error: No vehicle" in (str _killer)) then {
                _message = format ["%1 was killed near %2.", name _unit, _anyNearestLocation];
            } else {
                _message = format ["%1 has killed %2 near %3.", name _instigator, name _unit, _anyNearestLocation];
            };
            if (_message == "any") then {
                if (hasInterface) then {
                    systemChat "[ADKF::EH::Killed] Error: _message == any";
                };
            };
            ["Unit Killed", _message] call grom_adkf_api_fnc_simpleFeedEmbed;
        };
    }
];

// [+] chat messages
// TODO: Show respective channel type as embed title: ie: "Side Chat"
// https://community.bistudio.com/wiki/Description.ext#disableChannels:~:text=Channel%20ID%20number%20command%20correspondence
GROM_ADKF_FEED_EHS set ["HandleChatMessage",
    {
        params ["_channel", "_owner", "_from", "_text", "_person", "_name", "_strID", "_forcedDisplay", "_isPlayerMessage", "_sentenceType", "_chatMessageType", "_params"];
        if (count(_from) > 0 && count(_text) > 0) then {
            private _message = format ["%1: %2", _from, _text];
            ["Chat", _message] call grom_adkf_api_fnc_simpleFeedEmbed;
        };
    }
];

// [+] player joins - actually, when player loads in
GROM_ADKF_FEED_EHS set ["PlayerConnected",
    {
        params ["_id", "_uid", "_name", "_jip", "_owner", "_idstr"];
        private _message = format ["%1 has joined the server.", _name];
        ["User Connected", _message] call grom_adkf_api_fnc_simpleFeedEmbed;
    }
];
// [?] Player kicked - not sure if this will work tbh
// Reasons: 0 : "TIMEOUT", 1 : "DISCONNECTED", 2 : "KICKED", 3 : "BANNED", 4 : "MISSING ADDON", 5 : "BAD CD KEY", 6 : "CD KEY IN USE", 7 : "SESSION LOCKED", 8 : "BATTLEYE", 9 : "STEAM CHECK", 10 : "DLC CONTENT", 11 : "GS TIMEOUT", 12 : "SCRIPT", 13 : "OTHER"
GROM_ADKF_FEED_EHS set ["OnUserKicked",
    {
        params ["_networkId", "_kickTypeNumber", "_kickType", "_kickReason", "_kickMessageIncReason"];
        private _message = format ["%1 has been kicked from the server. Reason: %2", name player, _kickReason];
        ["User Kicked", _message] call grom_adkf_api_fnc_simpleFeedEmbed;
    }
];
////////// SOME OLD GARBAGE BELOW //////////

// [+] building destroyed - disabled - probably not useful unless we want to track building destruction - civ reputation etc
// GROM_ADKF_FEED_EH_IDS set ["building", addMissionEventHandler ["BuildingChanged", {
// 	params ["_from", "_to", "_isRuin"];
//     systemChat format ["BuildingChanged: %1, %2, %3", _from, _to, _isRuin];
//     if (_isRuin) then {
//         ["Building Destroyed", "A building has been destroyed!"] call grom_adkf_api_fnc_simpleFeedEmbed;
//     };
// }]];

// [?] player leaves
// Issue: getting "true has left the server." when player disconnects - Note: I've change this to use name _unit instead of _name since the issue was observed.
// https://community.bistudio.com/wiki/Arma_3:_Mission_Event_Handlers#:~:text=Triggered%20when%20player%20disconnects%20from%20the%20game.%20Similar%20to%20onPlayerDisconnected%20event%20but%20can%20be%20stacked%20and%20contains%20the%20unit%20occupied%20by%20player%20before%20disconnect.%20Must%20be%20added%20on%20the%20server%20and%20triggers%20only%20on%20the%20server.
// GROM_ADKF_FEED_EH_IDS set ["disconnect", addMissionEventHandler ["PlayerDisconnected", { 
//     params ["_unit", "_id", "_uid", "_name"];
//     private _message = format ["%1 has left the server.", name _unit];
//     ["User Disconnected", _message] call grom_adkf_api_fnc_simpleFeedEmbed;
// }]];

