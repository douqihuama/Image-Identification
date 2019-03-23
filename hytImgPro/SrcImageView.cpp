// SrcImageView.cpp : implementation file
//

#include "stdafx.h"
#include "hytImgPro.h"
#include "hytImgProDoc.h"
#include "SrcImageView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSrcImageView

IMPLEMENT_DYNCREATE(CSrcImageView, CView)

CSrcImageView::CSrcImageView()
{
}

CSrcImageView::~CSrcImageView()
{
}


BEGIN_MESSAGE_MAP(CSrcImageView, CView)
	//{{AFX_MSG_MAP(CSrcImageView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSrcImageView drawing

void CSrcImageView::OnDraw(CDC* pDC)
{
	CHytImgProDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// ��ʾ�ȴ����
	BeginWaitCursor();
	
	// ��ȡDIB
	HDIB hDIB = pDoc->m_hDIB;
	
	// �ж�DIB�Ƿ�Ϊ��
	if (hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		
		// ��ȡDIB���
		int cxDIB = (int) ::DIBWidth(lpDIB);
		
		// ��ȡDIB�߶�
		int cyDIB = (int) ::DIBHeight(lpDIB);
		
		::GlobalUnlock((HGLOBAL) hDIB);
		
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		
		// ���DIB
		::PaintDIB(pDC->m_hDC, &rcDIB, pDoc->m_hDIB,
			&rcDIB, pDoc->GetDocPalette());
	}
	
	// �ָ��������
	EndWaitCursor();
	
}

/////////////////////////////////////////////////////////////////////////////
// CSrcImageView diagnostics

#ifdef _DEBUG
void CSrcImageView::AssertValid() const
{
	CView::AssertValid();
}

void CSrcImageView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHytImgProDoc* CSrcImageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHytImgProDoc)));
	return (CHytImgProDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSrcImageView message handlers
