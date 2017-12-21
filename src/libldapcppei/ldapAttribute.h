#ifndef HEADER_923BC1026C02C103
#define HEADER_923BC1026C02C103

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
#include <vector>
#include <list>
#include <set>
#include <libldapcppei/ldapBerValue.h>

namespace ldapcppei
{ class attribute {
    public:
	 attribute( void );
	 attribute( char const * );
	 attribute( std::string const & );
	 attribute( std::string, int mode );
	 attribute( std::string, int, char const * );
	 attribute( std::string, int, std::string );
	 attribute( std::string, int, BerValue const * );
	 attribute( std::string, int, BerValue const & );
	 attribute( std::string, int, berValue const * );
	 attribute( std::string, int, berValue const & );
	 attribute( std::string, int, berValue*, bool=false );

	 attribute( std::string, int, char const *[] );
	 attribute( std::string, int, BerValue const *[] );
	 attribute( std::string, int, berValue const *[] );
	 attribute( std::string, int, berValue*[], bool=false );

	 attribute( LDAPMod const * );
	 attribute( LDAPMod const & );
	 attribute( attribute const * );
	 attribute( attribute const & );

	~attribute();

	inline attribute&		identifier	( char const *name )		{if ( !isNULL() && name ) m_identifier.assign( name ); else m_identifier.clear(); return *this;};
	inline attribute&		identifier	( std::string const &name )	{return identifier( name.c_str() );};		// set attribute name
	inline attribute&		name		( char const *name )		{return identifier( name );};
	inline attribute&		name		( std::string const &name )	{return identifier( name );};
	inline attribute&		setIdentifier	( char const *name=nullptr )	{return identifier( name );};
	inline attribute&		setIdentifier	( std::string const &name )	{return identifier( name );};
	inline std::string const &	identifier	( void ) const			{return m_identifier;};			// get attribute name
	inline std::string const &	getIdentifier	( void ) const			{return identifier();};
	inline std::string const &	name		( void ) const			{return identifier();};

	inline bool			isName		( berValue val ) const		{return( berValue(identifier()).stringEvaluate( val )==0 );};
	inline bool			isNameStartingWith( berValue val ) const	{return( berValue(identifier()).startsWith( val ) );};
	inline bool			isNameContaining( berValue val, size_t from=0 ) const
											{return( berValue(identifier()).contains( val, from ) );};
	inline bool			isNameEndingWith( berValue val ) const		{return( berValue(identifier()).endsWith( val ) );};

	inline attribute&		setNULL		( void )			{eraseAll(); setIdentifier(); m_mod_op = -1; return *this;};
	inline bool			isNULL		( void ) const			{return( m_mod_op == -1 );};

	inline int			ldap_mod_op	( void ) const			{return m_mod_op;};
	inline int			getMod_op	( void ) const			{return ldap_mod_op();};
	inline attribute&		ldap_mod_op	( int val )			{m_mod_op = ( val | LDAP_MOD_BVALUES ); return *this;};
	inline attribute&		setMod_op	( int val=LDAP_MOD_BVALUES )	{return ldap_mod_op( val );};
	inline attribute&		setAddAttribute	( void )			{return setMod_op( LDAP_MOD_ADD );};
	inline attribute&		setReplaceAttribute( void )			{return setMod_op( LDAP_MOD_REPLACE );};
	inline attribute&		setDeleteAttribute( void )			{return setMod_op( LDAP_MOD_DELETE );};
	inline attribute&		setSearchAttribute( void )			{return setMod_op( LDAP_MOD_BVALUES );};

	inline attribute&		clear		( void )			{eraseAll(); setMod_op(); setIdentifier(); return *this;};
	inline attribute&		eraseAll	( void )			{for( auto iter : m_values ) delete iter; m_values.clear(); m_iter = m_values.end();	return *this;};
	inline attribute&		erase		( attribute const *v )		{if (v) for( auto iter : v->m_values ) erase( iter ); m_iter = m_values.begin();	return *this;};
	inline attribute&		erase		( attribute const &v )		{for( auto iter : m_values ) erase( iter ); m_iter = m_values.begin();			return *this;};
	inline attribute&		erase		( char const *v )		{return find( v ).erase();};	// No template because of operator-=()...
	inline attribute&		erase		( std::string const &v )	{return find( v ).erase();};
	inline attribute&		erase		( BerValue const *v )		{return find( v ).erase();};
	inline attribute&		erase		( BerValue const &v )		{return find( v ).erase();};
	inline attribute&		erase		( berValue const *v )		{return find( v ).erase();};
	inline attribute&		erase		( berValue const &v )		{return find( v ).erase();};
	inline attribute&		erase		( void )			{if ( m_iter != m_values.end() ) {delete *m_iter; m_values.erase( m_iter++ );}; return *this;};
	inline attribute&		eraseNullValues	( void )			{for( auto iter : m_values ) if ( iter->empty() ) {delete iter; m_values.erase( iter );}; m_iter=m_values.begin(); return *this;};

	inline attribute&		insert		( char const *v )		{return( isNULL() ? *this : fastInsert( new berValue( v ) ) );};
	inline attribute&		insert		( std::string const &v )	{return  insert( v.c_str() );};	// No template because of operator+=()...
	inline attribute&		insert		( BerValue const *v )		{return( isNULL() ? *this : fastInsert( new berValue( v ) ) );};
	inline attribute&		insert		( BerValue const &v )		{return  insert( &v );};
	inline attribute&		insert		( berValue const *v )		{return( isNULL() ? *this : fastInsert( new berValue( v ) ) );};
	inline attribute&		insert		( berValue const &v )		{return  insert( &v );};
	inline attribute&		insert		( berValue*b, bool dl=false )	{return( isNULL() ? *this : (dl ? fastInsert( b ) : fastInsert( new berValue(b) )) );};
	inline attribute&		insert		( char const *v[] )		{if (v != nullptr) for( char const **val=v; *val; val++ )	insert( *val ); m_iter = m_values.begin(); return *this;};
	inline attribute&		insert		( BerValue const *v[] )		{if (v != nullptr) for( BerValue const **val=v; *val; val++ )	insert( *val ); m_iter = m_values.begin(); return *this;};
	inline attribute&		insert		( berValue const *v[] )		{if (v != nullptr) for( berValue const **val=v; *val; val++ )	insert( *val ); m_iter = m_values.begin(); return *this;};
	inline attribute&		insert		( berValue *v[], bool dl=false)	{if (v != nullptr) for( berValue**val=v; *val; val++ )		insert(*val, dl); m_iter=m_values.begin(); return *this;};

	inline bool			gotoBegin	( void )			{return( (m_iter =  m_values.begin()) != m_values.end() );};
	inline bool			gotoFirst	( void )			{return gotoBegin();};
	inline bool			begin		( void )			{return( m_iter == m_values.begin() );};
	inline bool			first		( void )			{return  begin();};
	inline bool			gotoPrevious	( void )			{return( (m_iter != m_values.begin()) ? (--m_iter != m_iter) : false );};
	inline bool			gotoNext	( void )			{return( (m_iter != m_values.end()) ? (++m_iter != m_values.end()) : false );};
	inline bool			gotoEnd		( void )			{return( (m_iter =  m_values.end()) == m_values.end() );};
	inline bool			gotoLast	( void )			{m_iter =  m_values.end(); return gotoPrevious();};
	inline bool			end		( void )			{return( m_iter == m_values.end() );};
	inline bool			last		( void )			{auto iter = m_iter; return( ++iter==m_values.end() );};	// Is current pos the last value?

	template<typename T> inline attribute& find	( T const v )			{berValue value(v); m_iter = m_values.find( &value ); return *this;};
	inline attribute&		find		( berValue *v )			{m_iter=m_values.find( v ); return *this;};
	inline attribute&		find		( berValue &v )			{return find( const_cast<berValue*>(&v) );};
//	inline attribute&		findStartingWith( berValue &v )			{for( m_iter=m_values.begin(); m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->startsWith( v ) ) break; return *this;};
	inline attribute&		findStartingWith( berValue &v )			{m_iter = m_values.upper_bound( &v ); if ( ! (*m_iter)->startsWith( v ) ) m_iter = m_values.end(); return *this;};
	inline attribute&		findContaining	( berValue &v, size_t from=0 )	{for( m_iter=m_values.begin(); m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->contains(v, from) ) break; return *this;};
	inline attribute&		findEndingWith	(  berValue &v )		{for( m_iter=m_values.begin(); m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->endsWith( v ) ) break; return *this;};
	inline attribute&		findNextStartingWith( berValue &v )		{if ( m_iter!=m_values.end() && ++m_iter!=m_values.end() && (*m_iter)->startsWith( v ) ) m_iter=m_values.end(); return *this;};
	inline attribute&		findNextContaining( berValue &v, size_t from=0)	{for( m_iter++; m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->contains(v, from) ) break; return *this;};
	inline attribute&		findNextEnding	(  berValue &v )		{for( m_iter++; m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->endsWith( v ) ) break; return *this;};

	berValue*				getBerValue	( void ) const		{static berValue nul((size_t)-1,nullptr); return( m_iter == m_values.end() ? &nul : *m_iter );};
	template<typename T> inline berValue*	getBerValue	( T const v )		{berValue value(v); m_iter = m_values.find( &value ); return getBerValue();};
	inline char const *			getStrValue	( void ) const		{return( (m_iter == m_values.end()) ? "" : (*m_iter)->toStr() );};
	template<typename T> inline char const*	getStrValue	( T const v )		{berValue value(v); m_iter = m_values.find( &value ); return getStrValue();}
	inline size_t				getValueSize	( void ) const		{return( (m_iter == m_values.end()) ? 0 : (*m_iter)->size() );};

	std::vector<berValue>		getBerValuesVector ( void ) const		{std::vector<berValue>	  ret; for( auto iter : m_values ) ret.push_back( *iter ); return ret;};
	std::vector<std::string>	getStrValuesVector ( void ) const		{std::vector<std::string> ret; for( auto iter : m_values ) ret.push_back( iter->c_str() ); return ret;};

	inline unsigned int		size		( void ) const			{return   m_values.size();}; // values count
	inline bool			empty		( void ) const			{return( !m_values.size() );};

//	Make LDAP easier interface:
	inline attribute&		operator=	( attribute const *v )		{if (v) copy( *v ); else clear(); return *this;};
	inline attribute&		operator=	( attribute const &v )		{copy( v ); return *this;};

	attribute&			operator+=	( attribute const * );
	inline attribute&		operator+=	( attribute const &v )		{return operator+=( &v );};
	inline attribute		operator+	( attribute const *v ) const	{return attribute( *this ).operator+=( v );};
	inline attribute		operator+	( attribute const &v ) const	{return attribute( *this ).operator+=( v );};

	inline attribute&		operator+=	( berValue const *v )		{return insert( v );};
	inline attribute&		operator+=	( berValue const &v )		{return insert( v );};
	inline attribute&		operator+	( berValue const *v ) const	{return attribute( *this ).operator+=( v );};
	inline attribute&		operator+	( berValue const &v ) const	{return attribute( *this ).operator+=( v );};

	attribute&			operator-=	( attribute const * );
	inline attribute&		operator-=	( attribute const &v )		{return operator-=( &v );};
	inline attribute		operator-	( attribute const *v ) const	{return attribute( *this ).operator-=( v );};
	inline attribute		operator-	( attribute const &v ) const	{return attribute( *this ).operator-=( v );};

	inline attribute&		operator-=	( berValue const *v )		{return erase( v );};
	inline attribute&		operator-=	( berValue const &v )		{return erase( v );};
	inline attribute&		operator-	( berValue const *v ) const	{return attribute( *this ).operator-=( v );};
	inline attribute&		operator-	( berValue const &v ) const	{return attribute( *this ).operator-=( v );};

	attribute&			operator&=	( attribute const * );
	inline attribute&		operator&=	( attribute const &v )		{return operator&=( &v );};
	inline attribute		operator&	( attribute const *v ) const	{return attribute( *this ).operator&=( v );};
	inline attribute		operator&	( attribute const &v ) const	{return attribute( *this ).operator&=( v );};

	inline attribute&		operator|=	( attribute const *v )		{return operator+=( v );};
	inline attribute&		operator|=	( attribute const &v )		{return operator|=( &v );};
	inline attribute		operator|	( attribute const *v ) const	{return attribute( *this ).operator|=( v );};
	inline attribute		operator|	( attribute const &v ) const	{return attribute( *this ).operator|=( v );};

	bool				operator==	( attribute const * ) const;
	inline bool			operator==	( attribute const &v ) const	{return  operator==( &v );};
	inline bool			operator!=	( attribute const *v ) const	{return !operator==(  v );};
	inline bool			operator!=	( attribute const &v ) const	{return !operator==(  v );};

	inline bool			operator<=	( attribute const *v ) const	{return( berValue( identifier() ).stringEvaluate( v==nullptr ? nullptr : v->identifier() ) <= 0 );};
	inline bool			operator<=	( attribute const &v ) const	{return( berValue( identifier() ).stringEvaluate( v.identifier() ) <= 0 );};

	inline bool			operator<	( attribute const *v ) const	{return( berValue( identifier() ).stringEvaluate( v==nullptr ? nullptr : v->identifier() ) <  0 );};
	inline bool			operator<	( attribute const &v ) const	{return( berValue( identifier() ).stringEvaluate( v.identifier() ) <  0 );};

	inline bool			operator>=	( attribute const *v ) const	{return( berValue( identifier() ).stringEvaluate( v==nullptr ? nullptr : v->identifier() ) >= 0 );};
	inline bool			operator>=	( attribute const &v ) const	{return( berValue( identifier() ).stringEvaluate( v.identifier() ) >= 0 );};

	inline bool			operator>	( attribute const *v ) const	{return( berValue( identifier() ).stringEvaluate( v==nullptr ? nullptr : v->identifier() ) >  0 );};
	inline bool			operator>	( attribute const &v ) const	{return( berValue( identifier() ).stringEvaluate( v.identifier() ) >  0 );};

	inline bool			operator++	()				{return gotoNext();};		// attribute++
	inline bool			operator++	( int )				{return gotoNext();};		// ++attribute
	inline bool			operator--	()				{return gotoPrevious();};	// attribute--
	inline bool			operator--	( int )				{return gotoPrevious();};	// --attribute

	inline berValue*		operator[]	( size_t pos )			{m_iter=m_values.begin(); while( pos-- && ++m_iter!=m_values.end() ); return getBerValue();};
	inline berValue*		operator[]	( berValue const &v )		{return( v.size() ? find( v.c_str() ).getBerValue() : getBerValue() );};

	friend std::ostream&		operator<<	( std::ostream& out, attribute const *v )	{return( out << ( v ? v->identifier() : std::string() ) );};
	friend std::ostream&		operator<<	( std::ostream& out, attribute const &v )	{return( out << v.identifier() );};

    protected:
	void				copy		( attribute const & );
	void				copy		( LDAPMod const & );

    private:
	struct lessberValue {
		inline bool operator() ( berValue const *a, berValue const *b ) const	{return( *a < b );};
	};	// needed because... operator<() is not called on * but & by the std::set ...

	std::string					m_identifier;
	int						m_mod_op;
	std::set<berValue*>::const_iterator		m_iter;
	std::set<berValue*, lessberValue>		m_values;

	inline attribute&				fastInsert	( berValue *v )	{if (v && !v->isNULL()) {auto ret=m_values.insert(v); m_iter=ret.first; if ( !ret.second ) delete v;}; return *this;};
//	inline static size_t				stringLen	( char *p )	{size_t ret=0; if (p) while (*p++) ret++; return ret;};
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
