//
//  MOAILuaGeneralCallback.mm
// Used to wrap a general lua callback method

#import "MOAILuaGeneralCallback.h"

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
    if(parameters == NULL)
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
