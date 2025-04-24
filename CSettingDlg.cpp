#include "pch.h"
#include "欢乐连连看.h"
#include "CGameSettingDlg.h"
#include "afxdialogex.h"
#include "global.h"
#include "ParamSetting.h"

// CGameSettingDlg 对话框

IMPLEMENT_DYNAMIC(CGameSettingDlg, CDialogEx)

CGameSettingDlg::CGameSettingDlg(CWnd *pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTING_DIALOG, pParent), m_nMapRows(4), m_nMapCols(4), m_nPicCount(4), m_nThemeIndex(0), m_bBGMusicOn(FALSE)
{
}

CGameSettingDlg::~CGameSettingDlg()
{
}

void CGameSettingDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ROW, m_editMapRows);
	DDX_Control(pDX, IDC_EDIT_COL, m_editMapCols);
	DDX_Control(pDX, IDC_EDIT_PIC, m_editPicCount);
	DDX_Control(pDX, IDC_COMBO_THEME, m_comboTheme);
}

BEGIN_MESSAGE_MAP(CGameSettingDlg, CDialogEx)
ON_BN_CLICKED(IDC_RADIO_OPEN, &CGameSettingDlg::OnBnClickedRadioBgmOn)
ON_BN_CLICKED(IDC_RADIO_CLOSE, &CGameSettingDlg::OnBnClickedRadioBgmOff)
ON_CBN_SELCHANGE(IDC_COMBO_THEME, &CGameSettingDlg::OnCbnSelchangeComboTheme)
ON_EN_CHANGE(IDC_EDIT_ROW, &CGameSettingDlg::OnEnChangeEditRows)
ON_EN_CHANGE(IDC_EDIT_COL, &CGameSettingDlg::OnEnChangeEditCols)
ON_EN_CHANGE(IDC_EDIT_PIC, &CGameSettingDlg::OnEnChangeEditPicCount)
ON_BN_CLICKED(IDOK, &CGameSettingDlg::OnBnClickedOk)
END_MESSAGE_MAP()

// CGameSettingDlg 消息处理程序

BOOL CGameSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置标题
	SetWindowText(_T("游戏设置"));

	// 初始化地图行列和图片个数
	CString strValue;

	// 设置当前值
	strValue.Format(_T("%d"), m_nMapRows);
	m_editMapRows.SetWindowText(strValue);

	strValue.Format(_T("%d"), m_nMapCols);
	m_editMapCols.SetWindowText(strValue);

	strValue.Format(_T("%d"), m_nPicCount);
	m_editPicCount.SetWindowText(strValue);

	// 添加主题选项
	m_comboTheme.AddString(_T("水果主题"));
	m_comboTheme.AddString(_T("动物主题"));
	m_comboTheme.AddString(_T("卡通主题"));

	// 设置当前选中的主题
	m_comboTheme.SetCurSel(m_nThemeIndex);

	// 设置音乐开关
	CButton *pRadioOn = (CButton *)GetDlgItem(IDC_RADIO_OPEN);
	CButton *pRadioOff = (CButton *)GetDlgItem(IDC_RADIO_CLOSE);

	if (m_bBGMusicOn)
		pRadioOn->SetCheck(BST_CHECKED);
	else
		pRadioOff->SetCheck(BST_CHECKED);

	return TRUE;
}

// 处理背景音乐开启
void CGameSettingDlg::OnBnClickedRadioBgmOn()
{
	m_bBGMusicOn = TRUE;
}

// 处理背景音乐关闭
void CGameSettingDlg::OnBnClickedRadioBgmOff()
{
	m_bBGMusicOn = FALSE;
}

// 处理主题选择变化
void CGameSettingDlg::OnCbnSelchangeComboTheme()
{
	m_nThemeIndex = m_comboTheme.GetCurSel();
}

// 处理地图行数变化
void CGameSettingDlg::OnEnChangeEditRows()
{
	CString strValue;
	m_editMapRows.GetWindowText(strValue);
	m_nMapRows = _ttoi(strValue);
}

// 处理地图列数变化
void CGameSettingDlg::OnEnChangeEditCols()
{
	CString strValue;
	m_editMapCols.GetWindowText(strValue);
	m_nMapCols = _ttoi(strValue);
}

// 处理图片个数变化
void CGameSettingDlg::OnEnChangeEditPicCount()
{
	CString strValue;
	m_editPicCount.GetWindowText(strValue);
	m_nPicCount = _ttoi(strValue);
}

// 处理确定按钮
void CGameSettingDlg::OnBnClickedOk()
{
	// 检查参数合法性
	if (m_nMapRows < 2 || m_nMapCols < 2)
	{
		MessageBox(_T("地图行列数至少为2！"), _T("提示"), MB_OK | MB_ICONWARNING);
		return;
	}

	if (m_nPicCount < 1)
	{
		MessageBox(_T("图片个数至少为1！"), _T("提示"), MB_OK | MB_ICONWARNING);
		return;
	}

	// 检查行列和图片数的关系
	if ((m_nMapRows * m_nMapCols) % (m_nPicCount * 2) != 0)
	{
		MessageBox(_T("行列数与图片个数不匹配！地图格子数必须是图片对数的整数倍。"), _T("提示"), MB_OK | MB_ICONWARNING);
		return;
	}

	// 应用设置 - 使用ParamSetting的静态方法
	ParamSetting::SetMapRow(m_nMapRows);
	ParamSetting::SetMapCol(m_nMapCols);
	ParamSetting::SetPicCount(m_nPicCount);
	ParamSetting::ThemeNo = m_nThemeIndex;
	ParamSetting::BGMusicOn = m_bBGMusicOn;

	// 根据主题设置资源路径
	switch (m_nThemeIndex)
	{
	case 0: // 水果主题
		
		ParamSetting::BGPath = _T("res/fruit_bg.bmp");
		ParamSetting::ElementPath = _T("res/fruit_element.bmp");
		ParamSetting::MaskPath = _T("res/fruit_mask.bmp");
		ParamSetting::ClickSoundPath = _T("sounds/fruit_click.wav");
		ParamSetting::ClearSoundPath = _T("sounds/fruit_clear.wav");
		//ParamSetting::BGMSoundPath = _T("sounds/bgm.mp3");
		break;
	case 1: // 动物主题
		ParamSetting::BGPath = _T("res/cxk_bg.bmp");
		ParamSetting::ElementPath = _T("res/cxk_element.bmp");
		ParamSetting::MaskPath = _T("res/cxk_mask.bmp");
		ParamSetting::ClickSoundPath = _T("sounds/cxk_click.wav");
		ParamSetting::ClearSoundPath = _T("sounds/cxk_clear.wav");
		break;
	case 2: // 卡通主题
		ParamSetting::BGPath = _T("res/mh_bg.bmp");
		ParamSetting::ElementPath = _T("res/mh_element.bmp");
		ParamSetting::MaskPath = _T("res/mh_mask.bmp");
		ParamSetting::ClickSoundPath = _T("sounds/mh_click.wav");
		ParamSetting::ClearSoundPath = _T("sounds/mh_clear.wav");
		break;
	}

	CDialogEx::OnOK();
}