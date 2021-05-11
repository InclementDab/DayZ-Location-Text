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
		delete this;
	}
	
	private void TextCrawl(string property_name, string text)
	{
		for (int i = 1; i <= text.Length(); i++) {
			EnScript.SetClassVar(GetTemplateController(), property_name, 0, text.Substring(0, i) + "|");
			GetTemplateController().NotifyPropertyChanged(property_name);
			EffectSound sound;
			GetGame().GetPlayer().PlaySoundSet(sound, "LocationText_Click", 0, 0);			
			Sleep(50);
		}
		
		Sleep(150);
		EnScript.SetClassVar(GetTemplateController(), property_name, 0, text);
		GetTemplateController().NotifyPropertyChanged(property_name);
	}
	
	private void TextDestroy(string property_name)
	{
		string text;
		EnScript.GetClassVar(GetTemplateController(), property_name, 0, text);
		for (int i = text.Length() - 1; i >= 0; i--) {
			EnScript.SetClassVar(GetTemplateController(), property_name, 0, text.Substring(0, i) + "|");
			GetTemplateController().NotifyPropertyChanged(property_name);
			EffectSound sound;
			GetGame().GetPlayer().PlaySoundSet(sound, "LocationText_Click", 0, 0);			
			Sleep(15);
		}
		
		Sleep(75);
		EnScript.SetClassVar(GetTemplateController(), property_name, 0, "");
		GetTemplateController().NotifyPropertyChanged(property_name);
	}
	
	override string GetLayoutFile() 
	{
		return "LocationText/LocationText/layouts/LocationTextUI.layout";
	}
}