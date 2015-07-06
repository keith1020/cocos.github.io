
#import "Adjust.h"
#import "AnalyticsAdjust.h"


@implementation AnalyticsAdjust


- (void) startSession: (NSString*) appKey
{
    [Adjust appDidLaunch:appKey];
    [Adjust setLogLevel:AILogLevelInfo];
#if COCOS2D_DEBUG >= 1
    [Adjust setEnvironment:AIEnvironmentSandbox];
#else
    [Adjust setEnvironment:AIEnvironmentProduction];
#endif
    
    NSLog(@"Adjust inited!!");
}

- (void) stopSession
{
    
    
}

- (void) setSessionContinueMillis: (long) millis
{
    
}

- (void) setCaptureUncaughtException: (BOOL) isEnabled
{
    
}

- (void) setDebugMode: (BOOL) isDebugMode
{
    self.debug = isDebugMode;
}

- (void) logError: (NSString*) errorId withMsg:(NSString*) message
{
    
}

- (void) logEvent: (NSString*) eventId
{
    [Adjust trackEvent:eventId];
}

- (void) logEvent: (NSString*) eventId withParam:(NSMutableDictionary*) paramMap
{
    [Adjust trackEvent:eventId];
    NSLog(@"Adjust trackEvent %@" , eventId );
}

- (void) logTimedEventBegin: (NSString*) eventId
{
    
}

- (void) logTimedEventEnd: (NSString*) eventId
{
    
}
- (NSString*) getSDKVersion
{
    return @"1.0.0";
}

- (NSString*) getPluginVersion
{
    return @"0.0.1";
}
- (void)trackRevenue:(NSMutableDictionary*)revenueEvent
{
    if( nil == revenueEvent ){
        return;
    }
    if ([revenueEvent objectForKey:@"money"] == nil) {
        return;
    }
    if ([revenueEvent objectForKey:@"receipt"] == nil) {
        return;
    }
    if ([revenueEvent objectForKey:@"token"] == nil) {
        return;
    }
    [Adjust trackRevenue:[[revenueEvent objectForKey:@"money"] doubleValue] transactionId:[revenueEvent objectForKey:@"receipt"] forEvent:[revenueEvent objectForKey:@"token"]];
}

@end
