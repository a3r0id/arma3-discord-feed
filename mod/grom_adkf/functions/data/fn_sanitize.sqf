params["_string"];
private _out = [_string, '"', "'"] call grom_adkf_data_fnc_stringReplace;
private _out = [_out, "|", "/"] call grom_adkf_data_fnc_stringReplace; // blehhh
_out
