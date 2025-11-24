#include "pch.h"
#include "EventBus.h"

std::unordered_map<std::wstring, std::map<EventBus::ListenerID, EventBus::EventCallback>> EventBus::m_listeners;
EventBus::ListenerID EventBus::m_nextID = 1;

EventBus::ListenerID EventBus::AddListener(const std::wstring& _evtName, EventCallback callback)
{
    ListenerID id = m_nextID++;
    m_listeners[_evtName][id] = std::move(callback);
    return id;
}

void EventBus::RemoveListener(const std::wstring& _evtName, ListenerID id)
{
    auto it = m_listeners.find(_evtName);
    if (it != m_listeners.end())
    {
        it->second.erase(id);
        if (it->second.empty())
            m_listeners.erase(it);
    }
}

void EventBus::Invoke(const wstring& _evtName)
{
    auto it = m_listeners.find(_evtName);
    if (it == m_listeners.end()) return;

    for (auto& [id, callback] : it->second)
        callback();
}
