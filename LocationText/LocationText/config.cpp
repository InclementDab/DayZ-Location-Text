class CfgPatches
{
	class LocationText_Sounds
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
		author = "InclementDab";
		name = "LocationText SFX";
	};
};

class CfgSoundShaders
{
	class LocationText_Click_Shader
	{
        volume = 1;
		limitation = 0;
		radius = 10;
		range = 10;

		samples[] = {
			{"LocationText\LocationText\sounds\counter", 1}
		};
	};
};

class CfgSoundSets
{
	class LocationText_Click
	{
        sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
        distanceFilter = "defaultDistanceFilter";
		soundShaders[] = {"LocationText_Click_Shader"};
	};
};