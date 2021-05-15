#include <cstddef> // for size_t
#include <cstdint> // for intmm_t
#include <type_traits>

namespace ct
{
namespace details {
    template <typename T, size_t numerator, size_t denominator>
    struct float_at_t {
        static_assert(false
            || std::is_same<T, float>::value
            || std::is_same<T, double>::value
        , "");
        static constexpr auto value = static_cast<T>(1) *
            numerator / denominator;
    };
}   // namespace details
template <typename T, T value_>
struct enum_t {
    static constexpr auto value = value_;
};
template <size_t numerator, size_t denominator>
struct f32 : details::float_at_t<float, numerator, denominator> {};
template <size_t numerator, size_t denominator>
struct f64 : details::float_at_t<double, numerator, denominator> {};
template <int8_t   value> struct i8  : enum_t<decltype(value), value> {};
template <int16_t  value> struct i16 : enum_t<decltype(value), value> {};
template <int32_t  value> struct i32 : enum_t<decltype(value), value> {};
template <int64_t  value> struct i64 : enum_t<decltype(value), value> {};
template <uint8_t  value> struct u8  : enum_t<decltype(value), value> {};
template <uint16_t value> struct u16 : enum_t<decltype(value), value> {};
template <uint32_t value> struct u32 : enum_t<decltype(value), value> {};
template <uint64_t value> struct u64 : enum_t<decltype(value), value> {};

template <typename... items>
class map;
namespace details {

template <typename T>
struct remove_cvref {
    using type = typename std::remove_cv<
                typename std::remove_reference<T>::type
            >::type;
};

template <typename T>
struct is_cv_or_ref {
    static constexpr auto value = !std::is_same<
            T, typename remove_cvref<T>::type
        >::value;
};

template <typename... args_t>
struct constant_false {
    static constexpr auto value = false;
};

template <typename T>
struct has_member_value {
  private:
    template <typename U, int = sizeof(&U::value)>
    static auto test(int) -> uint8_t;
    template <typename U>
    static auto test(long) -> uint16_t;
  public:
    static constexpr auto value = sizeof(test<T>(0)) == sizeof(uint8_t);
};

template <typename T, typename U>
struct pair_t {
    using first_t = T;
    using second_t = U;
};

template <typename key_t, typename...>
struct map_contains_t;
template <typename key_t, typename item, typename... items>
struct map_contains_t<key_t, item, items...> {
    static constexpr auto value = false
        || std::is_same<key_t, typename item::first_t>::value
        || map_contains_t<key_t, items...>::value
    ;
};
template <typename key_t>
struct map_contains_t<key_t> {
    static constexpr auto value = false;
};

template <typename key_t, typename... items>
struct map_at_find_t;
template <bool found, typename key_t, typename item, typename... items>
struct map_at_find_t_ {
    using type = typename map_at_find_t<key_t, items...>::type;
};
template <typename key_t, typename item, typename... items>
struct map_at_find_t_<true, key_t, item, items...> {
    using type = typename item::second_t;
};
template <typename key_t>
struct map_at_find_t<key_t> {
    static_assert(constant_false<key_t>::value, "no key");
};
template <typename key_t, typename item, typename... items>
struct map_at_find_t<key_t, item, items...> {
  private:
    static constexpr auto found = std::is_same<
            key_t, typename item::first_t
        >::value;
  public:
    using type = typename map_at_find_t_<found, key_t, item, items...>::type;
};

template <typename T, bool has_value = has_member_value<T>::value>
struct map_at_t_ {
    using type = T;
};
template <typename T>
struct map_at_t_<T, true>: map_at_t_<T, false> {
    static constexpr auto value = T::value;
};
template <typename key_t, typename... items>
struct map_at_t: map_at_t_<typename map_at_find_t<key_t, items...>::type> {};

template <typename key_t, typename value_t, typename... items>
struct map_insert_t {
    static_assert(!details::is_cv_or_ref<key_t>::value, "");
    static_assert(!details::is_cv_or_ref<value_t>::value, "");
    static_assert(!::ct::map<items...>::template contains<key_t>::value, "");
    using type = ::ct::map<items..., details::pair_t<key_t, value_t>>;
};

template <typename key_t, typename M, typename... items>
struct map_erase_t;
template <bool found, typename key_t, typename M, typename item,
                typename... items>
struct map_erase_t_;
template <typename key_t, typename... items1, typename item, typename... items2>
struct map_erase_t_<false, key_t, ::ct::map<items1...>, item, items2...> {
    using type = typename map_erase_t<
            key_t, ::ct::map<items1..., item>, items2...
        >::type;
};
template <typename key_t, typename... items1, typename item, typename... items2>
struct map_erase_t_<true, key_t, ::ct::map<items1...>, item, items2...> {
    using type = ::ct::map<items1..., items2...>;
};
template <typename key_t, typename M>
struct map_erase_t<key_t, M> {
    static_assert(constant_false<key_t>::value, "no key");
};
template <typename key_t, typename... items1, typename item, typename... items2>
struct map_erase_t<key_t, ::ct::map<items1...>, item, items2...> {
    using type = typename map_erase_t_<
            std::is_same<key_t, typename item::first_t>::value,
            key_t, ::ct::map<items1...>, item, items2...
        >::type;
};

template <typename M, typename... items>
struct map_merge_t_;
template <typename M>
struct map_merge_t_<M> {
    using type = M;
};
template <typename... items1, typename item, typename... items2>
struct map_merge_t_<::ct::map<items1...>, item, items2...> {
    static_assert(
        !::ct::map<items1...>::template contains<typename item::first_t>::value,
        "the same key exists"
    );
    using type = typename map_merge_t_<
            ::ct::map<items1..., item>, items2...
        >::type;
};
template <typename M1, typename M2>
struct map_merge_t;
template <typename M, typename... items>
struct map_merge_t<M, ::ct::map<items...>> {
    using type = typename map_merge_t_<M, items...>::type;
};

template <bool len_eq, typename M, typename... items>
struct map_equal_t_ {
    static constexpr auto value = false;
};
template <typename M>
struct map_equal_t_<true, M> {
    static constexpr auto value = true;
};
template <typename M, typename item, typename... items>
struct map_equal_t_<true, M, item, items...> {
    static constexpr auto value = true
        && M::template contains<typename item::first_t>::value
        && map_equal_t_<true, M, items...>::value
    ;
};
template <typename M1, typename M2>
struct map_equal_t;
template <typename M, typename... items>
struct map_equal_t<M, ::ct::map<items...>> {
    static constexpr auto value = map_equal_t_<
            M::size == sizeof...(items), M, items... 
        >::value;
};

template <typename M, typename... args_t>
struct map_from_list_t;
template <typename M>
struct map_from_list_t<M> {
    using type = M;
};
template <typename M, typename rem>
struct map_from_list_t<M, rem> {
    static_assert(constant_false<rem>::value, "missing value");
};
template <typename M, typename key_t, typename value_t, typename... args_t>
struct map_from_list_t<M, key_t, value_t, args_t...> {
    static_assert(!M::template contains<key_t>::value, "");
    using type = typename map_from_list_t<
            typename M::template insert_t<key_t, value_t>, args_t...
        >::type;
};

}   // namespace details

template <typename... args_t>
struct map_from_list: details::map_from_list_t<::ct::map<>, args_t...> {};
template <typename... args_t>
using map_from_list_t = typename map_from_list<args_t...>::type;

template <typename... items>
class map {
  private:
    using self_t = map<items...>;

  public:
    static constexpr auto size = sizeof...(items);
    template <typename key_t>
    struct contains: details::map_contains_t<key_t, items...> {};
    template <typename M>
    struct equal: details::map_equal_t<self_t, M> {};
#if defined(CT_ENABLE_CXX_17)
    template <typename key_t>
    static constexpr auto contains_v = self_t::template contains<key_t>::value;
    template <typename M>
    static constexpr auto equal_v = self_t::template equal<M>::value;
#endif

    template <typename key_t>
    struct at: details::map_at_t<key_t, items...> {};
    template <typename key_t>
    using at_t = typename self_t::template at<key_t>::type;
#if defined(CT_ENABLE_CXX_17)
    template <typename key_t>
    static constexpr auto at_v = self_t::template at<key_t>::value;
#endif

    template <typename key_t, typename value_t>
    struct insert: details::map_insert_t<key_t, value_t, items...> {};
    template <typename key_t, typename value_t>
    using insert_t = typename self_t::template insert<key_t, value_t>::type;

    template <typename key_t>
    struct erase: details::map_erase_t<key_t, map<>, items...> {};
    template <typename key_t>
    using erase_t = typename self_t::template erase<key_t>::type;

    template <typename M>
    struct merge: details::map_merge_t<self_t, M> {};
    template <typename M>
    using merge_t = typename self_t::template merge<M>::type;
};  // class map

} // namespace ct
