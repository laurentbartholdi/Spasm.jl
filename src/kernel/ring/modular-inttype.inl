// ==========================================================================
// $Source: /var/lib/cvs/Givaro/src/kernel/zpz/givzpzGen.inl,v $
// Copyright(c)'1994-2009 by The Givaro group
// This file is part of Givaro.
// Givaro is governed by the CeCILL-B license under French law
// and abiding by the rules of distribution of free software.
// see the COPYRIGHT file for more details.
// Authors: JG Dumas
// $Id: givzpzGen.inl,v 1.11 2011-02-02 17:16:43 bboyer Exp $
// ==========================================================================
#ifndef __GIVARO_zpz_gen_INL
#define __GIVARO_zpz_gen_INL
// Description:

// ---------
// -- normalized operations
// ---------

// r = a*b
// #define __GIVARO_ZPZIntType_N_MUL(r,p,a,b) { r = a*b % p; }
#define __GIVARO_ZPZIntType_N_MUL(r,p,a,b) { r = a; r*=b; r %= p; }
// r *= a
//#define __GIVARO_ZPZIntType_N_MULIN(r,p,a) {  r = (r*a % p);  }
#define __GIVARO_ZPZIntType_N_MULIN(r,p,a) {  r *= a; r %= p;  }

// r = a - b
//#define __GIVARO_ZPZIntType_N_SUB(r,p,a,b) { r = (a-b); r= (r < 0 ? r+p : r); }
#define __GIVARO_ZPZIntType_N_SUB(r,p,a,b) { r = (a>=b) ? a-b: (p-b)+a ; }
// r -= a
// #define __GIVARO_ZPZIntType_N_SUBIN(r,p,a) { r -= a; r= (r < 0 ? r+p : r); }
#define __GIVARO_ZPZIntType_N_SUBIN(r,p,a) { if (r<a) r+=(p-a); else r-=a; }

// r = a+b
// #define __GIVARO_ZPZIntType_N_ADD(r,p,a,b) { r = (a+b); r= (r < p ? r : r-p); }
#define __GIVARO_ZPZIntType_N_ADD(r,p,a,b) { r = (a+b); if (r >= p) r-=p; }
// r += a
// #define __GIVARO_ZPZIntType_N_ADDIN(r,p,a) { r += a;  r= (r < p ? r : r-p); }
#define __GIVARO_ZPZIntType_N_ADDIN(r,p,a) { r += a;  if (r >= p) r-=p; }

// r <- a*b+c % p
// #define __GIVARO_ZPZIntType_N_MULADD(r,p,a,b,c) { r = (a*b+c) % p;  }
#define __GIVARO_ZPZIntType_N_MULADD(r,p,a,b,c) { r = a; r*=b; r+=c; r %= p;  }

// #define __GIVARO_ZPZIntType_N_MULADDIN(r,p,a,b) { r = (a*b+r) % p;  }
#define __GIVARO_ZPZIntType_N_MULADDIN(r,p,a,b) { r += (a*b); r %= p;  }

// a*b-c
//#define __GIVARO_ZPZIntType_N_MULSUB(r,p,a,b,c) { r = (a*b+p-c); r= (r<p ? r : r % p);  }
#define __GIVARO_ZPZIntType_N_MULSUB(r,p,a,b,c) { r = a*b; r+=p; r-=c; r= (r<p ? r : r % p);  }
// a*b-c
//#define __GIVARO_ZPZIntType_N_SUBMULIN(r,p,a,b) { r -= (a*b); if (r<0) { r+=p; r = (r<0 ? r % p : r); } }
#define __GIVARO_ZPZIntType_N_SUBMULIN(r,p,a,b) { r = p-r; r += a*b; r= (r<p ? r : r % p); __GIVARO_ZPZIntType_N_NEGIN(r,p); }

#define __GIVARO_ZPZIntType_N_NEG(r,p,a) { r = ( isZero(a) ? zero : p-a); }
#define __GIVARO_ZPZIntType_N_NEGIN(r,p) { r = ( isZero(r) ? zero : p-r); }

namespace Givaro {

template<typename IntType>
inline typename Modular<IntType>::Residu_t Modular<IntType>::residu( ) const
{ return _p; }



 // ------------------------- Miscellaneous functions

template<typename IntType>
inline int Modular<IntType>::isZero(const Rep& a) const
{ return a == zero; }

template<typename IntType>
inline int Modular<IntType>::isOne(const Rep& a) const
{ return a == one; }

template<typename IntType>
inline int Modular<IntType>::isMOne(const Rep& a) const
{ return a == mOne; }



template<typename IntType>
inline size_t Modular<IntType>::length(const Rep& ) const
{ return Modular<IntType>::size_rep;}



 // ------------------------- Arithmetic functions




template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::mul (Rep& r,
							    const Rep& a, const Rep& b) const
{
    __GIVARO_ZPZIntType_N_MUL(r,_p,a,b); return r;
}

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::sub (Rep& r,
							    const Rep& a, const Rep& b) const
{
  __GIVARO_ZPZIntType_N_SUB(r,_p,a,b); return r;
}

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::add (Rep& r,
							    const Rep& a, const Rep& b) const
{
    __GIVARO_ZPZIntType_N_ADD(r,_p,a,b); return r;
}

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::neg (Rep& r,
							    const Rep& a) const
{
    __GIVARO_ZPZIntType_N_NEG(r,_p,a); return r;

}

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::negin (Rep& r) const
{
  __GIVARO_ZPZIntType_N_NEGIN(r,_p);
  return r;
}

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::inv (Rep& u1,
							    const Rep& a) const
{
    u1=one;
    IntType r0(_p), r1(a);
    IntType q(r0/r1);

    r0 -= q * r1;
    if (r0 == zero) return u1;
    IntType u0 = q;

    q = r1/r0;
    r1 -= q * r0;

    while (r1 != zero) {
        u1 += q * u0;

        q = r0/r1;
        r0 -= q * r1;
        if (r0 == zero) return u1;
        u0 += q * u1;

        q = r1/r0;
        r1 -= q * r0;

    };

    return u1=_p-u0;
}

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::div (Rep& r,
							    const Rep& a, const Rep& b) const
{
  typename Modular<IntType>::Rep ib;
  inv(ib, b);
  __GIVARO_ZPZIntType_N_MUL(r,_p,a,ib);
  return r;
}

 // -- inline array operations between typename Modular<IntType>::Rep
template<typename IntType>
inline void Modular<IntType>::mul (const size_t sz, Array r, constArray a, constArray b) const
{
  for ( size_t i=sz ; --i ; ) {
    __GIVARO_ZPZIntType_N_MUL(r[i], _p,a[i], b[i]);
  }
}

template<typename IntType>
inline void Modular<IntType>::mul (const size_t sz,
				  Array r, constArray a, const Rep& b) const
{
  for ( size_t i=sz ; --i ; ) {
    __GIVARO_ZPZIntType_N_MUL(r[i], _p, a[i], b);
  }
}

template<typename IntType>
inline void Modular<IntType>::div (const size_t sz, Array r, constArray a, constArray b) const
{
  for ( size_t i=sz ; --i ; ) {
    div( r[i], a[i], b[i]);
  }
}

template<typename IntType>
inline void Modular<IntType>::div (const size_t sz, Array r, constArray a,
				  const Rep& b) const
{
  typename Modular<IntType>::Rep ib;
  inv(ib, b);
  mul(sz, r, a, ib);
}

template<typename IntType>
inline void Modular<IntType>::add (const size_t sz, Array r, constArray a, constArray b) const
{
  for ( size_t i=sz ; --i ; ) {
    __GIVARO_ZPZIntType_N_ADD(r[i], _p, a[i], b[i]);
  }
}

template<typename IntType>
inline void Modular<IntType>::add (const size_t sz, Array r, constArray a,
				  const Rep& b) const
{
  for ( size_t i=sz ; --i ; ) {
    __GIVARO_ZPZIntType_N_ADD(r[i], _p, a[i], b);
  }
}

template<typename IntType>
inline void Modular<IntType>::sub (const size_t sz, Array r, constArray a, constArray b) const
{
  for ( size_t i=sz ; --i ; ) {
    __GIVARO_ZPZIntType_N_SUB(r[i], _p, a[i], b[i]);
  }
}

template<typename IntType>
inline void Modular<IntType>::sub (const size_t sz, Array r, constArray a,
				  const Rep& b) const
{
  for ( size_t i=sz ; --i ; ) {
    __GIVARO_ZPZIntType_N_SUB(r[i], _p, a[i], b);
  }
}

template<typename IntType>
inline void Modular<IntType>::neg (const size_t sz, Array r, constArray a) const
{
  for ( size_t i=sz ; --i ; ) {
    __GIVARO_ZPZIntType_N_NEG(r[i], _p, a[i]);
  }
}


template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::mulin (Rep& r,
							      const Rep& a) const
{
  __GIVARO_ZPZIntType_N_MULIN(r,_p, a);
  return r;
}

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::divin (Rep& r,
							      const Rep& a) const
{
  typename Modular<IntType>::Rep ia;
  inv(ia, a);
  return mulin(r, ia);
}

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::addin (Rep& r,
							      const Rep& a) const
{
  __GIVARO_ZPZIntType_N_ADDIN(r,_p, a);
  return r;
}

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::subin (Rep& r,
							      const Rep& a) const
{
  __GIVARO_ZPZIntType_N_SUBIN(r,_p, a);
  return r;
}


template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::invin (Rep& r) const
{
   typename Modular<IntType>::Rep t = r;
   return Modular<IntType>::inv(r,t);
}

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::axpy (Rep& r,
						    const Rep& a, const Rep& b, const Rep& c) const
{
  __GIVARO_ZPZIntType_N_MULADD(r, _p, a, b, c);
  return r;
}

template<typename IntType>
inline typename Modular<IntType>::Rep&  Modular<IntType>::axpyin (Rep& r,
						       const Rep& a, const Rep& b) const
{
  typename Modular<IntType>::Rep tmp = r;
  __GIVARO_ZPZIntType_N_MULADDIN(tmp, _p, a, b);
  return r = (Rep)tmp;
}


template<typename IntType>
inline void Modular<IntType>::axpy (const size_t sz, Array r,
				   constArray a, constArray x, constArray y) const
{
  for ( size_t i=sz ; --i ; ) {
    __GIVARO_ZPZIntType_N_MULADD(r[i], _p, a[i], x[i], y[i]);
  }
}

template<typename IntType>
inline void Modular<IntType>::axpyin (const size_t sz, Array r,
				     constArray a, constArray x) const
{
  for ( size_t i=sz ; --i ; ) {
    typename Modular<IntType>::Rep tmp = r[i];
    __GIVARO_ZPZIntType_N_MULADDIN(tmp, _p, a[i], x[i]);
    r[i] = (Rep)tmp;
  }
}

template<typename IntType>
inline typename Modular<IntType>::Rep&  Modular<IntType>::axmy (Rep& r,
						     const Rep& a, const Rep& b, const Rep& c) const
{
  __GIVARO_ZPZIntType_N_MULSUB(r, _p, a, b, c);
  return r;
}

// r = c - a*b
template<typename IntType>
inline typename Modular<IntType>::Rep&  Modular<IntType>::maxpy (Rep& r,
						      const Rep& a, const Rep& b, const Rep& c) const
{
  typename Modular<IntType>::Rep tmp = c;
  __GIVARO_ZPZIntType_N_SUBMULIN(tmp, _p, a, b );
  return r = (Rep)tmp;
}
// r -= a*b
template<typename IntType>
inline typename Modular<IntType>::Rep&  Modular<IntType>::maxpyin (Rep& r,
						       	const Rep& a, const Rep& b) const
{
  __GIVARO_ZPZIntType_N_SUBMULIN(r, _p, a, b );
  return r;
//   typename Modular<IntType>::Rep tmp = r;
//   __GIVARO_ZPZIntType_N_SUBMULIN(tmp, _p, a, b );
//   return r = (Rep)tmp;
}
// r = a*b - r
template<typename IntType>
inline typename Modular<IntType>::Rep&  Modular<IntType>::axmyin (Rep& r,
						       const Rep& a, const Rep& b) const
{
    maxpyin(r,a,b);
    return negin(r);
}


template<typename IntType>
inline void Modular<IntType>::axmy (const size_t sz, Array r,
				   constArray a, constArray x, constArray y) const
{
  for ( size_t i=sz ; --i ; ) {
    __GIVARO_ZPZIntType_N_MULSUB(r[i], _p, a[i], x[i], y[i]);
  }
}

// r -= a*b
template<typename IntType>
inline void Modular<IntType>::maxpyin (const size_t sz, Array r,
				     constArray a, constArray x) const
{
  for ( size_t i=sz ; --i ; ) {
    typename Modular<IntType>::Rep tmp = r[i];
    __GIVARO_ZPZIntType_N_SUBMULIN(tmp, _p, a[i], x[i]);
    r[i] = (Rep)tmp;
  }
}


// ---------
// -- misc operations
// ---------


template<typename IntType>
inline  typename Modular<IntType>::Rep&  Modular<IntType>::init ( Rep& r, const double a ) const
{
  int sign; double ua;
  if (a < 0.0) { sign =-1; ua = -a;}
  else { ua = a; sign =1; }
  r = IntType(ua);
  if (r >=_p) r %= _p;
  if (!isZero(r) && (sign == -1)) r = _p - r;
  return r;
}

template<typename IntType>
inline  typename Modular<IntType>::Rep&  Modular<IntType>::init ( Rep& r, const float a ) const
{
    return init(r, (double)a);
}



template<typename IntType>
inline  typename Modular<IntType>::Rep&  Modular<IntType>::init ( Rep& r, const unsigned long int a ) const
{
    r = IntType(a);
    if ( r >= _p ) r %= _p;
    return r ;
}

template<typename IntType>
inline  typename Modular<IntType>::Rep&  Modular<IntType>::init ( Rep& r, const long int a ) const
{
  int sign;
  if (a <0) { sign =-1; r = IntType(-a);}
  else { r = IntType(a); sign =1; }
  if (r >=_p) r %= _p;
  if (!isZero(r) && (sign ==-1)) r = _p - r;
  return r;
}

template<typename IntType>
inline  typename Modular<IntType>::Rep&  Modular<IntType>::init ( Rep& r, const IntType& a ) const
{
  int sign;
  if (a < zero) { sign =-1; r = IntType(-a);}
  else { r = IntType(a); sign =1; }
  if (r >=_p) r %= _p;
  if (!isZero(r) && (sign ==-1)) r = _p - r;
  return r;
}

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::init( Rep& a, const int i) const { return init(a,(long)i); }

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::init( Rep& a, const unsigned int i) const { return init(a,(unsigned long)i); }


template<typename IntType>
inline void Modular<IntType>::assign
  ( const size_t sz, Array r, constArray a ) const
{
  for ( size_t i=sz ; --i ; ) {
    if (a[i] <Modular<IntType>::zero) {
       r[i] = a[i] + _p;
       if (r[i] <Modular<IntType>::zero) r[i] %= _p;
    }
    else if (a[i] >_p) {
       r[i] = a[i] - _p;
       if (r[i] >=_p) r[i] %= _p;
    }
    else r[i] = a[i];
  }
}

template<typename IntType>
inline  typename Modular<IntType>::Rep&  Modular<IntType>::assign ( Rep& r, const long a ) const
{
  return r = typename Modular<IntType>::Rep(a);
}

template<typename IntType>
inline  typename Modular<IntType>::Rep&  Modular<IntType>::assign ( Rep& r, const short a ) const
{ return Modular<IntType>::assign( r, (long)a); }

template<typename IntType>
inline  typename Modular<IntType>::Rep&  Modular<IntType>::assign ( Rep& r, const unsigned long a ) const
{ return r = typename Modular<IntType>::Rep(a); }

template<typename IntType>
inline  typename Modular<IntType>::Rep&  Modular<IntType>::assign
  ( Rep& r, const unsigned short a ) const
{ return r = typename Modular<IntType>::Rep(a); }

template<typename IntType>
inline  typename Modular<IntType>::Rep&  Modular<IntType>::assign
  ( Rep& r, const Rep& a ) const
{ return r=a; }


template<typename IntType>
inline void Modular<IntType>::init
  ( const size_t sz, Array r, constArray a ) const
{
  for ( size_t i=sz ; --i ; )
       r[i] = a[i];
}

template<typename IntType>
inline typename Modular<IntType>::Rep& Modular<IntType>::init ( Rep& r ) const
{ return r = zero; }


  //  a -> r: int32_t to double
template<typename IntType>
inline void
  Modular<IntType>::i2d ( const size_t sz, double* r, constArray a ) const
{
  for (size_t i=0; i<sz; ++i) r[i] = a[i];
}

  //  a -> r: double to int32_t
template<typename IntType>
inline void
  Modular<IntType>::d2i ( const size_t sz, Array r, const double* a ) const
{
  union d_2_l {
    double d;
    int32_t r[2];
  };
//  static const double offset = 4503599627370496.0; // 2^52
  double offset = 4503599627370496.0; // 2^52
  for (size_t i=0; i<sz; ++i)
  {
       d_2_l tmp;
      // - normalization: put fractional part at the end of the representation
      tmp.d = a[i] + offset;
      r[i] = tmp.r[1];
      if (r[i] <_p) r[i] %= _p;
  }
  //    r[i] = (tmp.r[1] <_p ? tmp.r[1] : tmp.r[1]-_p);
  //    r[i] = (r[i] <_p ? r[i] : r[i]%_p);
  //    r[i] = (tmp.r[1] <_p ? tmp.r[1] : tmp.r[1]%_p);
}



 // -- Input: (z, <_p>)
template<typename IntType>
inline std::istream& Modular<IntType>::read (std::istream& s)
{
  char ch;
  s >> std::ws >> ch;
  if (ch != '(')
//    GivError::throw_error( GivBadFormat("Modular<IntType>::read: syntax error: no '('"));
    std::cerr << "GivBadFormat(Modular<IntType>::read: syntax error: no '('))" << std::endl;

  s >> std::ws >> ch;
  if (ch != 'z')
//    GivError::throw_error( GivBadFormat("Modular<IntType>::read: bad domain object"));
    std::cerr << "GivBadFormat(Modular<IntType>::read: bad domain object))" << std::endl;

  s >> std::ws >> ch;
  if (ch != ',')
//    GivError::throw_error( GivBadFormat("Modular<IntType>::read: syntax error: no ','"));
    std::cerr << "GivBadFormat(Modular<IntType>::read: syntax error: no ',')) " << std::endl;

  s >> std::ws >> _p;

  s >> std::ws >> ch;
  if (ch != ')')
//    GivError::throw_error( GivBadFormat("Modular<IntType>::read: syntax error: no ')'"));
    std::cerr << "GivBadFormat(Modular<IntType>::read: syntax error: no ')')) " << std::endl;

  return s;
}

template<typename IntType>
inline std::ostream& Modular<IntType>::write (std::ostream& s ) const
{
	return s << "Modular<[IntType]> modulo " << residu();
}

template<typename IntType>
inline std::istream& Modular<IntType>::read (std::istream& s,
					    Rep& a) const
{
  s >> a;
  init(a, a);
  return s;
}

template<typename IntType>
inline std::ostream& Modular<IntType>::write (std::ostream& s,
					     const Rep& a) const
{
  return s << a;
}

} // namespace Givaro

#endif // __GIVARO_zpz_gen_INL
// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
