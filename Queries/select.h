#pragma once

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
		: m_Collection(std::forward<TCollection>(collection))
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
