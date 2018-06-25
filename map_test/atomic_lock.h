#ifndef _ATOMIC_LOCK_H_
#define _ATOMIC_LOCK_H_

#include <atomic>
#include <thread>

namespace map_hash
{
	//TODO: lock_guard
	class atomic_lock
	{
	public:
		atomic_lock()
		{
			flag.clear();
		}

		auto lock() -> void
		{
			for (;;)
			{
				if (!flag.test_and_set(std::memory_order_acquire)) //std::memory_order_seq_cst
					break;
				std::this_thread::yield();
			}
		}

		auto unlock() -> void
		{
			flag.clear(std::memory_order_release); //std::memory_order_seq_cst;
		}

		virtual ~atomic_lock() = default;

	private:
		std::atomic_flag flag;
	};
}
#endif