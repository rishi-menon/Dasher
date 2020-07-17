#pragma once

template <typename T>
class CircularQueue
{
	using SizeT = unsigned int;

public:
	CircularQueue();

	inline T* Buffer() { return m_Buffer; }
	inline const T* Buffer() const { return m_Buffer; }

	inline SizeT Begin() const { return m_nStartIndex; }
	inline SizeT End()	  const { return m_nEndIndex; }

	inline SizeT Count() const { return m_nCount; }
	inline SizeT Size() const { return m_nBufferSize; }

	T* GetAt(SizeT queuePosition);
	const T* GetAt(SizeT queuePosition) const;

	void Reserve(SizeT size);

	void Push(const T& element);
	T* Push();
	
	T* Pop(); 

	void Print() const;

private:
	SizeT m_nBufferSize, m_nCount;
	SizeT m_nStartIndex, m_nEndIndex;	//range is inclusive
	T* m_Buffer;
};

#ifndef CIRCULAR_QUEUE_CPP
#include "Core/CircularQueue.cpp"
#endif