#ifndef SAHKDTREE_H
#define SAHKDTREE_H
#include "trianglemesh.h"
#include "bbox.h"
#include "float.h"
#include <boost/smart_ptr.hpp>

//Weights for the SAH evaluation
#define KT 1.0F
#define KI 10.0F

class SAHKDTree
{
public:
    SAHKDTree();
    ~SAHKDTree();
    /// Builds the hierarchy over a set of bounded primitives
    void build(const std::vector<boost::shared_ptr<BaseObject> >& objects);

    /// Intersects a ray with the SAH KdTree.
    bool hit(const Ray &_ray, double& tmin, ShadePacket& sp) const;

    /// It returns the the bounding box for the root of the SAH Kd Tree.
    BBox getSceneBBox() const;

    BBox sceneBox;

    struct KDPlane {
        double cost;
        int dimension;
        float splitVal, value;
        bool left;
    };

    struct BuildState {
        BuildState()
        {}
        ~BuildState()
        {
            objects.reset();
        }

        boost::shared_ptr<std::vector<size_t> > objects;
        BBox volume;
        size_t nodeIndex;
        int depth;
    };

    struct TravElem {
        size_t nodeIndex;
        float t_near, t_far;
    };

    struct Node {
        enum {LEAF_FLAG_BIT = sizeof(size_t) * 8 - 1};
        size_t dataIndex;
        int dimension;
        double splitVal;

        bool isLeaf() const
        {
          const size_t NODE_TYPE_MASK = ((size_t)1 << LEAF_FLAG_BIT);
          return (dataIndex & NODE_TYPE_MASK) != 0;
        }

        size_t getLeftChildOrLeaf() const
        {
          const size_t INDEX_MASK = ((size_t)1 << LEAF_FLAG_BIT) - 1;
          return dataIndex & INDEX_MASK;
        }

        Node(int dim, float value)
        {
          dimension = dim;
          splitVal=value;
        }

        Node()
        {
          dimension = -1;
          splitVal = FLT_MAX;
        }
    };


    struct SortElem
    {
        float value;
        //The type of node.
        int type;

        SortElem()
        {
            value = FLT_MAX;
            type = -1;
        }

        SortElem(float val, int typ)
        {
            value = val;
            type = typ;
        }
    };



    std::vector<Node> m_nodes;
    std::vector<boost::shared_ptr<BaseObject> > m_leafData;

    KDPlane findPlane(const boost::shared_ptr<std::vector<size_t> >& objects_, BBox volume_,
                      std::vector<BBox>& objectBBoxes);

    /// SAH cost function. It returns the cost of the surface area.
    float C(float Pl, float Pr, int Nl, int Nr) const;

    /// Main method to evaluate the costs.
    /// It returns the value of the heuristic for the given plane.,
    /// if the tringles lying exactly in the plane are put on the left side,
    /// otherwise the are on the right side.
    std::pair<float, bool>
    SAH(int planeDim, float planeValue, BBox volume, int Nl, int Nr, int Np) const;

private:

};

#endif // SAHKDTREE_H
