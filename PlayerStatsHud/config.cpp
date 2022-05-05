class CfgPatches
{
	class PlayerStatsHud
	{
		requiredAddons[]=
		{
			 "DZ_Data",
			 "DZ_Scripts",
			 "JM_CF_Scripts"
		};
	};
};

class CfgMods
{
	class PlayerStatsHud
	{
		type = "mod";
        dir = "PlayerStatsHud"
		class defs
		{
			
			class missionScriptModule
			{
				value = "";
				files[] = {"PlayerStatsHud/scripts/5_mission"};
			};
		};
	};
};