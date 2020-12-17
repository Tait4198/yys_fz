//
// Created by 2020/12/17.
//

#ifndef YYS_FZ_JSONCONVERT_H
#define YYS_FZ_JSONCONVERT_H

#include <json/json.h>
#include <string>

class JsonConvert {
public:
    JsonConvert();

    ~JsonConvert();

    bool convert(const std::string& jsonStr, Json::Value *jsonValue);

private:
    Json::CharReader *reader;
};


#endif //YYS_FZ_JSONCONVERT_H
