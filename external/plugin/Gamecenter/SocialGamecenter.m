

#import "SocialGamecenter.h"
#include "AdsWrapper.h"
#include <GameKit/GameKit.h>
#include "SocialWrapper.h"

@implementation SocialGamecenter

@synthesize dictionaryData = _dictionaryData;

- (BOOL) isGameCenterAvailable
{
    // check for presence of GKLocalPlayer API
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
    // check if the device is running iOS 4.1 or later
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
    return (gcClass && osVersionSupported);
}
- (void) loginGameCenterCB:(BOOL)isSuccess Information:(NSMutableDictionary*) cpInfo
{
    dispatch_async(dispatch_get_main_queue(), ^{
        if( TRUE == isSuccess )
        {
            [SocialWrapper onSocialResult:self withRet:kLoginSuccess withMsg:@""];
        }
        else
        {
            [SocialWrapper onSocialResult:self withRet:kLoginFailed withMsg:@""];
        }
    });
}
- (NSString*) getValue:(NSString*)Key
{
    return _dictionaryData[ Key ];
}
- (void) configDeveloperInfo : (NSMutableDictionary*) cpInfo
{
    if( nil == _dictionaryData )
    {
        _dictionaryData = [[[[NSMutableDictionary alloc] init] retain] autorelease];
    }
    _dictionaryData[@"authenticated"] = @"NO";
    if( TRUE == [self isGameCenterAvailable] )
    {
        if([GKLocalPlayer localPlayer].authenticated == NO)
        {
            [[GKLocalPlayer localPlayer] authenticateWithCompletionHandler:^(NSError *error)
             {
                 if( nil == error )
                 {
                     if( [[GKLocalPlayer localPlayer] respondsToSelector:@selector(generateIdentityVerificationSignatureWithCompletionHandler:)] )
                     {
                         [[GKLocalPlayer localPlayer] generateIdentityVerificationSignatureWithCompletionHandler:^(NSURL *publicKeyUrl, NSData *signature, NSData *salt, uint64_t timestamp, NSError *verifyError) {
                             if ( nil == verifyError)
                             {
                                 _dictionaryData[@"public_key_url"] = [publicKeyUrl absoluteString];
                                 _dictionaryData[@"timestamp"] = [NSString stringWithFormat:@"%llu", timestamp];
                                 _dictionaryData[@"signature"] = [signature base64EncodedStringWithOptions:0];
                                 _dictionaryData[@"salt"] = [salt base64EncodedStringWithOptions:0];
                                 _dictionaryData[@"player_id"] = [GKLocalPlayer localPlayer].playerID;
                                 _dictionaryData[@"player_nick"] = [GKLocalPlayer localPlayer].alias;
                                 _dictionaryData[@"app_bundle_id"] = [[NSBundle mainBundle] bundleIdentifier];
                                 _dictionaryData[@"player_displayName"] = [GKLocalPlayer localPlayer].displayName;
                                 _dictionaryData[@"authenticated"] = @"YES";
                                 [self loginGameCenterCB:TRUE Information:cpInfo];
                                 
                                 NSNotificationCenter* nc = [NSNotificationCenter defaultCenter];
                                 [nc removeObserver:self];
                                 [nc addObserver:self selector:@selector(authenticationChanged) name:GKPlayerDidChangeNotificationName object:nil];
                             }
                             else
                             {
                                 _dictionaryData[@"errorCode"] = [NSString stringWithFormat:@"%ld",(long)[verifyError code]];
                                 _dictionaryData[@"errorDesc"] = [verifyError localizedDescription];
                                 [self loginGameCenterCB:FALSE Information:cpInfo];
                             }
                         }];
                     }
                 }
                 else
                 {
                     _dictionaryData[@"errorCode"] = [NSString stringWithFormat:@"%ld",(long)[error code]];
                     _dictionaryData[@"errorDesc"] = [error localizedDescription];
                     [self loginGameCenterCB:FALSE Information:cpInfo];
                 }
             }];
        }
        else
        {
            _dictionaryData[@"authenticated"] = @"YES";
            [self loginGameCenterCB:TRUE Information:cpInfo];
        }
    }
}
-(void)authenticationChanged
{
    if ([GKLocalPlayer localPlayer].isAuthenticated )
    {
        if( [[GKLocalPlayer localPlayer] respondsToSelector:@selector(generateIdentityVerificationSignatureWithCompletionHandler:)] )
        {
            [[GKLocalPlayer localPlayer] generateIdentityVerificationSignatureWithCompletionHandler:^(NSURL *publicKeyUrl, NSData *signature, NSData *salt, uint64_t timestamp, NSError *verifyError) {
                if ( nil == verifyError)
                {
                    _dictionaryData[@"public_key_url"] = [publicKeyUrl absoluteString];
                    _dictionaryData[@"timestamp"] = [NSString stringWithFormat:@"%llu", timestamp];
                    _dictionaryData[@"signature"] = [signature base64EncodedStringWithOptions:0];
                    _dictionaryData[@"salt"] = [salt base64EncodedStringWithOptions:0];
                    _dictionaryData[@"player_id"] = [GKLocalPlayer localPlayer].playerID;
                    _dictionaryData[@"player_nick"] = [GKLocalPlayer localPlayer].alias;
                    _dictionaryData[@"app_bundle_id"] = [[NSBundle mainBundle] bundleIdentifier];
                    _dictionaryData[@"player_displayName"] = [GKLocalPlayer localPlayer].displayName;
                    _dictionaryData[@"authenticated"] = @"YES";
                    [SocialWrapper onSocialResult:self withRet:kStateChanged withMsg:@""];
                }
                else
                {
                    _dictionaryData[@"errorCode"] = [NSString stringWithFormat:@"%ld",(long)[verifyError code]];
                    _dictionaryData[@"errorDesc"] = [verifyError localizedDescription];
                    [SocialWrapper onSocialResult:self withRet:kStateChanged withMsg:@""];
                }
            }];
        }
    }
    else
    {
        _dictionaryData[@"authenticated"] = @"NO";
    }
}
- (void) submitScore: (NSString*) leaderboardID withScore: (long) score
{
    
}
- (void) showLeaderboard: (NSString*) leaderboardID
{
    
}
- (void) unlockAchievement: (NSMutableDictionary*) achInfo
{
    
}
- (void) showAchievements
{
    
}
- (void) setDebugMode: (BOOL) debug
{
    
}
- (NSString*) getSDKVersion
{
    return @"0.1";
}
- (NSString*) getPluginVersion
{
    return @"0.1";
}

@end
