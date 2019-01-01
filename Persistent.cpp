#include "stdafx.h"
#include "persistent_list.h"
#include "persistent_array.h"

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
			lElems.erase(pIter);
		}
		else if (query == "set")
		{
			T val;
			std::cin >> val;
			pIter->setVal(val);
		}
		else if (query == "exit")
			break;
	}
}


int main()
{
	testPersistentList<int>();
}