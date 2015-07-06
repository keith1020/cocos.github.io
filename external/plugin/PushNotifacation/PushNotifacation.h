
#import <Foundation/Foundation.h>
#import <UIKit/UIViewController.h>
#import "../protocols/platform/ios/InterfaceUser.h"

@interface PushNotifacation : NSObject <InterfaceUser>
{
}

@property BOOL debug;
@property (atomic,retain) NSString* deviceToken;
@property (atomic,retain) NSString* launchNotifacation;
@property (atomic,retain) NSString* currentNotifacation;
@property (atomic,retain) NSString* currentRemoteNotifacation;

- (void) configDeveloperInfo : (NSMutableDictionary*) cpInfo;
- (void) login;
- (void) logout;
- (BOOL) isLogined;
- (NSString*) getSessionID;
- (void) setDebugMode: (BOOL) debug;
- (NSString*) getSDKVersion;
- (NSString*) getPluginVersion;
- (void)setNotifacationToken:(NSString*)token;
- (void)receiveRemoteNotification:(NSString*)notifacation;
- (void)receiveLocalNotification:(NSString*)notifacation;
- (NSString*) getDeviceToken;
- (void) setBadgeNumber:(NSNumber*)badgeNumber;

- (void)createPushNotifacation:(NSString*)notifationName
                     AfterTime:(int) afterTimeSecond
                     SoundName:(NSString*) soundName
                   BadgeNumber:(int) badgeNumber
                        Repeat:(BOOL) isRepeat
                     AlertBody:(NSString*)alertBody
                      Priority:(int)priority;

- (void)createPushNotifacation:(NSDictionary*)params;
- (void)removePushNotifacation:(NSString*)notifationName;
- (void)refreshLocalNotifacationBadgeNumber;
- (void)removeTimecloseNotifacation:(NSNumber*)second;
- (void)removeAllNotifacation;
- (void)setLaunchNotification:(NSString*)notifacation;

- (NSString*)getLaunchNotification;
- (NSString*)getCurrentNotification;
- (void)clearRemoteNotifacation;

@end
