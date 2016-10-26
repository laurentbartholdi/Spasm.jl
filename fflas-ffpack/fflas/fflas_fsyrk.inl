/* -*- mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
// vim:sts=4:sw=4:ts=4:noet:sr:cino=>s,f0,{0,g0,(0,\:0,t0,+0,=s
/*
 * Copyright (C) 2016 the FFLAS-FFPACK group
 *
 * Written by Clement Pernet <Clement.Pernet@imag.fr>
 *
 *
 * ========LICENCE========
 * This file is part of the library FFLAS-FFPACK.
 *
 * FFLAS-FFPACK is free software: you can redistribute it and/or modify
 * it under the terms of the  GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * ========LICENCE========
 *.
 */

namespace FFLAS {
    template<class Field>
    inline typename Field::Element_ptr
    fsyrk (const Field& F,
           const FFLAS_UPLO UpLo,
           const FFLAS_TRANSPOSE trans,
           const size_t N,
           const size_t K,
           const typename Field::Element alpha,
           typename Field::ConstElement_ptr A, const size_t lda,
           const typename Field::Element beta,
           typename Field::Element_ptr C, const size_t ldc){
        
            //@TODO: write an optimized iterative basecase
        if (N==1){ // Base case
            F.mulin (*C, beta);
            F.axpyin (*C, alpha, fdot (F, K, A, 1, A, 1));
            return C;
        } else if (K==1){
            if (!F.isOne(beta))
                fscalin (F, N, N, beta, C, ldc);
            fger (F, N, N, alpha, A, lda, A, lda, C, ldc);
            return C;
        } else {
            size_t N1 = N>>1;
            size_t N2 = N - N1;
            size_t K1 = K>>1;
            size_t K2 = K - K1;
                // Comments written for the case UpLo==FflasUpper, trans==FflasNoTrans
            typename Field::ConstElement_ptr A21 = A + N1*lda;
            typename Field::ConstElement_ptr A22 = A21 + K1;
            typename Field::ConstElement_ptr A12 = A + K1;
            typename Field::Element_ptr C12 = C + N1;
            typename Field::Element_ptr C22 = C12 + N1*ldc;
                // C11 <- alpha A11 x A11^T + beta C11
            fsyrk (F, UpLo, trans, N1, K1, alpha, A, lda, beta, C, ldc);
                // C11 <- alpha A12 x A12^T + C11
            fsyrk (F, UpLo, trans, N1, K2, alpha, A12, lda, F.one, C, ldc);
                // C22 <- alpha A21 x A21^T + beta C22
            fsyrk (F, UpLo, trans, N2, K1, alpha, A21, lda, beta, C22, ldc);
                // C22 <- alpha A22 x A22^T + C22
            fsyrk (F, UpLo, trans, N2, K2, alpha, A22, lda, F.one, C22, ldc);
                // C12 <- alpha A11 * A21^T + beta C12
            fgemm (F, FflasNoTrans, FflasTrans, N1, N2, K1, alpha, A, lda, A21, lda, beta, C12, ldc);
                // C12 <- alpha A12 * A22^T + C12
            fgemm (F, FflasNoTrans, FflasTrans, N1, N2, K2, alpha, A12, lda, A22, lda, F.one, C12, ldc);
            return C;
        }
    }
}
