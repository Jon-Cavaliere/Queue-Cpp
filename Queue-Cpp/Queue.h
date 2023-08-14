#pragma once
#include <cmath>
#include <stdint.h>
#include <iostream>
#include <stdexcept>

/*
	Jonathan Cavaliere, 2023

	This is a class-template queue implementation that can grow dynamically to accomadate additional pushed objects

	It will reuse already allocated memory by wrapping back around when objects have been pushed and popped UNTIL
	it runs out of allocated slots and must grow

	Free to use for any purpose!
*/
namespace JC
{
	// Declaration
	template <typename T>
	class Queue
	{
	public:
		Queue<T>(uint32_t initial_size, float growth_factor = 2.0f);
		~Queue();

		// Pushes obj onto the back of the queue
		void push(T obj);
		// Returns a copy of the object at the front of the queue and removes it
		T pop();
		// Returns a reference to object at the front of the queue (Without removing it)
		T& peek();	
		// Resizes allocated memory to new_size # of elements, and copies data over
		void resize(uint32_t new_size);
		// Dumps entire queue to console from front to rear
		void print();
		// Returns true if the queue has another object
		bool hasNext();
		// Clears all of the elements from the queue
		void clear();
	private:
		// Index of the front of the queue
		uint32_t m_Front;
		// Index of the rear of the queue
		uint32_t m_Rear;
		// Size of the heap allocated storage array
		uint32_t m_Size;
		// Number of elements currently in queue
		uint32_t m_Count;
		// Size Multplier for when the queue grows
		uint32_t m_GrowthFactor;

		// Heap allocated dynamic array 
		T* p_Arr;
	};

	// Definitions 
	template <typename T>
	inline Queue<T>::Queue(uint32_t initial_size, float growth_factor)
		: m_Front(0), m_Rear(0), m_Size(initial_size), m_Count(0), p_Arr(nullptr), m_GrowthFactor(growth_factor)
	{
		// Allocate array of T objects of size initial_size
		p_Arr = new T[m_Size];
	}

	template<typename T>
	inline Queue<T>::~Queue()
	{
		delete[] p_Arr;
	}

	template<typename T>
	inline void Queue<T>::push(T obj)
	{
		if (m_Count == 0)
		{
			p_Arr[0] = obj;
		}
		// Check if it's full
		else if (m_Count < m_Size)
		{
			// Shift the rear index to the next available slot
			if (m_Rear < m_Size-1 && m_Rear >= m_Front)
				m_Rear++;
			// If we've reached the end of the allocated memory wrap around
			else
			{
				// If the front index isnt 0, move rear index into the first available slot
				if (m_Front > 0)
				{
					if (m_Rear > m_Front)
						m_Rear = 0;
					else
						m_Rear++;
				}
				else
				{
					// Queue has grown too large and needs to resize
					uint32_t newSize = lrint(m_Size * m_GrowthFactor);
					resize(newSize);
				}
			}
			p_Arr[m_Rear] = obj;
		}
		else
		{
			// Queue has grown too large and needs to resize
			uint32_t newSize = lrint(m_Size * m_GrowthFactor);
			resize(newSize);
			p_Arr[m_Rear] = obj;
		}
		
		m_Count++;
	}

	template <typename T>
	inline T Queue<T>::pop()
	{
		// Check if empty
		if (m_Count > 0)
		{
			// Check if the rear hasn't wrapped around to lower values
			if (m_Front < m_Rear)
			{
				// If it hasn't, easy peasy. Just decrease count, return a copy of the element, 
				// and move the front index one space toward rear
				m_Count--;
				uint32_t temp_front = m_Front++;
				if (m_Count == 0)
					clear();
				return p_Arr[temp_front];
			}
			// If the rear has wrapped around to the front
			else
			{
				// More complex, but still
				// Decrease element count
				m_Count--;

				
				// Check if the front is less than the size of allocated memory
				// if it is, increase front index towards the end
				if (m_Front < m_Size)
				{
					uint32_t temp_front = m_Front++;
					if (m_Count == 0)
						clear();
					return p_Arr[temp_front];
				}
				// if it isnt, 
				else
				{
					// The front wraps around to the first index
					m_Front = 0;
					if (m_Count == 0)
						clear();
					return p_Arr[0];
				}
			}
		}
		throw std::underflow_error("Queue<T>::pop(): Error! Queue is empty");
	}

	template<typename T>
	inline T& Queue<T>::peek()
	{
		if(m_Count > 0)
			return p_Arr[m_Front];
		throw std::underflow_error("Queue<T>::peek(): Error! Queue is empty");
	}

	template<typename T>
	inline void Queue<T>::resize(uint32_t new_size)
	{
		T* newArr = new T[new_size];
		// Loop from front to end
		uint32_t count = 0;
		if (m_Front < m_Rear)
		{
			for (uint32_t i = m_Front; i <= m_Rear; i++)
			{
				newArr[count++] = p_Arr[i];
			}
			m_Rear = count;
		}
		else
		{
			for (uint32_t i = m_Front; i < m_Size; i++)
			{
				newArr[count++] = p_Arr[i];
			}
			for (uint32_t i = 0; i <= m_Rear; i++)
			{
				newArr[count++] = p_Arr[i];
			}
			m_Rear = count;
		}
		m_Front = 0;
		delete[] p_Arr;
		p_Arr = new T[new_size];
		m_Size = new_size;
		// Copy new array into p_Arr
		for (uint32_t i = 0; i < m_Size; i++)
		{
			p_Arr[i] = newArr[i];
		}
		delete[] newArr;
		// Growth complete
	}

	template<typename T>
	inline void Queue<T>::print()
	{
		if (m_Count > 0)
		{
			if (m_Front < m_Rear)
			{
				for (int i = m_Front; i <= m_Rear; i++)
				{
					std::cout << p_Arr[i] << "\n";
				}
			}
			else if (m_Front > m_Rear)
			{
				for (int i = m_Front; i < m_Size; i++)
				{
					std::cout << p_Arr[i] << "\n";
				}
				for (int i = 0; i <= m_Rear; i++)
				{
					std::cout << p_Arr[i] << "\n";
				}
			}
			else if (m_Count == 1)
			{
				std::cout << p_Arr[m_Front] << "\n";
			}
		}
		else
			std::cout << "Queue is empty\n";
	}

	template<typename T>
	inline bool Queue<T>::hasNext()
	{
		return m_Count > 0 ? true : false;
	}

	template<typename T>
	inline void Queue<T>::clear()
	{
		// Kind of clears it. It really just allows existing objects to be overwritten
		m_Count = m_Front = m_Rear = 0;
	}

}