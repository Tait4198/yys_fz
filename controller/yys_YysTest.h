#pragma once

#include <drogon/HttpController.h>

using namespace drogon;
namespace yys {
    class YysTest : public drogon::HttpController<YysTest> {
    public:
        METHOD_LIST_BEGIN
            //use METHOD_ADD to add your custom processing function here;
            //METHOD_ADD(YysTest::get,"/{2}/{1}",Get);//path is /yys/YysTest/{arg2}/{arg1}
            //METHOD_ADD(YysTest::your_method_name,"/{1}/{2}/list",Get);//path is /yys/YysTest/{arg1}/{arg2}/list
            //ADD_METHOD_TO(YysTest::your_method_name,"/absolute/path/{1}/{2}/list",Get);//path is /absolute/path/{arg1}/{arg2}/list

            METHOD_ADD(getYysHwnds, "/hwnds", Get);
            METHOD_ADD(getScrennshot, "/img/{1}", Get);
        METHOD_LIST_END

    public:
        // your declaration of processing function maybe like this:
        // void get(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,int p1,std::string p2);
        // void your_method_name(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,double p1,int p2) const;
        void getYysHwnds(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);

        void getScrennshot(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback,
                           std::string &&hexHwnd);
    };
}
