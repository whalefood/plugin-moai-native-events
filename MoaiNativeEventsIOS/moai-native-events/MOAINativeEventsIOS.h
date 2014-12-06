

#import <moai-core/headers.h>
#import <Foundation/Foundation.h>

// A callback wrapper object
@interface MOAILuaGeneralCallback : NSObject
{
@private
    MOAILuaStrongRef* internalLuaRef;
}

- (id)initWithLuaRef:(MOAILuaStrongRef*) ref;

-(void) CallWithParams:(NSDictionary*) parameters;

@end


typedef void (^MOAIGenericEventHandler)(NSDictionary*, MOAILuaGeneralCallback*);

// The main interface for native events
@interface MOAINativeEventsIOS : NSObject

+(void)SetListener:(MOAIGenericEventHandler) listener forEvent:(NSString*)eventName;
+(void)TriggerLuaEvent:(NSString*)eventName withParameters:(NSDictionary*)parameters;
+(void)TriggerNativeEvent:(NSString*)eventName withParameters:(NSDictionary*)parameters
              andCallback:(MOAILuaGeneralCallback*) callback;

@end


