#ifndef GRAPHNODE_H_INCLUDED
#define GRAPHNODE_H_INCLUDED

template<typename T>
struct GRAPH_DATA_NODE {
	int _iXPos = 0;
	int _iYPos = 0;
	float _fXSpeed = 4.0f;
	T value = 0;
	wxColour _col = wxColour(0, 170, 0);
	wxPenStyle _penStyle = wxPENSTYLE_DOT;
	bool _bLineDraw = true;
	GRAPH_DATA_NODE<T>* _next;
	GRAPH_DATA_NODE(const T& v) : value(v), _next(nullptr) {}
};

template<typename T>
class CGLinkedList
{
public:
	CGLinkedList() {}
    ~CGLinkedList() { clear(); }

public:
    int GetSize() const { return size; }

	void push_back(T v, int xPos, int yPos, int h, bool secondLine = false, bool bOriginReverse = false)
	{
		secondindex = 0;
		m_bOriginReverse = bOriginReverse;

		GRAPH_DATA_NODE<T> *gnode = new GRAPH_DATA_NODE(v);
		gnode->_iXPos = xPos;
		gnode->_iYPos = (yPos - (v * h / 100));
		gnode->_bLineDraw = secondLine;

		if(!bOriginReverse)
			secondindex = 1;

		AddNode(gnode, v);
	}

	void push_back(T v, int xPos, int h, bool secondLine = false, bool bOriginReverse = false)
	{
		secondindex = 0;
		m_bOriginReverse = bOriginReverse;

		GRAPH_DATA_NODE<T> *gnode = new GRAPH_DATA_NODE(v);
		gnode->_iXPos = xPos;
		gnode->_iYPos = (int)(v * h / 100);
		gnode->_bLineDraw = secondLine;

		if(!bOriginReverse)
		{
			secondindex = 1;
			gnode->_iYPos += h;
		}

		AddNode(gnode, v);
	}

	GRAPH_DATA_NODE<T>* GetFirst()
	{
		return head;
	}

	GRAPH_DATA_NODE<T>* GetNext()
	{
		if(nodePosition == 0)
			nodeCurr = head;
		else
		{

			if(nodeCurr == nullptr)
				return nullptr;

			nodeCurr = nodeCurr->_next;
		}

        nodePosition++;
		return nodeCurr;
	}

	void InitNodeIndex() { nodePosition = 0; }
	void update()
	{
		GRAPH_DATA_NODE<T>* node = head;
		while(node->_next != nullptr)
		{
			node->_iXPos += secondCalc[secondindex];
			node = node->_next;
		}
	}

	void update(GRAPH_DATA_NODE<T>* node)
	{
		if(!node)
			return;

		node->_iXPos += secondCalc[secondindex];
	}

	void pop_front()
	{
		if(!head) return;

		GRAPH_DATA_NODE<T>* temp = head;
		head = head->_next;

		delete temp;

        nodePosition = 0;
		size--;
		if(size == 0) tail = nullptr;
	}

	void clear()
	{
		while(head)
			pop_front();
	}

private:
	void AddNode(GRAPH_DATA_NODE<T>* n, T v)
	{
		int iRed = (255 * v) / 100;
		int iGreen = iRed == 0 ? 255 : 255 - iRed;

		wxColour col(iRed, iGreen, 0);

		n->_col = col;

		if(!head)
			head = tail = n;
		else
		{
			tail->_next = n;
			tail = n;
		}

		size++;
	}
private:
	GRAPH_DATA_NODE<T>* head = nullptr;
	GRAPH_DATA_NODE<T>* tail = nullptr;

	size_t size = 0;
    unsigned int nodePosition = 0;

	GRAPH_DATA_NODE<T>* nodeCurr = nullptr;

	bool m_bOriginReverse = false;
	unsigned int secondindex = 0;
	int secondCalc[2]{CONST_SECOND_GAP, -CONST_SECOND_GAP};
};

#endif // GRAPHNODE_H_INCLUDED
