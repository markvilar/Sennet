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
        std::scoped_lock lock(m_Mutex);
        return m_Deque.front();
    }

    const T& back()
    {
        std::scoped_lock lock(m_Mutex);
        return m_Deque.back();
    }

    void push_front(const T& t)
    {
        std::scoped_lock lock(m_Mutex);
        m_Deque.push_front(t);
    }

    void push_back(const T& t)
    {
        std::scoped_lock lock(m_Mutex);
        m_Deque.push_back(t);
    }

    bool empty()
    {
        std::scoped_lock lock(m_Mutex);
        return m_Deque.empty();
    }

    uint64_t count()
    {
        std::scoped_lock lock(m_Mutex);
        return m_Deque.count();
    }

    void clear()
    {
        std::scoped_lock lock(m_Mutex);
        m_Deque.clear();
    }

    T pop_front()
    {
        std::scoped_lock lock(m_Mutex);
        auto t = std::move(m_Deque.front());
        m_Deque.pop_front();
        return t;
    }

    T pop_back()
    {
        std::scoped_lock lock(m_Mutex);
        auto t = std::move(m_Deque.back());
        m_Deque.pop_back();
        return t;
    }

    typename std::deque<T>::iterator begin()
    {
        std::scoped_lock lock(m_Mutex);
        return m_Deque.begin();
    }

    typename std::deque<T>::iterator end()
    {
        std::scoped_lock lock(m_Mutex);
        return m_Deque.end();
    }

    typename std::deque<T>::const_iterator begin() const
    {
        std::scoped_lock lock(m_Mutex);
        return m_Deque.cbegin();
    }

    typename std::deque<T>::const_iterator end() const
    {
        std::scoped_lock lock(m_Mutex);
        return m_Deque.cend();
    }

protected:
    std::mutex m_Mutex;
    std::deque<T> m_Deque;
};

} // namespace pine
