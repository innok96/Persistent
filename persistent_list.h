#include <cassert>
#include <functional>
#include <iostream>
#include <initializer_list>
#include <memory>
#include <vector>


template<typename T>
struct Node
{
	Node()
	{
		m_value = T{};
		m_pLeft = m_pRight = nullptr;
	}

	Node(T value)
	{
		m_value = value;
		m_pLeft = m_pRight = nullptr;
	}

	T m_value;
	std::shared_ptr<Node<T> > m_pLeft;
	std::shared_ptr<Node<T> > m_pRight;
};

template<typename T>
class PersistentList;

template<typename T>
class PersistentListIterator
{
public:
	void next()
	{
		assert(m_pItem != nullptr);
		if (m_pItem == nullptr)
			throw std::exception();

		m_pItem = m_pItem->m_pRight;
	}

	void prev()
	{
		assert(m_pItem != nullptr);
		if (m_pItem == nullptr)
			throw std::exception();

		m_pItem = m_pItem->m_pLeft;
	}

	bool done()
	{
		assert(m_pItem != nullptr);
		if (m_pItem == nullptr)
			throw std::exception();

		return m_pItem->m_pRight == nullptr;
	}

	void setVal(T val)
	{
		assert(m_pItem != nullptr);
		if (m_pItem == nullptr)
			throw std::exception();

		m_pItem->m_value = val;
	}

	T getVal()
	{
		assert(m_pItem != nullptr);
		if (m_pItem == nullptr)
			throw std::exception();

		return m_pItem->m_value;
	}

private:
	using NodePtr = std::shared_ptr<Node<T> >;
	friend class PersistentList<T>;

	PersistentListIterator(const NodePtr& pNode)
	{
		m_pItem = pNode;
	}

	NodePtr m_pItem;
};

template<typename T>
class PersistentList
{
public:
	friend class PersistentListIterator<T>;
	using PersistentListIteratorPtr = std::shared_ptr<PersistentListIterator<T> >;

	PersistentList()
	{
		m_pHead = std::make_shared<Node<T> >();
		m_pTail = m_pHead;
	}

	PersistentListIteratorPtr begin()
	{
		auto pBegin = std::shared_ptr<PersistentListIterator<T> >(new PersistentListIterator<T>(m_pHead));
		m_apIterators.push_back(pBegin);
		return pBegin;
	}

	PersistentListIteratorPtr end()
	{
		auto pEnd = std::shared_ptr<PersistentListIterator<T> >(new PersistentListIterator<T>(m_pTail));
		m_apIterators.push_back(pEnd);
		return pEnd;
	}

	PersistentListIteratorPtr insert(const PersistentListIteratorPtr& pIter, T obj)
	{
		assert(pIter != nullptr);
		if (pIter == nullptr)
			throw std::exception();

		auto pNode = std::make_shared<Node<T> >(obj);
		pNode->m_pLeft = pIter->m_pItem->m_pLeft;
		pNode->m_pRight = pIter->m_pItem;
		if (pNode->m_pLeft != nullptr)
		{
			pNode->m_pLeft->m_pRight = pNode;
		}
		pIter->m_pItem->m_pLeft = pNode;

		if (m_pHead->m_pLeft != nullptr)
			m_pHead = m_pHead->m_pLeft;

		auto pNewIter = std::shared_ptr<PersistentListIterator<T> >(new PersistentListIterator<T>(pNode));
		m_apIterators.push_back(pNewIter);
		return pNewIter;
	}

	PersistentListIteratorPtr erase(PersistentListIteratorPtr& pIter)
	{
		assert(pIter != nullptr && pIter->m_pItem->m_pRight != nullptr);
		if (pIter == nullptr && pIter->m_pItem->m_pRight != nullptr)
			throw std::exception();

		pIter->m_pItem->m_pRight->m_pLeft = pIter->m_pItem->m_pLeft;
		if (pIter->m_pItem->m_pLeft != nullptr)
		{
			pIter->m_pItem->m_pLeft->m_pRight = pIter->m_pItem->m_pRight;
		}

		pIter->next();
		m_apIterators.push_back(pIter);

		return pIter;
	}

	void print()
	{
		for (auto pIter = begin(); !pIter->done(); pIter->next())
		{
			std::cout << pIter->getVal() << " ";
		}
		puts("");
	}

private:
	using NodePtr = std::shared_ptr<Node<T> >;
	
	NodePtr m_pHead;
	NodePtr m_pTail;
	std::vector<PersistentListIteratorPtr> m_apIterators;
};
