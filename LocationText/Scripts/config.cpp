
class CfgPatches
{
	class LocationText_Scripts
	{
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Scripts", "DF_Scripts"};
	};
};

class CfgMods 
{
	class DZ_LocationText
	{
		name = "Location Text";
		dir = "LocationText";
		credits = "InclementDab";
		author = "InclementDab";
		creditsJson = "LocationText/Scripts/Data/Credits.json";
		versionPath = "LocationText/scripts/Data/Version.hpp";
		inputs = "LocationText/Scripts/Data/Inputs.xml";
		type = "mod";
		dependencies[] =
		{
			"Game", "World", "Mission"
		};
		class defs
		{
			class imageSets
			{
				files[]=
				{
					"LocationText/gui/imagesets/dayz_editor_gui.imageset"
				};
			};
			class engineScriptModule
			{
				value = "";
				files[] =
				{
					"LocationText/scripts/common",
					"LocationText/scripts/1_core"
				};
			};

			class gameScriptModule
			{
				value="";
				files[] = 
				{
					"LocationText/scripts/common",
					"LocationText/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] = 
				{
					"LocationText/scripts/common",
					"LocationText/scripts/4_World"
				};
			};

			class missionScriptModule 
			{
				value="";
				files[] = 
				{
					"LocationText/scripts/common",
					"LocationText/scripts/5_Mission"
				};
			};
		};
	};
};
