#pragma once
#include <vector>

namespace bs
{
	template<typename T>
	class circular_buffer
	{
	public:
		circular_buffer(size_t _size) : m_head(0, m_data)
		{
			m_data.resize(_size + 1);
		}

		struct iterator
		{
		public:
			iterator(int _index, std::vector<T>& _parent) : m_index(_index), m_parent(_parent)
			{}
			
			iterator operator++()
			{
				++m_index;
				m_index = m_index % (int)m_parent.size();
				return *this;
			}

			iterator operator--()
			{
				--m_index;
				m_index = m_index % (int)m_parent.size();
				return *this;
			}
			
			iterator operator++(int)
			{
				++m_index;
				m_index = m_index % (int)m_parent.size();
				return *this;
			}

			iterator operator--(int)
			{
				--m_index;
				m_index = m_index % (int)m_parent.size();
				return *this;
			}

			iterator operator+(int rhs)
			{
				return iterator((m_index + rhs) % (int)m_parent.size(), m_parent);
			}

			iterator operator-(int rhs)
			{
				return iterator((m_index + (int)m_parent.size() - rhs) % (int)m_parent.size(), m_parent);
			}
			
			bool operator==(const iterator& rhs) const
			{
				return m_index == rhs.m_index && (&m_parent) == (&rhs.m_parent);
			}
			
			bool operator!=(const iterator& rhs) const
			{
				return m_index != rhs.m_index || (&m_parent) != (&rhs.m_parent);
			}
			
			T& operator*()
			{
				return m_parent[m_index];
			}

			const T& operator*() const
			{
				return m_parent[m_index];
			}

		private:
			int m_index;
			std::vector<T>& m_parent;
		};

		void push_back(T _item)
		{
			*(m_head++) = _item;
		}

		iterator begin()
		{
			return m_head + 2;
		}

		iterator end()
		{
			return m_head + 1;
		}

		iterator back()
		{
			return m_head;
		}

		size_t capacity() const
		{
			return m_data.size() - 1;
		}

	private:
		iterator m_head;
		std::vector<T> m_data;
	};
}