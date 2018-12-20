#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <ctime>
#include <random>
#include <chrono>
#include <numeric>
#include <cassert>
#include <memory>

namespace
{

template<typename T>
struct Node 
{
	Node() 
	{
		m_index = 0;
		m_value = T{};
		m_id = 0;
		m_pLeft = m_pRight = nullptr;
	}

	Node(int index, T value, int id) 
	{
		m_index = index;
		m_value = value;
		m_id = id;
		m_pLeft = m_pRight = nullptr;
	}


	int m_index;
	T m_value;
	int m_id;

	std::shared_ptr<Node<T> > m_pLeft;
	std::shared_ptr<Node<T> > m_pRight;
};

template<typename T>
class PersistentArrayVersion
{

public:
	PersistentArrayVersion()
	{
		m_pRoot = nullptr;
		m_size = 0;
	}

	PersistentArrayVersion(int size)
	{
		m_size = size;
		std::vector<int> aIndexes(m_size - 1);
		std::iota(aIndexes.begin(), aIndexes.end(), 1);

		random_shuffle(aIndexes.begin(), aIndexes.end());

		m_pRoot = std::make_shared<Node<T> >();
		for (const auto& index : aIndexes) 
		{
			create(m_pRoot, index, 0);
		}
	}

	PersistentArrayVersion(const PersistentArrayVersion& other)
	{
		this->m_size = other.m_size;
		this->m_pRoot = other.m_pRoot;
	}

	PersistentArrayVersion& operator=(const PersistentArrayVersion& other)
	{
		this->m_size = other.m_size;
		this->m_pRoot = other.m_pRoot;
		return *this;
	}

	void setValue(int index, T value) 
	{
		m_pRoot = setValue(m_pRoot, index, value);
	}

	T getValue(int index) 
	{
		return getValue(m_pRoot, index);
	}

	void print()
	{
		int deep = 0;
		print(m_pRoot, deep);
		std::cout << std::endl;
		//std::cout << deep << std::endl;
	}
	
private:
	using NodePtr = std::shared_ptr<Node<T> >;

	int m_size;
	NodePtr m_pRoot;

	void create(NodePtr& pRoot, int index, int id) 
	{
		if (pRoot == nullptr) 
		{
			pRoot = std::make_shared<Node<T> >(index, T{}, id);
			return;
		}

		if (index < pRoot->m_index) 
		{
			create(pRoot->m_pLeft, index, id);
		}
		else 
		{
			create(pRoot->m_pRight, index, id);
		}
	}

	NodePtr setValue(NodePtr& pRoot, int index, T value)
	{
		if (pRoot == nullptr) 
		{
			return pRoot;
		}

		NodePtr pNode = nullptr;
		if (index == pRoot->m_index) 
		{
			pNode = std::make_shared<Node<T> >(index, value, pRoot->m_id + 1);
			pNode->m_pLeft = pRoot->m_pLeft;
			pNode->m_pRight = pRoot->m_pRight;
			return pNode;
		}

		if (index < pRoot->m_index) 
		{
			NodePtr pLeft = setValue(pRoot->m_pLeft, index, value);
			if (pLeft != nullptr)
			{
				pNode = std::make_shared<Node<T> >(pRoot->m_index, pRoot->m_value, pLeft->m_id);
				pNode->m_pLeft = pLeft;
				pNode->m_pRight = pRoot->m_pRight;
			}
		}
		else 
		{
			NodePtr pRight = setValue(pRoot->m_pRight, index, value);
			if (pRight != nullptr)
			{
				pNode = std::make_shared<Node<T> >(pRoot->m_index, pRoot->m_value, pRight->m_id);
				pNode->m_pLeft = pRoot->m_pLeft;
				pNode->m_pRight = pRight;
			}
		}
		return pNode;
	}

	T getValue(const NodePtr& pRoot, int index)
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

	void print(const NodePtr& pRoot, int& deep)
	{
		if (pRoot == nullptr)
			return;

		int deepLeft = 0, deepRight = 0;
		print(pRoot->m_pLeft, deepLeft);
		std::cout << pRoot->m_value << " ";
		print(pRoot->m_pRight, deepRight);
		deep = std::max(deepLeft, deepRight) + 1;
	}
};

}

template<typename T>
class PersistentArray
{
public:

	PersistentArray() {}

	PersistentArray(int size)
	{
		m_size = size;
		m_lastVer = m_curVer = 0;
		PersistentArrayVersion<T> initVer(size);
		m_versions.push_back(initVer);
	}

	void setValue(int index, T value)
	{
		if (index < 0 || index >= m_size)
		{
			assert(index >= 0 && index < m_size);
			return;
		}

		PersistentArrayVersion<T> newVer(m_versions[m_curVer]);
		newVer.setValue(index, value);
		while (m_lastVer > m_curVer)
		{
			m_versions.pop_back();
			m_lastVer--;
		}

		m_versions.push_back(newVer);
		m_lastVer = ++m_curVer;
	}

	T getValue(int index)
	{
		if (index < 0 || index >= m_size)
		{
			assert(index >= 0 && index < m_size);
			return 0;
		}
		return m_versions[m_curVer].getValue(index);
	}

	void undo(int numIter = 1)
	{
		m_curVer = std::max(0, m_curVer - numIter);
	}

	void redo(int numIter = 1)
	{
		m_curVer = std::min(m_lastVer, m_curVer + numIter);
	}

	void print()
	{
		m_versions[m_curVer].print();
	}

private:
	int m_size;
	int m_lastVer, m_curVer;
	std::vector<PersistentArrayVersion<T> >m_versions;
};