#include "pch.h"

template <typename TCollection, typename TFilter>
class query_where
{
public:
	template <typename TNestedIterator>
	class iterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = typename std::iterator_traits<TNestedIterator>::value_type;
		using difference_type = typename std::iterator_traits<TNestedIterator>::difference_type;
		using pointer = typename std::iterator_traits<TNestedIterator>::pointer;
		using reference = typename std::iterator_traits<TNestedIterator>::reference;

	private:
		TNestedIterator m_Iter;
		query_where<TCollection, TFilter>& m_Query;

	public:
		iterator(TNestedIterator iter, query_where<TCollection, TFilter>& query)
			: m_Iter(iter)
			, m_Query(query)
		{
		}

		decltype(auto) operator*() const
		{
			return (*m_Iter);
		}

		iterator& operator++()
		{
			if (m_Iter != m_Query.m_Collection.end())
			{
				while (true)
				{
					++m_Iter;
					if (m_Iter == m_Query.m_Collection.end())
					{
						break;
					}
					if (m_Query.m_Filter(*m_Iter))
					{
						break;
					}
				}
			}
			return *this;
		}

		iterator operator++(int)
		{
			iterator tmp = *this;
			++*this;
			return tmp;
		}

		bool operator == (const iterator& iter) const
		{
			return m_Iter == iter.m_Iter;
		}

		bool operator != (const iterator& iter) const
		{
			return m_Iter != iter.m_Iter;
		}
	};

private:
	TCollection m_Collection;
	TFilter m_Filter;

public:
	query_where(TCollection && collection, TFilter && filter)
		: m_Collection(std::forward<TCollection>(collection))
		, m_Filter(std::forward<TFilter>(filter))
	{
	}

	auto begin()
	{
		auto iter = m_Collection.begin();
		while (iter != m_Collection.end() && !m_Filter(*iter))
		{
			++iter;
		}
		return iterator<decltype(iter)>(iter, *this);
	}

	auto end()
	{
		auto iter = m_Collection.end();
		return iterator<decltype(iter)>(iter, *this);
	}
};

template <typename TCollection, typename TConverter>
class query_select
{
public:
	template <typename TNestedIterator>
	class iterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = typename std::iterator_traits<TNestedIterator>::value_type;
		using difference_type = typename std::iterator_traits<TNestedIterator>::difference_type;
		using pointer = typename std::iterator_traits<TNestedIterator>::pointer;
		using reference = typename std::iterator_traits<TNestedIterator>::reference;

	private:
		TNestedIterator m_Iter;
		query_select<TCollection, TConverter>& m_Query;

	public:
		iterator(TNestedIterator iter, query_select<TCollection, TConverter>& query)
			: m_Iter(iter)
			, m_Query(query)
		{
		}

		auto operator*() const
		{
			return m_Query.m_Converter(*m_Iter);
		}

		iterator& operator++()
		{
			++m_Iter;
			return *this;
		}

		iterator operator++(int)
		{
			iterator tmp = *this;
			++*this;
			return tmp;
		}

		bool operator == (const iterator& iter) const
		{
			return m_Iter == iter.m_Iter;
		}

		bool operator != (const iterator& iter) const
		{
			return m_Iter != iter.m_Iter;
		}
	};

private:
	TCollection m_Collection;
	TConverter m_Converter;

public:
	query_select(TCollection && collection, TConverter && converter)
		: m_Collection (std::forward<TCollection>(collection))
		, m_Converter(std::forward<TConverter>(converter))
	{
	}

	auto begin()
	{
		auto iter = m_Collection.begin();
		return iterator<decltype(iter)>(iter, *this);
	}

	auto end()
	{
		auto iter = m_Collection.end();
		return iterator<decltype(iter)>(iter, *this);
	}
};

template <typename TCollection, typename TComparer>
class query_order_by
{
private:

public:
	query_order_by(TCollection && collection, TComparer && comparer)
	{
	}
};

template <typename TCollection>
class query_collection
{
public:
	template <typename TNestedIterator>
	class iterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = typename std::iterator_traits<TNestedIterator>::value_type;
		using difference_type = typename std::iterator_traits<TNestedIterator>::difference_type;
		using pointer = typename std::iterator_traits<TNestedIterator>::pointer;
		using reference = typename std::iterator_traits<TNestedIterator>::reference;

	private:
		TNestedIterator m_Iter;

	public:
		explicit iterator(TNestedIterator iter)
			: m_Iter(iter)
		{
		}

		decltype(auto) operator*() const
		{
			return (*m_Iter);
		}

		iterator& operator++()
		{
			++m_Iter;
			return *this;
		}

		iterator operator++(int)
		{
			iterator tmp = *this;
			++*this;
			return tmp;
		}

		bool operator == (const iterator& iter) const
		{
			return m_Iter == iter.m_Iter;
		}

		bool operator != (const iterator& iter) const
		{
			return m_Iter != iter.m_Iter;
		}
	};

private:
	TCollection && m_Collection;

public:
	query_collection(TCollection && collection)
		: m_Collection(std::forward<TCollection>(collection))
	{
	}

	auto begin()
	{
		auto iter = m_Collection.begin();
		return iterator<decltype(iter)>(iter);
	}

	auto end()
	{
		auto iter = m_Collection.end();
		return iterator<decltype(iter)>(iter);
	}
};

template <typename TCollection>
class query
{
private:
	TCollection m_Collection;

public:
	query(TCollection && collection)
		: m_Collection (std::forward<TCollection>(collection))
	{
	}

	auto begin()
	{
		return m_Collection.begin();
	}

	auto end()
	{
		return m_Collection.end();
	}

	template <typename TFilter>
	auto where(TFilter && filter)
	{
		query_where<TCollection, std::decay<TFilter>::type> c(std::move(m_Collection), std::forward<TFilter>(filter));
		return query<decltype(c)>(std::move(c));
	}

	template <typename TConverter>
	auto select(TConverter && converter)
	{
		query_select<TCollection, std::decay<TConverter>::type> c(std::move(m_Collection), std::forward<TConverter>(converter));
		return query<decltype(c)>(std::move(c));
	}

	auto as_vector()
	{
		return std::vector<std::decay<decltype(*begin())>::type>(begin(), end());
	}
};

template <typename TCollection>
auto q(TCollection && collection)
{
	query_collection<decltype(collection)> coll(std::forward<TCollection>(collection));
	return query<decltype(coll)>(std::move(coll));
}

__declspec(noinline) std::vector<int> CreateTestVector()
{
	std::vector<int> vec = { 1, 2, 3, 4 };
	return vec;
}

template <typename TNestedIterator>
class test_iterator
{
public:
	using iterator_category = std::forward_iterator_tag;
	using value_type = typename std::iterator_traits<TNestedIterator>::value_type;
	using difference_type = typename std::iterator_traits<TNestedIterator>::difference_type;
	using pointer = typename std::iterator_traits<TNestedIterator>::pointer;
	using reference = typename std::iterator_traits<TNestedIterator>::reference;

private:
	TNestedIterator m_Iter;

public:
	explicit test_iterator(TNestedIterator iter)
		: m_Iter(iter)
	{
	}

	decltype(auto) operator*() const
	{
		return (*m_Iter);
	}

	test_iterator& operator++()
	{
		++m_Iter;
		return *this;
	}

	test_iterator operator++(int)
	{
		test_iterator tmp = *this;
		++*this;
		return tmp;
	}

	bool operator == (const test_iterator& iter) const
	{
		return m_Iter == iter.m_Iter;
	}

	bool operator != (const test_iterator& iter) const
	{
		return m_Iter != iter.m_Iter;
	}
};

int main()
{
	auto query = q(CreateTestVector())
		.where([](auto x){ return x > 1; })
		.select([](auto x) { return float(x); })
		.as_vector();

	float sum = 0;

	for (auto t : query)
	{
		sum += t;
	}

	std::cout << sum;
}
