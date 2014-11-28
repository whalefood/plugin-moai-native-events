package com.whalefood.moaiplugins;

import java.util.HashMap;
import org.codehaus.jackson.map.ObjectMapper;
import com.ziplinegames.moai.MoaiLog;



public class MoaiNativeEvents {

	public  interface EventHandler {
	    public HashMap<String,Object> eventMethod(HashMap<String,Object> eventParams);
	}
	
	//the main collection of event handlers
	private static HashMap<String,EventHandler> EventRegistrar = new HashMap<String, MoaiNativeEvents.EventHandler>();
	private static ObjectMapper mapper = new ObjectMapper();
	
	protected static native void NotifyEventFinished( String rslt);
	
	public static void registerEventHandler(String eventName, EventHandler handler)
	{
		EventRegistrar.put(eventName, handler);
	}
	
	@SuppressWarnings("unchecked")
	public static void triggerEvent(String eventName, String jsnParams)
	{
		EventHandler handler = EventRegistrar.get(eventName);
		
		if( handler == null)
		{
			MoaiLog.w("No event registered under the name "+eventName);
			return;
		}
		
		HashMap<String,Object> inParms =  new HashMap<String, Object>();
		try {
			inParms = mapper.readValue(jsnParams, HashMap.class);
		} catch (Exception e) {
			MoaiLog.e("Error parsing params for "+eventName);
			e.printStackTrace();
			return;
		} 

		HashMap<String,Object> rslts = handler.eventMethod(inParms);
		
		try {
			NotifyEventFinished(mapper.writeValueAsString(rslts));
		}catch (Exception e) {
			e.printStackTrace();
		} 
	}
}
