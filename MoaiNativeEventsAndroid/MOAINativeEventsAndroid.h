

#ifndef	MOAINATIVEEVENTSANDROID_H
#define	MOAINATIVEEVENTSANDROID_H

#include <moai-core/headers.h>

//================================================================//
// MOAINativeEventsAndroid
//================================================================//
/**	@name	MOAINativeEventsAndroid
	@text	Native event access for android devices.

*/
class MOAINativeEventsAndroid :
	public MOAIGlobalClass < MOAINativeEventsAndroid, MOAILuaObject > {
private:

	MOAILuaStrongRef	mEventCallback;
	
        //----------------------------------------------------------------//
        static int	_triggerEvent	( lua_State* L );

public:
	
	DECL_LUA_SINGLETON ( MOAINativeEventsAndroid )

			MOAINativeEventsAndroid		();
			~MOAINativeEventsAndroid		();
	void	NotifyEventFinished	( cc8* rslt );
	void	RegisterLuaClass		( MOAILuaState& state );
};

#endif