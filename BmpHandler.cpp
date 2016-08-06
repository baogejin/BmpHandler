#include "BmpHandler.h"

//构造函数
CBmpHandler::CBmpHandler()
{
}

//析构函数
CBmpHandler::~CBmpHandler()
{
    if(m_iBitCount == 8)delete []m_stColorTable;
    delete []m_szBmpBuf;
}

//加载图片
bool CBmpHandler::loadBmp(char* szFileName)
{
    //打开bmp文件
    FILE *fp = fopen(szFileName, "rb");

    //没有读到，返回失败
    if(!fp)return false;

    //跳过位图文件头结构 BITMAPFILEHEADER
    fseek(fp, sizeof(BITMAPFILEHEADER), 0);//BITMAPFILEHEADER需要include<wingdi.h>

    //获取图片信息结构 BITMAPINFOHEADER
    BITMAPINFOHEADER stInfoHeader;
    fread(&stInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    //获取图像宽、高
    m_iBmpWidth = stInfoHeader.biWidth;
    m_iBmpHeight = stInfoHeader.biHeight;
    int iSize = stInfoHeader.biSizeImage;
    //获取每像素位数，计算一行所占字节数
    m_iBitCount = stInfoHeader.biBitCount;
    m_iLineSize = (m_iBmpWidth*m_iBitCount/8+3)/4*4;//这里确保了是4的倍数

    if(m_iBitCount == 8)
    {
        //是灰度图片，申请颜色表所需空间，读颜色表进内存
        m_stColorTable = new RGBQUAD[256];
        fread(m_stColorTable, sizeof(RGBQUAD), 256, fp);
    }

    //申请位图数据所需空间，读位图数据进内存
    m_szBmpBuf = new unsigned char[m_iLineSize*m_iBmpHeight];
    fread(m_szBmpBuf, 1, m_iLineSize*m_iBmpHeight, fp);

    //关闭文件
    fclose(fp);

    return true;
}

//保存图片
bool CBmpHandler::saveBmpAs(char* szFileName)
{
    //没有图像数据
    if(!m_szBmpBuf)return false;

    //颜色表大小，以字节为单位，灰度图颜色表大小1024，彩色图为0
    int iColorTableSize = 0;
    if(m_iBitCount == 8)iColorTableSize = 1024;

    //以二进制写的形式打开文件
    FILE* fp = fopen(szFileName, "wb");

    //打开文件失败
    if(!fp)return false;

    //申请位图文件头结构变量，填写文件头信息
    BITMAPFILEHEADER stFileHeader;
    stFileHeader.bfType = 0x4D42;//bmp类型，为什么头文件里没有宏定义
    stFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + iColorTableSize + (m_iLineSize*m_iBmpHeight);//bfSize是4个部分之和
    stFileHeader.bfReserved1 = 0;
    stFileHeader.bfReserved2 = 0;
    stFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + iColorTableSize;//bfOffBits是前3部分之和
    fwrite(&stFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);//写文件头进文件

    //申请位图信息头结构变量，填写信息头信息
    BITMAPINFOHEADER stInfoHeader;
    stInfoHeader.biBitCount = m_iBitCount;
    stInfoHeader.biClrImportant = 0;
    stInfoHeader.biClrUsed = 0;
    stInfoHeader.biCompression = 0;
    stInfoHeader.biHeight = m_iBmpHeight;
    stInfoHeader.biWidth = m_iBmpWidth;
    stInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
    stInfoHeader.biSizeImage = m_iLineSize*m_iBmpHeight;
    stInfoHeader.biXPelsPerMeter = 0;
    stInfoHeader.biYPelsPerMeter = 0;
    fwrite(&stInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    //如果是灰度图，有颜色表，写入文件
    if(m_iBitCount == 8)fwrite(m_stColorTable, sizeof(RGBQUAD), 256, fp);

    //写位图数据进文件
    fwrite(m_szBmpBuf, m_iLineSize*m_iBmpHeight, 1, fp);

    //关闭文件
    fclose(fp);

    return true;
}

//更改亮度，参数范围-255~255
bool CBmpHandler::change_brightness(int iValue)
{
    //没有图片资源
    if(!m_szBmpBuf)return false;

    //参数超出范围
    if(iValue < -255 || iValue > 255)return false;

    for(int i=0; i<m_iBmpHeight; i++)
    {
        for(int j=0; j<m_iBmpWidth; j++)
        {
            //遍历所有像素
            for(int k=0; k<3; k++)
            {
                //修改rgb值，用int临时变量防止越界
                int iTmp = (int)m_szBmpBuf[i*m_iLineSize+j*3+k];
                iTmp += iValue;
                if(iTmp < 0)iTmp = 0;
                if(iTmp > 255)iTmp = 255;
                m_szBmpBuf[i*m_iLineSize+j*3+k] = (char)iTmp;
            }
        }
    }
    return true;
}

//将图片变成伪灰度图
bool CBmpHandler::change_grey(void)
{
    //没有图片资源
    if(!m_szBmpBuf)return false;

    for(int i=0; i<m_iBmpHeight; i++)
    {
        for(int j=0; j<m_iBmpWidth; j++)
        {
            //遍历所有像素
            int iGrey = 0;
            for(int k=0; k<3; k++)
            {
                //修改rgb值，用int临时变量防止越界
                int iTmp = (int)m_szBmpBuf[i*m_iLineSize+j*3+k];
                iGrey += iTmp;
            }
            iGrey = iGrey/3;
            for(int k=0; k<3; k++)
            {
                //修改rgb值，用int临时变量防止越界
                m_szBmpBuf[i*m_iLineSize+j*3+k] = (char)iGrey;
            }
        }
    }
    return true;
}
