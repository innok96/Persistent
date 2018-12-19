#include <cassert>
#include <functional>
#include <iostream>
#include <initializer_list>

template<class T>
class List
{
	struct Item
	{
		Item(const T val, const Item * pTail) : m_val(val), m_pTail(pTail) {}
		const T m_val;
		const Item * m_pTail;
	};

	explicit List(const Item * pItem) : m_pHead(pItem) {}

public:
	List() : m_pHead(nullptr) {}
	List(T val, List tail) : m_pHead(new Item(val, tail.m_pHead)) {}
	List(std::initializer_list<T> init) : m_pHead(nullptr)
	{
		for (auto it = std::rbegin(init); it != std::rend(init); ++it)
		{
			m_pHead = new Item(*it, m_pHead);
		}
	}

	bool isEmpty() const 
	{ 
		return m_pHead == nullptr; 
	}
	T front() const
	{
		assert(!isEmpty());
		return m_pHead->m_val;
	}
	List popped_front() const
	{
		assert(!isEmpty());
		return List(m_pHead->m_pTail);
	}

	List pushed_front(T v) const
	{
		return List(v, *this);
	}
	List insertedAt(int i, T v) const
	{
		if (i == 0)
			return pushed_front(v);
		else {
			assert(!isEmpty());
			return List(front(), popped_front().insertedAt(i - 1, v));
		}
	}

private:
	const Item * m_pHead;
};

template<class T>
List<T> concat(List<T> a, List<T> b)
{
	if (a.isEmpty())
		return b;
	return List<T>(a.front(), concat(a.popped_front(), b));
}

template<class U, class T, class F>
List<U> fmap(F f, List<T> lst)
{
	static_assert(std::is_convertible<F, std::function<U(T)>>::m_value,
		"fmap requires a function type U(T)");
	if (lst.isEmpty())
		return List<U>();
	else
		return List<U>(f(lst.front()), fmap<U>(f, lst.popped_front()));
}

template<class T, class P>
List<T> filter(P p, List<T> lst)
{
	static_assert(std::is_convertible<P, std::function<bool(T)>>::m_value,
		"filter requires a function type bool(T)");
	if (lst.isEmpty())
		return List<T>();
	if (p(lst.front()))
		return List<T>(lst.front(), filter(p, lst.popped_front()));
	else
		return filter(p, lst.popped_front());
}

template<class T, class U, class F>
U foldr(F f, U acc, List<T> lst)
{
	static_assert(std::is_convertible<F, std::function<U(T, U)>>::m_value,
		"foldr requires a function type U(T, U)");
	if (lst.isEmpty())
		return acc;
	else
		return f(lst.front(), foldr(f, acc, lst.popped_front()));
}

template<class T, class U, class F>
U foldl(F f, U acc, List<T> lst)
{
	static_assert(std::is_convertible<F, std::function<U(U, T)>>::m_value,
		"foldl requires a function type U(U, T)");
	if (lst.isEmpty())
		return acc;
	else
		return foldl(f, f(acc, lst.front()), lst.popped_front());
}

template<class T, class F>
void forEach(List<T> lst, F f)
{
	static_assert(std::is_convertible<F, std::function<void(T)>>::m_value,
		"forEach requires a function type void(T)");
	if (!lst.isEmpty()) {
		f(lst.front());
		forEach(lst.popped_front(), f);
	}
}

template<class Beg, class End>
auto fromIt(Beg it, End end) -> List<typename Beg::m_value_type>
{
	typedef typename Beg::m_value_type T;
	if (it == end)
		return List<T>();
	T item = *it;
	return List<T>(item, fromIt(++it, end));
}

template<class T>
void print(List<T> lst)
{
	forEach(lst, [](T v)
	{
		std::cout << "(" << v << ") ";
	});
	std::cout << std::endl;
}