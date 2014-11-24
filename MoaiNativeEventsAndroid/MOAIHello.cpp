// Copyright (c) 2010-2011 Zipline Games, Inc. All Rights Reserved.
// http://getmoai.com

#include <moai_config.h>
#include <zl-util/headers.h>
#include <moai-core/headers.h>
#include <moai-sim/headers.h>
#include <moai-util/headers.h>

#include "MOAIHello.h"

//================================================================//
// lua
//================================================================//

//----------------------------------------------------------------//
/**	@name	world
	@text	echos Hello World! to the logs
	@in     nil
	@out	nil
*/
int MOAIHello::_world ( lua_State* L ) {
	ZLLog::Print ( "Hello Android!");
}


//================================================================//
// MOAIHello
//================================================================//

//----------------------------------------------------------------//
MOAIHello::MOAIHello () {
	RTTI_SINGLE ( MOAILuaObject )
}

//----------------------------------------------------------------//
MOAIHello::~MOAIHello () {
}

//----------------------------------------------------------------//
void MOAIHello::RegisterLuaClass ( MOAILuaState& state ) {

	
	luaL_Reg regTable[] = {
		{ "world",	_world },
		{ NULL, NULL }
	};

	luaL_register( state, 0, regTable );
}
