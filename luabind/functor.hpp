// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#if !BOOST_PP_IS_ITERATING

#ifndef LUABIND_FUNCTOR_HPP_INCLUDED
#define LUABIND_FUNCTOR_HPP_INCLUDED

#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/type_traits/is_void.hpp>
#include <boost/mpl/if.hpp>

#include <luabind/config.hpp>
#include <luabind/detail/policy.hpp>
#include <luabind/detail/convert_to_lua.hpp>
#include <luabind/detail/error.hpp>

namespace luabind
{
//template<class Ret>
//class functor;

	namespace detail
	{

		struct functor_from;


		// if the proxy_functor_caller returns non-void
			template<class Ret, class Tuple>
			class proxy_functor_caller
			{
//			template<class T> friend class luabind::functor;
			public:

				proxy_functor_caller(luabind::functor<Ret>* o, const Tuple args)
					: m_func(o)
					, m_args(args)
					, m_called(false)
				{
				}

				proxy_functor_caller(const proxy_functor_caller& rhs)
					: m_func(rhs.m_func)
					, m_args(rhs.m_args)
					, m_called(rhs.m_called)
				{
					rhs.m_called = true;
				}

				~proxy_functor_caller()
				{
					if (m_called) return;

					m_called = true;
					lua_State* L = m_func->lua_state();

					// get the function
					m_func->pushvalue();

					push_args_from_tuple<1>::apply(L, m_args);
					if (lua_pcall(L, boost::tuples::length<Tuple>::value, 0, 0))
					{ 
#ifndef LUABIND_NO_EXCEPTIONS
						throw luabind::error();
#else
						assert(0);
#endif
					}
				}

				operator Ret()
				{
					typename default_policy::template generate_converter<Ret, lua_to_cpp>::type converter;

					m_called = true;
					lua_State* L = m_func->lua_state();
					detail::stack_pop p(L, 1); // pop the return value

					// get the function
					m_func->pushvalue();

					push_args_from_tuple<1>::apply(L, m_args);
					if (lua_pcall(L, boost::tuples::length<Tuple>::value, 1, 0))
					{ 
#ifndef LUABIND_NO_EXCEPTIONS
						throw luabind::error();
#else
						assert(0);
#endif
					}

#ifndef LUABIND_NO_ERROR_CHECKING
#ifndef LUABIND_NO_EXCEPTIONS

					if (converter.match(L, LUABIND_DECORATE_TYPE(Ret), -1) < 0)
					{
						throw cant_convert_return_value();
					}
#else
					assert(converter.match(L, LUABIND_DECORATE_TYPE(Ret), -1) >= 0);
#endif
#endif
					return converter.apply(L, LUABIND_DECORATE_TYPE(Ret), -1);
				}

				template<class Policies>
				Ret operator[](const Policies& p)
				{
					typedef typename detail::find_conversion_policy<0, Policies>::type converter_policy;
					typename converter_policy::template generate_converter<Ret, lua_to_cpp>::type converter;

					m_called = true;
					lua_State* L = m_func->lua_state();
					detail::stack_pop popper(L, 1); // pop the return value

					// get the function
					m_func->pushvalue();

					detail::push_args_from_tuple<1>::apply(L, m_args, p);
					if (lua_pcall(L, boost::tuples::length<Tuple>::value, 1, 0))
					{ 
#ifndef LUABIND_NO_EXCEPTIONS
						throw error();
#else
						assert(0);
#endif
					}

#ifndef LUABIND_NO_ERROR_CHECKING
#ifndef LUABIND_NO_EXCEPTIONS

					if (converter.match(L, LUABIND_DECORATE_TYPE(Ret), -1) < 0)
					{
						throw cant_convert_return_value();
					}
#else
					assert(converter.match(L, LUABIND_DECORATE_TYPE(Ret), -1) >= 0);
#endif
#endif
					return converter.apply(L, LUABIND_DECORATE_TYPE(Ret), -1);
				}

			private:

				luabind::functor<Ret>* m_func;
				Tuple m_args;
				mutable bool m_called;

			};

		// if the proxy_member_caller returns void
			template<class Tuple>
			class proxy_functor_void_caller
			{
//			template<class T> friend class luabind::functor;
			public:

				proxy_functor_void_caller(luabind::functor<void>* o, const Tuple args)
					: m_func(o)
					, m_args(args)
					, m_called(false)
				{
				}

				proxy_functor_void_caller(const proxy_functor_void_caller& rhs)
					: m_func(rhs.m_func)
					, m_args(rhs.m_args)
					, m_called(rhs.m_called)
				{
					rhs.m_called = true;
				}

				~proxy_functor_void_caller()
				{
					if (m_called) return;

					m_called = true;
					lua_State* L = m_func->lua_state();

					// get the function
					m_func->pushvalue();

					push_args_from_tuple<1>::apply(L, m_args);
					if (lua_pcall(L, boost::tuples::length<Tuple>::value, 0, 0))
					{ 
#ifndef LUABIND_NO_EXCEPTIONS
						throw luabind::error();
#else
						assert(0);
#endif
					}
				}

				template<class Policies>
				void operator[](const Policies& p)
				{
					m_called = true;
					lua_State* L = m_func->lua_state();

					// get the function
					m_func->pushvalue();

					detail::push_args_from_tuple<1>::apply(L, m_args, p);
					if (lua_pcall(L, boost::tuples::length<Tuple>::value, 0, 0))
					{ 
#ifndef LUABIND_NO_EXCEPTIONS
						throw error(); 
#else
						assert(0);
#endif
					}
				}

			private:

				luabind::functor<void>* m_func;
				Tuple m_args;
				mutable bool m_called;

			};

	} // detail

	template<class Ret>
	class functor
	{
	public:

		functor(lua_State* L, const char* name) : L_(L)
		{
			lua_pushstring(L, name);
			lua_gettable(L, LUA_GLOBALSINDEX);
			ref_ = detail::ref(L);
		}

		functor()
			: L_(0)
			, ref_(LUA_NOREF)
		{
		}

		functor(const functor<Ret>& obj): L_(obj.L_)
		{
			lua_getref(L_, obj.ref_);
			ref_ = detail::ref(L_);
		}

		~functor()
		{
			if (ref_ != LUA_NOREF) detail::unref(L_, ref_);
		}

		// this is a safe substitute for an implicit converter to bool
		typedef void (functor::*member_ptr)() const;
		operator member_ptr() const
		{
			if (is_valid()) return &functor::dummy;
			return 0;
		}

		const functor<Ret>& operator=(const functor<Ret>& rhs)
		{
			L_ = rhs.L_;
			lua_getref(L_, rhs.ref_);
			ref_ = detail::ref(L_);
			return *this;
		}

		inline bool is_valid() const { return ref_ != LUA_NOREF; }
	
		lua_State* lua_state() { return L_; }
		void pushvalue() { lua_getref(L_, ref_); }	

		#define BOOST_PP_ITERATION_PARAMS_1 (4, (0, LUABIND_MAX_ARITY, <luabind/functor.hpp>, 1))
		#include BOOST_PP_ITERATE()

// TODO: should be private

		functor(lua_State* L, int ref)
			: L_(L)
			, ref_(ref)
		{
		}

	private:

		void dummy() const {}

		lua_State* L_;
		int ref_;
	};
}

#endif // LUABIND_FUNCTOR_HPP_INCLUDED

#else

#define LUABIND_TUPLE_PARAMS(z, n, data) const A##n *
#define LUABIND_OPERATOR_PARAMS(z, n, data) const A##n & a##n

#if BOOST_PP_ITERATION() > 0
	template<BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), class A)>
#endif
	typename boost::mpl::if_<boost::is_void<Ret>
		, luabind::detail::proxy_functor_void_caller<boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> >
		, luabind::detail::proxy_functor_caller<Ret, boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> > >::type
	operator()(BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_OPERATOR_PARAMS, _)) const
	{
		typedef boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> tuple_t;
#if BOOST_PP_ITERATION() == 0
		tuple_t args;
#else
		tuple_t args(BOOST_PP_ENUM_PARAMS(BOOST_PP_ITERATION(), &a));
#endif

		typedef typename boost::mpl::if_<boost::is_void<Ret>
			, luabind::detail::proxy_functor_void_caller<boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> >
			, luabind::detail::proxy_functor_caller<Ret, boost::tuples::tuple<BOOST_PP_ENUM(BOOST_PP_ITERATION(), LUABIND_TUPLE_PARAMS, _)> > >::type proxy_type;
		
		return proxy_type(const_cast<luabind::functor<Ret>*>(this), args);
	}

	#define LUABIND_TUPLE_PARAMS(z, n, data) const A##n *
	#define LUABIND_OPERATOR_PARAMS(z, n, data) const A##n & a##n

#endif

