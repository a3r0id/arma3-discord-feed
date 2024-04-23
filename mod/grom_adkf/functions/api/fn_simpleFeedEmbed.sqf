_this spawn {
    params["_title", "_description"];
    "arma3-discord-kill-feed" callExtension format['simpleFeedEmbed|{"title": "%1", "description":"%2"}', [_title] call grom_adkf_data_fnc_sanitize, [_description] call grom_adkf_data_fnc_sanitize];    
};