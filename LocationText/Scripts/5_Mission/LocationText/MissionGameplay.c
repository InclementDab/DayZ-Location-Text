class Testing: MissionGameplay
{
	protected ref LocationTextUI m_LocationTextUI;
	protected ref TTownEntries m_TownEntries;
	
	protected Town m_CurrentTown;
	
	void DebugLocationText()
	{
		m_CurrentTown = null;
	}
	
	override void OnMissionStart()
	{	
		super.OnMissionStart();
		m_TownEntries = EnumerateTownEntries();
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(OnLocationUpdate, 10000, true);
	}	
	
	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(OnLocationUpdate);
		delete m_TownEntries;
	}
	
	void OnLocationUpdate()
	{		
		Town town_entry = GetClosestTown();
		if (!town_entry) {
			return;
		}
		
		if (m_CurrentTown != town_entry && town_entry.GetDistance() < town_entry.GetSize()) {
			m_CurrentTown = town_entry;
						
			// Bug is dumb but for whatever reason the LBM is causing it to log random shit to the text if its not initialized
			if (!MVC.GetInstance()) { 
				MVC.Start();
			}
			
			m_LocationTextUI = new LocationTextUI(m_CurrentTown);
		}
	}
		
	Town GetClosestTown()
	{		
		if (!GetGame().GetPlayer() || !(GetGame().IsClient() || !GetGame().IsMultiplayer()) || !m_TownEntries) {
			return null;
		}
		
		Town result;
		float closest = FLT_MAX;
		foreach (Town town_entry: m_TownEntries) {
			if (town_entry && town_entry.GetDistance() < closest) {
				result = town_entry;
				closest = town_entry.GetDistance();
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
		string allowed_types = "Capital City Village NameCapital NameCity NameVillage";
		for (int i = 0; i < GetGame().ConfigGetChildrenCount(cfg); i++) {
			string city;
			GetGame().ConfigGetChildName(cfg, i, city);	
			vector city_position; 
			TFloatArray float_array = {};
			GetGame().ConfigGetFloatArray(string.Format("%1 %2 position", cfg, city), float_array);
			city_position[0] = float_array[0]; city_position[2] = float_array[1];
			city_position[1] = GetGame().SurfaceY(city_position[0], city_position[2]);
			
			if (allowed_types.Contains(GetGame().ConfigGetTextOut(string.Format("%1 %2 type", cfg, city)))) {
				Town town_entry();
				town_entry.Type = GetGame().ConfigGetTextOut(string.Format("%1 %2 type", cfg, city));
				town_entry.Name = GetGame().ConfigGetTextOut(string.Format("%1 %2 name", cfg, city));
				town_entry.Position = city_position;
				town_positions.Insert(town_entry);
			}
		}
		
		return town_positions;
	}
	
	TTownEntries GetTownEntries()
	{
		return m_TownEntries;
	}
}