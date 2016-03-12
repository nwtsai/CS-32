#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include "MultiMapTuple.h"
#include "BinaryFile.h"

using namespace std;

class DiskMultiMap
{
public:
	class Iterator
	{
	public:
		Iterator();
		Iterator(BinaryFile * f, std::string k, int curAddress); // O(N/B)
		bool isValid() const;
		Iterator& operator++();
		MultiMapTuple operator*();
	private:
		BinaryFile* m_bf;
		string m_key;
		int m_address;
	};

	DiskMultiMap();
	~DiskMultiMap();
	bool createNew(const std::string& filename, unsigned int numBuckets); // O(B)
	bool openExisting(const std::string& filename); // O(1)
	void close(); // O(1)
	bool insert(const std::string& key, const std::string& value, const std::string& context); // O(N/B)
	Iterator search(const std::string& key); // O(N/B)
	int erase(const std::string& key, const std::string& value, const std::string& context); // O(N/B)

private:
	static const int none = -1; 
	static const int MAX_STRING_SIZE = 120;

	struct Head 
	{
		Head() 
		{
			numBucks = 0;
			butt = none;
			deleted = none;
		}
		int numBucks;
		int butt;
		int deleted;
	};

	struct Bucket 
	{
		Bucket() 
		{
			begin = none;
		}
		int begin;
	};

	struct Node 
	{
		Node() 
		{
			key[0] = 0;
			value[0] = 0;
			context[0] = 0;
			next = none;
		}
		Node(MultiMapTuple MMT) 
		{
			strcpy_s(key, MMT.key.c_str());
			strcpy_s(value, MMT.value.c_str());
			strcpy_s(context, MMT.context.c_str());
		}
		char key[MAX_STRING_SIZE];
		char value[MAX_STRING_SIZE];
		char context[MAX_STRING_SIZE];
		int next;
	};

	// PRIVATE DATA MEMBERS
	string file_name;
	BinaryFile bf;

	// PRIVATE MEMBER DECLARATIONS
	size_t generateHash(const string s);
	int updateHash(const size_t hT);
	bool updateHead(Head h);
	bool updateBucket(Bucket b, int offset);
	bool updateNode(Node n, int offset);
};

#endif // DISKMULTIMAP_H_