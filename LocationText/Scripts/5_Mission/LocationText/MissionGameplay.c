modded class MissionGameplay
{
	protected ref LocationTextUI m_LocationTextUI;
	protected ref array<ref Town> m_TownEntries;
	
	protected Town m_CurrentTown;
	
	void DebugLocationText()
	{
		m_CurrentTown = null;
	}
	
	override void OnMissionStart()
	{	
		super.OnMissionStart();
		m_TownEntries = Town.GetMapTowns(TownFlags.CAPITAL | TownFlags.CITY | TownFlags.VILLAGE);
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(OnLocationUpdate, 10000, true);
	}	
	
	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		if (GetGame()) {
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(OnLocationUpdate);
		}
		
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
		
		Town closest_town;
		foreach (Town town: m_TownEntries) {
			if (!closest_town || vector.Distance(GetGame().GetPlayer().GetPosition(), town.Position) < vector.Distance(GetGame().GetPlayer().GetPosition(), closest_town.Position)) {
				closest_town = town;
			}
		}
		
		return closest_town;
	}
		
	array<ref Town> GetTownEntries()
	{
		return m_TownEntries;
	}
}