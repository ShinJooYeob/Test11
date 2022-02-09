#pragma once

#include "Base.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

public:
	enum SCREENMODE { TYPE_FULLMODE, TYPE_WINMODE, TYPE_END };
public:
	typedef struct tagGraphicDesc
	{
		SCREENMODE		eScreenMode;
		HWND			hWnd;
		unsigned int	iWinCX, iWinCY; 
	}GRAPHICDESC;

private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;
	// com ��ü : ������ ��� �Ծ��� ���� �ϵ��� �����Ͽ� ���� ����� �����ϴ� ��ü

public:
	LPDIRECT3DDEVICE9			Get_Device(void) { return m_pDevice; }
	LPD3DXSPRITE				Get_Sprite(void) { return m_pSprite; }
	LPD3DXFONT					Get_Font(void) { return m_pFont; }
public:
	HRESULT		InitDevice(const GRAPHICDESC& GraphicDesc, LPDIRECT3DDEVICE9* ppOut);
	void		SetParameters(D3DPRESENT_PARAMETERS& d3dpp, const GRAPHICDESC& GraphicDesc);

	void		Render_Begin(void);
	void		Render_End();

private:
	LPDIRECT3D9				m_p3D;
	LPDIRECT3DDEVICE9		m_pDevice;

	// dx �󿡼� 2d �̹����� ������ִ� com��ü
	LPD3DXSPRITE			m_pSprite;
	LPD3DXFONT				m_pFont;

public:
	virtual void Free() override;
};

END