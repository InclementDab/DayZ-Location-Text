class LocationTextController: Controller
{
	string TownName;
	string TownLocation;
	string CurrentTime;
	string TownCoordinates;
}

class LocationTextUI: ScriptViewTemplate<LocationTextController>
{	
	protected ref map<string, bool> m_ProtectDestroyMap = new map<string, bool>();
	protected Town m_TownEntry;
	protected ref Timer m_UpdateTimer = new Timer(CALL_CATEGORY_GUI);
	
	protected string m_Location;
	protected string m_LatLong;
	protected string m_Date;
	protected string m_DaysSurvived;
	
	void LocationTextUI(Town town_entry)
	{
		m_TownEntry = town_entry;
		UpdateData();
		m_UpdateTimer.Run(0.5, this, "UpdateData", null, true);
		
		thread RunTextCrawl();
	}
	
	void ~LocationTextUI()
	{
		delete m_ProtectDestroyMap;
		delete m_UpdateTimer;
	}
	
	private void RunTextCrawl()
	{		
		thread TextCrawl("TownName", "m_Location");
		Sleep(2000);
		thread TextCrawl("CurrentTime", "m_Date");
		Sleep(2000);
		thread TextCrawl("TownCoordinates", "m_LatLong");
		Sleep(2000);
		thread TextCrawl("TownLocation", "m_DaysSurvived");
		Sleep(5000);
		thread TextDestroy("TownName");
		Sleep(1000);
		thread TextDestroy("CurrentTime");
		Sleep(1000);
		thread TextDestroy("TownCoordinates");
		Sleep(1000);
		thread TextDestroy("TownLocation");
		Sleep(1000);
		delete this;
	}
	
	private void UpdateData()
	{
		int year, month, day, hour, minute;
		GetGame().GetWorld().GetDate(year, month, day, hour, minute);
				
		m_Location = string.Format("%1, %2", m_TownEntry.Name, GetWorldName());
		m_LatLong = GetLatLong();
		m_Date = string.Format("%1/%2/%3 %4:%5", month.ToStringLen(2), day.ToStringLen(2), year.ToStringLen(2), hour.ToStringLen(2), minute.ToStringLen(2));
		m_DaysSurvived = GetTimeString(PlayerBase.Cast(GetGame().GetPlayer()).GetSurvivedTime());
	}
	
	private void TextCrawl(string property_name, string text_variable)
	{
		// registers as non-destroyed
		m_ProtectDestroyMap[property_name] = true;
		
		// Load text into view
		string text;
		EnScript.GetClassVar(this, text_variable, 0, text);
		for (int i = 1; i <= text.Length(); i++) {
			EnScript.GetClassVar(this, text_variable, 0, text);
			EnScript.SetClassVar(GetTemplateController(), property_name, 0, text.Substring(0, i) + "|");
			GetTemplateController().NotifyPropertyChanged(property_name);
			EffectSound sound;
			GetGame().GetPlayer().PlaySoundSet(sound, "LocationText_Click", 0, 0);			
			Sleep(50);
		}
		
		// Updates after the fact
		while (m_ProtectDestroyMap[property_name]) {
			EnScript.GetClassVar(this, text_variable, 0, text);
			EnScript.SetClassVar(GetTemplateController(), property_name, 0, text);
			GetTemplateController().NotifyPropertyChanged(property_name);
			Sleep(150);
		}
	}
	
	private void TextDestroy(string property_name)
	{
		m_ProtectDestroyMap[property_name] = false;
		
		string text;
		EnScript.GetClassVar(GetTemplateController(), property_name, 0, text);
		for (int i = text.Length() - 1; i >= 0; i--) {
			EnScript.SetClassVar(GetTemplateController(), property_name, 0, text.Substring(0, i) + "|");
			GetTemplateController().NotifyPropertyChanged(property_name);
			EffectSound sound;
			GetGame().GetPlayer().PlaySoundSet(sound, "LocationText_Click", 0, 0);		
			sound.SetSoundVolume(0.25);	
			Sleep(15);
		}
		
		Sleep(60);
		EnScript.SetClassVar(GetTemplateController(), property_name, 0, "");
		GetTemplateController().NotifyPropertyChanged(property_name);
	}
	
	static string GetWorldName()
	{
		string world_name;
		GetGame().GetWorldName(world_name);
		world_name.ToLower();
		
		switch (world_name) {
			case "chernarusplus": 	
				world_name = "chernarus";
				break;
			case "enoch": 			
				world_name = "livonia"; 
				break;
		}
		
		if (world_name[0]) {
			string w1 = world_name[0];
			w1.ToUpper();
			world_name[0] = w1;
		}
		
		return world_name;
	}
	
	static string GetLatLong()
	{
		float lat = GetGame().GetWorld().GetLatitude();
		float lon = GetGame().GetWorld().GetLongitude();
		
		return string.Format("%1, %2", LatLongGetFullValue(lat, "N"), LatLongGetFullValue(lon, "W"));
	}
	
	static string LatLongGetFullValue(float v, string nw)
	{
		float r1 = Math.Round(v);
		float m1 = (v - r1) * 60;
		float r2 = Math.Round(m1);
		float s1 = (m1 - r2) * 60;
		float r3 = Math.Round(s1);		
		
		return string.Format("%1°%2 %3'%4\"", r1, nw, r2, r3);
	}
	
	static string GetTimeString(int time_seconds)
	{
		return string.Format("Day %1", time_seconds / 3600 / 24);
	}	
	
	override string GetLayoutFile() 
	{
		return "LocationText/LocationText/layouts/LocationTextUI.layout";
	}
}