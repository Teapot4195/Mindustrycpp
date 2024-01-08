//
// Created by teapot on 22/11/23.
//

#ifndef MINDUSTRY_SRC_ARC_FUNC_H
#define MINDUSTRY_SRC_ARC_FUNC_H

#include <functional>

using Boolc = std::function<void(bool)>;
template<typename T>
using Boolf = std::function<bool(T)>;
template<typename A, typename B>
using Boolf2 = std::function<bool(A, B)>;
template<typename P1, typename P2, typename P3>
using Boolf3 = std::function<bool(P1, P2, P3)>;
using Boolp = std::function<bool()>;
template<typename T>
using Cons = std::function<void(T)>;
template<typename T, typename N>
using Cons2 = std::function<void(T, N)>;
template<typename P1, typename P2, typename P3>
using Cons3 = std::function<void(P1, P2, P3)>;
template<typename P1, typename P2, typename P3, typename P4>
using Cons4 = std::function<void(P1, P2, P3, P4)>;
template<typename T, typename E>
requires std::derived_from<E, std::exception>
using ConsT = std::function<void(T)>;
using Floatc = std::function<void(float)>;
using Floatc2 = std::function<void(float, float)>;
using Floatc4 = std::function<void(float, float, float, float)>;
template<typename T>
using Floatf = std::function<float(T)>;
using FloatFloatf = std::function<float(float)>;
using Floatp = std::function<float()>;
template<typename P, typename R>
using Func = std::function<R(P)>;
template<typename P1, typename P2, typename R>
using Func2 = std::function<R(P1, P2)>;
template<typename P1, typename P2, typename P3, typename R>
using Func3 = std::function<R(P1, P2, P3)>;
using Intc = std::function<void(int)>;
using Intc2 = std::function<void(int, int)>;
using Intc4 = std::function<void(int, int, int, int)>;
template<typename T>
using Intf = std::function<int(T)>;
using IntIntf = std::function<int(int)>;
using Intp = std::function<int()>;
template<typename T>
using Longf = std::function<long(T)>;
template<typename T>
using Prov = std::function<T()>;

#endif //MINDUSTRY_SRC_ARC_FUNC_H
