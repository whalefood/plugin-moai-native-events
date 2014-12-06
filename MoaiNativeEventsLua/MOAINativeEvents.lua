MOAINativeEvents = {}

local platformSpecifcObj = MOAINativeEventsBase;

MOAINativeEvents.RunningOnMobile = platformSpecifcObj ~= nil;

MOAINativeEvents.triggerEvent = function(eventName, paramTable, callbackMethod)
    paramTable = paramTable or {};
    local paramString = MOAIJsonParser.encode(paramTable);
    if not platformSpecifcObj then 
        print("Event "..eventName..": "..paramString);
        return;
    end
    
    local callbackHelper = nil;
    if callbackMethod then
        callbackHelper = function(json)
            if not json then callbackMethod() else
                callbackMethod( MOAIJsonParser.decode(json));
            end
        end
    end
    
    platformSpecifcObj.triggerEvent(eventName,paramString, callbackHelper);
end

