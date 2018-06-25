#ifndef _MAP_HASH_H_
#define _MAP_HASH_H_

#include "item_list.h"

#define HASH_ARRAY_SIZE 2048

namespace map_hash
{
	template <typename Key, typename Value>
	class map_hash
	{
	public:
		explicit map_hash(size_t size = HASH_ARRAY_SIZE)
			: hash_array(new item_list<Key, Value>[size]),
			size_(size)
		{}

		virtual ~map_hash() = default;

		inline auto index(const Key &key) -> const size_t
		{
			return hash(key) % size_;
		}

			auto insert(const Key &key, const Value &value) -> void
		{
			hash_array[index(key)].insert(key, value);
		}

		auto erase(const Key &key) -> void
		{
			hash_array[index(key)].erase(key);
		}

		auto find(const Key &key, Value &value) -> bool
		{
			return hash_array[index(key)].find(key, value);
		}

		//TODO: rehashing
		//TODO: iterator forward

		map_hash(const map_hash&) = delete;
		map_hash(map_hash&&) = delete;
		map_hash& operator=(const map_hash&) = delete;
		map_hash& operator=(map_hash&&) = delete;

	private:
		const size_t size_;
		std::unique_ptr<item_list<Key, Value>[]> hash_array;
		std::hash<Key> hash;
	};
}
#endif
