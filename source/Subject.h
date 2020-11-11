#pragma once
#include <vector>

namespace Elite
{
	class Observer;
	
	class Subject
	{
	public:
		virtual void Notify() = 0;
		virtual void AddObserver(Observer* obs);
		virtual void RemoveObserver(Observer* obs);

	protected:
		std::vector<Observer*> m_Observers;
	};
}

