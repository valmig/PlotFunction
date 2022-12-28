#ifndef VAL_WX_UTILS_INCLUDE
#define VAL_WX_UTILS_INCLUDE

#include <wx/wx.h>
#include <val_utils.h>

namespace val
{

template <class T> wxString TowxString(const T& a,unsigned prec=6);
template <class T> T FromString(const wxString &);

template <class T>
wxString TowxString(const T& a,unsigned prec)
{
	return wxString(ToString(a,prec));
}

template <class T>
T FromString(const wxString &s)
{
	return FromString<T>( std::string(s) ); 
}


} // end namespace val

#endif
