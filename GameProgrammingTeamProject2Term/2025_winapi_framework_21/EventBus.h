#pragma once
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>

class EventBus
{
public:
	using EventCallback = std::function<void()>;
	using ListenerID = size_t;

	static ListenerID AddListener(const std::wstring& _evtName, EventCallback callback);
	static void RemoveListener(const std::wstring& _evtName, ListenerID id);
	static void Invoke(const wstring& _evtName);
	
private:
	static std::unordered_map<std::wstring, std::map<ListenerID, EventCallback>> m_listeners;
	static ListenerID m_nextID;
};

