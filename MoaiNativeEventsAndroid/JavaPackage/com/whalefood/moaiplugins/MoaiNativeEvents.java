package com.whalefood.moaiplugins;

import java.util.HashMap;
import org.codehaus.jackson.map.ObjectMapper;
import com.ziplinegames.moai.MoaiLog;


public class MoaiNativeEvents {

	public static class LuaCallback
	{
		private long CallbackRef;
		
		public LuaCallback(long callbackRef)
		{
			this.CallbackRef = callbackRef;
		}
		
		public void Run(HashMap<String,Object> params) 
		{
			try
			{
				MoaiNativeEvents.RunLuaCallback(MoaiNativeEvents.jsonSerializer.writeValueAsString(params), this.CallbackRef);
			}
			catch(Exception ex)
			{
				MoaiLog.e("Error calling lua callback method.");
				ex.printStackTrace();
			}
		}
	}
	
	public  interface EventHandler {
	    public void eventMethod(HashMap<String,Object> eventParams, LuaCallback callback);
	}
	
	
	//the main collection of event handlers
	private static HashMap<String,EventHandler> EventRegistrar = new HashMap<String, MoaiNativeEvents.EventHandler>();
	

	private static ObjectMapper jsonSerializer = new ObjectMapper();
	
	protected static native void TriggerLuaEvent(String eventName, String params);

	protected static native void RunLuaCallback( String rslt, long callbackPntr);
	
	public static void RegisterEventHandler(String eventName, EventHandler handler)
	{
		EventRegistrar.put(eventName, handler);
	}
	
	public static void TriggerLuaEvent(String eventName, HashMap<String,Object> eventParams)
	{
		String strParams = null;
		try
		{
			strParams = jsonSerializer.writeValueAsString(eventParams);
		}
		catch(Exception ex)
		{
			MoaiLog.e("Error jsonifying event params for lua event");
			return;
		}
		MoaiNativeEvents.TriggerLuaEvent(eventName, strParams);
	}
	
	@SuppressWarnings("unchecked")
	protected static void triggerEvent(String eventName, String jsnParams, long callbackPointer)
	{
		
		EventHandler handler = EventRegistrar.get(eventName);
		
		if( handler == null)
		{
			MoaiLog.w("No event registered under the name "+eventName);
			return;
		}
		
		HashMap<String,Object> inParms =  new HashMap<String, Object>();
		if(jsnParams != null &&  !jsnParams.trim().equals(""))
		{
			try {
				inParms = jsonSerializer.readValue(jsnParams, HashMap.class);
			} catch (Exception e) {
				MoaiLog.e("Error parsing params for "+eventName);
				e.printStackTrace();
				return;
			} 
		}

		handler.eventMethod(inParms, new LuaCallback(callbackPointer));
	}
}
