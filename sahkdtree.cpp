#include "sahkdtree.h"
#include <stack>
#include <algorithm>

const float EPS = 0.000001f;
const int MAX_DEPTH = 100;

SAHKDTree::SAHKDTree()
{
	sceneBox = BBox();
}

SAHKDTree::~SAHKDTree()
{
	m_nodes.clear();
	for(int i = 0; i < m_leafData.size(); ++i)
		m_leafData[i].reset();
	m_leafData.clear();
}

void SAHKDTree::build(const std::vector<boost::shared_ptr<BaseObject> > &objects)
{
	// std::cout<<"in SAHKDTree::build"<<std::endl;
	std::vector<BBox> objectBBoxes(objects.size());
	BuildState curState;
	curState.objects.reset(new std::vector<size_t>);
	sceneBox = BBox();
	for(int i = 0; i < objects.size(); ++i)
		objectBBoxes[i] = BBox();

	for(size_t i = 0; i < objectBBoxes.size(); ++i){
		objectBBoxes[i] = objects[i]->getBBox();
		// std::cout<<objectBBoxes[i].toString()<<std::endl;
		sceneBox.extends(objectBBoxes[i]);
		curState.objects->push_back(i);
	}
	// std::cout<<sceneBox.toString()<<std::endl;

	curState.volume = sceneBox;
	curState.nodeIndex = 0;
	curState.depth = 0;

	m_nodes.resize(1);

	std::stack<BuildState> buildStack;

	// loop till stack is empty
	for(;;){

        KDPlane plane = findPlane(curState.objects,
		                          curState.volume,
		                          objectBBoxes);
		int splitDim = plane.dimension;
		float splitVal = plane.value;

		//Boundary condition
		if (plane.cost > KI * curState.objects->size() ||
		    curState.depth > MAX_DEPTH)
		{
			// std::cout<<"in boundary condition!"<<std::endl;
            const size_t NODE_TYPE_MASK = ((size_t)1 << Node::LEAF_FLAG_BIT);

			m_nodes[curState.nodeIndex].dataIndex =
			        m_leafData.size() | NODE_TYPE_MASK;
			for(int i = 0; i < curState.objects->size(); ++i){
				m_leafData.push_back(objects[(*curState.objects)[i]]);
			}
			// delete curState.objects;
			curState.objects.reset();

			// as delimiter, same as
			// m_leafData.push_back(NULL);
			m_leafData.push_back(boost::shared_ptr<BaseObject>());

			if (buildStack.empty())
				break;

			// update the stack
			curState = buildStack.top();
			buildStack.pop();

			continue;
		}

		// create a new state for the right child
		BuildState rightState;
		rightState.volume = curState.volume.split(splitDim, splitVal);
		rightState.objects.reset(new std::vector<size_t>);
		boost::shared_ptr<std::vector<size_t> > leftObjects(new std::vector<size_t>);

		++curState.depth;
		rightState.depth = curState.depth;

		for (int i = 0; i < curState.objects->size(); ++i){
			size_t objectIndex = (*curState.objects)[i];
			BBox objectBox = objectBBoxes[objectIndex];
			if (objectBox.min[splitDim] == splitVal &&
			    objectBox.min[splitDim] == objectBox.max[splitDim]){
				// object is planar and lies in the spliting plane
				if (plane.left){
					leftObjects->push_back(objectIndex);
				} else {
					rightState.objects->push_back(objectIndex);
				}
			} else {
				// assign the object to the correct side
				if (objectBox.max[splitDim] > splitVal){
					rightState.objects->push_back(objectIndex);
				}
				if (objectBox.min[splitDim] < splitVal){
					leftObjects->push_back(objectIndex);
				}
			}
		}

		// delete curState.objects;
		curState.objects = leftObjects;
		leftObjects.reset();

		m_nodes[curState.nodeIndex].dataIndex = m_nodes.size();
		m_nodes[curState.nodeIndex].dimension = splitDim;
		m_nodes[curState.nodeIndex].splitVal = splitVal;

		curState.nodeIndex = m_nodes.size();
		rightState.nodeIndex = curState.nodeIndex + 1;

		buildStack.push(rightState);

		// enlarge m_nodes to hold the left and right child
		m_nodes.resize(rightState.nodeIndex + 1);
	}
	// for(int i = 0; i != m_leafData.size(); ++i){
	//     if (m_leafData[i]){
    //         std::cout<<i<<", "<<m_leafData[i]->getBBox().toString()<<std::endl;
	//    }
	// }
}

bool SAHKDTree::hit(const Ray &ray, double& tmin, ShadePacket& sp) const
{
	std::stack<TravElem> traverseStack;

	TravElem curNode;
	curNode.nodeIndex = 0;

	// need update
	if (!sceneBox.intersect(ray, curNode.t_near, curNode.t_far))
		return false;
	// std::cout<<"in SAHKDTree::hit"<<std::endl;
	// std::cout<<"("<<curNode.t_near<<", "<<curNode.t_far<<")"<<std::endl;

	Vector3 inverse = ray.inv_d;
	bool leftIsNear[3];
	leftIsNear[0] = ray.d.x > 0;
	leftIsNear[1] = ray.d.y > 0;
	leftIsNear[2] = ray.d.z > 0;

	const float EPS = 0.000001f;

	double tmin_ = FLT_MAX, tmin_temp;
	ShadePacket sp_, sp_temp;

	for(;;){
		const SAHKDTree::Node& node = m_nodes[curNode.nodeIndex];
		if (node.isLeaf()){
			bool changed = false;
			size_t idx = node.getLeftChildOrLeaf();
			// std::cout<<"in node.isLeaf()"<<std::endl;
			while (m_leafData[idx]){
				// std::cout<<"try hitting!"<<std::endl;
				if (m_leafData[idx]->hit(ray, tmin_temp, sp_temp) &&
				    tmin_temp < tmin_){
					// std::cout<<"hit!!, tmin_ = "<<tmin_temp<<std::endl;
					tmin_ = tmin_temp;
					sp_ = sp_temp;
					changed = true;
				}
				idx++;
			}

			if (changed && tmin_ < curNode.t_far){
				break;
			}

			if (traverseStack.empty())
				break;

			curNode = traverseStack.top();
			traverseStack.pop();
		} else {
			float t_split =
			        (node.splitVal - ray.o.get(node.dimension))
			        * inverse[node.dimension];

			//The split is closer to the far child. The near child is ignored.
			if (curNode.t_near - t_split > EPS){
				curNode.nodeIndex = node.getLeftChildOrLeaf()
				                    + leftIsNear[node.dimension];
				continue;
			}
			//The split is closer to the near child. The far child is ignored.
            else if (t_split - curNode.t_far > EPS){
				curNode.nodeIndex = node.getLeftChildOrLeaf()
				                    + 1 - leftIsNear[node.dimension];
				continue;
			}
			//Otherwise, the right child is put on the stack.
			TravElem rightElement;

			rightElement.nodeIndex = node.getLeftChildOrLeaf()
			                         + leftIsNear[node.dimension];
			rightElement.t_near = t_split;
			rightElement.t_far = curNode.t_far;
			traverseStack.push(rightElement);

			curNode.nodeIndex = node.getLeftChildOrLeaf()
			                    + 1 - leftIsNear[node.dimension];
			curNode.t_far = t_split;
		}
	}
	tmin = tmin_;
	sp = sp_;
	return tmin < FLT_MAX;
}

float SAHKDTree::C(float Pl, float Pr, int Nl, int Nr) const
{
  return ((Nl == 0 || Nr == 0) ? 0.8f : 1.0f)
    * (KT + KI * (Pl * Nl + Pr * Nr));
}

std::pair<float, bool>
SAHKDTree::SAH(int planeDim, float planeValue, BBox volume, int Nl, int Nr, int Np) const
{
    float totalArea = volume.getSurfaceArea();
    //The  current volume is splitted into two parts: right and left side.
    //The left side is assigned to the current volume, while the right side
    //to a new volume.
    BBox right = volume.split(planeDim, planeValue);
    float Pl = volume.getSurfaceArea()/totalArea;
    float Pr = right.getSurfaceArea()/totalArea;
    //The cost function enables us to decide whether to put the triangle.
    float Cl = C(Pl, Pr, Nl + Np, Nr);
    float Cr = C(Pl, Pr, Nl, Np + Nr);
    std::pair<float, bool> ret;
    if(Cl < Cr){
        ret.first = Cl;
        ret.second = true;
        return ret;
    } else {
        ret.first = Cr;
        ret.second = false;
        return ret;
    }
}

//It checkss whether the node e1 is less than, equal to or greater than e2.
//If both are equal, then the decision is made based on the type of node.
bool compE(SAHKDTree::SortElem e1, SAHKDTree::SortElem e2)
{
    if (e1.value < e2.value) {
        return true;
    } else if (e1.value == e2.value) {
        return e1.type < e2.type;
    } else
		return false;
}


SAHKDTree::KDPlane SAHKDTree::findPlane
(const boost::shared_ptr<std::vector<size_t> > &objects_, BBox volume, std::vector<BBox> &triBoxes)
{

    //Splitting plane. By default the splitting plane must tell us that no
    //further split is possible.
    SAHKDTree::KDPlane plane;
    plane.dimension = -1;
    plane.value = FLT_MAX;
    plane.cost = FLT_MAX;
    //Number of triangles to the left, to the right and in the middle.
    int Nl, Np, Nr;

    //The area is too small. Therefore, we have to stop.
    if(volume.getArea() < EPS) {
        return plane;
    }

    //Sorting list.
    std::vector<SortElem> eventList(1);

    //The three branches are checked for all the triangles.
    for(int k = 0; k < 3; k++)
    {
        //New event list for each branch.
        eventList.clear();

        for(int i = 0; i < objects_->size(); i++)
        {
            BBox triBox = triBoxes[(*objects_)[i]];
            float triMin = std::max(triBox.min[k], volume.min[k]);
            float triMax = std::min(triBox.max[k], volume.max[k]);
            //The cut is in the middle (planar case).
            if(triMin == triMax)
            {
                eventList.push_back(SortElem(triMin, 1));
            }
            //We look for new events to the left and right.
            else
            {
                //Starting event.
                eventList.push_back(SortElem(triMin, 2));
                //End event.
                eventList.push_back(SortElem(triMax, 0));
            }
        }

        //Sorting the list, based on the costs.
        sort(eventList.begin(), eventList.end(), compE);

        Nl = 0;
        Np = 0;
        Nr = objects_->size();

        //Counters for the iterations.
        int PPlus, PMinus, PPlanar;
        //This variable stores the cost of the current plane.
        float PXi;

        //Based on the list, we find the best splitting plane.
        int i = 0;
        while(i < eventList.size()) {
            PPlus = 0; PMinus = 0; PPlanar = 0;
            //Current cost value.
            PXi = eventList[i].value;

            //We count all the triangles which are to the left/right and in
            //the middle.
            while(i < eventList.size() && eventList[i].value == PXi
                  && eventList[i].type == 0) {
                ++PMinus; ++i;
            }
            while(i < eventList.size() && eventList[i].value == PXi
                  && eventList[i].type == 1) {
                ++PPlanar; ++i;
            }
            while(i < eventList.size() && eventList[i].value == PXi
                  && eventList[i].type == 2) {
                ++PPlus; ++i;
            }

            //Found new plane, evaluate SAH for old plane.
            Np = PPlanar;
            Nr -= PPlanar;
            Nr -= PMinus;

            std::pair<float, bool> helpCost;
            //If the splitting is far enough from the volume boundaries,
            //we don't evaluate anything and the cost must be infty so that the
            //plane cannot be updated afterwards.
            if(PXi <= volume.min[k] + EPS || PXi + EPS >= volume.max[k]) {
                helpCost.first = FLT_MAX;
            }
            //Otherwise, the cost of the current plane is evaluated.
            else {
                helpCost = SAH(k, PXi, volume, Nl, Nr, Np);
            }

            //Updating the counts.
            Nl += PPlus;
            Nl += PPlanar;
            Np = 0;

            ///If the current cost is better than the cost of the plane, the
            //plane is updated.
            if (helpCost.first < plane.cost) {
                plane.cost = helpCost.first;
                plane.dimension = k;
                plane.value = PXi;
                plane.left = helpCost.second;
            }
        }
    }

    return plane;
}
