#import <Foundation/Foundation.h>

typedef enum
{
    PaySuccess = 0,
    PayFail,
    PayCancel,
    PayTimeOut,
    
    GetProductsInfoSuccess,
    GetProductsInfoFailed,
} PayResult;

@interface IAPWrapper : NSObject
{
    
}

+ (void) onPayResult:(id) obj withRet:(PayResult) ret withMsg:(NSString*) msg;

@end
