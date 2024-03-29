// what:  variant type boost::any
// who:   contributed by Kevlin Henney,
//        with features contributed and bugs found by
//        Ed Brey, Mark Rodgers, Peter Dimov, and James Curran
// when:  July 2001
// where: tested with BCC 5.5, MSVC 6.0/7.0, and g++ 2.95

#ifndef _ANY_H_
#define _ANY_H_

#include <algorithm>
#include <typeinfo>
#include <sstream>

namespace System
{
	namespace Types
	{
		class any
		{
		public: // structors

			any()
				: content(0)
			{
			}

			template<typename ValueType>
				any(const ValueType & value)
				: content(new holder<ValueType>(value))
			{
			}

			any(const any & other)
				: content(other.content ? other.content->clone() : 0)
			{
			}

			~any()
			{
				delete content;
			}

		public: // modifiers

			any & swap(any & rhs)
			{
				std::swap(content, rhs.content);
				return *this;
			}

			template<typename ValueType>
				any & operator=(const ValueType & rhs)
			{
				any(rhs).swap(*this);
				return *this;
			}

			any & operator=(const any & rhs)
			{
				any(rhs).swap(*this);
				return *this;
			}

		public: // queries

			bool empty() const
			{
				return !content;
			}

			const std::type_info & type() const
			{
				return content ? content->type() : typeid(void);
			}

		private: // types

			class placeholder
			{
			public: // structors

				virtual ~placeholder()
				{
				}

			public: // queries

				virtual const std::type_info & type() const = 0;

				virtual placeholder * clone() const = 0;

			};

			template<typename ValueType>
			class holder : public placeholder
			{
			public: // structors

				holder(const ValueType & value)
					: held(value)
				{
				}

			public: // queries

				virtual const std::type_info & type() const
				{
					return typeid(ValueType);
				}

				virtual placeholder * clone() const
				{
					return new holder(held);
				}

			public: // representation

				ValueType held;

			};

		private: // representation

			template<typename ValueType>
				friend ValueType * any_cast(any *);

			placeholder * content;

		};

		class bad_any_cast : public std::bad_cast
		{
		public:
			virtual const char * what() const throw()
			{
				return "boost::bad_any_cast: "
					"failed conversion using boost::any_cast";
			}
		};

		template<typename ValueType>
			ValueType * any_cast(any * operand)
		{
			return operand && operand->type() == typeid(ValueType)
				? &static_cast<any::holder<ValueType> *>(operand->content)->held
				: 0;
		}

		template<typename ValueType>
			const ValueType * any_cast(const any * operand)
		{
			return any_cast<ValueType>(const_cast<any *>(operand));
		}

		template<typename ValueType>
			ValueType any_cast(const any & operand)
		{
			const ValueType * result = any_cast<ValueType>(&operand);
			if(!result)
				throw new bad_any_cast();
			return *result;
		}

		template<typename result_type, typename arg_type>
			result_type interpret_cast (const arg_type &arg)
		{
			std::stringstream interpreter;
			interpreter << arg;

			result_type result = result_type();
			interpreter >> result;

			return result;
		}
	}
}
// Copyright Kevlin Henney, 2000, 2001, 2002. All rights reserved.
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.

#endif
