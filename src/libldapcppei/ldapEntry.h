#ifndef HEADER_FB360F1A9606491
#define HEADER_FB360F1A9606491

/*           libcppei (Version 3.0.0 - 2012/08)
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
#include <libldapcppei/ldapAttribute.h>
#define LDAP_MOD_SEARCH	LDAP_MOD_BVALUES

namespace ldapcppei
{ class entry {
    public:
	 entry( void );
	 entry( char const * );
	 entry( std::string const & );
	 entry( std::string, std::string, int );
	 entry( std::string, std::string, int, char const * );
	 entry( std::string, std::string, int, std::string );
	 entry( std::string, std::string, int, BerValue const * );
	 entry( std::string, std::string, int, BerValue const & );
	 entry( std::string, std::string, int, berValue const * );
	 entry( std::string, std::string, int, berValue const & );
	 entry( std::string, std::string, int, berValue*, bool=false );

	 entry( std::string, std::string, int, char const *[] );
	 entry( std::string, std::string, int, BerValue const *[] );
	 entry( std::string, std::string, int, berValue const *[] );
	 entry( std::string, std::string, int, berValue*[], bool=false );

	 entry( std::string, LDAPMod const * );
	 entry( std::string, LDAPMod const & );
	 entry( std::string, attribute const * );
	 entry( std::string, attribute const & );
	 entry( std::string, attribute*, bool=false );

	 entry( std::string, LDAPMod const *[] );
	 entry( std::string, attribute const *[] );
	 entry( std::string, attribute*[], bool=false );

	 entry( entry const * );
	 entry( entry const & );

	~entry();

	inline entry&		identifier		( char const *name )		{if ( !isNULL() && name ) m_identifier.assign( name ); else m_identifier.clear(); return *this;};
	inline entry&		identifier		( std::string const & name )	{return identifier( name.c_str() );};		// set attribute name
	inline entry&		name			( char const *name )		{return identifier( name );};
	inline entry&		name			( std::string const &name )	{return identifier( name );};
	inline entry&		dn			( std::string const &name )	{return identifier( name );};
	inline entry&		setIdentifier		( char const *name=nullptr )	{return identifier( name );};
	inline entry&		setIdentifier		( std::string const &name )	{return identifier( name );};
	inline std::string const &identifier		( void ) const			{return m_identifier;};			// get attribute name
	inline std::string const &getIdentifier		( void ) const			{return identifier();};
	inline std::string const &name			( void ) const			{return identifier();};
	inline std::string const &dn			( void ) const			{return identifier();};
	inline unsigned int	dnSize			( void ) const			{unsigned int ret=(!isNULL() && !m_identifier.empty()?1:0); char const *pt=m_identifier.c_str(); while (*pt) if (*pt++ == ',') ret++; return ret;};

	inline bool		isDn			( berValue val ) const		{return( berValue(identifier()).stringEvaluate( val )==0 );};
	inline bool		isDnStartingWith	( berValue val ) const		{return( berValue(identifier()).startsWith( val ) );};
	inline bool		isDnContaining		( berValue val, size_t from=0 ) const
											{return( berValue(identifier()).contains( val, from ) );};
	inline bool		isDnEndingWith		( berValue val ) const		{return( berValue(identifier()).endsWith( val ) );};

	inline entry&		ldap_mod_op		( int val )			{for( auto iter : m_values ) iter->ldap_mod_op( val ); return *this;};
	inline entry&		setMod_op		( int val=LDAP_MOD_BVALUES )	{return ldap_mod_op( val );};
	inline entry&		setAddAttributes	( void )			{return setMod_op( LDAP_MOD_ADD );};
	inline entry&		setReplaceAttributes	( void )			{return setMod_op( LDAP_MOD_REPLACE );};
	inline entry&		setDeleteAttributes	( void )			{return setMod_op( LDAP_MOD_DELETE );};
	inline entry&		setSearchAttributes	( void )			{return setMod_op( LDAP_MOD_SEARCH );};

	inline entry& 		setNULL			( void )			{eraseAll(); m_identifier.assign ( "#" ); return *this;};
	inline bool		isNULL			( void ) const			{return( *m_identifier.c_str() == '#' );};

	inline entry&		clear			( void )			{setIdentifier(); eraseAll(); return *this;};
	inline entry&		eraseAll		( void )			{for( auto iter : m_values ) delete iter; m_values.clear(); m_iter = m_values.end();	return *this;};
	inline entry&		erase			( entry const *v )		{if (v) for ( auto iter : v->m_values ) find( iter ).erase(); m_iter = m_values.begin();return *this;};
	inline entry&		erase			( entry const &v )		{for( auto iter : v.m_values ) find( iter ).erase(); m_iter=m_values.begin();		return *this;};
	inline entry&		erase			( char const *v )		{return find( v ).erase();};
	inline entry&		erase			( std::string const &v )	{return find( v ).erase();};
	inline entry&		erase			( int ldapModOp )		{for (auto iter=m_values.begin(); iter != m_values.end(); ++iter) if((*iter)->ldap_mod_op()==ldapModOp) find( *iter ).erase(); m_iter=m_values.begin(); return *this;};
	inline entry&		erase			( void )			{if (m_iter != m_values.end()) {delete *m_iter; m_values.erase( m_iter++ );}; return *this;};	// Current attribute ...
	entry&			eraseNullAttributes	( void );

	template<typename T>inline entry& insert	( T const a )			{return( isNULL() ? *this : fastInsert( new attribute(a) ) );};
	template<typename T>inline entry& insert	( std::string dn, int mod, T const a )
											{return( isNULL() ? *this : fastInsert( new attribute(dn, mod, a) ) );};
	inline entry&		insert			( attribute *a, bool dl=false )	{return( isNULL() ? *this : (dl ? fastInsert(a) : fastInsert( new attribute(a) )) );};
	inline entry&		insert			( LDAPMod const *v[] )		{if (v != nullptr) for(LDAPMod const **val=v; *val; val++) insert( *val ); m_iter = m_values.begin(); return *this;};
	inline entry&		insert			( LDAPMod *v[] )		{return insert( (LDAPMod const *)v );};
	inline entry&		insert			( attribute const *v[] )	{if (v != nullptr) for(attribute const **val=v; *val; val++) insert( *val ); m_iter = m_values.begin(); return *this;};
	inline entry&		insert			( attribute *v[], bool dl=false){if (v != nullptr) for(attribute **val=v; *val; val++) insert(*val, dl); m_iter=m_values.begin(); return *this;};

	inline attribute&	insertAddAttribute	( char const *id )		{insert( new attribute(id, LDAP_MOD_ADD), true );	return *getAttribute();};
	inline attribute&	insertAddAttribute	( std::string const &id )	{insert( new attribute(id, LDAP_MOD_ADD), true );	return *getAttribute();};
	inline attribute&	insertReplaceAttribute	( char const *id )		{insert( new attribute(id, LDAP_MOD_REPLACE), true );	return *getAttribute();};
	inline attribute&	insertReplaceAttribute	( std::string const &id )	{insert( new attribute(id, LDAP_MOD_REPLACE), true );	return *getAttribute();};
	inline attribute&	insertDeleteAttribute	( char const *id )		{insert( new attribute(id, LDAP_MOD_DELETE), true );	return *getAttribute();};
	inline attribute&	insertDeleteAttribute	( std::string const &id )	{insert( new attribute(id, LDAP_MOD_DELETE), true );	return *getAttribute();};
	inline attribute&	insertSearchAttribute	( char const *id )		{insert( new attribute(id, LDAP_MOD_SEARCH), true );	return *getAttribute();};
	inline attribute&	insertSearchAttribute	( std::string const &id )	{insert( new attribute(id, LDAP_MOD_SEARCH), true );	return *getAttribute();};

	inline bool		gotoBegin		( void )			{return( (m_iter = m_values.begin()) != m_values.end());};			// Go to the first value
	inline bool		gotoFirst		( void )			{return gotoBegin();};								// Go to the first value
	inline bool		begin			( void )			{return( m_iter == m_values.begin() ); };					// Is it the first value?
	inline bool		first			( void )			{return begin();};								// Is it the first value?
	inline bool		gotoPrevious		( void )			{return( (m_iter != m_values.begin()) ? (--m_iter != m_iter) : false );};		// Go to the the next value.
	inline bool		gotoNext		( void )			{return( (m_iter != m_values.end()) ? (++m_iter != m_values.end()) : false );};	// Go to the the next value.
	inline bool		gotoEnd			( void )			{return( (m_iter  = m_values.end())  == m_values.end());};			// Go to pos behind the last value.
	inline bool		gotoLast		( void )			{m_iter = m_values.end(); return gotoPrevious();};				// Go to the last value.
	inline bool		end			( void )			{return( m_iter == m_values.end() );};						// Is current pos the last value + 1?
	inline bool		last			( void )			{std::set<attribute*>::const_iterator iter = m_iter;return( ++iter==m_values.end() );};	// Is current pos the last value?
	inline void		gotoAllTops		( void )			{m_iter = m_values.begin(); for( auto iter : m_values ) iter->gotoBegin();};

	inline entry&		find			( char const *v )		{attribute value( v ); return find( &value );};
	inline entry&		find			( std::string const &v )	{return find( v.c_str() );};
	inline entry&		find			( attribute const *v )		{m_iter=m_values.find( const_cast<attribute*>(v) ); return *this;};
	inline entry&		find			( attribute const &v )		{return find( &v );};
//	inline entry&		findStartingWith	( char const *v )		{for( m_iter=m_values.begin(); m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->isNameStartingWith( v ) ) break; return *this;};
	inline entry&		findStartingWith	( char const *v )		{attribute value( v ); m_iter = m_values.upper_bound( &value ); if ( ! (*m_iter)->isNameStartingWith( v ) ) m_iter = m_values.end(); return *this;};
	inline entry&		findStartingWith	( std::string const &v )	{return findStartingWith( v.c_str() );};
	inline entry&		findContaining		( char const *v, size_t fr=0 )	{for( m_iter=m_values.begin(); m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->isNameContaining( v, fr ) ) break; return *this;};
	inline entry&		findContaining		( std::string const &v, size_t from=0 )
											{return findContaining( v.c_str(), from );};
	inline entry&		findEndingWith		( char const *v )		{for( m_iter=m_values.begin(); m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->isNameEndingWith( v ) ) break; return *this;};
	inline entry&		findEndingWith		( std::string const &v )	{return findEndingWith( v.c_str() );};
	inline entry&		findNextStartingWith	( char const *v )		{if ( m_iter!=m_values.end() && ++m_iter!=m_values.end() && (*m_iter)->isNameStartingWith( v ) ) m_iter=m_values.end(); return *this;};
	inline entry&		findNextStartingWith	( std::string const &v )	{return findNextStartingWith( v.c_str() );};
	inline entry&		findNextContaining	( char const *v, size_t fr=0 )	{for( m_iter++; m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->isNameContaining( v, fr ) ) break; return *this;};
	inline entry&		findNextContaining	( std::string const &v, size_t from=0 ) {return findNextContaining( v.c_str() );};
	inline entry&		findNextEnding		( char const *v )		{for( m_iter++; m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->isNameEndingWith( v ) ) break; return *this;};
	inline entry&		findNextEnding		( std::string const &v )	{return findNextEnding( v.c_str() );};

	attribute*				getAttribute	( void ) const		{static attribute null(std::string(),-1); return( m_iter == m_values.end() ? &null : *m_iter );};
	template<typename T> inline attribute*	getAttribute	( T const v )		{return find( v ).getAttribute();};
	std::vector<attribute>	getAttributesVector	( void ) const			{std::vector<attribute> ret; for( auto iter : m_values ) ret.push_back( *iter ); return ret;};

	inline unsigned int	size			( void ) const			{return  m_values.size();};	// attributes count
	inline bool		empty			( void ) const			{return( !m_values.size() );};	// is there none attribute?

//	Make LDAP easier interface:
	inline entry&		operator=		( entry const *v )		{if (v) copy( *v ); else clear(); return *this;};
	inline entry&		operator=		( entry const &v )		{copy( v ); return *this;};

	entry&			operator+=		( entry const * );
	inline entry&		operator+=		( entry const &v )		{return operator+=( &v );};
	inline entry		operator+		( entry const *v ) const	{return entry( *this ).operator+=( v );};
	inline entry		operator+		( entry const &v ) const	{return entry( *this ).operator+=( v );};

	inline entry&		operator+=		( attribute const *v )		{return insert( v );};
	inline entry&		operator+=		( attribute const &v )		{return insert( v );};
	inline entry&		operator+		( attribute const *v ) const	{return entry( *this ).operator+=( v );};
	inline entry&		operator+		( attribute const &v ) const	{return entry( *this ).operator+=( v );};

	entry&			operator-=		( entry const * );
	inline entry&		operator-=		( entry const &v )		{return operator-=( &v );};
	inline entry		operator-		( entry const *v ) const	{return entry( *this ).operator-=( v );};
	inline entry		operator-		( entry const &v ) const	{return entry( *this ).operator-=( v );};

	entry&			operator-=		( attribute const *v );
	inline entry&		operator-=		( attribute const &v )		{return operator-=( &v );};
	inline entry&		operator-		( attribute const *v ) const	{return entry( *this ).operator-=( v );};
	inline entry&		operator-		( attribute const &v ) const	{return entry( *this ).operator-=( v );};

	entry&			operator-=		( char const *v )		{return erase( v );};
	inline entry&		operator-=		( std::string const &v )	{return erase( v );};
	inline entry		operator-		( char const *v ) const		{return entry( *this ).operator-=( v );};
	inline entry		operator-		( std::string const &v ) const	{return entry( *this ).operator-=( v );};

	entry&			operator&=		( entry const * );
	inline entry&		operator&=		( entry const &v )		{return operator&=( &v );};
	inline entry		operator&		( entry const *v ) const	{return entry( *this ).operator&=( v );};
	inline entry		operator&		( entry const &v ) const	{return entry( *this ).operator&=( v );};

	entry&			operator|=		( entry const * );
	inline entry&		operator|=		( entry const &v )		{return operator|=( &v );};
	inline entry		operator|		( entry const *v ) const	{return entry( *this ).operator|=( v );};
	inline entry		operator|		( entry const &v ) const	{return entry( *this ).operator|=( v );};

	bool			operator==		( entry const * ) const;
	inline bool		operator==		( entry const &v ) const	{return  operator==( &v );};
	inline bool		operator!=		( entry const *v ) const	{return !operator==(  v );};
	inline bool		operator!=		( entry const &v ) const	{return !operator==(  v );};

	inline bool		operator<=		( entry const *v ) const	{return( swapDn( this->identifier() ).compare( swapDn( v ? v->identifier() : std::string() ) ) <= 0 );};
	inline bool		operator<=		( entry const &v ) const	{return( swapDn( this->identifier() ).compare( swapDn( v.identifier() ) ) <= 0 );};

	inline bool		operator<		( entry const *v ) const	{return( swapDn( this->identifier() ).compare( swapDn( v ? v->identifier() : std::string() ) ) <  0 );};
	inline bool		operator<		( entry const &v ) const	{return( swapDn( this->identifier() ).compare( swapDn( v.identifier() ) ) <  0 );};

	inline bool		operator>		( entry const *v ) const	{return( swapDn( this->identifier() ).compare( swapDn( v ? v->identifier() : std::string() ) ) >  0 );};
	inline bool		operator>		( entry const &v ) const	{return( swapDn( this->identifier() ).compare( swapDn( v.identifier() ) ) >  0 );};

	inline bool		operator>=		( entry const *v ) const	{return( swapDn( this->identifier() ).compare( swapDn( v ? v->identifier() : std::string() ) ) >= 0 );};
	inline bool		operator>=		( entry const &v ) const	{return( swapDn( this->identifier() ).compare( swapDn( v.identifier() ) ) >= 0 );};

	inline bool		operator++		()				{return gotoNext();};		// entry++
	inline bool		operator++		( int )				{return gotoNext();};		// ++entry
	inline bool		operator--		()				{return gotoPrevious();};	// entry--
	inline bool		operator--		( int )				{return gotoPrevious();};	//--entry

	inline attribute*	operator[]		( size_t pos )			{m_iter=m_values.begin(); while( pos-- && m_iter!=m_values.end() ) m_iter++; return getAttribute();};
	inline attribute*	operator[]		( std::string const &v )	{return( v.size() ? find( v.c_str() ).getAttribute() : getAttribute() );};

	friend std::ostream&	operator<<		( std::ostream& out, entry const *v )	{return( out << ( v ? v->identifier() : std::string() ) );};
	friend std::ostream&	operator<<		( std::ostream& out, entry const &v )	{return( out << v.identifier() );};

    protected:
	void			copy			( entry const & );

    private:
	struct lessattribute {
		inline bool operator() ( attribute const *a, attribute const *b ) const	{return( *a < b );};
	}; // needed because... * !

	std::string					m_identifier;
	std::set<attribute*>::const_iterator		m_iter;
	std::set<attribute*, lessattribute>		m_values;

	inline entry&		fastInsert		( attribute *v )		{if (v && !v->isNULL()) {auto ret=m_values.insert(v); m_iter=ret.first; if (!ret.second) delete v;}; return *this;};
	static std::string	swapDn			( std::string const & );		// swap dn components (for a correct sort in the set)
  };
}	// End namespace ldapcppei

/* Pour mémoire:
typedef struct berval {
	ber_len_t	bv_len;
	char		*bv_val;
} BerValue;


typedef struct ldapmod {
#define LDAP_MOD_OP		(0x0007)
#define LDAP_MOD_ADD		(0x0000)
#define LDAP_MOD_DELETE		(0x0001)
#define LDAP_MOD_REPLACE	(0x0002)
#define LDAP_MOD_INCREMENT	(0x0003) // OpenLDAP extension
#define LDAP_MOD_BVALUES	(0x0080)
	int		mod_op;
	char		*mod_type;
	union mod_vals_u {
		char		**modv_strvals;
		struct berval	**modv_bvals;
	} mod_vals;
#define mod_values	mod_vals.modv_strvals
#define mod_bvalues	mod_vals.modv_bvals
} LDAPMod;
*/

#endif // header guard
