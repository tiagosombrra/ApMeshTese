#ifndef _DATA_OSTREAM_H_
#define _DATA_OSTREAM_H_

#include "Data/Definitions.h"

namespace Data
{
    class TableStream
    {
    public:

        TableStream(Int width = -1, Int precision = -1);
        ~TableStream();

        Int width() const;
        Int width(Int width);

        Int precision() const;
        Int precision(Int precision);

        TableStream &operator<<(bool val);
        TableStream &operator<<(char val);
        TableStream &operator<<(unsigned char val);
        TableStream &operator<<(short val);
        TableStream &operator<<(unsigned short val);
        TableStream &operator<<(int val);
        TableStream &operator<<(unsigned int val);
        TableStream &operator<<(long val);
        TableStream &operator<<(unsigned long val);
        TableStream &operator<<(long long val);
        TableStream &operator<<(unsigned long long val);
        TableStream &operator<<(float val);
        TableStream &operator<<(double val);
        TableStream &operator<<(long double val);

        TableStream &operator<<(const char *val);

        TableStream &operator<<(const std::string &sb);
        TableStream &operator<<(const std::stringstream &sb);
        TableStream &operator<<(const std::streambuf *sb);

        TableStream &endl();

        std::string str();

    protected:

        enum Type
        {
            UNKNOWN = 0,
            BOOL,
            CHAR,
            UCHAR,
            SHORT,
            USHORT,
            INT,
            UINT,
            LONG,
            ULONG,
            LONG_LONG,
            ULONG_LONG,
            FLOAT,
            DOUBLE,
            LONG_DOUBLE,
            STRING,
            LINE_BREAK
        };

        union Un
        {
            Un();
            ~Un();

            bool b;
            char c;
            unsigned char uc;
            short s;
            unsigned short us;
            int i;
            unsigned int ui;
            long l;
            unsigned long ul;
            long long ll;
            unsigned long long ull;
            float f;
            double d;
            long double ld;
            std::string st;
        };

        struct Item
        {
            Item();
            Item(const Item &item);
            ~Item();

            enum Type type;
            Un un;
        };

        typedef std::list<Item> ItemList;

    protected:

        ItemList items;

        Int w;
        Int p;
    };

    class OStream
    {
    friend
        OStream &endl(OStream &s);

    public:

        OStream();
        OStream(std::ostream &stream);
        OStream(const OStream &s);
        ~OStream();

        void setStream(std::ostream &stream);
        std::ostringstream &getBuffer();

        OStream &operator<<(bool val);
        OStream &operator<<(char val);
        OStream &operator<<(unsigned char val);
        OStream &operator<<(short val);
        OStream &operator<<(unsigned short val);
        OStream &operator<<(int val);
        OStream &operator<<(unsigned int val);
        OStream &operator<<(long val);
        OStream &operator<<(unsigned long val);
        OStream &operator<<(long long val);
        OStream &operator<<(unsigned long long val);
        OStream &operator<<(float val);
        OStream &operator<<(double val);
        OStream &operator<<(long double val);

        OStream &operator<<(void *val);
        OStream &operator<<(const void *val);
        OStream &operator<<(const char *val);

        OStream &operator<<(const std::string &sb);
        OStream &operator<<(const std::stringstream &sb);
        OStream &operator<<(const std::streambuf *sb);

        OStream &operator<<(std::ostream &(*pf)(std::ostream &));
        OStream &operator<<(std::ios &(*pf)(std::ios &));
        OStream &operator<<(std::ios_base &(*pf)(std::ios_base &));

        OStream &operator<<(const OStream &sb);
        OStream &operator<<(OStream &(*pf)(OStream &));

        OStream &operator=(const OStream &s);

        static void init();
        static void finalize();

    protected:

        std::ostream *stream;
        std::ostringstream buffer;
    };
}

#endif //#ifndef _DATA_OSTREAM_H_
