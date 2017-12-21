# include <libldapcppei/ldapEntry.h>

namespace ldapcppei
{
// ******************************************** (Cons/Des)tructors:
entry::entry ( void ) : m_iter(m_values.end()) {
}
entry::entry ( char const *dn ) : m_identifier(dn?dn:(char)0), m_iter(m_values.end()) {
}
entry::entry ( std::string const &dn ) : m_identifier(dn), m_iter(m_values.end()) {
}
entry::entry( std::string dn, std::string attrName, int mod ) : m_identifier(dn), m_iter(m_values.end()) {
	insert( new attribute( attrName, mod ), true );
}
entry::entry( std::string dn, std::string attrName, int mod, char const *value ) : m_identifier(dn) {
	insert( new attribute( attrName, mod, value ), true );
	gotoAllTops();
}
entry::entry( std::string dn, std::string attrName, int mod, std::string value ) : m_identifier(dn) {
	insert( new attribute( attrName, mod, value ), true );
	gotoAllTops();
}
entry::entry( std::string dn, std::string attrName, int mod, BerValue const *value ) : m_identifier(dn) {
	insert( new attribute( attrName, mod, value ), true );
	gotoAllTops();
}
entry::entry( std::string dn, std::string attrName, int mod, BerValue const &value ) : m_identifier(dn) {
	insert( new attribute( attrName, mod, value ), true );
	gotoAllTops();
}
entry::entry( std::string dn, std::string attrName, int mod, berValue const *value ) : m_identifier(dn) {
	insert( new attribute( attrName, mod, value ), true );
	if ( gotoBegin() ) getAttribute()->gotoBegin();
}
entry::entry( std::string dn, std::string attrName, int mod, berValue const &value ) : m_identifier(dn) {
	insert( new attribute( attrName, mod, value ), true );
	gotoAllTops();
}
entry::entry( std::string dn, std::string attrName, int mod, berValue *value, bool del ) : m_identifier(dn) {
	insert( new attribute( attrName, mod, value, del ), true );
	gotoAllTops();
}

entry::entry( std::string dn, std::string attrName, int mod, char const *values[] ) : m_identifier(dn) {
	if ( values != nullptr )
		for ( char const **value=values; *value; value++ )
			insert( new attribute( attrName, mod, *value ), true );
	gotoAllTops();
}
entry::entry( std::string dn, std::string attrName, int mod, BerValue const *values[] ) : m_identifier(dn) {
	if ( values != nullptr )
		for ( BerValue const **value=values; *value; value++ )
			insert( new attribute( attrName, mod, *value ), true );
	gotoAllTops();
}
entry::entry( std::string dn, std::string attrName, int mod, berValue const *values[] ) : m_identifier(dn) {
	if ( values != nullptr )
		for ( berValue const **value=values; *value; value++ )
			insert( new attribute( attrName, mod, *value ), true );
	gotoAllTops();
}
entry::entry( std::string dn, std::string attrName, int mod, berValue *values[], bool del ) : m_identifier(dn) {
	if ( values != nullptr )
		for ( berValue **value=values; *value; value++ ) {
			insert( new attribute( attrName, mod, *value ), true );
			if ( del )	*value = nullptr;
		}
	gotoAllTops();
}

entry::entry( std::string dn, LDAPMod const *value ) : m_identifier(dn) {
	insert( new attribute( value ), true );
	gotoAllTops();
}

entry::entry( std::string dn, LDAPMod const &value ) : m_identifier(dn) {
	insert( new attribute( value ), true );
	gotoAllTops();
}

entry::entry( std::string dn, attribute const *value ) : m_identifier(dn) {
	insert( value );
	gotoAllTops();
}

entry::entry( std::string dn, attribute const &value ) : m_identifier(dn) {
	insert( value );
	gotoAllTops();
}
entry::entry( std::string dn, attribute *value, bool del ) : m_identifier(dn) {
	insert( value, del );
	gotoAllTops();
}

entry::entry( std::string dn, LDAPMod const *values[] ) : m_identifier(dn) {
	if ( values != nullptr )
		for ( LDAPMod const **value=values; *value; value++ )
			insert( *value );
	gotoAllTops();
}
entry::entry( std::string dn, attribute const *values[] ) : m_identifier(dn) {
	if ( values != nullptr )
		for ( attribute const **value=values; *value; value++ )
			insert( *value );
	gotoAllTops();
}
entry::entry( std::string dn, attribute *values[], bool del ) : m_identifier(dn) {
	if ( values != nullptr )
		for ( attribute **value=values; *value; value++ ) {
			insert( *value, del );
			if ( del )	*value = nullptr;
		}
	gotoAllTops();
}

entry::entry( entry const *value ) {
	if ( value ) copy( *value );
	gotoAllTops();
}
entry::entry( entry const &value ) {
	copy( value );
	gotoAllTops();
}

entry::~entry( void ) {for( auto iter : m_values ) delete iter;}
// ******************************************************************
void	entry::copy( entry const &value ) {
	if ( ! this->isNULL() ) {
		clear(); setIdentifier( value.getIdentifier() );
		if ( !value.isNULL() ) {
			for( auto iter : value.m_values )
				fastInsert( new attribute( iter ) );
			m_iter = ( (value.m_iter == value.m_values.end()) ? m_values.end() : m_values.find( *(value.m_iter) ) );
}	}	}

entry&	entry::eraseNullAttributes( void ) {
	for( auto iter : m_values ) {
		iter->eraseNullValues();
		if ( iter->empty() || iter->ldap_mod_op() == LDAP_MOD_DELETE )
			{delete iter; m_values.erase( iter );}
	} m_iter = m_values.begin();
	return *this;
}

std::string	entry::swapDn( std::string const &s ) {
	std::string	ret;
	int		i(0);
	for( char *p=const_cast<char*>(s.c_str()); *p; p++ )
		if ( *p == ',' )
			ret.insert( (i=0), 1, *p );
		else	ret.insert( i++, 1, (*p > 96 ) ? *p : (*p + 32) );
	return ret;
}

entry&	entry::operator+=( entry const *value ) {
	if ( value && !isNULL() && isDn( value->getIdentifier() ) ) {
		for( auto iter : value->m_values ) {
			if ( (m_iter=m_values.find( iter )) != m_values.end() )
				{delete *m_iter; m_values.erase( m_iter );}
			fastInsert( new attribute( iter ) );
	}	}
	m_iter = m_values.begin();
	return *this;
}

entry&	entry::operator-=( entry const *value ) {
	if ( value && !isNULL() && isDn( value->getIdentifier() ) )
		for( auto iter : value->m_values )
			this->operator-=( iter );
	m_iter = m_values.begin();
	return *this;
}

entry&	entry::operator-=( attribute const *value ) {
	if ( value && !isNULL() )
		if ( (m_iter=m_values.find( const_cast<attribute*>(value) )) != m_values.end()
				&& !(**m_iter -= value).size() )
			{delete *m_iter; m_values.erase( m_iter++ );}
	return *this;
}
entry&	entry::operator&=( entry const *value ) {
	if ( value && !value->isNULL() && !isNULL() && isDn( value->getIdentifier() ) )
		for( auto iter : m_values ) {
			m_iter = value->m_values.find( iter );
			if ( m_iter != value->m_values.end() ) {
				if ( ! (*iter &= *m_iter).size() )
					{delete iter; m_values.erase( iter );}
			}else{	delete iter; m_values.erase( iter );
		}	}
	m_iter = m_values.begin();
	return *this;
}

entry&	entry::operator|=( entry const *value ) {
	if ( value && !isNULL() && isDn( value->getIdentifier() ) )
		for( auto iter : value->m_values )
			if ( (m_iter=m_values.find( iter )) != m_values.end() )
				**m_iter |= iter;
			else	fastInsert( new attribute( iter ) );
	m_iter = m_values.begin();
	return *this;
}

bool	entry::operator==( entry const *that ) const {
	if ( ! that )
		return( ! getIdentifier().size() ? empty() : false );
	bool	ret = ( isDn( that->getIdentifier() ) && size()==that->size() );
	for( auto iterThis=m_values.begin(), iterThat=that->m_values.begin();
			ret && iterThis != m_values.end() && iterThat != that->m_values.end();
			iterThis++, iterThat++ )
		ret = ( **iterThis == *iterThat );
	return ret;
}

}	// End namespace ldapcppei

