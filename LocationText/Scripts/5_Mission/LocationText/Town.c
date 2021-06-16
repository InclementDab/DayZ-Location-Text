typedef array<ref Town> TTownEntries;

class Town
{
	string Name;
	string Type;
	vector Position;
	
	int GetSize()
	{
		switch (Type) {
			case "Capital": return 500;
			case "City": 	return 300;
			case "Village": return 100;
			
			// This is for incorrectly configured maps
			case "NameCapital": return 500;
			case "NameCity": return 300;
			case "NameVillage": return 100;
		}
		
		return 0;
	}
	
	float GetDistance()
	{	
		return vector.Distance(GetGame().GetPlayer().GetPosition(), Position);
	}
}