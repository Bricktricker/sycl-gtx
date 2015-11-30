
#ifndef SYCL_GTX

namespace cl {
namespace sycl {

#define SYCL_TYPE(type)	\
using type##1 = type;

SYCL_TYPE(bool)
SYCL_TYPE(float)
SYCL_TYPE(double)

#undef SYCL_TYPE


#define SYCL_TYPE(type)	\
using type##1 = type;	\
using u##type##1 = unsigned type;

SYCL_TYPE(int)
SYCL_TYPE(char)
SYCL_TYPE(short)
SYCL_TYPE(long)

#undef SYCL_TYPE

#define SYCL_ONE_ARG(name)			\
template <class First>				\
static First name(First first) {	\
	return ::std:: name(first);		\
}

SYCL_ONE_ARG(cos)
SYCL_ONE_ARG(fabs)
SYCL_ONE_ARG(sin)
SYCL_ONE_ARG(sqrt)

#undef SYCL_ONE_ARG

template <class First>
static First sqrt(First first) {
	return ::std::sqrt(first);
}

template <class First, class Second>
static auto min(First first, Second second) -> decltype(::std::min(first, second)) {
	return ::std::min(first, second);
}

} // namespace sycl
} // namespace cl


#define SYCL_IF(condition) if(condition)

#define SYCL_BEGIN {
#define SYCL_END }

#define SYCL_ELSE else
#define SYCL_ELSE_IF(condition)	else if(condition)

#define SYCL_WHILE(condition) while(condition)
#define SYCL_FOR(init, condition, increment) for(init; condition; increment)

#define SYCL_BREAK break;
#define SYCL_CONTINUE continue;
#define SYCL_RETURN	return;

#endif // SYCL_GTX
