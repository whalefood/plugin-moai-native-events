MOAINativeEvent = {}

local platformSpecifcObj = MOAINativeEventIOS or MOAINativeEventAndroid or nil;

MOAINativeEvent.RunningOnMobile = platformSpecifcObj ~= nil;

MOAINativeEvent.triggerEvent = function(eventName, paramTable)
    paramTable = paramTable or {};
    local paramString = MOAIJsonParser.encode(paramTable);
    if not platformSpecifcObj then 
        print("Event "..eventName..": "..paramString);
        return;
    end
    local rtnString = platformSpecifcObj.triggerEvent(eventName,paramString);
    return MOAIJsonParser.decode(rtnString);
end

MOAINativeEvent.triggerCallbackEvent = function(eventName, paramTable, callbackMethod)
    paramTable = paramTable or {};
    local paramString = MOAIJsonParser.encode(paramTable);
    if not platformSpecifcObj then 
        print("Event "..eventName..": "..paramString);
        return;
    end
    platformSpecifcObj.triggerCallbackEvent(eventName,paramString, function(json)
        if not json then callbackMethod() else
            callbackMethod( MOAIJsonParser.decode(json));
        end
    end);
end

