
#import "AdsChartboost.h"

#define OUTPUT_LOG(...)     if (self.debug) NSLog(__VA_ARGS__);

@implementation AdsChartboost

@synthesize debug = __debug;

- (void) configDeveloperInfo: (NSMutableDictionary*) devInfo
{
    NSString* appKey = [devInfo objectForKey:@"AppID"];
    NSString* appSignature = [devInfo objectForKey:@"AppSignature"];
    if (appKey && appSignature ) {
        [Chartboost startWithAppId:appKey appSignature:appSignature delegate:nil];
    }
    NSLog(@"Chartboost inited!!");
}
- (void) showAds: (NSMutableDictionary*) info position:(int) pos
{
    
}
- (void) hideAds: (NSMutableDictionary*) info
{
    
}
- (void) queryPoints
{
    
}
- (void) spendPoints: (int) points
{
    
}
- (void) setDebugMode: (BOOL) debug
{
    
}
- (NSString*) getSDKVersion
{
    return @"2.0.4";
}
- (NSString*) getPluginVersion
{
    return @"1.0.0";
}

@end
