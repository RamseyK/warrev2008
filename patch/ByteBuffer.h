
#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

//#include "Sockets.h"
#include "ByteB.h"
#include <string>
#include <stdlib.h>

using std::string;


class ByteBuffer {
    public:
        uint8 retries;
        class error {
        };

        const static size_t DEFAULT_SIZE = 0x1000;

        ByteBuffer(): _rpos(0), _wpos(0), retries(0) {
            _storage.reserve(DEFAULT_SIZE);
        }
        ByteBuffer(size_t res): _rpos(0), _wpos(0), retries(0) {
            _storage.reserve(res);
        }
        ByteBuffer(const ByteBuffer &buf): _sequence(buf._sequence), _rpos(buf._rpos), _wpos(buf._wpos), _storage(buf._storage), retries(0) { }

        void clear() {
            _storage.clear();
            _rpos = _wpos = 0;
        }

        //template <typename T> void insert(size_t pos, T value) {
        //  insert(pos, (uint8 *)&value, sizeof(value));
        //}
        template <typename T> void append(T value) {
            append((uint8 *)&value, sizeof(value));
        }
        template <typename T> void put(size_t pos,T value) {
            put(pos,(uint8 *)&value,sizeof(value));
        }

        // stream like operators for storing data
        ByteBuffer &operator<<(bool value) {
            append<char>((char)value);
            return *this;
        }
        ByteBuffer &operator<<(uint8 value) {
            append<uint8>(value);
            return *this;
        }
        ByteBuffer &operator<<(char value) {
            append<char>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint16 value) {
            append<uint16>(value);
            return *this;
        }

        ByteBuffer &operator<<(short value) {
            append<short>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint32 value) {
            append<uint32>(value);
            return *this;
        }
        ByteBuffer &operator<<(int32 value) { 
            append<int32>(value); 
            return *this; 
        }
        ByteBuffer &operator<<(int value) {
            append<int>(value);
            return *this;
        }
        ByteBuffer &operator<<(uint64 value) {
            append<uint64>(value);
            return *this;
        }
        /* 
           ByteBuffer &operator<<(unicode value) {
           append<unicode>(value);
           return *this;
           }
           */
        ByteBuffer &operator<<(float value) {
            append<float>(value);
            return *this;
        }
        ByteBuffer &operator<<(double value) {
            append<double>(value);
            return *this;
        }
        ByteBuffer &operator<<(const std::string &value) {
            append((uint8 *)value.c_str(), value.length());
            //append((uint8)0);
            return *this;
        }
        ByteBuffer &operator<<(const char *str) {
            append((uint8 *)str, strlen(str));
            //append((uint8)0);
            return *this;
        }

        // stream like operators for reading data
        ByteBuffer &operator>>(bool &value) {
            value = read<char>() > 0 ? true : false;
            return *this;
        }
        ByteBuffer &operator>>(uint8 &value) {
            value = read<uint8>();
            return *this;
        }
        ByteBuffer &operator>>(uint16 &value) {
            value = read<uint16>();
            return *this;
        }
        ByteBuffer &operator>>(uint32 &value) {
            value = read<uint32>();
            return *this;
        }
        ByteBuffer &operator>>(uint64 &value) {
            value = read<uint64>();
            return *this;
        }
        ByteBuffer &operator>>(float &value) {
            value = read<float>();
            return *this;
        }
        ByteBuffer &operator>>(double &value) {
            value = read<double>();
            return *this;
        }
        ByteBuffer &operator>>(std::string& value) {
            value.clear();
            while (true) {
                char c=read<char>();
                if (c==0)
                    break;
                value+=c;
            }
            return *this;
        }

        uint8 operator[](size_t pos) {
            return read<uint8>(pos);
        }


        size_t rpos() {
            return _rpos;
        };

        size_t rpos(size_t rpos) {
            _rpos = rpos;
            return _rpos;
        };

        size_t wpos() {
            return _wpos;
        }

        size_t wpos(size_t wpos) {
            _wpos = wpos;
            return _wpos;
        }

        template <typename T> T read() {
            T r=read<T>(_rpos);
            _rpos += sizeof(T);
            return r;
        }
        template <typename T> T read(size_t pos) const {
            assert(pos + sizeof(T) <= size());
            return *((T*)&_storage[pos]);
        }
        /*
           void read(unicode *dest, size_t len) {
           if (_rpos + len <= size()) {
           for(int i=0; i < len; i++)  dest[i] = (unicode)&_storage[(_rpos + i)];
//wmemcpy(dest, &_storage[_rpos], len);
} else {
throw error();
}
_rpos += len;
}
*/
void read(uint8 *dest, size_t len) {
    if (_rpos + len <= size()) {
        memcpy(dest, &_storage[_rpos], len);
    } else {
        throw error();
    }
    _rpos += len;
}

uint8 *read(size_t len) {
    uint8 *Temp = new uint8[len];
    if (_rpos + len <= size()) {
        memcpy(Temp, &_storage[_rpos], len);
    } else {
        throw error();
    }
    _rpos += len;
    return Temp;
}

char *contents() const { return (char*)&_storage[0]; };

inline std::vector<uint8>::size_type size() const { return _storage.size(); };
// one should never use resize probably
void resize(size_t newsize) {
    _storage.resize(newsize);
    _rpos = 0;
    _wpos = size();
};
void reserve(size_t ressize) {
    if (ressize > size()) _storage.reserve(ressize);
};

// appending to the end of buffer
void append(const std::string& str) {
    append((uint8 *)str.c_str(), str.size());
}
void append(const char *src, size_t cnt) {
    return append((const uint8 *)src, cnt);
}
void append(const uint8 *src, size_t cnt) {
    if (!cnt) return;

    // noone should even need uint8buffer longer than 10mb
    // if you DO need, think about it
    // then think some more
    // then use something else
    // -- qz
    assert(size() < 10000000);

    if (_storage.size() < _wpos + cnt)
        _storage.resize(_wpos + cnt);
    memcpy(&_storage[_wpos], src, cnt);
    _wpos += cnt;
}
void append(const ByteBuffer& buffer) {
    if(buffer.size() > 0) append(buffer.contents(),buffer.size());
}

void put(size_t pos, const uint8 *src, size_t cnt) {
    assert(pos + cnt <= size());
    memcpy(&_storage[pos], src, cnt);
}
void opcode(uint16 opcode) {
    append<uint16>(opcode);
}
uint16 opcode() {
    return read<uint16>();
}
void sequence(uint16 seq) {
    _sequence = seq;
    seq = htons(seq);
    append<uint16>(seq);
}
uint16 sequence() {
    return _sequence;

}
void opcode2(uint32 opcode) {
    append<uint32>(opcode);
}
uint32 opcode2() {
    return read<uint32>();
}
//void insert(size_t pos, const uint8 *src, size_t cnt) {
//  std::copy(src, src + cnt, inserter(_storage, _storage.begin() + pos));
//}

string packetstring()
{
	string s = "";
	rpos(0);
	bool b = false;
	int j = 0;
	for(int i=0; i < (int)size();i++)
	{		
		uint8 c = _storage[i];
		string t = itoh(c);
		s = s + t + " ";		
		if(j == 15)
		{
			s = s + "\n";
			j=0;
		}
		else
		{
			j++;
		}
	}
	return s;
}

private:
uint16 _sequence;

string itoh(uint8 n) 
{
	union 
	{              
    unsigned short num;
    unsigned char s[sizeof(short)];
	};    
  const char *hex = "0123456789abcdef";
  num = n;              // store number as a short
  string sS; 
  sS = sS + hex[s[0] >> 4];       // second byte - high nibble
  sS = sS + hex[s[0] & 15];       // second byte - low nibble
  return sS;  
}

protected:
// read and write positions
size_t _rpos, _wpos;
std::vector<uint8> _storage;
};

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

template <typename T> ByteBuffer &operator<<(ByteBuffer &b, std::vector<T> v)
{
    b << (uint32)v.size();
    for (typename std::vector<T>::iterator i = v.begin(); i != v.end(); i++) {
        b << *i;
    }
    return b;
}

template <typename T> ByteBuffer &operator>>(ByteBuffer &b, std::vector<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while(vsize--) {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename T> ByteBuffer &operator<<(ByteBuffer &b, std::list<T> v)
{
    b << (uint32)v.size();
    for (typename std::list<T>::iterator i = v.begin(); i != v.end(); i++) {
        b << *i;
    }
    return b;
}

template <typename T> ByteBuffer &operator>>(ByteBuffer &b, std::list<T> &v)
{
    uint32 vsize;
    b >> vsize;
    v.clear();
    while(vsize--) {
        T t;
        b >> t;
        v.push_back(t);
    }
    return b;
}

template <typename K, typename V> ByteBuffer &operator<<(ByteBuffer &b, std::map<K, V> &m)
{
    b << (uint32)m.size();
    for (typename std::map<K, V>::iterator i = m.begin(); i != m.end(); i++) {
        b << i->first << i->second;
    }
    return b;
}

template <typename K, typename V> ByteBuffer &operator>>(ByteBuffer &b, std::map<K, V> &m)
{
    uint32 msize;
    b >> msize;
    m.clear();
    while(msize--) {
        K k;
        V v;
        b >> k >> v;
        m.insert(make_pair(k, v));
    }
    return b;
}

#endif

// vim:set expandtab shiftwidth=4 softtabstop=4:
