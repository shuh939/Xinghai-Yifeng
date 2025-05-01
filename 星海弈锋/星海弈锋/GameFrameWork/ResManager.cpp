#include "ResManager.h"
#include <iostream>
#pragma comment(lib,"msimg32.lib")

ResManager* ResManager::instance()
{
    static ResManager mgr;
    return &mgr;
}

IMAGE* ResManager::loadImage(const std::string& imgPath, int w, int h)
{
	auto img = new IMAGE;
	::loadimage(img, imgPath.data(), w, h);
	if (img->getwidth() == 0 || img->getheight() == 0) {
		printf("load image <%s> failed\n", imgPath.data());
		delete img;
		return nullptr;
	}
	return img;
}

IMAGE* ResManager::cacheImage(const std::string& tag, const std::string& imgPath, int w, int h)
{
	auto it = m_images.find(tag);
	if (it != m_images.end()) {
		return it->second.get();
	}
	IMAGE* img = nullptr;
	if (m_prefix.empty())
		img = loadImage(imgPath, w, h);
	else
		img = loadImage(m_prefix + "/" + imgPath, w, h);
	m_images.emplace(tag,img );
	return img;
}

IMAGE* ResManager::getImage(const std::string& tag)
{
	auto it = m_images.find(tag);
	if (it == m_images.end()) {
		printf("tag <%s> image not found!\n", tag.data());
		return nullptr;
	}
	return it->second.get();
}

void ResManager::print()
{
	std::cout << "cache image count:" << m_images.size() << std::endl;
	for (auto& p : m_images) {
		std::cout << p.first << " " << p.second << std::endl;
	}
}

void drawImage(int x, int y, IMAGE* img)
{
	HDC dstDC = GetImageHDC(NULL);
	HDC srcDC = GetImageHDC((IMAGE*)img);
	int w = img->getwidth();
	int h = img->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

void drawImage(int x, int y, const std::string& tag_or_imgPath, int w, int h)
{
	std::string key;
	if (w != 0 || h != 0) {
		key = tag_or_imgPath + std::to_string(w) + "x" + std::to_string(h);
	}
	else {
		key = tag_or_imgPath;
	}
	auto img = ResManager::instance()->cacheImage(key, tag_or_imgPath, w, h);
	if (!img)
		return;
	drawImage(x, y, img);
}
