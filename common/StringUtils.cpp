#include "StringUtils.h"

std::string utf8Encode(const std::wstring &ws) {
    if (ws.empty()) {
        return std::string();
    }
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &ws[0], (int) ws.size(), nullptr, 0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &ws[0], (int) ws.size(), &strTo[0], size_needed, nullptr, nullptr);
    return strTo;
}