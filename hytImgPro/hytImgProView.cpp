// hytImgProView.cpp : implementation of the CHytImgProView class
//

#include "stdafx.h"
#include "hytImgPro.h"

#include "hytImgProDoc.h"
#include "hytImgProView.h"
#include "MainFrm.h"
#include "DlgGrayStre.h"
#include "DlgPowerTrans.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHytImgProView

IMPLEMENT_DYNCREATE(CHytImgProView, CView)

BEGIN_MESSAGE_MAP(CHytImgProView, CView)
	//{{AFX_MSG_MAP(CHytImgProView)
	ON_COMMAND(ID_MENU_CONVERT_GRAY, OnMenuConvertGray)
	ON_COMMAND(ID_BUTTON_RESET, OnButtonReset)
	ON_COMMAND(ID_MENU_OPPOSITE, OnMenuOpposite)
	ON_COMMAND(ID_MENU_GRAY_STRECTHING, OnMenuGrayStrecthing)
	ON_COMMAND(ID_MENU_POWER_TRANS, OnMenuPowerTrans)
	ON_COMMAND(ID_MENU_HISTOGRAM_PRO, OnMenuHistogramPro)
	ON_COMMAND(ID_MENU_AVER_FILTER, OnMenuAverFilter)
	ON_COMMAND(ID_MENU_MIDDLE_FILTER, OnMenuMiddleFilter)
	ON_COMMAND(ID_MENU_LAPLACE, OnMenuLaplace)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHytImgProView construction/destruction

CHytImgProView::CHytImgProView()
{
	

}

CHytImgProView::~CHytImgProView()
{

}

BOOL CHytImgProView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CHytImgProView drawing

void CHytImgProView::OnDraw(CDC* pDC)
{
	CHytImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_hDIB==NULL)
		return;
	
// 	if (pDoc->m_hProDIB==NULL)
// 	{
// 		// ΪDIB�����ڴ�
// 		pDoc->m_hProDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT, pDoc->m_FileSize);
// 		
// 		if (pDoc->m_hProDIB == 0)
// 		{
// 			// �ڴ����ʧ�ܣ�����NULL��
// 			return;
// 		}
// 		pDoc->m_hProDIB=(HDIB)CopyHandle((HANDLE) pDoc->GetHDIB());
// 	}
	
	
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);
		
	int cxDIB = (int) ::DIBWidth(lpDIB);
		
	int cyDIB = (int) ::DIBHeight(lpDIB);
		
	::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
		
	CRect rcDIB;
	rcDIB.top = rcDIB.left = 0;
	rcDIB.right = cxDIB;
	rcDIB.bottom = cyDIB;
		
	// ���DIB
	::PaintDIB(pDC->m_hDC, &rcDIB, pDoc->m_hDIB,
			&rcDIB, pDoc->GetDocPalette());
}

/////////////////////////////////////////////////////////////////////////////
// CHytImgProView printing

BOOL CHytImgProView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CHytImgProView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CHytImgProView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CHytImgProView diagnostics

#ifdef _DEBUG
void CHytImgProView::AssertValid() const
{
	CView::AssertValid();
}

void CHytImgProView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHytImgProDoc* CHytImgProView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHytImgProDoc)));
	return (CHytImgProDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHytImgProView message handlers

void CHytImgProView::OnMenuConvertGray() 
{
	CHytImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_hDIB==NULL)
		return;
		
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);	
	
	WORD NumColors = ::DIBNumColors(lpDIB);
	
	if (NumColors != 256)
	{
		MessageBox("����256ɫλͼ���������ܽ��лҶ�ת����", "��ʾ" , MB_ICONINFORMATION | MB_OK);		
		::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
		return;
	}

	LPBITMAPINFO lpbmiDIB;
	lpbmiDIB = (LPBITMAPINFO)lpDIB;
	BYTE BmpGray[256];
	BYTE GrayColor[256][4];
	int i,j;
	
	// ����Ҷ�ӳ������������ɫ�ĻҶ�ֵ����������DIB��ɫ��
	for (i=0;i<256;i++)
	{
		BmpGray[i]=(BYTE)(0.3*lpbmiDIB->bmiColors[i].rgbRed+
			0.59*lpbmiDIB->bmiColors[i].rgbGreen+0.11*lpbmiDIB->bmiColors[i].rgbBlue);
		lpbmiDIB->bmiColors[i].rgbRed=i;
		lpbmiDIB->bmiColors[i].rgbGreen=i;
		lpbmiDIB->bmiColors[i].rgbBlue=i;
		GrayColor[i][0]=GrayColor[i][1]=GrayColor[i][2]=i;
		GrayColor[i][3]=lpbmiDIB->bmiColors[i].rgbReserved=0;
	}

	LPSTR lpDIBBits = ::FindDIBBits(lpDIB);		
	LONG DibWidth = ::DIBWidth(lpDIB);
	LONG DibHeight = ::DIBHeight(lpDIB);
	LONG LineBytes = WIDTHBYTES(DibWidth * 8); //ÿ�е��ֽ���
	BYTE* lpBitData;
	for (i=0;i<DibHeight;i++)
	{
		for (j=0;j<DibWidth;j++)
		{
			lpBitData=(BYTE*)lpDIBBits+LineBytes*i+j;
			*lpBitData=BmpGray[*lpBitData];
		}
	}

	pDoc->GetDocPalette()->SetPaletteEntries(0, 256, (LPPALETTEENTRY) GrayColor);
	
	pDoc->SetModifiedFlag(TRUE);



	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �������
	::GlobalUnlock((HGLOBAL)  pDoc->m_hDIB);

}

LRESULT CHytImgProView::OnDoRealize(WPARAM wParam, LPARAM)
{
	ASSERT(wParam != NULL);
	
	CHytImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// �ж�DIB�Ƿ�Ϊ��
	if (pDoc->m_hDIB == NULL)
	{
		// ֱ�ӷ���
		return 0L;
	}
	
	// ��ȡPalette
	CPalette* pPal = pDoc->GetDocPalette();
	if (pPal != NULL)
	{
		// ��ȡMainFrame
		CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		ASSERT_KINDOF(CMainFrame, pAppFrame);
		
		CClientDC appDC(pAppFrame);
		
		// All views but one should be a background palette.
		// wParam contains a handle to the active view, so the SelectPalette
		// bForceBackground flag is FALSE only if wParam == m_hWnd (this view)
		CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd);
		
		if (oldPalette != NULL)
		{
			UINT nColorsChanged = appDC.RealizePalette();
			if (nColorsChanged > 0)
				pDoc->UpdateAllViews(NULL);
			appDC.SelectPalette(oldPalette, TRUE);
		}
		else
		{
			TRACE0("\tCHytImgProView::OnPaletteChanged�е���SelectPalette()ʧ�ܣ�\n");
		}
	}
	
	return 0L;
}

void CHytImgProView::OnButtonReset() 
{
	CHytImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if (pDoc->m_hSrcDIB==NULL)
		return;

	if (pDoc->m_hDIB==NULL)
	{
		// ΪDIB�����ڴ�
		pDoc->m_hDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,pDoc->m_FileSize);
		
		if (pDoc->m_hDIB == 0)
		{
			// �ڴ����ʧ�ܣ�����NULL��
			return;
		}
	}
	pDoc->m_hDIB=(HDIB)CopyHandle((HANDLE) pDoc->m_hSrcDIB);
	
	// ����DIB��С�͵�ɫ��
	pDoc->InitDIBData();
	
	pDoc->SetModifiedFlag(FALSE);
	
	// ʵ���µĵ�ɫ��
	OnDoRealize((WPARAM)m_hWnd,0);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
}

void CHytImgProView::OnMenuOpposite() 
{
	CHytImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if (pDoc->m_hDIB==NULL)
		return;
	
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);	
	
	WORD NumColors = ::DIBNumColors(lpDIB);
	
	if (NumColors != 256)
	{
		MessageBox("����256ɫλͼ���������ܽ��лҶ�ת����", "��ʾ" , MB_ICONINFORMATION | MB_OK);		
		::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
		return;
	}
	int i,j;
	
	
	LPSTR lpDIBBits = ::FindDIBBits(lpDIB);		
	LONG DibWidth = ::DIBWidth(lpDIB);
	LONG DibHeight = ::DIBHeight(lpDIB);
	LONG LineBytes = WIDTHBYTES(DibWidth * 8); //ÿ�е��ֽ���
	BYTE* lpBitData;
	for (i=0;i<DibHeight;i++)
	{
		for (j=0;j<DibWidth;j++)
		{
			lpBitData=(BYTE*)lpDIBBits+LineBytes*i+j;
			*lpBitData=255-*lpBitData;
		}
	}
	
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �������
	::GlobalUnlock((HGLOBAL)  pDoc->m_hDIB);
	
}

void CHytImgProView::OnMenuGrayStrecthing() 
{
	CHytImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if (pDoc->m_hDIB==NULL)
		return;
	
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);	
	
	WORD NumColors = ::DIBNumColors(lpDIB);
	
	if (NumColors != 256)
	{
		MessageBox("����256ɫλͼ���������ܽ��лҶ�ת����", "��ʾ" , MB_ICONINFORMATION | MB_OK);		
		::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
		return;
	}

	CDlgGrayStre dlgGrayStre;
	int	GrayStreR1;
	int	GrayStreS1;
	int	GrayStreR2;
	int	GrayStreS2; 

	if (dlgGrayStre.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	GrayStreR1=dlgGrayStre.m_GrayStreR1;
	GrayStreS1=dlgGrayStre.m_GrayStreS1;
	GrayStreR2=dlgGrayStre.m_GrayStreR2;
	GrayStreS2=dlgGrayStre.m_GrayStreS2;

	if (GrayStreR1>GrayStreR2)
	{
		MessageBox("��һ�������ӦС�ڵڶ�������꣬���������ã�", "��ʾ" , MB_ICONINFORMATION | MB_OK);		
		return;
	}
	
	delete dlgGrayStre;	

	int i,j;
	float k1,k2,k3,b2,b3,k,b;	
	LPSTR lpDIBBits = ::FindDIBBits(lpDIB);		
	LONG DibWidth = ::DIBWidth(lpDIB);
	LONG DibHeight = ::DIBHeight(lpDIB);
	LONG LineBytes = WIDTHBYTES(DibWidth * 8); //ÿ�е��ֽ���
	BYTE* lpBitData;

	//���������߶ε�б�ʺͽؾ�,ֱ��y=k*x+b
	k1=(float)GrayStreS1/GrayStreR1;      
	k2=(float)(GrayStreS1-GrayStreS2)/(GrayStreR1-GrayStreR2);
	b2=(float)(GrayStreR1*GrayStreS2-GrayStreR2*GrayStreS1)/(GrayStreR1-GrayStreR2);
	k3=(float)(255-GrayStreS2)/(255-GrayStreR2);
	b3=(float)(255*GrayStreS2-GrayStreR2*255)/(255-GrayStreR2);

	for (i=0;i<DibHeight;i++)
	{
		for (j=0;j<DibWidth;j++)
		{
			lpBitData=(BYTE*)lpDIBBits+LineBytes*i+j;
			if (*lpBitData<=GrayStreR1)
			{
				k=k1;
				b=0;
			}
			else if (*lpBitData<=GrayStreR2)
			{
				k=k2;
				b=b2;
			}
			else
			{
				k=k3;
				b=b3;
			}
			*lpBitData=(int)(k**lpBitData+b);
		}
	}
	
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �������
	::GlobalUnlock((HGLOBAL)  pDoc->m_hDIB);
}



void CHytImgProView::OnMenuPowerTrans() 
{
	CHytImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if (pDoc->m_hDIB==NULL)
		return;
	
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);	
	
	WORD NumColors = ::DIBNumColors(lpDIB);
	
	if (NumColors != 256)
	{
		MessageBox("����256ɫλͼ���������ܽ��лҶ�ת����", "��ʾ" , MB_ICONINFORMATION | MB_OK);		
		::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
		return;
	}
	
	CDlgPowerTrans dlgPowerTrans;
	float EditC;
	float EditR;
	
	if (dlgPowerTrans.DoModal() != IDOK)
	{
		// ����
		return;
	}
	
	EditC=dlgPowerTrans.m_EditC;
	EditR=dlgPowerTrans.m_EditR;
	
	if (EditC<=0||EditR<=0)
	{
		MessageBox("�ݴα任����ӦΪ��������", "��ʾ" , MB_ICONINFORMATION | MB_OK);		
		return;
	}
	
	delete dlgPowerTrans;	
	
	int i,j;	
	LPSTR lpDIBBits = ::FindDIBBits(lpDIB);		
	LONG DibWidth = ::DIBWidth(lpDIB);
	LONG DibHeight = ::DIBHeight(lpDIB);
	LONG LineBytes = WIDTHBYTES(DibWidth * 8); //ÿ�е��ֽ���
	BYTE* lpBitData;
	double L_propor;         //ʵ�ʻҶ�ռ�Ҷ���L�İٷֱ�
	
	for (i=0;i<DibHeight;i++)
	{
		for (j=0;j<DibWidth;j++)
		{
			lpBitData=(BYTE*)lpDIBBits+LineBytes*i+j;
			L_propor=*lpBitData/256.0;
			*lpBitData=(int)(EditC*pow(L_propor,EditR)*255.0);
			if (*lpBitData>255)
			{
				*lpBitData=255;
			}
		}
	}
	
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �������
	::GlobalUnlock((HGLOBAL)  pDoc->m_hDIB);	
}

void CHytImgProView::OnMenuHistogramPro() 
{
	CHytImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if (pDoc->m_hDIB==NULL)
		return;
	
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);	
	
	WORD NumColors = ::DIBNumColors(lpDIB);
	
	if (NumColors != 256)
	{
		MessageBox("����256ɫλͼ���������ܽ��лҶ�ת����", "��ʾ" , MB_ICONINFORMATION | MB_OK);		
		::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
		return;
	}
	
	int i,j;	
	LPSTR lpDIBBits = ::FindDIBBits(lpDIB);		
	LONG DibWidth = ::DIBWidth(lpDIB);
	LONG DibHeight = ::DIBHeight(lpDIB);
	LONG LineBytes = WIDTHBYTES(DibWidth * 8); //ÿ�е��ֽ���
	BYTE* lpBitData;
	DWORD n=0;
	DWORD nk[256];
	double sk[256];

	for (i=0;i<256;i++)
	{
		nk[i]=0;
		sk[i]=0;
	}
	
	for (i=0;i<DibHeight;i++)
	{
		for (j=0;j<DibWidth;j++)
		{
			lpBitData=(BYTE*)lpDIBBits+LineBytes*i+j;
			nk[*lpBitData]++;
			n++;
		}
	}

	for (i=0;i<256;i++)
	{
		for (j=0;j<=i;j++)
		{
			sk[i]+=nk[j];
		}
		sk[i]=sk[i]/n;
	}

	for (i=0;i<DibHeight;i++)
	{
		for (j=0;j<DibWidth;j++)
		{
			lpBitData=(BYTE*)lpDIBBits+LineBytes*i+j;
			*lpBitData=(int)(sk[*lpBitData]*255);
		}
	}
	
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �������
	::GlobalUnlock((HGLOBAL)  pDoc->m_hDIB);	
}

void CHytImgProView::OnMenuAverFilter() 
{
	CHytImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if (pDoc->m_hDIB==NULL)
		return;
	
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);	
	
	WORD NumColors = ::DIBNumColors(lpDIB);
	
	if (NumColors != 256)
	{
		MessageBox("����256ɫλͼ���������ܽ��лҶ�ת����", "��ʾ" , MB_ICONINFORMATION | MB_OK);		
		::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
		return;
	}

	//����һ������������ԭ����ͼ��
	HDIB hSrcDIB;
	hSrcDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,pDoc->m_FileSize);	
	if (hSrcDIB == 0)
	{
		// �ڴ����ʧ�ܣ�����NULL��
		MessageBox(" �ڴ����ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		return;
	}
	hSrcDIB=(HDIB)CopyHandle((HANDLE) pDoc->m_hDIB);
	LPSTR lpSrcDIB = (LPSTR) ::GlobalLock((HGLOBAL) hSrcDIB);	

	int i,j,PixelGray;	
	LPSTR lpDIBBits = ::FindDIBBits(lpDIB);	
	LPSTR lpSrcDIBBits = ::FindDIBBits(lpSrcDIB);	
	LONG DibWidth = ::DIBWidth(lpDIB);
	LONG DibHeight = ::DIBHeight(lpDIB);
	LONG LineBytes = WIDTHBYTES(DibWidth * 8); //ÿ�е��ֽ���
	BYTE* lpBitData;
	BYTE* lpSrcBitData1,*lpSrcBitData2,*lpSrcBitData3;
	
	for (i=1;i<DibHeight-1;i++)
	{
		for (j=1;j<DibWidth-1;j++)
		{
			lpSrcBitData1=(BYTE*)lpSrcDIBBits+LineBytes*(i-1)+j;
			lpSrcBitData2=(BYTE*)lpSrcDIBBits+LineBytes*i+j;
			lpSrcBitData3=(BYTE*)lpSrcDIBBits+LineBytes*(i+1)+j;
			lpBitData=(BYTE*)lpDIBBits+LineBytes*i+j;
			PixelGray=(int)((*(lpSrcBitData1-1)+*lpSrcBitData1*2+*(lpSrcBitData1+1)
					 +*(lpSrcBitData2-1)*2+*lpSrcBitData2*4+*(lpSrcBitData2+1)*2
				      +*(lpSrcBitData3-1)+*lpSrcBitData3*2+*(lpSrcBitData3+1))/16);
			*lpBitData=(unsigned char)PixelGray;
		}
	}
	
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �������
	::GlobalUnlock((HGLOBAL)  pDoc->m_hDIB);
	::GlobalUnlock((HGLOBAL)  hSrcDIB);

	// ���DIB����
	::GlobalFree((HGLOBAL) hSrcDIB);
	hSrcDIB=NULL;	
}

void CHytImgProView::OnMenuMiddleFilter() 
{
	CHytImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if (pDoc->m_hDIB==NULL)
		return;
	
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);	
	
	WORD NumColors = ::DIBNumColors(lpDIB);
	
	if (NumColors != 256)
	{
		MessageBox("����256ɫλͼ���������ܽ��лҶ�ת����", "��ʾ" , MB_ICONINFORMATION | MB_OK);		
		::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
		return;
	}
	
	//����һ������������ԭ����ͼ��
	HDIB hSrcDIB;
	hSrcDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,pDoc->m_FileSize);	
	if (hSrcDIB == 0)
	{
		// �ڴ����ʧ�ܣ�����NULL��
		MessageBox(" �ڴ����ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		return;
	}
	hSrcDIB=(HDIB)CopyHandle((HANDLE) pDoc->m_hDIB);
	LPSTR lpSrcDIB = (LPSTR) ::GlobalLock((HGLOBAL) hSrcDIB);	
	
	int i,j,sort[9];	
	LPSTR lpDIBBits = ::FindDIBBits(lpDIB);	
	LPSTR lpSrcDIBBits = ::FindDIBBits(lpSrcDIB);	
	LONG DibWidth = ::DIBWidth(lpDIB);
	LONG DibHeight = ::DIBHeight(lpDIB);
	LONG LineBytes = WIDTHBYTES(DibWidth * 8); //ÿ�е��ֽ���
	BYTE* lpBitData;
	BYTE* lpSrcBitData1,*lpSrcBitData2,*lpSrcBitData3;
	
	for (i=1;i<DibHeight-1;i++)
	{
		for (j=1;j<DibWidth-1;j++)
		{
			lpSrcBitData1=(BYTE*)lpSrcDIBBits+LineBytes*(i-1)+j;
			lpSrcBitData2=(BYTE*)lpSrcDIBBits+LineBytes*i+j;
			lpSrcBitData3=(BYTE*)lpSrcDIBBits+LineBytes*(i+1)+j;
			lpBitData=(BYTE*)lpDIBBits+LineBytes*i+j;
			sort[0]=(int)*(lpSrcBitData1-1);
			sort[1]=(int)*lpSrcBitData1;
			sort[2]=(int)*(lpSrcBitData1+1);
			sort[3]=(int)*(lpSrcBitData2-1);
			sort[4]=(int)*lpSrcBitData2;
			sort[5]=(int)*(lpSrcBitData2+1);
			sort[6]=(int)*(lpSrcBitData2-1);
			sort[7]=(int)*lpSrcBitData2;
			sort[8]=(int)*(lpSrcBitData2+1);
			PixelSort(sort);
			*lpBitData=(unsigned char)sort[4];
		}
	}
	
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �������
	::GlobalUnlock((HGLOBAL)  pDoc->m_hDIB);
	::GlobalUnlock((HGLOBAL)  hSrcDIB);
	
	// ���DIB����
	::GlobalFree((HGLOBAL) hSrcDIB);
	hSrcDIB=NULL;		
}

void CHytImgProView::PixelSort(int a[])
{
	int i,j,temp;

	for (i=0;i<8;i++)
	{
		for (j=i+1;j<9;j++)
		{
			if (a[i]>a[j])
			{
				temp=a[i];
				a[i]=a[j];
				a[j]=temp;
			}
		}
	}
}

void CHytImgProView::OnMenuLaplace() 
{	
	CHytImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if (pDoc->m_hDIB==NULL)
		return;
	
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);	
	
	WORD NumColors = ::DIBNumColors(lpDIB);
	
	if (NumColors != 256)
	{
		MessageBox("����256ɫλͼ���������ܽ��лҶ�ת����", "��ʾ" , MB_ICONINFORMATION | MB_OK);		
		::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
		return;
	}
	
	//����һ������������ԭ����ͼ��
	HDIB hSrcDIB;
	hSrcDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,pDoc->m_FileSize);	
	if (hSrcDIB == 0)
	{
		// �ڴ����ʧ�ܣ�����NULL��
		MessageBox(" �ڴ����ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		return;
	}
	hSrcDIB=(HDIB)CopyHandle((HANDLE) pDoc->m_hDIB);
	LPSTR lpSrcDIB = (LPSTR) ::GlobalLock((HGLOBAL) hSrcDIB);	
	
	int i,j,PixelGray;	
	LPSTR lpDIBBits = ::FindDIBBits(lpDIB);	
	LPSTR lpSrcDIBBits = ::FindDIBBits(lpSrcDIB);	
	LONG DibWidth = ::DIBWidth(lpDIB);
	LONG DibHeight = ::DIBHeight(lpDIB);
	LONG LineBytes = WIDTHBYTES(DibWidth * 8); //ÿ�е��ֽ���
	BYTE* lpBitData;
	BYTE* lpSrcBitData1,*lpSrcBitData2,*lpSrcBitData3;
	
	for (i=1;i<DibHeight-1;i++)
	{
		for (j=1;j<DibWidth-1;j++)
		{
			lpSrcBitData1=(BYTE*)lpSrcDIBBits+LineBytes*(i-1)+j;
			lpSrcBitData2=(BYTE*)lpSrcDIBBits+LineBytes*i+j;
			lpSrcBitData3=(BYTE*)lpSrcDIBBits+LineBytes*(i+1)+j;
			lpBitData=(BYTE*)lpDIBBits+LineBytes*i+j;
			PixelGray=-*(lpSrcBitData1-1)-*lpSrcBitData1-*(lpSrcBitData1+1)
							-*(lpSrcBitData2-1)+*lpSrcBitData2*9-*(lpSrcBitData2+1)
							-*(lpSrcBitData3-1)-*lpSrcBitData3-*(lpSrcBitData3+1);
			PixelGray=fabs(PixelGray);

			if (PixelGray>255)
			{
				PixelGray=255;
			}
			*lpBitData=(unsigned char)PixelGray;
		}
	}
	
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �������
	::GlobalUnlock((HGLOBAL)  pDoc->m_hDIB);
	::GlobalUnlock((HGLOBAL)  hSrcDIB);
	
	// ���DIB����
	::GlobalFree((HGLOBAL) hSrcDIB);
	hSrcDIB=NULL;	
}
