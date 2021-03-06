#include "stdafx.h"
#include "persistent_container.h"
#include "persistent_list.h"
#include "persistent_array.h"
#include "persistent_map.h"
#include "transaction.h"

#include <iostream>
#include <string>
#include <iterator>

template<typename T>
void testPersistentArray()
{
	int size;
	std::cin >> size;
	PersistentArray<T> aElems(size);
	while (true)
	{
		std::string query;
		std::cin >> query;
		if (query == "set")
		{
			int index;
			T value;
			std::cin >> index >> value;
			aElems.setValue(index, value);
		}
		else if (query == "get")
		{
			int index;
			std::cin >> index;
			std::cout << aElems.getValue(index) << std::endl;
		}
		else if (query == "print")
		{
			aElems.print();
		}
		else if (query == "undo")
		{
			int numIter;
			std::cin >> numIter;
			aElems.undo(numIter);
		}
		else if (query == "redo")
		{
			int numIter;
			std::cin >> numIter;
			aElems.redo(numIter);
		}
		else if (query == "exit")
			break;
	}
}

template<typename T>
void testPersistentList()
{
	PersistentList<T> lElems;
	auto pIter = lElems.begin();
	while (true)
	{
		std::string query;
		std::cin >> query;
		if (query == "next")
		{
			pIter->next();
		}
		else if (query == "prev")
		{
			pIter->prev();
		}
		else if (query == "print")
		{
			lElems.print();
		}
		else if (query == "insert")
		{
			T val;
			std::cin >> val;
			lElems.insert(pIter, val);
		}
		else if (query == "erase")
		{
			pIter = lElems.erase(pIter);
		}
		else if (query == "set")
		{
			T val;
			std::cin >> val;
			pIter->setVal(val);
		}
		else if (query == "get")
		{
			std::cout << pIter->getVal() << std::endl;
		}
		else if (query == "undo")
		{
			int val;
			std::cin >> val;
			lElems.undo(val);
		}
		else if (query == "redo")
		{
			int val;
			std::cin >> val;
			lElems.redo(val);
		}
		else if (query == "begin")
		{
			pIter = lElems.begin();
		}
		else if (query == "end")
		{
			pIter = lElems.end();
		}
		else if (query == "exit")
			break;
	}
}

template<typename KeyType, typename ValueType>
void testPersistentMap()
{
	PersistentMap<KeyType, ValueType> treap;

	std::string query;
	KeyType key{};
	ValueType value{};

	while (std::cin >> query)
	{
		if (query == "insert")
		{
			std::cin >> key >> value;
			treap.insert(key, value);
		}
		else if (query == "set")
		{
			std::cin >> key >> value;
			treap.setValue(key, value);
		}
		else if (query == "erase")
		{
			std::cin >> key;
			treap.erase(key);
		}
		else if (query == "get")
		{
			std::cin >> key;
			if (!treap.find(key, value))
			{
				std::cout << "Key has not been found.\n";
			}
			else
			{
				std::cout << value << std::endl;
			}
			continue;
		}
		else if (query == "print")
		{
			treap.print();
			std::cout << std::endl;
			continue;
		}
		else if (query == "undo")
		{
			int val;
			std::cin >> val;
			treap.undo(val);
		}
		else if (query == "redo")
		{
			int val;
			std::cin >> val;
			treap.redo(val);
		}
	}
}

void testTransaction()
{
	auto pArray = std::make_shared<PersistentArray<int> >(1);
	pArray->setValue(0, 1);

	std::cout << pArray->getValue(0) << std::endl;
	{
		Transaction transaction(pArray);

		transaction.run([&pArray]() {
			pArray->setValue(0, 2);
		});

		std::cout << pArray->getValue(0) << std::endl;

		transaction.run([&pArray]() {
			throw std::exception();
		});

	}
	//std::cout << pArray->getValue(0) << std::endl;
	pArray->print();
	std::cout << std::endl;
	pArray->redo(1);
	pArray->print();
	std::cout << std::endl;
}

int main()
{
	//testPersistentArray<int>();
	testPersistentList<int>();
	//testPersistentMap<int, int>();
	//testTransaction();
}