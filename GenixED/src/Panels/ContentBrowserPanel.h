#pragma once
#include <filesystem>

class Texture;

class ContentBrowserPanel
{
public:
	ContentBrowserPanel();

	void OnImGuiRender();

private:
	std::filesystem::path m_CurrentDirectory;

	Ref<Texture> m_DirectoryIcon;
	Ref<Texture> m_FileIcon;
};
