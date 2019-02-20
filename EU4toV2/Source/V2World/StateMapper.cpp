/*Copyright (c) 2019 The Paradox Game Converters Project

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.*/



#include "StateMapper.h"
#include "../Configuration.h"
#include "Log.h"
#include "Object.h"
#include "OSCompatibilityLayer.h"
#include "ParadoxParser8859_15.h"



Vic2::stateMapper::stateMapper()
{
	LOG(LogLevel::Info) << "Parsing region structure";

	std::string filename;
	if (Utils::DoesFileExist("./blankMod/output/map/region.txt"))
	{
		filename = "./blankMod/output/map/region.txt";
	}
	else
	{
		filename = theConfiguration.getVic2Path() + "/map/region.txt";
	}

	std::shared_ptr<Object> Vic2RegionsObj = parser_8859_15::doParseFile(filename);
	if (Vic2RegionsObj == NULL)
	{
		LOG(LogLevel::Error) << "Could not parse file " << filename;
		exit(-1);
	}
	if (Vic2RegionsObj->getLeaves().size() < 1)
	{
		LOG(LogLevel::Error) << "Could not parse region.txt";
		exit(-1);
	}
	initStateMap(Vic2RegionsObj);
}

void Vic2::stateMapper::initStateMap(std::shared_ptr<Object> obj)
{
	std::vector<std::shared_ptr<Object>> states = obj->getLeaves();

	for (unsigned int stateIndex = 0; stateIndex < states.size(); stateIndex++)
	{
		std::vector<std::string> provinces = states[stateIndex]->getTokens();
		std::vector<int> neighbors;

		for (auto province : provinces)
		{
			neighbors.push_back(std::stoi(province));
			stateIndexMap.insert(std::make_pair(std::stoi(province), stateIndex));
		}

		for (auto neighbor : neighbors)
		{
			stateProvincesMap.insert(std::make_pair(neighbor, neighbors));
		}
	}
}

std::vector<int> Vic2::stateMapper::getOtherProvincesInState(int province)
{
	auto mapping = stateProvincesMap.find(province);
	if (mapping != stateProvincesMap.end())
	{
		return mapping->second;
	}
	else
	{
		std::vector<int> empty;
		return empty;
	}
}

int Vic2::stateMapper::getStateIndex(int province)
{
	auto mapping = stateIndexMap.find(province);
	if (mapping != stateIndexMap.end())
	{
		return mapping->second;
	}
	else
	{
		return -1;
	}
}