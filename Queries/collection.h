#pragma once

template <typename TCollection>
class query_collection
{
private:
	TCollection m_Collection;

public:
	query_collection(TCollection && collection)
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

	decltype(auto) to_vector()
	{
		if constexpr (std::is_reference_v<TCollection>)
		{
			return (m_Collection);
		}
		else
		{
			return std::move(m_Collection);
		}
	}
};
