//
//  MOAINativeEventsBase.h
//  moai
//
//  Created by Jonah Wallerstein on 12/5/14.
//
//

#ifndef __moai__MOAINativeEventsBase__
#define __moai__MOAINativeEventsBase__

#include <moai-core/headers.h>

//================================================================//
// MOAINativeEventsIOS
//================================================================//
/**	@name	MOAINativeEventsIOS
	@text	Native event access for iOS devices.
 */
class MOAINativeEventsBase: public MOAIGlobalClass < MOAINativeEventsBase, MOAILuaObject >
{
private:
    MOAILuaStrongRef luaEventsListener;
    
    //----------------------------------------------------------------//
    static int	_triggerEvent	( lua_State* L );
    static int _registerLuaEventListener(lua_State* L );
    
public:
    
    DECL_LUA_SINGLETON ( MOAINativeEventsBase )
    
    MOAINativeEventsBase ();
    ~MOAINativeEventsBase ();
    void	RegisterLuaClass ( MOAILuaState& state );
    static void  TriggerLuaEvent(cc8* eventName, cc8* eventParams);
};
#endif
