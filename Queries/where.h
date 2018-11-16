#pragma once

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
