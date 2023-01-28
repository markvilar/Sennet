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
        std::scoped_lock lock(mutex);
        return queue.front();
    }

    const T& back()
    {
        std::scoped_lock lock(mutex);
        return queue.back();
    }

    void push_front(const T& t)
    {
        std::scoped_lock lock(mutex);
        queue.push_front(t);
    }

    void push_back(const T& t)
    {
        std::scoped_lock lock(mutex);
        queue.push_back(t);
    }

    bool empty()
    {
        std::scoped_lock lock(mutex);
        return queue.empty();
    }

    uint64_t count()
    {
        std::scoped_lock lock(mutex);
        return queue.count();
    }

    void clear()
    {
        std::scoped_lock lock(mutex);
        queue.clear();
    }

    T pop_front()
    {
        std::scoped_lock lock(mutex);
        auto t = std::move(queue.front());
        queue.pop_front();
        return t;
    }

    T pop_back()
    {
        std::scoped_lock lock(mutex);
        auto t = std::move(queue.back());
        queue.pop_back();
        return t;
    }

    typename std::deque<T>::iterator begin()
    {
        std::scoped_lock lock(mutex);
        return queue.begin();
    }

    typename std::deque<T>::iterator end()
    {
        std::scoped_lock lock(mutex);
        return queue.end();
    }

    typename std::deque<T>::const_iterator begin() const
    {
        std::scoped_lock lock(mutex);
        return queue.cbegin();
    }

    typename std::deque<T>::const_iterator end() const
    {
        std::scoped_lock lock(mutex);
        return queue.cend();
    }

protected:
    std::mutex mutex;
    std::deque<T> queue;
};

} // namespace pine
