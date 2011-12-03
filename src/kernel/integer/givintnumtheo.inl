// =================================================================== //
// Copyright(c)'1994-2009 by The Givaro group
// This file is part of Givaro.
// Givaro is governed by the CeCILL-B license under French law
// and abiding by the rules of distribution of free software.
// see the COPYRIGHT file for more details.
// Givaro : Euler's phi function
//          Primitive roots.
// Needs list structures : stl ones for instance
// Time-stamp: <23 Jun 09 19:26:23 Jean-Guillaume.Dumas@imag.fr>
// =================================================================== //

#ifndef __GIVARO_numtheo_INL
#define __GIVARO_numtheo_INL

#include "givaro/givintnumtheo.h"
#include <list>
#include <vector>

#include "givaro/givintrns.h"
#include "givaro/givpower.h"

namespace Givaro {

	// =================================================================== //
	// Euler's phi function
	// =================================================================== //
	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::phi(Rep& res, const Rep& n) const
	{
		if (isleq(n,1)) return res=n;
		if (isleq(n,3)) return Rep::sub(res,n,this->one);
		std::list<Rep> Lf;
		Father_t::set(Lf,n);
		//return phi (res,Lf,n);
		return phi (res,Lf,n);
	}


	template<class RandIter>
	template< template<class, class> class Container, template<class> class Alloc>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::phi(Rep& res, const Container<Rep, Alloc<Rep> >& Lf, const Rep& n) const
	{
		if (isleq(n,1)) return res=n;
		if (isleq(n,3)) return Rep::sub(res,n,this->one);
		res = n; Rep t,m;
		for(typename Container<Rep, Alloc<Rep> >::const_iterator f=Lf.begin(); f!=Lf.end(); ++f)
			Rep::mul(res, Rep::divexact(t,res,*f), Rep::sub(m, *f, this->one));
		return res;
	}

	// =================================================================== //
	// M�bius function
	// =================================================================== //
	//#ifndef __ECC
	template<class RandIter>
	template< template<class, class> class Container, template <class> class Alloc>
	//short IntNumTheoDom<RandIter>::mobius(const Container<unsigned long, Alloc<unsigned long> >& lpow) const
	//#else
	//template<class RandIter>
	//template<template <class, class> class Container, template <class> class Alloc>
	short IntNumTheoDom<RandIter>::mobius(const Container<Rep, Alloc<Rep> >& lpow) const
	{
		//#endif
		if (lpow.size()) {
			short mob = 1;
			for(typename Container<Rep, Alloc<Rep> >::const_iterator i=lpow.begin();i != lpow.end(); ++i) {
				if (*i > 1) {
					return 0;
				} else
					mob = -mob;
			}
			return mob;
		} else
			return 1;
	}

	template<class RandIter>
	short IntNumTheoDom<RandIter>::mobius(const Rep& a) const
	{
		std::list< Rep> lr;
		std::list<unsigned long> lp;
		Father_t::set(lr, lp, a);
		return mobius(lp);
	}


	// =================================================================== //
	// Primitive Root
	// =================================================================== //

	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::prim_root(Rep& A, unsigned long& runs, const Rep& n) const
	{
		// n must be in {2,4,p^m,2p^m} where p is an odd prime
		// else infinite loop

		if (isleq(n,4)) return sub(A,n,this->one);
		if (isZero(mod(A,n,4))) return A=this->zero;
		Rep p,ismod2, q, no2, root;
		if (isZero(mod(ismod2,n,2))) divexact(no2,n,2); else no2=n;
		p=no2;
		int k = 1;
		while (! isprime(p) ) {
			sqrt(root, p);
			while (mul(q,root,root) == p) {
				p = root;
				sqrt(root,p);
			}
			if (! isprime(p) ) {
				q=p;
				while( p == q ) factor(p, q);
				divin(q,p);
				if (q < p) p = q;
			}
		}
		if (isZero(ismod2)) mul(q,p,2); else q=p;
		for(;q != n;++k,q*=p) ;
		Rep phin, tmp;
		phi(phin,p);
		std::list<Rep> Lf;
		Father_t::set(Lf,phin);
		typename std::list<Rep>::iterator f;
		for(f=Lf.begin();f!=Lf.end();++f)
			this->div(*f,phin,*f);
		int found; runs = 0;
		A=2;
		found = (int) ++runs;
		for(f=Lf.begin();(f!=Lf.end() && found);f++)
			found = (! isOne( this->powmod(tmp,A,*f,p)) );
		if (! found) {
			A=3;
			found = (int) ++runs;
			for(f=Lf.begin();(f!=Lf.end() && found);f++)
				found = (! isOne( this->powmod(tmp,A,*f,p)) );
		}
		if (! found) {
			A=5;
			found = (int) ++runs;
			for(f=Lf.begin();(f!=Lf.end() && found);f++)
				found = (! isOne( this->powmod(tmp,A,*f,p)) );
		}
		if (! found) {
			A=6;
			found = (int) ++runs;
			for(f=Lf.begin();(f!=Lf.end() && found);f++)
				found = (! isOne( this->powmod(tmp,A,*f,p)) );
		}
		while (! found) {
			do {
				this->random(this->_g, A, p);
				addin( modin(A,sub(tmp,p,7)) , 7);
			} while ( ! isOne(gcd(tmp,A,p)) );
			found = (int) ++runs;
			for(f=Lf.begin();(f!=Lf.end() && found);f++)
				found = (! isOne( this->powmod(tmp,A,*f,p)) );
		}
		if (k == 1) {
			if (isZero(ismod2) && isZero(mod(ismod2, A, 2)))
				return A+=p;
			else
				return A;
		} else {
			if (! is_prim_root(A,no2))
				A+=p;
			if (isZero(ismod2) && isZero(mod(ismod2, A, 2)))
				return A+=no2;
			else
				return A;
		}
	}

	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::prim_root(Rep& A, const Rep& n) const
	{ unsigned long runs; return prim_root(A, runs, n); }

	/*
	   template<class T, template <class, class> class Container, template<class> class Alloc>
	   std::ostream& operator<< (std::ostream& o, const Container<T, Alloc<T> >& C) {
	   for(typename Container<T, Alloc<T> >::const_iterator refs =  C.begin();
	   refs != C.end() ;
	   ++refs )
	   o << (*refs) << " " ;
	   return o << std::endl;
	   }
	   */


	// =================================
	// Probable primitive roots
	//
	//  Polynomial-time generation of primitive roots
	//  L is number of loops of Pollard partial factorization of n-1
	//  10,000,000 gives at least 1-2^{-40} probability of success
	//  [Dubrois & Dumas, Industrial-strength primitive roots]
	//  Returns the probable primitive root and the probability of error.
	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::probable_prim_root(Rep& primroot, double& error, const Rep& p, const unsigned long L) const
	{
		// std::cerr << "L: " << L << std::endl;

		// partial factorisation
		std::vector<Rep> Lq;
		std::vector<unsigned long> e;
		Rep Q, pmun(p); --pmun;
		primroot = 1;

		bool complet = Father_t::set(Lq, e, pmun, L);
		// partial factorisation done

		//std::cerr << "Lq: " << Lq << std::endl;
		//std::cerr << "e: " << e << std::endl;


		Rep Temp;
		Rep essai, alea;


		if (!complet) {
			Q=Lq.back();
			Lq.pop_back();
			this->div(Temp, pmun, Q);
			do {
				nonzerorandom(this->_g, alea, p);
				modin(alea, p);
				this->powmod(essai, alea, Temp, p);
				//std::cerr << alea << " should be of order " << Q << " mod " << p << std::endl;
			} while (essai == 1);
			// looking for alea, of order Q with high probability

			mulin(primroot, essai);

			//  1-(1+2/(p-1))*(1-1/L^2)^log_B(Q)  < 1-(1+2^(-log_2(p)))*(1-1/L^2)^log_B(Q);
			essai = L;
			mul(Temp, essai, L);
			error = 1-1.0/(double)Temp;
			error = power(error, logp(Q,Temp) );
			error *= (1.0+1.0/((double)Q-1.0));
			error = 1-error;
		} else
			error = 0.0;

		typename std::vector<Rep>::const_iterator Lqi = Lq.begin();
		typename std::vector<unsigned long>::const_iterator ei = e.begin();
		for ( ; Lqi != Lq.end(); ++Lqi, ++ei) {
			this->div(Temp, pmun, *Lqi);
			do {
				nonzerorandom(this->_g, alea, p);
				modin(alea, p);
				this->powmod(essai, alea, Temp, p);
				//std::cerr << alea << " should be of order at least " << *Lqi << "^" << *ei << "==" << power(*Lqi,*ei) << " mod " << p << std::endl;
			} while( essai == 1 ) ;

			// looking for alea with order Lq[i]^e[i]

			//std::cerr << alea << " is of order at least " << (*Lqi) << "^" << (*ei) << "==" << power(*Lqi,*ei) << " mod " << p << std::endl;

			this->divin(Temp, power(*Lqi,*ei-1));
			mulin(primroot, this->powmod(essai, alea, Temp, p));
		}

		modin(primroot, p);

		return primroot;
		// return primroot with high probability
	}

#ifndef GIVABSDIFF
#define GIVABSDIFF(a,b) ((a)<(b)?((b)-(a)):((a)-(b)))
#endif
#include <math.h>

	//  Here L is computed so that the error is close to epsilon
	// Newton-Raphson iteration is used for
	// 1-epsilon = (1+2/(p-1))*(1-1/B)^(ln( (p-1)/2 )/ln(B))
	// see [Dubrois & Dumas]
	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::probable_prim_root(Rep& primroot, double& error, const Rep& p, const double epsilon) const
	{
		unsigned long L;
		double t1, t4, t5, t6, t7, t8, t10, t11, t17, t20, t23, t32(1.0/epsilon), B(1.0);

		t1 = (double)p-1.0;          // p-1
		t4 = 1.0+2.0/t1;             // 1+2/(p-1)
		t7 = log(t1/2.0);            // log( (p-1)/2 )
		do {
			B = t32;
			t5 = 1/B;                // 1/B
			t6 = 1.0-t5;             // 1-1/B
			t8 = log(B);             // log(B)
			t10 = t7/t8;             // log_B( (p-1)/2 )
			t11 = ::pow(t6,t10);     // (1-1/B)^log_B( (p-1)/2 )
			t17 = t8*t8;             // log^2(B)
			t20 = log(t6);           // log(1-1/B)
			t23 = B*B;               // B^2
			//  B-F(B)/diff(F(B),B)
			t32 = B-(t4*t11-1.0+epsilon)/t4/t11/(-t7/t17*t5*t20+t10/t23/t6);
		} while( (GIVABSDIFF(t32,B) > 0.5) && (B<1.8e+19) && ((1.0-t4*t11) > epsilon ) );
		//         std::cerr << "t32: " << t32 << std::endl;
		if (B<1.8e+19)
			L = (unsigned long)::sqrt(t32);
		else
			L = 0; // TOO small a precision, turning to deterministic process


		return probable_prim_root(primroot, error, p, L);
	}



	// =================================
	// Specializations for prime numbers
	// =================================

	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::prim_root_of_prime(Rep& A, const Rep& n) const
	{

		std::vector<Rep> Lf;
		Rep phin; sub(phin,n,this->one);
		Father_t::set(Lf,phin);
		return prim_root_of_prime(A, Lf, phin, n);
	}


	inline Integer& ppin(Integer& res, const Integer& prime) {
		IntegerDom I;
		Integer tmp;
		while( I.isZero(I.mod(tmp, res, prime)) ) {
			I.divexact(res, tmp = res, prime);
		}
		return res;
	}


	/// Add Jacobi for quadratic nonresidue

	template<class RandIter>
	template<class Array>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::prim_root_of_prime(Rep& A, const Array& aLf, const Rep& phin, const Rep& n) const
	{

		Rep tmp, expo, temp;
		A = this->one;

		Rep prime(2), Aorder=this->one;
		std::vector<Rep> Lf = aLf, newLf, oldLf;
		newLf.reserve(Lf.size());
		oldLf.reserve(Lf.size());

		Rep primeorder;

		for(bool exemp = true; exemp; nextprimein(prime) ) {
			A = prime;
			primeorder = phin;
			for(typename Array::const_iterator f = Lf.begin(); f != Lf.end(); ++f) {
				this->powmod(tmp, prime, this->div(expo, primeorder, *f), n);
				if (isOne(tmp)) {
					newLf.push_back(*f);
					while (isZero(mod(tmp,expo,*f)) && isOne( this->powmod(tmp, prime, this->div(temp, expo, *f), n) ) ) { expo = temp; }
					primeorder = expo;
					//                 std::cerr << "2 Order (Div): " << primeorder << std::endl;
				} else {
					oldLf.push_back(*f);
					exemp = false;
					//                 std::cerr << "2 Order : " << primeorder << std::endl;
				}
			}
		}

		Aorder = primeorder;
		Lf = newLf;

		// Now we have A with order > 1, we need to add other primes

		//     std::cerr << "Prime : 2" << std::endl;
		//     std::cerr << "Root : " << A << std::endl;
		//     std::cerr << "Order : " << Aorder << std::endl;

		for ( ; islt(Aorder,phin); nextprimein(prime) ) {
			newLf.resize(0); oldLf.resize(0);

			for(typename Array::const_iterator f = Lf.begin(); f != Lf.end(); ++f) {
				this->powmod(tmp, prime, this->div(expo, phin, *f), n);
				if (isOne(tmp)) {
					newLf.push_back(*f);
				} else {
					oldLf.push_back(*f);
				}
			}

			if (oldLf.size() > 0) {
				Rep g = phin;

				for(typename Array::const_iterator f = oldLf.begin(); f != oldLf.end(); ++f) {
					ppin(g, *f);
					ppin(Aorder, *f);
				}

				this->powmod(tmp, prime, g, n);

				modin( mulin(A, tmp), n );

				mulin(Aorder, this->div(tmp, phin, g));

				Lf = newLf;
			}

			//         std::cerr << "Prime : " << prime << std::endl;
			//         std::cerr << "Root : " << A << std::endl;
			//         std::cerr << "Order : " << Aorder << std::endl;

		}


		return A;
	}



	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::lowest_prim_root(Rep& A, const Rep& n) const
	{
		// n must be in {2,4,p^m,2p^m} where p is an odd prime
		// else returns zero
		if (isleq(n,4)) return Rep::sub(A,n,this->one);
		if (isZero(Rep::mod(A,n,4UL))) return A=this->zero;
		Rep phin, tmp;
		phi(phin,n);
		std::list<Rep> Lf;
		Father_t::set(Lf,phin);
		typename std::list<Rep>::iterator f;
		for(f=Lf.begin();f!=Lf.end();++f)
			this->div(*f,phin,*f);
		int found=0;
		for(A = 2;(isleq(A,n) && (! found)); Rep::addin(A,1UL)) {
			if (isOne(gcd(tmp,A,n))) {
				found = 1;
				for(f=Lf.begin();(f!=Lf.end() && found);f++)
					found = (! isOne( this->powmod(tmp,A,*f,n)) );
			}
		}
		if (isleq(A,n))
			return Rep::subin(A,1UL);
		else
			return A=this->zero;
	}

	template<class RandIter>
	bool IntNumTheoDom<RandIter>::is_prim_root(const Rep& p, const Rep& n) const
	{
		// returns 0 if failed
		bool found=false;
		Rep phin, tmp;
		phi(phin,n);
		std::list<Rep> Lf;
		Father_t::set(Lf,phin);
		typename std::list<Rep>::iterator f=Lf.begin();
		Rep A; mod(A,p,n);
		if (isOne(gcd(tmp,A,n))) {
			found = true;
			for(;(f!=Lf.end() && found);f++) {
				//             found = ( this->powmod(A,phin / (*f),n) != 1);
				found = (! isOne( this->powmod(tmp,A, this->div(tmp,phin,*f),n)) );
			}
		}
		return found;
	}

	template<class RandIter>
	bool IntNumTheoDom<RandIter>::isorder(const Rep& g, const Rep& p, const Rep& n) const
	{
		// returns 1 if p is of order g in Z/nZ
		Rep tmp;
		return (isOne( this->powmod(tmp, p, g, n) ) && areEqual( g, order(tmp,p,n) ) );
	}

	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::order(Rep& g, const Rep& p, const Rep& n) const
	{
		// returns 0 if failed
		Rep A; mod(A,p,n);
		if (isZero(A))
			return g = this->zero;
		if (isOne(A))
			return g = this->one;
		bool noprimroot=false;
		Rep phin,gg,tmp;
		phi(phin,n);
		std::list<Rep> Lf;
		Father_t::set(Lf,phin);
		Lf.sort();
		typename std::list<Rep>::iterator f=Lf.begin();
		if (isOne(gcd(tmp,A,n))) {
			noprimroot = false;
			for(;f!=Lf.end();++f)
				if ( (noprimroot = isOne( this->powmod(tmp,A, this->div(g,phin,*f),n)) ) )
					break;
			if (noprimroot) {
				for(;f!=Lf.end();++f)
					while (isZero(mod(tmp,g,*f)) && isOne(  this->powmod(tmp,A,  this->div(gg,g,*f),n) ) )
						g = gg;
				return g;
			} else
				return g=phin;
		}
		return g=this->zero;
	}

	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::prim_inv(Rep& A, const Rep& n) const
	{
		if (isleq(n,4)) return sub(A,n,this->one);
		if (areEqual(n,8)) return init(A,3);
		return prim_base(A, n);
	}

	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::prim_elem(Rep& A, const Rep& n) const
	{
		if (isleq(n,4)) {
			Rep tmp;
			return this->sub(A,n,this->one);
		}

		if (areEqual(n,8)) return init(A,2);
		return prim_base(A, n);
	}

	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::prim_base(Rep& A, const Rep& m) const
	{
		// Prerequisite : m > 4, and m != 8.
		std::vector<Rep> Lp; std::vector<unsigned long> Le;
		Father_t::set(Lp, Le, m);
		unsigned long nbf = Lp.size();
		std::vector<Rep> Pe(nbf);
		std::vector<Rep> Ra(nbf);

		typename std::vector<Rep>::const_iterator p=Lp.begin();
		typename std::vector<unsigned long>::const_iterator e=Le.begin() ;
		typename std::vector<Rep>::iterator pe = Pe.begin();
		typename std::vector<Rep>::iterator a = Ra.begin() ;
		for( ;p!=Lp.end();++p, ++e, ++pe, ++a) {
			dom_power( *pe, *p, *e, *this);
			if (areEqual(*p,2))
				init(*a, 3);
			else
				prim_root(*a, *pe);
		}

		IntRNSsystem<std::vector, std::allocator > RNs( Pe );
		//    IntRNSsystem<typename std::vector >, typename std::allocator > RNs( Pe );
		RNs.RnsToRing( A, Ra );
		return A;
	}


	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::lambda_primpow(Rep & z, const Rep& p, const unsigned long e) const
	{
		// Prerequisite : p prime.
		if (areEqual(p, 2)) {
			if (e<=3) return init(z,e);
			return dom_power(z, p, e-2, *this);
		} else {
			Rep tmp;
			return mulin( dom_power(z, p, e-1, *this), sub(tmp, p, this->one) );
		}
	}

	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::lambda_inv_primpow(Rep & z, const Rep& p, const unsigned long e) const
	{
		// Prerequisite : p prime.
		if (areEqual(p, 2)) {
			if (e<=2) return init(z,e);
			if (e==3) return init(z,2);
			return dom_power(z, p, e-2, *this);
		} else {
			Rep tmp;
			return mulin( dom_power(z, p, e-1, *this), sub(tmp, p, this->one) );
		}
	}




	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::lambda_inv(Rep & z, const Rep& m) const
	{
		if (areEqual(m,2)) return init(z,1);
		if (areEqual(m,3) || areEqual(m,4) || areEqual(m,8) ) return init(z,2);
		return lambda_base(z, m);
	}

	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::lambda(Rep & z, const Rep& m) const
	{
		if (areEqual(m,2)) return init(z,1);
		if (areEqual(m,3) || areEqual(m,4)) return init(z,2);
		if (areEqual(m,8) ) return init(z,3);
		return lambda_base(z, m);
	}


	template<class RandIter>
	typename IntNumTheoDom<RandIter>::Rep& IntNumTheoDom<RandIter>::lambda_base(Rep & z, const Rep& m) const
	{
		// Prerequisite: m > 4, and m != 8.
		std::vector<Rep> Lp; std::vector<unsigned long> Le;
		Father_t::set(Lp, Le, m);
		unsigned long nbf = Lp.size();

		lambda_inv_primpow(z, Lp.front(), Le.front() );

		if (nbf == 1) return z;

		typename std::vector<Rep>::const_iterator p=Lp.begin();
		typename std::vector<unsigned long>::const_iterator e=Le.begin() ;
		for( ++p, ++e; p != Lp.end(); ++p, ++e) {
			Rep tmp;
			lambda_inv_primpow(tmp, *p, *e);
			//            Rep g;
			//            gcd(g, z, tmp);
			//            mulin(z, this->divin(tmp, g));
			lcmin(z,tmp);
		}

		return z;
	}

} // namespace Givaro {
#endif // __GIVARO_numtheo_INL

// vim:sts=8:sw=8:ts=8:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
