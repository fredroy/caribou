#ifndef SOFACARIBOU_GRAPHCOMPONENTS_FORCEFIELD_HEXAHEDRONELASTICFORCE_H
#define SOFACARIBOU_GRAPHCOMPONENTS_FORCEFIELD_HEXAHEDRONELASTICFORCE_H

#include <sofa/core/behavior/ForceField.h>
#include <sofa/core/topology/BaseTopology.h>
#include <sofa/core/behavior/MechanicalState.h>

#include <Caribou/Algebra/Matrix.h>
#include <Caribou/Algebra/Vector.h>
#include <Caribou/Geometry/Hexahedron.h>

namespace SofaCaribou::GraphComponents::forcefield {

using namespace sofa::core;
using namespace sofa::core::objectmodel;
using namespace sofa::core::behavior;
using namespace sofa::core::topology;

template<class DataTypes>
class HexahedronElasticForce : public ForceField<DataTypes>
{
public:
    SOFA_CLASS(SOFA_TEMPLATE(HexahedronElasticForce, DataTypes), SOFA_TEMPLATE(ForceField, DataTypes));

    // Type definitions
    using Inherit  = ForceField<DataTypes>;
    using VecCoord = typename DataTypes::VecCoord;
    using VecDeriv = typename DataTypes::VecDeriv;
    using Coord    = typename DataTypes::Coord;
    using Deriv    = typename DataTypes::Deriv;
    using Real     = typename Coord::value_type;

    using Mat33   = typename caribou::algebra::Matrix<3, 3, Real>;
    using Vec3   = typename caribou::algebra::Vector<3, Real>;
    using Mat2424 = typename caribou::algebra::Matrix<24, 24, Real>;
    using Vec24   = typename caribou::algebra::Vector<24, Real>;

    using Hexahedron = caribou::geometry::Hexahedron<caribou::geometry::interpolation::Hexahedron8>;

    template <typename ObjectType>
    using Link = SingleLink<HexahedronElasticForce<DataTypes>, ObjectType, BaseLink::FLAG_STRONGLINK>;

    // Public methods

    HexahedronElasticForce();

    void init() override;
    void reinit() override;

    void addForce(
            const MechanicalParams* mparams,
            Data<VecDeriv>& d_f,
            const Data<VecCoord>& d_x,
            const Data<VecDeriv>& d_v) override;

    void addDForce(
            const MechanicalParams* /*mparams*/,
            Data<VecDeriv>& /*d_df*/,
            const Data<VecDeriv>& /*d_dx*/) override;

    void draw(const sofa::core::visual::VisualParams* vparams) override;

    SReal getPotentialEnergy(
            const MechanicalParams* /* mparams */,
            const Data<VecCoord>& /* d_x */) const override
    {return 0;}

    void addKToMatrix(sofa::defaulttype::BaseMatrix * /*matrix*/, SReal /*kFact*/, unsigned int & /*offset*/) override;

    void computeBBox(const sofa::core::ExecParams* params, bool onlyVisible) override;

    struct GaussNode {
        Real weight;
        Real jacobian_determinant;
        caribou::algebra::Matrix<Hexahedron::gauss_nodes.size(), 3> dN_dx;
    };

    template <typename T>
    inline
    Hexahedron make_hexa(std::size_t hexa_id, const T & x) const
    {
        auto * topology = d_topology_container.get();
        const auto &node_indices = topology->getHexahedron(hexa_id);

        std::array<caribou::geometry::Node<3>, 8> nodes;
        for (std::size_t j = 0; j < 8; ++j) {
            const auto &node_id = node_indices[j];
            nodes[j] = x[node_id];
        }

        return Hexahedron(nodes);
    }

protected:
    Data< Real > d_youngModulus;
    Data< Real > d_poissonRatio;
    Data< bool > d_linear_strain;
    Data< bool > d_corotated;
    Link<BaseMeshTopology> d_topology_container;

private:
    std::vector<caribou::algebra::Matrix<24, 24, Real>> p_stiffness_matrices;
    std::vector<std::array<GaussNode,8>> p_quatrature_nodes;
    std::vector<Mat33> p_initial_rotation;
    std::vector<Mat33> p_current_rotation;

};

} // namespace SofaCaribou::GraphComponents::forcefield

#endif //SOFACARIBOU_GRAPHCOMPONENTS_FORCEFIELD_HEXAHEDRONELASTICFORCE_H