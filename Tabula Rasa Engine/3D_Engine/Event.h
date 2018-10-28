#ifndef __EVENT_H__
#define __EVENT_H__

class Event {

public:
	enum event_type {
		NONE,
		GAMEOBJECT_DESTROYED
	};

public:

	Event() { type = event_type::NONE; }

	Event(event_type type) :type(type) {}

	const event_type GetType() const { return type; }

private:
	event_type type = event_type::NONE;

};

#endif