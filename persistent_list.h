#include <cassert>
#include <functional>
#include <iostream>
#include <initializer_list>
#include <memory>


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
	PersistentList()
	{
		m_pHead = std::make_shared<T>();
	}

	PersistentListIterator begin()
	{
		return PersistentListIterator<T>(m_pHead);
	}

	PersistentListIterator begin()
	{
		return PersistentListIterator<T>(m_pHead);
	}


private:
	using NodePtr = std::shared_ptr<Node<T> >;
	friend class PersistentListIterator<T>;
	
	NodePtr m_pHead;
	NodePtr m_pTail;
};

