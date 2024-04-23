// This is basically just for local debugging purposes, leaving this here for reference/heuristics.
if (!hasInterface) exitWith {};
params ["_name", "_function", "_data"];
if (count(_data) < 1) exitWith {systemChat "arma3-discord-kill-feed: ExtensionCallback: No data returned from extension!";};
systemChat format ["arma3-discord-kill-feed: ExtensionCallback: [%1]->%2(): %3", _name, _function, _data];
_functions = createHashMap;
_functions set ["simpleFeedEmbed", {
    params["_data"];
    _data = parseSimpleArray _data;
    // do things with _data...
    systemChat format ["arma3-discord-kill-feed: simpleFeedEmbed: Response: %1", _data];
}];