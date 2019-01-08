#pragma once
#include<memory>

class PersistentBase
{
public:
	virtual void undo(int numIter = 1) = 0;
	virtual int lastVersion() = 0;
};
using PersistentBasePtr = std::shared_ptr<PersistentBase>;