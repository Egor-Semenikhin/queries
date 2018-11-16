#pragma once

#include "collection.h"
#include "order_by.h"
#include "select.h"
#include "where.h"

template <typename TCollection>
class query
{
private:
	TCollection m_Collection;

public:
	query(TCollection && collection)
		: m_Collection(std::forward<TCollection>(collection))
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
		using collection_type = query_where<TCollection, std::decay<TFilter>::type>;
		return query<collection_type>(collection_type(std::move(m_Collection), std::forward<TFilter>(filter)));
	}

	template <typename TConverter>
	auto select(TConverter && converter)
	{
		using collection_type = query_select<TCollection, std::decay<TConverter>::type>;
		return query<collection_type>(collection_type(std::move(m_Collection), std::forward<TConverter>(converter)));
	}

	template <typename TComparer>
	auto order_by(TComparer && comparer)
	{

	}

	decltype(auto) to_vector()
	{
		if constexpr (std::is_rvalue_reference_v<decltype(m_Collection.to_vector())>)
		{
			return std::move(m_Collection.to_vector());
		}
		else
		{
			return m_Collection.to_vector();
		}
	}
};

namespace detail
{
	template <typename TCollection>
	struct query_collection_type_selector
	{
		using type = TCollection;
	};

	template <typename TCollection>
	struct query_collection_type_selector<TCollection&&>
	{
		using type = TCollection;
	};
}	// namespace detail

template <typename TCollection>
auto q(TCollection && collection)
{
	using collection_type = query_collection<typename ::detail::query_collection_type_selector<decltype(collection)>::type>;
	return query<collection_type>(collection_type(std::forward<TCollection>(collection)));
}
