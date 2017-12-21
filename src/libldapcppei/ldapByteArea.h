#ifndef HEADER_7B61DE6C9F72B392
#define HEADER_7B61DE6C9F72B392

/*           libcppei (Version 3.1.4 - 2014/06)
    <http://homepage.gna.org/ldapcppei/miniHowto.fr.html>

    Copyright (C) 2010  -  Philippe EYCHART
				<philippe.eychart@mail.pf>

    This program is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public
    License along with this program.
    If not, see <http://www.gnu.org/licenses/>.

    Details of this licence are available online under:
			http://www.gnu.org/licenses/gpl-3.0.html
*/
#include <iostream>
#include <string>

namespace ldapcppei
{typedef unsigned char byte;
 class byteArea {
    private:
	class byteRef {
	    public:
	    	byteRef	( void ) : m_ptr(nullptr) {};
	    	byteRef	( byte* v ) : m_ptr(v) {};
		inline byte&		operator* () const					{return *m_ptr;};
		inline byte*		operator->() const					{return m_ptr;};
		inline byte		operator= ( byte const & v ) const			{return( m_ptr != nullptr ? *m_ptr = v : (byte)0 );};
		inline byte		operator==( byte const & v ) const			{return( m_ptr != nullptr ? *m_ptr == v : false );};
		inline byte		operator!=( byte const & v ) const			{return( ! operator==(v) );};
		inline byte		operator>=( byte const & v ) const			{return( m_ptr != nullptr ? *m_ptr >= v : true );};
		inline byte		operator> ( byte const & v ) const			{return( m_ptr != nullptr ? *m_ptr >  v : true );};
		inline byte		operator<=( byte const & v ) const			{return( m_ptr != nullptr ? *m_ptr <= v : true );};
		inline byte		operator< ( byte const & v ) const			{return( m_ptr != nullptr ? *m_ptr <  v : true );};
		inline bool		operator++()						{return m_ptr++;};
		inline bool		operator++( int )					{return ++m_ptr;};
		friend std::ostream&	operator<<( std::ostream& out, byteRef const v )	{return( out << v );};
	    private:
		byte	*m_ptr;
	};

	char const *			cb64="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	char const *			cd64="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";
	void				encode64block	( unsigned char*, unsigned char*, int len ) const;
	void				decode64block	( unsigned char[4], unsigned char[3] ) const;
	unsigned char			getcForDecode64	( size_t& ) const;

    public:
	 byteArea( void );
	 byteArea( char const * );
	 byteArea( std::string const & );
	 byteArea( size_t, byte const * );
	 byteArea( byteArea const * );
	 byteArea( byteArea const & );

	~byteArea( void );

	inline byteArea&	clear		( void )						{fill( (byte)0, size() ); delete[] m_area; m_sizeOf=m_count=0; m_area=nullptr; return *this;};
	inline byteArea&	erase		( size_t from=0, size_t len=(size_t)(-1) )		{if (from < size()) fastErase( from, (size() < from+len) ? size()-from : len<size()-from?len:size()-from ); return *this;};
	inline byteArea&	eraseAll	( byteArea v, size_t from=0, size_t len=(size_t)(-1) )	{for (size_t n=indexOf(v, from); n<len; n=indexOf(v, from)) fastErase( n, v.size() ); return *this;};
	inline byteArea&	setNULL		( void )						{clear(); m_count--; return *this;};
	inline bool		isNULL		( void )						{return( m_count==(size_t)(-1) );};

	inline byte		pop_back	( void )						{byte ret=(size() ? area()[size()-1] : (byte const)0); erase( size()-1 ); return ret;};

	inline byteArea&	insert		( char const &b, size_t from=0 )			{return insert( 1, (byte const *)&b, from );};
	inline byteArea&	insert		( byte const &b, size_t from=0 )			{return insert( 1, (byte const *)&b, from );};
	inline byteArea&	insert		( char const *s, size_t from=0 )			{return insert( stringLen(s), (byte const *)s, from );};
	inline byteArea&	insert		( std::string const s, size_t from=0 )			{return insert( s.size(), (byte const *)s.c_str(), from );};
	byteArea&		insert		( size_t, byte const *, size_t = 0 );
	inline byteArea&	insert		( byteArea const *a, size_t from=0 )			{return( a==nullptr ? *this : insert( a->size(), a->area(), from ) );};
	inline byteArea&	insert		( byteArea const &a, size_t from=0 )			{return insert( a.size(), a.area(), from );};

	template<typename T> inline byteArea&
				append		( T const v )						{return insert( v, size() );};
	inline byteArea&	append		( size_t len, byte const *b )				{return insert( len, b, size() );};
	inline byteArea&	push_back	( char const c )					{return append( 1, (byte const *)&c );};
	inline byteArea&	push_back	( byte const c )					{return append( 1, (byte const *)&c );};

	template<typename T> inline byteArea&
				assign		( T const v )						{return erase( 0, size() ).append( v );};
	inline byteArea&	assign		( size_t len, byte const *b )				{return erase( 0, size() ).append( len, b );};

	inline byteArea&	fill		( byte const with, size_t from=0, size_t len=(size_t)(-1) )
													{byte *p; if (from>size()) from=size(); p=&m_area[from]; if (len>size()-from) len=size()-from; while (len--) *p++ = with; return *this;};
	inline byteArea&	fill		( char const with, size_t from=0, size_t len=(size_t)(-1) )
													{return fill( (byte)with, from, len );};

	inline byteArea&	replace		( size_t from, size_t len, size_t l, byte const *b )	{if (len>size()-from) len=size()-from; return erase(from, len).insert(l, b, from);};
	template<typename T> inline byteArea&
				replace		( size_t from, size_t len, T v )			{if (len>size()-from) len=size()-from; return erase(from, len).insert(v, from);};

	inline byte		at		( size_t n ) const					{return( area()[n] );};
	inline byte		front		( void ) const						{return( size() ? area()[0] : (byte const)0 );};
	inline byte		back		( void ) const						{return( size() ? area()[size()-1] : (byte const)0 );};
	inline bool		isSeparator	( size_t pos, byte const sep=(byte)0 )			{return( pos<size() && isSep( &area()[pos], sep ) );};
	inline bool		isSeparator	( size_t pos, byte const *sep )				{return( pos<size() && ( sep==nullptr ? isSep( &area()[pos] ) : fastStrCompare( &data()[pos], (char const *)sep )==0 ) );};

	template<typename T> inline int
				compare		( T const v ) const					{return byteAreaCompare( size(), area(), v );};
	inline int		compare		( size_t len, byte const *b ) const			{return byteAreaCompare( size(), area(), len, b );};

	template<typename T> inline int
				stringEvaluate	( T const v ) const					{return byteAreaEvaluate( size(), area(), v );};
	inline int		stringEvaluate	( size_t len, byte const *b ) const			{return byteAreaEvaluate( size(), area(), len, b );};

	inline size_t		indexOf		( char const &b, size_t from=0 ) const			{return indexOf( 1, (byte const *)&b, from );};
	inline size_t		indexOf		( byte const &b, size_t from=0 ) const			{return indexOf( 1, &b, from );};
	inline size_t		indexOf		( char const *s, size_t from=0 ) const			{return( s==nullptr ? !empty() : indexOf( stringLen(s), (byte const *)s, from ) );};
	inline size_t		indexOf		( std::string const s, size_t from=0 ) const		{return indexOf( s.size(), (byte const *)s.c_str(), from );};
	size_t			indexOf		( size_t, byte const *, size_t = 0 ) const;
	inline size_t		indexOf		( byteArea const *a, size_t from=0 ) const		{return( a==nullptr ? !empty() : indexOf( a->size(), a->area(), from ) );};
	inline size_t		indexOf		( byteArea const &a, size_t from=0 ) const		{return indexOf( a.size(), a.area(), from );};

	inline size_t		rindexOf	( char const &b, size_t from=(size_t)(-1) ) const	{return rindexOf( 1, (byte const *)&b, from );};
	inline size_t		rindexOf	( byte const &b, size_t from=(size_t)(-1) ) const	{return rindexOf( 1, &b, from );};
	inline size_t		rindexOf	( char const *s, size_t from=(size_t)(-1) ) const	{return( s==nullptr ? !empty() : rindexOf( stringLen(s), (byte const *)s, from ) );};
	inline size_t		rindexOf	( std::string const s, size_t from=(size_t)(-1) ) const	{return rindexOf( s.size(), (byte const *)s.c_str(), from );};
	size_t			rindexOf	( size_t, byte const *, size_t = (size_t)(-1) ) const;
	inline size_t		rindexOf	( byteArea const *a, size_t from=(size_t)(-1) ) const	{return( a==nullptr ? from==size() : rindexOf( a->size(), a->area(), from ) );};
	inline size_t		rindexOf	( byteArea const &a, size_t from=(size_t)(-1) ) const	{return rindexOf( a.size(), a.area(), from );};

	template<typename T> inline bool
				contains	( T const v, size_t from=0 ) const			{return( indexOf( v, from )	!= (size_t)(-1) );};
	inline bool		contains	( size_t l, byte const *b, size_t from=0 ) const	{return( indexOf( l, b, from )	!= (size_t)(-1) );};
	template<typename T> inline bool
				startsWith	( T const v ) const					{return( indexOf( v )	 == 0 );};
	inline bool		startsWith	( size_t l, byte const *b ) const			{return( indexOf( l, b ) == 0 );};
	template<typename T> inline bool
				endsWith	( T const v ) const					{return( indexOf( v )	 == size() - v.size() );};
	inline bool		endsWith	( size_t l, byte const *b ) const			{return( indexOf( l, b ) == size() - l );};

	inline byteArea		subByteArea	( size_t from=0, size_t len=(size_t)-1 ) const		{byteArea ret; if (from > size()) from=size(); ret.assign( (len <= size()-from)?len:(size()-from), &area()[from] ); return ret;};
	inline byteArea		subValue	( size_t from=0, size_t len=(size_t)-1 ) const		{return subByteArea( from, len );};

	byteArea		trimmed		( byte const c='\0' ) const;
	byteArea		simplified	( void ) const;

	inline char const*	data		( void ) const						{return( (char const *)m_area );};
	inline char const*	c_str		( void ) const						{return( size() ? data() : "" );};
	inline byte const*	area		( void ) const						{return  m_area;};

	inline byteArea		toUpper		( size_t from=0, size_t len= -1 ) const			{byteArea ret; ret.assign( *this ); if (from<size()) for( byte *a=&(ret.m_area)[from],*b=&(ret.m_area)[size()]; a!=b && len--; a++ ) *a = (byte)upper(*(char const *)a); return ret;};
	inline byteArea		toLower		( size_t from=0, size_t len= -1 ) const			{byteArea ret; ret.assign( *this ); if (from<size()) for( byte *a=&(ret.m_area)[from],*b=&(ret.m_area)[size()]; a!=b && len--; a++ ) *a = (byte)lower(*(char const *)a); return ret;};
	inline std::string	toStdString	( void ) const						{std::string ret( (char const *)area() ); return ret;};

	byteArea		toUtf8		( void ) const;
	byteArea		fromUtf8	( void ) const;
	bool			mustBeEncoded64	( void ) const;
	byteArea		encode64	( void ) const;
	byteArea		decode64	( void ) const;

	inline size_t		size		( void ) const						{return  m_count;};		// bytes count
	inline bool		empty		( void ) const						{return( !size() );};		// is there any value?
	inline byteArea&	resize		( void )						{newArea( size(), area() ); return *this;};

	inline byte		operator[]	( size_t i ) const					{return( (i < size()) ? m_area[i] : 0 );};
	inline byteRef		operator[]	( size_t i )						{return( (i < size()) ? &m_area[i] : nullptr );};
	inline byte		operator[]	( byteArea v ) const					{size_t i=indexOf( v ); return( (size() && i<size()) ? m_area[i] : 0 );};
	inline byteRef		operator[]	( byteArea v )						{size_t i=indexOf( v ); return( (size() && i<size()) ? &m_area[i] : nullptr );};

	template<typename T> inline bool
				operator==	( T const v ) const					{return( compare( v ) == 0 );};
	template<typename T> inline bool
				operator<=	( T const v ) const					{return( compare( v ) <= 0 );};
	template<typename T> inline bool
				operator<	( T const v ) const					{return( compare( v ) <  0 );};
	template<typename T> inline bool
				operator>	( T const v ) const					{return( compare( v ) >  0 );};
	template<typename T> inline bool
				operator>=	( T const v ) const					{return( compare( v ) >= 0 );};
	template<typename T> inline bool
				operator!=	( T const v ) const					{return( compare( v ) != 0 );};

	template<typename T> inline byteArea const &
				operator=	( T const v )						{return assign( v );};
	template<typename T> inline byteArea const &
				operator+=	( T const v )						{return append( v );};
	template<typename T> inline byteArea const
				operator+	( T const v ) const					{return byteArea(this).append( v );};
	template<typename T> inline byteArea const &
				operator-=	( T const v ) const					{return erase( indexOf(v), v.size() );};
	template<typename T> inline byteArea const
				operator-	( T const v ) const					{return byteArea(this).eraseAll( v );};
	template<typename T> inline byteArea&
				operator&=	( T const v ) const					{return( compare( v ) ? byteArea() : *this );};
	template<typename T> inline byteArea&
				operator|=	( T const v ) const					{return( indexOf( v )!=(size_t)(-1) ? byteArea() : *this );};

	friend std::ostream&	operator<<	( std::ostream& out, byteArea const *v )		{return( out << ( (v==nullptr) ? "" : v->c_str() ) );};
	friend std::ostream&	operator<<	( std::ostream& out, byteArea const &v )		{return( out << v.c_str() );};

    protected:
	bool			newArea		( size_t, byte const * = nullptr, size_t = 0 );
	inline size_t		sizeOf		( void )						{return m_sizeOf;};
	inline static void	rmemoryCopy	( byte *a, byte const *from, size_t l )			{while (l--) *(a--) = *from--;};
	inline static void	memoryCopy	( byte *a, byte const *from, size_t l )			{while (l--) *a++ = *from++;};
	inline static void	memorySet	( byte *a, byte const with, size_t l )			{while (l--) *a++ = with;};
	inline static size_t	stringLen	( char const *s )					{size_t ret=0; if(s) while( *s++ ) ret++; return ret;};
	inline static size_t	stringCompare	( char const *s1, char const *s2 )			{return( (s1==nullptr || s2==nullptr) ? (s1==s2) : fastStrCompare( s1, s2 ) );};

	static int		byteAreaCompare	( size_t, byte const *, size_t, byte const * );
	inline static int	byteAreaCompare	( size_t l, byte const *b, byteArea const *a )		{return( a==nullptr ? a->area()==nullptr : byteAreaCompare(  l, b, a->size(), a->area() ) );};
	inline static int	byteAreaCompare	( size_t l, byte const *b, byteArea const &a )		{return( byteAreaCompare( l, b, a.size(), a.area() ) );};

	static int		byteAreaEvaluate( size_t, byte const *, size_t, byte const * );
	inline static int	byteAreaEvaluate( size_t l, byte const *b, byteArea const *a )		{return( a==nullptr ? a->area()==nullptr : byteAreaEvaluate( l, b, a->size(), a->area() ) );};
	inline static int	byteAreaEvaluate( size_t l, byte const *b, byteArea const &a )		{return( byteAreaEvaluate( l, b, a.size(), a.area() ) );};

    public:
	inline static char	upper		( unsigned char c )					{return( ('a'<=c) ? ( (c<='z') ? (c-32) : c ) : c );};
	inline static char	lower		( unsigned char c )					{return( ('A'<=c) ? ( (c<='Z') ? (c+32) : c ) : c );};

    private:
	byte*			m_area;
	size_t			m_count, m_sizeOf;

	inline size_t		niceSize	( size_t size )						{size_t ret(2); if (++size < ret) ret=0; else while( (size=size >> 1) ) ret=ret << 1; return ret;};
	inline void		fastErase	( size_t from, size_t len )				{m_count -=len; memoryCopy( &m_area[from], &m_area[from+len], size()-from ); memorySet( &m_area[size()], 0, len );};
	inline static bool	fastStrCompare	( char const *s1, char const *s2 )			{for (; *s1; s1++,s2++ ) if (*s1 != *s2) return false; return(*s1 == *s2);};
	inline bool		isSep		( byte const *c, byte sep='\0'  )			{return( sep ? *c==sep : *c==' ' || *c=='\t' || *c=='\n' || *c=='\v' || *c=='\f' || *c=='\r' );};
  };
}	// End namespace ldapcppei

#endif // header guard
