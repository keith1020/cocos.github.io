

#import "../protocols/platform/ios/InterfaceShare.h"

@class FBSession;
@interface ShareFacebook : NSObject <InterfaceShare,FBLoginViewDelegate>
{
    
}

@property bool debug;
@property(readonly,atomic,retain) NSMutableDictionary* dictionaryData;
@property(atomic,retain) FBFrictionlessRecipientCache* friendCache;
@property(atomic,retain) NSArray* permissions;

- (void) configDeveloperInfo : (NSMutableDictionary*) cpInfo;
- (void) share: (NSMutableDictionary*) shareInfo;
- (void) login;
- (void) logout;
- (void) getInvitableFriends;
- (void) getFriends;
- (void) inviteFriends:(NSMutableDictionary*)inviteInfo;
- (void) getAppRequest;

- (void) Like;

- (void) setDebugMode: (BOOL) debug;
- (NSString*) getSDKVersion;
- (NSString*) getPluginVersion;
- (NSString*) getValue:(NSString*)param;
- (void) applicationOpen:(NSMutableDictionary*) cpInfo;
- (void) applicationDidBecomeActive;

@end
