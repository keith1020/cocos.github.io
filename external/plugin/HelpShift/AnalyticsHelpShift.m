
#import "AnalyticsHelpShift.h"
#include "AdsWrapper.h"

@implementation AnalyticsHelpShift

@synthesize debug = __debug;

- (void) configDeveloperInfo: (NSMutableDictionary*) devInfo
{
    NSString* appKey = [devInfo objectForKey:@"AppID"];
    NSString* appSignature = [devInfo objectForKey:@"AppSignature"];
	NSString* domainName = [devInfo objectForKey:@"domainName"];
	
    if (appKey && appSignature && domainName ) {
		[Helpshift installForApiKey:appKey domainName:domainName appID:appSignature];
    }
    NSLog(@"HelpShift inited!!");
}
- (void) showAds:(NSMutableDictionary*) info position:(int) pos
{
    UIViewController* controller = [AdsWrapper getCurrentRootViewController];
    if( nil != controller )
    {
        [[Helpshift sharedInstance] showFAQs:controller withOptions:nil];
    }
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
