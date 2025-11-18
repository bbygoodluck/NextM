#ifndef NEXTMGRAPHVIEW_H_INCLUDED
#define NEXTMGRAPHVIEW_H_INCLUDED

#include "../utility/MemoryDC.h"
#include "../system/IUpdateListener.h"

template<typename T>
struct GRAPH_DATA_NODE {
	float _iXPos = 0;
	float _iYPos = 0;
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

	void push_back(T v, float xPos, int h, bool secondLine = false)
	{
		GRAPH_DATA_NODE<T> *gnode = new GRAPH_DATA_NODE(v);
		gnode->_iXPos = xPos;
		gnode->_iYPos = (float)(v * h / 100.0);
		gnode->_bLineDraw = secondLine;
		/*
		if(v >= 75)
			gnode->_fXSpeed = 6.0f + (rand() % 2);
		else if(v >= 50)
			gnode->_fXSpeed = 4.0f + (rand() % 2);
		*/

		int iRed = (255 * v) / 100;
		int iGreen = iRed == 0 ? 255 : 255 - iRed;

		wxColour col(iRed, iGreen, 0);

		gnode->_col = col;

		if(!head)
			head = tail = gnode;
		else
		{
			tail->_next = gnode;
			tail = gnode;
		}

		size++;
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
			node->_iXPos += CONST_SECOND_GAP;
			node = node->_next;
		}
	}

	void update(GRAPH_DATA_NODE<T>* node)
	{
		if(!node)
			return;

		node->_iXPos += CONST_SECOND_GAP;//node->_fXSpeed;
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
	GRAPH_DATA_NODE<T>* head = nullptr;
	GRAPH_DATA_NODE<T>* tail = nullptr;

	size_t size = 0;
    unsigned int nodePosition = 0;

	GRAPH_DATA_NODE<T>* nodeCurr = nullptr;
};

class CNextMGraphView : public wxWindow, public IUpdateListener
{
	unsigned int rulerRGB  = 128;
	unsigned int secondRGB = 128;
	unsigned int timeVal   = 100;
public:
	explicit CNextMGraphView(wxWindow* parent, const int nID, const wxSize& sz, long lStyle = CHILD_WINDOW_STYLE);
	~CNextMGraphView();

public:
	void SetCoreIndex(unsigned int indx) {
		m_coreIndex = indx;
	}

private:
	void Render(wxDC* pDC);
	void DrawBackGround(wxDC* pDC, const wxColour& colPen, const wxColour& colBrush);
	void DrawGraph(wxDC* pDC);
	void DrawRuler(wxDC* pDC);

private:
	CMemoryDC m_memDC;
	//배경
	wxColour m_colBackPen;
	wxColour m_colBackBrush;
	//눈금컬러
	wxColour m_colRuler;
	wxColour m_colSecond;
	//전체크기
	wxRect m_viewRect = wxRect(0, 0, 0, 0);
    //그래프 표시 영역
    wxRect m_viewGraphRect = wxRect(0, 0, 0, 0);

	CGLinkedList<unsigned int> nodeList;

	wxFont m_viewFont = wxNullFont;
	bool m_bThreadStop = false;

	unsigned int m_coreIndex = 0;
	unsigned int m_Index = 0;;
	unsigned long m_coreUsage = 0;

private:
	void OnPaint(wxPaintEvent& event);
	void OnErase(wxEraseEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnTimer(wxTimerEvent& event);

	void UpdateListener(wxCommandEvent& event) override;
	wxDECLARE_EVENT_TABLE();
};

#endif // NEXTMGRAPHVIEW_H_INCLUDED
