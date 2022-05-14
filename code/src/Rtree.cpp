#include "Rtree.h"

bool comp(const Point* p1, const Point* p2) {

	const int* v1 = p1->coords ;
	const int* v2 = p2->coords ;
	int pos = 0;
	int curMax = 0;
	int temp = 0;
	int d = DIM;

	for (int i = 0; i < d; i++) {
		temp = v1[i] ^ v2[i];
		if (curMax < temp && curMax < (temp ^ curMax)) {
			// Record the dimension where the different most significant bit lies.
			pos = i;
			curMax = temp;
		}
	}
	return v1[pos] < v2[pos];

}

Rectangle::Rectangle() {
    this->minValues = new int[DIM];
    this->maxValues = new int[DIM];
}

Rectangle::Rectangle(int* minValues, int* maxValues) {
    this->minValues = minValues;
    this->maxValues = maxValues;
}

Rectangle::~Rectangle() {
    delete[] this->maxValues;
    delete[] this->minValues;
}


Rectangle& Rectangle::enlarge(const Rectangle& rec) {
    int d = DIM;
    for (int i = 0; i < d; i++) {
        //(this->minValues[i] < rec.minValues[i]) ?  : this->minValues[i] = rec.minValues[i];
        //(this->maxValues[i] > rec.maxValues[i]) ?  : this->maxValues[i] = rec.maxValues[i];
        //this->minValues[i] = min(this->minValues[i], rec.minValues[i]);
        //this->maxValues[i] = max(this->maxValues[i], rec.maxValues[i]);
        (this->minValues[i] < rec.minValues[i]) ? this->minValues[i] = this->minValues[i] : this->minValues[i] = rec.minValues[i];
        (this->maxValues[i] > rec.maxValues[i]) ? this->maxValues[i] = this->maxValues[i] : this->maxValues[i] = rec.maxValues[i];
    }
    return *(this);
}

void Rectangle::setMinCoords(const int* minCoords) {
    int d = DIM;
    if (this->minValues == NULL) {
        this->minValues = new int[DIM];
    }
    for (int i = 0; i < d; i++) {
        this->minValues[i] = minCoords[i];
    }
}

void Rectangle::setMaxCoords(const int* maxCoords) {
    int d = DIM;
    if (this->maxValues == NULL) {
        this->maxValues = new int[DIM];
    }
    for (int i = 0; i < d; i++) {
        this->maxValues[i] = maxCoords[i];
    }
}

int Rectangle::stateWithSphere(Point* q, double sqr_eps) {
    
    //clock_t rq3 ;
	//clock_t rq4 ;
    
    int d = DIM;
    unsigned int closestDist = 0;
    unsigned int farthestDist = 0;
    int temp = 0;
    int temp2 = 0;
    unsigned int sqr_temp = 0;
    unsigned int sqr_temp2 = 0;

    // Find the distances from the closest and farthest points to q in this grid cell.
    for (int i = 0; i < d; i++) {
        //ls++;
        //rq3 = clock();
        temp = this->minValues[i] - q->coords[i];
        temp2 = this->maxValues[i] - q->coords[i];
        sqr_temp = temp * temp;
        sqr_temp2 = temp2 * temp2;

        if (temp > 0) {
            // q is to the left of this rectangle in this dimension
            closestDist += sqr_temp;
        }
        else if (temp2 < 0) {
            // q is to the right of this rectangle in this dimension
            closestDist += sqr_temp2;
        }
        farthestDist += (sqr_temp <= sqr_temp2 ? sqr_temp2 : sqr_temp);
        //rq4 = clock();
        //zrq2 += (rq4-rq3);
    }
    
    if (closestDist <= sqr_eps) {
        if (farthestDist <= sqr_eps)
            return 1; // fully inside
        return 0; // intersect
    }
    return -1; // fully outside
}

RtreeNode::RtreeNode() {
    // Means that this node currently is empty and invalid.
    this->level = -1;
    this->pt = NULL;
}

RtreeNode::RtreeNode(Point* pt) {
    this->level = 0;
    this->pt = pt;
    this->mbr.setMinCoords(pt->coords);
    this->mbr.setMaxCoords(pt->coords);
}

RtreeNode::~RtreeNode() {
    this->releaseSpace();
}

void RtreeNode::releaseSpace() {
    int num = this->childNodes.size();
    if (num == 0)
        return;
    for (int i = 0; i < num; i++) {
        this->childNodes[i]->releaseSpace();				
        delete (this->childNodes[i]);
        this->childNodes[i] = NULL;
    }
    vector<RtreeNode*>().swap(this->childNodes);
    this->childNodes.clear();
}

vector<RtreeNode*>& RtreeNode::getChildNodes() {
    return this->childNodes;
}		

void RtreeNode::addChildNode(RtreeNode* child) {
    // Update the MBR for the parent node.
    if (this->level == -1) {
        this->mbr.setMinCoords(child->mbr.minValues);
        this->mbr.setMaxCoords(child->mbr.maxValues);
        this->level = child->level + 1;
    }

    this->mbr.enlarge(child->mbr);
    this->childNodes.emplace_back(child);
}

Rtree::Rtree() {
    this->root = NULL;
}

Rtree::~Rtree() {
    this->releaseSpace();
}

void Rtree::releaseSpace() {
    if (this->root != NULL) {
        // Release the space of R-tree.
        this->root->releaseSpace();
        delete (this->root);
        this->root = NULL;
    }
}

void Rtree::bulkLoadRtree(vector<Point*>& ptList) {
    int fanout = 10;
    int num = ptList.size();
    if (num == 0) {
        //showError("Error in Rtree::bulkLoadRtree: the point list is empty!\n");
        return;
    }
    else if (num == 1) {
        this->root = new RtreeNode(ptList[0]);
        return;
    }


    queue<RtreeNode*> que, que2;
    queue<RtreeNode*>& work = que;
    queue<RtreeNode*>& store = que2;

    /*************** Sort directly by the coordinates of points ****************/

    sort(ptList.begin(), ptList.end(), comp);
    for (int i = 0; i < num; i++) {
        que.push(new RtreeNode(ptList[i]));
    }


    RtreeNode* parent = NULL;
    bool flag = false;
    while (work.size() + store.size() != 1) {
        parent = new RtreeNode();

        for (int i = 0; i < fanout; i++) {
            if (!work.empty()) {
                parent->addChildNode(work.front());
                work.pop();
            }
            else {
                flag = true;
                break;
            }
        }

        if (flag) {
            if (parent->getChildNodes().size() > 0) {
                store.push(parent);
            }
            else {
                delete (parent);
            }
            std::swap(work, store);
            flag = false;
        }
        else {
            store.push(parent);
        }
    }
    if (work.empty())
        this->root = store.front();
    else
        this->root = work.front();
}

void Rtree::rangeQuery(Point* pt, double sqr_eps, vector<Point*>& targetPlace) {
    /*if (this->root == NULL) {
        //showError("Error in Rtree::rangeQuery: The root is NULL!\n");
    }*/

    int d = DIM;
    register unsigned int temp;
    //double temp;
    register unsigned int dist = 0;
    Point* pt2;

    // Clear the targetPlace.
    targetPlace.clear();

    int childNum = 0;
    int state = 0;
    unsigned int sqrDist = 0;

    queue<RtreeNode*> que;
    que.push(this->root);
    RtreeNode* cur_node = NULL;

    /*clock_t rq1 ;
    clock_t rq2 ;
    clock_t rq3 ;
    clock_t rq4 ;*/

    while (!que.empty()) {
        cur_node = que.front();
        que.pop();

        
        // If the current node is a leaf, compute the distance from query point to the point inside this leaf.
        if (cur_node->level == 0) {
            //
            //sqrDist = SqrDist(pt ,cur_node->getPoint() );
            //rq1 = clock();

            pt2 = cur_node->pt;
            dist = 0;
            
            for (int i = 0; i < d; ++i) {
                temp = pt->coords[i] - pt2->coords[i];
                dist += temp * temp;
            }
            
            sqrDist = dist;


            if (sqrDist <= sqr_eps)
                targetPlace.emplace_back(cur_node->pt);
            //rq2 = clock();
            //zrq1 += (rq2-rq1);
        }
        else {
            // If the current node is not a leaf, check its child nodes and add those intersecting with the rectangle of q to the queue.
            

            vector<RtreeNode*>& childList = cur_node->getChildNodes();
            childNum = childList.size();
            
            for (int i = 0; i < childNum; i++) {
                //				start1 = getCurrentTime();
                
                state = childList[i]->mbr.stateWithSphere(pt, sqr_eps);
                //				end1 = getCurrentTime();
                
                //rq3 = clock();
                if (state != -1) {//ls++;
                    // intersect or fully inside, then add this tree node to queue
                    que.push(childList[i]);
                }
                //rq4 = clock();
                //zrq2 += (rq4-rq3);
            }

            

        }
    }
}
