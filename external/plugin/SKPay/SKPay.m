
#import "SKPay.h"
#import "IAPWrapper.h"
#import "LUKeychainAccess.h"

#define IN_GAME_PURCHASE_KEY @"INGAMEPURCHASE"

@implementation SKProduct (DEStoreKitManager)

-(NSString *) localizedPrice {
    NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
    [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
    [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
    [numberFormatter setLocale:self.priceLocale];
    NSString *formattedString = [numberFormatter stringFromNumber:self.price];
    return formattedString;
}
@end
@interface SKPay ()
    @property (nonatomic) NSArray* _prodcutsArray;
    @property (nonatomic) SKProductsRequest* _productRequest;
    @property (nonatomic) NSString* _identifier;
    @property (nonatomic) NSString* _groudID;
    @property (nonatomic) NSString* _payTAG;
@end

@implementation SKPay

@synthesize _dictionaryData;
@synthesize _productPriceData;

- (NSString *)GetUUID:(NSString*)postfix TAG : (NSString*)payTag Cent:(int)moneyInCent  CurrencyCode:(NSString*)currencyCode
{
    CFUUIDRef theUUID = CFUUIDCreate(NULL);
    CFStringRef string = CFUUIDCreateString(NULL, theUUID);
    CFRelease(theUUID);
    return [NSString stringWithFormat:@"%@#%@#%@#%@#%d#%@",IN_GAME_PURCHASE_KEY,(__bridge NSString *)string,postfix,payTag,moneyInCent,currencyCode];
}

- (NSString *)GetCurrencyCode
{
    NSLocale *theLocale = [NSLocale currentLocale];
    NSString *code = [theLocale objectForKey:NSLocaleCurrencyCode];
    return code;
}

- (void)dealloc
{
    [self._payTAG release];
    [self._dictionaryData release];
    [self._productPriceData release];
    [_dictionaryData release];
    [_productPriceData release];
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
    if( nil != self._productRequest ){
        [self._productRequest release];
        self._productRequest = nil;
    }
    [super dealloc];
}
- (void) configDeveloperInfo: (NSMutableDictionary*) cpInfo
{
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    _dictionaryData = [[NSMutableDictionary alloc] init];
    _productPriceData = [[NSMutableDictionary alloc] init];
    self._payTAG = [[NSString alloc] initWithString:@""];
    self._identifier = [[NSString alloc] initWithFormat:@"%@%@",IN_GAME_PURCHASE_KEY,cpInfo[@"identifier"]];
    self._groudID = [[NSString alloc] initWithFormat:@"%@",cpInfo[@"accessGroup"]];
    [[LUKeychainAccess standardKeychainAccess] setService:self._identifier];
}
- (void)getProductsInformation:(NSString*)productsArray
{
    if( nil != self._productRequest ){
        return;
    }
    if( nil == self._productRequest && nil != productsArray ){
        NSArray *pArray = [productsArray componentsSeparatedByString:@","];
        if( nil != pArray ){
            self._productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:[NSSet setWithArray:pArray]];
            self._productRequest.delegate = self;
            [self._productRequest start];
            return;
        }
    }
    [IAPWrapper onPayResult:self withRet:GetProductsInfoFailed withMsg:@"Invalid product"];
}
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    NSArray *myProduct = response.products;
    if (myProduct.count == 0) {
        [IAPWrapper onPayResult:self withRet:GetProductsInfoFailed withMsg:@"Invalid product"];
        if( nil != self._productRequest ){
            [self._productRequest release];
            self._productRequest = nil;
        }
        return;
    }
    self._prodcutsArray = [[NSArray alloc] initWithArray:myProduct];
    for( int i = 0 ; i < myProduct.count ; i ++ ) {
        SKProduct* product = [myProduct objectAtIndex:i];
        _dictionaryData[ product.productIdentifier ] = [product localizedPrice];
        _productPriceData[ product.productIdentifier] = [NSNumber numberWithInt:[[product price] intValue] * 100];
    }
    if( nil != self._productRequest ){
        [self._productRequest release];
        self._productRequest = nil;
    }
    [IAPWrapper onPayResult:self withRet:GetProductsInfoSuccess withMsg:@"Invalid product"];
}
- (void) payForProduct: (NSMutableDictionary*) profuctInfo{
    if (![SKPaymentQueue canMakePayments]) {
        [IAPWrapper onPayResult:self withRet:PayFail withMsg:@"Cannot pay"];
        return;
    }
    NSString* productId = (NSString*)[profuctInfo objectForKey:@"productId"];
    if( [profuctInfo objectForKey:@"payTag"] != nil ){
        if( nil != self._payTAG ){
            [self._payTAG release];
        }
        self._payTAG = [[NSString alloc] initWithString: [profuctInfo objectForKey:@"payTag"]];
    }
    if( _dictionaryData[ productId ] == nil ){
        [IAPWrapper onPayResult:self withRet:PayFail withMsg:@"Cannot pay"];
        return;
    }
    for(int i=0;i<self._prodcutsArray.count;i++){
        SKProduct* product = [self._prodcutsArray objectAtIndex:i];
        if( NSOrderedSame == [product.productIdentifier compare:productId] ){
            SKPayment* payment = [SKPayment paymentWithProduct:product];
            [[SKPaymentQueue defaultQueue] addPayment:payment];
            return;
        }
    }
    [IAPWrapper onPayResult:self withRet:PayFail withMsg:@"Cannot pay"];
}
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    for (SKPaymentTransaction *transaction in transactions){
        switch (transaction.transactionState){
            case SKPaymentTransactionStatePurchased://交易完成
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed://交易失败
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored://已经购买过该商品
                [self restoreTransaction:transaction];
                break;
            case SKPaymentTransactionStatePurchasing: //商品添加进列表
                NSLog(@"商品添加进列表");
                break;
            case SKPaymentTransactionStateDeferred: //需要得到更多的信息来确认购买
                break;
            default:
                break;
        }
    }
} 
- (void)completeTransaction:(SKPaymentTransaction *)transaction {
    // Your application should implement these two methods.
    NSString * productIdentifier = transaction.payment.productIdentifier;
    NSString * receipt = [transaction.transactionReceipt base64EncodedStringWithOptions:0];
    if ([productIdentifier length] > 0) {
        [_dictionaryData setObject:productIdentifier forKey:@"productId"];
        NSString* key = [self GetUUID:self._groudID  TAG:self._payTAG Cent:[_productPriceData[transaction.payment.productIdentifier] intValue] CurrencyCode:[self GetCurrencyCode]];
        [self addReceipt: key Receipt:receipt];
        [IAPWrapper onPayResult:self withRet:PaySuccess withMsg:@"Everything is ok"];
    }
    // Remove the transaction from the payment queue.
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)failedTransaction:(SKPaymentTransaction *)transaction {
    if(transaction.error.code != SKErrorPaymentCancelled) {
        NSString* errorMsg = [transaction.error localizedDescription];
		if( nil != errorMsg ){
			[_dictionaryData setObject:errorMsg forKey:@"errorMsg"];
		}
		else{
			[_dictionaryData setObject:[NSString stringWithFormat:@"errorCode=%d", (int)(transaction.error.code)] forKey:@"errorMsg"];
		}
        [IAPWrapper onPayResult:self withRet:PayFail withMsg:errorMsg];
    } else {
        [IAPWrapper onPayResult:self withRet:PayCancel withMsg:@"Player cancelled the transaction"];
    }
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction {
    NSString * productIdentifier = transaction.payment.productIdentifier;
    NSString * receipt = [transaction.transactionReceipt base64EncodedStringWithOptions:NSDataBase64Encoding64CharacterLineLength];
    if ([productIdentifier length] > 0) {
        [_dictionaryData setObject:productIdentifier forKey:@"productId"];
        NSString* key = [self GetUUID:self._groudID  TAG:self._payTAG Cent:[_productPriceData[transaction.payment.productIdentifier] intValue] CurrencyCode:[self GetCurrencyCode]];
        [self addReceipt: key Receipt:receipt];
        [IAPWrapper onPayResult:self withRet:PaySuccess withMsg:@"Everything is ok"];
    }
    // 对于已购商品，处理恢复购买的逻辑
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
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
- (NSString*) getValue:(NSString*)key
{
    return [_dictionaryData objectForKey:key];
}
- (void)addReceipt:(NSString*)key Receipt:(NSString*)receipt{
    NSMutableDictionary* dict = [[LUKeychainAccess standardKeychainAccess] objectForKey:self._identifier];
    if( nil == dict ){
        dict = [[NSMutableDictionary alloc] init];
    }
    [dict setObject:receipt forKey:key];
    [[LUKeychainAccess standardKeychainAccess] setObject:dict forKey:self._identifier];
}
- (void)removeNoneVerifiedReceipt:(NSString*)key
{
    NSMutableDictionary* dict = [[LUKeychainAccess standardKeychainAccess] objectForKey:self._identifier];
    if( nil == dict ){
        return;
    }
    [dict removeObjectForKey:key];
    [[LUKeychainAccess standardKeychainAccess] setObject:dict forKey:self._identifier];
}
- (NSString*)getNoneVerifiedReceipt
{
    NSString* ret = @"";
    NSMutableDictionary* dict = [[LUKeychainAccess standardKeychainAccess] objectForKey:self._identifier];
    if( nil == dict ){
        return ret;
    }
    NSArray* allKeys = [dict allKeys];
    if( nil != allKeys ){
        for( int i = 0 ; i < [allKeys count]; i ++ ){
            NSString* key = [allKeys objectAtIndex:i];
            if( TRUE == [key hasPrefix:IN_GAME_PURCHASE_KEY] ){
                NSString* receipt = [dict objectForKey:key];
                ret = [ret stringByAppendingString:key];
                ret = [ret stringByAppendingString:@":"];
                ret = [ret stringByAppendingString:receipt];
                ret = [ret stringByAppendingString:@";"];
            }
        }
    }
    return ret;
}


@end
