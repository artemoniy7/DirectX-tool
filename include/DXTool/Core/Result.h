#pragma once

#include <string>
#include <utility>
#include <variant>

namespace DXTool
{
    enum class ErrorCode
    {
        Unknown = 0,
        InvalidArgument,
        InitializationFailed,
        WindowCreationFailed,
        GraphicsInitializationFailed,
        UnsupportedFeature,
        ResourceCreationFailed,
        FileNotFound,
        CompilationFailed
    };

    struct Error
    {
        ErrorCode code = ErrorCode::Unknown;
        std::string message;
    };

    template<typename T>
    class Result
    {
    public:
        static Result Success(T value)
        {
            return Result(std::move(value));
        }

        static Result Failure(Error error)
        {
            return Result(std::move(error));
        }

        static Result Failure(ErrorCode code, std::string message)
        {
            return Failure(Error{ code, std::move(message) });
        }

        bool IsOk() const noexcept
        {
            return std::holds_alternative<T>(m_data);
        }

        explicit operator bool() const noexcept
        {
            return IsOk();
        }

        T& Value()
        {
            return std::get<T>(m_data);
        }

        const T& Value() const
        {
            return std::get<T>(m_data);
        }

        T&& TakeValue()
        {
            return std::move(std::get<T>(m_data));
        }

        const Error& GetError() const
        {
            return std::get<Error>(m_data);
        }

    private:
        explicit Result(T value)
            : m_data(std::move(value))
        {
        }

        explicit Result(Error error)
            : m_data(std::move(error))
        {
        }

        std::variant<T, Error> m_data;
    };

    template<>
    class Result<void>
    {
    public:
        static Result Success()
        {
            return Result();
        }

        static Result Failure(Error error)
        {
            return Result(std::move(error));
        }

        static Result Failure(ErrorCode code, std::string message)
        {
            return Failure(Error{ code, std::move(message) });
        }

        bool IsOk() const noexcept
        {
            return !m_error.has_value();
        }

        explicit operator bool() const noexcept
        {
            return IsOk();
        }

        const Error& GetError() const
        {
            return m_error.value();
        }

    private:
        Result() = default;

        explicit Result(Error error)
            : m_error(std::move(error))
        {
        }

        std::optional<Error> m_error;
    };
}
