//
//  MOAILuaGeneralCallback.h
// Used to wrap a general lua callback method

#include <moai-core/headers.h>
#import <Foundation/Foundation.h>

@interface MOAILuaGeneralCallback : NSObject
{
@private
    MOAILuaStrongRef* internalLuaRef;
};

- (id)initWithLuaRef:(MOAILuaStrongRef*) ref;

-(void) CallWithParams:(NSDictionary*) parameters;

@end
