//
//  MOAINativeEventsBase.cpp


#include <moai-native-events/MOAINativeEventsBase.h>

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>

extern JavaVM* jvm;


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
    
    MOAILuaStrongRef* callback = 0;
    
    if ( state.IsType ( 3, LUA_TFUNCTION )) {
        callback = new MOAILuaStrongRef();
        callback->SetRef ( state, 3 );
    }
    
    JNI_GET_ENV ( jvm, env );
    
    JNI_GET_JSTRING ( eventName, jeventName );
    JNI_GET_JSTRING ( jsonParams, jjsonParams );
    
    jclass javaNativeEvents = env->FindClass ( "com/whalefood/moaiplugins/MoaiNativeEvents" );
    if ( javaNativeEvents == NULL ) {
        
        ZLLog::Print ( "MOAINativeEventsAndroid: Unable to find java class %s", "com/whalefood/moaiplugins" );
        
    } else {
        
        jmethodID jTriggerEvent = env->GetStaticMethodID ( javaNativeEvents, "triggerEvent", "(Ljava/lang/String;Ljava/lang/String;J)V" );
        if ( jTriggerEvent == NULL ) {
            
            ZLLog::Print ( "MOAINativeEventsAndroid: Unable to find static java method %s", "triggerEvent" );
        } else {
            
            env->CallStaticVoidMethod ( javaNativeEvents, jTriggerEvent, jeventName, jjsonParams, (jlong)callback);
        }
    }
    
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
// MOAINativeEventsBase
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
    if(MOAINativeEventsBase::Get().luaEventsListener.IsNil())
    {
        ZLLog::Print("No lua general event listener found.");
        return;
    }
    
    MOAIScopedLuaState state = MOAINativeEventsBase::Get().luaEventsListener.GetSelf ();
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

//================================================================//
// Miscellaneous JNI Functions
//================================================================//

//----------------------------------------------------------------//
extern "C"
{
    
    void Java_com_whalefood_moaiplugins_MoaiNativeEvents_RunLuaCallback ( JNIEnv* env, jclass obj, jstring rslt, jlong callbackPointer ) {
        
        cc8* cstrRslts = env->GetStringUTFChars(rslt, JNI_FALSE);
        
        if(callbackPointer == 0)
            return;
        
        MOAILuaStrongRef* funcRef = (MOAILuaStrongRef*)callbackPointer;
        
        if ( !funcRef->IsNil ()) {
            
            MOAIScopedLuaState state = funcRef->GetSelf ();
            
            lua_pushstring ( state, cstrRslts );
            
            state.DebugCall ( 1, 0 );
            
            funcRef->Clear ();
        }
        
        delete funcRef;
        
        env->ReleaseStringUTFChars(rslt, cstrRslts);
    }
    
    void Java_com_whalefood_moaiplugins_MoaiNativeEvents_TriggerLuaEvent( JNIEnv* env, jclass obj, jstring eventName, jstring parms) {
        
        cc8* cEventName = env->GetStringUTFChars(eventName, JNI_FALSE);
        cc8* cParms = env->GetStringUTFChars(parms, JNI_FALSE);
        
        MOAINativeEventsBase::TriggerLuaEvent(cEventName,cParms);
        
        env->ReleaseStringUTFChars(eventName, cEventName);
        env->ReleaseStringUTFChars(parms, cParms);
    }
    
}
