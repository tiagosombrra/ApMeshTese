#ifndef _DATA_IDENTIFIABLE_H_
#define _DATA_IDENTIFIABLE_H_

#include "Data/Definitions.h"

namespace Data
{
	class Identifiable
	{
	public:

		Identifiable(ULInt id = 0);
		virtual ~Identifiable();

		virtual void setId(ULInt id);
		virtual ULInt getId() const;

		static bool less(const Identifiable *i1, const Identifiable *i2);
		static bool greater(const Identifiable *i1, const Identifiable *i2);
		static bool equal(const Identifiable *i1, const Identifiable *i2);
		static bool different(const Identifiable *i1, const Identifiable *i2);

		struct Less
		{
			bool operator()(Identifiable *const &n1, Identifiable *const &n2) const
			{
				return Identifiable::less(n1, n2);
			};
		};

		struct Greater
		{
			bool operator()(Identifiable *const &n1, Identifiable *const &n2) const
			{
				return Identifiable::greater(n1, n2);
			};
		};

		struct Equal
		{
			bool operator()(Identifiable *const &n1, Identifiable *const &n2) const
			{
				return Identifiable::equal(n1, n2);
			};
		};

		struct Different
		{
			bool operator()(Identifiable *const &n1, Identifiable *const &n2) const
			{
				return Identifiable::different(n1, n2);
			};
		};

#if USE_C__11
		struct Hash
		{
			size_t operator()(Identifiable *const &n) const
			{
				return std::hash<ULInt>()(n->getId());
			}
		};
#endif //#if USE_C__11

	protected:

		ULInt id;
	};
}

#endif //#ifndef _DATA_IDENTIFIABLE_H_
