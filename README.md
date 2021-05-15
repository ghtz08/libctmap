# libctmap

[![Badge](https://img.shields.io/badge/link-996.icu-%23FF4D5B.svg?style=flat-square)](https://996.icu)
[![LICENSE](https://img.shields.io/badge/license-Anti%20996-blue.svg?style=flat-square)](/LICENSE)
[![Lang](https://img.shields.io/badge/lang-rust-brightgreen?style=flat-square)](https://www.rust-lang.org)
![Repo Size](https://img.shields.io/github/repo-size/ghtz08/libctmap?style=flat-square)
![Code Size](https://img.shields.io/github/languages/code-size/ghtz08/libctmap?style=flat-square)

一个编译时的 c++ map

## 构建

库本身不需要先编译。可以编译一下自带测试代码：

```bash
cd <project dir>
mkdir build
cd build
cmake -DBUILD_TEST=ON ..
```

编译成功即通过测试。

## 用法

在 CMakeLists.txt 中添加如下代码：

```cmake
add_subdirectory(libctmap_project_dir)
target_link_libraries(your_project PRIVATE libctmap)
```

> 编译器最低需要支持 C++11，也可通过定义宏 `CT_ENABLE_CXX_17` 开启 C++17 来提供一些新的接口。例如：

```c++
#define CT_ENABLE_CXX_17
#include <ctmap.hpp>
```

## 接口

后面的代码假设已经存在这样的环境：

```c++
#define CT_ENABLE_CXX_17
#include <ctmap.hpp>

struct k1 {}; struct k2 {}; struct k3 {}; struct k4 {};
struct v1 {}; struct v2 {}; struct v3 {}; using v4 = ct::i32<4>;
```

### constructor

```c++
using m1 = ct::map<>::insert_v<k1, v1>::insert_v<k2, v2>;
using m2 = ct::map_from_list_t<k1, v1, k2, v2>;
static_assert(m1::equal<m1, m2>::value, "");
```

### size

```c++
using m2 = ct::map_from_list_t<k1, v1, k2, v2>;
static_assert(m2::size == 2, "");
```

### contains

```c++
using m2 = ct::map_from_list_t<k1, v1, k2, v2>;
static_assert(m2::contains<k2>::value, "");
static_assert(!m2::contains<k3>::value, "");
static_assert(m2::contains_v<k2>);     // c++17
static_assert(!m2::contains_v<k3>);    // c++17
```

### equal

```c++
using m1 = ct::map_from_list_t<k2, v2, k1, v1>;
using m2 = ct::map_from_list_t<k1, v1, k2, v2>;
static_assert(m1::equal<m2>::value, "");
static_assert(!m1::equal<m2>::value, "");
static_assert(m1::equal_v<m2>);     // c++17
static_assert(!m1::equal_v<m2>);    // c++17
```

### at

```c++
using m = ct::map_from_list_t<k1, v1, k4, v4>;
static_assert(std::is_same<m::at_t<k1>, v1>::value, "");
static_assert(std::is_same<m::at_t<k4>, v4>::value, "");
static_assert(m::at<k4>::value == v4::value, "");
static_assert(m::at_v<k4> == v4::value);    // c++17
```

### erase

```c++
using m1 = ct::map_from_list_t<k1, v1, k2, v2, k3, v3>;
using m2 = ct::map_from_list_t<k1, v1, k3, v3>;
using t = m1::erase_t<k2>;
static_assert(t::equal<m2>::value, "");
```

### merge

```c++
using m1 = ct::map_from_list_t<k1, v1, k3, v3>;
using m2 = ct::map_from_list_t<k2, v2, k4, v4>;
using m3 = ct::map_from_list_t<k1, v1, k2, v2, k3, v3, k4, v4>;
using t = m1::merge_t<m2>;
static_assert(t::equal<m3>::value, "");
```

## 许可证

[反 996 许可证版本 1.0](/LICENSE_CN)
