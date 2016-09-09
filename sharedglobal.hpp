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

template <typename T>
struct SharedGlobalData
{
	public:
		char mem[sizeof(T)];
	
	private:
		SHARED_GLOBAL_ATOMIC_NAMESPACE_::atomic_size_t refsPlusOne;
		
	public:
		~SharedGlobalData()
		{
			std::size_t refsPlusOneCopy;
			
			do
			{
				refsPlusOneCopy = refsPlusOne;
			} while(!refsPlusOne.compare_exchange_weak(refsPlusOneCopy, refsPlusOneCopy - 1));
			
			if(refsPlusOneCopy == static_cast<std::size_t>(2))
				reinterpret_cast<T&>(mem).~T();
		}
		
		void init(void (*construct)())
		{
			std::size_t zero = static_cast<std::size_t>(0);
			
			if(refsPlusOne.compare_exchange_strong(zero, zero + 1))
				construct();
			else
				while(refsPlusOne == 1);
			
			++refsPlusOne;
		}
};

#define SHARED_GLOBAL_DECL(T, name, args) \
	static class name##_Initializer_ \
	{ \
		public: \
			typedef SharedGlobalData<T> Data; \
			static Data data; \
		\
		public: \
			name##_Initializer_() \
			{ \
				data.init(construct); \
			} \
		\
		private: \
			static void construct() \
			{ \
				new (&data.mem) T args; \
			} \
	} name##_initializer_; \
	\
	static T& name = reinterpret_cast<T&>(name##_Initializer_::data.mem)

#define SHARED_GLOBAL_DEF(name) \
	name##_Initializer_::Data name##_Initializer_::data

#endif
