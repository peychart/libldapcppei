# include <libldapcppei/ldapByteArea.h>

namespace ldapcppei
{
// ******************************************** (Cons/Des)tructors:
byteArea::byteArea( void )			: m_area(nullptr), m_count(0), m_sizeOf(0) {}
byteArea::byteArea( char const *s )		: m_area(nullptr), m_count(0), m_sizeOf(0) {newArea( stringLen(s), (byte const *)s );}
byteArea::byteArea( std::string const &s )	: m_area(nullptr), m_count(0), m_sizeOf(0) {newArea( s.size(), (byte const *)s.c_str() );}
byteArea::byteArea( size_t l, byte const *b )	: m_area(nullptr), m_count(0), m_sizeOf(0) {newArea( l, b );}
byteArea::byteArea( byteArea const *a )		: m_area(nullptr), m_count(0), m_sizeOf(0) {if (a != nullptr) newArea( a->size(), a->m_area );};
byteArea::byteArea( byteArea const &a )		: m_area(nullptr), m_count(0), m_sizeOf(0) {newArea( a.size(), a.m_area );};

byteArea::~byteArea( void ) {clear();}
// ****************************************************************
bool	byteArea::newArea( size_t len, byte const *area, size_t newSizeOf ) {
	if ( isNULL() )
		setNULL();
	else{	newSizeOf = niceSize( newSizeOf ? newSizeOf : len );
		byte*	newArea;
		if ( newSizeOf && (newArea=new byte[newSizeOf])!=nullptr ) {
			if ( area != nullptr ) {
				memoryCopy( newArea, area, len ); newArea[len]='\0';
			} clear();
			m_area=newArea; m_sizeOf=newSizeOf; if ( area != nullptr ) m_count=len;
			return true;
	}	}
	return false;
}

byteArea&	byteArea::insert( size_t len, byte const *v, size_t pos ) {
	if ( len && (v != nullptr) ) {
		size_t		newCount = size() + len;
		if ( pos > size() ) pos = size();
		if ( sizeOf() <= newCount && ! newArea( size(), area(), newCount ) )
			return *this;
		rmemoryCopy( &m_area[size()+len], &m_area[size()], size()-pos+1 );
		memoryCopy( &m_area[pos], v, len ); m_area[(m_count+=len)]='\0';
	} return *this;
}

size_t	byteArea::indexOf( size_t l, byte const *b, size_t from ) const {
	if ( b == nullptr )
		return( size() ? (size_t)(-1) : 0 );
	if ( size() < from )	from = size();
	if ( l <= size()-from ) for ( size_t i=from, n=size()-from; i < n; )
		if ( byteAreaCompare ( l, &m_area[i++], l, b ) == 0 )
			return (i-1);
	return (size_t)(-1);
}

size_t	byteArea::rindexOf( size_t l, byte const *b, size_t from ) const {
	if ( b == nullptr )
		return( size() ? (size_t)(-1) : 0 );
	if ( size() < from )	from = size();
	if ( l <= from ) for ( size_t i=size(); i > 0; )
		if ( byteAreaCompare ( l, &m_area[--i], l, b ) == 0 )
			return (i);
	return (size_t)(-1);
}

int	byteArea::byteAreaCompare( size_t l1, byte const *p1, size_t l2, byte const *p2 ) {
	if ( !l2 || !p2 ) return ( (!l1 || !p1) ? 0 : 1 ); if ( !l1 || !p1 ) return ( -1 );
	size_t n = ((l1 > l2) ? l1 : l2);
	while ( n && *p1 == *p2 ) {n--; p1++; p2++;}
	return( n ? ((*p1 < *p2) ? -1 : 1) : 0 );
}

int	byteArea::byteAreaEvaluate( size_t l1, byte const *p1, size_t l2, byte const *p2 ) {
	if ( !l2 || !p2 ) return ( (!l1 || !p1) ? 0 : 1 ); if ( !l1 || !p1 ) return ( -1 );
	size_t n = ((l1 > l2) ? l1 : l2);
	while ( n && upper(*p1) == upper(*p2) ) {n--; p1++; p2++;}
	return ( n ? ((upper(*p1) < upper(*p2)) ? -1 : 1) : 0 );
}

byteArea	byteArea::trimmed( byte const c ) const {
	byteArea	ret( *this );
	while ( ret.isSeparator( 0, c ) )
		ret.fastErase( 0, 1 );
	while ( ret.isSeparator( ret.size()-1, c ) )
		if ( ret.size()==1 || ret.data()[ret.size()-2] != '\\' )
			ret.fastErase( ret.size()-1, 1 );
		else	break;
	return ret;
}

byteArea	byteArea::simplified( void ) const {
	byteArea	ret( this->trimmed() );
	for ( byte const *p = &ret.area()[1], *e = &ret.area()[ret.size()-1]; p<e; )
		if ( *p == '\\' ) {
			p++; p++;
		}else if ( ret.isSep( p++ ) && ret.isSep( p ) ) {
			ret.fastErase( p-- - ret.area(), 1 );
			e = &ret.area()[ret.size()-1];
		}
	return ret;
}

byteArea byteArea::toUtf8 ( void ) const {
	byteArea	ret;
	for ( size_t i=0; i<size(); i++ )
		if ( (area()[i] & 0x80) ) {
			ret += (byte)( (area()[i]>>6 & 0x1F) | 0xC0 );
			ret += (byte)( (area()[i]    & 0x3F) | 0x80 );
		}else	ret += (byte)( area()[i] );
	return ret;
}

byteArea byteArea::fromUtf8 ( void ) const {
	byteArea	ret;
	for ( size_t i=0; i<size(); i++ )
		if ( (area()[i] & 0x80) ) {
			ret += (byte)( ((area()[i] & 0x1F) << 6) | (area()[i+1] & 0x3F) ); i++;
		}else	ret += (byte)( area()[i] );
	return ret;
}

void	byteArea::encode64block( unsigned char *in, unsigned char *out, int len ) const {
	out[0] = (unsigned char) cb64[ (int)(in[0] >> 2) ];
	out[1] = (unsigned char) cb64[ (int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)) ];
	out[2] = (unsigned char) (len > 1 ? cb64[ (int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)) ] : '=');
	out[3] = (unsigned char) (len > 2 ? cb64[ (int)(in[2] & 0x3f) ] : '=');
}

void	byteArea::decode64block( unsigned char in[4], unsigned char out[3] ) const {
	out[0] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
	out[1] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
	out[2] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

unsigned char	byteArea::getcForDecode64 ( size_t& i ) const {
	while ( i < size() && (at(i) == '=' || at(i) == '\r') )
		i++;
	if ( i < size() && at(i) == '\n' ) {
		if ( at(i) == ' ' || at(i) == '\t')
			i++;
		else    i = size();
	} return( i<size() ? at(i++) : 0 );
}

byteArea	byteArea::decode64( void ) const {
	unsigned char	in[4], out[3], v;
        byteArea	ret;

	for ( size_t pt(0); pt < size(); ) {
		size_t  len(0);
		for ( size_t i(0); i < 4 && pt < size(); i++ ) {
			for ( v=0; pt < size() && v==0; ) {
				if ( (v = getcForDecode64( pt )) ) {
					v = ((v < 43 || v > 122) ? 0 : cd64[v - 43]);
					if ( v )
						v = ((v == '$') ? 0 : v - 61);
			}	}
			if ( v ) {
				in[i] = v - 1;
				len++;
			}else	in[i] = 0;
		}
		if ( len ) {
			decode64block( in, out );
			for ( size_t i(0); i < len-1; i++ ) {
				ret += out[i];
		}	}
	} return ret;
}

byteArea	byteArea::encode64( void ) const {
	unsigned char	in[3], out[4];
	byteArea	ret;

	for ( size_t i(0); i < size(); ) {
		size_t  len(0);
		for ( size_t j(0); j < 3; j++ ) {
			if ( i < size() ) {
				in[j] = at(i++);
				len++;
			}else	in[j] = 0;
		}
		if ( len ) {
			encode64block( in, out, len );
			for ( size_t j=0; j < 4; j++ ) {
				ret += out[j];
	}	}	}
	return ret;
}

bool	byteArea::mustBeEncoded64( void ) const {
	if ( size() && (at(0)=='<' || at(0)==':' || at(0)==' ' || at( size()-1 )==' ' )  )
		return true;
	for ( size_t i=0; i < size(); i++ )
		if ( at(i) < ' ' || at(i) > '~' )
			return true;
	return false;
}

}	// End namespace ldapcppei

