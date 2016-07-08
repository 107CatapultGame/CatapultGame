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
    time_t now = time(NULL); // ��ȡʱ��
    struct tm * local = localtime(&now); // ��ȡϵͳʱ��
    strftime(temp, 20, "%R", local); // ����ʱ���ʽΪhh:mm
    result = temp;
    return result; // ����ֵ
}
