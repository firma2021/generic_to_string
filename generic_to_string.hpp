// generic_to_string
// Created by kiki on 2022/9/25.17:29

#pragma once

#include "get_demangled_name.hpp"
#include "get_enum_name.hpp"
#include "get_type_name.hpp"
#include <algorithm>
#include <any>
#include <array>
#include <bits/chrono.h>
#include <charconv>
#include <chrono>
#include <concepts>
#include <ios>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <ostream>
#include <ratio>
#include <sstream>
#include <string>
#include <syncstream>
#include <system_error>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

using namespace std;

// clang-format off
template <typename T>
concept Duration = is_same_v<T, chrono::duration<typename T::rep, typename T::period>>;

template <typename T>
concept TimePoint = is_same_v<remove_cvref_t<T>, chrono::time_point<typename remove_cvref_t<T>::clock, typename remove_cvref_t<T>::duration>>;

template <typename T>
concept Any = is_same_v<any, remove_cvref_t<T>>;

template <typename T>
concept Optional = requires(T t)
{
	remove_cvref_t<T> {nullopt};
    t.has_value();
	requires !Any<T>;
};

template <typename T>
concept Tuple = requires(T t)
{
	requires tuple_size<remove_reference_t<T>>::value == 0;
}
|| requires(T t)
{
	requires tuple_size<remove_reference_t<T>>::value > 0;
	requires is_same_v< remove_reference_t<decltype(get<0>(t))>, tuple_element_t<0, remove_reference_t<T>> >;
};

template <typename T>
concept Variant = requires(T t)
{
	t.index();
	t.valueless_by_exception();
};

template <typename T>
concept Pointer = is_pointer_v<remove_cvref_t<T>>;

template <typename T>
concept Iterator = requires(T t)
{
	requires !Pointer<T>;
	typename iterator_traits<remove_reference_t<T>>::iterator_category;
};

template <typename T>
concept IteratorAdaptor = requires(T t)
{
	requires Iterator<T>;
	t.base();
};

template <typename T>
concept SmartPointer = requires(T t)
{
    { t.operator->() } -> same_as<decltype(declval<T>().get())>;
    { t.operator*() } -> same_as<decltype(*declval<T>().get())>;
    t.operator bool();
};

template <typename T>
concept Enumeration = is_enum_v<remove_reference_t<T>>;

template <typename T>
concept Arithmetic = is_arithmetic_v<remove_cvref_t<T>> && !is_same_v<char, remove_cvref_t<T>>;

template <typename T>
concept Map = requires(T t)
{
	t.begin() != t.end();
    ++t.begin();
    *t.begin();
	requires is_same_v<typename remove_reference_t<T>::value_type, pair<const typename remove_reference_t<T>::key_type, typename remove_reference_t<T>::mapped_type>>;
};

template <typename T>
concept Set = requires(T t)
{
	t.begin() != t.end();
    ++t.begin();
    *t.begin();
	requires is_same_v<typename remove_reference_t<T>::key_type, typename remove_reference_t<T>::value_type>;
};

template< typename Derived, typename Base>
concept derived_from_with_ref = derived_from<remove_reference_t<Derived>, remove_reference_t<Base>>;

template <typename T>
concept StreamInsertable = requires(ostream& os, T&& obj)
{
    {  os << obj } -> derived_from_with_ref<ostream>;
	requires !Pointer<T>;
	requires !Iterator<T>;
	requires !Arithmetic<T>;
	requires !TimePoint<T>;
};

template <typename T>
concept SequenceContainer = requires(T t) // 只要语法正确即可，不会计算结果
{
    t.begin() != t.end();
    ++t.begin();
    *t.begin();

	requires !Map<T>;
	requires !Set<T>;
	requires !StreamInsertable<T>;
};

template <typename T>
concept Sizeable = requires(T t)
{
    size(t) -> is_convertible_v<size_t>;
};

template <typename T>
concept ContainerAdaptor = requires(T t)
{
	typename remove_reference_t<T>::container_type;
	remove_cvref_t<T>{}.pop();
};

template <typename T>
concept Frontable = requires(T t)
{
	t.front();
};
template <typename T>
concept Topable = requires(T t)
{
	t.top();
};

template <typename T>
concept InputStream = derived_from<remove_reference_t<T>, istream>;

// clang-format on
struct generic_to_string_wrapper
{
    generic_to_string_wrapper() = delete;

    static void generic_to_string(ostream& os, Pointer auto&& pointer)
    {
        os << get_type_name<remove_reference_t<decltype(pointer)>>();

        if constexpr (is_volatile_v<decltype(pointer)>)
        {
        }
        else
        {
            os << " at " << pointer;
        }

        if constexpr (is_void_v<remove_cv_t<remove_pointer_t<remove_cvref_t<decltype(pointer)>>>>)
        {
        }
        else
        {
            if (pointer)
            {
                os << ": ";
                generic_to_string(os, *pointer);
            }
            else { os << ": null"; }
        }
    }

    static void generic_to_string(ostream& os, SmartPointer auto&& pointer)
    {
        os << "smart pointer that wraps the raw pointer: ";
        generic_to_string(os, pointer.get());
    }

    static void generic_to_string(ostream& os, Iterator auto&& iter)
    {
        if constexpr (IteratorAdaptor<decltype(iter)>)
        {
            os << "[iterator adaptor] ";
        }

        if constexpr (contiguous_iterator<remove_cvref_t<decltype(iter)>>)
        {
            os << "contiguous_iterator";
        }
        else
        {
            using cate = typename iterator_traits<remove_cvref_t<decltype(iter)>>::iterator_category;

            constexpr variant<
                input_iterator_tag,
                output_iterator_tag,
                forward_iterator_tag,
                bidirectional_iterator_tag,
                random_access_iterator_tag>
                           categories {cate {}};   // contiguous_iterator_tag is iterator concept
            constexpr auto current_index {categories.index()};

            static const array cate_name {"input_iterator", "output_iterator", "forward_iterator", "bidirectional_iterator", "random_access_iterator"};

            os << cate_name.at(current_index);
        }

        os << " to " << get_type_name<remove_reference_t<typename remove_reference_t<decltype(iter)>::reference>>();   // const iterator的value_type不带const,而reference带const
    }

    static void generic_to_string(ostream& os, Arithmetic auto&& number)
    {
        static const map<ios::fmtflags, int> int_flag {
            {ios::dec, 10},
            {ios::oct,  8},
            {ios::hex, 16}
        };
        static const map<ios::fmtflags, int> float_flags {
            {ios::scientific, 10},
            {     ios::fixed,  8},
            {       ios::hex, 16}
        };

        auto                     flags {os.flags()};
        static array<char, 1024> buf {};
        to_chars_result          res {};
        bool                     failed {};

        if constexpr (is_same_v<remove_cvref_t<decltype(number)>, bool>)
        {
            os << number;
        }
        else if constexpr (is_integral_v<remove_reference_t<decltype(number)>>)
        {
            const int base = flags & ios::dec ? 10 : flags & ios::oct ? 8
                                                 : flags & ios::hex   ? 16
                                                                      : 0;

            if (base == 0)
            {
                res = to_chars(begin(buf), end(buf), number);
            }
            else
            {
                res = to_chars(begin(buf), end(buf), number, base);
            }

            if (flags & ios::showbase)
            {
                if (base == 8) { os.write("0", 1); }
                else if (base == 16) { os.write("0x", 2); }
            }

            if (auto [end_ptr, err] = res; err == errc {}) [[likely]]
            {
                os.write(data(buf), end_ptr - begin(buf));
            }
            else
            {
                failed = true;
            }
        }
        else if constexpr (is_floating_point_v<remove_reference_t<decltype(number)>>)
        {
            chars_format fmt {};
            if ((flags & ios::scientific) && (flags & ios::fixed))
            {
                fmt |= chars_format::hex;
                os.write("0x", 2);
            }
            else if (flags & ios::scientific)
            {
                fmt |= chars_format::scientific;
            }
            else if (flags & ios::fixed)
            {
                fmt |= chars_format::fixed;
            }
            else
            {
                fmt |= chars_format::general;
            }

            res = to_chars(begin(buf), end(buf), number, fmt, os.precision());

            if (auto [end_ptr, err] = res; err == errc {}) [[likely]]
            {
                os.write(data(buf), end_ptr - begin(buf));
            }
            else
            {
                failed = true;
            }
        }

        if (failed)
        {
            cerr << "std::to_chars() failed";
            os << number;
        }
    }

    static void generic_to_string(ostream& os, Enumeration auto&& enumer)
    {
        get_enum_name(os, enumer);
    }

    // 对象内含的元素个数超过这个值后，打印元素个数
    static constexpr size_t size_threshold {8};

    static void generic_to_string(ostream& os, Tuple auto&& tup)   // pair、array、tuple
    {
        constexpr size_t tup_size {tuple_size_v<remove_reference_t<decltype(tup)>>};
        if constexpr (tup_size == 0)
        {
            os << "()";
            return;   // fake return
        }
        else
        {
            os << '(';

            if constexpr (tup_size > size_threshold)
            {
                os << "size=" << tup_size << ' ';
            }

            // args可以是任意数量的参数, 甚至是0个
            // 左折叠；长度为0的参数包允许一元折叠
            auto dump =
                [&os](auto&& arg,
                      auto&&... args)
            {
                generic_to_string(os, std::forward<decltype(arg)>(arg));

                (..., (os << ' ', generic_to_string(os, std::forward<decltype(args)>(args))));
            };

            apply(dump, tup);

            os << ')';
        }
    }

    static void generic_to_string(ostream& os, SequenceContainer auto&& container)
    {
        size_t container_size {};
        if constexpr (Sizeable<decltype(container)>)
        {
            container_size = size(container);
        }
        else
        {
            container_size = distance(begin(container), end(container));
        }

        if (container_size == 0)
        {
            os << '[' << ']';
            return;
        }

        os << '[';

        if (container_size > size_threshold)
        {
            os << "size=" << container_size << ' ';
        }

        auto iter {begin(container)};
        auto end_iter {end(container)};
        for (; iter != end_iter;)
        {
            generic_to_string(os, std::forward<decltype(*iter)>(*iter));
            if (++iter != end_iter) [[likely]] { os << ' '; }
        }

        os << ']';
    }

    static void generic_to_string(ostream& os, Set auto&& set)
    {
        os << '{';
        auto iter {begin(set)};
        auto end_iter {end(set)};
        for (; iter != end_iter;)
        {
            generic_to_string(os, std::forward<decltype(*iter)>(*iter));
            if (++iter != end_iter) [[likely]] { os << ' '; }
        }
        os << '}';
    }

    static void generic_to_string(ostream& os, Map auto&& map)
    {
        os << '{';
        auto iter {cbegin(map)};
        auto end_iter {cend(map)};
        for (; iter != end_iter;)
        {
            generic_to_string(os, std::forward<decltype(iter->first)>(iter->first));
            os << ':';
            generic_to_string(os, std::forward<decltype((iter->second))>(iter->second));
            if (++iter != end_iter) [[likely]] { os << ' '; }
        }
        os << '}';
    }

    static void generic_to_string(ostream& os, ContainerAdaptor auto&& adaptor)
    {
        remove_cvref_t<decltype(adaptor)> copy {adaptor};

        os << '[';

        if constexpr (Topable<decltype(adaptor)>)
        {
            while (copy.size() > 1)
            {
                generic_to_string(os, copy.top());
                copy.pop();
                os << ' ';
            }

            if (!copy.empty()) { generic_to_string(os, copy.top()); }
        }
        else if constexpr (Frontable<decltype(adaptor)>)
        {
            while (copy.size() > 1)
            {
                generic_to_string(os, copy.front());
                copy.pop();
                os << ' ';
            }

            if (!copy.empty()) { generic_to_string(os, copy.front()); }
        }
        else
        {
            os << "container adaptor";
        }
        os << ']';
    }

    static void generic_to_string(ostream& os, InputStream auto&& input_stream)
    {
        os << "input stream with state: ";

        auto state {input_stream.rdstate()};

        ostringstream oss;
        if (state == ios::goodbit)   // goodbit = 0, badbit = 1 << 0, eofbit = 1 << 1, failbit = 1 << 2
        {
            oss << "good,";
        }
        array state_str {
            pair { ios::badbit,  "bad,"},
            pair {ios::failbit, "fail,"},
            pair { ios::eofbit,  "eof,"}
        };
        for_each(begin(state_str), end(state_str), [&oss, &state](auto&& p)
                 {
            if ((static_cast<unsigned>(state) & static_cast<unsigned>(p.first)) != 0)
            {
                oss << p.second;
            } });

        oss.seekp(-1, ostream::end);
        oss << '\n';

        os << oss.str();

        if (state != ios::goodbit)
        {
            return;
        }

        auto istream_size = [](istream& input_stream, ostream::pos_type current_pos) -> streamoff
        {
            input_stream.seekg(0, istream::end);
            auto end_pos {input_stream.tellg()};

            if (current_pos == -1 || end_pos == -1) { return -1; }

            input_stream.seekg(current_pos);

            return end_pos - current_pos;
        };

        auto count_line = [](istream& input_stream, ostream::pos_type beg_pos)
        {
            size_t line_tally {1};

            for_each(istreambuf_iterator<char> {input_stream},
                     istreambuf_iterator<char> {},
                     [&line_tally](auto ch)
                     {
                         if (ch == '\n')
                         {
                             ++line_tally;
                         }
                     });

            input_stream.seekg(beg_pos);

            return line_tally;
        };

        auto beg_pos {input_stream.tellg()};
        auto stream_size {istream_size(input_stream, beg_pos)};
        auto line_tally {count_line(input_stream, beg_pos)};

        os << "input stream with size " << stream_size << ", line " << line_tally << '\n';

        if (stream_size != 0)
        {
            os << input_stream.rdbuf();
        }

        input_stream.seekg(beg_pos);
    }

    static void generic_to_string(ostream& os, Optional auto&& opt)
    {
        if (opt.has_value())
        {
            os << "optional: ";
            generic_to_string(os, opt.value());   // value()返回内含对象的引用
        }
        else
        {
            os << "optional: nullopt";
        }
    }

    static void generic_to_string(ostream& os, Variant auto&& variant)
    {
        constexpr auto constexpr_for = []<size_t... N>(index_sequence<N...>, auto&& f) constexpr
        {
            (f.template operator()<N>(), ...);
        };

        constexpr size_t variant_size {variant_size_v<remove_reference_t<decltype(variant)>>};
        if constexpr (variant_size == 0)
        {
            os << "empty variant";
        }
        else
        {
            if (variant.valueless_by_exception())
            {
                os << "variant: valueless by exception";
            }
            else
            {
                vector<string_view> types;
                constexpr_for(make_index_sequence<variant_size> {}, [&]<size_t n>() mutable
                              { types.push_back(get_type_name<variant_alternative_t<n, remove_reference_t<decltype(variant)>>>()); });

                auto idx {variant.index()};
                os << "variant<" << idx << ", ";
                os << types.at(idx) << ">: ";

                auto dump = [&os](auto&& arg)
                { generic_to_string(os, std::forward<decltype(arg)>(arg)); };

                visit(dump, std::forward<decltype(variant)>(variant));
            }
        }
    }

    static void generic_to_string(ostream& os, Any auto&& any)
    {
        os << "any<";
        get_typename_from_typeinfo(os, any.type());
        os << '>';
    }

    static void generic_to_string(ostream& os, Duration auto&& d)
    {
        os << duration_cast<chrono::duration<double, milli>>(std::forward(d)).count()
           << " ms";
    }

    static void generic_to_string(ostream& os, TimePoint auto&& time_point)
    {
        if constexpr (is_same_v<remove_cvref_t<decltype(time_point)>, decltype(chrono::system_clock::now())>)
        {
            chrono::zoned_time zt {chrono::current_zone(), time_point};
            os << zt;
        }
        else
        {
            os << time_point.time_since_epoch();
        }
    }

    static void generic_to_string(ostream& os, StreamInsertable auto&& val)
    {
        os << val;
    }

    static void generic_to_string(ostream& os, auto&& t)
    {
        os << get_type_name(t) << ' ' << '[';

        if constexpr (!is_rvalue_reference_v<decltype(t)>)
        {
            os << "addr " << addressof(t) << ' ';
        }

        os << "size " << sizeof(t) << ']';
    }
};

class generic_ostream
{
private:
    ostream& os;

public:
    explicit generic_ostream(ostream& os) noexcept:
    os {os} { }

    generic_ostream& operator<<(auto&& arg)
    {
        generic_to_string_wrapper::generic_to_string(os, std::forward<decltype(arg)>(arg));
        return *this;
    }

    generic_ostream& operator<<(ostream& (*func)(ostream&))
    {
        func(os);   //os << func;
        return *this;
    }

    ostream& get() { return os; }

    template <typename... Args>
    void operator()(Args&&... args)
    {
        (generic_to_string_wrapper::generic_to_string(os, std::forward<Args>(args)), ...);
    }
};

class generic_osyncstream
{
private:
    ostream& os;

public:
    explicit generic_osyncstream(ostream& os) noexcept:
    os {os} { }

    generic_osyncstream& operator<<(auto&& arg)
    {
        generic_to_string_wrapper::generic_to_string(osyncstream {os}, std::forward<decltype(arg)>(arg));
        return *this;
    }

    generic_osyncstream& operator<<(ostream& (*func)(ostream&))
    {
        func(os);
        return *this;
    }

    ostream& get() { return os; }
};

class generic_ostringstream
{
private:
    ostringstream oss;

public:
    generic_ostringstream& operator<<(auto&& arg)
    {
        generic_to_string_wrapper::generic_to_string(oss, std::forward<decltype(arg)>(arg));
        return *this;
    }

    string str()
    {
        string s {oss.str()};
        oss.str("");
        return s;
    }
    string shrink()
    {
        return std::move(oss).str();
    }

    ostream& get() { return oss; }
};

class generic_osyncstringstream
{
private:
    static thread_local ostringstream oss;

public:
    generic_osyncstringstream& operator<<(auto&& arg)
    {
        generic_to_string_wrapper::generic_to_string(osyncstream {oss}, std::forward<decltype(arg)>(arg));
        return *this;
    }

    static string str()
    {
        string s {oss.str()};
        oss.str("");
        return s;
    }
    static string shrink()
    {
        return std::move(oss).str();
    }

    static ostream& get() { return oss; }
};

inline generic_ostream     gout {cout};
inline generic_osyncstream sync_gout {cout};

inline generic_ostringstream     sout;
inline generic_osyncstringstream sync_sout;
