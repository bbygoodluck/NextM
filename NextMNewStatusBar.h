#ifndef NEXTMNEWSTATUSBAR_H_INCLUDED
#define NEXTMNEWSTATUSBAR_H_INCLUDED

#include "./utility/MemoryDC.h"

///////////////////////////////////////////////////////////////////////////////
// Name:        NextMNewStatusBar.h
// Purpose:
// Author:      Bae ByoungYong
// Modified by:
// Created:     09.11.2024
// Copyright:   (c) 2024 BaeByoungYong <bbygoodluck@gmail.com>
// Licence:     GPL3
///////////////////////////////////////////////////////////////////////////////

class CNextMStatusBarItem
{
public:
	int GetWidth() const     { return m_nWidth; }
    int GetStyle() const     { return m_nStyle; }
    wxString GetText() const { return m_text;   }
    wxRect GetRect() const   { return m_rect;   }

	void SetText(const wxString& text) {
		m_text = text;
	}

    void SetWidth(int width) { m_nWidth = width; }
    void SetRect(const wxRect& rc) { m_rect = rc;}
    void SetTextAlignment(wxAlignment align) {
		m_textAligment = align;
    }

    wxAlignment GetAlign() {
    	return m_textAligment;
    }

    void SetMouseOver(bool bMouseOver) { m_bMouseOver = bMouseOver; }
    const bool GetMouseMove() const { return m_bMouseOver; }

    void SetMouseOverTextColour(const wxColour& colText) { m_colMouseOverText = colText; }
    void SetMouseOverTextColor(const wxColour& colText) { m_colMouseOverText = colText; }
    wxColour GetMouseOverTextColour() { return m_colMouseOverText; }
    wxColor GetMouseOverTextColor() { return m_colMouseOverText; }

    void SetMouseOverBackgroundColour(const wxColour& colBack) { m_colMouseOverBackground = colBack; }
    void SetMouseOverBackgroundColor(const wxColour& colBack) { m_colMouseOverBackground = colBack; }
	wxColour GetMouseOverBackgroundColour() { return m_colMouseOverBackground; }
	wxColor GetMouseOverBackgroundColor() { return m_colMouseOverBackground; }

private:
    int m_nStyle;
    int m_nWidth;

	bool m_bMouseOver = false;
	wxRect m_rect;
    wxString m_text;

    wxColour m_colMouseOverText = wxColour(0, 0, 0);
    wxColour m_colMouseOverBackground = wxColour(255, 255, 128);

    wxAlignment m_textAligment = wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT;
};

WX_DECLARE_EXPORTED_OBJARRAY(CNextMStatusBarItem, NextMStatusBarItemArray);

class CNextMNewStatusBar : public wxControl
{
	enum _DEFAULT
	{
		Field_F2 = 0,
		Field_F3,
		Field_F4,
		Field_F5,
		Field_F7,
		Field_F8,
		Field_F10,
		Field_Shift,
		Field_CapsLockIndicator,
		Field_NumLockIndicator,
		Field_Clock,
		Field_Max
	};

	enum _CTRL
	{
		CTRL_Field_T = 0, // Ctrl_T 대응
		CTRL_Field_C,     // Ctrl_C
		CTRL_Field_X,     // Ctrl_X
		CTRL_Field_V,     // Ctrl_V
		CTRL_Field_M,     // Ctrl_M
		CTRL_Field_F11,   // Ctrl_F11
		CTRL_Field_Max
	};

	enum _ALT
	{
		ALT_Field_C = 0, // Alt_C 대응
		ALT_Field_M,     // Alt_M
		ALT_Field_S,     // Alt_S
		ALT_Field_V,     // Alt_V
		ALT_Field_H,     // Alt_H
		ALT_Field_X,     // Alt_X
		ALT_Field_Max
	};

	enum _SHAPE_TYPE
	{
		SHAPE_NONE = -1,
		SHAPE_LEFT_LINE = 0,
		SHAPE_RIGHT_LINE,
		SHAPE_SHADOW_LINE_RAISED,
		SHAPE_SHADOW_LINE_SUNKEN,
		SHAPE_RECTANGLE
	};

	enum _KEY_INPUT_FLAG
	{
		KEY_DEFAULT = 0,
		KEY_CTRL,
		KEY_ALT,
		KEY_INPUT_MAX
	};

	enum _KEY_NUM_CAPS_SHIFT
	{
		KEY_NUMLOCKS = 0,
		KEY_CAPSLOCK,
		KEY_SHIFT,
		KEY_NUM_CAPS_SHIFT_MAX
	};
public:
	CNextMNewStatusBar();
	CNextMNewStatusBar(wxWindow *parent,
                wxWindowID id = wxID_ANY,
                long style = CHILD_WINDOW_STYLE,
                const wxString& name = wxT(""));

    bool Create(wxWindow *parent,
                wxWindowID id = wxID_ANY,
                long style = CHILD_WINDOW_STYLE,
                const wxString& name = wxT(""));

	~CNextMNewStatusBar();

	void SetFieldsCount(int number, const int *widths = nullptr);
	void SetStatusWidths(int n, const int widths[]);
	void SetStatusRect(int n, const wxRect& rc);
	void SetStatusText(const wxString& text, int n);
	void SetStatusTextAlign(int n, wxAlignment align);
	void SetStatusMouseOverColor(int n, bool bMouseOver);

	const CNextMStatusBarItem& GetField(int index) const;

	/*
	*
	* 백그라운드 컬러
	*
	*/
	void SetBackgroundColor(const wxColour& colBackground);

	/*
	*
	* 컬럼 테두리 컬러
	*
	*/
	void SetPaneColor(const wxColour& colPane);

	/*
	*
	* 컬럼과 컬럼사이 스페이스 크기
	*
	*/
	void SetColSpacing(const int space);

private:
	void Render(wxDC* pDC);
	void DrawShape(wxDC* pDC);

	void Settings(wxDC* pDC);
	void Settings();

	void UpdateClock();

	void InitDefaultInfo(wxDC* pDC);
	void InitCtrlInfo(wxDC* pDC);
	void InitAltInfo(wxDC* pDC);

	void CalcRect(int itemCount, const int widths[]);
	bool FindItemInMousePoint(const wxPoint& pt);

	void ExecuteStatusBarClick(const std::unordered_map<int, wxString>& _statusMap, int nIndex);

private:
	/*
	*
	* memoryDC
	*
	*/
	CMemoryDC m_memDC;

	NextMStatusBarItemArray m_panes;

	wxRect m_viewRect = wxRect(0, 0, 0, 0);
	wxRect m_viewClientRect = wxRect(0, 0, 0, 0);

	bool m_bSameWidthForAllPanes = false;
	bool m_bSet = false;
	bool m_bChangedSize = false;

	int m_defaultWidth  = -1;
	int m_defaultHeight = 22;

	wxColour m_colBackground = wxColour(220, 220, 220);
	wxColour m_colEdgeColour = wxColour(157, 120, 98);

	int m_colSpacing = 2;
	int m_spacing = 0;
	wxFont m_viewFont = wxNullFont;

	wxTimer m_timer;

	bool m_bKeyPress[KEY_INPUT_MAX] = {false, false, false }; //none, ctrl, alt
	int m_Num_Caps_Shift[KEY_NUM_CAPS_SHIFT_MAX] = {0, 0, 0};

	unsigned int m_shapeType = SHAPE_LEFT_LINE;

	bool m_bMouseEnter   = false;
	bool m_bMouseFound   = false;
	int  m_indxMouseOver = -1;

	wxPoint m_ptMouse;

	std::unordered_map<int, wxString> m_defaultMap;
	std::unordered_map<int, wxString> m_ctrlMap;
	std::unordered_map<int, wxString> m_altMap;

private:
	void OnErase(wxEraseEvent& event);
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnTimer(wxTimerEvent& WXUNUSED(event));
	void OnIdle(wxIdleEvent& event);
	void OnMouseMove(wxMouseEvent& event);
	void OnMouseEnter(wxMouseEvent& event);
	void OnMouseLeave(wxMouseEvent& event);
	void OnMouseLBottonDown(wxMouseEvent& event);

	wxDECLARE_EVENT_TABLE();
	wxDECLARE_DYNAMIC_CLASS_NO_COPY(CNextMNewStatusBar);
};
#endif // NEXTMNEWSTATUSBAR_H_INCLUDED
