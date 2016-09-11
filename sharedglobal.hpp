#ifndef SHARED_GLOBAL_H_
#define SHARED_GLOBAL_H_

#include <new>

#ifndef __has_feature
	#define __has_feature(feature) 0
#endif

#if (__cplusplus >= 201103L) || defined(__GXX_EXPERIMENTAL_CXX0X__) || __has_feature(cxx_atomic)
	#include <atomic>
	#define SHARED_GLOBAL_ATOMIC_NAMESPACE_ std
#else
	#include <boost/atomic.hpp>
	#define SHARED_GLOBAL_ATOMIC_NAMESPACE_ boost
#endif

namespace _shared_global_
{
	template <std::size_t size>
	struct GlobalData
	{
		char mem[size];
		SHARED_GLOBAL_ATOMIC_NAMESPACE_::atomic_size_t refsPlusOne;
	};

	inline void init(char* mem, SHARED_GLOBAL_ATOMIC_NAMESPACE_::atomic_size_t& refsPlusOne, void (*construct)(char*))
	{
		std::size_t zero = static_cast<std::size_t>(0);
		
		if(refsPlusOne.compare_exchange_strong(zero, zero + 1))
			construct(mem);
		else
			while(refsPlusOne == 1);
		
		++refsPlusOne;
	}

	inline void deinit(char* mem, SHARED_GLOBAL_ATOMIC_NAMESPACE_::atomic_size_t& refsPlusOne, void (*deconstruct)(char*))
	{
		std::size_t refsPlusOneCopy;
		
		do
		{
			refsPlusOneCopy = refsPlusOne;
		} while(!refsPlusOne.compare_exchange_weak(refsPlusOneCopy, refsPlusOneCopy - 1));
		
		if(refsPlusOneCopy == static_cast<std::size_t>(2))
			deconstruct(mem);
	}
}

#define SHARED_GLOBAL_DECL(T, name, args) \
	static class _##name##_Initializer_ \
	{ \
		public: \
			typedef T type; \
			typedef _shared_global_::GlobalData<sizeof(type)> Data; \
			static Data data; \
		\
		private: \
			static void construct(char* mem) { new (mem) type args; } \
			static void deconstruct(char* mem) { reinterpret_cast<type*>(mem)->~type(); } \
		\
		public: \
			_##name##_Initializer_() { _shared_global_::init(data.mem, data.refsPlusOne, &construct); } \
			~_##name##_Initializer_() { _shared_global_::deinit(data.mem, data.refsPlusOne, &deconstruct); } \
	} _##name##_initializer_; \
	\
	static T& name = *reinterpret_cast<T*>(_##name##_Initializer_::data.mem)

#define SHARED_GLOBAL_DEF(name) \
	_##name##_Initializer_::Data _##name##_Initializer_::data

#endif
