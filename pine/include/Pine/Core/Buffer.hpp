#pragma once

#include "Pine/Core/Base.hpp"

class Buffer
{
public:
    Buffer() = default;
    Buffer(const void* data, const uint32_t size) : m_Data(data), m_Size(size) {}
    ~Buffer() = default;

    inline uint32_t GetSize() const { return m_Size; }

    static Buffer Copy(void* data, const uint32_t size)
    {
        // TODO: Implement.
        return Buffer();
    }

    void Allocate(const uint32_t size)
    {
        // TODO: Implement.
    }

    void Reserve(const uint32_t size)
    {
        // TODO: Implement.
    }

    void Write(void* data, const uint32_t size, const uint32_t offset)
    {
        // TODO: Implement.
    }

    template <typename T> T& Read(const uint32_t offset)
    {
        return *(T*)((byte*)m_Data + offset);
    }

    byte* ReadBytes(const uint32_t size, const uint32_t offset)
    {
        // TODO: Implement.
        return nullptr;
    }

    template <typename T> T* As() const { return (T*)m_Data; }

    operator bool() const { return m_Data; }

    byte& operator[](const uint32_t index) const
    {
        // TODO: Implement.
    }

    byte operator[](const uint32_t index) const { return (byte*)m_Data[index]; }

private:
    void* m_Data;
    uint32_t m_Size;
};
