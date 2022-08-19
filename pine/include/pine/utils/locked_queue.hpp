#pragma once

#include <deque>
#include <mutex>

namespace pine
{

template <typename T>
class LockedQueue
{
public:
    LockedQueue() = default;

    LockedQueue(const LockedQueue<T>&) = delete;
    LockedQueue(LockedQueue<T>&&) = delete;

    ~LockedQueue() = default;

    LockedQueue& operator=(const LockedQueue<T>&) = delete;
    LockedQueue& operator=(LockedQueue<T>&&) = delete;

    const T& front()
    {
        std::scoped_lock lock(m_mutex);
        return m_deque.front();
    }

    const T& back()
    {
        std::scoped_lock lock(m_mutex);
        return m_deque.back();
    }

    void push_front(const T& t)
    {
        std::scoped_lock lock(m_mutex);
        m_deque.push_front(t);
    }

    void push_back(const T& t)
    {
        std::scoped_lock lock(m_mutex);
        m_deque.push_back(t);
    }

    bool empty()
    {
        std::scoped_lock lock(m_mutex);
        return m_deque.empty();
    }

    uint64_t count()
    {
        std::scoped_lock lock(m_mutex);
        return m_deque.count();
    }

    void clear()
    {
        std::scoped_lock lock(m_mutex);
        m_deque.clear();
    }

    T pop_front()
    {
        std::scoped_lock lock(m_mutex);
        auto t = std::move(m_deque.front());
        m_deque.pop_front();
        return t;
    }

    T pop_back()
    {
        std::scoped_lock lock(m_mutex);
        auto t = std::move(m_deque.back());
        m_deque.pop_back();
        return t;
    }

    typename std::deque<T>::iterator begin()
    {
        std::scoped_lock lock(m_mutex);
        return m_deque.begin();
    }

    typename std::deque<T>::iterator end()
    {
        std::scoped_lock lock(m_mutex);
        return m_deque.end();
    }

    typename std::deque<T>::const_iterator begin() const
    {
        std::scoped_lock lock(m_mutex);
        return m_deque.cbegin();
    }

    typename std::deque<T>::const_iterator end() const
    {
        std::scoped_lock lock(m_mutex);
        return m_deque.cend();
    }

protected:
    std::mutex m_mutex;
    std::deque<T> m_deque;
};

} // namespace pine
