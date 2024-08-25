#ifndef KEYWORDS_RANDOM_H
#define KEYWORDS_RANDOM_H

#include <random>
#include <vector>

namespace Keywords
{
	namespace Random
	{
		inline std::mt19937 generate()
		{
			std::random_device rd {};

			std::seed_seq ss {rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};

			return std::mt19937 {ss};
		}

		inline std::mt19937 mt {generate()};

		inline int get(int min, int max)
		{
			return std::uniform_int_distribution {min, max} (mt);
		}

		template <typename T>
		T getElement(const std::vector<T>& container)
		{
			auto iter {container.begin()};

			std::uniform_int_distribution<> dis {0, static_cast<int>(container.size()) - 1};
			std::advance(iter, dis(mt));

			return *iter;
		}
	}
}

#endif