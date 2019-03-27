#pragma once
#ifndef EVENT_H
#define EVENT_H

#include "headers.hpp"

template <typename ... Args>
class Event
{
private:
	using Slot = std::function<void(Args ...)>;
	using SlotList = std::unordered_map<uint16_t, Slot>;

	SlotList slots_;

public:
	Event() {}
	Event(const Event &) = delete;
	Event(Event &&) = default;
	Event & operator=(const Event &) = delete;
	Event & operator= (Event &&) = default;

	void notify_all(Args... args) {
		for (auto& f : slots_)
			f.second(args...);
	}

	void notify_by_ID(uint16_t id, Args... args) {
		if(slots_.find(id) != slots_.end())
			slots_[id](args...);
	}

	void connect(uint16_t id, Slot slot) {
		slots_[id] = slot;
	}

	void disconnect(uint16_t id) {
		slots_.erase(id);
	}

};

#endif // !EVENT_H


