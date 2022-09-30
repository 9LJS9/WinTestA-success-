
// WinTestAView.cpp: CWinTestAView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "WinTestA.h"
#endif

#include "WinTestADoc.h"
#include "WinTestAView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWinTestAView

IMPLEMENT_DYNCREATE(CWinTestAView, CScrollView)

BEGIN_MESSAGE_MAP(CWinTestAView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_COMMAND(IDM_CONST_ADD, &CWinTestAView::OnConstAdd)
	ON_COMMAND(IDM_CONST_SUB, &CWinTestAView::OnConstSub)
	ON_COMMAND(IDM_CONST_MUL, &CWinTestAView::OnConstMul)
	ON_COMMAND(IDM_CONST_DIV, &CWinTestAView::OnConstDiv)
END_MESSAGE_MAP()

// CWinTestAView 생성/소멸

CWinTestAView::CWinTestAView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	height = width = 256;
	int rwsize = (((width)+31) / 32 * 4);

	BmInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));

	BmInfo->bmiHeader.biBitCount = 8;
	BmInfo->bmiHeader.biClrImportant = 256;
	BmInfo->bmiHeader.biClrUsed = 256;
	BmInfo->bmiHeader.biCompression = 0;
	BmInfo->bmiHeader.biHeight = height;
	BmInfo->bmiHeader.biPlanes = 1;
	BmInfo->bmiHeader.biSize = 40;
	BmInfo->bmiHeader.biSizeImage = rwsize * height;
	BmInfo->bmiHeader.biWidth = width;
	BmInfo->bmiHeader.biXPelsPerMeter = 0;
	BmInfo->bmiHeader.biYPelsPerMeter = 0;
	for (int i = 0; i < 256; i++) {
		BmInfo->bmiColors[i].rgbRed = BmInfo->bmiColors[i].rgbGreen = BmInfo->bmiColors[i].rgbBlue = i;
		BmInfo->bmiColors[i].rgbReserved = 0;
	}

}

CWinTestAView::~CWinTestAView()
{
	free(BmInfo);
}

BOOL CWinTestAView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CWinTestAView 그리기

void CWinTestAView::OnDraw(CDC* pDC)
{
	CWinTestADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	/*for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			unsigned char InVal = pDoc->m_InImg[i][j];
			unsigned char OutVal = pDoc->m_OutImg[i][j];

			pDC->SetPixel(j, i, RGB(InVal, InVal, InVal));
			pDC->SetPixel(j+300, i, RGB(OutVal, OutVal, OutVal));
		}
	}*/
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			m_RevImg[i][j] = pDoc->m_InImg[height - i - 1][j];
		}
	}

	SetDIBitsToDevice(pDC->GetSafeHdc(), 0, 0, width, height, 0, 0, 0, height, m_RevImg, BmInfo, DIB_RGB_COLORS);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			m_RevImg[i][j] = pDoc->m_OutImg[height - i - 1][j];
		}
	}

	SetDIBitsToDevice(pDC->GetSafeHdc(), 300, 0, width, height, 0, 0, 0, height, m_RevImg, BmInfo, DIB_RGB_COLORS);
}

void CWinTestAView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 이 뷰의 전체 크기를 계산합니다.
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CWinTestAView 인쇄

BOOL CWinTestAView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CWinTestAView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CWinTestAView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CWinTestAView 진단

#ifdef _DEBUG
void CWinTestAView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWinTestAView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWinTestADoc* CWinTestAView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinTestADoc)));
	return (CWinTestADoc*)m_pDocument;
}
#endif //_DEBUG


// CWinTestAView 메시지 처리기


void CWinTestAView::OnReverseImg()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int i, j;
	for (i = 0; i < 256; i++) {
		for (j = 0; j < 256; j++) {
			pDoc->m_OutImg[i][j] = 255 - pDoc->m_InImg[i][j];
		}
	}
	Invalidate(FALSE);
	
}


void CWinTestAView::OnConstAdd()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int tempVal = pDoc->m_InImg[i][j] + 60;
			tempVal = tempVal > 255 ? 255 : tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}
	Invalidate(false);
}


void CWinTestAView::OnConstSub()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int tempVal = pDoc->m_InImg[i][j] - 60;
			tempVal = tempVal < 0 ? 0 : tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}
	Invalidate(false);
}


void CWinTestAView::OnConstMul()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int tempVal = (int)(pDoc->m_InImg[i][j] *1.4);
			tempVal = tempVal > 255 ? 255 : tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}
	Invalidate(false);
}


void CWinTestAView::OnConstDiv()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CWinTestADoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int tempVal = (int)(pDoc->m_InImg[i][j] /1.4);
			tempVal = tempVal > 255 ? 255 : tempVal;
			tempVal = tempVal < 0 ? 0 : tempVal;
			pDoc->m_OutImg[i][j] = (unsigned char)tempVal;
		}
	}
	Invalidate(false);
}
