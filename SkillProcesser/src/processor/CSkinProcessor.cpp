#include "CSkinProcessor.h"
#include "../tools/CommonFuncs.h"
#include <io.h>
#include <algorithm>

CSkinProcessor::CSkinProcessor()
{

}

void CSkinProcessor::SetHeroPackageRoot(std::string path)
{
	std::replace(path.begin(), path.end(), '\\', '/');
	struct __finddata64_t fileinfo;
	__int64 handle = _findfirst64((path + "*").c_str(), &fileinfo);
	if (-1 != handle)
	{
		m_path = path;
		m_heros.clear();
		m_heros_name.clear();
		do
		{
			if (0 == strcmp(fileinfo.name, ".") || 0 == strcmp(fileinfo.name, ".."))
			{
				continue;
			}
			std::string name = fileinfo.name;
			std::string heroId = "";
			std::string skinId = "";
			if (std::string::npos != name.find("common"))
			{
				auto index = name.find("_common");
				heroId = name.substr(0, index);
			}
			else
			{
				heroId = name.substr(0, name.length() - 3);
				skinId = name;

				auto heroPath = path + name + "/presentations/";
				std::string heroName(DeepFindInDir(heroPath.c_str(), ".prt"));
				if (!heroName.empty())
				{
					std::replace(heroName.begin(), heroName.end(), '\\', '/');
					auto index_1 = heroName.rfind("/");
					auto index_2 = heroName.rfind("/", index_1 - 1);
					heroName = heroName.substr(index_2 + 1, index_1 - index_2 - 1);
					m_heros_name[skinId] = heroName;
				}
			}

			if (!heroId.empty())
			{
				std::vector<std::string>& vSkins = m_heros[heroId];
				if (!skinId.empty())
				{
					vSkins.push_back(skinId);
				}
			}
		} while (!_findnext64(handle, &fileinfo));
		_findclose(handle);
	}
}

std::vector<std::string>& CSkinProcessor::getSkins(std::string heroId)
{
	return m_heros[heroId];
}

std::map<std::string, std::vector<std::string> >& CSkinProcessor::getHeros()
{
	return m_heros;
}

std::string CSkinProcessor::getSkinName(std::string skinId)
{
	return m_heros_name[skinId];
}