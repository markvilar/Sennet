#pragma once

namespace Pine::TCP
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

        const auto i = message.Body.size();
        message.Body.resize(message.Body.size() + sizeof(D));
        std::memcpy(message.Body.data() + i, &data, sizeof(D));
        message.Header.Size = message.Size();

        return message;
    }

    template <typename D>
    friend Message<T>& operator<<(
        Message<T>& message, const std::vector<D>& data)
    {
        static_assert(std::is_standard_layout<D>::value,
            "Data is too complex to be pushed into vector.");

        const auto vectorSize = data.size();
        const auto i = message.Body.size();
        message.Body.resize(message.Body.size() + vectorSize);
        std::memcpy(message.Body.data() + i, data.data(), vectorSize);
        message.Header.Size = message.Size();

        return message;
    }

    template <typename D>
    friend Message<T>& operator>>(Message<T>& message, D& data)
    {
        static_assert(std::is_standard_layout<D>::value,
            "Data is too complex to be pushed into vector.");

        const auto i = message.Body.size() - sizeof(D);
        std::memcpy(&data, message.Body.data() + i, sizeof(D));
        message.Body.resize(i);
        message.Header.Size = message.Size();

        return message;
    }

    template <typename D>
    friend Message<T>& operator>>(Message<T>& message, std::vector<D>& data)
    {
        static_assert(std::is_standard_layout<D>::value,
            "Data is too complex to be pushed into vector.");

        const auto vectorSize = data.size();
        const auto i = message.Body.size() - vectorSize;
        std::memcpy(data.data(), message.Body.data() + i, vectorSize);
        message.Body.resize(i);
        message.Header.Size = message.Size();

        return message;
    }
};

template <typename T> class Connection;

template <typename T> struct OwnedMessage
{
    std::shared_ptr<Connection<T>> Remote = nullptr;
    Message<T> Msg;

    friend std::ostream& operator<<(
        std::ostream& os, const OwnedMessage<T>& message)
    {
        os << message.Msg;
        return os;
    }
};

} // namespace Pine::TCP
