
// cellprg238View.cpp: Ccellprg238View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "cellprg238.h"
#endif

#include"cdib.h"
#include "stdafx.h"
#include "cellprg238Doc.h"
#include "cellprg238View.h"
#include "MainFrm.h"
#include <stack>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DISTANCE(x0,y0,x1,y1) sqrt((float)((x0-x1)*(x0-x1)+(y0-y1)*(y0-y1)))

// Ccellprg238View

IMPLEMENT_DYNCREATE(Ccellprg238View, CView)

BEGIN_MESSAGE_MAP(Ccellprg238View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Ccellprg238View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_CELLPROC_MARK, &Ccellprg238View::OnCellprocMark)

	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_32772, &Ccellprg238View::Reload)

	ON_COMMAND(ID_CELLPROC_FILLHOLES, &Ccellprg238View::OnCellprocFillholes)
	ON_COMMAND(ID_CELLPROC_TWOVALUE, &Ccellprg238View::OnCellprocTwovalue)
	ON_COMMAND(ID_CELLPROC_SHRINK, &Ccellprg238View::OnCellprocShrink)
	ON_COMMAND(ID_CELLPROC_FINDCENTER, &Ccellprg238View::OnCellprocFindcenter)
	ON_COMMAND(ID_CELLPROC_COUNT, &Ccellprg238View::OnCellprocCount)
	ON_UPDATE_COMMAND_UI(ID_CELLPROC_TWOVALUE, &Ccellprg238View::OnUpdateCellprocTwovalue)
	ON_UPDATE_COMMAND_UI(ID_CELLPROC_MARK, &Ccellprg238View::OnUpdateCellprocMark)
	ON_UPDATE_COMMAND_UI(ID_CELLPROC_FILLHOLES, &Ccellprg238View::OnUpdateCellprocFillholes)
	ON_UPDATE_COMMAND_UI(ID_CELLPROC_SHRINK, &Ccellprg238View::OnUpdateCellprocShrink)
	ON_UPDATE_COMMAND_UI(ID_CELLPROC_FINDCENTER, &Ccellprg238View::OnUpdateCellprocFindcenter)
	ON_UPDATE_COMMAND_UI(ID_CELLPROC_COUNT, &Ccellprg238View::OnUpdateCellprocCount)
	ON_COMMAND(ID_CELLPROC_ALLSTEP, &Ccellprg238View::OnCellprocAllstep)
	ON_UPDATE_COMMAND_UI(ID_CELLPROC_ALLSTEP, &Ccellprg238View::OnUpdateCellprocAllstep)
END_MESSAGE_MAP()

// Ccellprg238View 构造/析构

Ccellprg238View::Ccellprg238View() noexcept
{
	// TODO: 在此处添加构造代码

}

Ccellprg238View::~Ccellprg238View()
{
}

BOOL Ccellprg238View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Ccellprg238View 绘图

void Ccellprg238View::OnDraw(CDC* pDC)
{
	Ccellprg238Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CSize size;
	
	if (!fopen_flag)
	{
		LoadBmp();
		StartPoint.x = 0;
		EndPoint.x = m_nWidth - 1;
		StartPoint.y = 0;
		EndPoint.y = m_nHeight - 1;
		fopen_flag = true;
	}
	else
	{
		size.cx = m_nWidth; size.cy = m_nHeight;
		dispaly(pDC, CPoint(0, 0), size);
	}
	
	CRect ProcRect;
	ProcRect = CRect(StartPoint, EndPoint);
	CPen Mypen;
	Mypen.CreatePen(PS_DOT, 1, RGB(255, 0, 0));
	pDC->SelectObject(Mypen);
	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	pDC->Rectangle(ProcRect);
}


// Ccellprg238View 打印


bool Ccellprg238View ::LoadBmp()
{
	Ccellprg238Doc* pDoc = GetDocument();

	CFile file;
	if (file.Open(_T("Blood.bmp"), CFile::modeRead, NULL))
	{
		pDoc->m_pDib->Read(&file);
		file.Close();

	}

	m_nHeight = pDoc->m_pDib->m_lpBMIH->biHeight;
	m_nWidth = pDoc->m_pDib->m_lpBMIH->biWidth;
	m_nDrawHeight = pDoc->m_pDib->m_lpBMIH->biHeight;
	m_nDrawWidth = pDoc->m_pDib->m_lpBMIH->biWidth;
	m_nBitCount = pDoc->m_pDib->m_lpBMIH->biBitCount;   //每个像素所占位数  
									//计算图像每行像素所占的字节数（必须是32的倍数）  
	m_nLineByte = (m_nWidth * m_nBitCount + 31) / 32 * 4; //修改处
	
	m_nImage = m_nLineByte * m_nHeight;
	//位图实际使用的颜色表中的颜色数 biClrUsed  
	m_nPalette = 0;                       //初始化  
	if (pDoc->m_pDib->m_lpBMIH->biClrUsed)
		pDoc->m_pDib->m_lpBMIH->biClrUsed;
	//申请位图空间 大小为位图大小 m_nImage  
	//malloc只能申请4字节的空间 （未知） 
	/*m_pImage = (BYTE*)malloc(m_nImage);*/
	m_pImage=pDoc->m_pDib->m_lpImage;
	return true;
}
bool Ccellprg238View::dispaly(CDC* pDC, CPoint origin, CSize size)
{
	Ccellprg238Doc* pDoc = GetDocument();
	if (pDoc->m_pDib->m_lpBMIH == NULL)
		return FALSE;
	else 
	{
		size.cx = m_nWidth;
		size.cy = m_nHeight;
		pDoc->m_pDib->Draw(pDC, CPoint(0, 0), size);
	}
	return TRUE;
}

void Ccellprg238View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Ccellprg238View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Ccellprg238View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Ccellprg238View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void Ccellprg238View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Ccellprg238View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Ccellprg238View 诊断

#ifdef _DEBUG
void Ccellprg238View::AssertValid() const
{
	CView::AssertValid();
}

void Ccellprg238View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ccellprg238Doc* Ccellprg238View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ccellprg238Doc)));
	return (Ccellprg238Doc*)m_pDocument;
}
#endif //_DEBUG


// Ccellprg238View 消息处理程序


void Ccellprg238View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	/*CString str;
	str.Format(_T("x=%d,y=%d"), point.x, point.y);
	((CMainFrame*)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0, str);  //在左下角显示
	//AfxMessageBox(str);      //弹窗显示*/
	if (!m_bDrag) {
		m_LastPoint = point;
		m_bDrag = true;
	}
	CView::OnLButtonDown(nFlags, point);
}

void Ccellprg238View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bDrag = false;
	StartPoint.x = min(m_LastPoint.x, point.x);
	StartPoint.y = min(m_LastPoint.y, point.y);
	EndPoint.x = max(m_LastPoint.x, point.x);
	EndPoint.y = max(m_LastPoint.y, point.y);
	if ((EndPoint.x - StartPoint.x < 5)&& (EndPoint.y - StartPoint.y < 5))
	{
		StartPoint.x = 0;
		EndPoint.x = m_nWidth-1;
		StartPoint.y = 0;
		EndPoint.y = m_nHeight-1;
	}
	Invalidate(true);
	CView::OnLButtonUp(nFlags, point);
}


void Ccellprg238View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RGB rgb; HSI hsi; CString str; HDC hDC = ::GetDC(NULL);
	COLORREF val = ::GetPixel(hDC, point.x, point.y);
	int posin = m_nLineByte * (m_nHeight - 1 - point.y) + point.x * 3;
	unsigned char* lpSrc;
	//m_pImage[posin + 2], m_pImage[posin + 1], m_pImage[posin]
	if ((point.x > 0) && (point.x < m_nWidth) && (point.y > 0) && (point.y < m_nHeight))
	{

		if (m_nBitCount == 8) {
			lpSrc = m_pImage + m_nLineByte * (m_nHeight - 1 - point.y) + point.x;
			str.Format(TEXT("(x=%d y=%d)=%d"), point.x, point.y, *lpSrc);
		}


		if (m_nBitCount == 24)
		{
			rgb.b = m_pImage[posin];
			rgb.g = m_pImage[posin + 1];
			rgb.r = m_pImage[posin + 2];
			RgbtoHsi(&rgb, &hsi);
			int gray = (int)(0.114 * rgb.r + 0.587 * rgb.g + 0.299 * rgb.b);        //灰度
			str.Format(TEXT("Pos(%d %d) RGB(%d %d %d) Gray(%d) HSI(%4.1f %3.2f %3.2f--%d %d %d)"), point.x, point.y
				, rgb.r, rgb.g, rgb.b, gray, hsi.Hue, hsi.Saturation, hsi.Intensity,
				(int)(hsi.Hue * 255.0 / 360), (int)(hsi.Saturation * 255.0), (int)(hsi.Intensity * 255.0));
		}
		
		((CMainFrame*)AfxGetMainWnd())->m_wndStatusBar.SetPaneText(0, str);
	}
}

void Ccellprg238View::RgbtoHsi(RGB* pRgb, HSI* pHsi)
{
	// TODO: 在此处添加实现代码.
	double R, G, B;

	R = ((double)pRgb->r) / 255.0;
	G = ((double)pRgb->g) / 255.0;
	B = ((double)pRgb->b) / 255.0;

	
	int p;
	p = (pRgb->r < pRgb->g) ? pRgb->r : pRgb->g;
	p = (pRgb->b < p) ? pRgb->b : p;


	pHsi->Intensity = ((double)(pRgb->r + pRgb->g + pRgb->b)) / 255.0 / 3;
	pHsi->Saturation = 1 - 3.0 / (pRgb->r + pRgb->g + pRgb->b) * p;
	if (pRgb->g >= pRgb->b)
		pHsi->Hue = 180.0 / 3.14159 * acos(((double)(pRgb->r - pRgb->g) + (pRgb->r - pRgb->b)) / 2 / sqrt((double)(pRgb->r - pRgb->g) * (pRgb->r - pRgb->g) + (pRgb->r - pRgb->b) * (pRgb->g - pRgb->b)));
	else
		pHsi->Hue = 360 - 180.0 / 3.14159 * acos(((double)(pRgb->r - pRgb->g) + (pRgb->r - pRgb->b)) / 2 / sqrt((double)(pRgb->r - pRgb->g) * (pRgb->r - pRgb->g) + (pRgb->r - pRgb->b) * (pRgb->g - pRgb->b)));

	return;
}

void Ccellprg238View::OnCellprocMark()
{
	// TODO: 在此添加命令处理程序代码
	// 更改光标形状
	BeginWaitCursor();

	if (ProStep == 0)
	{
		unsigned char* lpSrc;

		// 指向转置图像对应象素的指针
		unsigned char* lpDst;

		// 指向转置图像的指针
		unsigned char* lpNewDIBBits;
		// 暂时分配内存，以保存新图像
		lpNewDIBBits = new unsigned char[m_nHeight * m_nLineByte];
		// 判断是否内存分配失败
		if (lpNewDIBBits == NULL)
		{
			// 分配内存失败
			return;
		}

		lpSrc = (unsigned char*)m_pImage;
		memcpy(lpNewDIBBits, lpSrc, m_nLineByte * m_nHeight);

		//不要取中心点，应该向远离背景方向偏移（小）
		double meanH = 210.0 * 360 / 255;//you select 
		double meanS = 55.0 / 255;//you select 
		double MarkDoor = 0.09;            //Mark门限距离
		double MayBeMarkDoor = 0.15;

		// 针对图像每行进行操作
		for (int i = StartPoint.y; i < EndPoint.y; i++)
		{
			// 针对每行图像每列进行操作
			for (int j = StartPoint.x; j < EndPoint.x; j++)
			{

				// 指向源DIB第i行，第j个象素的指针
				lpSrc = m_pImage + m_nLineByte * (m_nHeight - 1 - i) + j * 3;
				//if(*lpSrc<10) {
				//  *lpSrc=10;
				//}
				RGB rgb;
				HSI Hsi;
				rgb.b = *lpSrc; rgb.g = *(lpSrc + 1); rgb.r = *(lpSrc + 2);
				RgbtoHsi(&rgb, &Hsi);
				//get area (value get from cell area)



				double x1 = Hsi.Hue;//0—360
				double x2 = meanH;//近似
				if (x1 < 90) x1 += 360;  //

				//归一化
				double y1 = Hsi.Saturation;
				double y2 = meanS;
				x1 /= 180; x2 /= 180;//0--1         //180？
				//y1*=2;y2*=2;//0--2

				double dis = DISTANCE(x1, y1, x2, y2);
				if (dis < MarkDoor) {//Mark
					*lpSrc = 0; *(lpSrc + 1) = 0; *(lpSrc + 2) = 255;//Red
				}
				else if (dis < MayBeMarkDoor) {//may be Mark
					*lpSrc = 255; *(lpSrc + 1) = 0; *(lpSrc + 2) = 0;//Blue
				}
				//为了避开后面判断
				else {//not Mark/maybe Mark
					if (*lpSrc == 0) *lpSrc = 1;//Mark判断
					else if (*lpSrc == 255) *lpSrc = 254;//maybe mark判断
					else if (*(lpSrc + 2) == 128) *lpSrc = 127;
					if (*(lpSrc + 1) == 255) *(lpSrc + 1) = 254; //edge判断

				}
			}
		}

		Invalidate(true);
		MessageBox(TEXT("Mark(Red) & maybe Mark(Blue) "));

		//maybe Mark Point to Mark
		bool MarkChg = true;         //是否有需要改变的点
		while (MarkChg) {
			MarkChg = false;
			for (int i = StartPoint.y; i < EndPoint.y; i++)
			{
				// 针对每行图像每列进行操作
				for (int j = StartPoint.x; j < EndPoint.x; j++)
				{

					// 指向源DIB第i行，第j个象素的指针
					lpSrc = m_pImage + m_nLineByte * (m_nHeight - 1 - i) + j * 3;
					if (*lpSrc == 255) {//maybe Mark 
						bool bProc = false;
						if (j > 0) if (*(lpSrc - 3) == 0) bProc = true;
						if (j < m_nWidth - 1) if (*(lpSrc + 3) == 0) bProc = true;
						if (i > 0) if (*(lpSrc + m_nLineByte) == 0) bProc = true;
						if (i < m_nLineByte - 1) if (*(lpSrc - m_nLineByte) == 0) bProc = true;
						// maybe Mark have Mark Point to Mark 
						if (bProc) {
							*lpSrc = 0;
							MarkChg = true;
							*(lpSrc + 2) = 128;
						}
					}
				}
			}
		}

		Invalidate(true);
		MessageBox(TEXT("maybe Mark to Mark (Bright Red(128,0,0)"));

		const int edgeDoor = 45;
		//get edge information
		for (int i = StartPoint.y + 1; i < EndPoint.y - 1; i++) {//boarder no edge
			for (int j = StartPoint.x + 1; j < EndPoint.x - 1; j++) {
				lpDst = m_pImage + m_nLineByte * (m_nHeight - 1 - i) + j * 3;
				if (*(lpDst) == 0 || *(lpDst) == 255) {//Mark /Maybe Mark
					double pixel[9];
					lpSrc = lpNewDIBBits + m_nLineByte * (m_nHeight - 1 - i) + j * 3;
					for (int m = -1; m < 2; m++)                    //3*3矩阵
						for (int n = -1; n < 2; n++) {
							unsigned char* lpSrc1 = lpSrc - m_nLineByte * m + 3 * n;
							pixel[(m + 1) * 3 + n + 1] = ((int)*lpSrc1 + *(lpSrc1 + 1) + *(lpSrc1 + 2)) / 3;      //可修改
						}
					//Sobel
					double tmp1 =
						pixel[0] + 2 * pixel[1] + pixel[2] - pixel[6] - 2 * pixel[7] - pixel[8];
					double tmp2 =
						pixel[0] + 2 * pixel[3] + pixel[6] - pixel[2] - 2 * pixel[5] - pixel[8];
					double edge = sqrt(tmp1 * tmp1 + tmp2 * tmp2);
					if (edge > edgeDoor) {
						*(lpDst + 1) = 255;//edge


					}

				}
			}
		}

		const int M = 5;  //5*5窗口滤波
		bool bdelete;
		//filter 
		for (int i = StartPoint.y + M; i < EndPoint.y - M; i++)//
		{
			// 针对每行图像每列进行操作
			for (int j = StartPoint.x + M; j < EndPoint.x - M; j++)
			{

				// 指向源DIB第i行，第j个象素的指针
				lpDst = m_pImage + m_nLineByte * (m_nHeight - 1 - i) + j * 3;
				if (*(lpDst + 1) == 255)//edge 
				{
					bdelete = true;
					for (int m = -M; m <= M; m++)
						for (int n = -M; n <= M; n++)
						{
							if (m == -M || m == M || n == -M || n == M) {
								if (*(lpDst + m_nLineByte * m + n * 3) || (*(lpDst + m_nLineByte * m + n * 3 + 1) == 255))//noMark && no Edge
								{
									bdelete = false;
									m = M + 1; n = M + 1;//out
								}
							}
						}
					if (bdelete)
						*(lpDst + 1) = 0;//delete edge
				}
			}
		}



		delete[] lpNewDIBBits;
		Invalidate(true);
		// 恢复光标
		EndWaitCursor();
		ProStep = 1;
	}
}






void Ccellprg238View::Reload()
{
	
	// TODO: 在此添加命令处理程序代码
	Ccellprg238Doc* pDoc = GetDocument();
	LoadBmp();
	ProStep = 0;
	Invalidate(true);
}

void Ccellprg238View::OnCellprocTwovalue()
{
	// TODO: 在此添加命令处理程序代码
	Ccellprg238Doc* pDoc = GetDocument();
	if (ProStep == 1)
	{
		// TODO: 在此添加命令处理程序代码
		ProStep = 2;
		
		unsigned char* lpSrc;

		// 指向转置图像对应象素的指针
		unsigned char* lpDst;

		long int lNewLineBytes = (m_nWidth  + 3) / 4 * 4;  //保证每一行字节数都是4的倍数


		// 指向转置图像的指针
		unsigned char* lpNewDIBBits;
		// 暂时分配内存，以保存新图像
		lpNewDIBBits = new unsigned char[m_nHeight * lNewLineBytes];

		// 判断是否内存分配失败
		if (lpNewDIBBits == NULL)
		{
			// 分配内存失败
			return;
		}


		for (int i = 0; i < m_nHeight; i++) {
			for (int j = 0; j < m_nWidth; j++) {
				lpSrc = m_pImage + m_nLineByte * (m_nHeight - 1 - i) + j * 3;
				lpDst = lpNewDIBBits + lNewLineBytes * (m_nHeight - 1 - i) + j;
				unsigned char v;
				v = 0;
				if (*(lpSrc) == 0)//Mark
				{
					v = TWOVALUE_H;
					if (*(lpSrc + 1))
						v |= EDGEPOINT;//set edge
					/*else if (j == 0 || j == m_nWidth - 1 || i == 0 || i == m_nHeight - 1)
						v |= EDGEPOINT;//set edge*/
				}
				*lpDst = (unsigned char)v;
			}

		}
		m_nLineByte = lNewLineBytes;
		


		delete pDoc->m_pDib;
		pDoc->m_pDib = new CDib(CSize(lNewLineBytes, m_nHeight), 8);
		m_pImage = pDoc->m_pDib->m_lpImage;
		m_nHeight = pDoc->m_pDib->m_lpBMIH->biHeight;
		m_nWidth = pDoc->m_pDib->m_lpBMIH->biWidth;
		m_nBitCount = pDoc->m_pDib->m_lpBMIH->biBitCount;

		lpSrc = (unsigned char*)pDoc->m_pDib->m_lpvColorTable;
		for (int i = 0; i < 256; i++) {
			*lpSrc = (unsigned char)i; lpSrc++;
			*lpSrc = (unsigned char)i; lpSrc++;
			*lpSrc = (unsigned char)i; lpSrc++;
			*lpSrc = 0; lpSrc++;
		}

		memcpy(m_pImage,lpNewDIBBits, lNewLineBytes * m_nHeight);

		
		delete[]lpNewDIBBits;

		Invalidate(true);//彩色变二值
	}
	

}




void Ccellprg238View::OnCellprocFillholes()
{
	// TODO: 在此添加命令处理程序代码
	if (ProStep == 2)
	{
		// TODO: 在此添加命令处理程序代码
		ProStep = 3;
		//CImgProcDoc* pDoc = GetDocument();

		// 更改光标形状
		BeginWaitCursor();


		unsigned char* lpSrc;
		//0x7X---edge
		//0x8X---Mark--not edge
		//0xfX--Mark --edge
		//0xX1---visited
		for (int i = StartPoint.y + 1; i <= EndPoint.y - 1; i++)
		{
			lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - i) + StartPoint.x;
			for (int j = StartPoint.x + 1; j <= EndPoint.x - 1; j++)
			{
				lpSrc++;

				//if no-marked & no-visited
				if (!(*lpSrc & MARK_VISITED)) {//未访问过的黑点
					ProcessFillHoles(j, i);//line,col
				}

			}
		}
		//Invalidate(true);
		MessageBox(TEXT("FillHole"));

		//edge area back
		//0xfX--Mark --edge
		for (int i = StartPoint.y; i <= EndPoint.y; i++)
		{
			lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - i) + StartPoint.x - 1;
			for (int j = StartPoint.x; j <= EndPoint.x; j++)
			{
				lpSrc++;
				if (!(*lpSrc & MARKED))
					*lpSrc = 0;
				else //if marked & edge
					if (*lpSrc & EDGEPOINT)
						*lpSrc = 0;

			}
		}

		Invalidate(true);
		// 恢复光标
		EndWaitCursor();
	}
	
}

void Ccellprg238View::ProcessFillHoles(int wd, int ht)//填洞
{
	// TODO: 在此处添加实现代码.
	CPoint str;
	//CImgProcDoc* pDoc = GetDocument();
	stack<CPoint> s;
	vector<CPoint> v;//v save for fill holes

	const int MAX_HOLE = 100;

	int xt, yt;
	xt = wd;
	yt = ht;
	s.push(CPoint(xt, yt));
	v.push_back(CPoint(xt, yt));

	unsigned char* lpSrc;
	lpSrc = (unsigned char*)m_pImage +
		m_nLineByte * (m_nHeight - 1 - yt) + xt;
	*lpSrc |= VISITED;//vistied	

	bool bBorder = false;

	while (s.size())
	{
		//Add new members to stack
		//Above current pixel
		lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - yt) + xt;
		if (yt > StartPoint.y) {
			//if no-marked & no-visited
			if (!(*(lpSrc + m_nLineByte) & MARK_VISITED))
			{
				s.push(CPoint(xt, yt - 1));
				v.push_back(CPoint(xt, yt - 1));
				*(lpSrc + m_nLineByte) |= VISITED;
			}
		}
		else bBorder = true;
		//Below current pixel

		if (yt < EndPoint.y) {
			//if no-marked & no-visited
			if (!(*(lpSrc - m_nLineByte) & MARK_VISITED))
			{
				s.push(CPoint(xt, yt + 1));
				v.push_back(CPoint(xt, yt + 1));
				*(lpSrc - m_nLineByte) |= VISITED;
			}
		}
		else bBorder = true;
		//Left of current pixel
		if (xt > StartPoint.x) {
			//if no-marked & no-visited
			if (!(*(lpSrc - 1) & MARK_VISITED))
			{
				s.push(CPoint(xt - 1, yt));
				v.push_back(CPoint(xt - 1, yt));
				*(lpSrc - 1) |= VISITED;
			}
		}
		else bBorder = true;
		//Right of current pixel
		if (xt < EndPoint.x) {
			//if no-marked & no-visited
			if (!(*(lpSrc + 1) & MARK_VISITED))
			{
				s.push(CPoint(xt + 1, yt));
				v.push_back(CPoint(xt + 1, yt));
				*(lpSrc + 1) |= VISITED;
			}
		}
		else bBorder = true;
		//Retrieve current stack member
		xt = s.top().x;
		yt = s.top().y;
		s.pop();
	}
	if (v.size() < MAX_HOLE && !bBorder)
	{
		CString msg;
		msg.Format(_T("\n%d--(%d %d)"), v.size(), wd, ht);
		//for see	
		if (v.size() > 50) {
			Invalidate(true);
			MessageBox(msg);
		}
		else
			TRACE(msg);

		for (UINT k = 0; k < v.size(); k++) {
			xt = v[k].x;
			yt = v[k].y;
			lpSrc = m_pImage + m_nLineByte * (m_nHeight - 1 - yt) + xt;
			*lpSrc |= MARKED;
		}
	}

}






void Ccellprg238View::OnCellprocShrink()
{
	// TODO: 在此添加命令处理程序代码
	if (ProStep == 3)
	{
		// TODO: 在此添加命令处理程序代码
		ProStep = 4;
		//CImgProcDoc* pDoc = GetDocument();
		const int pre_shrink_count = 3;    //
		unsigned char* lpSrc;
		// 先去掉pre_shrink_count层皮
		GenEdge();
		for (int k = 0; k < pre_shrink_count; k++)
		{
			for (int i = StartPoint.y; i <= EndPoint.y; i++)
			{
				lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - i) + StartPoint.x - 1;
				for (int j = StartPoint.x; j <= EndPoint.x; j++)
				{
					lpSrc++;

					// 去掉边界!
					if (*lpSrc & EDGEPOINT)
						(*lpSrc) &= NO_MARK;//marked=0;
				}
			}
			if (k %2== 0)
				GenEdge();
			else
				GenEdge4();

		}
		Invalidate(true);
	}
	
}
void Ccellprg238View::GenEdge(void)
{
	// TODO: 在此处添加实现代码.

	unsigned char* lpSrc;


	for (int j = StartPoint.y; j <= EndPoint.y; j++)
	{
		lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
		for (int i = StartPoint.x; i <= EndPoint.x; i++)
		{
			lpSrc++;
			*lpSrc &= NO_EDGE_POINT;//no edge
			if (*lpSrc & MARKED)//marked
			{
				if (j == StartPoint.y || i == StartPoint.x || j == EndPoint.y || i == EndPoint.x)	// 
				{
					*lpSrc |= EDGEPOINT;
				}

				else if              // normal

					(
						!((*(lpSrc - m_nLineByte - 1) & MARKED)
							&& (*(lpSrc - m_nLineByte) & MARKED)
							&& (*(lpSrc - m_nLineByte + 1) & MARKED)
							&& (*(lpSrc - 1) & MARKED)
							&& (*(lpSrc + 1) & MARKED)
							&& (*(lpSrc + m_nLineByte - 1) & MARKED)
							&& (*(lpSrc + m_nLineByte) & MARKED)
							&& (*(lpSrc + m_nLineByte + 1) & MARKED)
							)
						)* lpSrc |= EDGEPOINT;
			}

		}
	}
}

void Ccellprg238View::GenEdge4(void)
{
	// TODO: 在此处添加实现代码.
	//CImgProcDoc* pDoc = GetDocument();
	unsigned char* lpSrc;


	for (int j = StartPoint.y; j <= EndPoint.y; j++)
	{
		lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
		for (int i = StartPoint.x; i <= EndPoint.x; i++)
		{
			lpSrc++;
			*lpSrc &= NO_EDGE_POINT;//no edge
			if (*lpSrc & MARKED)//marked
			{
				if (j == StartPoint.y || i == StartPoint.x || j == EndPoint.y || i == EndPoint.x)	// 
				{
					*lpSrc |= EDGEPOINT;
				}

				else if              // normal
					(
						!((*(lpSrc - m_nLineByte) & MARKED)
							&& (*(lpSrc - 1) & MARKED)
							&& (*(lpSrc + 1) & MARKED)
							&& (*(lpSrc + m_nLineByte) & MARKED)
							)
						) *lpSrc |= EDGEPOINT;
			}

		}
	}
}


void Ccellprg238View::OnCellprocFindcenter()
{
	// TODO: 在此添加命令处理程序代码
	if (ProStep == 4)
	{
		// TODO: 在此添加命令处理程序代码
		ProStep = 5;
		//CImgProcDoc* pDoc = GetDocument();

		CENTER_POINT pt;

		points_temp.clear();
		bool changed;
		unsigned char* lpSrc;
		BeginWaitCursor();

		for (int k = 0; k < 100; k++) // 标志中心点的腐蚀	
		{
			changed = false;
			// 清除visited标志
			for (int j = StartPoint.y; j <= EndPoint.y; j++)
			{
				lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
				for (int i = StartPoint.x; i <= EndPoint.x; i++)
				{
					lpSrc++;
					*lpSrc &= NO_VISITED;//
				}
			}

			for (int j = StartPoint.y; j <= EndPoint.y; j++)
			{
				lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
				for (int i = StartPoint.x; i <= EndPoint.x; i++)
				{
					lpSrc++;
					{
						if (j > StartPoint.y && j<EndPoint.y && i>StartPoint.x && i < EndPoint.x) // 最边上的不用处理
						{
							m_bFullEdge = true;
							if (*lpSrc & EDGEPOINT && !(*lpSrc & VISITED))	// 没有访问过的边界
							{
								if (!(*(lpSrc - 1) & MARKED) &&
									!(*(lpSrc + 1) & MARKED) &&
									!(*(lpSrc + m_nLineByte) & MARKED) &&
									!(*(lpSrc - m_nLineByte) & MARKED))
								{
									if (k <= 2) // 基本上这种是噪音
									{
										continue;
									}
									// 孤立的点

									*lpSrc |= CENTERED;
									// 保存一下CENTER_POINT信息
									pt.x = i;
									pt.y = j;
									pt.radius = k + pre_shrink_count + 4;//circle adjust 
									points_temp.push_back(pt);

									continue;
								}
								else
									MarkIt(i, j); // 判断是否需要保存
							// 没有访问过 标志了 并且是非边缘邻域
							// 需要保存!
								if (m_bFullEdge)
									SaveIt(i, j, k + pre_shrink_count + 4);//保存
							}
						}
					}
				}
			}
			for (int j = StartPoint.y; j <= EndPoint.y; j++)
			{
				lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
				for (int i = StartPoint.x; i <= EndPoint.x; i++)
				{
					lpSrc++;
					// 去掉边界!
					if (*lpSrc & EDGEPOINT)
					{
						changed = true;
						*lpSrc &= NO_MARK;
					}
				}
			}
			if (k % 2 == 0)
				GenEdge4();
			else
				GenEdge();

			if (!changed)
				break;


		}

		CString msg;
		msg.Format(TEXT("获得的中心点数目= %d"), points_temp.size());
		MessageBox(msg);

		// 取平均值,获得中心点
		vector<CENTER_POINT>	points;
		for (int j = StartPoint.y; j <= EndPoint.y; j++)
		{
			lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
			for (int i = StartPoint.x; i <= EndPoint.x; i++)
			{
				lpSrc++;
				if (j > StartPoint.y && j<EndPoint.y && i>StartPoint.x && i < EndPoint.x) // 最边上的不用处理
					if (*lpSrc & CENTERED)
					{
						if (!(*(lpSrc - 1) & CENTERED) && !(*(lpSrc + 1) & CENTERED) &&
							!(*(lpSrc + m_nLineByte) & CENTERED) &&
							!(*(lpSrc - m_nLineByte) & CENTERED) &&
							!(*(lpSrc + m_nLineByte - 1) & CENTERED) &&
							!(*(lpSrc + m_nLineByte + 1) & CENTERED) &&
							!(*(lpSrc - m_nLineByte - 1) & CENTERED) &&
							!(*(lpSrc - m_nLineByte + 1) & CENTERED))
						{
							// 孤立的点
							pt.x = i;
							pt.y = j;
							for (unsigned int n = 0; n < points_temp.size(); n++)
							{
								if (points_temp.at(n).x == i && points_temp.at(n).y == j)
								{
									pt.radius = points_temp.at(n).radius;
									break;
								}
							}
							points.push_back(pt);
							continue;
						}
						else
						{                        //多个点求其质点
							tot_area = 0;
							max_radius = 0;
							tot_x = 0;
							tot_y = 0;
							CalcCenterArea(i, j);         //计算其邻域（相连）中心点的质点与最大半径并去除其中心点标志
							pt.x = tot_x / tot_area;
							pt.y = tot_y / tot_area;
							pt.radius = max_radius;
							*(lpSrc - (pt.y - j) * m_nLineByte + pt.x - i) |= CENTERED;
							points.push_back(pt);
						}
					}
			}

		}

		msg.Format(TEXT("取平均值,获得中心点数目= %d"), points.size());
		MessageBox(msg);

		// draw to display points
		CDC* pdc = GetDC();
		CPen	Redpen;
		Redpen.CreatePen(PS_DOT, 1, RGB(255, 0, 0));
		CPen	Redpen1;
		Redpen1.CreatePen(PS_DOT, 3, RGB(255, 0, 0));
		CPen Greenpen;
		Greenpen.CreatePen(PS_DOT, 1, RGB(0, 255, 0));
		CPen Bluepen;
		Bluepen.CreatePen(PS_DOT, 1, RGB(0, 0, 255));
		CPen Bluepen1;
		Bluepen1.CreatePen(PS_DOT, 3, RGB(0, 0, 255));

		m_vCenterPoints.clear();
		int x0, y0;
		bool adj;
		// 清除center标志
		for (int j = StartPoint.y; j <= EndPoint.y; j++)
		{
			lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + StartPoint.x - 1;
			for (int i = StartPoint.x; i <= EndPoint.x; i++)
			{
				lpSrc++;
				*lpSrc &= NO_CENTER;
			}
		}
		// 平均化相近的中心点

		for (unsigned int i = 0; i < points.size(); i++)
		{
			x0 = points.at(i).x;
			y0 = points.at(i).y;
			pt = points.at(i);

			adj = false;

			//Red 相近- delete
			pdc->SelectObject(Redpen);
			for (unsigned int j = i + 1; j < points.size() - 1; j++)
			{
				int x = points.at(j).x;
				int y = points.at(j).y;
				if (abs(x0 - x) + abs(y0 - y) < 10) // 相近 //圆心距离
				{
					points.at(i).x = (x + x0) / 2;
					points.at(i).y = (y + y0) / 2;
					points.at(i).radius = (points.at(i).radius + points.at(j).radius) / 2;

					pt = points.at(j);
					//display err position--delete 
					Arc(pdc->m_hDC,//-3 for display 
						pt.x - pt.radius + 3,
						pt.y - pt.radius + 3,
						pt.x + pt.radius - 3,
						pt.y + pt.radius - 3,
						pt.x + pt.radius - 3,
						pt.y - 3,
						pt.x + pt.radius - 3,
						pt.y - 3
					);

					points.erase(points.begin() + j);//&points.at(j));
					i--;
					adj = true;
					break;
				}
			}
			if (!adj) // 非相近
			{
				if (points.at(i).radius > 4)
				{
					m_vCenterPoints.push_back(points.at(i));
					*(m_pImage + m_nLineByte * (m_nHeight - 1 - points.at(i).y) + points.at(i).x) |= CENTERED;
					if (i % 5 == 0 && i)	TRACE("\n");
					TRACE("%3d:(%3d %3d)--%2d\t", i, points.at(i).x, points.at(i).y, points.at(i).radius);
					//display position
					pdc->SelectObject(Greenpen);

					Arc(pdc->m_hDC,
						pt.x - pt.radius,
						pt.y - pt.radius,
						pt.x + pt.radius,
						pt.y + pt.radius,
						pt.x + pt.radius,
						pt.y,
						pt.x + pt.radius,
						pt.y
					);

				}
			}
		}
		msg.Format(TEXT("平均化相近的中心点后数目= %d"), m_vCenterPoints.size());
		MessageBox(msg);

		int r0, r;
		int tx, ty;
		// 去掉被包含的圆
		//Blue 相近- delete
		bool bdelete = false;
		pdc->SelectObject(Bluepen1);
		for (unsigned int i = 0; i < m_vCenterPoints.size(); i++)
		{
			x0 = m_vCenterPoints.at(i).x;
			y0 = m_vCenterPoints.at(i).y;
			r0 = m_vCenterPoints.at(i).radius;
			for (unsigned int j = i + 1; j < m_vCenterPoints.size() - 1; j++)
			{
				int x = m_vCenterPoints.at(j).x;
				int y = m_vCenterPoints.at(j).y;
				r = m_vCenterPoints.at(j).radius;
				if (DISTANCE(x0, y0, x, y) < abs(r0 - r) + 1) // 包含
				{
					bdelete = true;
					if (r0 > r) // 去掉r0
						pt = m_vCenterPoints.at(i);
					else
						pt = m_vCenterPoints.at(j);
					Arc(pdc->m_hDC,
						pt.x - pt.radius,
						pt.y - pt.radius,
						pt.x + pt.radius,
						pt.y + pt.radius,
						pt.x + pt.radius,
						pt.y,
						pt.x + pt.radius,
						pt.y
					);
					if (r0 > r) // 去掉r0
					{
						m_vCenterPoints.erase(m_vCenterPoints.begin() + i);//(&m_vCenterPoints.at(i));
						i--;
					}
					else
						m_vCenterPoints.erase(m_vCenterPoints.begin() + j);//(&m_vCenterPoints.at(j));
				}
			}
		}
		if (bdelete) {
			msg.Format(TEXT("去掉被包含的圆(Blue )后数目= %d"), m_vCenterPoints.size());
			MessageBox(msg);
		}

		vector<CENTER_POINT> tocheck;
		int total;
		bool isok;

		// 去掉潜在的错误(圆 r<9)
		pdc->SelectObject(Redpen1);
		bdelete = false;
		for (unsigned int i = 0; i < m_vCenterPoints.size(); i++)
		{	//baord area process
			CENTER_POINT centerp;
			centerp = m_vCenterPoints.at(i);
			if (centerp.x - centerp.radius < 0)
				centerp.radius -= (centerp.x - centerp.radius);
			if (centerp.y - centerp.radius < 0)
				centerp.radius -= (centerp.y - centerp.radius);
			if (centerp.x + centerp.radius > m_nWidth - 1)
				centerp.radius += (centerp.x + centerp.radius - m_nWidth);
			if (centerp.y + centerp.radius > m_nHeight - 1)
				centerp.radius += (centerp.y + centerp.radius - m_nHeight);

			if (m_vCenterPoints.at(i).radius < 8) // need adjust <
			{

				Arc(pdc->m_hDC,
					centerp.x - centerp.radius,
					centerp.y - centerp.radius,
					centerp.x + centerp.radius,
					centerp.y + centerp.radius,
					centerp.x + centerp.radius,
					centerp.y,
					centerp.x + centerp.radius,
					centerp.y
				);

				m_vCenterPoints.erase(m_vCenterPoints.begin() + i);//(&m_vCenterPoints.at(i));
				i--;
				bdelete = true;
			}
		}
		if (bdelete) {
			msg.Format(TEXT("去掉潜在的错误(圆 r<8 Redpen)后数目=%d"), m_vCenterPoints.size());
			MessageBox(msg);
		}
		bdelete = false;
		pdc->SelectObject(Bluepen1);
		// 去掉潜在的错误(同两个圆相交,并且不相交的部分是噪声)
		for (unsigned int i = 0; i < m_vCenterPoints.size(); i++)
		{
			tocheck.clear();
			x0 = m_vCenterPoints.at(i).x;
			y0 = m_vCenterPoints.at(i).y;
			r0 = m_vCenterPoints.at(i).radius;
			for (unsigned int j = 0; j < m_vCenterPoints.size(); j++)
			{
				if (i == j)
					continue;
				int x = m_vCenterPoints.at(j).x;
				int y = m_vCenterPoints.at(j).y;
				r = m_vCenterPoints.at(j).radius;
				if (DISTANCE(x0, y0, x, y) < abs(r0 + r)) // 相交
				{
					pt.x = x; pt.y = y; pt.radius = r;
					tocheck.push_back(pt);
				}
			}
			unsigned int size = (unsigned int)tocheck.size();
			if (size > 0) // 同两个以上的圆相交
			{
				total = 0;
				for (tx = x0 - r0; tx < x0 + r0; tx++)
					for (ty = y0 - r0; ty < y0 + r0; ty++)
					{
						if (DISTANCE(x0, y0, tx, ty) < r0) // 所有圆内部的点
						{
							if (tx<StartPoint.x || tx>EndPoint.x || ty<StartPoint.y || ty>EndPoint.y)
								continue;
							isok = true;
							for (unsigned int n = 0; n < size; n++)
							{
								pt = tocheck.at(n);// 取得
								if (DISTANCE(tx, ty, pt.x, pt.y) < pt.radius)
								{
									isok = false;
									break;
								}
							}
							if (isok) // 同所有的圆都不相交的部分
							{
								total++;
							}
						}
					}
				if (total < Pi * r0 * r0 * .5) // need adjust 50%
				{
					CENTER_POINT centerp;
					centerp = m_vCenterPoints.at(i);
					Arc(pdc->m_hDC,
						centerp.x - centerp.radius,
						centerp.y - centerp.radius,
						centerp.x + centerp.radius,
						centerp.y + centerp.radius,
						centerp.x + centerp.radius,
						centerp.y,
						centerp.x + centerp.radius,
						centerp.y
					);
					m_vCenterPoints.erase(m_vCenterPoints.begin() + i);//(&m_vCenterPoints.at(i));
					i--; bdelete = true;
				}
			}
		}

		if (bdelete) {
			msg.Format(TEXT("去掉潜在的错误(同两个圆相交,并且不相交的部分是噪声)Bluepen后数目=%d"), m_vCenterPoints.size());
			MessageBox(msg);
		}

		DeleteObject(Redpen);
		DeleteObject(Greenpen);
		DeleteObject(Bluepen);


		// 恢复光标
		EndWaitCursor();
	}
	
}


void Ccellprg238View::MarkIt(int i, int j)
{
	// TODO: 在此处添加实现代码.
	//CImgProcDoc* pDoc = GetDocument();
	unsigned char* lpSrc;

	lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + i;
	*(lpSrc) |= VISITED;

	//if (!m_bFullEdge)  //加入：1）速度慢 2）丢细胞
	//	return;

	if (j == StartPoint.y || j == EndPoint.y || i == StartPoint.x || i == EndPoint.x) // 最边上的不用处理
	{
		return;
	}
	if (!(*(lpSrc - 1) & VISITED) &&	// 没有访问过
		*(lpSrc - 1) & MARKED)		// 标志了
	{
		if (*(lpSrc - 1) & EDGEPOINT)		// 并且是边缘
			MarkIt(i - 1, j); // 左边
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc + 1) & VISITED) &&	// 没有访问过
		*(lpSrc + 1) & MARKED)		// 标志了
	{
		if (*(lpSrc + 1) & EDGEPOINT)		// 并且是边缘
			MarkIt(i + 1, j); // 右边
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc + m_nLineByte) & VISITED) &&	// 没有访问过
		*(lpSrc + m_nLineByte) & MARKED)	// 标志了
	{
		if (*(lpSrc + m_nLineByte) & EDGEPOINT)		// 并且是边缘
			MarkIt(i, j - 1); // 上面
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc - m_nLineByte) & VISITED) &&	// 没有访问过
		*(lpSrc - m_nLineByte) & MARKED)	// 标志了
	{
		if (*(lpSrc - m_nLineByte) & EDGEPOINT)		// 并且是边缘
			MarkIt(i, j + 1); // 下面
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc + m_nLineByte - 1) & VISITED) &&	// 没有访问过
		*(lpSrc + m_nLineByte - 1) & MARKED)		// 标志了
	{
		if (*(lpSrc + m_nLineByte - 1) & EDGEPOINT)		// 并且是边缘
			MarkIt(i - 1, j - 1); // 左上
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc - m_nLineByte - 1) & VISITED) &&	// 没有访问过
		*(lpSrc - m_nLineByte - 1) & MARKED)		// 标志了
	{
		if (*(lpSrc - m_nLineByte - 1) & EDGEPOINT)		// 并且是边缘
			MarkIt(i - 1, j + 1); // 左下
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc + m_nLineByte + 1) & VISITED) &&	// 没有访问过
		*(lpSrc + m_nLineByte + 1) & MARKED)		// 标志了
	{
		if (*(lpSrc + m_nLineByte + 1) & EDGEPOINT)		// 并且是边缘
			MarkIt(i + 1, j - 1); // 右上
		else
			m_bFullEdge = false;
	}

	if (!(*(lpSrc - m_nLineByte + 1) & VISITED) &&	// 没有访问过
		*(lpSrc - m_nLineByte + 1) & MARKED)		// 标志了
	{
		if (*(lpSrc - m_nLineByte + 1) & EDGEPOINT)		// 并且是边缘
			MarkIt(i + 1, j + 1); // 右下
		else
			m_bFullEdge = false;
	}
}


void Ccellprg238View::SaveIt(int i, int j, int radius)
{
	// TODO: 在此处添加实现代码.
	unsigned char* lpSrc;
	lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + i;
	if (j == StartPoint.y || j == EndPoint.y || i == StartPoint.x || i == EndPoint.x) // 最边上的不用处理
		return;
	if (!(*lpSrc & CENTERED)) {
		CENTER_POINT pt;
		pt.x = i;
		pt.y = j;
		pt.radius = radius;
		points_temp.push_back(pt);

		// marke current point
		*lpSrc |= CENTERED;
	}

	*lpSrc &= NO_VISITED;//访问过才进栈，去掉则多次进栈

	if (*(lpSrc - 1) & VISITED)
	{
		SaveIt(i - 1, j, radius);
	}
	if (*(lpSrc + 1) & VISITED)
	{
		SaveIt(i + 1, j, radius);
	}
	if (*(lpSrc + m_nLineByte) & VISITED)
	{
		SaveIt(i, j - 1, radius);
	}
	if (*(lpSrc - m_nLineByte) & VISITED)
	{
		SaveIt(i, j + 1, radius);
	}

	if (*(lpSrc - m_nLineByte + 1) & VISITED)
	{
		SaveIt(i + 1, j + 1, radius);
	}
	if (*(lpSrc + m_nLineByte - 1) & VISITED)
	{
		SaveIt(i - 1, j + 1, radius);
	}
	if (*(lpSrc - m_nLineByte + 1) & VISITED)
	{
		SaveIt(i + 1, j - 1, radius);
	}
	if (*(lpSrc - m_nLineByte - 1) & VISITED)
	{
		SaveIt(i - 1, j - 1, radius);
	}
}

void Ccellprg238View::CalcCenterArea(int i, int j)
{
	// TODO: 在此处添加实现代码.
	unsigned char* lpSrc;
	lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - j) + i;
	if (j == StartPoint.y || j == EndPoint.y || i == StartPoint.x || i == EndPoint.x) // 最边上的不用处理
	{
		return;
	}
	tot_area++;
	tot_x += i;
	tot_y += j;
	*lpSrc &= NO_CENTER;

	for (unsigned int n = 0; n < points_temp.size(); n++)
	{
		if (points_temp.at(n).x == i && points_temp.at(n).y == j)
		{
			if (points_temp.at(n).radius > max_radius)
				max_radius = points_temp.at(n).radius;
			break;
		}
	}

	if (*(lpSrc - 1) & CENTERED)
	{
		CalcCenterArea(i - 1, j);
	}
	if (*(lpSrc + 1) & CENTERED)
	{
		CalcCenterArea(i + 1, j);
	}
	if (*(lpSrc + m_nLineByte) & CENTERED)
	{
		CalcCenterArea(i, j - 1);
	}
	if (*(lpSrc - m_nLineByte) & CENTERED)
	{
		CalcCenterArea(i, j + 1);
	}

	if (*(lpSrc - m_nLineByte + 1) & CENTERED)
	{
		CalcCenterArea(i + 1, j + 1);
	}
	if (*(lpSrc - m_nLineByte - 1) & CENTERED)
	{
		CalcCenterArea(i - 1, j + 1);
	}
	if (*(lpSrc + m_nLineByte + 1) & CENTERED)
	{
		CalcCenterArea(i + 1, j - 1);
	}
	if (*(lpSrc + m_nLineByte - 1) & CENTERED)
	{
		CalcCenterArea(i - 1, j - 1);
	}
}

void Ccellprg238View::OnCellprocCount()
{
	// TODO: 在此添加命令处理程序代码
	if (ProStep == 5)
	{
		// TODO: 在此添加命令处理程序代码
		ProStep = 0;
		//CImgProcDoc* pDoc = GetDocument();
		// reopen color image
		//OnFileReload();
		LoadBmp();
		double tota, totr;
		tota = 0; totr = 0;
		double m_nHistHSI[256 * 3];
		unsigned char* lpSrc;
		for (int i = 0; i < 256 * 3; i++)
		{
			// 清零
			m_nHistHSI[i] = 0;
		}


		for (unsigned int i = 0; i < m_vCenterPoints.size(); i++)
		{
			tota += m_vCenterPoints.at(i).radius * m_vCenterPoints.at(i).radius * 3.14f;
			totr += m_vCenterPoints.at(i).radius;

			//get Hsi 	
			for (int m = m_vCenterPoints.at(i).x - m_vCenterPoints.at(i).radius; m < m_vCenterPoints.at(i).x + m_vCenterPoints.at(i).radius; m++)
				for (int n = m_vCenterPoints.at(i).y - m_vCenterPoints.at(i).radius; n < m_vCenterPoints.at(i).y + m_vCenterPoints.at(i).radius; n++)
				{
					if (m >= StartPoint.x && m <= EndPoint.x && n >= StartPoint.y && n <= EndPoint.y)
					{
						if (DISTANCE(m, n, m_vCenterPoints.at(i).x, m_vCenterPoints.at(i).y) <= m_vCenterPoints.at(i).radius)
						{
							lpSrc = (unsigned char*)m_pImage + m_nLineByte * (m_nHeight - 1 - n) + m * 3;
							RGB Rgb;
							HSI Hsi;
							Rgb.b = *lpSrc; Rgb.g = *(lpSrc + 1); Rgb.r = *(lpSrc + 2);
							RgbtoHsi(&Rgb, &Hsi);
							unsigned int H, S, I;
							H = (unsigned int)(Hsi.Hue / 360.0 * 255.0);
							S = (unsigned int)(Hsi.Saturation * 255.0);
							I = (unsigned int)(Hsi.Intensity * 255.0);

							// 计数加1
							m_nHistHSI[H]++;//H
							m_nHistHSI[256 + S]++;//S
							m_nHistHSI[256 * 2 + I]++;//I
						}
					}
				}

		}
		int min[3];
		int max[3];
		min[0] = min[1] = min[2] = 255;
		max[0] = max[1] = max[2] = 0;
		double add;
		for (int j = 0; j < 3; j++)
		{
			add = 0;
			for (int i = 0; i < 256; i++)
			{
				// 清零
				add += m_nHistHSI[j * 256 + i];
				if (add > tota / 20) {//>5%
					min[j] = i;
					i = 256;//out
				}

			}
		}
		for (int j = 0; j < 3; j++)
		{
			add = 0;
			for (int i = 255; i > 0; i--)
			{
				// 清零
				add += m_nHistHSI[j * 255 + i];
				if (add > tota / 20) {//>5%
					max[j] = i;
					i = 0;//out
				}

			}
		}
		Invalidate(true);
		MessageBox(TEXT("ReLoad Image"));


		CString msg;
		
		/*
	CMainFrame* pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus = &pFrame->m_wndStatusBar;
	if (pStatus)
		pStatus->SetPaneText(0, msg);
		*/
		//归一化
		for (int j = 0; j < 3; j++)
		{
			double max = 0;
			for (int i = 0; i < 256; i++)
				if (max < m_nHistHSI[j * 256 + i])
					max = m_nHistHSI[j * 256 + i];
			for (int i = 0; i < 256; i++)
				m_nHistHSI[j * 256 + i] /= max;
		}
		CDC* pdc = GetDC();
		CPen	Redpen;
		Redpen.CreatePen(PS_DOT, 1, RGB(255, 0, 0));

		pdc->SelectObject(Redpen);
		for (unsigned int i = 0; i < m_vCenterPoints.size(); i++) {
			Arc(pdc->m_hDC,
				m_vCenterPoints.at(i).x - m_vCenterPoints.at(i).radius,
				m_vCenterPoints.at(i).y - m_vCenterPoints.at(i).radius,
				m_vCenterPoints.at(i).x + m_vCenterPoints.at(i).radius,
				m_vCenterPoints.at(i).y + m_vCenterPoints.at(i).radius,
				m_vCenterPoints.at(i).x + m_vCenterPoints.at(i).radius,
				m_vCenterPoints.at(i).y,
				m_vCenterPoints.at(i).x + m_vCenterPoints.at(i).radius,
				m_vCenterPoints.at(i).y
			);
		}
		msg.Format(L"共有%d个细胞,平均半径%d,平均面积%d  : H(%3.1f,%3.1f) S(%3.2f,%3.2f) I(%3.2f,%3.2f)",
			m_vCenterPoints.size(),
			(int)(totr / m_vCenterPoints.size() + .5),
			(int)(tota / m_vCenterPoints.size() + .5),
			360.0 * min[0] / 255.0, 360.0 * max[0] / 255.0,
			1.0 * min[1] / 255.0, 1.0 * max[1] / 255.0,
			1.0 * min[2] / 255.0, 1.0 * max[2] / 255.0);

		MessageBox(msg);
		DeleteObject(Redpen);
	}
	
}

void Ccellprg238View::OnCellprocAllstep()
{
	// TODO: 在此添加命令处理程序代码
	MessageBox(TEXT("Step1:Mark"));
	OnCellprocMark();
	MessageBox(TEXT("Step2:TwoValue"));
	OnCellprocTwovalue();
	MessageBox(TEXT("Step3:Fillholes"));
	OnCellprocFillholes();
	MessageBox(TEXT("Step4:Shrink()"));
	OnCellprocShrink();
	MessageBox(TEXT("Step5:Findcenter"));
	OnCellprocFindcenter();
	MessageBox(TEXT("Step6:Count"));
	OnCellprocCount();
}

void Ccellprg238View::OnUpdateCellprocMark(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (ProStep == 0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void Ccellprg238View::OnUpdateCellprocTwovalue(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(ProStep==1)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void Ccellprg238View::OnUpdateCellprocFillholes(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (ProStep == 2)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void Ccellprg238View::OnUpdateCellprocShrink(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (ProStep == 3)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void Ccellprg238View::OnUpdateCellprocFindcenter(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (ProStep == 4)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void Ccellprg238View::OnUpdateCellprocCount(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (ProStep == 5)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}




void Ccellprg238View::OnUpdateCellprocAllstep(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (ProStep == 0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}


void Ccellprg238View::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	// TODO: 在此添加专用代码和/或调用基类
	/*Ccellprg238Doc* pDoc = GetDocument();
	
	CFile file;
	if (file.Open(_T("Blood.bmp"), CFile::modeRead, NULL));
	{
		pDoc->m_pDib->Read(&file);
		file.Close();

	}*/


	
	
	
}
