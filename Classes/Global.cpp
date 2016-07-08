#include "Global.h"

#include <ctime>

const uint_8 Global::LAYER_BACKGROUND = 0;
const uint_8 Global::LAYER_GAMEPLAY = 10;
const uint_8 Global::LAYER_UI = 20;

Global::Global() {}

Global::~Global() {}

std::string Global::getSystemTime() {
    std::string result = "";
    char temp[20];
    time_t now = time(NULL); // 获取时间
    struct tm * local = localtime(&now); // 获取系统时间
    strftime(temp, 20, "%R", local); // 设置时间格式为hh:mm
    result = temp;
    return result; // 返回值
}
