#ifndef _PERFORMER_STATUS_H_
#define _PERFORMER_STATUS_H_

#include "Performer/Definitions.h"

namespace Performer
{
	class Status
	{
	public:

		Status(UInt type = 0, const std::string &message = "success");
		virtual ~Status();

		virtual void setType(UInt type);
		virtual UInt getType() const;

		virtual void setMessage(const std::string &message);
		virtual void setMessage(const Status &s);
		virtual const std::string &getMessage() const;

		virtual void prepend(const std::string &message);
		virtual void prepend(const Status &s);
		virtual void append(const std::string &message);
		virtual void append(const Status &s);

		virtual void print(std::ostream str) const;
		virtual void print() const;

		virtual bool operator==(const Status &s) const;
		virtual bool operator!=(const Status &s) const;
		virtual bool operator<(const Status &s) const;
		virtual bool operator>(const Status &s) const;
		virtual bool operator<=(const Status &s) const;
		virtual bool operator>=(const Status &s) const;

		virtual bool operator==(UInt type) const;
		virtual bool operator!=(UInt type) const;
		virtual bool operator<(UInt type) const;
		virtual bool operator>(UInt type) const;
		virtual bool operator<=(UInt type) const;
		virtual bool operator>=(UInt type) const;

	protected:

		virtual std::string eliminateEqual(std::string s1, std::string s2) const;

	protected:

		UInt type;

		std::string message;
	};
}

#endif //#ifndef _PERFORMER_STATUS_H_
