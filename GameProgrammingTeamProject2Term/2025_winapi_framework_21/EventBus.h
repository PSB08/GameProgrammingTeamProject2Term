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
	//unordered_map은 해시 기반이라 탐색, 삽입, 삭제가 빠르기에 이벤트에 적합하다 생각하여 개발
	//map은 레드블랙 트리 기반이라 매번 비교하며 트리를 타기에 점점 느려짐 정렬도 필요 없어서 정렬 기반인 map X 
	static ListenerID m_nextID;
};

