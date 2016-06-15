/* Created by Junior on 6/14/2016.
 *
 * Very similar to QCString, except all copies are deep.
 * Memory is only resized if an assignment requires more space,
 * or if resize() or truncate() is called.
 *
*/
#ifndef NSTRING_H
#define NSTRING_H
#include <stream.h>
typedef unsigned int uint;
typedef unsigned short ushort;


class NCString
{
public:
	NCString();
	NCString(int size);
	NCString(const char *data);
	NCString(const char *data, uint maxlen);
	NCString(const NCString &toCopy);
	~NCString();

	operator const char *() const;
	NCString&	operator  = (const NCString &in);
	NCString&	operator  = (const char *in);
	NCString&	operator += (const NCString &in);
	NCString&	operator += (const char *in);
	NCString&	operator += (char in);
	char&		operator [] (int i) const;

	char&		at(uint i) const;
	bool		isNull() const;
	bool		isEmpty() const;
	uint		length() const;
	bool		resize(uint len);
	bool		truncate(uint len);
	bool		fill(char c, int len = -1);
	NCString&	sprintf(const char* format, ...);
	int			find(char c, int index = 0, bool cs = true) const;
	int			find(const char *str, int index = 0, bool cs = true) const;
	int			findRev(char c, int index = -1, bool cs = true) const;
	int			findRev(const char *str, int index = -1, bool cs = true) const;
	int			contains(char c, bool cs) const;
	int			contains(const char *str, bool cs) const;
	NCString	left(uint len) const;
	NCString	right(uint len) const;
	NCString	mid(uint index, uint len=0xffffffff) const;
	NCString	leftJustify(uint width, char fill=' ', bool trunc=false) const;
	NCString	rightJustify(uint width, char fill=' ', bool trunc=false) const;
	NCString	lower() const;
	NCString	upper() const;
	NCString	simplifyWhiteSpace() const;
	NCString	stripWhiteSpace() const;

	NCString&	insert(uint index, const char *str);
	NCString&	insert(uint index, char c);
	NCString&	append(const char* str);
	NCString&	prepend(const char* str);
	NCString&	remove(uint index, uint len);
	NCString&	replace(uint index, uint len, const char *str);

	static short		toShort (const char *str, bool *ok=0);
	static ushort		toUShort(const char *str, bool *ok=0);
	static int			toInt   (const char *str, bool *ok=0);
	static uint			toUInt  (const char *str, bool *ok=0);
	static int			toLong  (const char *str, bool *ok=0);
	static uint			toULong (const char *str, bool *ok=0);
	static float		toFloat (const char *str, bool *ok=0);
	static double		toDouble(const char *str, bool *ok=0);
	short		toShort (bool *ok=0) const;
	ushort		toUShort(bool *ok=0) const;
	int			toInt   (bool *ok=0) const;
	uint		toUInt  (bool *ok=0) const;
	int			toLong  (bool *ok=0) const;
	uint		toULong (bool *ok=0) const;
	float		toFloat (bool *ok=0) const;
	double		toDouble(bool *ok=0) const;

	NCString&	setStr(const char *s);
	NCString&	setNum(short);
	NCString&	setNum(ushort);
	NCString&	setNum(int);
	NCString&	setNum(uint);
	NCString&	setNum(float, char f='g', int prec=6);
	NCString&	setNum(double, char f='g', int prec=6);

	bool		setExpand(uint index, char c);

	NCString	split(char separator);
	NCString	splitCSV(char separator);

	int		size()	{ return dsize; }

private:
	uint		dsize;
	char*		ddata;

	void deleteData();
	void set(const char *dataIn);
	bool minsize(uint len) { if(len+1 > dsize) return resize(len+1); return true; }
};

/** compare @relates NCString */	bool operator != (const NCString &s1, const char     *s2);
/** compare @relates NCString */	bool operator != (const char     *s1, const NCString &s2);
/** compare @relates NCString */	bool operator != (const NCString &s1, const NCString &s2);
/** compare @relates NCString */	bool operator == (const NCString &s1, const char     *s2);
/** compare @relates NCString */	bool operator == (const char     *s1, const NCString &s2);
/** compare @relates NCString */	bool operator == (const NCString &s1, const NCString &s2);
/** compare @relates NCString */	bool operator <  (const NCString &s1, const char	 *s2);
/** compare @relates NCString */	bool operator <  (const char	 *s1, const NCString &s2);
/** compare @relates NCString */	bool operator <  (const NCString &s1, const NCString &s2);
/** compare @relates NCString */	bool operator >  (const NCString &s1, const char	 *s2);
/** compare @relates NCString */	bool operator >  (const char	 *s1, const NCString &s2);
/** compare @relates NCString */	bool operator >  (const NCString &s1, const NCString &s2);
/** compare @relates NCString */	bool operator <= (const NCString &s1, const char	 *s2);
/** compare @relates NCString */	bool operator <= (const char	 *s1, const NCString &s2);
/** compare @relates NCString */	bool operator <= (const NCString &s1, const NCString &s2);
/** compare @relates NCString */	bool operator >= (const NCString &s1, const char	 *s2);
/** compare @relates NCString */	bool operator >= (const char	 *s1, const NCString &s2);
/** compare @relates NCString */	bool operator >= (const NCString &s1, const NCString &s2);
/** add     @relates NCString */	NCString operator + (const NCString &s1, char c);
/** add     @relates NCString */	NCString operator + (char c, const NCString &s1);
/** add     @relates NCString */	NCString operator + (const NCString &s1, const NCString &s2);
/** add     @relates NCString */	NCString operator + (const char     *s1, const NCString &s2);
/** add     @relates NCString */	NCString operator + (const NCString &s1, const char     *s2);
/** stream  @relates NCString */	ostream& operator << (ostream &os, const NCString &s1);

#endif //NSTRING_H
