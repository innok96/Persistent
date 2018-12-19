#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>
#include <chrono>
#include <numeric>

const int kMax = 10000;

struct Node 
{

	Node() 
	{
		m_index = 0;
		m_value = 0;
		m_id = 0;
		m_pLeft = m_pRight = nullptr;
	}

	Node(int i, int v, int u) 
	{
		m_index = i;
		m_value = v;
		m_id = u;
		m_pLeft = m_pRight = nullptr;
	}


	int m_index;
	int m_value;
	int m_id;

	Node* m_pLeft;
	Node* m_pRight;
};

class PersistentArray 
{

public:
	PersistentArray() 
	{
		m_pRoot = nullptr;
		m_isDetached = false;
		m_size = 0;
	}

	PersistentArray(unsigned size) 
	{
		m_size = size;
		m_isDetached = true;
		std::vector<int> aIndexes(m_size - 1);
		std::iota(aIndexes.begin(), aIndexes.end(), 1);

		// Randomize indices to raise the chances of having a balanced tree.
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		shuffle(aIndexes.begin(), aIndexes.end(), std::default_random_engine(seed));

		// Create entries for all indices
		m_pRoot = new Node;
		for (const auto& index : aIndexes) 
		{
			m_pRoot = create(m_pRoot, index, 0);
		}
	}

	PersistentArray(const PersistentArray& other) 
	{
		this->m_size = other.m_size;
		this->m_isDetached = false;
		this->m_pRoot = other.m_pRoot;
	}

	PersistentArray& operator=(const PersistentArray& other) 
	{
		this->m_size = other.m_size;
		this->m_isDetached = false;
		this->m_pRoot = other.m_pRoot;
		return *this;
	}

	void setValue(int index, int value) 
	{
		if (!m_isDetached) 
		{
			m_pRoot = setAndDetach(m_pRoot, index, value);
			m_isDetached = true;
		}
		else 
		{
			setValue(m_pRoot, index, value);
		}
	}

	int getValue(int index) 
	{
		return getValue(m_pRoot, index);
	}
	
	~PersistentArray() 
	{
		if (m_isDetached && m_pRoot != nullptr) 
		{
			clean(m_pRoot, m_pRoot->m_id);
			delete m_pRoot;
		}
	}

private:
	unsigned m_size;
	bool m_isDetached;
	Node* m_pRoot;

	Node* create(Node* pRoot, int index, int id) 
	{
		if (pRoot == nullptr) 
		{
			return new Node(index, 0, id);
		}

		if (index < pRoot->m_index) 
		{
			pRoot->m_pLeft = create(pRoot->m_pLeft, index, id);
		}
		else 
		{
			pRoot->m_pRight = create(pRoot->m_pRight, index, id);
		}
		return pRoot;
	}

	void setValue(Node* pRoot, int index, int value) 
	{
		if (pRoot == nullptr) 
		{
			return;
		}

		if (index == pRoot->m_index) 
		{
			pRoot->m_value = value;
			return;
		}

		if (index < pRoot->m_index) 
		{
			setValue(pRoot->m_pLeft, index, value);
		}
		else 
		{
			setValue(pRoot->m_pRight, index, value);
		}
	}

	Node* setAndDetach(Node* pRoot, int index, int value) 
	{
		if (pRoot == nullptr) 
		{
			return pRoot;
		}

		Node* pNode = nullptr;
		if (index == pRoot->m_index) 
		{
			pNode = new Node(index, value, pRoot->m_id + 1);
			pNode->m_pLeft = pRoot->m_pLeft;
			pNode->m_pRight = pRoot->m_pRight;
			return pNode;
		}

		if (index < pRoot->m_index) 
		{
			Node* pLeft = setAndDetach(pRoot->m_pLeft, index, value);
			if (pLeft != nullptr)
			{
				pNode = new Node(pRoot->m_index, pRoot->m_value, pLeft->m_id);
				pNode->m_pLeft = pLeft;
				pNode->m_pRight = pRoot->m_pRight;
			}
		}
		else 
		{
			Node* pRight = setAndDetach(pRoot->m_pRight, index, value);
			if (pRight != nullptr)
			{
				pNode = new Node(pRoot->m_index, pRoot->m_value, pRight->m_id);
				pNode->m_pLeft = pRoot->m_pLeft;
				pNode->m_pRight = pRight;
			}
		}
		return pNode;
	}

	int getValue(Node* pRoot, int index)
	{
		if (pRoot == nullptr)
		{
			return 0;
		}

		if (index == pRoot->m_index)
		{
			return pRoot->m_value;
		}

		if (index < pRoot->m_index)
		{
			return getValue(pRoot->m_pLeft, index);
		}
		else 
		{
			return getValue(pRoot->m_pRight, index);
		}
	}

	void clean(Node* pRoot, int id)
	{
		if (pRoot == nullptr)
		{
			return;
		}

		if (pRoot->m_pLeft != nullptr && pRoot->m_pLeft->m_id == id)
		{
			clean(pRoot->m_pLeft, id);
			delete pRoot->m_pLeft;
		}

		if (pRoot->m_pRight && pRoot->m_pRight->m_id == id)
		{
			clean(pRoot->m_pRight, id);
			delete pRoot->m_pRight;
		}
	}
};