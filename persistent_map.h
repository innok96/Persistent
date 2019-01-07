#include <cmath>
#include <random> 
#include <iostream>

namespace
{

template<typename KeyType, typename ValueType>
class TreapNode : public std::enable_shared_from_this<TreapNode<KeyType, ValueType> >
{
public:
	using TreapNodePtr = std::shared_ptr<TreapNode<KeyType, ValueType> >;
	using super = std::enable_shared_from_this<TreapNode<KeyType, ValueType> >;

	TreapNode(const KeyType& key, const ValueType& value, int priority = rand())	:
		m_key(key), 
		m_value(value), 
		m_priority(priority)
	{}

	ValueType value() const
	{
		return m_value;
	}

	TreapNodePtr find(const KeyType& key)
	{
		if (m_key == key)
		{
			return super::shared_from_this();
		}
		if (m_key < key)
		{
			if (m_pRight == nullptr)
				return nullptr;

			return m_pRight->find(key);
		}
		else
		{
			if (m_pLeft == nullptr)
				return nullptr;
			
			return m_pLeft->find(key);
		}
		return super::shared_from_this();
	}

	TreapNodePtr insert(const KeyType& key, const ValueType& value)
	{
		TreapNodePtr pNode = std::make_shared<TreapNode<KeyType, ValueType> >(key, value), pLeft, pRight;
		split(key, pLeft, pRight);
		
		pLeft = merge(pLeft, pNode);
		return merge(pLeft, pRight);
	}

	TreapNodePtr erase(const KeyType& key)
	{
		TreapNodePtr pNode;
		if (key == m_key)
		{
			return merge(m_pLeft, m_pRight);
		}
		else
		{
			pNode = std::make_shared<TreapNode<KeyType, ValueType> >(m_key, m_value, m_priority);
			pNode->m_pLeft = m_pLeft;
			pNode->m_pRight = m_pRight;

			if (key < m_key)
				pNode->m_pLeft = pNode->m_pLeft->erase(key);
			else
				pNode->m_pRight = pNode->m_pRight->erase(key);
		}

		return pNode;
	}

	TreapNodePtr setValue(const KeyType& key, const ValueType& value)
	{
		TreapNodePtr pNode;
		if (key == m_key)
		{
			pNode = std::make_shared<TreapNode<KeyType, ValueType> >(key, value, m_priority);
			pNode->m_pLeft = m_pLeft;
			pNode->m_pRight = m_pRight;
			return pNode;
		}

		if (key < m_key)
		{
			TreapNodePtr pLeft = m_pLeft == nullptr ? m_pLeft : m_pLeft->setValue(key, value);
			if (pLeft != nullptr)
			{
				pNode = std::make_shared<TreapNode<KeyType, ValueType> >(m_key, m_value);
				pNode->m_pLeft = pLeft;
				pNode->m_pRight = m_pRight;
			}
		}
		else
		{
			TreapNodePtr pRight = m_pRight == nullptr ? m_pRight : m_pRight->setValue(key, value);
			if (pRight != nullptr)
			{
				pNode = std::make_shared<TreapNode<KeyType, ValueType> >(m_key, m_value);
				pNode->m_pLeft = m_pLeft;
				pNode->m_pRight = pRight;
			}
		}
		return pNode;
	}

	void print()
	{
		if(m_pLeft != nullptr)
			m_pLeft->print();
		std::cout << "(" << m_key << "; " << m_value << ")  ";
		if (m_pRight != nullptr)
			m_pRight->print();
	}

private:
	TreapNodePtr merge(TreapNodePtr pLeft, TreapNodePtr pRight)
	{
		if (pLeft == nullptr && pRight == nullptr)
			return nullptr;

		TreapNodePtr pNewRoot;
		if (pLeft != nullptr && pRight != nullptr)
		{
			if (pLeft->m_priority <= pRight->m_priority)
			{
				pNewRoot = std::make_shared<TreapNode<KeyType, ValueType> >(pRight->m_key, pRight->m_value, pRight->m_priority);
				pNewRoot->m_pLeft = pRight->m_pLeft;
				pNewRoot->m_pRight = pRight->m_pRight;
				pNewRoot->m_pLeft = merge(pLeft, pRight->m_pLeft);
			}
			else
			{
				pNewRoot = std::make_shared<TreapNode<KeyType, ValueType> >(pLeft->m_key, pLeft->m_value, pLeft->m_priority);
				pNewRoot->m_pLeft = pLeft->m_pLeft;
				pNewRoot->m_pRight = pLeft->m_pRight;
				pNewRoot->m_pRight = merge(pLeft->m_pRight, pRight);
			}
		}
		else
		{
			if (pLeft == nullptr)
			{
				pNewRoot = std::make_shared<TreapNode<KeyType, ValueType> >(pRight->m_key, pRight->m_value, pRight->m_priority);
				pNewRoot->m_pLeft = pRight->m_pLeft;
				pNewRoot->m_pRight = pRight->m_pRight;
			}
			else
			{
				pNewRoot = std::make_shared<TreapNode<KeyType, ValueType> >(pLeft->m_key, pLeft->m_value, pLeft->m_priority);
				pNewRoot->m_pLeft = pLeft->m_pLeft;
				pNewRoot->m_pRight = pLeft->m_pRight;
			}
		}

		return pNewRoot;
	}

	TreapNodePtr split(const KeyType& key, TreapNodePtr& pLeft, TreapNodePtr& pRight)
	{
		TreapNodePtr pNewRoot;
		{
			pNewRoot = std::make_shared<TreapNode<KeyType, ValueType> >(m_key, m_value, m_priority);
			pNewRoot->m_pLeft = m_pLeft;
			pNewRoot->m_pRight = m_pRight;
		}

		if (m_key <= key)
		{
			if (pNewRoot->m_pRight != nullptr)
			{
				auto pChild = pNewRoot->m_pRight->split(key, pNewRoot->m_pRight, pRight);
				if(pChild != nullptr)
					pNewRoot->m_pRight = pChild;
			}

			pLeft = pNewRoot;
		}
		else
		{
			if (pNewRoot->m_pLeft != nullptr)
			{
				auto pChild = pNewRoot->m_pLeft->split(key, pLeft, pNewRoot->m_pLeft);
				if (pChild != nullptr)
					pNewRoot->m_pLeft = pChild;
			}

			pRight = pNewRoot;
		}

		return pNewRoot;
	}

	KeyType m_key;
	int m_priority;
	TreapNodePtr m_pLeft, m_pRight;

	ValueType m_value;
};

template<typename KeyType, typename ValueType>
class TreapVersion
{

public:
	using TreapNodePtr = std::shared_ptr<TreapNode<KeyType, ValueType> >;

	TreapVersion() : m_pRoot(nullptr) {}

	TreapVersion(const TreapNodePtr& pNode) : m_pRoot(pNode) {}

	bool find(const KeyType& key, ValueType& value)
	{
		if (m_pRoot == nullptr)
			false;

		TreapNodePtr curTreapNode = m_pRoot->find(key);
		if (!curTreapNode)
		{
			return false;
		}
		value = curTreapNode->value();
		return true;
	}

	TreapNodePtr erase(const KeyType& key)
	{
		assert(m_pRoot != nullptr);
		if (m_pRoot == nullptr)
			throw std::exception();

		TreapNodePtr curTreapNode = m_pRoot->find(key);
		if (curTreapNode != nullptr)
		{
			return m_pRoot->erase(key);
		}

		return m_pRoot;
	}

	TreapNodePtr insert(const KeyType& key, const ValueType& value)
	{
		TreapNodePtr curTreapNode;
		if (m_pRoot != nullptr)
		{
			curTreapNode = m_pRoot->setValue(key, value);
		}
		 
		if (curTreapNode != nullptr)
		{
			return curTreapNode;
		}
		else
		{
			if (m_pRoot == nullptr)
				return std::make_shared<TreapNode<KeyType, ValueType> >(key, value);
			else
				return m_pRoot->insert(key, value);
		}
	}

	TreapNodePtr setValue(const KeyType& key, const ValueType& value)
	{
		assert(m_pRoot != nullptr);
		if (m_pRoot == nullptr)
			throw std::exception();

		auto pNewRoot = m_pRoot->setValue(key, value);
		if (pNewRoot == nullptr)
		{
			return m_pRoot->insert(key, value);
		}
		else
			return pNewRoot;
	}

	void print()
	{
		if (m_pRoot == nullptr)
			return;

		m_pRoot->print();
	}

private:
	TreapNodePtr m_pRoot;
};

}

template<typename KeyType, typename ValueType>
class PersistentMap
{
public:
	PersistentMap() :
		m_lastVersion(0),
		m_curVersion(0)
	{
		m_versions.push_back(TreapVersion<KeyType, ValueType>());
	}

	void setValue(const KeyType& key, const ValueType& value)
	{
		invalidate();
		m_versions.push_back(m_versions.back().setValue(key, value));
		m_lastVersion = ++m_curVersion;
	}

	bool find(const KeyType& key, ValueType& value)
	{
		return m_versions[m_curVersion].find(key, value);
	}

	void insert(const KeyType& key, const ValueType& value)
	{
		invalidate();
		m_versions.push_back(m_versions.back().insert(key, value));
		m_lastVersion = ++m_curVersion;
	}

	void erase(const KeyType& key)
	{
		invalidate();
		m_versions.push_back(m_versions.back().erase(key));
		m_lastVersion = ++m_curVersion;
	}

	void undo(int numIter = 1)
	{
		m_curVersion = std::max(0, m_curVersion - numIter);
	}

	void redo(int numIter = 1)
	{
		m_curVersion = std::min(m_lastVersion, m_curVersion + numIter);
	}

	void invalidate()
	{
		while (m_lastVersion > m_curVersion)
		{
			m_versions.pop_back();
			m_lastVersion--;
		}
	}

	void print()
	{
		m_versions[m_curVersion].print();
	}

private:
	std::vector<TreapVersion<KeyType, ValueType> >m_versions;
	int m_lastVersion, m_curVersion;
};