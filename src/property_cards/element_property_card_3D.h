/*
 * MAST: Multidisciplinary-design Adaptation and Sensitivity Toolkit
 * Copyright (C) 2013-2014  Manav Bhatia
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

#ifndef __mast__element_property_card_3D__
#define __mast__element_property_card_3D__

// MAST includes
#include "property_cards/element_property_card_base.h"



namespace MAST
{
    
    class ElementPropertyCard3D:
    public MAST::ElementPropertyCardBase {
        
    public:
    public:
        ElementPropertyCard3D(unsigned int pid):
        MAST::ElementPropertyCardBase(pid),
        _material(NULL)
        { }
        
        /*!
         *   virtual destructor
         */
        virtual ~ElementPropertyCard3D() { }
        
        
        /*!
         *   dimension of the element for which this property is defined
         */
        virtual unsigned int dim() const {
            return 3;
        }

        /*!
         *   return true if the property is isotropic
         */
        virtual bool if_isotropic() const {
            return true;
        }

        
        /*!
         *    sets the material card
         */
        virtual void set_material(MAST::MaterialPropertyCardBase& mat) {
            _material = &mat;
        }
        
        
        /*!
         *    returns a reference to the material
         */
        const MAST::MaterialPropertyCardBase& get_material() const {
            libmesh_assert(_material); // make sure it has already been set
            return *_material;
        }
        

        /*!
         *  returns true if the property card depends on the function \p f
         */
        virtual bool depends_on(const MAST::FunctionBase& f) const;

        
        virtual std::auto_ptr<MAST::FieldFunction<RealMatrixX> >
        stiffness_A_matrix(const MAST::ElementBase& e) const;
        
        virtual std::auto_ptr<MAST::FieldFunction<RealMatrixX> >
        stiffness_B_matrix(const MAST::ElementBase& e) const;
        
        virtual std::auto_ptr<MAST::FieldFunction<RealMatrixX> >
        stiffness_D_matrix(const MAST::ElementBase& e) const;
        
        virtual std::auto_ptr<MAST::FieldFunction<RealMatrixX> >
        damping_matrix(const MAST::ElementBase& e) const;
        
        virtual std::auto_ptr<MAST::FieldFunction<RealMatrixX> >
        inertia_matrix(const MAST::ElementBase& e) const;
        
        virtual std::auto_ptr<MAST::FieldFunction<RealMatrixX> >
        thermal_expansion_A_matrix(const MAST::ElementBase& e) const;
        
        virtual std::auto_ptr<MAST::FieldFunction<RealMatrixX> >
        thermal_expansion_B_matrix(const MAST::ElementBase& e) const;
        
        virtual std::auto_ptr<MAST::FieldFunction<RealMatrixX> >
        transverse_shear_stiffness_matrix(const MAST::ElementBase& e) const;
        
        virtual std::auto_ptr<MAST::FieldFunction<RealMatrixX> >
        prestress_A_matrix(const MAST::ElementBase& e) const;
        
        virtual std::auto_ptr<MAST::FieldFunction<RealMatrixX> >
        prestress_B_matrix(const MAST::ElementBase& e) const;
        
        virtual std::auto_ptr<MAST::FieldFunction<RealMatrixX> >
        thermal_conductance_matrix(const MAST::ElementBase& e) const;
        
        virtual std::auto_ptr<MAST::FieldFunction<RealMatrixX> >
        thermal_capacitance_matrix(const MAST::ElementBase& e) const;
        
    protected:
        
        /*!
         *    pointer to the material property card
         */
        MAST::MaterialPropertyCardBase* _material;
    };
}


#endif // __mast__element_property_card_3D__