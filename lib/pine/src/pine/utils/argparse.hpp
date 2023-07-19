#pragma once

#include <string>
#include <string_view>

#include <argparse/argparse.hpp>

namespace pine {

// TODO: Look into compile time string analysis for argument names
constexpr bool is_optional(const std::string_view view) {
    return view.at(0) == '-';
}

class ArgumentParser {
public:
    explicit ArgumentParser(const std::string& description) noexcept;

    // Add positional argument
    template <char Format, typename T>
    void add_positional(const std::string_view name,
        const std::string_view help) {
        impl.add_argument(name).help(std::string(help)).scan<Format, T>();
    }

    // Add positional argument with default value
    template <typename T>
    void add_positional(const std::string_view name,
        const T& default_value,
        const std::string_view help) {
        impl.add_argument(name)
            .help(std::string(help))
            .default_value(default_value);
    }

    // Add optional flag
    template <typename T>
    void add_flag(const std::string_view name,
        const T& default_value,
        const T& implicit_value,
        const std::string_view help = "") {
        impl.add_argument(name)
            .help(std::string(help))
            .default_value(default_value)
            .implicit_value(implicit_value);
    }

    bool parse_arguments(const int count, char** arguments);

    template <typename T>
    T get(const std::string_view name) const {
        return impl.get<T>(name);
    }

private:
    argparse::ArgumentParser impl;
};

} // namespace pine
