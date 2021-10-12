#pragma once

namespace Sennet
{

namespace TCP
{

template <typename T> struct MessageHeader
{
    T ID{};
    uint32_t Size = 0;
};

template <typename T> struct Message
{
    MessageHeader<T> Header{};
    std::vector<uint8_t> Body;

    uint64_t Size() const { return Body.size(); }

    friend std::ostream& operator<<(std::ostream& os, const Message<T>& message)

    {
        os << "ID: " << int(message.Header.ID)
           << ", Size: " << message.Header.Size;
        return os;
    }

    template <typename D>
    friend Message<T>& operator<<(Message<T>& message, const D& data)
    {
        static_assert(std::is_standard_layout<D>::value,
            "Data is too complex to be pushed into vector.");
        uint64_t i = message.Body.size();

        // Resize body.
        message.Body.resize(message.Body.size() + sizeof(D));

        // Copy data into vector.
        std::memcpy(message.Body.data() + i, &data, sizeof(D));

        // Update header size.
        message.Header.Size = message.Size();

        return message;
    }

    template <typename D>
    friend Message<T>& operator<<(Message<T>& message,
        const std::vector<D>& data)
    {
        static_assert(std::is_standard_layout<D>::value,
            "Data is too complex to be pushed into vector.");
        uint64_t i = message.Body.size();
        uint64_t vectorSize = data.size();

        // Resize body.
        message.Body.resize(message.Body.size() + vectorSize);

        // Copy data into vector.
        std::memcpy(message.Body.data() + i, data.data(), vectorSize);

        // Update header size.
        message.Header.Size = message.Size();

        return message;
    }

    template <typename D>
    friend Message<T>& operator>>(Message<T>& message, D& data)
    {
        static_assert(std::is_standard_layout<D>::value,
            "Data is too complex to be pushed into vector.");

        uint64_t i = message.Body.size() - sizeof(D);

        // Copy data from vector.
        std::memcpy(&data, message.Body.data() + i, sizeof(D));

        // Resize body.
        message.Body.resize(i);

        // Update header size.
        message.Header.Size = message.Size();

        return message;
    }

    template <typename D>
    friend Message<T>& operator>>(Message<T>& message, std::vector<D>& data)
    {
        static_assert(std::is_standard_layout<D>::value,
            "Data is too complex to be pushed into vector.");
        uint64_t vecSize = data.size();
        uint64_t i = message.Body.size() - vecSize;

        // Copy data from vector.
        std::memcpy(data.data(), message.Body.data() + i, vecSize);

        // Resize body.
        message.Body.resize(i);

        // Update header size.
        message.Header.Size = message.Size();

        return message;
    }
};

template <typename T> class Connection;

template <typename T> struct OwnedMessage
{
    Ref<Connection<T>> Remote = nullptr;
    Message<T> Msg;

    friend std::ostream& operator<<(std::ostream& os,
        const OwnedMessage<T>& message)
    {
        os << message.Msg;
        return os;
    }
};

} // namespace TCP
} // namespace Sennet
