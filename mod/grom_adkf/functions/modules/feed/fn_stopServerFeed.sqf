if (!isNil "GROM_ADKF_FEED_EH_IDS") then {
    {
        removeMissionEventHandler [_x, _y];
    } forEach GROM_ADKF_FEED_EH_IDS;
};
if (hasInterface) then {
    systemChat "GROM_ADKF_FEED_EH_IDS cleared";
};