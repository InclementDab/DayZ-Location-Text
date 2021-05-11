modded class PlayerBase
{
	protected ref Timer m_LocationUpdateTimer;
	
	protected int m_TimePlayed;
	
	override void Init()
	{
		super.Init();
		
		RegisterNetSyncVariableInt("m_TimePlayed");
		
		if (GetGame().IsServer()) {
			// Just pass the update real quick
			m_LocationUpdateTimer = new Timer(CALL_CATEGORY_GAMEPLAY);
			m_LocationUpdateTimer.Run(2.0, this, "OnLocationUpdateTimer", null, true);
		}
	}
	
	void ~PlayerBase()
	{
		delete m_LocationUpdateTimer;
	}
	
	void OnLocationUpdateTimer()
	{
		m_TimePlayed = StatGet(AnalyticsManagerServer.STAT_PLAYTIME);
		SetSynchDirty();
	}
	
	int GetSurvivedTime()
	{
		return m_TimePlayed;
	}
}