modded class MissionServer
{
	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);
		player.SetHealth("", "Shock", 30);
	}
}

class LocationTextController: Controller
{
	string TownName;
	string TownLocation;
	string CurrentTime;
	string TownCoordinates;
}

class LocationTextUI: ScriptViewTemplate<LocationTextController>
{	
	void LocationTextUI(string town_name, string town_location, string current_time, string town_coordinates)
	{
		thread RunTextCrawl(town_name, town_location, current_time, town_coordinates);
	}
	
	private void RunTextCrawl(string town_name, string town_location, string current_time, string town_coordinates)
	{		
		TextCrawl("TownName", town_name);
		Sleep(1000);
		TextCrawl("CurrentTime", current_time);
		Sleep(1000);
		TextCrawl("TownCoordinates", town_coordinates);
		Sleep(1000);
		TextCrawl("TownLocation", town_location);
		Sleep(5000);
		TextDestroy("TownName");
		TextDestroy("CurrentTime");
		TextDestroy("TownCoordinates");
		TextDestroy("TownLocation");
		
		Sleep(1000);
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
			EffectSound sound = SEffectManager.CreateSound("LocationText_Click", GetGame().GetPlayer().GetPosition());
			if (sound) {
				sound.SetParent(GetGame().GetPlayer());
				AbstractWave wave;
				EnScript.GetClassVar(sound, "m_SoundWaveObject", 0, wave);
				Print(wave);
				if (wave) {
					wave.SetFrequency(2.0);
				}
				
				sound.SoundPlay();
			}
			
			Sleep(20);
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