#include "host.h"

#include <moai_config.h>
#include <zl-util/headers.h>
#include <moai-core/headers.h>
#include <moai-sim/headers.h>
#include <moai-util/headers.h>

#include "moai-core/pch.h"
#include "moai-sim/pch.h"

#include "MOAINativeEventsBase.h"

//----------------------------------------------------------------//
void MOAINativeEventsAppFinalize () {
}

//----------------------------------------------------------------//
void MOAINativeEventsAppInitialize () {

}

//----------------------------------------------------------------//
void MOAINativeEventsContextInitialize () {
    
    REGISTER_LUA_CLASS ( MOAINativeEventsBase );
}
