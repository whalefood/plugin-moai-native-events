//
//  MOAINativeEventsBase.cpp
//  moai
//
//  Created by Jonah Wallerstein on 12/5/14.
//
//

#include <moai-native-events/MOAINativeEventsBase.h>
#import <Foundation/Foundation.h>
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

int MOAINativeEventsBase::_triggerEvent(lua_State* L ) {
    
    MOAILuaState state ( L );
    
    cc8* eventName = lua_tostring ( state, 1 );
    cc8* jsonParams = lua_tostring ( state, 2 );
    
    
    NSString * stringName = [NSString stringWithUTF8String:eventName];
    string strEventName(eventName);
    
    MOAILuaGeneralCallback* callback = NULL;
    
    if ( state.IsType ( 3, LUA_TFUNCTION )) {
        MOAILuaStrongRef* funcRef = new MOAILuaStrongRef();
        funcRef->SetRef ( state, 3 );
        callback = [[MOAILuaGeneralCallback alloc] initWithLuaRef:funcRef];
    }
    
    NSError *e = nil;
    NSDictionary * paramDic = [NSJSONSerialization JSONObjectWithData:[[NSString stringWithUTF8String:jsonParams] dataUsingEncoding:NSUTF8StringEncoding]
                                                              options: NULL error: &e];
    
    [MOAINativeEventsIOS TriggerNativeEvent:stringName withParameters:paramDic andCallback:callback];
    
    return 0;
}

//----------------------------------------------------------------//
/**	@name	registerGeneralListener
	@text	registers a general lua listener for events raised on the native end
 
	@opt	function	listener		A function to call with native info
	@out 	nil
 */

int MOAINativeEventsBase::_registerLuaEventListener(lua_State* L ) {
    
    MOAILuaState state ( L );
    
    MOAINativeEventsBase::Get().luaEventsListener.SetRef( state, 1 );
    
    return 0;
}


//================================================================//
// MOAINativeEventsIOS
//================================================================//

//----------------------------------------------------------------//
MOAINativeEventsBase::MOAINativeEventsBase () {
    
    RTTI_SINGLE ( MOAINativeEventsBase )
}

//----------------------------------------------------------------//
MOAINativeEventsBase::~MOAINativeEventsBase () {
}

//----------------------------------------------------------------//
void MOAINativeEventsBase::RegisterLuaClass ( MOAILuaState& state ) {
    
    luaL_Reg regTable [] = {
        { "triggerEvent",	_triggerEvent },
        { "registerGeneralListener",	_registerLuaEventListener },
        { NULL, NULL }
    };
    
    luaL_register ( state, 0, regTable );
}

void MOAINativeEventsBase::TriggerLuaEvent(cc8 *eventName, cc8 *eventParams)
{
    auto instance = MOAINativeEventsBase::Get();
    if(instance.luaEventsListener.IsNil())
    {
        ZLLog::Print("No lua general event listener found.");
        return;
    }
    
    MOAIScopedLuaState state = instance.luaEventsListener.GetSelf ();
    lua_pushstring(state, eventName);
    if(eventParams == NULL)
    {
        lua_pushnil(state);
    }
    else
    {
        lua_pushstring(state, eventParams);
    }
    
    state.DebugCall ( 2, 0 );
}