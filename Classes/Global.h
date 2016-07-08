#ifndef __GLOBAL_SCENE_H__
#define __GLOBAL_SCENE_H__
#include <string>

typedef unsigned char uint_8;

class Global {
    public:
    Global();
    ~Global();
    /** ��ȡ��ǰʱ��, ��ʽhh:mm */
    static std::string getSystemTime();

    // ����ָ��Sprite�Ĳ㼶
    const static uint_8 LAYER_BACKGROUND; // ������
    const static uint_8 LAYER_GAMEPLAY;   // ��Ϸ��
    const static uint_8 LAYER_UI;         // �����
};

#endif // __GLOBAL_SCENE_H__
