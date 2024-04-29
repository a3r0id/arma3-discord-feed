params ["_name", "_function", "_data"];

// Bail if no data was returned from the extension
if (count(_data) < 1) exitWith {diag_log "arma3-discord-feed: ExtensionCallback: No data returned from extension!";};

// Local server debug
if (hasInterface) then {
    systemChat format ["arma3-discord-feed: ExtensionCallback: [%1]->%2(): %3", _name, _function, _data];
};

// Get the function from the GROM_ADKF_CALLBACKS array
private _fnc = GROM_ADKF_CALLBACKS get _function;
if (isNil "_fnc") exitWith {diag_log format ["arma3-discord-feed: ExtensionCallback: Function %1 not found!", _function];};

// call the function with the data, return the result (for no good reason)..
[_data] call _fnc 