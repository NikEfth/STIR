//
// $Id$: $Date$
//
/*!

  \file
  \ingroup buildblock
  \brief Declaration of class Succeeded

  \author Kris Thielemans
  \author PARAPET project

  \date $Date$
  \version $Revision$
*/
#include "tomo/common.h"

START_NAMESPACE_TOMO

/*! 
  \brief 
  a class containing an enumeration type that can be used by functions to signal 
  successful operation or not

  Example:
  \code
  Succeeded f() { do_something;  return Succeeded::yes; }
  void g() { if (f() == Succeeded::no) error("Error calling f"); }
  \endcode
*/
class Succeeded
{
public:
  enum value { yes, no };
  Succeeded(const value& v) : v(v) {}
  bool operator==(const Succeeded &v2) { return v == v2.v; }
  bool operator!=(const Succeeded &v2) { return v != v2.v; }
private:
  value v;
};

END_NAMESPACE_TOMO
