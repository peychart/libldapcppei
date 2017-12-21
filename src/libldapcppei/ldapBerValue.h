#ifndef HEADER_FFD95501E8C14D81
#define HEADER_FFD95501E8C14D81

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
#include <libldapcppei/ldapByteArea.h>
#include <ldap.h>

namespace ldapcppei
{ class berValue : public byteArea {
    public:
	berValue( void );
	berValue( char const * );
	berValue( std::string const & );
	berValue( size_t, byte const * );
	berValue( BerValue const * );
	berValue( BerValue const & );
	berValue( berValue const * );
	berValue( berValue const & );
	berValue( byteArea );

	inline char const *	toStr		( void ) const	{static std::string ret; ret = fromUtf8().c_str(); return ret.c_str();};
	inline char const *	toCode64	( void ) const	{static std::string ret; ret = encode64().c_str(); return ret.c_str();};
	friend std::ostream&	operator<<	( std::ostream& out, berValue const *v )		{return( out << ( (v==nullptr) ? "" : v->toStr() ) );};
	friend std::ostream&	operator<<	( std::ostream& out, berValue const &v )		{return( out << v.toStr() );};
  };
}	// End namespace ldapcppei

/* Pour mémoire:
typedef struct berval {
	ber_len_t	bv_len;
	char		*bv_val;
} BerValue;
*/

#endif // header guard
