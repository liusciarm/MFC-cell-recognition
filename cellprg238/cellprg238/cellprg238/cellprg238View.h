
// cellprg238View.h: Ccellprg238View 类的接口
//

#pragma once

#include "cdib.h"
#include <complex>
using namespace std;
#include <vector>
#include "afxwin.h"

struct  RGB
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
};

struct HSI
{
	double	Hue;
	double	Saturation;
	double	Intensity;
};

typedef struct pyy {
	BYTE b;
	BYTE g;
	BYTE r;
} PIX_VALUE;

struct CENTER_POINT
{
	int x;
	int y;
	int radius;
};
class Ccellprg238View : public CView
{
protected: // 仅从序列化创建
	Ccellprg238View() noexcept;
	DECLARE_DYNCREATE(Ccellprg238View)

// 特性
public:
	Ccellprg238Doc* GetDocument() const;

// 操作
public:
	
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Ccellprg238View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	bool LoadBmp();
	bool dispaly(CDC* pDC, CPoint origin, CSize size);
	void RgbtoHsi(RGB* pRgb, HSI* pHsi);
	void ProcessFillHoles(int wd, int ht);
	void GenEdge(void);
	void GenEdge4(void);
	void MarkIt(int i, int j);
	void SaveIt(int i, int j, int radius);
	void CalcCenterArea(int i, int j);

	CBitmap m_bitmaplin;   //创建临时位图对象进行处理---  
	CString BmpNameLin;    //保存图像副本文件---  
	CBitmap m_bitmaplin1;   //创建临时位图对象进行处理---  
	CString BmpNameLin1;    //保存图像副本文件---  
	CPoint m_LastPoint;
	CPoint StartPoint;
	CPoint EndPoint;
	bool m_bDrag = false;
	bool noclick = true;
	int ProStep = 0;


	int m_nWidth;       //图像实际宽度  
	int m_nHeight;      //图像实际高度  
	int m_nDrawWidth;   //图像显示宽度  
	int m_nDrawHeight;  //图像显示高度  
	DWORD m_nImage;     //图像数据的字节数 只含位图  
	DWORD m_nSize;      //图像文件大小  
	int m_nLineByte;    //图像一行所占字节数  
	int m_nBitCount;    //图像每个像素所占位数  
	int m_nPalette;     //位图实际使用的颜色表中的颜色数  
	BYTE* m_pImage;         //读入图片数据后的指针  
	BITMAPFILEHEADER bfh;   //全局变量文件头  
	BITMAPINFOHEADER bih;   //全局变量信息头  
	RGBQUAD m_pPal;         //颜色表指针  
	bool fopen_flag=false;

	
	PIX_VALUE** pix;
	const int TWOVALUE_H = 0x80;	// =TWOVALUE_H: set 1
	const int EDGEPOINT = 0x70;//|= set 1, & edge
	LPVOID m_lpvColorTable;
	const int MARK_VISITED = 0x81;//& 
	const int MARKED = 0x80;//|=set & mark
	const int VISITED = 0x01;//|= set 1 ,& visited
	const int pre_shrink_count = 3;
	const int NO_MARK = 0x7f;//& set 0
	const int NO_EDGE_POINT = 0x8f;//&= noedge
	const int CENTERED = 0x2;//|= set 1 ,& center
	const int NO_CENTER = 0xfd;//&= nocenter
	const double Pi = 3.14159;
	const unsigned char TWOVALUE_L = 0x0;
	const int NO_VISITED = 0xfe;//&= novisited 
	bool m_bFullEdge;
	long tot_area, tot_x, tot_y, max_radius;		// 用于递归
	vector<CENTER_POINT>	points_temp;// 用于临时存储CENTER_POINT
	vector<CENTER_POINT>	m_vCenterPoints;

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCellprocMark();

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void Reload();
	afx_msg void OnCellprocFillholes();
//	afx_msg void OnUpdate32772(CCmdUI* pCmdUI);
	afx_msg void OnCellprocTwovalue();
	afx_msg void OnCellprocShrink();
	afx_msg void OnCellprocFindcenter();
	afx_msg void OnCellprocCount();
	afx_msg void OnUpdateCellprocTwovalue(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCellprocMark(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCellprocFillholes(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCellprocShrink(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCellprocFindcenter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCellprocCount(CCmdUI* pCmdUI);
	afx_msg void OnCellprocAllstep();
	afx_msg void OnUpdateCellprocAllstep(CCmdUI* pCmdUI);
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // cellprg238View.cpp 中的调试版本
inline Ccellprg238Doc* Ccellprg238View::GetDocument() const
   { return reinterpret_cast<Ccellprg238Doc*>(m_pDocument); }
#endif

