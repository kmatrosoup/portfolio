#pragma once

#include "aqua.h"
#include <string>

class CCommonData : public aqua::IGameObject
{
public:
	CCommonData(IGameObject* parent);
	~CCommonData() override = default;

	const std::string& GetSaveDataFileName() const;
	void SetBGParamR(float param);
	void SetBGParamG(float param);
	void SetBGParamB(float param);
	float GetBGParamR() const;
	float GetBGParamG() const;
	float GetBGParamB() const;
	void SetSelectedLevel(int level);
	int GetSelectedLevel() const;

private:
	static const std::string m_savedata_filename;	// ƒZ[ƒuƒf[ƒ^‚Ìƒtƒ@ƒCƒ‹–¼

	float m_BGParam_R;		// ”wŒi‚ÌR’l( 0.0f`1.0f )
	float m_BGParam_G;		// ”wŒi‚ÌG’l( 0.0f`1.0f )
	float m_BGParam_B;		// ”wŒi‚ÌB’l( 0.0f`1.0f )
	int m_SelectedLevel;	// ‘I‘ğƒŒƒxƒ‹
};
