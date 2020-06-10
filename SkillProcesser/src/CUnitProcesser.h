#pragma once

#include <string>
#include <map>
#include <vector>

#include "IDllExport.h"

class DLL_API CUnitProcesser
{
public:
	CUnitProcesser();

public:
	void SetHeroPackageRoot(std::string path);
	std::vector<std::string>& getSkins(std::string heroId);
	std::map<std::string, std::vector<std::string> >& getHeros();
	std::string getSkinName(std::string skinId);

private:
	std::string m_path;
	std::map<std::string, std::vector<std::string> > m_heros;
	std::map<std::string, std::string> m_heros_name;
};