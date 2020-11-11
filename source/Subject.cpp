#include "pch.h"
#include "Subject.h"

void Elite::Subject::AddObserver(Observer* obs)
{
	if (std::find(m_Observers.begin(), m_Observers.end(), obs) == m_Observers.end())
		m_Observers.push_back(obs);
}

void Elite::Subject::RemoveObserver(Observer* obs)
{
	auto iter = std::find(m_Observers.begin(), m_Observers.end(), obs);
	if (iter != m_Observers.end()) m_Observers.erase(iter);
}
