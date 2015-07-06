/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2012 James Chen
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "UIEditBoxImpl-ios.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define kLabelZOrder  9999

#include "UIEditBox.h"
#include "base/CCDirector.h"
#include "2d/CCLabel.h"
#import "platform/ios/CCEAGLView-ios.h"

#define getEditBoxImplIOS() ((cocos2d::ui::EditBoxImplIOS*)editBox_)

static const int CC_EDIT_BOX_PADDING = 5;

@implementation UICustomUITextField
- (CGRect)textRectForBounds:(CGRect)bounds
{
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();

    float padding = CC_EDIT_BOX_PADDING * glview->getScaleX() / glview->getContentScaleFactor();
    return CGRectMake(bounds.origin.x + padding, bounds.origin.y + padding,
                      bounds.size.width - padding*2, bounds.size.height - padding*2);
}
- (CGRect)editingRectForBounds:(CGRect)bounds {
    return [self textRectForBounds:bounds];
}
@end

@implementation CCCustomUITextView
- (CGRect)textRectForBounds:(CGRect)bounds
{
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    
    float padding = CC_EDIT_BOX_PADDING * glview->getScaleX() / glview->getContentScaleFactor();
    return CGRectMake(bounds.origin.x + padding, bounds.origin.y + padding,
                      bounds.size.width - padding*2, bounds.size.height - padding*2);
}
- (CGRect)editingRectForBounds:(CGRect)bounds {
    return [self textRectForBounds:bounds];
}
@end

@implementation UIEditBoxImplIOS_objc

@synthesize textField = textField_;
@synthesize textView = textView_;
@synthesize editState = editState_;
@synthesize editBox = editBox_;
@synthesize multilineEnable = multilineEnable_;

- (void)releaseEditBox
{
    if( nil != textField_ )
    {
        [textField_ resignFirstResponder];
        [textField_ removeFromSuperview];
    }
    if( nil != textView_ )
    {
        [textView_ resignFirstResponder];
        [textView_ removeFromSuperview];
    }
    self.textField = NULL;
    self.textView = NULL;
}
- (void)dealloc
{
    [self releaseEditBox];
    [super dealloc];
}
-(void) alloc
{
    self.multilineEnable = false;
}
-(void) setMultilineEnable:(bool)enable
{
    if( multilineEnable_ != enable )
    {
        [self releaseEditBox];
        if( true == self.multilineEnable )
        {
            [self createSingleLineBox:editBoxFrame_];
        }
        else
        {
            [self createMultilineBox:editBoxFrame_];
        }
    }
    multilineEnable_ = enable;
}
- (void)createSingleLineBox:(CGRect) frameRect
{
    if( nil == self.textField  )
    {
        self.textField = [[[UICustomUITextField alloc] initWithFrame: frameRect] autorelease];
        [textField_ setTextColor:[UIColor whiteColor]];
         //TODO: need to delete hard code here.
        textField_.font = [UIFont systemFontOfSize:frameRect.size.height*2/3];
        textField_.contentVerticalAlignment = UIControlContentVerticalAlignmentCenter;
        textField_.backgroundColor = [UIColor clearColor];
        textField_.borderStyle = UITextBorderStyleNone;
        textField_.delegate = self;
        textField_.hidden = true;
        textField_.returnKeyType = UIReturnKeyDefault;
        [textField_ addTarget:self action:@selector(textChanged) forControlEvents:UIControlEventEditingChanged];
    }
}
- (void)createMultilineBox:(CGRect) frameRect
{
    if( nil == self.textView )
    {
        self.textView = [[[CCCustomUITextView alloc] initWithFrame: frameRect] autorelease];
        [textView_ setTextColor:[UIColor whiteColor]];
        textView_.font = [UIFont systemFontOfSize:frameRect.size.height*2/3]; //TODO need to delete hard code here.
        textView_.backgroundColor = [UIColor clearColor];
        textView_.delegate = self;
        textView_.hidden = true;
        textView_.returnKeyType = UIReturnKeyDefault;
        textView_.autocorrectionType = UITextAutocorrectionTypeNo;
    }
}

-(id) initWithFrame: (CGRect) frameRect editBox: (void*) editBox
{
    self = [super init];
    if (self)
    {
        self.editBox = editBox;
        editState_ = NO;
        editBoxFrame_ = frameRect;
        multilineEnable_ = getEditBoxImplIOS()->isMultilineEnabled();
        if( true == self.multilineEnable )
        {
            [self createMultilineBox:editBoxFrame_];
        }
        else
        {
            [self createSingleLineBox:editBoxFrame_];
        }
        
    }
    return self;
}

-(void) doAnimationWhenKeyboardMoveWithDuration:(float)duration distance:(float)distance
{
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();

    [eaglview doAnimationWhenKeyboardMoveWithDuration:duration distance:distance];
}

-(void) setPosition:(CGPoint) pos
{
    if( nil != textField_ )
    {
        CGRect frame = [textField_ frame];
        frame.origin = pos;
        [textField_ setFrame:frame];
    }
    if( nil != textView_)
    {
        CGRect frame = [textView_ frame];
        frame.origin = pos;
        [textView_ setFrame:frame];
    }
}

-(void) setContentSize:(CGSize) size
{
    if( nil != textField_ )
    {
        CGRect frame = [textField_ frame];
        frame.size = size;
        [textField_ setFrame:frame];
    }
    if( nil != textView_ )
    {
        CGRect frame = [textView_ frame];
        frame.size = size;
        [textView_ setFrame:frame];
    }
}

-(void) visit
{
    
}

-(void) openKeyboard
{
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
    if( self.multilineEnable )
    {
        if( nil != textView_ )
        {
            [eaglview addSubview:textView_];
            [textView_ performSelector:@selector(becomeFirstResponder) withObject:nil afterDelay:0.1f];
        }
    }
    else
    {
        if( nil != textField_ )
        {
            [eaglview addSubview:textField_];
            [textField_ performSelector:@selector(becomeFirstResponder) withObject:nil afterDelay:0.1f];
        }
    }
}

-(void) closeKeyboard
{
    if( self.multilineEnable )
    {
        if( nil != textView_ )
        {
            [textView_ resignFirstResponder];
            [textView_ removeFromSuperview];
        }
    }
    else
    {
        if( nil != textField_ )
        {
            [textField_ resignFirstResponder];
            [textField_ removeFromSuperview];
        }
    }
}
- (BOOL)textFieldShouldReturn:(UITextField *)sender
{
    if (sender == textField_) {
        [sender resignFirstResponder];
    }
    return NO;
}

-(void)animationSelector
{
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();

    [eaglview doAnimationWhenAnotherEditBeClicked];
}

- (BOOL)shouldBeginEditing
{
    CCLOG("textFieldShouldBeginEditing...");
    editState_ = YES;
    
    auto view = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) view->getEAGLView();
    
    if ([eaglview isKeyboardShown])
    {
        [self performSelector:@selector(animationSelector) withObject:nil afterDelay:0.0f];
    }
    cocos2d::ui::EditBoxDelegate* pDelegate = getEditBoxImplIOS()->getDelegate();
    if (pDelegate != NULL)
    {
        pDelegate->editBoxEditingDidBegin(getEditBoxImplIOS()->getEditBox());
    }
    
#if CC_ENABLE_SCRIPT_BINDING
    cocos2d::ui::EditBox*  pEditBox= getEditBoxImplIOS()->getEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CommonScriptData data(pEditBox->getScriptEditBoxHandler(), "began",pEditBox);
        cocos2d::ScriptEvent event(cocos2d::kCommonEvent,(void*)&data);
        cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
#endif
    return YES;
}
- (BOOL)shouldEndEditing
{
    CCLOG("textFieldShouldEndEditing...");
    editState_ = NO;
    getEditBoxImplIOS()->refreshInactiveText();
    
    cocos2d::ui::EditBoxDelegate* pDelegate = getEditBoxImplIOS()->getDelegate();
    if (pDelegate != NULL)
    {
        pDelegate->editBoxEditingDidEnd(getEditBoxImplIOS()->getEditBox());
        pDelegate->editBoxReturn(getEditBoxImplIOS()->getEditBox());
    }
    
#if CC_ENABLE_SCRIPT_BINDING
    cocos2d::ui::EditBox*  pEditBox= getEditBoxImplIOS()->getEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CommonScriptData data(pEditBox->getScriptEditBoxHandler(), "ended",pEditBox);
        cocos2d::ScriptEvent event(cocos2d::kCommonEvent,(void*)&data);
        cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
        memset(data.eventName, 0, sizeof(data.eventName));
        strncpy(data.eventName, "return", sizeof(data.eventName));
        event.data = (void*)&data;
        cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
#endif
    
    if(editBox_ != nil)
    {
        getEditBoxImplIOS()->onEndEditing();
    }
    return YES;
}
- (BOOL)textViewShouldBeginEditing:(UITextView *)textView
{
    return [self shouldBeginEditing];
}
- (BOOL)textViewShouldEndEditing:(UITextView *)textView
{
    return [self shouldEndEditing];
}
- (BOOL)textFieldShouldBeginEditing:(UITextField *)sender        // return NO to disallow editing.
{
    return [self shouldBeginEditing];
}

- (BOOL)textFieldShouldEndEditing:(UITextField *)sender
{
    return [self shouldEndEditing];
}
/**
 * Delegate method called before the text has been changed.
 * @param textField The text field containing the text.
 * @param range The range of characters to be replaced.
 * @param string The replacement string.
 * @return YES if the specified text range should be replaced; otherwise, NO to keep the old text.
 */
- (BOOL)shouldChangeTextInRange:(NSUInteger)oldLength shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    if (getEditBoxImplIOS()->getMaxLength() < 0)
    {
        return YES;
    }
    NSUInteger replacementLength = [text length];
    NSUInteger rangeLength = range.length;
    
    NSUInteger newLength = oldLength - rangeLength + replacementLength;
    
    return newLength <= getEditBoxImplIOS()->getMaxLength();
}
- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    NSUInteger oldLength = [textView.text length];
    return [self shouldChangeTextInRange:oldLength shouldChangeTextInRange:range replacementText:text];
}
- (BOOL)textField:(UITextField *) textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    NSUInteger oldLength = [textField.text length];
    return [self shouldChangeTextInRange:oldLength shouldChangeTextInRange:range replacementText:string];
}

/**
 * Called each time when the text field's text has changed.
 */
- (void)textViewDidChange:(UITextView *)textView
{
    [self textChanged];
}
- (void) textChanged
{
    // NSLog(@"text is %@", self.textField.text);
    cocos2d::ui::EditBoxDelegate* pDelegate = getEditBoxImplIOS()->getDelegate();
    if (pDelegate != NULL)
    {
        pDelegate->editBoxTextChanged(getEditBoxImplIOS()->getEditBox(), getEditBoxImplIOS()->getText());
    }
    
#if CC_ENABLE_SCRIPT_BINDING
    cocos2d::ui::EditBox*  pEditBox= getEditBoxImplIOS()->getEditBox();
    if (NULL != pEditBox && 0 != pEditBox->getScriptEditBoxHandler())
    {
        cocos2d::CommonScriptData data(pEditBox->getScriptEditBoxHandler(), "changed",pEditBox);
        cocos2d::ScriptEvent event(cocos2d::kCommonEvent,(void*)&data);
        cocos2d::ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
#endif
}
- (void)setInputModeDirect:(UIKeyboardType) inputMode
{
    if( self.textField )
    {
        self.textField.keyboardType = inputMode;
    }
    if( self.textView )
    {
        self.textView.keyboardType = inputMode;
    }
}
- (void)setInputMode:(cocos2d::ui::EditBox::InputMode) inputMode
{
    switch (inputMode)
    {
        case cocos2d::ui::EditBox::InputMode::EMAIL_ADDRESS:
            [self setInputModeDirect: UIKeyboardTypeEmailAddress];
            break;
        case cocos2d::ui::EditBox::InputMode::NUMERIC:
            [self setInputModeDirect: UIKeyboardTypeDecimalPad];
            break;
        case cocos2d::ui::EditBox::InputMode::PHONE_NUMBER:
            [self setInputModeDirect: UIKeyboardTypePhonePad];
            break;
        case cocos2d::ui::EditBox::InputMode::URL:
            [self setInputModeDirect: UIKeyboardTypeURL];
            break;
        case cocos2d::ui::EditBox::InputMode::DECIMAL:
            [self setInputModeDirect: UIKeyboardTypeDecimalPad];
            break;
        case cocos2d::ui::EditBox::InputMode::SINGLE_LINE:
            [self setInputModeDirect: UIKeyboardTypeDefault];
            break;
        default:
            [self setInputModeDirect: UIKeyboardTypeDefault];
            break;
    }
}
- (void)setInputFlag:(cocos2d::ui::EditBox::InputFlag)inputFlag
{
    switch (inputFlag)
    {
        case cocos2d::ui::EditBox::InputFlag::PASSWORD:
            if( self.textField )
            {
                self.textField.secureTextEntry = YES;
            }
            if( self.textView )
            {
                self.textView.secureTextEntry = YES;
            }
            break;
        case cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_WORD:
            if( self.textField )
            {
                self.textField.autocapitalizationType = UITextAutocapitalizationTypeWords;
            }
            if( self.textView )
            {
                self.textView.autocapitalizationType = UITextAutocapitalizationTypeWords;
            }
            break;
        case cocos2d::ui::EditBox::InputFlag::INITIAL_CAPS_SENTENCE:
            if( self.textField )
            {
                self.textField.autocapitalizationType = UITextAutocapitalizationTypeSentences;
            }
            if( self.textView )
            {
                self.textView.autocapitalizationType = UITextAutocapitalizationTypeSentences;
            }
            break;
        case cocos2d::ui::EditBox::InputFlag::INTIAL_CAPS_ALL_CHARACTERS:
            if( self.textField )
            {
                self.textField.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
            }
            if( self.textView )
            {
                self.textView.autocapitalizationType = UITextAutocapitalizationTypeAllCharacters;
            }
            break;
        case cocos2d::ui::EditBox::InputFlag::SENSITIVE:
            if( self.textField )
            {
                self.textField.autocorrectionType = UITextAutocorrectionTypeNo;
            }
            if( self.textView )
            {
                self.textView.autocorrectionType = UITextAutocorrectionTypeNo;
            }
            break;
        default:
            break;
    }
}
- (void)setReturnType:(cocos2d::ui::EditBox::KeyboardReturnType)returnType
{
    switch (returnType) {
        case cocos2d::ui::EditBox::KeyboardReturnType::DEFAULT:
            if( self.textField )
            {
                self.textField.returnKeyType = UIReturnKeyDefault;
            }
            if( self.textView )
            {
                self.textView.returnKeyType = UIReturnKeyDefault;
            }
            break;
        case cocos2d::ui::EditBox::KeyboardReturnType::DONE:
            if( self.textField )
            {
                self.textField.returnKeyType = UIReturnKeyDone;
            }
            if( self.textView )
            {
                self.textView.returnKeyType = UIReturnKeyDone;
            }
            break;
        case cocos2d::ui::EditBox::KeyboardReturnType::SEND:
            if( self.textField )
            {
                self.textField.returnKeyType = UIReturnKeySend;
            }
            if( self.textView )
            {
                self.textView.returnKeyType = UIReturnKeySend;
            }
            break;
        case cocos2d::ui::EditBox::KeyboardReturnType::SEARCH:
            if( self.textField )
            {
                self.textField.returnKeyType = UIReturnKeySearch;
            }
            if( self.textView )
            {
                self.textView.returnKeyType = UIReturnKeySearch;
            }
            break;
        case cocos2d::ui::EditBox::KeyboardReturnType::GO:
            if( self.textField )
            {
                self.textField.returnKeyType = UIReturnKeyGo;
            }
            if( self.textView )
            {
                self.textView.returnKeyType = UIReturnKeyGo;
            }
            break;
        default:
            if( self.textField )
            {
                self.textField.returnKeyType = UIReturnKeyDefault;
            }
            if( self.textView )
            {
                self.textView.returnKeyType = UIReturnKeyDefault;
            }
            break;
    }
}
- (NSString*)getFontName
{
    if( self.textField )
    {
        return [self.textField.font fontName];
    }
    if( self.textView )
    {
        return [self.textView.font fontName];
    }
    return @"";
}

- (BOOL)getSecureTextEntry
{
    if( nil != self.textField )
    {
        return self.textField.secureTextEntry;
    }
    if( nil != self.textView )
    {
        return self.textView.secureTextEntry;
    }
    return FALSE;
}
- (void)setFont:(UIFont*)font
{
    if( nil != self.textField )
    {
        [self.textField setFont:font];
    }
    if( nil != self.textView )
    {
        [self.textView setFont:font];
    }
}
- (void)setTextColor:(UIColor*)color
{
    if( nil != self.textField )
    {
        self.textField.textColor = color;
    }
    if( nil != self.textView )
    {
        self.textView.textColor = color;
    }
}
- (BOOL)isHidden
{
    if( nil != self.textField )
    {
        return self.textField.hidden;
    }
    if( nil != self.textView )
    {
        return self.textView.hidden;
    }
    return NO;
}
- (void)setHidden:(BOOL)hide
{
    if( nil != self.textField )
    {
        self.textField.hidden = hide;
    }
    if( nil != self.textView )
    {
        self.textView.hidden = hide;
    }
}
- (void)setText:(NSString*)text
{
    if( nil != self.textField )
    {
        self.textField.text = text;
    }
    if( nil != self.textView )
    {
        self.textView.text = text;
    }
}
-(NSString*)getText
{
    if( nil != self.textField )
    {
        return self.textField.text;
    }
    if( nil != self.textView )
    {
        return self.textView.text;
    }
    return @"";
}
- (void)setPlaceHolder:(NSString*)placeHolder
{
    if( nil != self.textField )
    {
        self.textField.placeholder = placeHolder;
    }
}

@end


NS_CC_BEGIN

namespace ui {

EditBoxImpl* __createSystemEditBox(EditBox* pEditBox)
{
    return new EditBoxImplIOS(pEditBox);
}

EditBoxImplIOS::EditBoxImplIOS(EditBox* pEditText)
: EditBoxImpl(pEditText)
, _label(nullptr)
, _labelPlaceHolder(nullptr)
, _anchorPoint(Vec2(0.5f, 0.5f))
, _systemControl(nullptr)
, _maxTextLength(-1)
, _enableMultiline(false)
{
    if( pEditText )
    {
        _enableMultiline = pEditText->isMultilineEnabled();
    }
}

EditBoxImplIOS::~EditBoxImplIOS()
{
    [_systemControl release];
}

void EditBoxImplIOS::doAnimationWhenKeyboardMove(float duration, float distance)
{
    if ([_systemControl isEditState] || distance < 0.0f)
    {
        [_systemControl doAnimationWhenKeyboardMoveWithDuration:duration distance:distance];
    }
}

bool EditBoxImplIOS::initWithSize(const Size& size)
{
    do 
    {
        auto glview = cocos2d::Director::getInstance()->getOpenGLView();

        CGRect rect = CGRectMake(0, 0, size.width * glview->getScaleX(),size.height * glview->getScaleY());

        CCEAGLView *eaglview = static_cast<CCEAGLView *>(glview->getEAGLView());
        float factor = eaglview.contentScaleFactor;
        rect.size.width /= factor;
        rect.size.height /= factor;
        _systemControl = [[UIEditBoxImplIOS_objc alloc] initWithFrame:rect editBox:this];
        if (!_systemControl) break;
        
		initInactiveLabels(size);
        setContentSize(size);
		
        return true;
    }while (0);
    
    return false;
}

void EditBoxImplIOS::initInactiveLabels(const Size& size)
{
	const char* pDefaultFontName = [[_systemControl getFontName] UTF8String];

	_label = Label::create();
    _label->setAnchorPoint(Vec2(0, 0.5f));
    _label->setColor(Color3B::WHITE);
    _label->setVisible(false);
    _editBox->addChild(_label, kLabelZOrder);
    if( isMultilineEnabled() )
    {
        _label->setAnchorPoint(Vec2(0, 0.0f));
        _label->setVerticalAlignment( TextVAlignment::TOP );
        _label->setWidth( size.width );
    }
	
    _labelPlaceHolder = Label::create();
	// align the text vertically center
    _labelPlaceHolder->setAnchorPoint(Vec2(0, 0.5f));
    _labelPlaceHolder->setColor(Color3B::GRAY);
    _editBox->addChild(_labelPlaceHolder, kLabelZOrder);
    
    setFont(pDefaultFontName, size.height*2/3);
    setPlaceholderFont(pDefaultFontName, size.height*2/3);
}

void EditBoxImplIOS::placeInactiveLabels()
{
    _label->setPosition(CC_EDIT_BOX_PADDING, _contentSize.height / 2.0f);
    _labelPlaceHolder->setPosition(CC_EDIT_BOX_PADDING, _contentSize.height / 2.0f);
    if( isMultilineEnabled() )
    {
        _label->setPosition(Vec2(CC_EDIT_BOX_PADDING, _contentSize.height - _label->getContentSize().height - CC_EDIT_BOX_PADDING));
    }
}

void EditBoxImplIOS::setInactiveText(const char* pText)
{
    if([_systemControl getSecureTextEntry] == YES)
    {
        std::string passwordString;
        for(int i = 0; i < strlen(pText); ++i)
            passwordString.append("\u25CF");
        _label->setString(passwordString.c_str());
    }
    else
        _label->setString(getText());

    // Clip the text width to fit to the text box
    float fMaxWidth = _editBox->getContentSize().width - CC_EDIT_BOX_PADDING * 2;
    Size labelSize = _label->getContentSize();
    if(labelSize.width > fMaxWidth) {
        _label->setDimensions(fMaxWidth,labelSize.height);
    }
}

void EditBoxImplIOS::setFont(const char* pFontName, int fontSize)
{
    bool isValidFontName = true;
	if(pFontName == NULL || strlen(pFontName) == 0) {
        isValidFontName = false;
    }

    CCEAGLView *eaglview = static_cast<CCEAGLView *>(cocos2d::Director::getInstance()->getOpenGLView()->getEAGLView());
    float retinaFactor = eaglview.contentScaleFactor;
	NSString * fntName = [NSString stringWithUTF8String:pFontName];

    auto glview = cocos2d::Director::getInstance()->getOpenGLView();

    float scaleFactor = glview->getScaleX();
    UIFont *textFont = nil;
    if (isValidFontName) {
        textFont = [UIFont fontWithName:fntName size:fontSize * scaleFactor / retinaFactor];
    }
    
    if (!isValidFontName || textFont == nil){
        textFont = [UIFont systemFontOfSize:fontSize * scaleFactor / retinaFactor];
    }

	if(textFont != nil) {
		[_systemControl setFont:textFont];
    }

	_label->setSystemFontName(pFontName);
	_label->setSystemFontSize(fontSize);
}

void EditBoxImplIOS::setFontColor(const Color3B& color)
{
    [_systemControl setTextColor:[UIColor colorWithRed:color.r / 255.0f green:color.g / 255.0f blue:color.b / 255.0f alpha:1.0f]];
	_label->setColor(color);
}

void EditBoxImplIOS::setPlaceholderFont(const char* pFontName, int fontSize)
{
	_labelPlaceHolder->setSystemFontName(pFontName);
	_labelPlaceHolder->setSystemFontSize(fontSize);
}

void EditBoxImplIOS::setPlaceholderFontColor(const Color3B& color)
{
	_labelPlaceHolder->setColor(color);
}

void EditBoxImplIOS::setInputMode(EditBox::InputMode inputMode)
{
    [_systemControl setInputMode:inputMode];
}

void EditBoxImplIOS::setMaxLength(int maxLength)
{
    _maxTextLength = maxLength;
}
void EditBoxImplIOS::enableMultiline( bool enable )
{
    if( _enableMultiline != enable )
    {
        _enableMultiline = enable;
        if( _systemControl )
        {
            [_systemControl setMultilineEnable:_enableMultiline];
        }
    }
}
int EditBoxImplIOS::getMaxLength()
{
    return _maxTextLength;
}

void EditBoxImplIOS::setInputFlag(EditBox::InputFlag inputFlag)
{
    [_systemControl setInputFlag:inputFlag];
}

void EditBoxImplIOS::setReturnType(EditBox::KeyboardReturnType returnType)
{
    [_systemControl setReturnType:returnType];
}

bool EditBoxImplIOS::isEditing()
{
    return [_systemControl isEditState] ? true : false;
}

void EditBoxImplIOS::refreshInactiveText()
{
    const char* text = getText();
    if([_systemControl isHidden] == YES)
    {
		setInactiveText(text);
		if(strlen(text) == 0)
		{
			_label->setVisible(false);
			_labelPlaceHolder->setVisible(true);
		}
		else
		{
			_label->setVisible(true);
			_labelPlaceHolder->setVisible(false);
		}
	}
}

void EditBoxImplIOS::setText(const char* text)
{
    NSString* nsText =[NSString stringWithUTF8String:text];
    if ([nsText compare:[_systemControl getText]] != NSOrderedSame)
    {
        [_systemControl setText:nsText];
    }
    
    refreshInactiveText();
}

NSString* removeSiriString(NSString* str)
{
    NSString* siriString = @"\xef\xbf\xbc";
    return [str stringByReplacingOccurrencesOfString:siriString withString:@""];
}

const char*  EditBoxImplIOS::getText(void)
{
    return [removeSiriString([_systemControl getText]) UTF8String];
}

void EditBoxImplIOS::setPlaceHolder(const char* pText)
{
    [_systemControl setPlaceHolder:[NSString stringWithUTF8String:pText]];
	_labelPlaceHolder->setString(pText);
}

static CGPoint convertDesignCoordToScreenCoord(const Vec2& designCoord)
{
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    CCEAGLView *eaglview = (CCEAGLView *) glview->getEAGLView();

    float viewH = (float)[eaglview getHeight];
    
    Vec2 visiblePos = Vec2(designCoord.x * glview->getScaleX(), designCoord.y * glview->getScaleY());
    Vec2 screenGLPos = visiblePos + glview->getViewPortRect().origin;
    
    CGPoint screenPos = CGPointMake(screenGLPos.x, viewH - screenGLPos.y);
    
    float factor = eaglview.contentScaleFactor;
    screenPos.x = screenPos.x / factor;
    screenPos.y = screenPos.y / factor;

    CCLOGINFO("[EditBox] pos x = %f, y = %f", screenGLPos.x, screenGLPos.y);
    return screenPos;
}

void EditBoxImplIOS::setPosition(const Vec2& pos)
{
	_position = pos;
	adjustTextFieldPosition();
}

void EditBoxImplIOS::setVisible(bool visible)
{
//    _systemControl.textField.hidden = !visible;
}

void EditBoxImplIOS::setContentSize(const Size& size)
{
    _contentSize = size;
    CCLOG("[Edit text] content size = (%f, %f)", size.width, size.height);
    placeInactiveLabels();
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    CGSize controlSize = CGSizeMake(size.width * glview->getScaleX(),size.height * glview->getScaleY());
    
    CCEAGLView *eaglview = static_cast<CCEAGLView *>(glview->getEAGLView());
    float factor = eaglview.contentScaleFactor;
    controlSize.width /= factor;
    controlSize.height /= factor;
    
    [_systemControl setContentSize:controlSize];
}
bool EditBoxImplIOS::isMultilineEnabled() const
{
    return _enableMultiline;
}
void EditBoxImplIOS::setAnchorPoint(const Vec2& anchorPoint)
{
    CCLOG("[Edit text] anchor point = (%f, %f)", anchorPoint.x, anchorPoint.y);
	_anchorPoint = anchorPoint;
	setPosition(_position);
}

void EditBoxImplIOS::visit(void)
{
}

void EditBoxImplIOS::onEnter(void)
{
    adjustTextFieldPosition();
    const char* pText = getText();
    if (pText) {
        setInactiveText(pText);
    }
}

void EditBoxImplIOS::updatePosition(float dt)
{
    if (nullptr != _systemControl) {
        this->adjustTextFieldPosition();
    }
}



void EditBoxImplIOS::adjustTextFieldPosition()
{
	Size contentSize = _editBox->getContentSize();
	Rect rect = Rect(0, 0, contentSize.width, contentSize.height);
    rect = RectApplyAffineTransform(rect, _editBox->nodeToWorldTransform());
	
	Vec2 designCoord = Vec2(rect.origin.x, rect.origin.y + rect.size.height);
    [_systemControl setPosition:convertDesignCoordToScreenCoord(designCoord)];
}
void EditBoxImplIOS::finishEdit()
{
    onEndEditing();
    closeKeyboard();
}
void EditBoxImplIOS::openKeyboard()
{
	_label->setVisible(false);
	_labelPlaceHolder->setVisible(false);

	[_systemControl setHidden:NO];
    [_systemControl openKeyboard];
}

void EditBoxImplIOS::closeKeyboard()
{
    [_systemControl closeKeyboard];
}

void EditBoxImplIOS::onEndEditing()
{
    [_systemControl setHidden:YES];
	if(strlen(getText()) == 0)
	{
		_label->setVisible(false);
		_labelPlaceHolder->setVisible(true);
	}
	else
	{
		_label->setVisible(true);
		_labelPlaceHolder->setVisible(false);
		setInactiveText(getText());
        placeInactiveLabels();
	}
}

}

NS_CC_END

#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) */


