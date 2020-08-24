#ifndef _DATA_EXACT_H_
#define _DATA_EXACT_H_

#include "Data/Definitions.h"

#if USE_EXACT

#include "Data/predicates.h"

namespace Data
{
	template <typename T>
	class Exact
	{
	friend
		class ExactFraction<T>;

	public:

		Exact()
		{
			this->initialize();
		};

		Exact(T t)
		{
			this->initialize();

			this->resize(1);

			this->values[0] = t;
		};

		Exact(UInt size, T t = T())
		{
			this->initialize();

			this->resize(size);

			for (UInt i = 0; i < this->size(); i++)
			{
				this->values[i] = t;
			}
		};

		Exact(const Exact<T> &et)
		{
			this->initialize();

			this->resize(et.size());

			for (UInt i = 0; i < this->size(); i++)
			{
				this->values[i] = et.values[i];
			}
		};

#if USE_C__11
		Exact(Exact<T> &&et)
		{
			this->initialize();

			this->swap(et);
		};
#endif //#if USE_C__11

		virtual ~Exact()
		{
#if USE_ARRAY
			delete [] this->values;
#endif //#if USE_ARRAY
		};

		virtual inline UInt size() const
		{
#if USE_ARRAY
			return static_cast<UInt>(this->length);
#else
			return static_cast<UInt>(this->values.size());
#endif //#if USE_ARRAY
		};

		//arithmetic operators

		virtual inline const Exact<T> &operator=(const Exact<T> &et)
		{
			if (this != &et)
			{
#if USE_ARRAY
				if (this->length != et.length)
				{
					delete [] this->values;

					this->length = et.length;

					this->values = new T[this->length];
				}

				//for (UShort i = 0; i < this->length; i++)
				for (UInt i = 0; i < this->length; i++)
				{
					this->values[i] = et.values[i];
				}
#else
				this->values = et.values;
#endif //#if USE_ARRAY
			}

			return *this;
		};

#if USE_C__11
		virtual inline const Exact<T> &operator=(Exact<T> &&et)
		{
			if (this != &et)
			{
                this->swap(et);
			}

			return *this;
		};
#endif //#if USE_C__11

		virtual inline const Exact<T> &operator=(T t)
		{
			if (this->size() != 1)
			{
#if USE_ARRAY
				delete [] this->values;

				this->length = 1;

				this->values = new T[this->length];
#else
				this->values.resize(1);
#endif //#if USE_ARRAY
			}

			this->values[0] = t;

			return *this;
		};

		virtual inline const Exact<T> operator+(const Exact<T> &et) const
		{
			Exact<T> result(this->size() + et.size());

#if USE_FAST_SUM
	#if USE_ZERO_ELIM
			UInt size = Predicates::fast_expansion_sum_zeroelim(this->size(), &this->values[0], et.size(), &et.values[0], &result.values[0]);
	#else
			Predicates::fast_expansion_sum(this->size(), &this->values[0], et.size(), &et.values[0], &result.values[0]);
	#endif //#if USE_ZERO_ELIM
#elif USE_LINEAR_SUM
	#if USE_ZERO_ELIM
			UInt size = Predicates::linear_expansion_sum_zeroelim(this->size(), &this->values[0], et.size(), &et.values[0], &result.values[0]);
	#else
			Predicates::linear_expansion_sum(this->size(), &this->values[0], et.size(), &et.values[0], &result.values[0]);
	#endif //#if USE_ZERO_ELIM
#else
	#if USE_ZERO_ELIM
			UInt size = Predicates::expansion_sum_zeroelim2(this->size(), &this->values[0], et.size(), &et.values[0], &result.values[0]);
	#else
			Predicates::expansion_sum(this->size(), &this->values[0], et.size(), &et.values[0], &result.values[0]);
	#endif //#if USE_ZERO_ELIM
#endif //#if USE_FAST_SUM

#if USE_ZERO_ELIM
			result.resize(size);
#endif //USE_ZERO_ELIM

			return result;
		};

		virtual inline const Exact<T> operator+(T t) const
		{
			Exact<T> result(this->size() + 1);

#if USE_ZERO_ELIM
			UInt size = Predicates::grow_expansion_zeroelim(this->size(), &this->values[0], t, &result.values[0]);

			result.resize(size);
#else
			Predicates::grow_expansion(this->size(), &this->values[0], t, &result.values[0]);
#endif //#if USE_ZERO_ELIM

			return result;
		};

		virtual inline const Exact<T> operator-(const Exact<T> &et) const
		{
			Exact<T> result(this->size() + et.size());

#if USE_FAST_SUM
			Exact<T> net = -et;

	#if USE_ZERO_ELIM
			UInt size = Predicates::fast_expansion_sum_zeroelim(this->size(), &this->values[0], net.size(), &net.values[0], &result.values[0]);
	#else
			Predicates::fast_expansion_sum(this->size(), &this->values[0], net.size(), &net.values[0], &result.values[0]);
	#endif //#if USE_ZERO_ELIM
#elif USE_LINEAR_SUM
			Exact<T> net = -et;

	#if USE_ZERO_ELIM
			UInt size = Predicates::linear_expansion_sum_zeroelim(this->size(), &this->values[0], net.size(), &net.values[0], &result.values[0]);
	#else
			Predicates::linear_expansion_sum(this->size(), &this->values[0], net.size(), &net.values[0], &result.values[0]);
	#endif //#if USE_ZERO_ELIM
#else
			//this is done because, in the expansion_sum, the first and last
			//  expansions can be the same arrays

			for (UInt i = 0; i < et.size(); i++)
			{
				result.values[i] = -et.values[i];
			}

	#if USE_ZERO_ELIM
			UInt size = Predicates::expansion_sum_zeroelim2(et.size(), &result.values[0], this->size(), &this->values[0], &result.values[0]);
	#else
			Predicates::expansion_sum(et.size(), &result.values[0], this->size(), &this->values[0], &result.values[0]);
	#endif //#if USE_ZERO_ELIM
#endif //#if USE_FAST_SUM

#if USE_ZERO_ELIM
			result.resize(size);
#endif //USE_ZERO_ELIM

			return result;
		};

		virtual inline const Exact<T> operator-(T t) const
		{
			Exact<T> result(this->size() + 1);

#if USE_ZERO_ELIM
			UInt size = Predicates::grow_expansion_zeroelim(this->size(), &this->values[0], -t, &result.values[0]);

			result.resize(size);
#else
			Predicates::grow_expansion(this->size(), &this->values[0], -t, &result.values[0]);
#endif //#if USE_ZERO_ELIM

			return result;
		};

		virtual inline const Exact<T> operator+() const
		{
			return *this;
		};

		virtual inline const Exact<T> operator-() const
		{
			Exact<T> result(this->size());

			for (UInt i = 0; i < this->size(); i++)
			{
				result.values[i] = -this->values[i];
			}

			return result;
		};

		virtual inline const Exact<T> operator*(const Exact<T> &et) const
		{
			Exact<T> results[et.size()];

			for (UInt i = 0; i < et.size(); i++)
			{
				results[i] = (*this) * (et[i]);
			}

			return this->distillation(et.size(), results);
		};

		virtual inline const Exact<T> operator*(T t) const
		{
			Exact<T> result(2*this->size());

#if USE_ZERO_ELIM
			UInt size = Predicates::scale_expansion_zeroelim(this->size(), &this->values[0], t, &result.values[0]);

			result.resize(size);
#else
			Predicates::scale_expansion(this->size(), &this->values[0], t, &result.values[0]);
#endif //#if USE_ZERO_ELIM

			return result;
		};

		/*virtual inline const Exact<T> &operator++();

		virtual inline const Exact<T> operator++(int);

		virtual inline const Exact<T> &operator--();

		virtual inline const Exact<T> operator--(int);*/

		//comparison operators

		virtual inline bool operator==(const Exact<T> &et) const
		{
			return (((*this) - et) == static_cast<T>(0.0));
		};

		virtual inline bool operator==(T t) const
		{
			if (t == static_cast<T>(0.0))
			{
				if (this->size() == 0)
				{
					return true;
				}

				bool allequal = true;

				for (UInt i = 0; i < this->size(); i++)
				{
					if (this->values[i] != static_cast<T>(0.0))
					{
						allequal = false;

						break;
					}
				}

				if (allequal)
				{
					return true;
				}

#if USE_ZERO_ELIM
				return false;
#endif //#if USE_ZERO_ELIM

				Exact<T> compressed;

				this->compress(&compressed);

				if (compressed.size() == 0)
				{
					return true;
				}

				for (UInt i = 0; i < compressed.size(); i++)
				{
					if (compressed.values[i] != static_cast<T>(0.0))
					{
						return false;
					}
				}

				return true;
			}

			return (((*this) - t) == static_cast<T>(0.0));
		};

		virtual inline bool operator!=(const Exact<T> &et) const
		{
			return !((*this) == et);
		};

		virtual inline bool operator!=(T t) const
		{
			return !((*this) == t);
		};

		virtual inline bool operator>(const Exact<T> &et) const
		{
			return (((*this) - et) > static_cast<T>(0.0));
		};

		virtual inline bool operator>(T t) const
		{
			return (t == static_cast<T>(0.0)) ? (this->sign() > 0) : (((*this) - t) > static_cast<T>(0.0));
		};

		virtual inline bool operator<(const Exact<T> &et) const
		{
			return (((*this) - et) < static_cast<T>(0.0));
		};

		virtual inline bool operator<(T t) const
		{
			return (t == static_cast<T>(0.0)) ? (this->sign() < 0) : (((*this) - t) < static_cast<T>(0.0));
		};

		virtual inline bool operator>=(const Exact<T> &et) const
		{
			return !((*this) < et);
		};

		virtual inline bool operator>=(T t) const
		{
			return !((*this) < t);
		};

		virtual inline bool operator<=(const Exact<T> &et) const
		{
			return !((*this) > et);
		};

		virtual inline bool operator<=(T t) const
		{
			return !((*this) > t);
		};

		//compound assignment operators

		virtual inline const Exact<T> &operator+=(const Exact<T> &et)
		{
#if USE_FAST_SUM
			Exact<T> result(this->size() + et.size());

	#if USE_ZERO_ELIM
			UInt size = Predicates::fast_expansion_sum_zeroelim(this->size(), &this->values[0], et.size(), &et.values[0], &result.values[0]);
	#else
			Predicates::fast_expansion_sum(this->size(), &this->values[0], et.size(), &et.values[0], &result.values[0]);
	#endif //#if USE_ZERO_ELIM

			this->swap(result);
#elif USE_LINEAR_SUM
			Exact<T> result(this->size() + et.size());

	#if USE_ZERO_ELIM
			UInt size = Predicates::linear_expansion_sum_zeroelim(this->size(), &this->values[0], et.size(), &et.values[0], &result.values[0]);
	#else
			Predicates::linear_expansion_sum(this->size(), &this->values[0], et.size(), &et.values[0], &result.values[0]);
	#endif //#if USE_ZERO_ELIM

			this->swap(result);
#else
			//this is done because, in the expansion_sum, the first and last
			//  expansions can be the same arrays

			UInt size = this->size();

			this->resize(size + et.size());

	#if USE_ZERO_ELIM
			size = Predicates::expansion_sum_zeroelim2(size, &this->values[0], et.size(), &et.values[0], &this->values[0]);
	#else
			Predicates::expansion_sum(size, &this->values[0], et.size(), &et.values[0], &this->values[0]);
	#endif //#if USE_ZERO_ELIM
#endif //#if USE_FAST_SUM

#if USE_ZERO_ELIM
			this->resize(size);
#endif //USE_ZERO_ELIM

			return *this;
		};

		virtual inline const Exact<T> &operator+=(T t)
		{
			UInt size = this->size();

			this->resize(size + 1);

#if USE_ZERO_ELIM
			size = Predicates::grow_expansion_zeroelim(size, &this->values[0], t, &this->values[0]);

			this->resize(size);
#else
			Predicates::grow_expansion(size, &this->values[0], t, &this->values[0]);
#endif //#if USE_ZERO_ELIM

			return *this;
		};

		virtual inline const Exact<T> &operator-=(const Exact<T> &et)
		{
			Exact<T> net = -et;

#if USE_FAST_SUM
			Exact<T> result(this->size() + et.size());

	#if USE_ZERO_ELIM
			UInt size = Predicates::fast_expansion_sum_zeroelim(this->size(), &this->values[0], net.size(), &net.values[0], &result.values[0]);
	#else
			Predicates::fast_expansion_sum(this->size(), &this->values[0], net.size(), &net.values[0], &result.values[0]);
	#endif //#if USE_ZERO_ELIM

			this->swap(result);
#elif USE_LINEAR_SUM
			Exact<T> result(this->size() + et.size());

	#if USE_ZERO_ELIM
			UInt size = Predicates::linear_expansion_sum_zeroelim(this->size(), &this->values[0], net.size(), &net.values[0], &result.values[0]);
	#else
			Predicates::linear_expansion_sum(this->size(), &this->values[0], net.size(), &net.values[0], &result.values[0]);
	#endif //#if USE_ZERO_ELIM

			this->swap(result);
#else
			//this is done because, in the exapansion_sum, the first and last
			//  expansions can be the same arrays

			UInt size = this->size();

			this->resize(size + et.size());

	#if USE_ZERO_ELIM
			size = Predicates::expansion_sum_zeroelim2(size, &this->values[0], net.size(), &net.values[0], &this->values[0]);
	#else
			Predicates::expansion_sum(size, &this->values[0], net.size(), &net.values[0], &this->values[0]);
	#endif //#if USE_ZERO_ELIM
#endif //#if USE_FAST_SUM

#if USE_ZERO_ELIM
			this->resize(size);
#endif //USE_ZERO_ELIM

			return *this;
		};

		virtual inline const Exact<T> &operator-=(T t)
		{
			UInt size = this->size();

			this->resize(size + 1);

#if USE_ZERO_ELIM
			size = Predicates::grow_expansion_zeroelim(size, &this->values[0], -t, &this->values[0]);

			this->resize(size);
#else
			size = Predicates::grow_expansion(size, &this->values[0], -t, &this->values[0]);
#endif //#if USE_ZERO_ELIM

			return *this;
		};

		virtual inline const Exact<T> &operator*=(const Exact<T> &et)
		{
			Exact<T> result = (*this) * (et);

			this->swap(result);

			return *this;
		};

		virtual inline const Exact<T> &operator*=(T t)
		{
			Exact<T> result(2*this->size());

#if USE_ZERO_ELIM
			UInt size = Predicates::scale_expansion_zeroelim(this->size(), &this->values[0], t, &result.values[0]);

			result.resize(size);
#else
			Predicates::scale_expansion(this->size(), &this->values[0], t, &result.values[0]);
#endif //#if USE_ZERO_ELIM

			this->swap(result);

			return *this;
		};

		//member and pointer operators

		virtual inline const T &operator[](Int i) const
		{
			return this->values[i];
		};

		virtual inline const T &operator[](UInt i) const
		{
			return this->values[i];
		};

		//other operators
		virtual inline operator T() const
		{
			T t(0.0);

			for (UInt i = 0; i < this->size(); i++)
            {
                t += this->values[i];
            }

			return t;
		};

		friend Exact<T> operator+(T t, const Exact<T> &et)
		{
			return et + t;
		};

		friend Exact<T> operator-(T t, const Exact<T> &et)
		{
			return -et + t;
		};

		friend Exact<T> operator*(T t, const Exact<T> &et)
		{
			return et * t;
		};

		friend bool operator==(T t, const Exact<T> &et)
		{
			return et == t;
		};

		friend bool operator!=(T t, const Exact<T> &et)
		{
			return et != t;
		};

		friend bool operator>(T t, const Exact<T> &et)
		{
			return et < t;
		};

		friend bool operator<(T t, const Exact<T> &et)
		{
			return et > t;
		};

		friend bool operator>=(T t, const Exact<T> &et)
		{
			return et <= t;
		};

		friend bool operator<=(T t, const Exact<T> &et)
		{
			return et >= t;
		};

		friend std::ostream &operator<<(std::ostream &stream, const Exact<T> &et)
		{
			stream << et.text();

			return stream;
		};

		//methods
		virtual inline std::string text() const
		{
			std::stringstream stream;

			stream << this->size();

			for (UInt i = this->size(); i > 0; i--)
			{
				stream << " " << this->values[i - 1];
			}

			return stream.str();
		}

		virtual inline Int sign() const
		{
			UInt s = this->size();

			T t = (s == 0) ? T() : this->values[s - 1];

			return (t > 0) ? 1 : ((t < 0) ? -1 : 0);
		};

		virtual inline T estimate() const
		{
			return Predicates::estimate(this->size(), &this->values[0]);
		};

		virtual inline void compress()
		{
			this->compress(this);
		};

		virtual inline void compress(Exact<T> *compressed) const
		{
			compressed->resize(this->size());

			UInt size = Predicates::pcompress(this->size(), &this->values[0], &compressed->values[0]);

			compressed->resize(size);
		};

		virtual inline void initialize()
		{
#if USE_ARRAY
			this->length = 0;
			this->values = NULL;
#endif //#if USE_ARRAY
		};

		static void init()
		{
			Predicates::exactinit();
		};

		static Exact<T> distillation(UInt size, Exact<T> ets[])
		{
			if (size == 0)
			{
				return Exact<T>();
			}

			if (size == 1)
			{
				return ets[0];
			}

			if (size == 2)
			{
				return ets[0] + ets[1];
			}

			UInt newsize = size/2;
			UInt offset = size%2;

			for (UInt i = 0; i < newsize; i++)
			{
				ets[i] = ets[2*i] + ets[2*i + 1];
			}

			if (offset == 1)
			{
				ets[newsize] = ets[size - 1];
			}

			return distillation(newsize + offset, ets);
		};

	protected:

		virtual inline void resize(UInt length)
		{
#if USE_ARRAY
			if (length != this->length)
			{
				T *values = this->values;

				this->values = (length == 0) ? NULL : new T[length];

				UInt min = std::min(length, static_cast<UInt>(this->length));

				if (values)
				{
					for (UInt i = 0; i < min; i++)
					{
						this->values[i] = values[i];
					}

					delete [] values;
				}

				for (UInt i = min; i < length; i++)
				{
					this->values[i] = T();
				}

				//this->length = static_cast<UShort>(length);
				this->length = static_cast<UInt>(length);
			}
#else
			this->values.resize(length, T());
#endif //#if USE_ARRAY
		};

		virtual inline void swap(Exact<T> &et)
		{
#if USE_ARRAY
			//UShort tmplength = this->length;
			UInt tmplength = this->length;
			T *tmpvalues = this->values;

			this->length = et.length;
			this->values = et.values;

			et.length = tmplength;
			et.values = tmpvalues;
#else
			this->values.swap(et.values);
#endif //#if USE_ARRAY
		};

	protected:

#if USE_ARRAY
		//UShort length;
		UInt length;
		T *values;
#else
		typedef TVector std::vector<T>;

		TVector values;
#endif //#if USE_ARRAY
	};

	template <typename T>
	class ExactFraction
	{
	public:

		ExactFraction()
		{
			this->num = Exact<T>(0.0);
			this->den = Exact<T>(1.0);
		};

		ExactFraction(T eft)
		{
			this->num = eft;
			this->den = Exact<T>(1.0);
		};

		ExactFraction(T eftnum, T eftden)
		{
			this->num = eftnum;
			this->den = eftden;

			this->adjust();
		};

		ExactFraction(const Exact<T> &eft)
		{
			this->num = eft;
			this->den = Exact<T>(1.0);
		};

#if USE_C__11
		ExactFraction(Exact<T> &&eft)
		{
			this->num.swap(eft);
			this->den = Exact<T>(1.0);
		};
#endif //#if USE_C__11

		ExactFraction(const Exact<T> &eftnum, const Exact<T> &eftden)
		{
			this->num = eftnum;
			this->den = eftden;

			this->adjust();
		};

#if USE_C__11
		ExactFraction(const Exact<T> &eftnum, Exact<T> &&eftden)
		{
			this->num = eftnum;
			this->den.swap(eftden);

			this->adjust();
		};

		ExactFraction(Exact<T> &&eftnum, const Exact<T> &eftden)
		{
			this->num.swap(eftnum);
			this->den = eftden;

			this->adjust();
		};

		ExactFraction(Exact<T> &&eftnum, Exact<T> &&eftden)
		{
			this->num.swap(eftnum);
			this->den.swap(eftden);

			this->adjust();
		};
#endif //#if USE_C__11

		ExactFraction(const ExactFraction<T> &eft)
		{
			this->num = eft.num;
			this->den = eft.den;
		};

#if USE_C__11
		ExactFraction(ExactFraction<T> &&eft)
		{
			this->num.swap(eft.num);
			this->den.swap(eft.den);
		};
#endif //#if USE_C__11

		virtual ~ExactFraction()
		{

		};

		virtual inline void numerator(const Exact<T> &num)
		{
			this->num = num;
		};

		virtual inline const Exact<T> &numerator() const
		{
			return this->num;
		};

		virtual inline void denominator(const Exact<T> &den)
		{
			this->den = den;

			this->adjust();
		};

		virtual inline const Exact<T> &denominator() const
		{
			return this->den;
		};

		virtual inline void adjust()
		{
			if (this->den < static_cast<T>(0.0))
			{
				this->num = -this->num;
				this->den = -this->den;
			}
		}

		//arithmetic operators

		virtual inline const ExactFraction<T> &operator=(const ExactFraction<T> &eft)
		{
			if (this != &eft)
			{
				this->num = eft.num;
				this->den = eft.den;
			}

			return *this;
		};

#if USE_C__11
		virtual inline const ExactFraction<T> &operator=(ExactFraction<T> &&eft)
		{
			if (this != &eft)
			{
				this->swap(eft);
			}

			return *this;
		};
#endif //#if USE_C__11

		virtual inline const ExactFraction<T> &operator=(const Exact<T> &eft)
		{
			this->num = eft;
			this->den = Exact<T>(1.0);

			return *this;
		};

#if USE_C__11
		virtual inline const ExactFraction<T> &operator=(Exact<T> &&eft)
		{
			this->num.swap(eft);
			this->den = Exact<T>(1.0);

			return *this;
		};
#endif //#if USE_C__11

		virtual inline const ExactFraction<T> &operator=(T eft)
		{
			this->num = eft;
			this->den = Exact<T>(1.0);

			return *this;
		};

		virtual inline const ExactFraction<T> operator+(const ExactFraction<T> &eft) const
		{
			ExactFraction result;

			result.num = this->num*eft.den + eft.num*this->den;
			result.den = this->den*eft.den;

			return result;
		};

		virtual inline const ExactFraction<T> operator+(const Exact<T> &eft) const
		{
			ExactFraction<T> result = *this;

			result.num += result.den*eft;

			return result;
		};

		virtual inline const ExactFraction<T> operator+(T eft) const
		{
			ExactFraction<T> result = *this;

			result.num += result.den*eft;

			return result;
		};

		virtual inline const ExactFraction<T> operator-(const ExactFraction<T> &eft) const
		{
			ExactFraction result;

			result.num = this->num*eft.den - eft.num*this->den;
			result.den = this->den*eft.den;

			return result;
		};

		virtual inline const ExactFraction<T> operator-(const Exact<T> &eft) const
		{
			ExactFraction<T> result = *this;

			result.num -= result.den*eft;

			return result;
		};

		virtual inline const ExactFraction<T> operator-(T eft) const
		{
			ExactFraction<T> result = *this;

			result.num -= result.den*eft;

			return result;
		};

		virtual inline const ExactFraction<T> operator+() const
		{
			return *this;
		};

		virtual inline const ExactFraction<T> operator-() const
		{
			ExactFraction<T> result(*this);

			result.num = -result.num;

			return result;
		};

		virtual inline const ExactFraction<T> operator*(const ExactFraction<T> &eft) const
		{
			ExactFraction<T> result = *this;

			result.num *= eft.num;
			result.den *= eft.den;

			return result;
		};

		virtual inline const ExactFraction<T> operator*(const Exact<T> &eft) const
		{
			ExactFraction<T> result = *this;

			result.num *= eft;

			return result;
		};

		virtual inline const Exact<T> operator*(T eft) const
		{
			ExactFraction<T> result = *this;

			result.num *= eft;

			return result;
		};

		virtual inline const ExactFraction<T> operator/(const ExactFraction<T> &eft) const
		{
			ExactFraction<T> result = *this;

			result.num *= eft.den;
			result.den *= eft.num;

			result.adjust();

			return result;
		};

		virtual inline const ExactFraction<T> operator/(const Exact<T> &eft) const
		{
			ExactFraction<T> result = *this;

			result.den *= eft;

			result.adjust();

			return result;
		};

		virtual inline const Exact<T> operator/(T eft) const
		{
			ExactFraction<T> result = *this;

			result.den *= eft;

			result.adjust();

			return result;
		};

		/*virtual inline const Exact<T> &operator++();

		virtual inline const Exact<T> operator++(int);

		virtual inline const Exact<T> &operator--();

		virtual inline const Exact<T> operator--(int);*/

		//comparison operators

		virtual inline bool operator==(const ExactFraction<T> &eft) const
		{
			return (this->num*eft.den == eft.num*this->den);
		};

		virtual inline bool operator==(const Exact<T> &eft) const
		{
			return (this->num == eft*this->den);
		};

		virtual inline bool operator==(T eft) const
		{
			return (this->num == eft*this->den);
		};

		virtual inline bool operator!=(const ExactFraction<T> &eft) const
		{
			return !((*this) == eft);
		};

		virtual inline bool operator!=(const Exact<T> &eft) const
		{
			return !((*this) == eft);
		};

		virtual inline bool operator!=(T eft) const
		{
			return !((*this) == eft);
		};

		virtual inline bool operator>(const ExactFraction<T> &eft) const
		{
			return (this->num*eft.den > eft.num*this->den);
		};

		virtual inline bool operator>(const Exact<T> &eft) const
		{
			return (this->num > eft*this->den);
		};

		virtual inline bool operator>(T eft) const
		{
			return (this->num > eft*this->den);
		};

		virtual inline bool operator<(const ExactFraction<T> &eft) const
		{
			return (this->num*eft.den < eft.num*this->den);
		};

		virtual inline bool operator<(const Exact<T> &eft) const
		{
			return (this->num < eft*this->den);
		};

		virtual inline bool operator<(T eft) const
		{
			return (this->num < eft*this->den);
		};

		virtual inline bool operator>=(const ExactFraction<T> &eft) const
		{
			return !((*this) < eft);
		};

		virtual inline bool operator>=(const Exact<T> &eft) const
		{
			return !((*this) < eft);
		};

		virtual inline bool operator>=(T eft) const
		{
			return !((*this) < eft);
		};

		virtual inline bool operator<=(const ExactFraction<T> &eft) const
		{
			return !((*this) > eft);
		};

		virtual inline bool operator<=(const Exact<T> &eft) const
		{
			return !((*this) > eft);
		};

		virtual inline bool operator<=(T eft) const
		{
			return !((*this) > eft);
		};

		//compound assignment operators

		virtual inline const ExactFraction<T> &operator+=(const ExactFraction<T> &eft)
		{
			this->num = this->num*eft.den + this->den*eft.num;
			this->den *= eft.den;

			return *this;
		};

		virtual inline const ExactFraction<T> &operator+=(const Exact<T> &eft)
		{
			this->num += this->den*eft;

			return *this;
		};

		virtual inline const ExactFraction<T> &operator+=(T eft)
		{
			this->num += this->den*eft;

			return *this;
		};

		virtual inline const ExactFraction<T> &operator-=(const ExactFraction<T> &eft)
		{
			this->num = this->num*eft.den - this->den*eft.num;
			this->den *= eft.den;

			return *this;
		};

		virtual inline const ExactFraction<T> &operator-=(const Exact<T> &eft)
		{
			this->num -= this->den*eft;

			return *this;
		};

		virtual inline const ExactFraction<T> &operator-=(T eft)
		{
			this->num -= this->den*eft;

			return *this;
		};

		virtual inline const ExactFraction<T> &operator*=(const ExactFraction<T> &eft)
		{
			this->num *= eft.num;
			this->den *= eft.den;

			return *this;
		};

		virtual inline const ExactFraction<T> &operator*=(const Exact<T> &eft)
		{
			this->num *= eft;

			return *this;
		};

		virtual inline const ExactFraction<T> &operator*=(T eft)
		{
			this->num *= eft;

			return *this;
		};

		virtual inline const ExactFraction<T> &operator/=(const ExactFraction<T> &eft)
		{
			this->num *= eft.den;
			this->den *= eft.num;

			this->adjust();

			return *this;
		};

		virtual inline const ExactFraction<T> &operator/=(const Exact<T> &eft)
		{
			this->den *= eft;

			this->adjust();

			return *this;
		};

		virtual inline const ExactFraction<T> &operator/=(T eft)
		{
			this->den *= eft;

			this->adjust();

			return *this;
		};

		//other operators
		virtual inline operator T() const
		{
			return T(this->operator Exact<T>());
		};

		virtual inline operator Exact<T>() const
		{
			return this->num*(T(1.0)/static_cast<T>(this->den));
		};

		friend ExactFraction<T> operator+(T t, const ExactFraction<T> &eft)
		{
			return eft + t;
		};

		friend ExactFraction<T> operator+(const Exact<T> &t, const ExactFraction<T> &eft)
		{
			return eft + t;
		};

		friend ExactFraction<T> operator-(T t, const ExactFraction<T> &eft)
		{
			return -eft + t;
		};

		friend ExactFraction<T> operator-(const Exact<T> &t, const ExactFraction<T> &eft)
		{
			return -eft + t;
		};

		friend ExactFraction<T> operator*(T t, const ExactFraction<T> &eft)
		{
			return eft * t;
		};

		friend ExactFraction<T> operator*(const Exact<T> &t, const ExactFraction<T> &eft)
		{
			return eft * t;
		};

		friend ExactFraction<T> operator/(T t, const ExactFraction<T> &eft)
		{
			return eft.inversion() * t;
		};

		friend ExactFraction<T> operator/(const Exact<T> &t, const ExactFraction<T> &eft)
		{
			return eft.inversion() * t;
		};

		friend bool operator==(T t, const ExactFraction<T> &eft)
		{
			return eft == t;
		};

		friend bool operator==(const Exact<T> &t, const ExactFraction<T> &eft)
		{
			return eft == t;
		};

		friend bool operator!=(T t, const ExactFraction<T> &eft)
		{
			return eft != t;
		};

		friend bool operator!=(const Exact<T> &t, const ExactFraction<T> &eft)
		{
			return eft != t;
		};

		friend bool operator>(T t, const ExactFraction<T> &eft)
		{
			return eft < t;
		};

		friend bool operator>(const Exact<T> &t, const ExactFraction<T> &eft)
		{
			return eft < t;
		};

		friend bool operator<(T t, const ExactFraction<T> &eft)
		{
			return eft > t;
		};

		friend bool operator<(const Exact<T> &t, const ExactFraction<T> &eft)
		{
			return eft > t;
		};

		friend bool operator>=(T t, const ExactFraction<T> &eft)
		{
			return eft <= t;
		};

		friend bool operator>=(const Exact<T> &t, const ExactFraction<T> &eft)
		{
			return eft <= t;
		};

		friend bool operator<=(T t, const ExactFraction<T> &eft)
		{
			return eft >= t;
		};

		friend bool operator<=(const Exact<T> &t, const ExactFraction<T> &eft)
		{
			return eft >= t;
		};

		friend std::ostream &operator<<(std::ostream &stream, const ExactFraction<T> &eft)
		{
			stream << eft.text();

			return stream;
		};

		//methods
		virtual inline std::string text() const
		{
			std::stringstream stream;

			stream << this->num << " " << this->den;

			return stream.str();
		}

		virtual inline Int sign() const
		{
			return (this->num.sign()*this->den.sign());
		};

		virtual inline ExactFraction<T> inversion() const
		{
			return ExactFraction(this->den, this->num);
		};

		virtual inline T estimate() const
		{
			return (this->num.estimate()/this->den.estimate());
		};

		virtual inline void compress()
		{
			this->num.compress();
			this->den.compress();
		};

		virtual inline void swap(ExactFraction<T> &eft)
		{
			this->num.swap(eft.num);
			this->den.swap(eft.den);
		};

	protected:

		Exact<T> num;
		Exact<T> den;
	};

	typedef Exact<Real> EReal;
	typedef Exact<LReal> ELReal;

	typedef ExactFraction<Real> EFReal;
	typedef ExactFraction<LReal> EFLReal;

	typedef std::vector<EReal> ERealVector;
	typedef std::vector<EFReal> EFRealVector;
}

#else

namespace Data
{
	typedef Real EReal;
	typedef LReal ELReal;

	typedef std::vector<EReal> ERealVector;
}

#endif //#if USE_EXACT

#endif //#ifndef _DATA_EXACT_H_
