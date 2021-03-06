#include "pch.h"

#include "query.h"

__declspec(noinline) std::vector<int> CreateTestVector()
{
	std::vector<int> vec = { 1, 2, 3, 4 };
	return vec;
}

template <typename T>
int compare_to(const T& x, const T& y)
{
	if (x < y)
	{
		return -1;
	}
	else if (x == y)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int main()
{
	auto vec = CreateTestVector();

	const auto v = q(vec).to_vector();

	//auto v =
	//	q(vec)
	//	.where([](auto x) { return x > 1; })
	//	.select([](auto x) { return x * x; })
	//	.to_vector();

	//auto query = q(CreateTestVector())
	//	.where([](auto x){ return x > 1; })
	//	.select([](auto x) { return float(x); })
	//	.to_vector();

	float sum = 0;

	for (auto t : v)
	{
		sum += t;
	}

	std::cout << sum;


}
