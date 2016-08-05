#ifndef BMPHANDLER_H_INCLUDED
#define BMPHANDLER_H_INCLUDED

//------------------------------------------------------------
//头文件
#include<stdio.h>
#include<windows.h>
#include<wingdi.h>//这个前面必须include<windows.h>，否则报错

//------------------------------------------------------------
//CBmpHandler类
class CBmpHandler
{
private:
    //图像数据
    int m_iBmpWidth;//图像的宽
    int m_iBmpHeight;//图像的高
    int m_iBitCount;//图像类型，每像素位数
    int m_iLineSize;//图像每行的字节数
    RGBQUAD* m_stColorTable;//颜色表指针
    unsigned char* m_szBmpBuf;//图像数据的指针

private:
    //私有函数

public:

//----------------------------------------
//基本函数

    //构造函数
    CBmpHandler();
    //析构函数
    ~CBmpHandler();
    //加载bmp图片
    bool loadBmp(char* szFileName);
    //保存图片
    bool saveBmpAs(char* szFileName);

//----------------------------------------
//以下是图像处理函数，新功能请在下面添加

    //更改亮度，参数范围-255~255
    bool change_brightness(int iValue);

};

#endif // BMPHANDLER_H_INCLUDED
