#pragma once

#define TEMPLATE_CONSTRUCTOR_FORWARD_FLOOD_WITH_INITIALIZER(THISCLASS,MEMBER,INITIALIZER)	\
																																				THISCLASS() :																																														MEMBER() INITIALIZER	\
	template<typename t_param1>																													THISCLASS(const t_param1 & p_param1) :																																								MEMBER(p_param1) INITIALIZER	\
	template<typename t_param1,typename t_param2>																								THISCLASS(const t_param1 & p_param1,const t_param2 & p_param2) :																																	MEMBER(p_param1,p_param2) INITIALIZER	\
	template<typename t_param1,typename t_param2,typename t_param3>																				THISCLASS(const t_param1 & p_param1,const t_param2 & p_param2,const t_param3 & p_param3) :																											MEMBER(p_param1,p_param2,p_param3) INITIALIZER	\
	template<typename t_param1,typename t_param2,typename t_param3,typename t_param4>															THISCLASS(const t_param1 & p_param1,const t_param2 & p_param2,const t_param3 & p_param3,const t_param4 & p_param4) :																				MEMBER(p_param1,p_param2,p_param3,p_param4) INITIALIZER	\
	template<typename t_param1,typename t_param2,typename t_param3,typename t_param4,typename t_param5>											THISCLASS(const t_param1 & p_param1,const t_param2 & p_param2,const t_param3 & p_param3,const t_param4 & p_param4,const t_param5 & p_param5) :														MEMBER(p_param1,p_param2,p_param3,p_param4,p_param5) INITIALIZER	\
	template<typename t_param1,typename t_param2,typename t_param3,typename t_param4,typename t_param5,typename t_param6>						THISCLASS(const t_param1 & p_param1,const t_param2 & p_param2,const t_param3 & p_param3,const t_param4 & p_param4,const t_param5 & p_param5,const t_param6 & p_param6) :							MEMBER(p_param1,p_param2,p_param3,p_param4,p_param5,p_param6) INITIALIZER	\
	template<typename t_param1,typename t_param2,typename t_param3,typename t_param4,typename t_param5,typename t_param6, typename t_param7>	THISCLASS(const t_param1 & p_param1,const t_param2 & p_param2,const t_param3 & p_param3,const t_param4 & p_param4,const t_param5 & p_param5,const t_param6 & p_param6,const t_param7 & p_param7) :	MEMBER(p_param1,p_param2,p_param3,p_param4,p_param5,p_param6,p_param7) INITIALIZER

#define TEMPLATE_CONSTRUCTOR_FORWARD_FLOOD(THISCLASS,MEMBER) TEMPLATE_CONSTRUCTOR_FORWARD_FLOOD_WITH_INITIALIZER(THISCLASS,MEMBER,{})

template<typename BASE_T>
class CUnicodeWnd : public BASE_T
{
public:
	TEMPLATE_CONSTRUCTOR_FORWARD_FLOOD(CUnicodeWnd, BASE_T)

	int GetDlgItemTextW(int nID, CStringW& rString) const
	{
		rString.Empty();

		HWND hWnd = ::GetDlgItem(m_hWnd, nID);
		if (hWnd != NULL)
		{
			int nLen = ::GetWindowTextLengthW(hWnd);
			::GetWindowTextW(hWnd, rString.GetBufferSetLength(nLen), nLen+1);
			rString.ReleaseBuffer();
		}
		return rString.GetLength();
	}
	void SetDlgItemTextW(int nID, LPCWSTR lpszString)
	{
		::SetDlgItemTextW(m_hWnd, nID, lpszString);
	}
};

typedef CUnicodeWnd<CWnd> CWndW;
typedef CUnicodeWnd<CDialog> CDialogW;
typedef CUnicodeWnd<CPropertyPage> CPropertyPageW;
