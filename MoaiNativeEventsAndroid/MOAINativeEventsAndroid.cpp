

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include <jni.h>

#include <moai-android/moaiext-jni.h>
#include "MOAINativeEventsAndroid.h"

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
int MOAINativeEventsAndroid::_triggerEvent ( lua_State* L ) {
	
	MOAILuaState state ( L );	
	
	cc8* eventName = lua_tostring ( state, 1 );
	cc8* jsonParams = lua_tostring ( state, 2 );
	
	if ( state.IsType ( 3, LUA_TFUNCTION )) {
		
		MOAINativeEventsAndroid::Get().mEventCallback.SetRef ( state, 3 );
	}	
	
	JNI_GET_ENV ( jvm, env );

	JNI_GET_JSTRING ( eventName, jeventName );
	JNI_GET_JSTRING ( jsonParams, jjsonParams );

	jclass javaNativeEvents = env->FindClass ( "com/whalefood/MoaiNativeEventsPlugin/MoaiNativeEvents" );
    if ( javaNativeEvents == NULL ) {

		ZLLog::Print ( "MOAINativeEventsAndroid: Unable to find java class %s", "com/whalefood/MoaiNativeEvents" );
        
    } else {

    	jmethodID jTriggerEvent = env->GetStaticMethodID ( javaNativeEvents, "triggerEvent", "(Ljava/lang/String;Ljava/lang/String;)V" );
    	if ( jTriggerEvent == NULL ) {

			ZLLog::Print ( "MOAINativeEventsAndroid: Unable to find static java method %s", "triggerEvent" );
    	} else {

			env->CallStaticVoidMethod ( javaNativeEvents, jTriggerEvent, jeventName, jjsonParams);
		}
	}
	
	return 0;
}

//================================================================//
// MOAINativeEventsAndroid
//================================================================//

//----------------------------------------------------------------//
MOAINativeEventsAndroid::MOAINativeEventsAndroid () {

	RTTI_SINGLE ( MOAINativeEventsAndroid )
}

//----------------------------------------------------------------//
MOAINativeEventsAndroid::~MOAINativeEventsAndroid () {
}

//----------------------------------------------------------------//
void MOAINativeEventsAndroid::RegisterLuaClass ( MOAILuaState& state ) {

	luaL_Reg regTable [] = {
		{ "triggerEvent",	_triggerEvent },
		{ NULL, NULL }
	};

	luaL_register ( state, 0, regTable );
}

//----------------------------------------------------------------//
void MOAINativeEventsAndroid::NotifyEventFinished (  cc8* rslt ) {

	if ( !mEventCallback.IsNil ()) {
		
		MOAIScopedLuaState state = mEventCallback.GetSelf ();

		lua_pushstring ( state, rslt );
		
		state.DebugCall ( 1, 0 );
		
		mEventCallback.Clear ();
	}
}

//================================================================//
// Miscellaneous JNI Functions
//================================================================//

//----------------------------------------------------------------//
extern "C" void Java_com_whalefood_MoaiNativeEventsPlugin_MoaiNativeEvents_NotifyEventFinished ( JNIEnv* env, jclass obj, jstring rslt ) {

    cc8* cstrRslts = env->GetStringUTFChars(rslt, JNI_FALSE);

	MOAINativeEventsAndroid::Get().NotifyEventFinished ( cstrRslts );
    
     env->ReleaseStringUTFChars(rslt, cstrRslts);
    
}

extern "C" void Java_com_whalefood_MoaiNativeEventsPlugin_Test ( JNIEnv* env, jclass obj) {
    
    ZLLog::Print ( "Test worked");
}
