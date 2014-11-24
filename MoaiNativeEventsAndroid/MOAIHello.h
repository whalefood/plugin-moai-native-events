#ifndef	MOAIHELLO_H
#define	MOAIHELLO_H


//================================================================//
// MOAIHello
//================================================================//
/**	@name	MOAIHello
	@text	Singleton for MOAIHello functions.
*/
class MOAIHello :
	public MOAIGlobalClass < MOAIHello, MOAILuaObject > {
private:

	//----------------------------------------------------------------//
	static int		_world		( lua_State* L );

	//----------------------------------------------------------------//

public:
	
	DECL_LUA_SINGLETON ( MOAIHello )
	
	//----------------------------------------------------------------//
					MOAIHello					();
					~MOAIHello  				();
	void			RegisterLuaClass		( MOAILuaState& state );
};

#endif
