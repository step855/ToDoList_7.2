// TaskFileTest.cpp: implementation of the CTaskFileTest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TDLTest.h"
#include "ToDoCtrlTaskLinkTest.h"

#include "..\ToDoList\ToDoCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////

// Stubs
CTDLContentMgr mgrContent;
CONTENTFORMAT cfDefault;
TDCCOLEDITFILTERVISIBILITY visDefault;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CToDoCtrlTaskLinkTest::CToDoCtrlTaskLinkTest(const CTestUtils& utils) 
	: 
	CTDLTestBase(utils),
	m_tdc(mgrContent, cfDefault, visDefault)
{

}

CToDoCtrlTaskLinkTest::~CToDoCtrlTaskLinkTest()
{

}

void CToDoCtrlTaskLinkTest::Run()
{
	TestFormatTaskLink();
	TestFormatTaskDependency();
	TestParseTaskLink();
}

void CToDoCtrlTaskLinkTest::TestFormatTaskLink()
{
	BeginTest(_T("CToDoCtrl::FormatTaskLink"));

	//  ---------------------------------------

	m_tdc.SetFilePath(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectEQ(m_tdc.FormatTaskLink(0, FALSE), CString(_T(""))); // Must provide task ID
	ExpectEQ(m_tdc.FormatTaskLink(0, TRUE), CString(_T(""))); // Must provide task ID

	ExpectEQ(m_tdc.FormatTaskLink(10, FALSE), CString(_T("tdl://10")));
	ExpectEQ(m_tdc.FormatTaskLink(10, TRUE), CString(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?10")));

	//  ---------------------------------------

	EndTest();
}

void CToDoCtrlTaskLinkTest::TestFormatTaskDependency()
{
	BeginTest(_T("CToDoCtrl::FormatTaskDependency"));

	//  ---------------------------------------

	m_tdc.SetFilePath(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectEQ(m_tdc.FormatTaskDependency(0, FALSE), CString(_T(""))); // Must provide task ID
	ExpectEQ(m_tdc.FormatTaskDependency(0, TRUE), CString(_T(""))); // Must provide task ID

	ExpectEQ(m_tdc.FormatTaskDependency(10, FALSE), CString(_T("10")));
	ExpectEQ(m_tdc.FormatTaskDependency(10, TRUE), CString(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl?10")));
	
	//  ---------------------------------------

	EndTest();
}

void CToDoCtrlTaskLinkTest::TestParseTaskLink()
{
	BeginTest(_T("CToDoCtrl::ParseTaskLink"));

	//  ---------------------------------------

	m_tdc.SetFilePath(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	DWORD dwTaskID = 0;
	CString sFilePath;

	//  ---------------------------------------

	// Task links (bURL = TRUE)
	// Note: Handles ONLY URLs

	// Task ID only
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://10"), TRUE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T(""));

	// Task ID and path
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?10"), TRUE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Task path only
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl"), TRUE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Missing taskID is okay
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?"), TRUE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Missing tdl:// is NOT okay
	ExpectFalse(m_tdc.ParseTaskLink(_T("10"), TRUE, dwTaskID, sFilePath));
	ExpectFalse(m_tdc.ParseTaskLink(_T("C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?10"), TRUE, dwTaskID, sFilePath));
	ExpectFalse(m_tdc.ParseTaskLink(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl?10"), TRUE, dwTaskID, sFilePath));

	//  ---------------------------------------

	// Task Dependencies (bURL = FALSE)
	// Note: Handles both URLs and non_URLs

	// Task ID only
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://10"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T(""));

	ExpectTrue(m_tdc.ParseTaskLink(_T("10"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T(""));

	// Task ID and path
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?10"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?10"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 10UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Task path only
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	// Missing taskID is okay
	ExpectTrue(m_tdc.ParseTaskLink(_T("tdl://C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("C:/Users/Daniel%20Godson/AppData/Local/A%20Tasklist.tdl?"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	ExpectTrue(m_tdc.ParseTaskLink(_T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl?"), FALSE, dwTaskID, sFilePath));
	ExpectEQ(dwTaskID, 0UL);
	ExpectEQ(sFilePath, _T("C:\\Users\\Daniel Godson\\AppData\\Local\\A Tasklist.tdl"));

	//  ---------------------------------------

	EndTest();
}