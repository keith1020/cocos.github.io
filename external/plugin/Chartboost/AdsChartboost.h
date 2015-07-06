
#import <Foundation/Foundation.h>
#import <UIKit/UIViewController.h>
#import "../protocols/platform/ios/InterfaceAds.h"
#import "chartboost.h"

@interface AdsChartboost : NSObject <InterfaceAds,ChartboostDelegate>
{
}

@property BOOL debug;



- (void) configDeveloperInfo: (NSMutableDictionary*) devInfo;
- (void) showAds: (NSMutableDictionary*) info position:(int) pos;
- (void) hideAds: (NSMutableDictionary*) info;
- (void) queryPoints;
- (void) spendPoints: (int) points;
- (void) setDebugMode: (BOOL) debug;
- (NSString*) getSDKVersion;
- (NSString*) getPluginVersion;

@end
