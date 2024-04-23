params["_string"];
private _out     = _string;
private _filters = [
    ['"', "'"],
    ['|', "/"]
];
{
    _out = [_out, _x#0, _x#1] call grom_adkf_data_fnc_stringReplace;
    systemChat format ["%1 -> %2", _x#0, _x#1];
} forEach _filters;
_out
