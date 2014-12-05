//
//  MOAINativeEventsIOS.cpp

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#import "MOAINativeEventsIOS.h"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	triggerEvent
	@text	Trigger a native event
 
 @in		string		eventName		The name of the triggered event
 @in		string		jsonParams		A string of json data used for input parameters
	@opt	function	callback		A function to callback when event is done.
	@out 	nil
 */
int MOAINativeEventsIOS::_triggerEvent(lua_State* L ) {
    
    MOAILuaState state ( L );
    
    cc8* eventName = lua_tostring ( state, 1 );
    cc8* jsonParams = lua_tostring ( state, 2 );
    
    string strEventName(eventName);
    
    auto instance = MOAINativeEventsIOS::Get();
    
    auto listenerItr =instance.nativeEventListeners.find(strEventName);
    
    if(listenerItr == instance.nativeEventListeners.end())
    {
        ZLLog::Print ( "No native listener for event %s",eventName);
        return 0;
    }
    
    MOAILuaGeneralCallback* callback = NULL;
    
    if ( state.IsType ( 3, LUA_TFUNCTION )) {
        MOAILuaStrongRef* funcRef = new MOAILuaStrongRef();
        funcRef->SetRef ( state, 3 );
        callback = [[MOAILuaGeneralCallback alloc] initWithLuaRef:funcRef];
    }
    
    NSError *e = nil;
    NSDictionary * paramDic = [NSJSONSerialization JSONObjectWithData:[[NSString stringWithUTF8String:jsonParams] dataUsingEncoding:NSUTF8StringEncoding]
                                                              options: NULL error: &e];

    // call the actual method
    listenerItr->second(paramDic,callback);
    return 0;
}

//----------------------------------------------------------------//
/**	@name	registerGeneralListener
	@text	registers a general lua listener for events raised on the native end
 
	@opt	function	listener		A function to call with native info
	@out 	nil
 */
int MOAINativeEventsIOS::_registerLuaEventListener(lua_State* L ) {
    
    MOAILuaState state ( L );
    
    auto instance = MOAINativeEventsIOS::Get();
    instance.luaEventsListener.SetRef( state, 1 );
    
    return 0;
}


//================================================================//
// MOAINativeEventsIOS
//================================================================//

//----------------------------------------------------------------//
MOAINativeEventsIOS::MOAINativeEventsIOS () {
    
    RTTI_SINGLE ( MOAINativeEventsIOS )
}

//----------------------------------------------------------------//
MOAINativeEventsIOS::~MOAINativeEventsIOS () {
}

//----------------------------------------------------------------//
void MOAINativeEventsIOS::RegisterLuaClass ( MOAILuaState& state ) {
    
    luaL_Reg regTable [] = {
        { "triggerEvent",	_triggerEvent },
        { "registerGeneralListener",	_registerLuaEventListener },
        { NULL, NULL }
    };
    
    luaL_register ( state, 0, regTable );
}

void MOAINativeEventsIOS::TriggerLuaEvent(NSString *eventName, NSDictionary *parameters)
{
    auto instance = MOAINativeEventsIOS::Get();
    if(instance.luaEventsListener.IsNil())
    {
        ZLLog::Print("No lua general event listener found.");
        return;
    }
    
    MOAIScopedLuaState state = instance.luaEventsListener.GetSelf ();
    lua_pushstring(state, [eventName UTF8String]);
    if(parameters == NULL)
    {
        lua_pushnil(state);
    }
    else
    {
        NSError *e = nil;
        NSData* rtnData = [NSJSONSerialization dataWithJSONObject:parameters options:NULL error:&e];
        lua_pushstring( state,[[[NSString alloc] initWithData:rtnData
                                                     encoding:NSUTF8StringEncoding] UTF8String]  );
    }
    
    state.DebugCall ( 2, 0 );
    
}
