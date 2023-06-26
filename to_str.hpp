// file_io
// Created by kiki on 2022/9/25.17:29
#pragma once
#include <algorithm>
#include <any>
#include <array>
#include <bitset>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <syncstream>
#include <tuple>
#include <variant>
#if (defined(__GNUC__) || defined(__clang__)) && __has_include(<cxxabi.h>)
#include <cxxabi.h>
#define DEMANGLE 1
#else
#define DEMANGLE 0
#endif

using std::array;
using std::bitset;
using std::ios;
using std::optional, std::variant, std::any, std::any_cast, std::bad_any_cast;
using std::ostream, std::ostringstream, std::cout, std::istream, std::istreambuf_iterator, std::osyncstream;
using std::ostream_iterator;
using std::put_time;
using std::string, std::pair, std::byte;
using std::tuple_size, std::tuple_size_v;
using std::unique_ptr;
using std::chrono::system_clock, std::chrono::time_point, std::chrono::duration, std::chrono::duration_cast, std::chrono::milliseconds;

template <typename T>
concept NativePointer = std::is_pointer_v<T>;

template <typename T>
concept PointerLikeClass = requires(T t) {
                               t.operator->();
                               t.operator*();
                           };

template <typename T>
concept Pointer = NativePointer<T> || PointerLikeClass<T>;

template <typename T>
concept NullablePtr =
    NativePointer<T> || (PointerLikeClass<T> && requires(T t) { { t.operator bool() } -> std::same_as<bool>; });

template <typename T>
concept RangedContainer = requires(T a) {
                              a.begin() != a.end(); // 只要语法正确即可，不会计算结果
                          };

template <typename T>
concept TupleLikeType = requires {
                            tuple_size<T>::value; // 判断类型T是否为pair、array、tuple之一
                        };

template <typename T, typename U>
concept IsBaseOf = std::is_base_of_v<std::remove_reference_t<T>, U>;

template <typename T, typename OStream>
concept StreamInsertionClass = requires(OStream& os, const T& obj) {
                                   {
                                       os << obj
                                       } -> IsBaseOf<OStream>;
                               };

struct to_str_wrapper
{
    // 对指针的特化
    template <typename OutStream, typename T>
        requires Pointer<T>
    static void to_str(OutStream& os, const T& pointer) // 类型T可能为智能指针，按引用传递
    {
        if constexpr (std::is_same_v<std::decay_t<T>, void*> || std::is_same_v<std::decay_t<T>, volatile void*> || std::is_same_v<std::decay_t<T>, const volatile void*>)
        {
            if (pointer)
            {
                os << "void* ptr stored address " << const_cast<void*>(pointer);
            }
            else
            {
                os << "null void* ptr";
            }
        }
        else
        {
            if constexpr (NullablePtr<T>)
            {
                if (!pointer)
                {
                    os << "null pointer";
                    return;
                }

                os << "pointer to the address " << &(*pointer) << ", value: ";
                to_str(os, *pointer);
            }
            else
            {
                // fixme: 可能对非法的指针解引用
                // os << "smart pointer (maybe wild)";
                os << "pointer to the address " << &(*pointer) << ", value: ";
                to_str(os, *pointer);
            }
        }
    }

    // 对支持流插入运算符的类型的特化
    // T被概念StreamInsertionClass<T, OStream>所限制
    template <typename OStream, StreamInsertionClass<OStream> T>
        requires(!Pointer<T> && !std::invocable<T>)
    static void to_str(OStream& os, const T& val)
    {
        os << val;
    }

    // 对象内含的元素个数超过这个值后，打印元素个数
    static constexpr size_t size_threshold{4};

    // 对元组的特化
    template <typename OutStream, TupleLikeType Tuple>
    static void to_str(OutStream& os, const Tuple& tup) // pair、array、tuple
    {
        if constexpr (tuple_size_v<Tuple> == 0)
        {
            os << "()";
            return; // fake return
        }
        else
        {
            if constexpr (tuple_size_v < Tuple >> size_threshold)
            {
                os << "<tuple with size " << tuple_size_v<Tuple> << "> ";
            }

            os << '(';

            auto dump = [&os](const auto& arg, const auto&... args) // args可以是任意数量的参数，甚至是0个
            {
                to_str(os, arg);
                (..., (os << ' ', to_str(os, args))); // 左折叠；长度为0的参数包允许一元折叠
            };

            std::apply(dump, tup);

            os << ')';
        }
    }

    // 对容器的特化
    template <typename OutStream, RangedContainer Container>
        requires(!StreamInsertionClass<Container, OutStream> && !TupleLikeType<Container>)
    static void to_str(OutStream& os, const Container& container)
    {

        auto iter = begin(container);
        auto end_iter = end(container);

        if (iter == end_iter)
        {
            os << '[' << ']';
            return;
        }

        auto container_size = std::distance(iter, end_iter);
        if (container_size > size_threshold)
        {
            os << "<container with size " << container_size << "> ";
        }

        os << '[';
        to_str(os, *iter++);
        for (; iter != end_iter; ++iter)
        {
            os << ' ';
            to_str(os, *iter);
        }
        os << ']';
    }

    // 对输入流的特化
    template <typename OutStream, typename Istream>
        requires IsBaseOf<istream, Istream>
    static void to_str(OutStream& os, Istream& input_stream)
    {
        os << "input stream with state ";

        auto state{input_stream.rdstate()};
        ostringstream oss;         // hint: using sting.append
        if (state == ios::goodbit) // ios::goodbit equals to 0
        {
            oss << "good,";
        }
        array<const char*, 3> state_str{"bad,", "fail,", "eof,"};
        array<ios::iostate, 3> state_arr{ios::badbit, ios::failbit, ios::eofbit};
        for (int i{}; i < 3; ++i)
        {
            if (state & state_arr[i])
            {
                oss << state_str[i];
            }
        }
        oss.seekp(-1, ostream::end);
        oss << '\n';

        os << oss.str();

        if (!input_stream.good()) // eofbit	or failbit	or badbit
        {
            return;
        }

        auto istream_size = [](istream& input_stream) -> std::streamoff
        {
            auto current_pos{input_stream.tellg()};
            if (current_pos == -1)
            {
                return -1;
            }
            input_stream.seekg(0, istream::end);
            auto end_pos = input_stream.tellg();

            input_stream.seekg(current_pos);

            return end_pos - current_pos;
        };

        auto count_line_and_get_data = [](istream& input_stream)
        {
            auto stream_beg_pos{input_stream.tellg()};

            size_t line_tally{0};
            string data;

            auto cb = [&data, &line_tally](char ch)
            {
                if (ch == '\n')
                {
                    ++line_tally;
                }
                data.push_back(ch);
            };

            std::for_each(istreambuf_iterator<char>{input_stream}, istreambuf_iterator<char>{}, cb);

            input_stream.seekg(stream_beg_pos);
            // 没有读取EOF字符，流仍是正常状态

            return pair<size_t, string>{line_tally, std::move(data)};
        };

        auto stream_size = istream_size(input_stream);
        auto [line_tally, data] = count_line_and_get_data(input_stream);

        os << "input stream with size of " << stream_size
           << ", line " << line_tally << '\n'
           << data;
    }

    // 对可调用对象的特化
    template <typename OutStream, typename Func, typename... Args>
        requires std::invocable<Func, Args...>
    static void to_str(OutStream& os, Func&& func, Args&&... args)
    {
        to_str(os, std::forward<Func>(func)(std::forward<Args>(args)...));
    }

    // 对optional的重载
    template <typename OutStream, typename T>
    static void to_str(OutStream& os, const optional<T>& opt)
    {
        if (opt)
        {
            to_str(os, opt.value());
        } // value()返回内含对象的引用
        else
        {
            os << "nullopt";
        }
    }

    // 对variant的重载
    template <typename OutStream, typename... Args>
    static void to_str(OutStream& os, const variant<Args...>& var)
    {
        auto dump = [&os](auto&& arg)
        {
            to_str(os, arg);
        };

        std::visit(dump, var);
    }

    // 对any的重载
    template <typename OutStream, typename T>
    static void to_str(OutStream& os, const any& any_obj, T)
    {
        try
        {
            to_str(os, std::any_cast<const T&>(any_obj));
        }
        catch (const std::bad_any_cast& e)
        {
            os << "bad any cast: cannot convert " << any_obj.type().name() << " to " << typeid(T).name();
        }
    }

    // 对时间间隔的重载
    template <typename OutStream, typename T, typename U>
    static void to_str(OutStream& os, duration<T, U> d)
    {
        os << duration_cast<duration<double, std::ratio<1, 1000>>>(d).count() << " ms";
    }

    // 对时间点的重载
    template <typename OutStream, typename T, typename U>
    static void to_str(OutStream& os, time_point<T, U> t)
    {
        time_t tt{system_clock::to_time_t(t)};
        tm* tm_ptr{localtime(&tt)}; // todo: thread safe
        os << put_time(tm_ptr, "%Y/%m/%d %H:%M:%S");
    }

    // 对其它未知类型的重载；如果类型是上面的类型，编译器总是选择上面约束最严格的特化版本、类型最具体的重载版本
    template <typename OutStream, typename T>
    static void to_str(OutStream& os, const T& t)
    {
        const void* addr = std::addressof(t);

        os << get_pretty_type_name<T>() << " object at " << addr;
    }

    template <typename OutStream, typename T>
    static void to_str(OutStream& os, const T&& t)
    {
        os << get_pretty_type_name<T>() << " temporary object";
    }

    template <typename T>
    static string get_pretty_type_name()
    {
        string pretty_type_name;

        const char* name{typeid(T).name()};

#if !DEMANGLE
        pretty_type_name = name;
#else
        unique_ptr<char> true_name{abi::__cxa_demangle(name, nullptr, nullptr, nullptr)};
        if (!true_name)
        {
            return string{"invalid type name"};
        }

        pretty_type_name = string{true_name.get()};
        true_name.reset();
#endif

        using CompleteType = T;
        using Type = std::remove_reference_t<CompleteType>;

        if (std::is_const_v<Type>)
        {
            pretty_type_name += " const";
        } // 若T为引用类型则 is_const_v<T> 始终为 false
        if (std::is_volatile_v<CompleteType>)
        {
            pretty_type_name += " volatile";
        }
        if (std::is_lvalue_reference_v<CompleteType>)
        {
            pretty_type_name += " &";
        }
        if (std::is_rvalue_reference_v<CompleteType>)
        {
            pretty_type_name += " &&";
        }

        return pretty_type_name;
    }
};

template <typename T>
string type_str()
{
    return to_str_wrapper::get_pretty_type_name<T>();
}

template <typename T>
constexpr string type_str(T obj)
{
    return type_str<decltype(obj)>();
}

template <typename... Args>
static string to_str(Args&&... args)
{
    static thread_local ostringstream oss;
    oss.str("");
    to_str_wrapper::to_str(oss, std::forward<Args>(args)...);
    return oss.str();
}

template <typename... Args>
static void console_print(Args&&... args)
{
    to_str_wrapper::to_str(cout, std::forward<Args>(args)...);
    cout << '\n';
}

template <typename... Args>
static void sync_print(Args&&... args)
{
    to_str_wrapper::to_str(osyncstream{cout}, std::forward<Args>(args)...);
    cout << '\n';
}
