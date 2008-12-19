// Copyright (C) 2008  Davis E. King (davisking@users.sourceforge.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_MATRIx_ASSIGn_
#define DLIB_MATRIx_ASSIGn_

#include "../geometry.h"
#include "matrix.h"
#include "matrix_utilities.h"
#include "../enable_if.h"
#include "matrix_assign_fwd.h"

namespace dlib
{
    /*

        This file is where all the implementations of matrix_assign() live.  The point of the
        matrix_assign() functions is to contain all the various optimizations that help the 
        matrix assign a matrix_exp to an actual matrix object quickly.

    */

    namespace ma
    {
        // This namespace defines whatever helpers we need in the rest of this file.

    // ------------------------------------------------------------------------------------

        struct op_null
        {
            template <typename EXP>
            struct op : has_nondestructive_aliasing, preserves_dimensions<EXP>
            {
                const static long cost = EXP::cost;
                typedef typename EXP::type type;
                template <typename M>
                static type apply ( const M& m, long r, long c)
                { 
                    return m(r,c);
                }
            };
        };

        template <
            typename EXP
            >
        const matrix_unary_exp<EXP,op_null> null_exp (
            const matrix_exp<EXP>& m
        )
        /*!
            The only point of this function is to make it easy to cause the overloads
            of matrix_assign to not trigger for a matrix expression.
        !*/
        {
            return matrix_unary_exp<EXP,op_null>(m.ref());
        }

    // ------------------------------------------------------------------------------------

        template < typename EXP, typename enable = void >
        struct matrix_is_vector { static const bool value = false; };
        template < typename EXP >
        struct matrix_is_vector<EXP, typename enable_if_c<EXP::NR==1 || EXP::NC==1>::type > { static const bool value = true; };

    }

// ----------------------------------------------------------------------------------------

    template <
        typename matrix_dest_type,
        typename EXP1,
        typename EXP2
        >
    typename disable_if_c<ma::matrix_is_vector<EXP1>::value || ma::matrix_is_vector<EXP2>::value>::type 
    matrix_assign_big (
        matrix_dest_type& dest,
        const matrix_multiply_exp<EXP1,EXP2>& src
    )
    /*!
        This overload catches assignments like:
            dest = lhs*rhs
            where lhs and rhs are both not vectors
    !*/
    {
        using namespace ma;
        const EXP1& lhs = src.lhs;
        const EXP2& rhs = src.rhs;
        const long bs = 90;
        set_all_elements(dest,0);

        // if the matrices are small enough then just use the simple multiply algorithm
        if (lhs.nc() <= 2 || rhs.nc() <= 2 || lhs.nr() <= 2 || rhs.nr() <= 2 || (lhs.size() <= bs*10 && rhs.size() <= bs*10) )
        {
            // This loop is optimized assuming that the data is laid out in 
            // row major order in memory.
            for (long r = 0; r< lhs.nr(); ++r)
            {
                for (long c = 0; c< lhs.nc(); ++c)
                {
                    const typename EXP2::type temp = lhs(r,c);
                    for (long i = 0; i < rhs.nc(); ++i)
                    {
                        dest(r,i) += rhs(c,i)*temp;
                    }
                }
            }
        }
        else
        {
            // if the lhs and rhs matrices are big enough we should use a cache friendly
            // algorithm that computes the matrix multiply in blocks.  


            // Loop over all the blocks in the lhs matrix
            for (long r = 0; r < lhs.nr(); r+=bs)
            {
                for (long c = 0; c < lhs.nc(); c+=bs)
                {
                    // make a rect for the block from lhs 
                    rectangle lhs_block(c, r, std::min(c+bs-1,lhs.nc()-1), std::min(r+bs-1,lhs.nr()-1));

                    // now loop over all the rhs blocks we have to multiply with the current lhs block
                    for (long i = 0; i < rhs.nc(); i += bs)
                    {
                        // make a rect for the block from rhs 
                        rectangle rhs_block(i, c, std::min(i+bs-1,rhs.nc()-1), std::min(c+bs-1,rhs.nr()-1));

                        // make a target rect in res
                        rectangle res_block(rhs_block.left(),lhs_block.top(), rhs_block.right(), lhs_block.bottom());

                        // This loop is optimized assuming that the data is laid out in 
                        // row major order in memory.
                        for (long r = lhs_block.top(); r <= lhs_block.bottom(); ++r)
                        {
                            for (long c = lhs_block.left(); c<= lhs_block.right(); ++c)
                            {
                                const typename EXP2::type temp = lhs(r,c);
                                for (long i = rhs_block.left(); i <= rhs_block.right(); ++i)
                                {
                                    dest(r,i) += rhs(c,i)*temp;
                                }
                            }
                        }
                    }
                }
            }
        }


    }

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_MATRIx_ASSIGn_

