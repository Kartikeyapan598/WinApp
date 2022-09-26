#pragma once

#include "Windows.h"

#include <Windows.h>

namespace Core
{
	class WindowContainer
	{
	private :
		WindowContainer();
		WindowContainer(const WindowContainer& other) = delete;
		WindowContainer(const WindowContainer&& other) = delete;
	public:

		void insert(Window* window, void* hwnd);
		size_t size() { return m_handleMap.size(); }
		void erase(void* handle);
	
	public :
		void Broadcast();
		static WindowContainer* GetContainer();
		Window* GetWindowFromHandle(void* handle);
		HINSTANCE GetHinstance() { return m_hInstance; }
		std::string GetWindowClassTitle() { return m_windowClass; }
		LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		std::unordered_map<void*, Window*>& GetWindowVec() { return m_handleMap; }
		std::unordered_map<void*, Window*>::iterator begin() { return m_handleMap.begin(); }
		std::unordered_map<void*, Window*>::iterator end() { return m_handleMap.end(); }
	private :
		HINSTANCE m_hInstance;
		std::string m_windowClass;
		static inline WindowContainer* m_container;
		static inline std::unordered_map<void*, Window*> m_handleMap;
	};
}