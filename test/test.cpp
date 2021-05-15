#include <iostream>
#include <ctmap.hpp>

#if defined(CT_ENABLE_CXX_17)
#   pragma message("C++ 17 is enabled")
#endif

int main() {
    struct k1 {}; struct k2 {}; struct k3 {}; struct k4 {};
    struct v1 {}; struct v2 {}; struct v3 {}; using v4 = ct::i32<4>;

    using m0 = ct::map<>;
    using m1 = m0::insert_t<k1, v1>;
    using m2 = m1::insert_t<k2, v2>;
    using m3 = m2::insert_t<k3, v3>;
    using m4 = m3::insert_t<k4, v4>;
    // test size
    {
        static_assert(m0::size == 0, "");
        static_assert(m1::size == 1, "");
        static_assert(m2::size == 2, "");
        static_assert(m3::size == 3, "");
        static_assert(m4::size == 4, "");
    }
    // test contains
    {
        static_assert(m2::contains<k2>::value, "");
        static_assert(!m2::contains<k3>::value, "");
        static_assert(!m0::contains<k1>::value, "");
#if defined(CT_ENABLE_CXX_17)
        static_assert(m2::contains_v<k2>, "");
        static_assert(!m2::contains_v<k3>, "");
        static_assert(!m0::contains_v<k1>, "");
#endif
    }
    // test equal
    {
        using t = ct::map_from_list_t<k1, v1, k2, v2>;
        static_assert(t::equal<m2>::value, "");
#if defined(CT_ENABLE_CXX_17)
        static_assert(t::equal_v<m2>, "");
#endif
    }
    // test at
    {
        static_assert(std::is_same<m1::at_t<k1>, v1>::value, "");
        static_assert(std::is_same<m2::at_t<k2>, v2>::value, "");
        static_assert(std::is_same<m3::at_t<k3>, v3>::value, "");
        static_assert(std::is_same<m4::at_t<k4>, v4>::value, "");
        static_assert(m4::at<k4>::value == v4::value, "");
#if defined(CT_ENABLE_CXX_17)
        static_assert(m4::at_v<k4> == v4::value, "");
#endif
    }
    // test erase
    {
        using t = m3::erase<k2>::type;
        using u = ct::map_from_list<k1, v1, k3, v3>::type;
        static_assert(t::equal<u>::value, "");
    }
    // test merge
    {
        using a = ct::map_from_list_t<k1, v1, k3, v3>;
        using b = ct::map_from_list_t<k2, v2, k4, v4>;
        using t = a::merge<b>::type;
        static_assert(t::equal<m4>::value, "");
    }
}
