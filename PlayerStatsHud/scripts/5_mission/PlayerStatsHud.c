

modded class MissionServer
{
   void MissionServer()
    { 
		if (GetGame().IsServer())
		{
			GetRPCManager().AddRPC( "RPCPlayerSats", "SyncPlayersRequest", this, SingeplayerExecutionType.Server );		
		}
    }
	
		void SyncPlayersRequest( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		
		ref TFloatArray PlayerValues		= new TFloatArray;
		array<Man> players 				= new array<Man>;
		PlayerBase player;
		PlayerIdentity PlayerIdentity_;
       
		if ( type == CallType.Server && GetGame().IsServer() ) {

			GetGame().GetPlayers(players);
			for (int i = 0; i < players.Count(); ++i) {
				player = PlayerBase.Cast(players.Get(i));
				PlayerIdentity_ = player.GetIdentity();
			
				if (PlayerIdentity_.GetPlayerId() == sender.GetPlayerId()) {
					 
				PlayerValues.Insert(player.GetHealth("","Health"));
				PlayerValues.Insert(player.GetHealth("","Blood"));
				PlayerValues.Insert(player.GetStatWater().Get());
				PlayerValues.Insert(player.GetStatEnergy().Get());
				PlayerValues.Insert(player.IsAlive());
				PlayerValues.Insert(player.m_DiseaseCount);
				PlayerValues.Insert(player.m_Environment.GetTemperature());
				
		
					
						
					
				}
							
			}
			GetRPCManager().SendRPC( "RPCPlayerSats", "SyncPlayers", new Param1<ref TFloatArray> (PlayerValues), true, sender );
		
		}
	}
    
};

modded class MissionGameplay
{
   
	private  Widget m_WidgetRoot;
	private TextWidget m_pValuesListboxWidget;
	
	float ticktime = 0;
	float UpdateInterval_ = 4.0;
	int playerIsAlive = 0;
	//PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
    void MissionGameplay()
    {
      
		
		if (GetGame().IsClient())
		{
			  GetRPCManager().AddRPC( "RPCPlayerSats", "SyncPlayers", this, SingeplayerExecutionType.Client );
		
		}			   
  	
       
        
     m_WidgetRoot = GetGame().GetWorkspace().CreateWidgets("PlayerStatsHud/gui/day_z_debug_monitor.layout");
		
		
		
		m_pValuesListboxWidget = TextWidget.Cast( m_WidgetRoot.FindAnyWidget("TextWidget0") );
		
		
		


			
	
		m_WidgetRoot.Show(false);
		
    }
	
	void SyncPlayers( CallType type, ref ParamsReadContext ctx, ref PlayerIdentity sender, ref Object target ) {
		Param1<ref TFloatArray> PlayerServerData;
        ref TFloatArray ClientPlayerData = new TFloatArray;
        
		if ( type == CallType.Client && GetGame().IsClient() || !GetGame().IsMultiplayer() ) {
			if ( !ctx.Read( PlayerServerData ) ) {
			
				return;
			}

		    ClientPlayerData = PlayerServerData.param1;
			
			int PSD_Health =  ClientPlayerData[0];
			int PSD_Blood = ClientPlayerData[1];
			int PSD_Water = ClientPlayerData[2];
			int PSD_Energy = ClientPlayerData[3];
			playerIsAlive = ClientPlayerData[4];
			int PSD_Disease = ClientPlayerData[5];
			int PSD_Temp = ClientPlayerData[6];
			string Disease_Message;
			if(PSD_Disease <= 0) 
			{
				Disease_Message = " Diseases: None";
			} else 
			{
				Disease_Message = " Diseases: " + PSD_Disease.ToString();
			}
           
		
		}
           if(m_WidgetRoot.IsVisible()) {
		    m_pValuesListboxWidget.SetText(" Health: " + PSD_Health.ToString() + "/100 " + " Blood: " +  PSD_Blood.ToString() +  "/5000 " + " Water: " + PSD_Water.ToString() + "/5000 "  + " Energy: " + PSD_Energy.ToString() + "/5000 " + " Temperature: " + PSD_Temp.ToString() +  Disease_Message);
		
		}
			//Print(sHealth.ToString());
			
		
	}
    
	
	override void TickScheduler(float timeslice)
	{
		super.TickScheduler( timeslice);
		
		
		ticktime += timeslice;
		if(ticktime > UpdateInterval_) 
		{
		    ticktime = 0;
			
		
			if(playerIsAlive == 1) {
			m_WidgetRoot.Show(true);
			} else {
			
				m_WidgetRoot.Show(false);
			
			}
			
			
			
		 
			 GetRPCManager().SendRPC( "RPCPlayerSats", "SyncPlayersRequest", new Param1< int >(0),true,NULL  );
	    }	
	}
	

};