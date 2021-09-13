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
	
	// not calling SetSyncDirty since its a really lazy call, ill let something else update it for me
	void OnLocationUpdateTimer()
	{
		m_TimePlayed = StatGet(AnalyticsManagerServer.STAT_PLAYTIME) * GetGame().ServerConfigGetInt("serverTimeAcceleration");
	}
	
	int GetSurvivedTime()
	{
		return m_TimePlayed;
	}
}