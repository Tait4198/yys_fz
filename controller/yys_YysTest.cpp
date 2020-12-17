#include "yys_YysTest.h"
#include "../game/GameUtil.h"
#include "../game/FzManager.h"
#include "../common/Base64.h"
#include <json/json.h>

using namespace yys;
using namespace std;

extern FzManager fzManager;

template <typename Iterable>
Json::Value iterable2json(Iterable const& cont) {
	Json::Value v;
	for (auto&& element : cont) {
		v.append(element);
	}
	return v;
}

void yys::YysTest::getYysHwndList(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
	Json::Value result;
	vector<string> hexHwndList = fzManager.getHexHwndList();
	Json::Value hexHwndListJson = iterable2json(hexHwndList);
	result["hexHwndList"] = hexHwndListJson;
	auto resp = HttpResponse::newHttpJsonResponse(result);
	callback(resp);
}

void yys::YysTest::getScreenshot(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback, std::string&& hexHwnd) {
	HWND hwnd = fzManager.getHwndByHexHwnd(move(hexHwnd));
	auto resp = HttpResponse::newHttpResponse();
	if (IsWindow(hwnd)) {
		cv::Mat scMat = getScreenshotMat(hwnd);
		vector<uchar> buf;
		cv::imencode(".png", scMat, buf);
		auto* enc_msg = reinterpret_cast<unsigned char*>(buf.data());
		string encoded = base64_encode(enc_msg, buf.size());
		scMat.release();
		resp->setBody(encoded);
	}
	else {
		resp->setStatusCode(HttpStatusCode::k404NotFound);
	}
	callback(resp);
}
