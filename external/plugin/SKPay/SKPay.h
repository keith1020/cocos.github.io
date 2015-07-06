
#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>
#import "../protocols/platform/ios/InterfaceIAP.h"

@interface SKPay : NSObject <InterfaceIAP, SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
    
}

- (void)dealloc;
- (void) configDeveloperInfo: (NSMutableDictionary*) cpInfo;
- (void) payForProduct: (NSMutableDictionary*) profuctInfo;
- (void) setDebugMode: (BOOL) debug;
- (NSString*) getSDKVersion;
- (NSString*) getPluginVersion;
- (NSString*) getValue:(NSString*)key;

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response;
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
- (void)completeTransaction:(SKPaymentTransaction *)transaction;
- (void)failedTransaction:(SKPaymentTransaction *)transaction;
- (void)restoreTransaction:(SKPaymentTransaction *)transaction;
- (void)getProductsInformation:(NSString*)productsArray;
- (NSString*)getNoneVerifiedReceipt;
- (void)removeNoneVerifiedReceipt:(NSString*)key;

@property(readonly,atomic,retain) NSMutableDictionary* _dictionaryData;
@property(readonly,atomic,retain) NSMutableDictionary* _productPriceData;

@end
