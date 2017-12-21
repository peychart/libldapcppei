#ifndef HEADER_722CC69A878235AD
#define HEADER_722CC69A878235AD

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
#include <libldapcppei/ldapEntry.h>

namespace ldapcppei
{ class ldapSet {
    public:
	 ldapSet( void );
	 ldapSet( ldapSet const * );
	 ldapSet( ldapSet const & );
	~ldapSet( void );

	inline ldapSet&		clear			( void )			{return eraseAll();};
	inline ldapSet&		eraseAll		( void )			{for( auto iter : m_values ) delete iter; m_values.clear(); m_iter = m_values.end(); return *this;};
	inline ldapSet&		erase			( void )			{if ( m_iter!=m_values.end() ) {delete *m_iter; m_values.erase( m_iter++ );} return *this;};
	inline ldapSet&		erase			( char const *dn )		{return find( dn ).erase();};
	inline ldapSet&		erase			( std::string const &dn )	{return find( dn.c_str() ).erase();};
	inline ldapSet&		erase			( entry const *e )		{return find( e ? e->dn() : std::string() ).erase();};
	inline ldapSet&		erase			( entry const &e )		{return find( e.dn() ).erase();};
	inline ldapSet&		erase			( ldapSet const *v )		{if (v) for ( auto iter : v->m_values ) erase( iter->dn() ); return *this;};
	inline ldapSet&		erase			( ldapSet const &v )		{for( auto iter : v.m_values ) erase( iter->dn() ); return *this;};
	ldapSet&		eraseNullEntries	( void );

	inline ldapSet&		ldap_mod_op		( int mod )			{for( auto iter : m_values ) iter->ldap_mod_op( mod ); return *this;};
	inline ldapSet&		setMod_op		( int mod=LDAP_MOD_BVALUES )	{return ldap_mod_op( mod );};
	inline ldapSet&		setAddAttributes	( void )			{return setMod_op( LDAP_MOD_ADD );};
	inline ldapSet&		setReplaceAttributes	( void )			{return setMod_op( LDAP_MOD_REPLACE );};
	inline ldapSet&		setDeleteAttributes	( void )			{return setMod_op( LDAP_MOD_DELETE );};
	inline ldapSet&		setSearchAttributes	( void )			{return setMod_op( LDAP_MOD_BVALUES );};

	template<typename T>inline ldapSet& insert	( T const e )			{return fastInsert( new entry(e) );};
	template<typename T>inline ldapSet& insert	( std::string dn, std::string attrName, int mod, T const bv=nullptr )
											{return( isNULL() ? *this : ((bv==nullptr) ? fastInsert( new entry(dn, attrName, mod ) ) : fastInsert( new entry(dn, attrName, mod, bv) )) );};
	template<typename T>inline ldapSet& insert	( std::string dn, T const attr ){return( isNULL() ? *this : fastInsert( new entry(dn, attr) ) );};
	inline ldapSet&		insert			( entry* e, bool dl=false )	{return( dl ? fastInsert(e) : fastInsert( new entry(e) ) );};
	inline ldapSet&		insert			( entry const *v[] )		{if (v != nullptr) for(entry const **val=v; *val; val++) insert( *val ); m_iter = m_values.begin();			return *this;};
	inline ldapSet&		insert			( entry *v[], bool dl=false)	{if (v != nullptr) for(entry **val=v; *val; val++) insert(*val, dl); m_iter=m_values.begin(); return *this;};
	inline ldapSet&		insert			( ldapSet const *l )		{if (l) for( auto iter=l->m_values.begin(); iter != l->m_values.end(); ++iter ) insert( *iter );			return *this;};
	inline ldapSet&		insert			( ldapSet const &l )		{return insert( &l );};
	inline ldapSet&		insert			( ldapSet *l )			{return insert( (ldapSet const *)l );};
	inline ldapSet&		insert			( ldapSet &l )			{return insert( (ldapSet const *)&l );};

	inline bool		gotoBegin		( void )			{return( ((m_iter = m_values.begin()) != m_values.end()) );};
	inline bool		gotoFirst		( size_t l=0 )			{if ( ! gotoBegin() ) return false; if ( !l || (!end()&&(*m_iter)->dnSize()==l) ) return true; return this->gotoNext(l);};
	inline bool		begin			( void )			{return( (m_iter == m_values.begin()) );};
	inline bool		first			( void )			{return begin();};
	bool			gotoNext		( size_t l=0 );
	bool			gotoPrevious		( size_t l=0 );
	inline bool		gotoEnd			( void )			{return( ((m_iter = m_values.end())== m_values.end()) );};
	inline bool		gotoLast		( size_t l=0 )			{if ( !this->gotoEnd() ) return false; if ( !l || (!end()&&(*m_iter)->dnSize()==l) ) return true; return this->gotoPrevious(l);};
	inline bool		end			( void )			{return( (m_iter == m_values.end()) );};
	inline bool		last			( void )			{auto iter=m_iter; return( ++iter==m_values.end() );};
	inline void		gotoAllTops		( void )			{m_iter=m_values.begin(); for( auto iter : m_values ) iter->gotoAllTops();};

	inline ldapSet&		setNULL	 		( void )			{return clear();};
	inline bool		isNULL	  		( void ) const			{return empty();};

	inline ldapSet&		find			( char const *v )		{entry value( v ); return find( &value );};
	inline ldapSet&		find			( std::string const &v )	{return find( v.c_str() );};
	inline ldapSet&		find			( entry const *v )		{m_iter = m_values.find( const_cast<entry*>(v) );  return *this;};
	inline ldapSet&		find			( entry const &v )		{m_iter = m_values.find( const_cast<entry*>(&v) ); return *this;};
	inline ldapSet&		findStartingWith	( char const *v )		{for( m_iter=m_values.begin(); m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->isDnStartingWith( v ) ) break;		return *this;};
//	inline ldapSet&		findStartingWith	( char const *v )		{entry value( v ); m_iter = m_values.upper_bound( &value ); if ( ! (*m_iter)->isDnStartingWith( v ) ) m_iter = m_values.end(); return *this;};	// Pb with swapDn...
	inline ldapSet&		findStartingWith	( std::string const &v )	{return findStartingWith( v.c_str() );};
	inline ldapSet&		findContaining		( char const *v, size_t fr=0 )	{for( m_iter=m_values.begin(); m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->isDnContaining( v, fr ) ) break;	return *this;};
	inline ldapSet&		findContaining		( std::string const &v, size_t fr=0 )
											{return findContaining( v.c_str(), fr );};
	inline ldapSet&		findNextStartingWith	( char const *v )		{for( m_iter++; m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->isDnStartingWith( v ) ) break;			return *this;};
	inline ldapSet&		findNextStartingWith	( std::string const &v )	{return findNextStartingWith( v.c_str() );};
	inline ldapSet&		findNextContaining	( char const *v, size_t fr=0 )	{for( m_iter++; m_iter!=m_values.end(); m_iter++ ) if( (*m_iter)->isDnContaining( v, fr ) ) break;			return *this;};
	inline ldapSet&		findNextContaining	( std::string const &v, size_t fr=0 )
											{return findNextContaining( v.c_str(), fr );};

	entry*					getEntry( void )			{static entry nullEntry("#"); return( m_iter == m_values.end() ? &nullEntry: *m_iter );};
	template<typename T> inline entry*	getEntry( T const v )			{return find( v ).getEntry();};
	std::vector<entry>	getEntriesVector	( void ) const			{std::vector<entry> ret; for( auto iter : m_values ) ret.push_back( *iter ); return ret;};

	inline size_t		size			( void ) const			{return  m_values.size();};
	inline bool		empty			( void ) const			{return !m_values.size();};

//	Make LDAP easier interface:
	inline ldapSet&		operator=		( ldapSet const *v )		{if (v) copy( *v ); else clear(); return *this;};;
	inline ldapSet&		operator=		( ldapSet const &v )		{copy( v ); return *this;};

	ldapSet&		operator+=		( ldapSet const * );
	inline ldapSet&		operator+=		( ldapSet const &v )		{return operator+=( &v );};
	inline ldapSet		operator+		( ldapSet const *v ) const	{return ldapSet( *this ).operator+=( v );};
	inline ldapSet		operator+		( ldapSet const &v ) const	{return ldapSet( *this ).operator+=( v );};

	inline ldapSet&		operator+=		( entry const *v )		{return insert( v );};
	inline ldapSet&		operator+=		( entry const &v )		{return insert( v );};
	inline ldapSet&		operator+		( entry const *v ) const	{return ldapSet( *this ).operator+=( v );};
	inline ldapSet&		operator+		( entry const &v ) const	{return ldapSet( *this ).operator+=( v );};

	ldapSet&		operator-=		( ldapSet const * );
	inline ldapSet&		operator-=		( ldapSet const &v )		{return operator-=( &v );};
	inline ldapSet		operator-		( ldapSet const *v ) const	{return ldapSet( this ).operator-=( v );};
	inline ldapSet		operator-		( ldapSet const &v ) const	{return ldapSet( this ).operator-=( v );};

	inline ldapSet&		operator-=		( entry const *v )		{return erase( v->dn() );};
	inline ldapSet&		operator-=		( entry const &v )		{return erase( v );};
	inline ldapSet		operator-		( entry const *v ) const	{return ldapSet( this ).operator-=( v );};
	inline ldapSet		operator-		( entry const &v ) const	{return ldapSet( this ).operator-=( v );};

	inline ldapSet&		operator-=		( char const *v )		{return erase( v );};
	inline ldapSet&		operator-=		( std::string const &v )	{return erase( v );};
	inline ldapSet		operator-		( char const *v ) const		{return ldapSet( this ).operator-=( v );};
	inline ldapSet		operator-		( std::string const &v ) const	{return ldapSet( this ).operator-=( v );};

	ldapSet&		operator&=		( ldapSet const *v );
	inline ldapSet&		operator&=		( ldapSet const &v )		{return operator&=( &v );};
	inline ldapSet		operator&		( ldapSet const *v ) const	{return ldapSet( this ).operator&=( v );};
	inline ldapSet		operator&		( ldapSet const &v ) const	{return ldapSet( this ).operator&=( v );};

	ldapSet&		operator|=		( ldapSet const *v );
	inline ldapSet&		operator|=		( ldapSet const &v )		{return operator|=( &v );};
	inline ldapSet		operator|		( ldapSet const *v ) const	{return ldapSet( this ).operator|=( v );};
	inline ldapSet		operator|		( ldapSet const &v ) const	{return ldapSet( this ).operator|=( v );};

	bool			operator==		( ldapSet const *v ) const;
	inline bool		operator==		( ldapSet const &v ) const 	{return  operator==( &v );};
	inline bool		operator!=		( ldapSet const *v ) const 	{return !operator==(  v );};
	inline bool		operator!=		( ldapSet const &v ) const 	{return !operator==(  v );};

	inline bool		operator++		()				{return gotoNext();};
	inline bool		operator++		( int )				{return gotoNext();};
	inline bool		operator--		()				{return gotoPrevious();};
	inline bool		operator--		( int )				{return gotoPrevious();};

	inline entry*		operator[]		( size_t pos )			{m_iter=m_values.begin(); for( size_t i=0; i<pos && m_iter!=m_values.end(); i++ ) m_iter++; return getEntry();};
	inline entry*		operator[]		( std::string const &v )	{return( v.size() ? find( v.c_str() ).getEntry() : getEntry() );};

    protected:
	void			copy			( ldapSet const & );

    private:
	struct lessentry
	{	inline bool operator() ( entry const *a, entry const *b ) const		{return( *a < b );};
	}; // needed because... operator<() is not automatically called on * !!!

	std::set<entry*>::const_iterator		m_iter;
	std::set<entry*, lessentry>			m_values;

	inline ldapSet&		fastInsert		( entry* v )			{if (v && !v->isNULL()) {auto ret=m_values.insert(v); m_iter=ret.first; if (!ret.second) delete v;}; return *this;};
	inline static int	countFieldsInDn		( std::string& dn )		{int n=0,pos=0,len=dn.size(); while( (pos=dn.find(",",pos)) < len ) n++; return n;};
  };
}	// End namespace ldapcppei

#endif // header guard
