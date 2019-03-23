// hytImgProDoc.cpp : implementation of the CHytImgProDoc class
//

#include "stdafx.h"
#include "hytImgPro.h"

#include "hytImgProDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHytImgProDoc

IMPLEMENT_DYNCREATE(CHytImgProDoc, CDocument)

BEGIN_MESSAGE_MAP(CHytImgProDoc, CDocument)
	//{{AFX_MSG_MAP(CHytImgProDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHytImgProDoc construction/destruction

CHytImgProDoc::CHytImgProDoc()
{
	m_hProDIB=NULL;
	m_hDIB = NULL;
	m_hSrcDIB=NULL;
	m_palDIB = NULL;
	m_sizeDoc = CSize(1,1);
}

CHytImgProDoc::~CHytImgProDoc()
{
	// �ж�DIB�����Ƿ����
	if (m_hDIB != NULL)
	{
		// ���DIB����
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB=NULL;
	}

	if (m_hSrcDIB != NULL)
	{
		// ���DIB����
		::GlobalFree((HGLOBAL) m_hSrcDIB);
		m_hSrcDIB=NULL;
	}
	
	// �жϵ�ɫ���Ƿ����
	if (m_palDIB != NULL)
	{
		// �����ɫ��
		delete m_palDIB;
	}

	if (m_hProDIB != NULL)
	{
		// ���DIB����
		::GlobalFree((HGLOBAL) m_hProDIB);
		m_hProDIB=NULL;
	}
}

BOOL CHytImgProDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CHytImgProDoc serialization

void CHytImgProDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHytImgProDoc diagnostics

#ifdef _DEBUG
void CHytImgProDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHytImgProDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHytImgProDoc commands

BOOL CHytImgProDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	CFile file;
	CFileException fe;
	
	// ���ļ�
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		// ʧ��
		ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		// ����FALSE
		return FALSE;
	}
	
	DeleteContents();
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ���Ե���ReadDIBFile()��ȡͼ��
	TRY
	{
		m_hDIB = ::ReadDIBFile(file);
	}
	CATCH (CFileException, eLoad)
	{
		// ��ȡʧ��
		file.Abort();
		
		// �ָ������״
		EndWaitCursor();
		
		// ����ʧ��
		ReportSaveLoadException(lpszPathName, eLoad,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		
		// ����DIBΪ��
		m_hDIB = NULL;
		
		// ����FALSE
		return FALSE;
	}
	END_CATCH

	// ��ʼ��DIB
	InitDIBData();
	
	// �ָ������״
	EndWaitCursor();
	
	// �ж϶�ȡ�ļ��Ƿ�ɹ�
	if (m_hDIB == NULL)
	{
		// ʧ�ܣ����ܷ�BMP��ʽ
		CString strMsg;
		strMsg = "��ȡͼ��ʱ���������ǲ�֧�ָ����͵�ͼ���ļ���";
		
		// ��ʾ����
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// ����FALSE
		return FALSE;
	}
	
	// �����ļ�����
	SetPathName(lpszPathName);
	
	// ��ʼ���ĵ��Ƿ��޸ı��ΪFALSE
	SetModifiedFlag(FALSE);

	// ��ȡDIB���ļ������ȣ��ֽڣ�
	m_FileSize= file.GetLength();
	
	//���´�ͼ����ͷ�ָ���ڴ�
	if (m_hSrcDIB != NULL)
	{
		// ���DIB����
		::GlobalFree((HGLOBAL) m_hSrcDIB);
		m_hSrcDIB=NULL;
	}
	if (m_hProDIB != NULL)
	{
		// ���DIB����
		::GlobalFree((HGLOBAL) m_hProDIB);
		m_hProDIB=NULL;
	}

	if (m_hSrcDIB==NULL)
	{
		// ΪDIB�����ڴ�
		m_hSrcDIB = (HDIB) ::GlobalAlloc(GMEM_MOVEABLE | GMEM_ZEROINIT,m_FileSize);
		
		if (m_hSrcDIB == 0)
		{
			// �ڴ����ʧ�ܣ�����NULL��
			MessageBox(NULL," CHytImgProDoc���ڴ����ʧ�ܣ�", "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		}
	}
	m_hSrcDIB=(HDIB)CopyHandle((HANDLE) m_hDIB);
	
	return TRUE;
}

void CHytImgProDoc::InitDIBData()
{
	// ��ʼ��DIB����
	
	// �жϵ�ɫ���Ƿ�Ϊ��
	if (m_palDIB != NULL)
	{
		// ɾ����ɫ�����
		delete m_palDIB;
		
		// ���õ�ɫ��Ϊ��
		m_palDIB = NULL;
	}
	
	// ���DIB����Ϊ�գ�ֱ�ӷ���
	if (m_hDIB == NULL)
	{
		// ����
		return;
	}
	
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	
	// �ж�ͼ���Ƿ����
	if (::DIBWidth(lpDIB) > INT_MAX ||::DIBHeight(lpDIB) > INT_MAX)
	{
		::GlobalUnlock((HGLOBAL) m_hDIB);
		
		// �ͷ�DIB����
		::GlobalFree((HGLOBAL) m_hDIB);
		
		// ����DIBΪ��
		m_hDIB = NULL;
		
		CString strMsg;
		strMsg = "BMPͼ��̫��";
		
		// ��ʾ�û�
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}
	
	// �����ĵ���С
	m_sizeDoc = CSize((int) ::DIBWidth(lpDIB), (int) ::DIBHeight(lpDIB));
	
	::GlobalUnlock((HGLOBAL) m_hDIB);
	
	// �����µ�ɫ��
	m_palDIB = new CPalette;
	
	// �ж��Ƿ񴴽��ɹ�
	if (m_palDIB == NULL)
	{
		// ʧ�ܣ��������ڴ治��
		::GlobalFree((HGLOBAL) m_hDIB);
		
		// ����DIB����Ϊ��
		m_hDIB = NULL;
		
		// ����
		return;
	}
	
	// ����CreateDIBPalette��������ɫ��
	if (::CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
	{
		// ���ؿգ����ܸ�DIB����û�е�ɫ��
		
		// ɾ��
		delete m_palDIB;
		
		// ����Ϊ��
		m_palDIB = NULL;
		
		// ����
		return;
	}
}

BOOL CHytImgProDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;
	
	// ���ļ�
	if (!file.Open(lpszPathName, CFile::modeCreate |
		CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		// ʧ��
		ReportSaveLoadException(lpszPathName, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		
		// ����FALSE
		return FALSE;
	}
	
	// ���Ե���SaveDIB����ͼ��
	BOOL bSuccess = FALSE;
	TRY
	{
		
		// ���Ĺ����״
		BeginWaitCursor();
		
		// ���Ա���ͼ��
		bSuccess = ::SaveDIB(m_hDIB, file);
		
		// �ر��ļ�
		file.Close();
	}
	CATCH (CException, eSave)
	{
		// ʧ��
		file.Abort();
		
		// �ָ������״
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eSave,
			TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		
		// ����FALSE
		return FALSE;
	}
	END_CATCH
		
		// �ָ������״
		EndWaitCursor();
	
	// �����ͱ��ΪFALSE
	SetModifiedFlag(FALSE);
	
	if (!bSuccess)
	{
		// ����ʧ�ܣ�������������ʽ��DIB�����Զ�ȡ���ǲ��ܱ���
		// ������SaveDIB��������
		
		CString strMsg;
		strMsg = "�޷�����BMPͼ��";
		
		// ��ʾ����
		MessageBox(NULL, strMsg, "ϵͳ��ʾ", MB_ICONINFORMATION | MB_OK);
	}
	
	return bSuccess;
	
/*	return CDocument::OnSaveDocument(lpszPathName);*/
}




