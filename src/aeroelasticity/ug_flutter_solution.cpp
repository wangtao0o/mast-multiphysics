/*
 * MAST: Multidisciplinary-design Adaptation and Sensitivity Toolkit
 * Copyright (C) 2013-2020  Manav Bhatia and MAST authors
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
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
 */


// C++ includes
#include <iomanip>



// MAST includes
#include "aeroelasticity/ug_flutter_solution.h"
#include "aeroelasticity/ug_flutter_root.h"
#include "numerics/lapack_zggev_base.h"


MAST::UGFlutterSolution::UGFlutterSolution():
MAST::FlutterSolutionBase()
{ }



MAST::UGFlutterSolution::~UGFlutterSolution()
{ }




void
MAST::UGFlutterSolution::init (const MAST::UGFlutterSolver& solver,
                               const Real kr_ref,
                               const Real b_ref,
                               const MAST::LAPACK_ZGGEV_Base& eig_sol) {
    
    // make sure that it hasn't already been initialized
    libmesh_assert(!_roots.size());
    
    _ref_val           = kr_ref;
    
    // iterate over the roots and initialize the vector
    _Amat              = eig_sol.A();
    _Bmat              = eig_sol.B();
    const ComplexMatrixX
    &VR                = eig_sol.right_eigenvectors(),
    &VL                = eig_sol.left_eigenvectors();
    const ComplexVectorX
    &num               = eig_sol.alphas();
    
    const ComplexVectorX
    &den               = eig_sol.betas();
    
    // use only half the modes that have a positive frequency.
    unsigned int nvals = (int)_Bmat.rows();
    
    _roots.resize(nvals);
    for (unsigned int i=0; i<nvals; i++) {
        
        MAST::UGFlutterRoot* root = new MAST::UGFlutterRoot;
        root->init(kr_ref,
                   b_ref,
                   num(i),
                   den(i),
                   _Bmat,
                   VR.col(i),
                   VL.col(i));
        
        _roots[i] = root;
    }
}







void
MAST::UGFlutterSolution::sort(const MAST::FlutterSolutionBase& sol)
{
    const unsigned int nvals = this->n_roots();
    libmesh_assert_equal_to(nvals, sol.n_roots());
    
    // two roots with highest modal_participation dot product are sorted
    // in the same serial order
    for (unsigned int i=0; i<nvals-1; i++)
    {
        const MAST::FlutterRootBase& r = sol.get_root(i);
        Real max_val = 0.;
        Complex val = 0.;
        unsigned int max_val_root = nvals-1;
        for (unsigned int j=i; j<nvals; j++) {
            
            val = r.eig_vec_left.dot(_Bmat*_roots[j]->eig_vec_right);
            //_roots[j]->modal_participation.dot(r.modal_participation);
            // scale by the eigenvalue separation with the assumption that
            // the roots will be closer to each other than any other
            // root at two consecutive eigenvalues. In other words,
            // we are penalizing the dot product with the eigenvalue
            // distance
            val /= abs(r.root-_roots[j]->root);
            if (abs(val) > max_val) {
                max_val = abs(val);
                max_val_root = j;
            }
        }
        
        // now we should have the one with highest dot product
        std::swap(_roots[i], _roots[max_val_root]);
    }
}







void
MAST::UGFlutterSolution::print(std::ostream &output)
{
    // make sure that some roots exist for this solution
    libmesh_assert(this->n_roots() > 0);
    
    const unsigned int nvals = this->n_roots();
    unsigned int n_participation_vals =
    (unsigned int)this->get_root(0).modal_participation.size();
    libmesh_assert(nvals);
    
    // first write the reference values of the root
    output << " Flutter Root " << std::endl;
    
    // now write the root
    output
    << std::setw(5) << "#"
    << std::setw(15) << "k_ref"
    << std::setw(15) << "V_ref"
    << std::setw(15) << "g"
    << std::setw(15) << "omega"
    << std::setw(15) << "Re"
    << std::setw(15) << "Im"
    << std::setw(3)  << " | ";
    
    // output the headers for flutter mode participation
    for (unsigned int i=0; i<n_participation_vals; i++)
        output
        << std::setw(2) << " "
        << std::setw(5) << "Mode "
        << std::setw(5) << i
        << std::setw(2) << " ";
    
    // output the headers for flutter mode
    for (unsigned int i=0; i<nvals; i++)
        output
        << std::setw(10) << "|         "
        << std::setw(5) << "Mode "
        << std::setw(5) << i
        << std::setw(10) << "         |";
    output << std::endl;
    
    for (unsigned int i=0; i<nvals; i++)
    {
        const MAST::FlutterRootBase& root = this->get_root(i);
        
        if (root.if_nonphysical_root)
        {
            std::stringstream oss;
            oss << "**" << i;
            output << std::setw(5) << oss.str();
        }
        else
            output << std::setw(5) << i;
        
        // flutter root details
        output
        << std::setw(15) << root.kr
        << std::setw(15) << root.V
        << std::setw(15) << root.g
        << std::setw(15) << root.omega
        << std::setw(15) << std::real(root.root)
        << std::setw(15) << std::imag(root.root)
        << std::setw(3)  << " | ";
        
        // now write the modal participation
        for (unsigned int j=0; j<n_participation_vals; j++)
            output
            << std::setw(12) << root.modal_participation(j)
            << std::setw(2) << " ";
        
        // now write the flutter mode
        for (unsigned int j=0; j<nvals; j++)
        {
            output
            << std::setw(2) << "| "
            << std::setw(12) << std::real(root.eig_vec_right(j))
            << std::setw(2) << " "
            << std::setw(12) << std::imag(root.eig_vec_right(j))
            << std::setw(2) << " |";
        }
        output << std::endl;
    }
}



