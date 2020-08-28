
#ifndef CIRCULAR_QUEUE_CPP
#define CIRCULAR_QUEUE_CPP

#include "Core/CircularQueue.h"
#include "Log.h"

template <typename T>
CircularQueue<T>::CircularQueue() :
	m_nBufferSize (0),
	m_nCount (0),
	m_nStartIndex (0),
	m_nEndIndex (0),
	m_Buffer (0)
{
}

template <typename T>
CircularQueue<T>::~CircularQueue()
{
	ClearAll(true);
}
template <typename T>
void CircularQueue<T>::Reserve(SizeT size)
{
	ASSERT(size < (SizeT)(-50), "Size mightve been negative");

	if (m_nBufferSize >= size) { /*ignore the reserve command */ return; }

	T* newBuffer = (T*)malloc (sizeof(T) * size);

	if (!m_nBufferSize)
	{
		int index = m_nStartIndex;
		for (SizeT i = 0; i < m_nCount; i++)
		{
			new(newBuffer+i) T(std::move(m_Buffer[index]));
			m_Buffer[index].~T();
			index = (index + 1) % m_nBufferSize;
		}
	}

	if (m_Buffer)
	{
		free(m_Buffer);
	}
	m_Buffer = newBuffer;
	m_nBufferSize = size;
	m_nStartIndex = 0; 
	m_nEndIndex = m_nCount;
}

template <typename T>
void CircularQueue<T>::Push(const T& element)
{
	if (m_nCount >= m_nBufferSize) { ASSERT(false, "Not enough space in queue to insert new element"); return; } //No space to allocate;

	ASSERT(m_nEndIndex >= 0 && m_nEndIndex < m_nBufferSize, "Index out of bounds");
	
	new (m_Buffer + m_nEndIndex) T(std::move(element));

	m_nCount++;
	m_nEndIndex++;
	if (m_nEndIndex >= m_nBufferSize)
	{
		m_nEndIndex %= m_nBufferSize;
	}
}

template <typename T>
T* CircularQueue<T>::Push()
{
	if (m_nCount >= m_nBufferSize) { ASSERT(false, "Not enough space in queue to insert new element"); return nullptr; } //No space to allocate;

	ASSERT(m_nEndIndex >= 0 && m_nEndIndex < m_nBufferSize, "Index out of bounds");
	T* newElement = new (m_Buffer + m_nEndIndex) T();

	m_nCount++;
	m_nEndIndex++;
	if (m_nEndIndex >= m_nBufferSize)
	{
		m_nEndIndex %= m_nBufferSize;
	}
	return newElement;
}


template <typename T>
void CircularQueue<T>::Pop()
{
	if (m_nCount <= 0) { LOG_WARN("Tried to pop from an empty queue"); return; }
	
	T* poppedElement = (m_Buffer + m_nStartIndex);
	m_nCount--;
	m_nStartIndex++;
	if (m_nStartIndex >= m_nBufferSize)
	{
		m_nStartIndex %= m_nBufferSize;
	}

	poppedElement->~T();
}

template <typename T>
T* CircularQueue<T>::GetAtPosition(SizeT queuePosition)
{
	if (queuePosition >= m_nCount)
	{
		LOG_WARN ("Index out of queue range");
		return nullptr;
	}
	int nIndex = (m_nStartIndex + queuePosition) % m_nBufferSize;
	return m_Buffer + nIndex;
}
template <typename T>
const T* CircularQueue<T>::GetAtPosition(SizeT queuePosition) const
{
	if (queuePosition >= m_nCount)
	{
		LOG_WARN("Index out of queue range");
		return nullptr;
	}
	int nIndex = (m_nStartIndex + queuePosition) % m_nBufferSize;
	return m_Buffer + nIndex;
}

template <typename T>
void CircularQueue<T>::ClearAll(bool bDeallocateBuffer /*= false*/)
{
	int index = m_nStartIndex;
	for (SizeT i = 0; i < m_nCount; i++)
	{
		m_Buffer[i].~T();
		index = (index + 1) % m_nBufferSize;
	}

	if (m_Buffer && bDeallocateBuffer)
	{
		free(m_Buffer);
		m_Buffer = nullptr;
		m_nBufferSize = 0;
	}

	m_nCount = 0;
	m_nStartIndex = 0;
	m_nEndIndex = 0;
}
#endif CIRCULAR_QUEUE_CPP