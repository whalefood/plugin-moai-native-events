#ifndef MOAINativeEventsIOS_H
#define MOAINativeEventsIOS_H

#import <Foundation/Foundation.h>
#include <moai-core/headers.h>
#import "MOAILuaGeneralCallback.h"


typedef void (^MOAIGenericEventHandler)(NSDictionary*, MOAILuaGeneralCallback*);

//================================================================//
// MOAINativeEventsIOS
//================================================================//
/**	@name	MOAINativeEventsIOS
	@text	Native event access for iOS devices.
 */
class MOAINativeEventsIOS: public MOAIGlobalClass < MOAINativeEventsIOS, MOAILuaObject >
{
private:
    
    MOAILuaStrongRef luaEventsListener;
    map<string,MOAIGenericEventHandler> nativeEventListeners;
    
    //----------------------------------------------------------------//
    static int	_triggerEvent	( lua_State* L );
    static int _registerLuaEventListener(lua_State* L );
    
public:
    
    DECL_LUA_SINGLETON ( MOAINativeEventsIOS )
    
    MOAINativeEventsIOS ();
    ~MOAINativeEventsIOS ();
    void	RegisterLuaClass ( MOAILuaState& state );
    static void SetListener(NSString* eventName, MOAIGenericEventHandler);
    static void TriggerLuaEvent(NSString * eventName, NSDictionary* parameters);
};

#endif
