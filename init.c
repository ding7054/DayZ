/*void DeleteAllItemsInMap()
{
	array<EntityAI> ents= new array<EntityAI>();
	DayZPlayerUtils.SceneGetEntitiesInBox("0 0 0", "20000.0 1200.0 20000.0", ents);
	foreach (EntityAI ent: ents)
	{
		if (ent && !UndergroundStash.Cast(ent) && !TentBase.Cast(ent) && !BaseBuildingBase.Cast(ent) && !ent.IsContainer())
			GetGame().ObjectDelete(ent);
		else if (ent && ent.IsTransport())
			GetGame().ObjectDelete(ent);
	} 
};*/

void main()
{
	//INIT WEATHER BEFORE ECONOMY INIT------------------------
	Weather weather = g_Game.GetWeather();

	weather.MissionWeather(false);    // false = use weather controller from Weather.c

	weather.GetOvercast().Set( Math.RandomFloatInclusive(0.4, 0.6), 1, 0);
	weather.GetRain().Set( 0, 0, 1);
	weather.GetFog().Set( Math.RandomFloatInclusive(0.05, 0.1), 1, 0);

	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();
	
	//CEApi TestHive = GetCEApi();
    //TestHive.ExportProxyProto();
   //GetCEApi().ExportProxyData( "4096 0 4096", 8192 );
    //GetCEApi().ExportProxyData(vector.Zero, 100000); //Loot
    //TestHive.ExportClusterData() ;
   // DeleteAllItemsInMap();
	
	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 7, reset_day = 10;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
}

class CustomMission: MissionServer
{
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			int rndHlt = Math.RandomInt(55,100);
			itemEnt.SetHealth("","",rndHlt);
		}
	}

	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
		Class.CastTo(m_player, playerEnt);

		GetGame().SelectPlayer(identity, m_player);

		return m_player;
	}

	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		EntityAI itemTop;
		EntityAI itemEnt;
		ItemBase itemBs;
		float rand;

		itemTop = player.FindAttachmentBySlotName("Body");

		if ( itemTop )
		{
			itemEnt = itemTop.GetInventory().CreateInInventory("Rag");
			if ( Class.CastTo(itemBs, itemEnt ) )
				itemBs.SetQuantity(3);
				itemBs.SetCleanness( 1 );

			// SetRandomHealt	h(itemEnt);

			// itemEnt = itemClothing.GetInventory().CreateInInventory( "Rag" );
			// if ( Class.CastTo( itemBs, itemEnt ) )
			// {
			// 	itemBs.SetQuantity( 2 );
			// 	itemBs.SetCleanness( 1 );
			// }

			string chemlightArray[] = { "Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red" };
			int rndIndex = Math.RandomInt(0, 4);
			itemEnt = itemTop.GetInventory().CreateInInventory(chemlightArray[rndIndex]);
			SetRandomHealth(itemEnt);

			rand = Math.RandomFloatInclusive(0.0, 1.0);
			if ( rand < 0.35 )
				itemEnt = player.GetInventory().CreateInInventory("Apple");
			else if ( rand > 0.65 )
				itemEnt = player.GetInventory().CreateInInventory("Pear");
			else
				itemEnt = player.GetInventory().CreateInInventory("Plum");

			SetRandomHealth(itemEnt);
   			 player.GetInventory().CreateInInventory("SardinesCan");    // added items
   			 player.GetInventory().CreateInInventory("CombatKnife");    // added items
		}
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}
