class LocationTextController: Controller
{
	string LocationTextLine0;
	string LocationTextLine1;
	string LocationTextLine2;
	string LocationTextLine3;
}

class LocationData
{	
	string TownName;
	string TownLocation;
	string SomethingElse;
	string CurrentTime;
}

class LocationTextUI: ScriptViewTemplate<LocationTextController>
{
	void LocationTextUI(LocationData location_data)
	{
		GetTemplateController().LocationTextLine0 = location_data.TownName;
		GetTemplateController().LocationTextLine1 = location_data.TownLocation;
		GetTemplateController().LocationTextLine2 = location_data.SomethingElse;
		GetTemplateController().LocationTextLine3 = location_data.CurrentTime;
		GetTemplateController().NotifyPropertyChanged();
	}
	
	override string GetLayoutFile() 
	{
		return "LocationText/layouts/LocationTextUI.layout";
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
	
	void OnLocationUpdate()
	{
		string town_name;
		float distance = GetClosestTown(town_name);
		Print(m_TownPositions.Count());
		Print(town_name);
		Print(m_CurrentTown);
		if (m_CurrentTown != town_name && distance < 100) {
			Print("yay");
			m_CurrentTown = town_name;
			
			LocationData location_data();
			location_data.TownName = town_name;
			location_data.TownLocation = distance.ToString(); // todo: coordinates
			location_data.SomethingElse = "something else";
			location_data.CurrentTime = GetGame().GetTime().ToString();
			
			thread RunLocationUpdate(location_data);
		}
	}
	
	private void RunLocationUpdate(LocationData location_data)
	{
		LocationTextUI text_ui = new LocationTextUI(location_data);
		Sleep(5000);
		delete text_ui;
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
		
		string allowed_types = "Capital City";
		
		for (int i = 0; i < GetGame().ConfigGetChildrenCount(cfg); i++) {
			string city;
			GetGame().ConfigGetChildName(cfg, i, city);			
			vector city_position = GetGame().ConfigGetVector(string.Format("%1 %2 position", cfg, city));
			if (allowed_types.Contains(GetGame().ConfigGetTextOut(string.Format("%1 %2 type", cfg, city)))) {
				town_positions.Insert(city, city_position);
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