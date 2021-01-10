class LocationTextController: Controller
{
	string TownName;
	string TownLocation;
	string LocationTextLine2;
	string LocationTextLine3;
}

class LocationTextUI: ScriptViewTemplate<LocationTextController>
{	
	void LocationTextUI(string town_name, string town_location)
	{
		thread RunTextCrawl(town_name, town_location);
	}
	
	private void RunTextCrawl(string town_name, string town_location)
	{
		TextCrawl("TownName", town_name);
		Sleep(1000);
		TextCrawl("TownLocation", town_location);
		Sleep(5000);
		TextDestroy("TownName");
		TextDestroy("TownLocation");
		Delete();
	}
	
	private void TextCrawl(string property_name, string text)
	{
		string text_crawl;
		for (int i = 0; i < text.Length(); i++) {
			text_crawl += text[i];
			EnScript.SetClassVar(GetTemplateController(), property_name, 0, text_crawl);
			GetTemplateController().NotifyPropertyChanged(property_name);
			EffectSound sound;
			GetGame().GetPlayer().PlaySoundSet(sound, "LocationText_Click", 0, 0);
			Sleep(50);
		}
	}
	
	private void TextDestroy(string property_name)
	{
		string text;
		EnScript.GetClassVar(GetTemplateController(), property_name, 0, text);
		for (int i = text.Length() - 1; i >= 0; i--) {
			text[i] = "";
			EnScript.SetClassVar(GetTemplateController(), property_name, 0, text);
			GetTemplateController().NotifyPropertyChanged(property_name);
			EffectSound sound;
			GetGame().GetPlayer().PlaySoundSet(sound, "LocationText_Click", 0, 0);
			Sleep(25);
		}
	}
	
	override string GetLayoutFile() 
	{
		return "LocationText/LocationText/layouts/LocationTextUI.layout";
	}
	
	void Delete()
	{
		delete this;
	}
}

modded class JMModuleConstructor
{
	override void RegisterModules(out array<typename> modules)
	{
		super.RegisterModules(modules);
		modules.Insert(LocationTextModule);
	}
}

class LocationTextModule: JMModuleBase
{
	protected ref Timer m_LocationTimer;
	protected ref map<string, vector> m_TownPositions = new map<string,vector>();
	
	protected string m_CurrentTown;
		
	override void OnMissionStart()
	{	
		GetTownPosition(m_TownPositions);
		m_LocationTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
		m_LocationTimer.Run(10.0, this, "OnLocationUpdate", null, true);
	}	
	
	override void OnMissionFinish()
	{
		if (m_LocationTimer) {
			m_LocationTimer.Stop();
		}
		
		delete m_LocationTimer;
		delete m_TownPositions;
	}
	
	protected ref LocationTextUI m_LocationTextUI;
	
	void OnLocationUpdate()
	{
		string town_name;
		float distance = GetClosestTown(town_name);
		
		if (m_CurrentTown != town_name && distance < 500) {
			m_CurrentTown = town_name;
			m_LocationTextUI = new LocationTextUI(town_name, distance.ToString());
		}
	}
		
	float GetClosestTown(out string town_name)
	{
		float closest = FLT_MAX;
		if (!GetGame().GetPlayer() || !IsMissionClient()) return 0.0;
		foreach (string name, vector position: m_TownPositions) {
			if (vector.Distance(GetGame().GetPlayer().GetPosition(), position) < closest) {
				closest = vector.Distance(GetGame().GetPlayer().GetPosition(), position);
				town_name = name;
			}
		}
		
		return closest;
	}
	
	static void GetTownPosition(out map<string, vector> town_positions)
	{
		string world_name;
		GetGame().GetWorldName(world_name);
		string cfg = "CfgWorlds " + world_name + " Names";		
		string allowed_types = "Capital City Village";
		for (int i = 0; i < GetGame().ConfigGetChildrenCount(cfg); i++) {
			string city;
			GetGame().ConfigGetChildName(cfg, i, city);	
			vector city_position; 
			TFloatArray float_array = {};
			GetGame().ConfigGetFloatArray(string.Format("%1 %2 position", cfg, city), float_array);
			city_position[0] = float_array[0]; city_position[2] = float_array[1];
			city_position[1] = GetGame().SurfaceY(city_position[0], city_position[2]);
			
			if (allowed_types.Contains(GetGame().ConfigGetTextOut(string.Format("%1 %2 type", cfg, city)))) {
				town_positions.Insert(GetGame().ConfigGetTextOut(string.Format("%1 %2 name", cfg, city)), city_position);
			}
		}
	}
	
	override bool IsClient()
	{
		return true;
	}
	
	override bool IsServer()
	{
		return false;
	}
}