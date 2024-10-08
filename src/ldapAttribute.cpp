# include <libldapcppei/ldapAttribute.h>

namespace ldapcppei
{
// ******************************************** (Cons/Des)tructors:
attribute::attribute( void ) : m_mod_op(LDAP_MOD_BVALUES), m_iter(m_values.end()) {
}
attribute::attribute( char const *dn ) : m_identifier(dn), m_mod_op(LDAP_MOD_BVALUES), m_iter(m_values.end()) {
}
attribute::attribute( std::string const &dn ) : m_identifier(dn), m_mod_op(LDAP_MOD_BVALUES), m_iter(m_values.end()) {
}
attribute::attribute( std::string dn, int mod ) : m_identifier(dn), m_mod_op(mod|LDAP_MOD_BVALUES), m_iter(m_values.end()) {
}
attribute::attribute( std::string dn, int mod, char const *value ) : m_identifier(dn), m_mod_op(mod|LDAP_MOD_BVALUES) {
	insert( value );
	m_iter = m_values.begin();
}
attribute::attribute( std::string dn, int mod, std::string value ) : m_identifier(dn), m_mod_op(mod|LDAP_MOD_BVALUES) {
	insert( value );
	m_iter = m_values.begin();
}
attribute::attribute( std::string dn, int mod, BerValue const *value ) : m_identifier(dn), m_mod_op(mod|LDAP_MOD_BVALUES) {
	insert( value );
	m_iter = m_values.begin();
}
attribute::attribute( std::string dn, int mod, BerValue const &value ) : m_identifier(dn), m_mod_op(mod|LDAP_MOD_BVALUES) {
	insert( value );
	m_iter = m_values.begin();
}
attribute::attribute( std::string dn, int mod, berValue const *value ) : m_identifier(dn), m_mod_op(mod|LDAP_MOD_BVALUES) {
	insert( value );
	m_iter = m_values.begin();
}
attribute::attribute( std::string dn, int mod, berValue const &value ) : m_identifier(dn), m_mod_op(mod|LDAP_MOD_BVALUES) {
	insert( value );
	m_iter = m_values.begin();
}
attribute::attribute( std::string dn, int mod, berValue *v, bool del ) : m_identifier(dn), m_mod_op(mod|LDAP_MOD_BVALUES) {
	insert( v, del );
	m_iter = m_values.begin();
}

attribute::attribute( std::string const dn, int mod, char const *values[] ) : m_identifier(dn), m_mod_op(mod|LDAP_MOD_BVALUES) {
	insert( values );
	m_iter = m_values.begin();
}
attribute::attribute( std::string const dn, int mod, BerValue const *values[] ) : m_identifier(dn), m_mod_op(mod|LDAP_MOD_BVALUES) {
	insert( values );
	m_iter = m_values.begin();
}
attribute::attribute( std::string const dn, int mod, berValue const *values[] ) : m_identifier(dn), m_mod_op(mod|LDAP_MOD_BVALUES) {
	insert( values );
	m_iter = m_values.begin();
}
attribute::attribute( std::string dn, int mod, berValue *values[], bool del ) : m_identifier(dn), m_mod_op(mod|LDAP_MOD_BVALUES) {
	insert( values, del );
	m_iter = m_values.begin();
}

attribute::attribute( LDAPMod const *value ) {
	if ( value ) copy( *value );
}
attribute::attribute( LDAPMod const &value ) {
	copy( value );
	m_iter = m_values.begin();
}
attribute::attribute( attribute const *value ) {
	if ( value ) copy( *value );
	m_iter = m_values.begin();
}
attribute::attribute( attribute const &value ) {
	copy( value );
	m_iter = m_values.begin();
}

attribute::~attribute() {for( auto iter : m_values ) delete iter;}
// *****************************************************************
void	attribute::copy( attribute const &value ) {
	if ( !this->isNULL() ) {
		clear(); setIdentifier( value.getIdentifier() );
		if ( !value.isNULL() ) {
			setMod_op( value.getMod_op() );
			for( auto iter : value.m_values )
				fastInsert( new berValue( iter ) );
			m_iter = ( value.m_iter==value.m_values.end() ? m_values.end() : m_values.find( *value.m_iter ) );
}	}	}
void	attribute::copy( LDAPMod const &value ) {
	if ( !this->isNULL() ) {
		clear(); setIdentifier( value.mod_type );
		setMod_op( value.mod_op );
		if ( value.mod_bvalues )
			for( BerValue **iter=value.mod_bvalues; *iter; iter++ )
				fastInsert( new berValue ( *iter ) );
		else if ( value.mod_values )
			for( char **iter=value.mod_values; *iter; iter++ )
				fastInsert( new berValue ( *iter ) );
	} m_iter = m_values.begin();
}

attribute&	attribute::operator+=( attribute const *value ) {
	if ( value && !value->isNULL() && !isNULL() && isName( value->getIdentifier() ) )
		for( auto iter : value->m_values )
			if ( m_values.find( iter ) == m_values.end() )
				fastInsert( new berValue( iter ) );
	m_iter = m_values.begin();
	return *this;
}

attribute&	attribute::operator-=( attribute const *value ) {
	if ( value && !value->isNULL() && !isNULL() && isName( value->getIdentifier() ) )
		for( auto iter : value->m_values )
			if ( (m_iter=m_values.find( iter )) != m_values.end() )
				{delete *m_iter; m_values.erase( m_iter );}
	m_iter = m_values.begin();
	return *this;
}

attribute&	attribute::operator&=( attribute const *value ) {
	if ( value && !value->isNULL() && !isNULL() && isName( value->getIdentifier() ) )
		for( auto iter : m_values )
			if ( value->m_values.find( iter ) == value->m_values.end() )
				{delete iter; m_values.erase( iter );}
	m_iter = m_values.begin();
	return *this;
}

bool	attribute::operator==( attribute const *that ) const {
	if ( ! that )
		return( ! getIdentifier().size() ? empty() : false );
	bool	ret = ( isName( that->getIdentifier() ) && getMod_op()==that->getMod_op() && size()==that->size() );
	for( auto iterThis=m_values.begin(), iterThat=that->m_values.begin();
			ret && iterThis != m_values.end() && iterThat != that->m_values.end();
			iterThis++, iterThat++ )
		ret = ( **iterThis == *iterThat );
	return ret;
}

}	// End namespace ldapcppei

