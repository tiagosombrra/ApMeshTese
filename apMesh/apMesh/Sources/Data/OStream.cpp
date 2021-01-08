#include "OStream.h"

#include <iomanip>

#if USE_OPENMP
#include <omp.h>
#endif //#if USE_OPENMP

namespace Data
{
/*#if USE_GUI*/
    static OStream **ocouts;
    static OStream **ocerrs;
    static OStream **oclogs;
    static UInt numStreams;
/*#else
    static OStream *ocout;
    static OStream *ocerr;
    static OStream *oclog;

#if USE_OPENMP
    #pragma omp threadprivate(ocout)
    #pragma omp threadprivate(ocerr)
    #pragma omp threadprivate(oclog)
#endif //#if USE_OPENMP
#endif //#if USE_GUI*/
}

using namespace Data;

Data::OStream::OStream()
{
    this->stream = NULL;
}

Data::OStream::OStream(std::ostream &stream)
{
    this->stream = &stream;
}

Data::OStream::OStream(const OStream &s)
{
    this->stream = s.stream;
    this->buffer.str(s.buffer.str());
}

Data::OStream::~OStream()
{

}

void Data::OStream::setStream(std::ostream &stream)
{
    this->stream = &stream;
}

std::ostringstream &Data::OStream::getBuffer()
{
    return this->buffer;
}

OStream &Data::OStream::operator<<(bool val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(char val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(unsigned char val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(short val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(unsigned short val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(int val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(unsigned int val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(long val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(unsigned long val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(long long val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(unsigned long long val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(float val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(double val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(long double val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(void *val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(const void *val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(const char *val)
{
    this->buffer << val;

    return *this;
}

OStream &Data::OStream::operator<<(const std::string &sb)
{
    this->buffer << sb;

    return *this;
}

OStream &Data::OStream::operator<<(const std::stringstream &sb)
{
    this->buffer << sb.str();

    return *this;
}

OStream &Data::OStream::operator<<(const std::streambuf *sb)
{
    this->buffer << sb;

    return *this;
}

OStream &Data::OStream::operator<<(std::ostream &(*pf)(std::ostream &))
{
    pf(this->buffer);

    return *this;
}

OStream &Data::OStream::operator<<(std::ios &(*pf)(std::ios &))
{
    pf(this->buffer);

    return *this;
}

OStream &Data::OStream::operator<<(std::ios_base &(*pf)(std::ios_base &))
{
    pf(this->buffer);

    return *this;
}

OStream &Data::OStream::operator<<(const OStream &sb)
{
    this->buffer << sb.buffer.str();

    return *this;
}

OStream &Data::OStream::operator<<(OStream &(*pf)(OStream &))
{
    return pf(*this);
}

OStream &Data::OStream::operator=(const OStream &s)
{
    this->stream = s.stream;
    this->buffer.str(s.buffer.str());

    return *this;
}

void Data::OStream::init()
{
/*#if USE_GUI*/
#if USE_OPENMP
    Data::numStreams = omp_get_max_threads();
#else
    Data::numStreams = 1;
#endif //#if USE_OPENMP

    Data::ocouts = new Data::OStream*[Data::numStreams];
    Data::ocerrs = new Data::OStream*[Data::numStreams];
    Data::oclogs = new Data::OStream*[Data::numStreams];

    for (UInt i = 0; i < Data::numStreams; i++)
    {
        Data::ocouts[i] = new Data::OStream(std::cout);
        Data::ocerrs[i] = new Data::OStream(std::cerr);
        Data::oclogs[i] = new Data::OStream(std::clog);
    }
/*#else
#if USE_OPENMP
    #pragma omp parallel
#endif //#if USE_OPENMP
    {
        Data::ocout = new Data::OStream(std::cout);
        Data::ocerr = new Data::OStream(std::cerr);
        Data::oclog = new Data::OStream(std::clog);

#if USE_OPENMP
        #pragma omp barrier
#endif //#if USE_OPENMP
    }
#endif //#endif //#if USE_GUI*/
}

void Data::OStream::finalize()
{
/*#if USE_GUI*/
    for (UInt i = 0; i < Data::numStreams; i++)
    {
        delete Data::ocouts[i];
        delete Data::ocerrs[i];
        delete Data::oclogs[i];
    }

    delete [] Data::ocouts;
    delete [] Data::ocerrs;
    delete [] Data::oclogs;

    Data::ocouts = NULL;
    Data::ocerrs = NULL;
    Data::oclogs = NULL;
    Data::numStreams = 0;
/*#else
#if USE_OPENMP
    #pragma omp parallel
#endif //#if USE_OPENMP
    {
        delete Data::ocout;
        delete Data::ocerr;
        delete Data::oclog;

        Data::ocout = NULL;
        Data::ocerr = NULL;
        Data::oclog = NULL;

#if USE_OPENMP
        #pragma omp barrier
#endif //#if USE_OPENMP
    }
#endif //#if USE_GUI*/
}

OStream &Data::cout()
{
/*#if USE_GUI*/
#if USE_OPENMP
    return (*Data::ocouts[omp_get_thread_num()]);
#else
    return (*Data::ocouts[0]);
#endif //#if USE_OPENMP
/*#else
    return (*Data::ocout);
#endif //#endif //#if USE_GUI*/
}

OStream &Data::cerr()
{
/*#if USE_GUI*/
#if USE_OPENMP
    return (*Data::ocerrs[omp_get_thread_num()]);
#else
    return (*Data::ocerrs[0]);
#endif //#if USE_OPENMP
/*#else
    return (*Data::ocerr);
#endif //#endif //#if USE_GUI*/
}

OStream &Data::clog()
{
/*#if USE_GUI*/
#if USE_OPENMP
    return (*Data::oclogs[omp_get_thread_num()]);
#else
    return (*Data::oclogs[0]);
#endif //#if USE_OPENMP
/*#else
    return (*Data::oclog);
#endif //#if USE_GUI*/
}

OStream &Data::endl(OStream &s)
{
    s.buffer << std::endl;
    (*s.stream) << s.buffer.str();
    s.buffer.str("");

    return s;
}

Data::TableStream::Un::Un()
{

}

Data::TableStream::Un::~Un()
{

}

Data::TableStream::Item::Item()
{

}

Data::TableStream::Item::Item(const Item &item)
{
    this->type = item.type;

    switch (this->type)
    {
    case BOOL:
        this->un.b = item.un.b;
        break;
    case CHAR:
        this->un.c = item.un.c;
        break;
    case UCHAR:
        this->un.uc = item.un.uc;
        break;
    case SHORT:
        this->un.s = item.un.s;
        break;
    case USHORT:
        this->un.us = item.un.us;
        break;
    case INT:
        this->un.i = item.un.i;
        break;
    case UINT:
        this->un.ui = item.un.ui;
        break;
    case LONG:
        this->un.l = item.un.l;
        break;
    case ULONG:
        this->un.ul = item.un.ul;
        break;
    case LONG_LONG:
        this->un.ll = item.un.ll;
        break;
    case ULONG_LONG:
        this->un.ull = item.un.ull;
        break;
    case FLOAT:
        this->un.f = item.un.f;
        break;
    case DOUBLE:
        this->un.d = item.un.d;
        break;
    case LONG_DOUBLE:
        this->un.ld = item.un.ld;
        break;
    case STRING:
        new (&this->un.st) std::string;
        this->un.st = item.un.st;
        break;
    case LINE_BREAK:
        break;
    }
}

Data::TableStream::Item::~Item()
{
    if (this->type == STRING)
    {
        this->un.st.~basic_string<char>();
    }
}

Data::TableStream::TableStream(Int width, Int precision)
{
    this->width(width);
    this->precision(precision);
}

Data::TableStream::~TableStream()
{

}

Int Data::TableStream::width() const
{
    return this->w;
}

Int Data::TableStream::width(Int width)
{
    this->w = width;
    return this->width();
}

Int Data::TableStream::precision() const
{
    return this->p;
}

Int Data::TableStream::precision(Int precision)
{
    this->p = precision;
    return this->precision();
}

TableStream &Data::TableStream::operator<<(bool val)
{
    Item item;
    item.type = TableStream::Type::BOOL;
    item.un.b = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(char val)
{
    Item item;
    item.type = TableStream::Type::CHAR;
    item.un.c = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(unsigned char val)
{
    Item item;
    item.type = TableStream::Type::UCHAR;
    item.un.uc = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(short val)
{
    Item item;
    item.type = TableStream::Type::SHORT;
    item.un.s = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(unsigned short val)
{
    Item item;
    item.type = TableStream::Type::USHORT;
    item.un.us = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(int val)
{
    Item item;
    item.type = TableStream::Type::INT;
    item.un.i = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(unsigned int val)
{
    Item item;
    item.type = TableStream::Type::UINT;
    item.un.ui = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(long val)
{
    Item item;
    item.type = TableStream::Type::LONG;
    item.un.l = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(unsigned long val)
{
    Item item;
    item.type = TableStream::Type::ULONG;
    item.un.ul = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(long long val)
{
    Item item;
    item.type = TableStream::Type::LONG_LONG;
    item.un.ll = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(unsigned long long val)
{
    Item item;
    item.type = TableStream::Type::ULONG_LONG;
    item.un.ull = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(float val)
{
    Item item;
    item.type = TableStream::Type::FLOAT;
    item.un.f = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(double val)
{
    Item item;
    item.type = TableStream::Type::DOUBLE;
    item.un.d = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(long double val)
{
    Item item;
    item.type = TableStream::Type::LONG_DOUBLE;
    item.un.ld = val;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(const char *val)
{
    Item item;
    item.type = TableStream::Type::STRING;

    std::stringstream s;

    s << val;

    new (&item.un.st) std::string;

    item.un.st = s.str();

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(const std::string &sb)
{
    Item item;
    item.type = TableStream::Type::STRING;

    new (&item.un.st) std::string;

    item.un.st = sb;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(const std::stringstream &sb)
{
    Item item;
    item.type = TableStream::Type::STRING;

    new (&item.un.st) std::string;

    item.un.st = sb.str();

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::operator<<(const std::streambuf *sb)
{
    Item item;
    item.type = TableStream::Type::STRING;

    std::stringstream s;

    s << sb;

    item.un.st = s.str();

    new (&item.un.st) std::string;

    this->items.push_back(item);

    return *this;
}

TableStream &Data::TableStream::endl()
{
    Item item;
    item.type = TableStream::Type::LINE_BREAK;

    this->items.push_back(item);

    return *this;
}

std::string Data::TableStream::str()
{
    if (this->items.empty())
    {
        return "";
    }

    UInt width = (this->w < 0) ? 0 : this->w;

    std::stringstream str;

    if (this->precision() > 0)
    {
        str.setf(std::ios::fixed, std::ios::floatfield);
        str.precision(this->precision());
    }

    for (ItemList::const_iterator iter = this->items.begin();
         iter != this->items.end(); iter++)
    {
        if ((*iter).type != TableStream::Type::STRING)
        {
            continue;
        }

        if ((*iter).un.st.length() > width)
        {
            width = (*iter).un.st.length();
        }
    }

    str.width(width);

    bool first = true;

    for (ItemList::const_iterator iter = this->items.begin();
         iter != this->items.end(); iter++)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            str << " ";
        }

        switch ((*iter).type)
        {
        case BOOL:
            str << std::setw(width) << std::left << std::boolalpha << (*iter).un.b;
            break;
        case CHAR:
            str << std::setw(width) << std::left << (*iter).un.c;
            break;
        case UCHAR:
            str << std::setw(width) << std::left << (*iter).un.uc;
            break;
        case SHORT:
            str << std::setw(width) << std::right << (*iter).un.s;
            break;
        case USHORT:
            str << std::setw(width) << std::right << (*iter).un.us;
            break;
        case INT:
            str << std::setw(width) << std::right << (*iter).un.i;
            break;
        case UINT:
            str << std::setw(width) << std::right << (*iter).un.ui;
            break;
        case LONG:
            str << std::setw(width) << std::right << (*iter).un.l;
            break;
        case ULONG:
            str << std::setw(width) << std::right << (*iter).un.ul;
            break;
        case LONG_LONG:
            str << std::setw(width) << std::right << (*iter).un.ll;
            break;
        case ULONG_LONG:
            str << std::setw(width) << std::right << (*iter).un.ull;
            break;
        case FLOAT:
            str << std::setw(width) << std::right << (*iter).un.f;
            break;
        case DOUBLE:
            str << std::setw(width) << std::right << (*iter).un.d;
            break;
        case LONG_DOUBLE:
            str << std::setw(width) << std::right << (*iter).un.ld;
            break;
        case STRING:
            str << std::setw(width) << std::left << (*iter).un.st;
            break;
        case LINE_BREAK:
            str << std::endl;
            first = true;
            break;
        }
    }

    this->items.clear();

    return str.str();
}

