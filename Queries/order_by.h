#pragma once

template <typename TCollection, typename TComparer>
class query_order_by
{
public:
	using value_type = typename TCollection::value_type;

private:
	std::vector<value_type> m_Collection;

public:
	query_order_by(TCollection && collection, TComparer && comparer)
		: m_Collection(collection.to_vector())
	{
		std::sort(m_Collection.begin(), m_Collection.end(), comparer);
	}
};
