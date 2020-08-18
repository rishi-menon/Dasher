#pragma once
#include "Log.h"

template <typename T>
class CircularQueue
{
	using SizeT = unsigned int;

public:
	CircularQueue();
	~CircularQueue();

	inline T* Buffer() { return m_Buffer; }
	inline const T* Buffer() const { return m_Buffer; }

	inline SizeT Begin() const { return m_nStartIndex; }
	inline SizeT End()	  const { return m_nEndIndex; }

	inline SizeT Count() const { return m_nCount; }
	inline SizeT Size() const { return m_nBufferSize; }

	inline T* GetAtRaw(SizeT rawBufferIndex) {
		
#if _DEBUG
		SizeT i = rawBufferIndex;
		if (m_nStartIndex <= m_nEndIndex)
		{
			ASSERT(i >= m_nStartIndex && i <= m_nEndIndex, "Out of bounds");
		}
		else {
			ASSERT((i >= m_nStartIndex && i < m_nBufferSize) || (i <= m_nEndIndex), "Out of bounds");
		}
#endif
		return (m_Buffer+rawBufferIndex); 
	
	}

	inline const T* GetAtRaw(SizeT rawBufferIndex) const { ASSERT(rawBufferIndex < m_nCount&& rawBufferIndex < m_nBufferSize, "Out of bounds"); return (m_Buffer + rawBufferIndex); }
	
	T* GetAtPosition(SizeT queuePosition);
	const T* GetAtPosition(SizeT queuePosition) const;

	void Reserve(SizeT size);

	void Push(const T& element);
	T* Push();
	
	void Pop(); 
	void ClearAll(bool bDeallocateBuffer = false);

private:
	SizeT m_nBufferSize, m_nCount;
	SizeT m_nStartIndex, m_nEndIndex;	//range is inclusive
	T* m_Buffer;
};

#ifndef CIRCULAR_QUEUE_CPP
#include "Core/CircularQueue.cpp"
#endif