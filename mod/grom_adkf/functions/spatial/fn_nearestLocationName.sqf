params[
    "_pos", 
    ["_searchRange", worldSize / 2],
    ["_locationTypes", [
        "Name", 
        "NameMarine", 
        "NameVillage", 
        "NameCity", 
        "NameLocal", 
        "NameCityCapital"
    ]],
    ["_defaultLocation", "an unknown location"]
];
private _anyNearestLocation = nearestLocations [_pos, _locationTypes, _searchRange];
private _location = _defaultLocation;
if (count _anyNearestLocation > 0) then {
    _location = str(_anyNearestLocation#0);
};
_location