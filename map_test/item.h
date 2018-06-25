#ifndef _ITEM_H_
#define _ITEM_H_

#include <memory>

namespace map_hash
{
	template <typename Key, typename Value>
	class item
	{
	public:
		item() : next(nullptr)
		{}

		//TODO: std::pair ?
		item(const Key &key, const Value &value) : key_(key), value_(value), next(nullptr)
		{}

		virtual ~item() = default;

		auto get_key() const -> const Key&
		{
			return key_;
		}

			auto get_value() const -> const Value&
		{
			return value_;
		}

			auto set_value(const Value &value) -> void
		{
			value_ = value;
		}

		std::shared_ptr<item> next;

	private:
		Key key_;
		Value value_;
	};

}
#endif

