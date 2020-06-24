#include <string>
#include <vector>
#include <io.h>
#include "CommonFuncs.h"
#include "CodeSwitcher.h"
#include<Windows.h>

string GetFileCodeHeader(FILE_TYPE eType)
{
    string header;
    switch (eType)
    {
        case FILE_TYPE::UNICODE_LITTLE_ENDIAN:  //65534
        {
            header += 0xfffe;
        }break;
        case FILE_TYPE::UNICODE_BIG_ENDIAN://65279
        {
            header += 0xfeff;
        }break;
        case FILE_TYPE::UTF_8_BOM://61371
        {
            header += 0xefbb;
        }break;
        default:
        {
        }
    }

    return header;
}

bool IsStrUtf8(const char* str)
{
    unsigned int nBytes = 0;//UFT8����1-6���ֽڱ���,ASCII��һ���ֽ�
    unsigned char chr = *str;
    bool bAllAscii = true;
    for (unsigned int i = 0; str[i] != '\0'; ++i) {
        chr = *(str + i);
        //�ж��Ƿ�ASCII����,�������,˵���п�����UTF8,ASCII��7λ����,���λ���Ϊ0,0xxxxxxx
        if (nBytes == 0 && (chr & 0x80) != 0) {
            bAllAscii = false;
        }
        if (nBytes == 0) {
            //�������ASCII��,Ӧ���Ƕ��ֽڷ�,�����ֽ���
            if (chr >= 0x80) {
                if (chr >= 0xFC && chr <= 0xFD) {
                    nBytes = 6;
                }
                else if (chr >= 0xF8) {
                    nBytes = 5;
                }
                else if (chr >= 0xF0) {
                    nBytes = 4;
                }
                else if (chr >= 0xE0) {
                    nBytes = 3;
                }
                else if (chr >= 0xC0) {
                    nBytes = 2;
                }
                else {
                    return false;
                }
                nBytes--;
            }
        }
        else {
            //���ֽڷ��ķ����ֽ�,ӦΪ 10xxxxxx
            if ((chr & 0xC0) != 0x80) {
                return false;
            }
            //����Ϊ��Ϊֹ
            nBytes--;
        }
    }
    //Υ��UTF8�������
    if (nBytes != 0) {
        return false;
    }
    if (bAllAscii) { //���ȫ������ASCII, Ҳ��UTF8
        return false;
    }
    return true;
}

void UnicodeBigToLittleEndian(std::wstring& wide_string)
{
    for (auto& c : wide_string)
    {
        auto pC = (unsigned char*)&c;
        unsigned char tc = *pC;
        *(pC) = *(pC + 1);
        *(pC + 1) = tc;
    }
}

std::string WideToAnsi(const std::wstring& wide_string)
{
	int count = WideCharToMultiByte(CP_ACP, 0, wide_string.data(), wide_string.length(), NULL, 0, NULL, NULL);
	std::string ansiString(count, '\0');
	WideCharToMultiByte(CP_ACP, 0, wide_string.data(), wide_string.length(), &ansiString[0], count, NULL, NULL);
	return ansiString;
}

std::string WideToUtf8(const std::wstring& wide_string)
{
	int count = WideCharToMultiByte(CP_UTF8, 0, wide_string.data(), wide_string.length(), NULL, 0, NULL, NULL);
	std::string utf8String(count, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wide_string.data(), wide_string.length(), &utf8String[0], count, NULL, NULL);
	return utf8String;
}

std::wstring AnsiToWide(const std::string& ansiString)
{
	int count = MultiByteToWideChar(CP_ACP, 0, ansiString.data(), ansiString.length(), NULL, 0);
	std::wstring unicodeString(count, '\0');
	MultiByteToWideChar(CP_ACP, 0, ansiString.data(), ansiString.length(), &unicodeString[0], count);
	return unicodeString;
}

std::string AnsiToUtf8(const std::string& ansiString)
{
	return WideToUtf8(AnsiToWide(ansiString));
}

std::wstring Utf8ToWide(const std::string& utf8String)
{
	int count = MultiByteToWideChar(CP_UTF8, 0, utf8String.data(), utf8String.length(), NULL, 0);
	std::wstring unicodeString(count, '\0');
	MultiByteToWideChar(CP_UTF8, 0, utf8String.data(), utf8String.length(), &unicodeString[0], count);
	return unicodeString;
}

std::string Utf8ToAnsi(const std::string& utf8String)
{
	return WideToAnsi(Utf8ToWide(utf8String));
}
