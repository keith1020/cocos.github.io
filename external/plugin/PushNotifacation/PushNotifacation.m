
#import <UIKit/UIKit.h>
#import "PushNotifacation.h"
#import "../protocols/platform/ios/UserWrapper.h"

@implementation PushNotifacation

@synthesize debug = __debug;
@synthesize deviceToken = __deviceToken;
@synthesize launchNotifacation = _launchNotifacation;
@synthesize currentNotifacation = _currentNotifacation;
@synthesize currentRemoteNotifacation = _currentRemoteNotifacation;

- (void) configDeveloperInfo : (NSMutableDictionary*) cpInfo
{
    if( [[UIApplication sharedApplication] respondsToSelector:@selector(registerUserNotificationSettings:)] )
    {
        UIUserNotificationSettings* notificationSettings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeAlert | UIUserNotificationTypeBadge | UIUserNotificationTypeSound categories:nil];
        [[UIApplication sharedApplication] registerUserNotificationSettings:notificationSettings];
    }
    else
    {
        [[UIApplication sharedApplication] registerForRemoteNotificationTypes:UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeAlert | UIRemoteNotificationTypeSound];
    }
    [UIApplication sharedApplication].applicationIconBadgeNumber = 0;
}
- (void) login
{
    
}
- (void) logout
{
    
}
- (BOOL) isLogined
{
    return FALSE;
}
- (NSString*) getSessionID
{
    return @"";
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

- (void)createPushNotifacation:(NSDictionary*)params
{
    if( [params objectForKey:@"name"] == nil )
        return;
    if( [params objectForKey:@"delay"] == nil )
        return;
    if( [params objectForKey:@"sound"] == nil )
        return;
    if( [params objectForKey:@"badge"] == nil )
        return;
    if( [params objectForKey:@"repeat"] == nil )
        return;
    if( [params objectForKey:@"body"] == nil )
        return;
    if( [params objectForKey:@"priority"] == nil )
        return;
    [self createPushNotifacation: params[@"name"]
                       AfterTime: [params[@"delay"] integerValue]
                       SoundName: params[@"sound"]
                     BadgeNumber: [params[@"badge"] integerValue]
                          Repeat: [params[@"repeat"] boolValue]
                       AlertBody: params[@"body"]
                        Priority: [params[@"priority"] integerValue]];
}
- (void)removeAllNotifacation
{
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
}
- (void)removeTimecloseNotifacation:(NSNumber*)second
{
    if( nil == second )
    {
        return;
    }
    NSArray *notificationArray = [[UIApplication sharedApplication] scheduledLocalNotifications];
    NSArray *sortedAllDataArray = [notificationArray sortedArrayUsingComparator:^NSComparisonResult(UILocalNotification *dict1, UILocalNotification *dict2)
                                        {
                                            int priority1 = [[dict1.userInfo objectForKey:@"Priority"] integerValue];
                                            int priority2 = [[dict2.userInfo objectForKey:@"Priority"] integerValue];
                                            return [[NSNumber numberWithInt:priority1] compare:[NSNumber numberWithInt:priority2]];
                                        }];
    NSMutableArray* needRemoveArray = [[[NSMutableArray alloc] init] autorelease];
    for (int i = 0 ; i < [sortedAllDataArray count]; i ++ )
    {
        for( int j = i + 1 ; j < [sortedAllDataArray count] ; j ++ )
        {
            UILocalNotification* a = [sortedAllDataArray objectAtIndex:i];
            UILocalNotification* b = [sortedAllDataArray objectAtIndex:j];
            int delay1 = [[a.userInfo objectForKey:@"Delay"] integerValue];
            int delay2 = [[b.userInfo objectForKey:@"Delay"] integerValue];
            ///!删除时间非常近的
            if( fabs( delay1 - delay2) <= [second integerValue] )
            {
                BOOL needInsert = TRUE;
                for (int k = 0 ; k < [needRemoveArray count]; k ++ )
                {
                    UILocalNotification* exist = [needRemoveArray objectAtIndex:k];
                    if( exist == b )
                    {
                        needInsert = FALSE;
                    }
                }
                if( TRUE == needInsert )
                {
                    [needRemoveArray addObject:b];
                }
            }
        }
    }
    for (int i = 0 ; i < [needRemoveArray count]; i ++ )
    {
        UILocalNotification* a = [needRemoveArray objectAtIndex:i];
        NSString* existName = [a.userInfo objectForKey:@"Name"];
        [self removePushNotifacation:existName];
    }
}
- (void)refreshLocalNotifacationBadgeNumber
{
    NSArray *notificationArray = [[UIApplication sharedApplication] scheduledLocalNotifications];
    NSArray *sortedAllDataArray = [notificationArray sortedArrayUsingComparator:^NSComparisonResult(UILocalNotification *dict1, UILocalNotification *dict2)
    {
        return [dict1.fireDate compare:dict2.fireDate];
    }];
    [[UIApplication sharedApplication] cancelAllLocalNotifications];
    int badgeNumber = 1;
    for(UILocalNotification *notification in sortedAllDataArray)
    {
        notification.applicationIconBadgeNumber = badgeNumber;
        [[UIApplication sharedApplication] scheduleLocalNotification:notification];
        badgeNumber ++;
    }
}

- (void)createPushNotifacation:(NSString*)notifationName
                     AfterTime:(int) afterTimeSecond
                     SoundName:(NSString*) soundName
                   BadgeNumber:(int) badgeNumber
                        Repeat:(BOOL) isRepeat
                     AlertBody:(NSString*)alertBody
                      Priority:(int)priority
{
    [self removePushNotifacation:notifationName];
    UILocalNotification *notification = [[UILocalNotification alloc]  init];
    
    NSDate *now=[NSDate date];
    notification.fireDate = [now dateByAddingTimeInterval:afterTimeSecond];
    notification.timeZone = [NSTimeZone localTimeZone] ;
    notification.hasAction = NO;
    notification.alertBody = alertBody;
    if( TRUE == isRepeat )
    {
        notification.repeatInterval= NSMinuteCalendarUnit;
    }
    else
    {
        notification.repeatInterval= 0;
    }
    notification.soundName=soundName;
    notification.applicationIconBadgeNumber = badgeNumber;
    [notification setUserInfo:[NSDictionary dictionaryWithObjectsAndKeys:notifationName, @"Name",
                               [NSNumber numberWithInt:priority], @"Priority",
                               [NSNumber numberWithInt:afterTimeSecond], @"Delay",
                               nil]];
    [[UIApplication sharedApplication] scheduleLocalNotification:notification];
}

- (void)removePushNotifacation:(NSString*)notifationName
{
    NSArray *notificationArray = [[UIApplication sharedApplication] scheduledLocalNotifications];
    for(UILocalNotification *notification in notificationArray)
    {
        NSString* existName = [notification.userInfo objectForKey:@"Name"];
        if ( [existName compare:notifationName] == NSOrderedSame )
        {
            [[UIApplication sharedApplication] cancelLocalNotification:notification];
            break;
        }
    }
}
- (void) setBadgeNumber:(NSNumber*)badgeNumber
{
    [UIApplication sharedApplication].applicationIconBadgeNumber = [badgeNumber intValue];
}
- (NSString*) getDeviceToken
{
    return self.deviceToken;
}
- (void)receiveLocalNotification:(NSString*)info
{
    self.currentNotifacation = [[NSString alloc] initWithString:info];
    [UserWrapper onActionResult:self withRet:kUserType_2 withMsg:info];
}
- (void)setNotifacationToken:(NSString*)token
{
    self.deviceToken = [[NSString alloc] initWithString:token];
    [UserWrapper onActionResult:self withRet:kUserType_1 withMsg:token];
}
- (void)setLaunchNotification:(NSString*)notifacation
{
    self.launchNotifacation = [[NSString alloc] initWithString:notifacation];
    NSLog( @"launchNotifacation = %@", self.launchNotifacation );
}
- (NSString*)getLaunchNotification
{
    if( nil != self.launchNotifacation )
    {
        NSString* notifacation = self.launchNotifacation;
        self.launchNotifacation = nil;
        return notifacation;
    }
    return @"";
}
- (void)receiveRemoteNotification:(NSString*)info
{
    self.currentRemoteNotifacation = [[NSString alloc] initWithString:info];
    [UserWrapper onActionResult:self withRet:kUserType_3 withMsg:info];
}
- (void)clearRemoteNotifacation
{
    self.currentRemoteNotifacation = nil;
}
- (NSString*)getCurrentNotification
{
    if( nil != self.currentRemoteNotifacation )
    {
        return self.currentRemoteNotifacation;
    }
    return @"";
}
@end
