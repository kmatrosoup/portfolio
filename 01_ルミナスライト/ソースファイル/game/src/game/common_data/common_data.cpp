#include "common_data.h"

const std::string CCommonData::m_savedata_filename = "data\\savedata.txt";

CCommonData::CCommonData(IGameObject* parent)
	: IGameObject(parent, "CommonData")
	, m_BGParam_R(0.0f)
	, m_BGParam_G(0.0f)
	, m_BGParam_B(0.0f)
	, m_SelectedLevel(1)
{
}

const std::string& CCommonData::GetSaveDataFileName() const
{
	return m_savedata_filename;
}

void CCommonData::SetBGParamR(float param)
{
	m_BGParam_R = param;
}

void CCommonData::SetBGParamG(float param)
{
	m_BGParam_G = param;
}

void CCommonData::SetBGParamB(float param)
{
	m_BGParam_B = param;
}

float CCommonData::GetBGParamR() const
{
	return m_BGParam_R;
}

float CCommonData::GetBGParamG() const
{
	return m_BGParam_G;
}

float CCommonData::GetBGParamB() const
{
	return m_BGParam_B;
}

void CCommonData::SetSelectedLevel(int level)
{
	m_SelectedLevel = level;
}

int CCommonData::GetSelectedLevel() const
{
	return m_SelectedLevel;
}
