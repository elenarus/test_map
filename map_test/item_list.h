#ifndef _ITEM_LIST_H_
#define _ITEM_LIST_H_

#include <assert.h>

#include "item.h"
#include "atomic_lock.h"

namespace map_hash
{
	template <typename Key, typename Value>
	class item_list
	{
	public:
		item_list() : head(nullptr)
		{}

		virtual ~item_list() = default;

		auto insert(const Key &key, const Value &value) -> void
		{
			lock.lock();

			std::shared_ptr<item<Key, Value>> prev(nullptr);
			std::shared_ptr<item<Key, Value>> current(head);

			while (current != nullptr && current->get_key() != key)
			{
				prev = current;
				current = current->next;
			}

			if (current == nullptr)
			{
				//insert
				//TODO: from memory pool
				current.reset(new item<Key, Value>(key, value));
				if (head == nullptr)
				{
					head = current;
				}
				else
				{
					assert(prev != nullptr);
					assert(prev->next == nullptr);
					prev->next = current;
				}
			}
			else
			{
				//update
				current->set_value(value);
			}

			lock.unlock();
		}

		auto erase(const Key &key) -> void
		{
			lock.lock();

			std::shared_ptr<item<Key, Value>> prev(nullptr);
			std::shared_ptr<item<Key, Value>> current(head);

			while (current != nullptr && current->get_key() != key)
			{
				prev = current;
				current = current->next;
			}

			if (current != nullptr)
			{
				if (head == current)
				{
					head = current->next;
				}
				else
				{
					prev->next = current->next;
				}
				//TODO: to memory pool
				//current = nullptr;
			}

			lock.unlock();
		}

		auto find(const Key &key, Value &value) -> bool
		{
			lock.lock();

			std::shared_ptr<item<Key, Value>> current(head);

			while (current != nullptr)
			{
				if (current->get_key() != key)
				{
					current = current->next;
					continue;
				}
				value = current->get_value();

				lock.unlock();
				return true;
			}

			lock.unlock();
			return false;
		}

	private:
		std::shared_ptr<item<Key, Value>> head;
		atomic_lock lock;
	};
}
#endif
