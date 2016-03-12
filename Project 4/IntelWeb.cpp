#include "BinaryFile.h"
#include "DiskMultiMap.h"
#include "IntelWeb.h"
#include "MultiMapTuple.h"
#include <math.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <set>
#include <map>
using namespace std;

// CONST STRING MEMBER DEFINITIONS

const string IntelWeb::STRING_FOR_OLDMAP("_old_map");
const string IntelWeb::STRING_FOR_NEWMAP("_new_map");

// INTELWEB IMPLEMENTATION

IntelWeb::IntelWeb()
{}

IntelWeb::~IntelWeb()
{
	close();
}

bool IntelWeb::createNew(const std::string& filePrefix, unsigned int maxDataItems)
{
	close();

	int bucketNumber = (int)((double)maxDataItems / 0.75);

	if (old_map.createNew(filePrefix + STRING_FOR_OLDMAP, bucketNumber) &&
		new_map.createNew(filePrefix + STRING_FOR_NEWMAP, bucketNumber))
	{
		m_filename = filePrefix;
		return true;
	}
	return false;
}

bool IntelWeb::openExisting(const std::string& filePrefix)
{
	close();

	bool oldSuccess;
	bool newSuccess;

	oldSuccess = old_map.openExisting(filePrefix + STRING_FOR_OLDMAP);
	newSuccess = new_map.openExisting(filePrefix + STRING_FOR_NEWMAP);

	if (oldSuccess == true && newSuccess == true)
	{
		m_filename = filePrefix;
		return true;
	}
	return false;
}

void IntelWeb::close()
{
	old_map.close();
	new_map.close();
}

bool IntelWeb::ingest(const std::string& telemetryFile)
{
	ifstream tf(telemetryFile);
	if (! tf)
	{
		cerr << "Unable to open " << telemetryFile << " file" << endl;
		return false;
	}

	unordered_map<string, int>::iterator it1;
	unordered_map<string, int>::iterator it2;
	string s;

	while (getline(tf, s))
	{
		string key;
		string value;
		string context;
		istringstream stringstream(s);

		if (! (stringstream >> context >> key >> value))
		{
			cerr << "Ignoring badly-formatted input line: " << s << endl;
			continue;
		}
		old_map.insert(key, value, context);
		new_map.insert(value, key, context);
	}

	return true;
}

unsigned int IntelWeb::crawl(const std::vector<std::string>& indicators, unsigned int minPrevalenceToBeGood,
							std::vector<std::string>& badEntitiesFound, std::vector<InteractionTuple>& badInteractions)
{
	badEntitiesFound.clear();

	queue<string> qq;
	unordered_set<string> malSet;
	unordered_map<string, bool> importance;
	unsigned int maliciousCount = 0;

	set<InteractionTuple, tupesComp> badInteractionSet;

	for (size_t f = 0; f < indicators.size(); f++)
	{
		DiskMultiMap::Iterator it1 = new_map.search(indicators[f]);
		DiskMultiMap::Iterator it2 = old_map.search(indicators[f]);
		
		if (it1.isValid() || it2.isValid())
		{
			qq.push(indicators[f]);
			malSet.insert(indicators[f]);
			maliciousCount++;
		}
	}

	while (qq.empty() == false)
	{
		string fr0nt= qq.front();
		qq.pop();
		DiskMultiMap::Iterator it1 = old_map.search(fr0nt);
		DiskMultiMap::Iterator it2 = new_map.search(fr0nt);
		
		maliciousCount = maliciousCount + Inspect(true, it1, malSet, badInteractionSet, qq, minPrevalenceToBeGood, importance);
		maliciousCount = maliciousCount + Inspect(false, it2, malSet, badInteractionSet, qq, minPrevalenceToBeGood, importance);
	}

	// set badEntitiesFound equal to the vector we made inside of the method
	badEntitiesFound = vector<string>(malSet.begin(), malSet.end());

	// set badInteractions equal to the vector we made inside of the method 
	badInteractions = vector<InteractionTuple>(badInteractionSet.begin(), badInteractionSet.end());

	// sort the badEntitiesFound
	sort(badEntitiesFound.begin(), badEntitiesFound.end());

	// erase all duplicates
	badInteractions.erase(unique(badInteractions.begin(), badInteractions.end(), (tupesEqual())), badInteractions.end());
	
	return maliciousCount;
}

bool IntelWeb::purge(const std::string& entity)
{
	DiskMultiMap::Iterator it;
	MultiMapTuple tupac;
	int numPurged = 0;

	it = new_map.search(entity);
	while (it.isValid() == true) 
	{
		tupac = *it;
		numPurged = numPurged + new_map.erase(tupac.key, tupac.value, tupac.context);
		++it;
	}

	it = old_map.search(entity);
	while (it.isValid() == true) 
	{
		tupac = *it;
		numPurged = numPurged + old_map.erase(tupac.value, tupac.key, tupac.context);
		++it;
	}

	if (numPurged > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// PRIVATE MEMBER FUNCTION IMPLEMENTATION

bool IntelWeb::isImportant(string key, unsigned int minImportanceRequired)
{
	DiskMultiMap::Iterator oldIT = old_map.search(key);
	DiskMultiMap::Iterator newIT = new_map.search(key);
	unsigned int count = 0;

	// simple while loop that counts priority of items in old map
	while (oldIT.isValid() == true)
	{
		count++;
		if (count >= minImportanceRequired)
		{
			return true;
		}
		++oldIT;
	}

	// simple while loop that counts priority of items in new map
	while (newIT.isValid() == true)
	{
		count++;
		{
			if (count >= minImportanceRequired)
			{
				return true;
			}
		}
		++newIT;
	}
	return false;
}

int IntelWeb::Inspect(bool frontOrBack, DiskMultiMap::Iterator& it, unordered_set<string>& malSet, set<InteractionTuple, tupesComp>& badInteractionSet,
	                  queue<string>& qq, unsigned int minImportanceRequired, unordered_map<string, bool>& importance)
{
	int maliciousCount = 0;

	while (it.isValid()) 
	{
		MultiMapTuple MMT = *it;
		
		if (frontOrBack == false)
		{
			badInteractionSet.insert(InteractionTuple(MMT.value, MMT.key, MMT.context));
		}
		else
		{
			badInteractionSet.insert(InteractionTuple(MMT.key, MMT.value, MMT.context));
		}

		unordered_set<string>::iterator unorderedIT = malSet.find(MMT.value);

		if (unorderedIT != malSet.end())
		{
			++it;
			continue;
		}

		unordered_map<string, bool>::iterator importanceIT = importance.find(MMT.value);
		bool old;
		
		if (importanceIT == importance.end())
		{
			old = isImportant(MMT.value, minImportanceRequired);
			importance.insert({ MMT.value, old });
		}
		else 
		{
			old = importanceIT->second;
		}

		if (old == false)
		{
			string v = MMT.value;
			qq.push(v);
			malSet.insert(v);
			maliciousCount++;
		}
		++it;
	}

	return maliciousCount;
}