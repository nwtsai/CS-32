#ifndef INTELWEB_H_
#define INTELWEB_H_

#include "InteractionTuple.h"
#include "DiskMultiMap.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <set>
#include <stdio.h>

class IntelWeb
{
public:
	IntelWeb();
	~IntelWeb();
	bool createNew(const std::string& filePrefix, unsigned int maxDataItems);
	bool openExisting(const std::string& filePrefix);
	void close();
	bool ingest(const std::string& telemetryFile);
	unsigned int crawl(const std::vector<std::string>& indicators,
		unsigned int minPrevalenceToBeGood,
		std::vector<std::string>& badEntitiesFound,
		std::vector<InteractionTuple>& interactions
		);
	bool purge(const std::string& entity);

private:

	//PRIVATE STRUCTS
	struct tupesComp
	{
		bool operator() (const InteractionTuple &a, const InteractionTuple &b) const 
		{
			if (a.context < b.context) 
			{
				return true;
			}
			else if (a.context == b.context) 
			{
				if (a.from < b.from) 
				{
					return true;
				}
				else if (a.from == b.from) 
				{
					if (a.to < b.to) 
					{
						return true;
					}
				}
			}
			return false;
		}
	};

	struct tupesEqual
	{
		bool operator() (const InteractionTuple &a, const InteractionTuple &b) const
		{
			if (a.to == b.to && a.from == b.from && a.context == b.context)
			{
				return true;
			}
			return false;
		}
	};

	struct multTupesEqual
	{
		bool operator() (const MultiMapTuple &a, const MultiMapTuple &b) const
		{
			if (a.key == b.key && a.value == b.value && a.context == b.context)
			{
				return true;
			}
			return false;
		}
	};

	//	PRIVATE DATA MEMBERS
	DiskMultiMap old_map;
	DiskMultiMap new_map;
	
	string m_filename;

	// PRIVATE STATIC CONSTANTS
	static const string STRING_FOR_OLDMAP;
	static const string STRING_FOR_NEWMAP;

	// PRIVATE MEMBER FUNCTIONS
	bool isImportant(string key, unsigned int minImportanceRequired);
	int Inspect(bool frontOrBack, DiskMultiMap::Iterator& it, unordered_set<string>& badSet, set<InteractionTuple, tupesComp>& badInteractionSet, 
		        queue<string>& qq, unsigned int minPrevalenceToBeGood, unordered_map<string, bool>& importance);
};

#endif // INTELWEB_H_