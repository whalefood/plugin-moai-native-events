//
//  MOAINativeEventsIOS.cpp

#import "MOAINativeEventsIOS.h"
#include "MOAINativeEventsBase.h"

@implementation MOAILuaGeneralCallback

- (id)initWithLuaRef:(MOAILuaStrongRef*) ref
{
    if ( self = [super init] ) {
        
        self->internalLuaRef = ref;
        return self;
    }
    return NULL;
}

-(void) CallWithParams:(NSDictionary*) parameters
{
    if(self->internalLuaRef->IsNil())
    {
        return;
    }
    
    MOAIScopedLuaState state = self->internalLuaRef->GetSelf ();
    if(parameters == NULL || [parameters count]==0)
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
    
    state.DebugCall ( 1, 0 );
    
    self->internalLuaRef->Clear();
}

-(void)dealloc {
    if(!self->internalLuaRef->IsNil())
    {
        self->internalLuaRef->Clear();
    }
    [super dealloc];
}
@end

@implementation MOAINativeEventsIOS


+(NSMutableDictionary *) EventListenerDic
{
    static NSMutableDictionary* eventListenerDic = nil;
    
    if (eventListenerDic == nil)
    {
        eventListenerDic = [[NSMutableDictionary alloc] init];
    }
    
    return eventListenerDic;
    
}


+(void)SetListener:(MOAIGenericEventHandler) listener forEvent:(NSString*)eventName
{
    [[MOAINativeEventsIOS EventListenerDic] setValue:(id)listener forKey:eventName];
}

+(void)TriggerNativeEvent:(NSString*)eventName withParameters:(NSDictionary*)parameters
              andCallback:(MOAILuaGeneralCallback*) callback;
{
    MOAIGenericEventHandler handler = (MOAIGenericEventHandler)[[MOAINativeEventsIOS EventListenerDic] valueForKey:eventName];
    if(handler == NULL)
    {
        NSLog(@"No native event listener for event %@", eventName);
        return;
    }
    
    handler(parameters, callback);
}



+(void)TriggerLuaEvent:(NSString*)eventName withParameters:(NSDictionary*)parameters
{
    cc8* luaParamsString = NULL;
    
    if(parameters != NULL)
    {
    NSError *e = nil;
    NSData* rtnData = [NSJSONSerialization dataWithJSONObject:parameters options:NULL error:&e];
        luaParamsString = [[[NSString alloc] initWithData:rtnData
                                                 encoding:NSUTF8StringEncoding] UTF8String];
    }
    MOAINativeEventsBase::TriggerLuaEvent([eventName UTF8String], luaParamsString);
    
}

@end
