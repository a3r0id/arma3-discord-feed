// establish handlers to broadcast messages from the server to discord
if (GROM_ADKF_SERVER_FEED_RUNNING) exitWith {
    diag_log "[arma3-discord-feed] Error: Feed already running, exiting";
    if (hasInterface) then {
        systemChat "[arma3-discord-feed] Error: Feed already running, exiting";
    };
};

GROM_ADKF_FEED_EH_IDS = createHashMap; // killed, chat, connect, disconnect, kick, building
GROM_ADKF_SERVER_FEED_RUNNING = true;
GROM_ADKF_FEED_NAME = "Server";

// deaths & blue on blue kills
GROM_ADKF_FEED_EH_IDS set ["killed", addMissionEventHandler ["EntityKilled", {
	params ["_unit", "_killer", "_instigator", "_useEffects"];
    // systemChat format ["EntityKilled: %1, %2, %3", _unit, _killer, _instigator];
    if (isNull _instigator) then { _instigator = UAVControl vehicle _killer select 0 }; // UAV/UGV player operated road kill
    if (isNull _instigator) then { _instigator = _killer }; // player driven vehicle road kill
    if (isPlayer _unit) then {
        private _anyNearestLocation = nearestLocations [getPosATL _unit, [
            "Name",
            "NameMarine",
            "NameVillage", 
            "NameCity", 
            "NameLocal", 
            "NameCityCapital"
        ], 1000];
        private _location = "<unknown>";
        if (count _anyNearestLocation > 0) then {
            _location = str(_anyNearestLocation#0);
        };
        if ("Error: No vehicle" in (str _killer)) then {
            _message = format ["%1 was killed near %2.", name _unit, _anyNearestLocation];
        } else {
            _message = format ["%1 has killed %2 near %3.", name _instigator, name _unit, _anyNearestLocation];
        };
        if (_message == "any") then {
            systemChat "[EH::Killed] Error: _message == any";
        };
        ["Unit Killed", _message] call grom_adkf_api_fnc_simpleFeedEmbed;
    };
}]];

// chat messages
GROM_ADKF_FEED_EH_IDS set ["chat", addMissionEventHandler ["HandleChatMessage", {
	params ["_channel", "_owner", "_from", "_text", "_person", "_name", "_strID", "_forcedDisplay", "_isPlayerMessage", "_sentenceType", "_chatMessageType", "_params"];
    systemChat str _this;
    if (count(_from) > 0) then {
        private _message = format ["%1: %2", _from, _text];
        ["Chat", _message] call grom_adkf_api_fnc_simpleFeedEmbed;
    };
}]];

// player joins
GROM_ADKF_FEED_EH_IDS set ["connect", addMissionEventHandler ["PlayerConnected", {
	params ["_id", "_uid", "_name", "_jip", "_owner", "_idstr"];
    // systemChat format ["PlayerConnected: %1, %2, %3, %4, %5, %6", _id, _uid, _name, _jip, _owner, _idstr];
    private _message = format ["%1 has joined the server.", _name];
    ["User Connected", _message] call grom_adkf_api_fnc_simpleFeedEmbed;
}]];

// player leaves
// https://community.bistudio.com/wiki/Arma_3:_Mission_Event_Handlers#:~:text=Triggered%20when%20player%20disconnects%20from%20the%20game.%20Similar%20to%20onPlayerDisconnected%20event%20but%20can%20be%20stacked%20and%20contains%20the%20unit%20occupied%20by%20player%20before%20disconnect.%20Must%20be%20added%20on%20the%20server%20and%20triggers%20only%20on%20the%20server.
GROM_ADKF_FEED_EH_IDS set ["disconnect", addMissionEventHandler ["PlayerDisconnected", { 
    // systemChat format ["PlayerDisconnected: %1", _this#3];
    private _message = format ["%1 has left the server.", _this#3];
    ["User Disconnected", _message] call grom_adkf_api_fnc_simpleFeedEmbed;
}]];

// player kicks
GROM_ADKF_FEED_EH_IDS set ["kick", addMissionEventHandler ["OnUserKicked", {
    // Reasons: 0 : "TIMEOUT", 1 : "DISCONNECTED", 2 : "KICKED", 3 : "BANNED", 4 : "MISSING ADDON", 5 : "BAD CD KEY", 6 : "CD KEY IN USE", 7 : "SESSION LOCKED", 8 : "BATTLEYE", 9 : "STEAM CHECK", 10 : "DLC CONTENT", 11 : "GS TIMEOUT", 12 : "SCRIPT", 13 : "OTHER"
    params ["_networkId", "_kickTypeNumber", "_kickType", "_kickReason", "_kickMessageIncReason"];
    //systemChat format ["OnUserKicked: %1, %2, %3, %4, %5", _networkId, _kickTypeNumber, _kickType, _kickReason, _kickMessageIncReason];
    private _message = format ["%1 has been kicked from the server. Reason: %2", name player, _kickReason];
    ["User Kicked", _message] call grom_adkf_api_fnc_simpleFeedEmbed;
}]];

// building destroyed
GROM_ADKF_FEED_EH_IDS set ["building", addMissionEventHandler ["BuildingChanged", {
	params ["_from", "_to", "_isRuin"];
    systemChat format ["BuildingChanged: %1, %2, %3", _from, _to, _isRuin];
    if (_isRuin) then {
        // private _anyNearestLocation = nearestLocation [_killed, ""];
        // private _message = format ["%1 has been destroyed near %2", _from, ];
        ["Building Destroyed", "A building has been destroyed!"] call grom_adkf_api_fnc_simpleFeedEmbed;
    };
}]];





