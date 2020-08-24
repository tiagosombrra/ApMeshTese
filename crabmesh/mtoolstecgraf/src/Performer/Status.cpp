#include "Performer/Status.h"

using namespace Performer;

Performer::Status::Status(UInt type, const std::string &message)
{
	this->setType(type);
	this->setMessage(message);
}

Performer::Status::~Status()
{

}

void Performer::Status::setType(UInt type)
{
	this->type = type;
}

UInt Performer::Status::getType() const
{
	return this->type;
}

void Performer::Status::setMessage(const std::string &message)
{
	this->message = message;
}

void Performer::Status::setMessage(const Status &s)
{
	this->setMessage(s.getMessage());
}

const std::string &Performer::Status::getMessage() const
{
	return this->message;
}

std::string Performer::Status::eliminateEqual(std::string s1, std::string s2) const
{
	std::stringstream str1, str2;

	str1 << s1;
	str2 << s2;

	while ((!str1.eof()) && (!str2.eof()))
	{
		str1 >> s1;
		str2 >> s2;

		if (s1 != s2)
		{
			break;
		}

		size_t size1 = (str1.str().size() > s1.size()) ? s1.size() + 1 : s1.size();
		size_t size2 = (str2.str().size() > s2.size()) ? s2.size() + 1 : s2.size();

		str1.str(str1.str().substr(size1));
		str2.str(str2.str().substr(size2));
	}

	return str2.str();
}

void Performer::Status::prepend(const std::string &message)
{
	std::stringstream str;

	str << message << " " << this->eliminateEqual(message, this->message);

	this->message = str.str();
}

void Performer::Status::prepend(const Status &s)
{
	this->prepend(s.getMessage());
}

void Performer::Status::append(const std::string &message)
{
	std::stringstream str;

	str << this->message << " " << this->eliminateEqual(this->message, message);

	this->message = str.str();
}

void Performer::Status::append(const Status &s)
{
	this->append(s.getMessage());
}

void Performer::Status::print(std::ostream str) const
{
	str << this->message << std::endl;
}

void Performer::Status::print() const
{
	std::cout << this->message << std::endl;
}

bool Performer::Status::operator==(const Status &s) const
{
	return ((*this) == s.type);
}

bool Performer::Status::operator!=(const Status &s) const
{
	return ((*this) != s.type);
}

bool Performer::Status::operator<(const Status &s) const
{
	return ((*this) < s.type);
}

bool Performer::Status::operator>(const Status &s) const
{
	return ((*this) > s.type);
}

bool Performer::Status::operator<=(const Status &s) const
{
	return ((*this) <= s.type);
}

bool Performer::Status::operator>=(const Status &s) const
{
	return ((*this) >= s.type);
}

bool Performer::Status::operator==(UInt type) const
{
	return (this->type == type);
}

bool Performer::Status::operator!=(UInt type) const
{
	return (this->type != type);
}

bool Performer::Status::operator<(UInt type) const
{
	return (this->type < type);
}

bool Performer::Status::operator>(UInt type) const
{
	return (this->type > type);
}

bool Performer::Status::operator<=(UInt type) const
{
	return (this->type <= type);
}

bool Performer::Status::operator>=(UInt type) const
{
	return (this->type >= type);
}
