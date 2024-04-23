_this spawn {
    params["_title", "_description"];
    systemChat format["simpleFeedEmbed -> %1: %2", _title, _description];
    private _titleSanitized = [_title] call grom_adkf_data_fnc_sanitize;
    private _descriptionSanitized = [_description] call grom_adkf_data_fnc_sanitize;
    systemChat format["simpleFeedEmbed [1] -> %1: %2", _title, _description];
    "arma3-discord-feed" callExtension format['simpleFeedEmbed|{"title": "%1", "description":"%2"}', _title, _description];    
};