/****************************************************************************
 Copyright (c) 2014 Chukong Technologies Inc.
 
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


#include "base/CCDirector.h"
#include "platform/CCFileUtils.h"


NS_CC_BEGIN
namespace ui {

	WebView::WebView()
		:_onJSCallback(nullptr),
		_onShouldStartLoading(nullptr),
		_onDidFinishLoading(nullptr),
		_onDidFailLoading(nullptr)
	{
	}

	WebView::~WebView()
	{

	}

	WebView *WebView::create()
	{ 
		auto webView = new(std::nothrow) WebView();
		if (webView && webView->init())
		{
			webView->autorelease();
			return webView;
		}
		CC_SAFE_DELETE(webView);  
		return nullptr;
	}

	void WebView::setJavascriptInterfaceScheme(const std::string &scheme)
	{

	}

	void WebView::loadData(const cocos2d::Data &data,
		const std::string &MIMEType,
		const std::string &encoding,
		const std::string &baseURL)
	{

	}

	void WebView::loadHTMLString(const std::string &string, const std::string &baseURL)
	{

	}

	void WebView::loadURL(const std::string &url)
	{

	}

	void WebView::loadFile(const std::string &fileName)
	{

	}

	void WebView::stopLoading()
	{

	}

	void WebView::reload()
	{

	}

	bool WebView::canGoBack()
	{
		return false;
	}

	bool WebView::canGoForward()
	{
		return false;
	}

	void WebView::goBack()
	{

	}

	void WebView::goForward()
	{

	}

	void WebView::evaluateJS(const std::string &js)
	{

	}

	void WebView::setScalesPageToFit(bool const scalesPageToFit)
	{

	}

	void WebView::draw(cocos2d::Renderer *renderer, cocos2d::Mat4 const &transform, uint32_t flags)
	{

	}

	void WebView::setVisible(bool visible)
	{

	}

	cocos2d::ui::Widget* WebView::createCloneInstance()
	{
		return nullptr;
	}

	void WebView::copySpecialProperties(Widget* model)
	{
		WebView* webView = dynamic_cast<WebView*>(model);
		if (webView)
		{
			this->_impl = nullptr;
			this->_onShouldStartLoading = webView->_onShouldStartLoading;
			this->_onDidFinishLoading = webView->_onDidFinishLoading;
			this->_onDidFailLoading = webView->_onDidFailLoading;
			this->_onJSCallback = webView->_onJSCallback;
		}
	}

	void WebView::setOnDidFailLoading(const ccWebViewCallbak &callback)
	{
		_onDidFailLoading = callback;
	}

	void WebView::setOnDidFinishLoading(const ccWebViewCallbak &callback)
	{
		_onDidFinishLoading = callback;
	}

	void WebView::setOnShouldStartLoading(const std::function<bool(WebView *sender, std::string url)> &callback)
	{
		_onShouldStartLoading = callback;
	}

	void WebView::setOnJSCallback(const ccWebViewCallbak &callback)
	{
		_onJSCallback = callback;
	}

	std::function<bool(WebView *sender, std::string url)> WebView::getOnShouldStartLoading()const
	{
		return _onShouldStartLoading;
	}

	WebView::ccWebViewCallbak WebView::getOnDidFailLoading()const
	{
		return _onDidFailLoading;
	}

	WebView::ccWebViewCallbak WebView::getOnDidFinishLoading()const
	{
		return _onDidFinishLoading;
	}

	WebView::ccWebViewCallbak WebView::getOnJSCallback()const
	{
		return _onJSCallback;
	}   
	NS_CC_END
} //namespace cocos2d

