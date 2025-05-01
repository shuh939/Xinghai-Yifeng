#pragma once

//取消unicode字符集
#ifdef UNICODE
#undef UNICODE
#endif // UNICODE

#include <easyx.h>
#include <string>
#include <unordered_map>
#include <memory>
class ResManager
{
public:
	static ResManager* instance();
	static IMAGE* loadImage(const std::string& imgPath, int w = 0, int h = 0);
	IMAGE* cacheImage(const std::string& tag, const std::string& imgPath, int w = 0, int h = 0);
	IMAGE* getImage(const std::string& tag);

	void setPrefix(const std::string& prefix) { m_prefix = prefix; }
	const std::string prefix()const { return m_prefix; }

	void print();
private:
	std::unordered_map<std::string, std::unique_ptr<IMAGE>> m_images;
	std::string m_prefix;			//加载图片的前缀路径
};

void drawImage(int x, int y, IMAGE* img);
void drawImage(int x, int y, const std::string& tag_or_imgPath,int w = 0,int h = 0);


