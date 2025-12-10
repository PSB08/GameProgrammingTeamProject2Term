#include "pch.h"
#include "EventBus.h"

std::unordered_map<std::wstring, std::map<EventBus::ListenerID, EventBus::EventCallback>> EventBus::m_listeners;
EventBus::ListenerID EventBus::m_nextID = 1;

EventBus::ListenerID EventBus::AddListener(const std::wstring& _evtName, EventCallback callback)
{
    ListenerID id = m_nextID++;  //Id 생성, 저장하기 
    m_listeners[_evtName][id] = std::move(callback);
    return id;  //반환하기
}

void EventBus::RemoveListener(const std::wstring& _evtName, ListenerID id)
{
    auto it = m_listeners.find(_evtName);  //이벤트 이름 찾기
    if (it != m_listeners.end())
    {
        it->second.erase(id);  //삭제
        if (it->second.empty())
            m_listeners.erase(it);  //만약 리스너가 없으면 이벤트 자체를 제거 
    }
    m_listeners.clear();
}

void EventBus::Invoke(const wstring& _evtName)
{
    auto it = m_listeners.find(_evtName);  //찾기
    if (it == m_listeners.end()) return;

    for (auto& [id, callback] : it->second)
        callback();  //다 실행하기
}
