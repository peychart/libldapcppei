#include <libldapcppei/ldapSet.h>

namespace ldapcppei
{
// ******************************************** (Cons/Des)tructors:
ldapSet::ldapSet( void ) : m_iter(m_values.end()) {
}
ldapSet::ldapSet( ldapSet const *value ) {
	if ( value ) copy( *value );
	gotoAllTops();
}
ldapSet::ldapSet( ldapSet const &value ) {
	copy( value );
	gotoAllTops();
}

ldapSet::~ldapSet( void ) {for( auto iter : m_values ) delete iter;}
// *******************************************************************************************************************************************************
void	ldapSet::copy( ldapSet const &value ) {
	eraseAll();
	for( auto iter : value.m_values )
		this->fastInsert( new entry ( iter ) );
	m_iter = ( value.m_iter==value.m_values.end() ? m_values.end() : m_values.find( *value.m_iter ) );
}

ldapSet&	ldapSet::eraseNullEntries( void ) {
	for( auto iter : m_values ) {
		iter->eraseNullAttributes();
		if ( iter->empty() ) {
			delete iter; m_values.erase( iter );
	}	}
	m_iter = m_values.begin();
	return *this;
}

ldapSet&	ldapSet::operator+=( ldapSet const *value ) {
	if ( value ) for( auto iter : value->m_values ) {
		if ( (m_iter=m_values.find( iter )) != m_values.end() )
			{delete *m_iter; m_values.erase( m_iter );}
		this->fastInsert ( new entry ( iter ) );
	} m_iter = m_values.begin();
	return *this;
}

ldapSet&	ldapSet::operator-=( ldapSet const *value ) {
	if ( value ) for( auto iter : value->m_values )
		if ( (m_iter=m_values.find( iter )) != m_values.end()
				&& !(**m_iter -= iter).size() )
			{delete *m_iter; m_values.erase( m_iter );}
	m_iter = m_values.begin();
	return *this;
}

ldapSet&	ldapSet::operator&=( ldapSet const *value ) {
	if ( value )
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

ldapSet&	ldapSet::operator|=( ldapSet const *value ) {
	if ( value ) for( auto iter : value->m_values )
		if ( (m_iter=m_values.find ( iter )) != m_values.end() )
			**m_iter |= iter;
		else	this->fastInsert( new entry ( iter ) );
	m_iter = m_values.begin();
	return *this;
}

bool	ldapSet::operator==( ldapSet const *that ) const {
	if ( ! that )
		return ( ! this->size() );
	bool	ret = ( this->size() == that->size() );
	for( auto iterThis=this->m_values.begin(), iterThat=that->m_values.begin();
			ret && iterThis != this->m_values.end() && iterThat != that->m_values.end();
			iterThis++, iterThat++ )
		ret = ( **iterThis == *iterThat );
	return ret;
}

bool	ldapSet::gotoPrevious( size_t l ) {
	if ( m_iter == m_values.begin() )
		return false;
	for( m_iter--; m_iter != m_values.begin() && m_iter != m_values.end() && l && (*m_iter)->dnSize() != l; )
		m_iter--;
	return( (m_iter != m_values.end() && !l) || (*m_iter)->dnSize() == l );
}

bool	ldapSet::gotoNext( size_t l ) {
	if ( m_iter == m_values.end() )
		return false;
	for( m_iter++; m_iter != m_values.end() && l && (*m_iter)->dnSize() != l; )
		m_iter++;
	return( m_iter != m_values.end() );
}

}	// End namespace ldapcppei

