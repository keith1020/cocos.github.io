
#import <Foundation/Foundation.h>
#import "../protocols/platform/ios/InterfaceSocial.h"


@interface SocialGamecenter : NSObject <InterfaceSocial>
{
    
}

- (void) configDeveloperInfo : (NSMutableDictionary*) cpInfo;
- (void) submitScore: (NSString*) leaderboardID withScore: (long) score;
- (void) showLeaderboard: (NSString*) leaderboardID;
- (void) unlockAchievement: (NSMutableDictionary*) achInfo;
- (void) showAchievements;
- (void) setDebugMode: (BOOL) debug;
- (NSString*) getSDKVersion;
- (NSString*) getPluginVersion;
- (NSString*) getValue:(NSString*)Key;

@property(readonly,atomic,retain) NSMutableDictionary* dictionaryData;

@end
