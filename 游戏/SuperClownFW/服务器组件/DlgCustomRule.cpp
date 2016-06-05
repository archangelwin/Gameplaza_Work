#include "Stdafx.h"
#include "Resource.h"
#include "DlgCustomRule.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgCustomRule, CDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgCustomRule::CDlgCustomRule() : CDialog(IDD_CUSTOM_RULE)
{
	//���ñ���
	ZeroMemory(&m_CustomRule,sizeof(m_CustomRule));

	return;
}

//��������
CDlgCustomRule::~CDlgCustomRule()
{
}

//���ú���
BOOL CDlgCustomRule::OnInitDialog()
{
	__super::OnInitDialog();

	//���ÿؼ�
	((CEdit *)GetDlgItem(IDC_TIME_OUT_CARD))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_TIME_START_GAME))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_TIME_CALL_SCORE))->LimitText(2);
	((CEdit *)GetDlgItem(IDC_MAX_SCORE_m_nTimes))->LimitText(3);
	((CEdit *)GetDlgItem(IDC_FLEE_SCORE_m_nTimes))->LimitText(3);
	((CEdit *)GetDlgItem(IDC_TIME_HEAD_OUT_CARD))->LimitText(2);

	//���²���
	FillDataToControl();

	return FALSE;
}

//ȷ������
VOID CDlgCustomRule::OnOK() 
{ 
	//Ͷ����Ϣ
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDOK,0),0);

	return;
}

//ȡ����Ϣ
VOID CDlgCustomRule::OnCancel() 
{ 
	//Ͷ����Ϣ
	GetParent()->PostMessage(WM_COMMAND,MAKELONG(IDCANCEL,0),0);

	return;
}

//���¿ؼ�
bool CDlgCustomRule::FillDataToControl()
{
	//��������
	SetDlgItemInt(IDC_TIME_OUT_CARD,m_CustomRule.cbTimeOutCard);
	SetDlgItemInt(IDC_TIME_START_GAME,m_CustomRule.cbm_nTimestartGame);
	SetDlgItemInt(IDC_TIME_CALL_SCORE,m_CustomRule.cbTimeCallScore);
	SetDlgItemInt(IDC_TIME_HEAD_OUT_CARD,m_CustomRule.cbTimeHeadOutCard);

	//��Ϸ����
	SetDlgItemInt(IDC_MAX_SCORE_m_nTimes,m_CustomRule.wMaxScorem_nTimes);
	SetDlgItemInt(IDC_FLEE_SCORE_m_nTimes,m_CustomRule.wFleeScorem_nTimes);
	((CButton *)GetDlgItem(IDC_FLEE_SCORE_PATCH))->SetCheck((m_CustomRule.cbFleeScorePatch==TRUE)?BST_CHECKED:BST_UNCHECKED);

	return true;
}

//��������
bool CDlgCustomRule::FillControlToData()
{
	//��������
	m_CustomRule.cbTimeOutCard=(BYTE)GetDlgItemInt(IDC_TIME_OUT_CARD);
	m_CustomRule.cbm_nTimestartGame=(BYTE)GetDlgItemInt(IDC_TIME_START_GAME);
	m_CustomRule.cbTimeCallScore=(BYTE)GetDlgItemInt(IDC_TIME_CALL_SCORE);
	m_CustomRule.cbTimeHeadOutCard=(BYTE)GetDlgItemInt(IDC_TIME_HEAD_OUT_CARD);

	//��Ϸ����
	m_CustomRule.wMaxScorem_nTimes=(WORD)GetDlgItemInt(IDC_MAX_SCORE_m_nTimes);
	m_CustomRule.wFleeScorem_nTimes=(WORD)GetDlgItemInt(IDC_FLEE_SCORE_m_nTimes);
	m_CustomRule.cbFleeScorePatch=(((CButton *)GetDlgItem(IDC_FLEE_SCORE_PATCH))->GetCheck()==BST_CHECKED);

	//��ʼʱ��
	if ((m_CustomRule.cbm_nTimestartGame<5)||(m_CustomRule.cbm_nTimestartGame>60))
	{
		AfxMessageBox(TEXT("��ʼʱ�����÷�Χ�������������ã�"),MB_ICONSTOP);
		return false;
	}

	//�з�ʱ��
	if ((m_CustomRule.cbTimeCallScore<5)||(m_CustomRule.cbTimeCallScore>60))
	{
		AfxMessageBox(TEXT("�з�ʱ�����÷�Χ�������������ã�"),MB_ICONSTOP);
		return false;
	}

	//�׳�ʱ��
	if ((m_CustomRule.cbTimeHeadOutCard<5)||(m_CustomRule.cbTimeHeadOutCard>60))
	{
		AfxMessageBox(TEXT("�׳�ʱ�����÷�Χ�������������ã�"),MB_ICONSTOP);
		return false;
	}

	//����ʱ��
	if ((m_CustomRule.cbTimeOutCard<5)||(m_CustomRule.cbTimeOutCard>60))
	{
		AfxMessageBox(TEXT("����ʱ�����÷�Χ�������������ã�"),MB_ICONSTOP);
		return false;
	}

	//�����
	if ((m_CustomRule.wMaxScorem_nTimes<32)||(m_CustomRule.wMaxScorem_nTimes>512))
	{
		AfxMessageBox(TEXT("��������÷�Χ�������������ã�"),MB_ICONSTOP);
		return false;
	}

	//���ܷ���
	if ((m_CustomRule.wFleeScorem_nTimes<2)||(m_CustomRule.wFleeScorem_nTimes>512))
	{
		AfxMessageBox(TEXT("���ܱ������÷�Χ�������������ã�"),MB_ICONSTOP);
		return false;
	}

	return true;
}

//��ȡ����
bool CDlgCustomRule::GetCustomRule(tagCustomRule & CustomRule)
{
	//��ȡ����
	if (FillControlToData()==true)
	{
		CustomRule=m_CustomRule;
		return true;
	}

	return false;
}

//��������
bool CDlgCustomRule::SetCustomRule(tagCustomRule & CustomRule)
{
	//���ñ���
	m_CustomRule=CustomRule;

	//���²���
	if (m_hWnd!=NULL) FillDataToControl();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
