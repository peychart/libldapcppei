#ifndef HEADER_82939A17B0E05B35
#define HEADER_82939A17B0E05B35

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
#include <stack>
#include <map>
#include <libldapcppei/ldapSet.h>

namespace ldapcppei
{ class ldapEasyInterface {
    public:
	 ldapEasyInterface( char const * = nullptr );
	 ldapEasyInterface( std::string const & );
	 ldapEasyInterface( ldapEasyInterface const * );
	 ldapEasyInterface( ldapEasyInterface const & );
	~ldapEasyInterface( void );

	inline char const *		getVersion		( void )			{return "3.1 - 20120830";};

	std::string			uri			( void );
	inline std::string		getUri			( void )			{return uri();};
	ldapEasyInterface&		uri			( std::string );
	inline ldapEasyInterface&	setUri			( std::string s )		{return uri( s );};
	inline ldapEasyInterface&	clearUri		( void )			{m_uries.clear(); m_opennedUriIter=m_uries.end(); return *this;};

	inline std::string		opennedUri		( void ) const			// the current uri choosed by open()
												{if (m_opennedUriIter==m_uries.end()) return std::string(); return *m_opennedUriIter;};
	inline std::string		getOpennedUri		( void )			{return opennedUri();};

	inline LDAP const *		getLdapDescriptor	( void )			{return m_desc;};

	ldapEasyInterface&		setBindDn		( char const *dn=nullptr, char const *pwd=nullptr );
	inline std::string		bindDn			( void )			{m_errNo = LDAP_SUCCESS; return m_bindDn;};
	inline std::string		getBindDn		( void )			{return bindDn();};

	std::string			bindPwd			( void );
	inline std::string		getBindPwd		( void )			{return bindPwd();};
	inline ldapEasyInterface&	clearBinDn		( void )			{m_errNo = LDAP_SUCCESS; return setBindDn();};

	ldapEasyInterface&		setOption		( int option, void const *invalue );
	ldapEasyInterface&		getOption		( int option, void *outvalue );
	inline ldapEasyInterface&	setLdapSizeLimit	( size_t value )		{m_errNo = LDAP_SUCCESS; m_ldapSizeLimit = value; return *this;};
	inline size_t			getLdapSizeLimit	( void )			{m_errNo = LDAP_SUCCESS; return m_ldapSizeLimit;};

												// change the mod_op flag of all input entries ...
	inline ldapEasyInterface&	ldap_mod_op		( int mod )			{getInputSet()->ldap_mod_op( mod ); return *this;};
	inline ldapEasyInterface&	setMod_op		( int mod=LDAP_MOD_BVALUES )	{return ldap_mod_op( mod );};
	inline ldapEasyInterface&	setAddAttributes	( void )			{return setMod_op( LDAP_MOD_ADD );};
	inline ldapEasyInterface&	setReplaceAttributes	( void )			{return setMod_op( LDAP_MOD_REPLACE );};
	inline ldapEasyInterface&	setDeleteAttributes	( void )			{return setMod_op( LDAP_MOD_DELETE );};
	inline ldapEasyInterface&	setSearchAttributes	( void )			{return setMod_op( LDAP_MOD_BVALUES );};

	bool				open			( std::string = std::string(), char const * = nullptr, char const * = nullptr ); // effective connection to LDAP ...
	bool				close			( void );
	inline bool			isOpenned		( void )			{return getOpennedUri().size();};

	inline ldapSet*			getInputSet		( void ) const			{return m_input;};
	inline ldapSet*			getOutputSet		( void ) const			{return m_output;};
	inline entry*			getEntry		( void )			{return getOutEntry();};
	inline entry*			getOutEntry		( void )			{return getOutputSet()->getEntry();};
	inline entry*			getInEntry		( void )			{return getOutputSet()->getEntry();};

	inline ldapEasyInterface&	clear			( void )			{return eraseAll();};

	inline ldapEasyInterface&	eraseAll		( void )			{m_errNo=LDAP_SUCCESS; getInputSet()->erase(); getOutputSet()->erase(); return *this;};
	inline ldapEasyInterface&	erase			( void )			{m_errNo=LDAP_SUCCESS; getOutputSet()->erase(); return *this;};
	inline ldapEasyInterface&	erase			( char const *v )		{getOutputSet()->find( v ).erase(); return *this;};
	inline ldapEasyInterface&	erase			( std::string const &v )	{getOutputSet()->find( v ).erase(); return *this;};
	inline ldapEasyInterface&	erase			( ldapSet const *v )		{getOutputSet()->erase( v ); return *this;};
	inline ldapEasyInterface&	erase			( ldapSet const &v )		{getOutputSet()->erase( v ); return *this;};
	inline ldapEasyInterface&	eraseNullEntries	( ldapSet const &v )		{getInputSet()->eraseNullEntries(); getOutputSet()->eraseNullEntries(); return *this;};

	inline ldapEasyInterface&	setNULL			( void )			{return clear();};
	inline bool			isNULL	  		( void )			{return empty();};

	template<typename T>inline ldapEasyInterface& insert	( T const v )			{getInputSet()->insert( v );				return *this;};
	template<typename T>inline ldapEasyInterface& insert	( std::string dn, std::string attrName, int mod, T const bv )
												{getInputSet()->insert( dn, attrName, mod, bv );	return *this;};
	template<typename T>inline ldapEasyInterface& insert	( std::string dn, T const attr ){getInputSet()->insert( dn, attr );			return *this;};
	inline ldapEasyInterface&	insert			( entry *e, bool del )		{getInputSet()->insert( e, del );			return *this;};
	inline ldapEasyInterface&	insert			( ldapEasyInterface const *l )	{if ( l ) getInputSet()->insert( l->getInputSet() );	return *this;};
	inline ldapEasyInterface&	insert			( ldapEasyInterface const &l )	{getInputSet()->insert( l.getInputSet() );		return *this;};
	inline ldapEasyInterface&	insert			( ldapEasyInterface *l )	{return insert( (ldapEasyInterface const *)l );};
	inline ldapEasyInterface&	insert			( ldapEasyInterface &l )	{return insert( (ldapEasyInterface const *)&l );};

	inline ldapEasyInterface&	find			( char const *v )		{m_errNo=( !getOutputSet()->find( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );			return *this;};
	inline ldapEasyInterface&	find			( std::string const &v )	{m_errNo=( !getOutputSet()->find( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );			return *this;};
	inline ldapEasyInterface&	find			( entry const *v )		{m_errNo=( !getOutputSet()->find( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );			return *this;};
	inline ldapEasyInterface&	find			( entry const &v )		{m_errNo=( !getOutputSet()->find( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );			return *this;};
	inline ldapEasyInterface&	findStartingWith	( char const *v )		{m_errNo=( !getOutputSet()->findStartingWith( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );		return *this;};
	inline ldapEasyInterface&	findStartingWith	( std::string const &v )	{m_errNo=( !getOutputSet()->findStartingWith( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );		return *this;};
	inline ldapEasyInterface&	findContaining		( char const *v, size_t fr )	{m_errNo=( !getOutputSet()->findContaining( v, fr ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );	return *this;};
	inline ldapEasyInterface&	findContaining		( std::string const &v, size_t fr )
												{m_errNo=( !getOutputSet()->findContaining( v, fr ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );	return *this;};
	inline ldapEasyInterface&	findNextStartingWith	( char const *v )		{m_errNo=( !getOutputSet()->findNextStartingWith( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );	return *this;};
	inline ldapEasyInterface&	findNextStartingWith	( std::string const &v )	{m_errNo=( !getOutputSet()->findNextStartingWith( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );	return *this;};
	inline ldapEasyInterface&	findNextContaining	( char const *v, size_t fr )	{m_errNo=( !getOutputSet()->findNextContaining( v, fr ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );	return *this;};
	inline ldapEasyInterface&	findNextContaining	( std::string const &v, size_t fr )
												{m_errNo=( !getOutputSet()->findNextContaining( v, fr ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );	return *this;};

	inline ldapEasyInterface&	findInEntry		( char const *v )		{m_errNo=( !getInputSet()->find( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );			return *this;};
	inline ldapEasyInterface&	findInEntry		( std::string const &v )	{m_errNo=( !getInputSet()->find( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );			return *this;};
	inline ldapEasyInterface&	findInEntry		( entry const *v )		{m_errNo=( !getInputSet()->find( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );			return *this;};
	inline ldapEasyInterface&	findInEntry		( entry const &v )		{m_errNo=( !getInputSet()->find( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );			return *this;};
	inline ldapEasyInterface&	findInEntryStartingWith	( char const *v )		{m_errNo=( !getInputSet()->findStartingWith( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );		return *this;};
	inline ldapEasyInterface&	findInEntryStartingWith	( std::string const &v )	{m_errNo=( !getInputSet()->findStartingWith( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );		return *this;};
	inline ldapEasyInterface&	findInEntryContaining	( char const *v, size_t fr=0 )	{m_errNo=( !getInputSet()->findContaining( v, fr ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );	return *this;};
	inline ldapEasyInterface&	findInEntryContaining	( std::string const &v, size_t fr=0 )
												{m_errNo=( !getInputSet()->findContaining( v, fr ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );	return *this;};
	inline ldapEasyInterface&	findNextInEntryStartingWith( char const *v )		{m_errNo=( !getInputSet()->findNextStartingWith( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );	return *this;};
	inline ldapEasyInterface&	findNextInEntryStartingWith( std::string const &v )	{m_errNo=( !getInputSet()->findNextStartingWith( v ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );	return *this;};
	inline ldapEasyInterface&	findNextInEntryContaining( char const *v, size_t fr=0 )	{m_errNo=( !getInputSet()->findNextContaining( v, fr ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );	return *this;};
	inline ldapEasyInterface&	findNextInEntryContaining( std::string const &v, size_t fr=0 )
												{m_errNo=( !getInputSet()->findNextContaining( v, fr ).end() ? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT );	return *this;};

	inline bool			gotoBegin		( void )			{return( (m_errNo=(getOutputSet()->gotoBegin()		? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT)) == LDAP_SUCCESS );};
	inline bool			gotoFirst		( size_t l=0 )			{return( (m_errNo=(getOutputSet()->gotoFirst( l )	? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT)) == LDAP_SUCCESS );};
	inline bool			begin			( void )			{return( (m_errNo=(getOutputSet()->begin()		? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT)) == LDAP_SUCCESS );};
	inline bool			first			( void )			{return( (m_errNo=(getOutputSet()->first()		? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT)) == LDAP_SUCCESS );};
	inline bool			gotoNext		( size_t l=0 )			{return( (m_errNo=(getOutputSet()->gotoNext( l )	? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT)) == LDAP_SUCCESS );};
	inline bool			gotoPrevious		( size_t l=0 )			{return( (m_errNo=(getOutputSet()->gotoPrevious( l )	? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT)) == LDAP_SUCCESS );};
	inline bool			gotoEnd			( void )			{return( (m_errNo=(getOutputSet()->gotoEnd()		? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT)) == LDAP_SUCCESS );};
	inline bool			gotoLast		( size_t l=0 )			{return( (m_errNo=(getOutputSet()->gotoLast( l )	? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT)) == LDAP_SUCCESS );};
	inline bool			end			( void )			{return( (m_errNo=(getOutputSet()->end()		? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT)) == LDAP_SUCCESS );};
	inline bool			last			( void )			{return( (m_errNo=(getOutputSet()->last()		? LDAP_SUCCESS : LDAP_NO_SUCH_OBJECT)) == LDAP_SUCCESS );};
	inline void			gotoAllTops		( void )			{getInputSet()->gotoAllTops(); getOutputSet()->gotoAllTops(); m_errNo=LDAP_SUCCESS;};

	inline size_t			size			( void )			{return getOutputSet()->size();};
	inline bool			empty			( void )			{return( !size() );};

	ldapEasyInterface&	  	doAddEntries		( void );			// proccess all the input entries ...
	inline ldapEasyInterface&	doAddEntries		( ldapSet const *l )		{getInputSet()->clear().insert( l ); return doAddEntries();};
	inline ldapEasyInterface&	doAddEntries		( ldapSet const &l )		{getInputSet()->clear().insert( l ); return doAddEntries();};
	inline ldapEasyInterface&	doAddEntries		( ldapEasyInterface const *l )	{getInputSet()->clear().insert( l->getInputSet() ); return doAddEntries();};
	inline ldapEasyInterface&	doAddEntries		( ldapEasyInterface const &l )	{return doAddEntries( &l );};
	inline ldapEasyInterface&	doAddEntry		( entry const *e )		{getInputSet()->clear().insert( e ); return doAddEntries();};
	inline ldapEasyInterface&	doAddEntry		( entry const &e )			{return doAddEntry( &e );};

	ldapEasyInterface&	  	doModifyEntries		( void );			//	(and return errors numbre)
	inline ldapEasyInterface&	doModifyEntries		( ldapEasyInterface const *l )	{getInputSet()->clear().insert( l->getInputSet() ); return doModifyEntries();};
	inline ldapEasyInterface&	doModifyEntries		( ldapEasyInterface const &l )	{return doModifyEntries( &l );};
	inline ldapEasyInterface&	doModifyEntry		( entry const *e )		{getInputSet()->clear().insert( e );	return doModifyEntries();};
	inline ldapEasyInterface&	doModifyEntry		( entry const &e )			{return doModifyEntry( &e );};
	ldapEasyInterface&	 	doModifyEntry		( std::string, std::vector<attribute> * );
	inline ldapEasyInterface&	doModifyEntry		( std::string s, std::vector<attribute> v ) {return doModifyEntry( s, &v );};

	ldapEasyInterface&	 	doDeleteEntries		( void );
	inline ldapEasyInterface&	doDeleteEntries		( ldapSet const *l )		{getInputSet()->clear().insert( l );			return doDeleteEntries();};
	inline ldapEasyInterface&	doDeleteEntries		( ldapSet const &l )		{getInputSet()->clear().insert( l );			return doDeleteEntries();};
	inline ldapEasyInterface&	doDeleteEntries		( ldapEasyInterface const *l )	{getInputSet()->clear().insert( l->getInputSet() );	return doDeleteEntries();};
	inline ldapEasyInterface&	doDeleteEntries		( ldapEasyInterface const &l )	{return doDeleteEntries( &l );};
	inline ldapEasyInterface&	doDeleteEntry		( std::string s )		{getInputSet()->clear().insert( s.c_str() );		return doDeleteEntries();};
	inline ldapEasyInterface&	doDeleteEntry		( entry const *e )		{getInputSet()->clear().insert( e );			return doDeleteEntries();};
	inline ldapEasyInterface&	doDeleteEntry		( entry const &e )		{return doDeleteEntry ( &e );};

	inline ldapEasyInterface&	doMoveEntryTo		( entry const *e, std::string newRdn )	{getInputSet()->clear().insert( e );		return moveEntries( newRdn, 2 );};
	inline ldapEasyInterface&	doMoveEntryTo		( entry const &e, std::string newRdn )	{getInputSet()->clear().insert( e );		return moveEntries( newRdn, 2 );};
	inline ldapEasyInterface&	doRenameEntryTo		( entry const *e, std::string newRdn )	{getInputSet()->clear().insert( e );		return moveEntries( newRdn, 1 );};
	inline ldapEasyInterface&	doRenameEntryTo		( entry const &e, std::string newRdn )	{getInputSet()->clear().insert( e );		return moveEntries( newRdn, 1 );};
	inline ldapEasyInterface&	doDuplicateEntryTo	( entry const *e, std::string newRdn )	{getInputSet()->clear().insert( e );		return moveEntries( newRdn, 0 );};
	inline ldapEasyInterface&	doDuplicateEntryTo	( entry const &e, std::string newRdn )	{getInputSet()->clear().insert( e );		return moveEntries( newRdn, 0 );};
	inline ldapEasyInterface&	doDuplicateEntriesTo	( std::string newRdn )		{return moveEntries( newRdn, 0 );};

	inline ldapEasyInterface&	setSearchFilter		( std::string s )		{m_errNo = LDAP_SUCCESS; m_searchFilter.assign( s );	return *this;};
	inline std::string const &	getSearchFilter		( void )			{m_errNo = LDAP_SUCCESS;				return m_searchFilter;};

	inline ldapEasyInterface&	doSearchScopeBase	( void )			{getOutputSet()->clear(); return doSearchForEntries( LDAP_SCOPE_BASE );};
	inline ldapEasyInterface&	doAddSearchScopeBase	( void )			{return doSearchForEntries( LDAP_SCOPE_BASE );};
	inline ldapEasyInterface&	doSearchScopeOneLevel	( void )			{getOutputSet()->clear(); return doSearchForEntries( LDAP_SCOPE_ONELEVEL );};
	inline ldapEasyInterface&	doAddSearchScopeOneLevel( void )			{return doSearchForEntries( LDAP_SCOPE_ONELEVEL );};
	inline ldapEasyInterface&	doSearchScopeSubTree	( void )			{getOutputSet()->clear(); return doSearchForEntries( LDAP_SCOPE_SUBTREE );};
	inline ldapEasyInterface&	doAddSearchScopeSubTree	( void )			{return doSearchForEntries( LDAP_SCOPE_SUBTREE );};
	inline ldapEasyInterface&	doSearchScopeChildren 	( void )			{getOutputSet()->clear(); return doSearchForEntries( LDAP_SCOPE_CHILDREN );};
	inline ldapEasyInterface&	doAddSearchScopeChildren( void )			{return doSearchForEntries( LDAP_SCOPE_CHILDREN );};
												// An other (easier) way to define search criterious ...
	inline ldapEasyInterface&	doAddSearchScopeBase	( std::string baseDn, std::string  attributesListe=std::string(), std::string filter=std::string() )
						{if ( !setEntryAttributesListeAndFilterForSearch( baseDn, attributesListe, filter ) ) return *this; return doSearchForEntries( LDAP_SCOPE_BASE );};
	inline ldapEasyInterface&	doSearchScopeBase	( std::string baseDn, std::string  attributesListe=std::string(), std::string filter=std::string() )
						{getOutputSet()->clear(); return doAddSearchScopeBase	 ( baseDn, attributesListe, filter );};
	inline ldapEasyInterface&	doAddSearchScopeBase	( std::string baseDn, std::vector<std::string> const &attributesListe, std::string filter=std::string() )
						{if ( !setEntryAttributesListeAndFilterForSearch ( baseDn, attributesListe, filter ) ) return *this; return doSearchForEntries( LDAP_SCOPE_BASE );};
	inline ldapEasyInterface&	doSearchScopeBase	( std::string baseDn, std::vector<std::string> const &attributesListe, std::string filter=std::string() )
						{getOutputSet()->clear(); return doAddSearchScopeBase	 ( baseDn, attributesListe, filter );};
	inline ldapEasyInterface&	doAddSearchScopeOneLevel( std::string baseDn, std::string  attributesListe=std::string(), std::string filter=std::string() )
						{if ( !setEntryAttributesListeAndFilterForSearch ( baseDn, attributesListe, filter ) ) return *this; return doSearchForEntries( LDAP_SCOPE_ONELEVEL );};
	inline ldapEasyInterface&	doSearchScopeOneLevel	( std::string baseDn, std::string  attributesListe=std::string(), std::string filter=std::string() )
						{getOutputSet()->clear(); return doAddSearchScopeOneLevel( baseDn, attributesListe, filter );};
	inline ldapEasyInterface&	doAddSearchScopeOneLevel( std::string baseDn, std::vector<std::string> const &attributesListe, std::string filter=std::string() )
						{if ( !setEntryAttributesListeAndFilterForSearch ( baseDn, attributesListe, filter ) ) return *this; return doSearchForEntries( LDAP_SCOPE_ONELEVEL );};
	inline ldapEasyInterface&	doSearchScopeOneLevel	( std::string baseDn, std::vector<std::string> const &attributesListe, std::string filter=std::string() )
						{getOutputSet()->clear(); return doAddSearchScopeOneLevel( baseDn, attributesListe, filter );};
	inline ldapEasyInterface&	doAddSearchScopeSubTree	( std::string baseDn, std::string  attributesListe=std::string(), std::string filter=std::string() )
						{if ( !setEntryAttributesListeAndFilterForSearch ( baseDn, attributesListe, filter ) ) return *this; return doSearchForEntries( LDAP_SCOPE_SUBTREE );};
	inline ldapEasyInterface&	doSearchScopeSubTree	( std::string baseDn, std::string  attributesListe=std::string(), std::string filter=std::string() )
						{getOutputSet()->clear(); return doAddSearchScopeSubTree ( baseDn, attributesListe, filter );};
	inline ldapEasyInterface&	doAddSearchScopeSubTree	( std::string baseDn, std::vector<std::string> const &attributesListe, std::string filter=std::string() )
						{if ( !setEntryAttributesListeAndFilterForSearch ( baseDn, attributesListe, filter ) ) return *this; return doSearchForEntries( LDAP_SCOPE_SUBTREE );};
	inline ldapEasyInterface&	doSearchScopeSubTree	( std::string baseDn, std::vector<std::string> const &attributesListe, std::string filter=std::string() )
						{getOutputSet()->clear(); return doAddSearchScopeSubTree ( baseDn, attributesListe, filter );};
	inline ldapEasyInterface&	doAddSearchScopeChildren( std::string baseDn, std::string  attributesListe=std::string(), std::string filter=std::string() )
						{if ( !setEntryAttributesListeAndFilterForSearch ( baseDn, attributesListe, filter ) ) return *this; return doSearchForEntries( LDAP_SCOPE_CHILDREN );};
	inline ldapEasyInterface&	doSearchScopeChildren	( std::string baseDn, std::string  attributesListe=std::string(), std::string filter=std::string() )
						{getOutputSet()->clear(); return doAddSearchScopeChildren( baseDn, attributesListe, filter );};
	inline ldapEasyInterface&	doAddSearchScopeChildren( std::string baseDn, std::vector<std::string> const &attributesListe, std::string filter=std::string() )
						{if ( !setEntryAttributesListeAndFilterForSearch ( baseDn, attributesListe, filter ) ) return *this; return doSearchForEntries( LDAP_SCOPE_CHILDREN );};
	inline ldapEasyInterface&	doSearchScopeChildren	( std::string baseDn, std::vector<std::string> const &attributesListe, std::string filter=std::string() )
						{getOutputSet()->clear(); return doAddSearchScopeChildren( baseDn, attributesListe, filter );};

	// Some tools ...
	ldapEasyInterface&		doSwapEntriesWithResults( int = LDAP_MOD_SEARCH );

	inline ldapSet&			doSaveInEntries		( void )			{return *doLifoBackupEntries  ( m_input );};
	inline ldapSet&			doSaveInEntries		( std::string name )		{return *doNamedBackupEntries ( name, m_input );};
	inline ldapSet&			doSaveOutEntries	( void )			{return *doLifoBackupEntries  ( m_output );};
	inline ldapSet&			doSaveOutEntries	( std::string name )		{return *doNamedBackupEntries ( name, m_output );};
	inline bool			doRestoreInEntries	( void )			{return  doLifoRestoreEntries ( m_input );};
	inline bool			doRestoreInEntries	( std::string name )		{return  doNamedRestoreEntries( name, m_input );};
	inline bool			doRestoreOutEntries	( void )			{return  doLifoRestoreEntries ( m_output );};
	inline bool			doRestoreOutEntries	( std::string name )		{return  doNamedRestoreEntries( name, m_output );};

	inline int			getErrorNumber		( void )			{return m_errNo;};
	inline int			errNo			( void )			{return getErrorNumber();};
	inline std::string		getErrorString		( void ) 			{return ldap_err2string( m_errNo );};
	inline std::string		errstring		( void ) 			{return getErrorString();};

	inline bool			operator++		()				{return getOutputSet()->gotoNext();};
	inline bool			operator++		( int )				{return getOutputSet()->gotoNext();};
	inline bool			operator--		()				{return getOutputSet()->gotoPrevious();};
	inline bool			operator--		( int )				{return getOutputSet()->gotoPrevious();};

	inline entry*			operator[]		( size_t pos )			{gotoBegin(); while( pos-- && !end() ) (*this)++; return getOutEntry();};
	inline entry*			operator[]		( std::string const &v )	{return( v.size() ? find( v.c_str() ).getOutEntry() : getOutEntry() );};

	friend std::ostream&		operator<<		( std::ostream& out, ldapEasyInterface const *v ) {return( out << ( v ? v->opennedUri() : std::string() ) );};
	friend std::ostream&		operator<<		( std::ostream& out, ldapEasyInterface const &v ) {return( out << v.opennedUri() );};

    private:
	LDAP						*m_desc;
	int						m_errNo;

	std::vector<std::string>			m_uries;
	std::vector<std::string>::const_iterator	m_opennedUriIter;
	int						m_ldapVersion;
	size_t						m_ldapSizeLimit;
	std::string					m_bindDn;
	BerValue					m_bindPwd;
	std::string					m_searchFilter;

	ldapSet						*m_input, *m_output, m_set1, m_set2;

	std::stack<ldapSet*>				m_lifoBuffer;
	std::map<std::string, ldapSet*>			m_namedBuffer;

	static bool			convert			( attribute*, LDAPMod* & );	// SOME (internal) TOOLS...
	static bool			convert			( entry*, LDAPMod** & );
	static bool			convert			( entry*, int ifMode, LDAPMod** & );
	static bool			convert			( entry*, int ifMode, char** & );
	static void			freeMemory		( LDAPMod* & );
	inline static void		freeMemory 		( LDAPMod** &m )		{if (m) {for( LDAPMod **p=m; *p; p++ ) freeMemory ( *p ); delete[] m; m = nullptr;}};
	inline static void		freeMemory		( char** &m )			{if (m) {for( char **p=m; *p; p++ ) delete[] *p; delete[] m; m = nullptr;}};
	inline static void		freeMemory		( char* &m )			{if (m) {delete[] m; m = nullptr;}};
	inline static void		freeMemory		( BerElement* &m )		{if (m) {ber_free ( m, 0 ); m = nullptr;}};

	ldapEasyInterface&		doSearchForEntries	( int );
	bool				setEntryAttributesListeAndFilterForSearch( std::string baseDn, std::vector<std::string> const & , std::string filter=std::string() );
	bool				setEntryAttributesListeAndFilterForSearch( std::string baseDn, std::string = std::string(), std::string = std::string() );
	ldapSet*			doLifoBackupEntries	( ldapSet* & );
	ldapSet*			doNamedBackupEntries	( std::string name, ldapSet* & );
	bool				doLifoRestoreEntries	( ldapSet* & );
	bool				doNamedRestoreEntries	( std::string name, ldapSet* & );
	ldapEasyInterface&		moveEntries		( std::string const &, int );
	inline static int		countFieldsInDn		( std::string& dn )		{int n=0,pos=0,len=dn.size(); while( (pos=dn.find(",",pos)) < len ) n++; return n;};
	inline static void		memoryCopy		( char *p, char *q, size_t l )	{if (p) if (q) while (l--) *p++ = *q++;};
	inline static void		stringCopy		( char *p, char *q )		{if (p) if (q) while ( *q) *p++ = *q++;};
  };
}	// End namespace ldapcppei

#endif // header guard
