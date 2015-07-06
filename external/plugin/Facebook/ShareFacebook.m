
#import "FacebookSDK/Headers/FacebookSDK.h"
#import "ShareFacebook.h"
#import "ShareWrapper.h"
#import "ParseUtils.h"

@implementation ShareFacebook

@synthesize dictionaryData = _dictionaryData;
@synthesize friendCache = _friendCache;
@synthesize permissions = _permissions;

+ (UIViewController *)getCurrentRootViewController {
    
    UIViewController *result = nil;
    
    // Try to find the root view controller programmically
    
    // Find the top window (that is not an alert view or other window)
    UIWindow *topWindow = [[UIApplication sharedApplication] keyWindow];
    if (topWindow.windowLevel != UIWindowLevelNormal)
    {
        NSArray *windows = [[UIApplication sharedApplication] windows];
        for(topWindow in windows)
        {
            if (topWindow.windowLevel == UIWindowLevelNormal)
                break;
        }
    }
    
    UIView *rootView = [[topWindow subviews] objectAtIndex:0];
    id nextResponder = [rootView nextResponder];
    
    if ([nextResponder isKindOfClass:[UIViewController class]])
        result = nextResponder;
    else if ([topWindow respondsToSelector:@selector(rootViewController)] && topWindow.rootViewController != nil)
        result = topWindow.rootViewController;
    else
        NSAssert(NO, @"Could not find a root view controller.");
    
    return result;
}
- (float)getOSVersion
{
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    return [currSysVer floatValue];
}

- (BOOL)isFaceBookEnable
{
    if( [self getOSVersion] >= 6.0 )
    {
        return TRUE;
    }
    return FALSE;
}
- (void) configDeveloperInfo : (NSMutableDictionary*) cpInfo
{
    if( [self isFaceBookEnable] == TRUE )
    {
        NSString* appKey = [cpInfo objectForKey:@"AppID"];
        NSString* permission = [cpInfo objectForKey:@"Permission"];
        if( nil != permission ){
            self.permissions = [permission componentsSeparatedByString: @","];
        }
        [FBSettings setDefaultAppID:appKey];
        [FBSettings setLoggingBehavior:[NSSet setWithObjects:FBLoggingBehaviorAppEvents,nil]];
        [FBAppEvents activateApp];
        [FBAppEvents flush];
        if( nil == _dictionaryData )
        {
            _dictionaryData = [[[[NSMutableDictionary alloc] init] retain] autorelease];
        }
        [FBSession openActiveSessionWithReadPermissions:nil
                                           allowLoginUI:NO
                                      completionHandler:^(FBSession *session,
                                                          FBSessionState state,
                                                          NSError *error) {
                                      }];
    }
}
- (NSString*) getValue:(NSString*)Key
{
    return _dictionaryData[ Key ];
}
- (void) getUserEmail
{
    [[FBRequest requestForMe] startWithCompletionHandler:^(FBRequestConnection *connection, NSDictionary<FBGraphUser> *user, NSError *emailError) {
        if (!emailError)
        {
            NSArray* keys = [user allKeys];
            int count = (int)([keys count]);
            for (int i = 0; i < count; i++)
            {
                NSString* key = [keys objectAtIndex: i];
                id o = [user objectForKey:key];
                if( [o isKindOfClass: [NSString class]] )
                {
                    _dictionaryData[key] = [[NSString alloc] initWithString: o];
                }
            }
            [ShareWrapper onShareResult:self withRet:kLoginSuccess withMsg:nil];
        } else{
            [ShareWrapper onShareResult:self withRet:kLoginFail withMsg:nil];
        }
    }];
}
- (void) applicationDidBecomeActive
{
    if( TRUE == [self isFaceBookEnable] )
    {
        [FBSession.activeSession handleDidBecomeActive];
    }
}
- (void) login
{
    if( [self isFaceBookEnable] != TRUE )
    {
        [ShareWrapper onShareResult:self withRet:kLoginFail withMsg:nil];
    }
    if( nil == self.permissions ){
        [ShareWrapper onShareResult:self withRet:kLoginFail withMsg:nil];
        return;
    }
    if (FBSession.activeSession.state == FBSessionStateOpen
        || FBSession.activeSession.state == FBSessionStateOpenTokenExtended) {
        [self getUserEmail];
        return;
    }
    [FBSession openActiveSessionWithReadPermissions:self.permissions allowLoginUI:YES completionHandler:^(FBSession *sessionInner,
                                                                                                     FBSessionState statusInner,
                                                                                                     NSError *errorInner) {
        if( nil == errorInner )
        {
            [self getUserEmail];
        }
        else
        {
            [ShareWrapper onShareResult:self withRet:kLoginFail withMsg:nil];
        }
    }];
}
- (void) applicationOpen:(NSMutableDictionary*) cpInfo
{
    if( TRUE == [self isFaceBookEnable] )
    {
        NSString* url = [cpInfo objectForKey:@"Param1"];
        NSString* sourceApplication = [cpInfo objectForKey:@"Param2"];
        if( url && sourceApplication )
        {
            [FBAppCall handleOpenURL:[NSURL URLWithString:url] sourceApplication:sourceApplication];
        }
    }
}
- (void)logout
{
    if (FBSession.activeSession.state == FBSessionStateOpen
        || FBSession.activeSession.state == FBSessionStateOpenTokenExtended) {
        // Close the session and remove the access token from the cache
        // The session state handler (in the app delegate) will be called automatically
        [FBSession.activeSession closeAndClearTokenInformation];
        // If the session state is not any of the two "open" states when the button is clicked
    }
}
- (void) Like
{
    
}
- (void) setDebugMode: (BOOL) debug
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
- (void)trackRevenue:(NSNumber*)cent
{
    
}
- (NSDictionary*)parseURLParams:(NSString *)query {
    NSArray *pairs = [query componentsSeparatedByString:@"&"];
    NSMutableDictionary *params = [[NSMutableDictionary alloc] init];
    for (NSString *pair in pairs) {
        NSArray *kv = [pair componentsSeparatedByString:@"="];
        NSString *val =
        [[kv objectAtIndex:1]
         stringByReplacingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
        
        [params setObject:val forKey:[kv objectAtIndex:0]];
    }
    return params;
}
- (void) share: (NSMutableDictionary*) shareInfo
{
    NSString *link = [shareInfo objectForKey:@"link"];
    NSString *title = [shareInfo objectForKey:@"title"];
    NSString *caption = [shareInfo objectForKey:@"caption"];
    NSString *desc = [shareInfo objectForKey:@"description"];
    NSString *photo = [shareInfo objectForKey:@"picture"];
    // Check if the Facebook app is installed and we can present the share dialog
    FBLinkShareParams *params = [[FBLinkShareParams alloc] init];
    params.link = [NSURL URLWithString:link];
    params.name =title;
    params.caption =caption;
    params.picture = [NSURL URLWithString:photo];
    params.linkDescription = desc;
    // If the Facebook app is installed and we can present the share dialog
    if ([FBDialogs canPresentShareDialogWithParams:params]) {
        // Present share dialog
        [FBDialogs presentShareDialogWithParams:params clientState:nil
                                        handler:^(FBAppCall *call, NSDictionary *results, NSError *error) {
                                            if(error) {
                                                // An error occurred, we need to handle the error
                                                // See: https://developers.facebook.com/docs/ios/errors
                                                NSLog(@"Error publishing story: %@", error.description);
                                                [ShareWrapper onShareResult:self withRet:kShareFail withMsg:nil];
                                            } else {
                                                // Success?
                                                NSLog(@"result %@", results);
                                                if ([results objectForKey:@"postId"]) {
                                                    [ShareWrapper onShareResult:self withRet:kShareSuccess withMsg:nil];
                                                }
                                                else {
                                                    [ShareWrapper onShareResult:self withRet:kShareFail withMsg:nil];
                                                }
                                            }
                                        }];
        
        // If the Facebook app is NOT installed and we can't present the share dialog
    } else {
        // FALLBACK: publish just a link using the Feed dialog
        // Put together the dialog parameters
        NSMutableDictionary *params = [NSMutableDictionary dictionaryWithObjectsAndKeys:
                                       title, @"name",
                                       caption, @"caption",
                                       desc, @"description",
                                       link, @"link",
                                       photo, @"picture",
                                       nil];
        // Show the feed dialog
        [FBWebDialogs presentFeedDialogModallyWithSession:nil
                                               parameters:params
                                                  handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
                                                      if (error) {
                                                          // An error occurred, we need to handle the error
                                                          // See: https://developers.facebook.com/docs/ios/errors
                                                          NSLog(@"Error publishing story: %@", error.description);
                                                          [ShareWrapper onShareResult:self withRet:kShareFail withMsg:nil];
                                                      } else {
                                                          if (result == FBWebDialogResultDialogNotCompleted) {
                                                              // User canceled.
                                                              NSLog(@"User cancelled.");
                                                              [ShareWrapper onShareResult:self withRet:kShareFail withMsg:nil];
                                                          } else {
                                                              // Handle the publish feed callback
                                                              NSDictionary *urlParams = [self parseURLParams:[resultURL query]];
                                                              
                                                              if (![urlParams valueForKey:@"post_id"]) {
                                                                  // User canceled.
                                                                  NSLog(@"User cancelled.");
                                                                  [ShareWrapper onShareResult:self withRet:kShareFail withMsg:nil];
                                                              } else {
                                                                  // User clicked the Share button
                                                                  NSString *result = [NSString stringWithFormat: @"Posted story, id: %@", [urlParams valueForKey:@"post_id"]];
                                                                  NSLog(@"result %@", result);
                                                                  [ShareWrapper onShareResult:self withRet:kShareSuccess withMsg:nil];
                                                              }
                                                          }
                                                      }
                                                  }];
    }
}
- (void) inviteFriends:(NSMutableDictionary*)inviteInfo
{
    if( nil == inviteInfo ){
        [ShareWrapper onShareResult:self withRet:kInviteFriendsFailed withMsg:nil];
        return;
    }
    NSString* friendIDs = [ inviteInfo objectForKey:@"friendsIDs"];
    NSString* title = [ inviteInfo objectForKey:@"title"];
    NSString* message = [ inviteInfo objectForKey:@"message"];
    NSString* data = [ inviteInfo objectForKey:@"data"];
    if( nil == friendIDs || nil == title || nil == message ){
        [ShareWrapper onShareResult:self withRet:kInviteFriendsFailed withMsg:nil];
        return;
    }
    
    NSArray* friendsArray = [friendIDs componentsSeparatedByString:@","];
    NSMutableDictionary* params = [NSMutableDictionary dictionaryWithObjectsAndKeys: [friendsArray componentsJoinedByString:@","], @"to", nil];
    params[@"data"] = data;
    [self.friendCache prefetchAndCacheForSession:nil];
    [FBWebDialogs presentRequestsDialogModallyWithSession:FBSession.activeSession
                                                  message:message
                                                    title:title
                                               parameters:params
                                                  handler:^(FBWebDialogResult result, NSURL *resultURL, NSError *error) {
                                                      if (error) {
                                                          // Case A: Error launching the dialog or sending request.
                                                          [ShareWrapper onShareResult:self withRet:kInviteFriendsFailed withMsg:nil];
                                                      } else {
                                                          if (result == FBWebDialogResultDialogNotCompleted) {
                                                              // Case B: User clicked the "x" icon
                                                              [ShareWrapper onShareResult:self withRet:kInviteFriendsFailed withMsg:nil];
                                                          } else {
                                                              NSString* resultString = [resultURL absoluteString];
                                                              if ([resultString rangeOfString:@"error_code"].location == NSNotFound) {
                                                                  NSDictionary *params = [self parseURLParams:[resultURL query]];
                                                                  NSString* strTemp = @"";
                                                                  NSArray *sortedKeys = [[params allKeys] sortedArrayUsingSelector:@selector(localizedCaseInsensitiveCompare:)];
                                                                  for(NSString *valor in sortedKeys){
                                                                      if ([valor hasPrefix:@"to"]){
                                                                          if( [strTemp length] == 0 ){
                                                                              strTemp = [params objectForKey:valor];
                                                                          }else{
                                                                              strTemp = [NSString stringWithFormat:@"%@,%@", strTemp,[params objectForKey:valor]];
                                                                          }
                                                                      }
                                                                  }
                                                                  _dictionaryData[@"invite_result"] = strTemp;
                                                                  [ShareWrapper onShareResult:self withRet:kInviteFriendsSuccess withMsg:nil];
                                                              } else {
                                                                  [ShareWrapper onShareResult:self withRet:kInviteFriendsFailed withMsg:nil];
                                                              }
                                                          }
                                                      }}
                                              friendCache:self.friendCache];
}

- (void) getAppRequest
{
    if (FBSession.activeSession.state == FBSessionStateOpen
        || FBSession.activeSession.state == FBSessionStateOpenTokenExtended) {
        [FBRequestConnection startWithGraphPath:@"/me/apprequests"
                                     parameters:nil
                                     HTTPMethod:@"GET"
                              completionHandler:^(
                                                  FBRequestConnection *connection,
                                                  id result,
                                                  NSError *error
                                                  ) {
                                  if( nil == error ){
                                      _dictionaryData[@"apprequests"] = [NSString stringWithFormat:@"%@", [result objectForKey:@"data"]];
                                      [ShareWrapper onShareResult:self withRet:kGetAppRequestSuccess withMsg:nil];
                                  } else {
                                      [ShareWrapper onShareResult:self withRet:kGetAppRequestFailed withMsg:nil];
                                  }
                              }];
    }
    [ShareWrapper onShareResult:self withRet:kGetAppRequestFailed withMsg:nil];
}

- (void) performRequestConnection:(NSString*)url
                       Parameters:(NSMutableDictionary*)params
                    DictionaryKey:(NSString*)dicKey
                      SuccessCode:(int)successCode
                       FailedCode:(int)failedCode
{
    if (FBSession.activeSession.state == FBSessionStateOpen
        || FBSession.activeSession.state == FBSessionStateOpenTokenExtended) {
        [FBRequestConnection startWithGraphPath:url
                                     parameters:params
                                     HTTPMethod:@"GET"
                              completionHandler:^(FBRequestConnection *connection,id result,NSError *error){
                                  if (!error){
                                      ///!format is { id,name,picurl;id,name,picurl }
                                      NSArray *array = [result objectForKey:@"data"];
                                      NSUInteger count = [array count];
                                      NSString* totalString = @"";
                                      for (int i = 0; i < count; i++){
                                          NSDictionary *dic = [array objectAtIndex:i];
                                          NSString* s = [dic objectForKey:@"id"];
                                          s = [s stringByAppendingString:@","];
                                          s = [s stringByAppendingString:[dic objectForKey:@"name"]];
                                          if( nil != [dic objectForKey:@"picture"] )
                                          {
                                              NSDictionary *pictureDic = [[dic objectForKey:@"picture"] objectForKey:@"data"];
                                              if( nil != pictureDic ) {
                                                  s = [s stringByAppendingString:@","];
                                                  s = [s stringByAppendingString:[pictureDic objectForKey:@"url"]];
                                              }
                                          }
                                          if( [totalString length] > 0 )
                                          {
                                              totalString = [ totalString stringByAppendingString:@";"];
                                          }
                                          totalString = [ totalString stringByAppendingString:s];
                                      }
                                      NSString* invitableFriends = _dictionaryData[dicKey];
                                      if( [invitableFriends length] != 0 )
                                      {
                                          invitableFriends = [invitableFriends stringByAppendingString:@";"];
                                      }
                                      _dictionaryData[dicKey] = [ invitableFriends stringByAppendingString:totalString];
                                      bool hasNextPage = false;
                                      NSDictionary *paging = [result objectForKey:@"paging"];
                                      if( nil != paging )
                                      {
                                          NSString *next = [paging objectForKey:@"next"];
                                          if( nil != next )
                                          {
                                              hasNextPage = true;
                                              NSRange range = [next rangeOfString:_dictionaryData[@"id"]];
                                              NSString* substring = [[next substringFromIndex:NSMaxRange(range)] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
                                              NSString* newReqest = @"/me";
                                              newReqest = [ newReqest stringByAppendingString:substring];
                                              [self performRequestConnection:newReqest
                                                                  Parameters:nil
                                                               DictionaryKey:dicKey
                                                                 SuccessCode:successCode
                                                                  FailedCode:failedCode];
                                          }
                                      }
                                      if( false == hasNextPage )
                                      {
                                          [ShareWrapper onShareResult:self withRet:successCode withMsg:nil];
                                      }
                                      return;
                                  }
                                  else{
                                      [ShareWrapper onShareResult:self withRet:failedCode withMsg:nil];
                                      return;
                                  }
                              }];
        return;
    }
    [ShareWrapper onShareResult:self withRet:failedCode withMsg:nil];
}
- (void) getFriends
{
    if (FBSession.activeSession.state == FBSessionStateOpen
        || FBSession.activeSession.state == FBSessionStateOpenTokenExtended) {
        NSString* dictionaryKey = @"friends";
        _dictionaryData[ dictionaryKey ] = @"";
        NSMutableDictionary* params = [NSMutableDictionary dictionaryWithObjectsAndKeys: @"2048", @"limit", nil];
        params[@"fields"] = @"id,name,picture";
        [self performRequestConnection:@"/me/friends" Parameters:params DictionaryKey:dictionaryKey SuccessCode:kGetFriendsSuccess FailedCode:kGetFriendsFailed];
        return;
    }
    [ShareWrapper onShareResult:self withRet:kGetFriendsFailed withMsg:nil];
}
- (void) getInvitableFriends
{
    if (FBSession.activeSession.state == FBSessionStateOpen
        || FBSession.activeSession.state == FBSessionStateOpenTokenExtended) {
        NSString* dictionaryKey = @"invitable_friends";
        _dictionaryData[dictionaryKey] = @"";
        NSMutableDictionary* params = [NSMutableDictionary dictionaryWithObjectsAndKeys: @"2048", @"limit", nil];
        params[@"fields"] = @"id,name,picture";
        [self performRequestConnection:@"/me/invitable_friends" Parameters:params DictionaryKey:dictionaryKey SuccessCode:kGetInvitableFriendsSuccess FailedCode:kGetInvitableFriendsFailed];
        return;
    }
    [ShareWrapper onShareResult:self withRet:kGetFriendsFailed withMsg:nil];
}
- (void)loginViewShowingLoggedInUser:(FBLoginView *)loginView
{
    
}
- (void)loginViewFetchedUserInfo:(FBLoginView *)loginView
                            user:(id<FBGraphUser>)user
{
    
}
- (void)loginViewShowingLoggedOutUser:(FBLoginView *)loginView
{
    
}
- (void)loginView:(FBLoginView *)loginView
      handleError:(NSError *)error
{
    
}

@end
