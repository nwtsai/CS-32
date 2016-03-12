#include "DiskMultiMap.h"
#include <string.h>
#include <stdio.h>
#include <functional>

using namespace std;

// DISKMULTIMAP IMPLEMENTATION

DiskMultiMap::DiskMultiMap()
{}

DiskMultiMap::~DiskMultiMap()
{
	close();
}

bool DiskMultiMap::createNew(const std::string& filename, unsigned int numBuckets)
{
	close();

	bool success = bf.createNew(filename);
	if (success == false)
	{
		return false;
	}

	// save the file name 
	file_name = filename;

	int co = sizeof(Head);
	int bucketSize = sizeof(Bucket);
	bool b00l = true;

	for (unsigned int a = 0; a < numBuckets; a++)
	{
		b00l = bf.write(Bucket(), co) && b00l;
		co += bucketSize;
	}

	Head hd;
	hd.butt = co;
	hd.numBucks = numBuckets;

	b00l = bf.write(hd, 0) && b00l;
	return b00l;
}

bool DiskMultiMap::openExisting(const std::string& filename)
{
	close();

	bool success = bf.openExisting(filename);
	file_name = filename;
	return success;
}

void DiskMultiMap::close()
{
	bf.close();
}

bool DiskMultiMap::insert(const std::string& key, const std::string& value, const std::string& context)
{
	if (bf.isOpen() == false)
	{
		return false;
	}
		
	if (key.size() > MAX_STRING_SIZE || value.size() > MAX_STRING_SIZE || context.size() > MAX_STRING_SIZE)
	{
		return false;
	}

	MultiMapTuple MMT;
	MMT.key = key;
	MMT.value = value;
	MMT.context = context;

	Node add(MMT);

	size_t hash = generateHash(key);

	int bIndex = updateHash(hash);
	Bucket thisBuck;
	if (bf.read(thisBuck, bIndex) == false)
	{
		cerr << "Unable to read current bucket" << endl;
	}

	Head hd;
	if (bf.read(hd, 0) == false)
	{
		cerr << "Unable to read head" << endl;
	}

	int nIndex = none;
	if (hd.deleted == none)
	{
		nIndex = hd.butt;
		hd.butt = hd.butt + sizeof(Node);
		add.next = thisBuck.begin;
		thisBuck.begin = nIndex;
	}
	else
	{
		bf.read(add, hd.deleted);
		nIndex = hd.deleted;
		int nextD = add.next;
		add.next = thisBuck.begin;
		thisBuck.begin = nIndex;
		hd.deleted = nextD;
	}

	if (updateHead(hd) == false)
	{
		return false;
	}
	if (updateBucket(thisBuck, bIndex) == false)
	{
		return false;
	}
	
	// insert node to the right index
	return updateNode(add, nIndex);
}

DiskMultiMap::Iterator DiskMultiMap::search(const std::string& key)
{
	bool isopen;
	isopen = bf.isOpen();
	if (isopen == false)
	{
		return Iterator();
	}

	int bIndex = updateHash(generateHash(key));

	Bucket b;
	bf.read(b, bIndex);

	Iterator it = Iterator(&bf, key, b.begin);
	return it;
}

int DiskMultiMap::erase(const std::string& key, const std::string& value, const std::string& context)
{
	bool isopen;
	isopen = bf.isOpen();
	if (isopen == false)
	{
		return 0;
	}

	Head hd;
	if (bf.read(hd, 0) == false)
	{
		cerr << "Unable to read head" << endl;
	}

	int bIndex = updateHash(generateHash(key));

	Bucket b;
	bf.read(b, bIndex);
	Node node;
	Node buttNode;

	if (b.begin != none)
	{
		bf.read(node, b.begin);
	}

	int currAdd = b.begin;
	int prevAdd = none;

	int count = 0;

	while (currAdd != none)
	{
		if (prevAdd != none)
		{
			bf.read(buttNode, prevAdd);
		}
		bf.read(node, currAdd);
		int nNext = node.next;

		string k = node.key;
		string v = node.value;
		string c = node.context;

		if (key == k && value == v && context == c)
		{
			count++;

			int U_KNIGHTED = hd.deleted;
			if (prevAdd == none)
			{
				b.begin = node.next;
				updateBucket(b, bIndex);
			}
			else
			{
				buttNode.next = node.next;
			}

			node.next = U_KNIGHTED;

			hd.deleted = currAdd;
			if (prevAdd != none)
			{
				updateNode(buttNode, prevAdd);
			}
			updateNode(node, currAdd);
			updateHead(hd);
			currAdd = nNext;
		}
		else
		{
			currAdd = nNext;
			prevAdd = currAdd;
		}
	}
	return count;
}

// ITERATOR IMPLEMENTATION

DiskMultiMap::Iterator::Iterator()
{
	m_address = none;
	m_key = "";
	m_bf = NULL;
}

DiskMultiMap::Iterator::Iterator(BinaryFile* bfPointer, string key, int currentAddress)
{
	m_bf = bfPointer;
	m_key = key;
	Node n;
	m_address = currentAddress;

	while (m_address != none)
	{
		m_bf->read(n, m_address);
		if (strcmp(n.key, m_key.c_str()) == 0)
		{
			// cerr << "Something was found at addr : " << m_address << endl;
			// cerr << n.key << endl;
			break;
		}
		m_address = n.next;
	}
}

bool DiskMultiMap::Iterator::isValid() const
{
	if (m_address == none)
	{
		return false;
	}
	return true;
}

DiskMultiMap::Iterator& DiskMultiMap::Iterator::operator++()
{
	if (isValid() == false)
	{
		return *this;
	}
	Node n;
	m_bf->read(n, m_address);
	m_address = n.next;
	while (m_address != none)
	{
		m_bf->read(n, m_address);
		if (strcmp(n.key, m_key.c_str()) == 0)
		{
			break;
		}
		int nextAdd = n.next;
		m_address = nextAdd;
	}
	return *this;
}

MultiMapTuple DiskMultiMap::Iterator::operator*()
{
	if (isValid() == false)
	{
		return MultiMapTuple();
	}
	Node n;
	if (m_bf->read(n, m_address) == false)
	{
		return MultiMapTuple();
	}

	MultiMapTuple MMT;
	MMT.key = n.key;
	MMT.value = n.value;
	MMT.context = n.context;
	return MMT;
}

// PRIVATE MEMBER FUNCTION IMPLEMENTATIONS

size_t DiskMultiMap::generateHash(const string s)
{
	hash<string> HK;
	return HK(s);
}

int DiskMultiMap::updateHash(const size_t hT)
{
	Head hd;
	if (bf.read(hd, 0) == false)
	{
		cerr << "Unable to read head" << endl;
	}
		
	return (hT % hd.numBucks) * sizeof(Bucket) + sizeof(Head);
}

bool DiskMultiMap::updateHead(Head h)
{
	return bf.write(h, 0);
}

bool DiskMultiMap::updateBucket(Bucket b, int offset)
{
	return bf.write(b, offset);
}

bool DiskMultiMap::updateNode(Node n, int offset)
{
	return bf.write(n, offset);
}