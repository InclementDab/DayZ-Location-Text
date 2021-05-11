class TownEntry
{
	string TownName;
	string TownType;
	vector TownPosition;
	
	int GetTownSize()
	{
		switch (TownType) {
			case "Capital": return 500;
			case "City": 	return 300;
			case "Village": return 100;
		}
		
		return 0;
	}
	
	float GetTownDistance()
	{	
		return vector.Distance(GetGame().GetPlayer().GetPosition(), TownPosition);
	}
}

typedef array<ref TownEntry> TTownEntries;

class LocationTextModule: JMModuleBase
{
	protected ref LocationTextUI m_LocationTextUI;
	protected ref Timer m_LocationTimer;
	protected ref TTownEntries m_TownEntries = EnumerateTownEntries();
	
	protected TownEntry m_CurrentTown;
		
	void DebugRun()
	{
		m_CurrentTown = null;
	}
	
	LocationTextModule GetInstance()
	{
		return LocationTextModule.Cast(GetModuleManager().GetModule(LocationTextModule));
	}
	
	override void OnMissionStart()
	{	
		m_LocationTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
		m_LocationTimer.Run(10.0, this, "OnLocationUpdate", null, true);
	}	
	
	override void OnMissionFinish()
	{
		if (m_LocationTimer) {
			m_LocationTimer.Stop();
		}
		
		delete m_LocationTimer;
		delete m_TownEntries;
	}
	
	void OnLocationUpdate()
	{
		string world_name;
		GetGame().GetWorldName(world_name);
		if (world_name[0]) {
			string w1 = world_name[0];
			w1.ToUpper();
			world_name[0] = w1;
		}
		
		TownEntry town_entry = GetClosestTown();
		if (!town_entry) {
			return;
		}
		
		if (m_CurrentTown != town_entry && town_entry.GetTownDistance() < town_entry.GetTownSize()) {
			m_CurrentTown = town_entry;
			int year, month, day, hour, minute;
			GetGame().GetWorld().GetDate(year, month, day, hour, minute);
			
			// Bug is dumb but for whatever reason the LBM is causing it to log random shit to the text if its not initialized
			if (!g_LayoutBindingManager) { 
				g_LayoutBindingManager = new LayoutBindingManager();
			}
			
			string latlong = string.Format("%1°N %2°W", GetGame().GetWorld().GetLatitude(), GetGame().GetWorld().GetLongitude());
			m_LocationTextUI = new LocationTextUI(string.Format("%1, %2", m_CurrentTown.TownName, world_name), town_entry.GetTownDistance().ToString(), string.Format("%1/%2/%3 %4:%5", month.ToStringLen(2), day.ToStringLen(2), year.ToStringLen(2), hour.ToStringLen(2), minute.ToStringLen(2)), latlong);
		}
	}
		
	TownEntry GetClosestTown()
	{
		float closest = FLT_MAX;
		TownEntry result;
		if (!GetGame().GetPlayer() || !IsMissionClient()) return null;
		
		foreach (TownEntry town_entry: m_TownEntries) {
			if (town_entry.GetTownDistance() < closest) {
				result = town_entry;
				closest = town_entry.GetTownDistance();
			}
		}
		
		return result;
	}
	
	static TTownEntries EnumerateTownEntries()
	{
		TTownEntries town_positions();
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
				TownEntry town_entry();
				town_entry.TownType = GetGame().ConfigGetTextOut(string.Format("%1 %2 type", cfg, city));
				town_entry.TownName = GetGame().ConfigGetTextOut(string.Format("%1 %2 name", cfg, city));
				town_entry.TownPosition = city_position;
				town_positions.Insert(town_entry);
			}
		}
		
		return town_positions;
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