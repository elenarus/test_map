#include <chrono>
#include <iostream>
#include <vector>
#include <map>
#include <mutex>

#include "map_hash.h"

#define ELEMENT_COUNT 500000
#define THREAD_COUNT 20


auto map_hash_thread(map_hash::map_hash<int, int> &collection) -> void
{
	int value;
	for (auto i = 0; i < ELEMENT_COUNT; i++)
	{
		auto rnd = rand();
		if (collection.find(rnd, value))
			collection.erase(rnd);
		else
			collection.insert(rnd, rnd);
		std::this_thread::yield();
	}
}

auto test_map_hash() -> void
{
	map_hash::map_hash<int, int> map_h;
	std::vector<std::thread> threads(THREAD_COUNT);

	auto begin = std::chrono::high_resolution_clock::now();
	
	for (auto &t : threads)
	{
		t = std::move(std::thread(map_hash_thread, std::ref(map_h)));
	}
	for (auto &t : threads)
	{
		t.join();
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "map_hash \t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}

auto map_thread(std::map<int, int> &collection, std::mutex &mtx) -> void
{
	for (auto i = 0; i < ELEMENT_COUNT; i++)
	{
		{
			std::lock_guard<std::mutex> lock(mtx);
			auto rnd = rand();
			auto pos = collection.find(rnd);
			if (pos != collection.end())
				collection.erase(rnd);
			else
				collection.emplace(rnd, rnd);
		}
		std::this_thread::yield();
	}
}

auto test_map() -> void
{
	std::map<int, int> std_map;
	std::mutex mtx;
	std::vector<std::thread> threads(THREAD_COUNT);

	auto begin = std::chrono::high_resolution_clock::now();

	for (auto &t : threads)
	{
		t = std::move(std::thread(map_thread, std::ref(std_map), std::ref(mtx)));
	}
	for (auto &t : threads)
	{
		t.join();
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "std::map \t";
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;
}

int main()
{
	test_map_hash();
	test_map();
	system("pause");
	return 0;
}

