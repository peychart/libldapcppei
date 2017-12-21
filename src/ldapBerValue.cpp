# include <libldapcppei/ldapBerValue.h>

namespace ldapcppei
{
// ******************************************** (Cons/Des)tructors:
berValue::berValue( void )					{}
berValue::berValue( char const *s )				{newArea( stringLen(s), (byte const *)s );}
berValue::berValue( std::string const &s )			{newArea( s.size(), (byte const *)s.c_str() );}
berValue::berValue( size_t len, byte const *s )			{newArea( len, (byte const *)s );}
berValue::berValue( BerValue const *value )			{if ( value != nullptr ) newArea( value->bv_len, (byte const *)value->bv_val );}
berValue::berValue( BerValue const &value )			{newArea( value.bv_len, (byte const *)value.bv_val );}
berValue::berValue( berValue const *value )			{if ( value != nullptr ) newArea( value->size(), value->area() );}
berValue::berValue( berValue const &value )			{newArea( value.size(), value.area() );}
berValue::berValue( byteArea value )				{newArea( value.size(), value.area() );}
// ****************************************************************
}	// End namespace ldapcppei

