# include <libldapcppei/ldapEasyInterface.h>

namespace ldapcppei
{
// ******************************************** (Cons/Des)tructors:
ldapEasyInterface::ldapEasyInterface( char const *u ) : m_desc(nullptr), m_errNo(LDAP_SUCCESS), m_ldapVersion(LDAP_VERSION3), m_ldapSizeLimit(LDAP_NO_LIMIT) {
	m_bindPwd.bv_val = nullptr; m_bindPwd.bv_len = 0;
	uri( u ? u : std::string() ); m_opennedUriIter = m_uries.end();

	m_input = &m_set1; m_output = &m_set2;
}
ldapEasyInterface::ldapEasyInterface( std::string const &u ) : m_desc(nullptr), m_errNo(LDAP_SUCCESS), m_ldapVersion(LDAP_VERSION3), m_ldapSizeLimit(LDAP_NO_LIMIT) {
	m_bindPwd.bv_val = nullptr; m_bindPwd.bv_len = 0;
	uri( u ); m_opennedUriIter = m_uries.end();

	m_input = &m_set1; m_output = &m_set2;
}
ldapEasyInterface::ldapEasyInterface( ldapEasyInterface const *value ) : m_desc(nullptr), m_errNo(LDAP_SUCCESS), m_ldapVersion(LDAP_VERSION3), m_ldapSizeLimit(LDAP_NO_LIMIT) {
	m_bindPwd.bv_val = nullptr; m_bindPwd.bv_len = 0;
	if ( !value )
		return;
	m_uries.insert( m_uries.begin(), value->m_uries.begin(), value->m_uries.end() );
	m_opennedUriIter = m_uries.end(); m_ldapVersion = value->m_ldapVersion; m_ldapSizeLimit = value->m_ldapSizeLimit;
	setBindDn( value->m_bindDn.c_str(), value->m_bindPwd.bv_val ); m_searchFilter = value->m_searchFilter;
	m_input = &m_set1; m_output = &m_set2;

	*m_input  = *value->m_input;
	*m_output = *value->m_output;

}
ldapEasyInterface::ldapEasyInterface( ldapEasyInterface const &value ) : m_desc(nullptr), m_errNo(LDAP_SUCCESS) {
	m_bindPwd.bv_val = nullptr; m_bindPwd.bv_len = 0;
	eraseAll();

	m_uries.insert( m_uries.begin(), value.m_uries.begin(), value.m_uries.end() );
	m_opennedUriIter = m_uries.end(); m_ldapVersion = value.m_ldapVersion; m_ldapSizeLimit = value.m_ldapSizeLimit;
	setBindDn( value.m_bindDn.c_str(), value.m_bindPwd.bv_val ); m_searchFilter = value.m_searchFilter;
	m_input = &m_set1; m_output = &m_set2;

	*m_input  = *value.m_input;
	*m_output = *value.m_output;

}

ldapEasyInterface::~ldapEasyInterface( void ) {
	while ( doLifoRestoreEntries( m_output ) );
	for( auto iter = m_namedBuffer.begin(); iter != m_namedBuffer.end(); iter++ )
		if ( (*iter).second != &m_set1 && (*iter).second != &m_set2 )
			delete (*iter).second;
	close (); setBindDn(); eraseAll();
	if ( m_input  != &m_set1 && m_input  != &m_set2 )	delete m_input;
	if ( m_output != &m_set1 && m_output != &m_set2 )	delete m_output;
}
// ******************************************** Connection Init.:
ldapEasyInterface&	ldapEasyInterface::uri( std::string uries ) {
	if ( uries.empty() ) {
		m_errNo = LDAP_SUCCESS;
		return clearUri();
	} bool ret = true;
	// Initialize m_uries whith all the URIs in "uries" ...
	for( int len=uries.length(),i=0,ii=0; i < len; ) { LDAPURLDesc	*l;
		if ( (ii = uries.find( ' ', i )) < 0 )	ii=len;
		if ( ii > i ) {std::string newUri ( uries.substr(i,ii-i).c_str() );
			// URIs format check ...
			if ( ldap_url_parse( newUri.c_str(), &l) == 0 )
				m_uries.push_back( newUri );
			else	{ldap_free_urldesc( l ); ret = false;}
		} i = ++ii;
	} m_opennedUriIter = m_uries.end();
	m_errNo = (ret ? LDAP_SUCCESS : LDAP_URL_ERR_PARAM );
	return *this;
}

std::string	ldapEasyInterface::uri( void ) {
	std::string	ret;
	for( auto iter : m_uries )
		ret.append( iter + " " );
	m_errNo = LDAP_SUCCESS;
	return ( ret );
}

ldapEasyInterface&	ldapEasyInterface::setBindDn( char const *dn, char const *pwd ) {
	m_errNo = LDAP_SUCCESS;
	if ( ! dn || ! *dn ) {	// Erase m_BinDn ...
		m_bindPwd.bv_len = 0; if ( m_bindPwd.bv_val ) {ber_memfree( m_bindPwd.bv_val ); m_bindPwd.bv_val = nullptr;}
		m_bindDn.clear();
		return *this;
	}else	m_bindDn.assign ( dn );
	m_bindPwd.bv_len = 0;
	if ( !pwd ) {
		if ( m_bindPwd.bv_val ) ber_memfree( m_bindPwd.bv_val );
		return *this;
	}

	if ( m_bindPwd.bv_val ) ber_memfree( m_bindPwd.bv_val );
	if ( (m_bindPwd.bv_val = ber_strdup ( pwd )) == nullptr ) {
		m_errNo = LDAP_NO_MEMORY;
		return *this;
	} m_bindPwd.bv_len = strlen (m_bindPwd.bv_val);
	return *this;
}

std::string	ldapEasyInterface::bindPwd( void ) {
	m_errNo = LDAP_SUCCESS;
	std::string	ret;
	if ( m_bindPwd.bv_len )
		ret.append( m_bindPwd.bv_val );
	return ret;
}

ldapEasyInterface&	ldapEasyInterface::setOption( int option, void const *invalue ) {
	if ( ! option )
		{m_errNo = LDAP_PARAM_ERROR; return *this;}
	if ( ! m_desc )
		{m_errNo = LDAP_UNAVAILABLE; return *this;}
	m_errNo = ldap_set_option ( m_desc, option, invalue );
	return *this;
}

ldapEasyInterface&	ldapEasyInterface::getOption( int option, void *outvalue ) {
	if ( ! option )
		{m_errNo = LDAP_PARAM_ERROR; return *this;}
	if ( ! m_desc || m_opennedUriIter == m_uries.end() )
		{m_errNo = LDAP_UNAVAILABLE; return *this;}
	m_errNo = ldap_get_option( m_desc, option, outvalue );
	return *this;
}

bool	ldapEasyInterface::open( std::string uries, char const *dn, char const *pwd ) {
	if ( uries.size() ) if ( uri( uries ).errNo() )
		{m_errNo = LDAP_PARAM_ERROR; return false;}
	if ( dn && setBindDn( dn, pwd ).errNo() )
		return false;
	if ( m_opennedUriIter != m_uries.end() )
		close ();
	if ( m_desc || ! m_uries.size() ) {	// Already open !...
		if ( m_desc )
			m_errNo = LDAP_TYPE_OR_VALUE_EXISTS;
		else	m_errNo = LDAP_PARAM_ERROR;
		return false;
	} m_errNo = LDAP_URL_ERR_PARAM;

	m_opennedUriIter = m_uries.end();
	for( auto iter=m_uries.begin(); iter != m_uries.end(); ++iter ) {
		if ( (m_errNo=ldap_initialize ( &m_desc, iter->c_str() )) == LDAP_SUCCESS ) {
			if ( setOption ( LDAP_OPT_PROTOCOL_VERSION, &m_ldapVersion).errNo() == LDAP_SUCCESS ) { BerValue *srvcredp;
				if ( (m_errNo=ldap_sasl_bind_s( m_desc, m_bindDn.c_str(), LDAP_SASL_SIMPLE, &m_bindPwd, nullptr, nullptr, &srvcredp )) == LDAP_SUCCESS ) {
					ber_bvfree ( srvcredp ); m_opennedUriIter = iter; break;
				}else	{ldap_memfree ( m_desc ); m_desc = nullptr;}
			}else{	ldap_memfree ( m_desc );m_desc = nullptr;	}
	}	}
	return( m_errNo == LDAP_SUCCESS );
}

bool	ldapEasyInterface::close( void ) {
	if ( !m_desc )	// Already closed ...
		m_errNo = LDAP_UNAVAILABLE;
	else{	eraseAll();
		m_opennedUriIter = m_uries.end();
		 if ( (m_errNo=ldap_unbind_ext_s ( m_desc, nullptr, nullptr )) == LDAP_SUCCESS)
			m_desc = nullptr;
	} return( m_errNo == LDAP_SUCCESS );
}

// ****************************************** Entries buffers ********************************************************
ldapSet*	ldapEasyInterface::doLifoBackupEntries( ldapSet* &entries ) {
	ldapSet	*newSet;
	if ( ! (newSet = new ldapSet) ) {
		m_errNo = LDAP_NO_MEMORY;
		return nullptr;
	} m_lifoBuffer.push( entries ); entries = newSet;
	m_errNo = LDAP_SUCCESS;
	return m_lifoBuffer.top();
}

ldapSet*	ldapEasyInterface::doNamedBackupEntries( std::string name, ldapSet* &entries ) {
	ldapSet	*newSet;
	if ( ! (newSet = new ldapSet) ) {
		m_errNo = LDAP_NO_MEMORY;
		return nullptr;
	} if ( m_namedBuffer.find( name ) != m_namedBuffer.end() ) {
		if ( m_namedBuffer[name] == &m_set1 || m_namedBuffer[name] == &m_set2 ) {
			*m_namedBuffer[name] = entries;
			entries = newSet;
			m_errNo = LDAP_SUCCESS;
			return m_namedBuffer[name];
		}else	delete m_namedBuffer[name];
		m_namedBuffer.erase( name );
	} m_namedBuffer[name] = entries; entries = newSet;
	m_errNo = LDAP_SUCCESS;
	return m_namedBuffer[name];
}

bool	ldapEasyInterface::doLifoRestoreEntries( ldapSet* &entries ) {
	if ( ! m_lifoBuffer.size() ) {
		m_errNo = LDAP_NO_SUCH_OBJECT;
		return false;
	} if ( entries == &m_set1 || entries == &m_set2 ) {
		*entries = m_lifoBuffer.top();
	}else{	delete entries; entries = m_lifoBuffer.top();
	} m_lifoBuffer.pop();
	m_errNo = LDAP_SUCCESS;
	return true;
}

bool	ldapEasyInterface::doNamedRestoreEntries( std::string name, ldapSet* &entries ) {
	if ( m_namedBuffer.find( name ) == m_namedBuffer.end() ) {
		m_errNo = LDAP_NO_SUCH_OBJECT;
		return false;
	} if ( entries == &m_set1 || entries == &m_set2 ) {
		*entries = m_namedBuffer[name]; delete m_namedBuffer[name];
	}else{	delete entries ; entries = m_namedBuffer[name];
	} m_namedBuffer.erase( name );
	m_errNo = LDAP_SUCCESS;
	return true;
}

ldapEasyInterface&	ldapEasyInterface::doSwapEntriesWithResults( int mod ) {
	ldapSet	*tmp = m_input; m_input = m_output; m_output = tmp;
	m_input->ldap_mod_op ( mod );
	m_errNo = LDAP_SUCCESS;
	return *this;
}

// ************************************************************************************************** Add/Modify/Delete LDAP Entrie:
// cf.: http://www.mozilla.org/directory/csdk-docs/addmod.htm
ldapEasyInterface&	ldapEasyInterface::doAddEntries( void ) {
	m_output->eraseAll(); int errNo;
	if ( ! m_desc || m_opennedUriIter == m_uries.end() ) {
		doSwapEntriesWithResults(); m_errNo = LDAP_UNAVAILABLE;
		m_input->gotoBegin();										return *this;
	} else if ( ! m_input->gotoBegin() ) {
		m_errNo = LDAP_NO_SUCH_OBJECT;									return *this;
	} LDAPMod **tmpMods=nullptr; m_errNo = LDAP_SUCCESS;

	do {	if ( m_input->getEntry()->isNULL() ) {
			m_input->erase();
			continue;
		} else if ( !convert( m_input->getEntry(), LDAP_MOD_ADD|LDAP_MOD_BVALUES, tmpMods ) ) {
			freeMemory( tmpMods );
			m_output->insert( m_input->getEntry() ); m_input->erase();
			m_errNo = LDAP_NO_MEMORY;								return *this;
		}

		errNo = ldap_add_ext_s( m_desc, byteArea( m_input->getEntry()->getIdentifier() ).toUtf8().c_str(), tmpMods, nullptr, nullptr );
		freeMemory( tmpMods );

		if ( errNo != LDAP_SUCCESS ) {
			m_output->insert( m_input->getEntry() ); m_input->erase();
			m_errNo = ( (m_errNo!=LDAP_SUCCESS) ? ((m_errNo!=errNo) ? LDAP_NO_RESULTS_RETURNED : errNo) : errNo);
		}else	m_input->erase();

	} while ( ! m_input->end() );
	if ( m_errNo ) m_output->gotoEnd(); else m_output->gotoBegin();
	return *this;
}

ldapEasyInterface&	ldapEasyInterface::doModifyEntry( std::string dn, std::vector<attribute> *aa ) {
	LDAPMod	**tmpMods, **p;
	m_output->eraseAll();
	if ( ! m_desc || m_opennedUriIter == m_uries.end() ) {
		m_errNo = LDAP_UNAVAILABLE;
		m_input->gotoBegin();										return *this;
	} else if ( !aa ) {
		m_errNo = LDAP_NO_SUCH_OBJECT;									return *this;
	} if ( !(tmpMods = new LDAPMod*[aa->size()+1]) ) {
		m_errNo = LDAP_NO_MEMORY;									return *this;
	} p=tmpMods; m_errNo = LDAP_SUCCESS;

	for( auto m_attr=aa->begin(); m_attr != aa->end(); m_attr++ ) {
		if ( (*m_attr).isNULL() ) {
			continue;
		}else{	if ( ! convert( &(*m_attr), *(p++) ) ) {
			freeMemory( tmpMods );									return *this;
		}	}
	} *p = nullptr;

	m_errNo = ldap_modify_ext_s( m_desc, byteArea( dn ).toUtf8().c_str(), tmpMods, nullptr, nullptr );

	freeMemory( tmpMods );
	m_input->gotoEnd();
	if ( m_errNo ) m_output->gotoEnd(); m_output->find( dn );
	return *this;
}

ldapEasyInterface&	ldapEasyInterface::doModifyEntries( void ) {
	m_output->eraseAll(); int errNo;
	if ( ! m_desc || m_opennedUriIter == m_uries.end() ) {
		doSwapEntriesWithResults(); m_errNo = LDAP_UNAVAILABLE;
		m_input->gotoBegin();										return *this;
	} else if ( ! m_input->gotoBegin() ) {
		m_errNo = LDAP_NO_SUCH_OBJECT;									return *this;
	} LDAPMod **tmpMods=nullptr; m_errNo = LDAP_SUCCESS;
	std::string	first( m_input->getEntry()->name() );

	do {	if ( m_input->getEntry()->isNULL() ) {
			m_input->erase();
			continue;
		} else if ( !convert( m_input->getEntry(), tmpMods ) ) {
			freeMemory( tmpMods );
			m_output->insert( m_input->getEntry() ); m_input->erase();
			m_errNo = LDAP_NO_MEMORY;								return *this;
		}

		errNo = ldap_modify_ext_s( m_desc, byteArea( m_input->getEntry()->getIdentifier()).toUtf8().c_str(), tmpMods, nullptr, nullptr );
		freeMemory ( tmpMods );

		if ( errNo != LDAP_SUCCESS ) {
			m_output->insert( m_input->getEntry() ); m_input->erase();
			m_errNo = ( (m_errNo!=LDAP_SUCCESS) ? ((m_errNo!=errNo) ? LDAP_NO_RESULTS_RETURNED : errNo) : errNo);
		}else	m_input->erase();

	} while ( ! m_input->end() );
	if ( m_errNo ) m_output->gotoEnd(); else m_output->find( first );
	return *this;
}

ldapEasyInterface&	ldapEasyInterface::doDeleteEntries( void ) {
	m_output->eraseAll(); int errNo;
	if ( ! m_desc || m_opennedUriIter == m_uries.end() ) {
		doSwapEntriesWithResults(); m_errNo = LDAP_UNAVAILABLE;
		m_input->gotoBegin();										return *this;
	} else if ( ! m_input->gotoBegin() ) {
		m_errNo = LDAP_NO_SUCH_OBJECT;									return *this;
	} m_input->gotoEnd(); m_errNo = LDAP_SUCCESS;

	for( int i=0, nb=m_input->size(); i < nb; i++ ) { m_input->gotoPrevious();
		if ( m_input->getEntry()->isNULL() ) {
			m_input->erase();
			continue;
		}

		if ( (errNo = ldap_delete_ext_s( m_desc, byteArea( m_input->getEntry()->getIdentifier()).toUtf8().c_str(), nullptr, nullptr) != LDAP_SUCCESS ) ) {
			m_output->insert( m_input->getEntry() ); m_input->erase();
			m_errNo = ( (m_errNo!=LDAP_SUCCESS) ? ((m_errNo!=errNo) ? LDAP_NO_RESULTS_RETURNED : errNo) : errNo);
		}else	m_input->erase();

	} if ( m_errNo ) m_output->gotoBegin(); else m_output->gotoEnd();
	return *this;
}

/*	inline ldapEasyInterface& doMoveEntryTo		( entry *e, std::string const newRnd )	{getInputSet()->clear(); insert( e ); return moveEntries( newRnd, 2 );};
	inline ldapEasyInterface& doRenameEntryTo	( entry *e, std::string const newRnd )	{getInputSet()->clear(); insert( e ); return moveEntries( newRnd, 1 );};
	inline ldapEasyInterface& doDuplicateEntryTo	( entry *e, std::string const newRnd )	{getInputSet()->clear(); insert( e ); return moveEntries( newRnd, 0 );};
	inline ldapEasyInterface& doDuplicateEntriesTo	( std::string const newRnd )		{return moveEntries( newRnd, 0 );};
//(cf ".h")	*/

ldapEasyInterface&	ldapEasyInterface::moveEntries( std::string const &newDn, int del ) {
	m_output->eraseAll(); int errNo;
	if ( !m_desc || m_opennedUriIter == m_uries.end() ) {
		m_errNo = LDAP_UNAVAILABLE;									return *this;
	} else if ( ! m_input->gotoBegin() || newDn.empty() ) {
		m_errNo = LDAP_NO_SUCH_OBJECT;									return *this;
	} std::string newRdn ( byteArea( newDn ).toUtf8().c_str() ), newParent;
	if ( del == 1 )	newParent.assign( byteArea( m_input->getEntry()->getIdentifier().substr( m_input->getEntry()->getIdentifier().find(",") + 1) ).toUtf8().c_str() );
	if ( del == 2 ) {newRdn.assign( m_input->getEntry()->getIdentifier() ); newRdn.assign( newRdn.substr ( 0, newRdn.find (",") ) ); newParent.assign( byteArea( newDn ).toUtf8().c_str() );}
	m_errNo = LDAP_SUCCESS;

	for( m_input->begin(); ! m_input->end(); m_input->gotoNext() ) {
		if ( m_input->getEntry()->isNULL() ) {
			m_input->erase();
			continue;
		}

		if ( (errNo = ldap_rename_s( m_desc, byteArea(m_input->getEntry()->getIdentifier()).toUtf8().c_str(),
						newRdn.c_str(), newParent.c_str(),
						del, nullptr, nullptr )) != LDAP_SUCCESS ) {
			m_output->insert( m_input->getEntry() ); m_input->erase();
			m_errNo = ( (m_errNo!=LDAP_SUCCESS) ? ((m_errNo!=errNo) ? LDAP_NO_RESULTS_RETURNED : errNo) : errNo);
		}else	m_input->erase();

	} m_output->gotoBegin();
	return *this;
}

// ******************************************** Search Entrie:
bool	ldapEasyInterface::setEntryAttributesListeAndFilterForSearch( std::string baseDn, std::vector<std::string> const &attributesListe, std::string filter ) {
	m_input->eraseAll();
	for( auto entryName : attributesListe ) {
		if ( m_input->insert( new entry( baseDn, entryName, LDAP_MOD_BVALUES ), true ).end() ) {
			m_errNo = LDAP_NO_SUCH_OBJECT;
			return false;
	}	}
	setSearchFilter( filter );
	m_errNo = LDAP_SUCCESS;
	return true;
}

bool	ldapEasyInterface::setEntryAttributesListeAndFilterForSearch( std::string baseDn, std::string attributesListe, std::string filter ) {
	m_input->eraseAll();
	if ( m_input->insert( new entry( baseDn ), true ).end() ) {
		m_errNo = LDAP_NO_SUCH_OBJECT;									return false;
	} for( int len=attributesListe.size(),i=0,ii=0; i < len; ) {
		if ( (ii = attributesListe.find( ' ', i )) < 0 ) if ( (ii = attributesListe.find( ',', i )) < 0 )
			if ( (ii = attributesListe.find( ';', i )) < 0 ) if ( (ii = attributesListe.find( '|', i )) < 0 )
				ii=len;
		if ( ii > i ) {
			if ( m_input->getEntry()->insert( new attribute( attributesListe.substr(i,ii-i), LDAP_MOD_BVALUES ), true ).end() ) {
				m_errNo = LDAP_NO_SUCH_OBJECT;							return false;
		}	}
		i = ++ii;
	} setSearchFilter( filter );
	m_errNo = LDAP_SUCCESS;
	return true;
}

// cf.: http://www.mozilla.org/directory/csdk-docs/search.htm#c6-example-searching-the-directory-synchronous
ldapEasyInterface&	ldapEasyInterface::doSearchForEntries( int scope ) {
	if ( !m_desc || m_opennedUriIter == m_uries.end() || m_input->empty() ) {
		m_errNo = LDAP_UNAVAILABLE;									return *this;
	} if ( !m_searchFilter.size() ) m_searchFilter = "(objectClass=*)";
	m_errNo = LDAP_SUCCESS;
	if ( m_input->gotoBegin() ) do {
		LDAPMessage *result=nullptr, *msg;
		char *baseName=nullptr, **attrs=nullptr;
		BerElement *ber=nullptr;

		// Particular baseDN or attributs?
		if ( ! m_input->end() && ! m_input->getEntry()->isNULL() ) {
			if ( (baseName=new char[m_input->getEntry()->getIdentifier().size()+1]) == nullptr )	return *this;
			memoryCopy( baseName, (char*)(m_input->getEntry()->getIdentifier().c_str()), m_input->getEntry()->getIdentifier().size()+1 );
			if ( ! convert(m_input->getEntry(), LDAP_MOD_BVALUES, attrs ) ) {
				m_errNo = LDAP_NO_MEMORY; freeMemory ( baseName );				return *this;
		}	}

		// Only "dn" attribut?
		if ( attrs ) if ( *attrs ) if ( attrs[1] == nullptr ) if ( strcmp( attrs[0], "dn" )==0 ) {
			freeMemory ( attrs );
			attrs = new char*[2]; if ( ! attrs ) {
				m_errNo = LDAP_NO_MEMORY; freeMemory( baseName );				return *this;
			} *attrs = new char[strlen((char *)LDAP_NO_ATTRS)+1]; if ( ! *attrs ) {
				delete[] attrs; freeMemory( baseName );
				m_errNo = LDAP_NO_MEMORY;							return *this;
			} stringCopy( *attrs, (char*)LDAP_NO_ATTRS ); attrs[1] = nullptr;
		}

		if ( (m_errNo=ldap_search_ext_s( m_desc, byteArea( baseName ).toUtf8().c_str(), scope, (char *)m_searchFilter.c_str(), attrs, 0,
				nullptr, nullptr, LDAP_NO_LIMIT, m_ldapSizeLimit, &result )) == LDAP_SUCCESS ) {
			for ( msg = ldap_first_message( m_desc, result ); msg != nullptr; msg = ldap_next_message( m_desc, msg ) ) {
		    	switch ( ldap_msgtype( msg ) ) {
				case LDAP_RES_SEARCH_ENTRY: {
					// Result entry:
					char *attr = ldap_get_dn( m_desc, msg );
					entry *r = new entry( byteArea( attr ).fromUtf8().c_str() );
					ldap_memfree( attr );

					if ( r == nullptr ) {
						ldap_msgfree( result ); freeMemory( attrs );
						freeMemory( baseName );
						m_errNo = LDAP_NO_MEMORY;					return *this;
					}else for ( attr=ldap_first_attribute( m_desc, msg, &ber ); attr != nullptr; attr=ldap_next_attribute( m_desc, msg, ber ) ) {
						BerValue const **b = (BerValue const **)ldap_get_values_len(m_desc, msg, attr);
						if ( r->insert( new attribute( attr, LDAP_MOD_BVALUES, b ), true ).end() ) {
							ldap_memfree( attr ); ldap_value_free_len( (BerValue **)b ); freeMemory( ber );
							ldap_msgfree( result ); freeMemory( attrs );
							freeMemory( baseName );
							m_errNo = LDAP_NO_MEMORY;				return *this;
						} ldap_memfree ( attr ); ldap_value_free_len( (BerValue **)b );
					} freeMemory ( ber );

					if ( m_output->insert( r, true ).end() ) {
						if ( ! m_output->getEntry()->isNULL() ) m_output->erase();
						if ( m_output->insert ( r, true ).end() ) {
							delete r;
							ldap_msgfree( result ); freeMemory( attrs );
							freeMemory( baseName );
							m_errNo = LDAP_NO_SUCH_OBJECT;				return *this;
					}	}
					if ( m_ldapSizeLimit ) if ( m_output->size() >= m_ldapSizeLimit ) {
						ldap_msgfree( result ); freeMemory( attrs );
						freeMemory( baseName );
						m_errNo = LDAP_RESULTS_TOO_LARGE;				return *this;
					}

					break;}
				case LDAP_RES_SEARCH_REFERENCE:
					// comming soon ...
					break;
				case LDAP_RES_SEARCH_RESULT:
					// comming soon ...
					break;
				default:break;
			}    } ldap_msgfree( result );
		} freeMemory( attrs ); freeMemory( baseName );

		if ( m_errNo == LDAP_SUCCESS )
			m_input->erase();
		else	m_input->gotoNext();

	} while ( ! m_input->end() );
	if ( m_errNo == LDAP_SUCCESS )
		m_output->gotoAllTops();
	return *this;
}

// ******************************************** TOOLS:
bool	ldapEasyInterface::convert( attribute *attr, LDAPMod* &lm ) {
	if ( !(lm = new LDAPMod) )										return false;
	lm->mod_op = attr->ldap_mod_op() | LDAP_MOD_BVALUES; lm->mod_vals.modv_bvals = nullptr;
	if ( ! (lm->mod_type = new char[attr->getIdentifier().size()+1]) ) {
		freeMemory( lm );										return false;
	} memoryCopy( (char*)(lm->mod_type), (char*)(attr->getIdentifier().data()), attr->getIdentifier().size()+1 );
	if ( attr->empty() ) {
		lm->mod_vals.modv_bvals = nullptr;
//		To protect from OpenLdap(ITS#6581):
//		if ( lm->mod_op == LDAP_MOD_BVALUES )								return false;
	}else{	 if ( ! (lm->mod_vals.modv_bvals = new BerValue*[attr->size()+1]) ) {
			freeMemory ( lm );									return false;
		} BerValue **p=lm->mod_vals.modv_bvals;
		for ( attr->gotoBegin(); !attr->end(); p++, attr->gotoNext() ) {
			if ( !(*p = new BerValue) ) {
				freeMemory ( lm );								return false;
			} (*p)->bv_len = attr->getValueSize(); (*p)->bv_val = nullptr;
			if ( !((*p)->bv_val = new char[attr->getValueSize()+1]) ) {
				freeMemory( lm );								return false;
			} memoryCopy( (char*)((*p)->bv_val), (char*)(attr->getBerValue()->data()), attr->getBerValue()->size()+1 );
		} *p = nullptr;
	} return true;
}

bool	ldapEasyInterface::convert( entry *e, LDAPMod** &lm ) {
	if ( ! e->size() ) { lm = nullptr; return true; }
	if ( !(lm = new LDAPMod*[e->size()+1]) )								return false;
	LDAPMod **p=lm; for ( e->gotoBegin(); ! e->end(); e->gotoNext() )
		if ( ! convert ( e->getAttribute(), *(p++) ) ) {
			freeMemory( lm );									return false;
		}
	*p = nullptr;
	return true;
}
bool	ldapEasyInterface::convert( entry *e, int ifMode, LDAPMod** &lm ) {
	if ( ! e->size() ) { lm = nullptr; return true; }
	if ( !(lm = new LDAPMod*[e->size()+1]) )								return false;
	LDAPMod **p=lm; for ( e->gotoBegin(); ! e->end(); e->gotoNext() )
		if ( e->getAttribute()->ldap_mod_op() == ifMode )
			if ( ! convert ( e->getAttribute(), *(p++) ) ) {
				freeMemory( lm );								return false;
		}
	*p = nullptr;
	return true;
}
bool	ldapEasyInterface::convert( entry *e, int ifMode, char** &attrNames ) {
	if ( ! e->size() ) { attrNames = nullptr; return true; }
	if ( !(attrNames = new char*[e->size()+1]) )								return false;
	char **p=attrNames; for ( e->gotoBegin(); ! e->end(); p++, e->gotoNext() ) {
		attribute *a = e->getAttribute();
		if ( a->ldap_mod_op() == ifMode ) {
			if ( !(*p = new char[a->getIdentifier().size()+1]) ) {
				freeMemory ( attrNames );							return false;
			} memoryCopy( *p, (char*)(a->getIdentifier().c_str()), a->getIdentifier().size()+1 );
	}	}
	*p = nullptr;
	return true;
}

void	ldapEasyInterface::freeMemory( LDAPMod* &m ) {
	BerValue **p=m->mod_vals.modv_bvals;
	if (p) {for ( ; *p; p++ ) {
			if ( (*p)->bv_val ) {
				memset( (*p)->bv_val, 0, (*p)->bv_len );
				delete[] (*p)->bv_val;
			} delete *p;
		} delete[] m->mod_vals.modv_bvals;
	} if ( m->mod_type ) delete[] m->mod_type; delete m; m = nullptr;
}

}	// End namespace ldapcppei

