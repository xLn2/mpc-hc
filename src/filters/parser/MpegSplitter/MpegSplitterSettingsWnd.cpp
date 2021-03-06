/*
 * (C) 2006-2012 see Authors.txt
 *
 * This file is part of MPC-HC.
 *
 * MPC-HC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPC-HC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "stdafx.h"
#include "MpegSplitterSettingsWnd.h"
#include "../../../DSUtil/DSUtil.h"
#include "resource.h"
#include "../../../mpc-hc/InternalFiltersConfig.h"

#define ResStr(id) CString(MAKEINTRESOURCE(id))

CMpegSplitterSettingsWnd::CMpegSplitterSettingsWnd()
{
}

bool CMpegSplitterSettingsWnd::OnConnect(const CInterfaceList<IUnknown, &IID_IUnknown>& pUnks)
{
    ASSERT(!m_pMSF);

    m_pMSF.Release();

    POSITION pos = pUnks.GetHeadPosition();
    while (pos && !(m_pMSF = pUnks.GetNext(pos))) {
        ;
    }

    if (!m_pMSF) {
        return false;
    }

    return true;
}

void CMpegSplitterSettingsWnd::OnDisconnect()
{
    m_pMSF.Release();
}

bool CMpegSplitterSettingsWnd::OnActivate()
{
    ASSERT(IPP_FONTSIZE == 13);
    const int h20 = IPP_SCALE(20);
    const int h25 = IPP_SCALE(25);
    const int h30 = IPP_SCALE(30);
    DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_TABSTOP;
    CPoint p(10, 10);

    m_cbFastStreamChange.Create(ResStr(IDS_MPEGSPLITTER_FSTREAM_CHANGE), dwStyle | BS_AUTOCHECKBOX | BS_LEFTTEXT, CRect(p, CSize(IPP_SCALE(305), m_fontheight)), this, IDC_PP_FAST_STREAM_SELECT);
    p.y += h20;

    m_cbForcedSub.Create(ResStr(IDS_MPEGSPLITTER_SUB_FORCING), dwStyle | BS_AUTOCHECKBOX | BS_LEFTTEXT, CRect(p, CSize(IPP_SCALE(305), m_fontheight)), this, IDC_PP_SUBTITLE_FORCED);
    p.y += h20;

    m_cbTrackPriority.Create(ResStr(IDS_MPEGSPLITTER_TRACKS_ORDER), dwStyle | BS_AUTOCHECKBOX | BS_LEFTTEXT, CRect(p, CSize(IPP_SCALE(305), m_fontheight)), this, IDC_PP_TRACK_PRIORITY);
    p.y += h20;

    m_cbAlternativeDuration.Create(ResStr(IDS_MPEGSPLITTER_ALT_DUR_CALC), dwStyle | BS_AUTOCHECKBOX | BS_LEFTTEXT, CRect(p, CSize(IPP_SCALE(305), m_fontheight)), this, IDC_PP_ALTERNATIVE_DURATION);
    p.y += h20;

    m_txtAudioLanguageOrder.Create(ResStr(IDS_MPEGSPLITTER_LANG_ORDER), WS_VISIBLE | WS_CHILD, CRect(p, CSize(IPP_SCALE(200), m_fontheight)), this, (UINT)IDC_STATIC);
    p.y += h20;
    m_edtAudioLanguageOrder.CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(p, CSize(IPP_SCALE(305), m_fontheight + 6)), this, IDC_PP_AUDIO_LANGUAGE_ORDER);
    p.y += h25;

    m_txtSubtitlesLanguageOrder.Create(ResStr(IDS_MPEGSPLITTER_SUB_ORDER), WS_VISIBLE | WS_CHILD, CRect(p, CSize(IPP_SCALE(200), m_fontheight)), this, (UINT)IDC_STATIC);
    p.y += h20;
    m_edtSubtitlesLanguageOrder.CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), _T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(p, CSize(IPP_SCALE(305), m_fontheight + 6)), this, IDC_PP_SUBTITLES_LANGUAGE_ORDER);
    p.y += h25;

    m_txtVC1_GuidFlag.Create(ResStr(IDS_MPEGSPLITTER_VC1_GUIDFLAG), WS_VISIBLE | WS_CHILD, CRect(p, CSize(IPP_SCALE(200), m_fontheight)), this, (UINT)IDC_STATIC);
    p.y += h20;
    m_cbVC1_GuidFlag.Create(dwStyle | CBS_DROPDOWNLIST | WS_VSCROLL, CRect(p, CSize(IPP_SCALE(305), 200)), this, IDC_PP_VC1_GUIDFLAG);
    m_cbVC1_GuidFlag.AddString(_T("Default"));
    m_cbVC1_GuidFlag.AddString(_T("Cyberlink VC-1 Decoder"));
    m_cbVC1_GuidFlag.AddString(_T("ArcSoft VC-1 Decoder"));
    SetClassLongPtr(GetDlgItem(IDC_PP_VC1_GUIDFLAG)->m_hWnd, GCLP_HCURSOR, (LONG_PTR)AfxGetApp()->LoadStandardCursor(IDC_HAND));
    p.y += h30;

    m_grpTrueHD.Create(ResStr(IDS_MPEGSPLITTER_TRUEHD_OUTPUT), WS_VISIBLE | WS_CHILD | BS_GROUPBOX, CRect(p + CPoint(-5, 0), CSize(IPP_SCALE(305), h20 + h20)), this, (UINT)IDC_STATIC);
    p.y += h20;
    m_cbTrueHD.Create(_T("TrueHD"), dwStyle | BS_AUTORADIOBUTTON | BS_TOP | BS_MULTILINE | WS_GROUP, CRect(p, CSize(IPP_SCALE(95), m_fontheight + 2)), this, IDC_PP_TRUEHD);
    m_cbAC3Core.Create(_T("AC-3 core"), dwStyle | BS_AUTORADIOBUTTON | BS_TOP | BS_MULTILINE, CRect(p + CPoint(IPP_SCALE(100), 0), CSize(IPP_SCALE(95), m_fontheight + 2)), this, IDC_PP_AC3CORE);
    m_cbAsIs.Create(ResStr(IDS_MPEGSPLITTER_THD_NOSPLIT), dwStyle | BS_AUTORADIOBUTTON | BS_TOP | BS_MULTILINE, CRect(p + CPoint(IPP_SCALE(200), 0), CSize(IPP_SCALE(95), m_fontheight + 2)), this, IDC_PP_ASIS);

    if (m_pMSF) {
        m_cbFastStreamChange.SetCheck(m_pMSF->GetFastStreamChange());
        m_cbForcedSub.SetCheck(m_pMSF->GetForcedSub());
        m_cbTrackPriority.SetCheck(m_pMSF->GetTrackPriority());
        m_edtAudioLanguageOrder.SetWindowText(m_pMSF->GetAudioLanguageOrder());
        m_edtSubtitlesLanguageOrder.SetWindowText(m_pMSF->GetSubtitlesLanguageOrder());
        m_cbVC1_GuidFlag.SetCurSel(m_pMSF->GetVC1_GuidFlag() - 1);
        m_cbTrueHD.SetCheck(m_pMSF->GetTrueHD() == 0);
        m_cbAC3Core.SetCheck(m_pMSF->GetTrueHD() == 1);
        m_cbAsIs.SetCheck(!m_cbTrueHD.GetCheck() && !m_cbAC3Core.GetCheck());
        m_cbAlternativeDuration.SetCheck(m_pMSF->GetAlternativeDuration());
    }

#ifndef STANDALONE_FILTER
    m_edtAudioLanguageOrder.EnableWindow(FALSE);
    m_edtSubtitlesLanguageOrder.EnableWindow(FALSE);
#endif


    for (CWnd* pWnd = GetWindow(GW_CHILD); pWnd; pWnd = pWnd->GetNextWindow()) {
        pWnd->SetFont(&m_font, FALSE);
    }

    return true;
}

void CMpegSplitterSettingsWnd::OnDeactivate()
{
}

bool CMpegSplitterSettingsWnd::OnApply()
{
    OnDeactivate();

    if (m_pMSF) {
        m_pMSF->SetFastStreamChange(m_cbFastStreamChange.GetCheck());
        m_pMSF->SetForcedSub(m_cbForcedSub.GetCheck());
        m_pMSF->SetTrackPriority(m_cbTrackPriority.GetCheck());
        m_pMSF->SetVC1_GuidFlag(m_cbVC1_GuidFlag.GetCurSel() + 1);
        m_pMSF->SetTrueHD(m_cbTrueHD.GetCheck() ? 0 : m_cbAC3Core.GetCheck() ? 1 : 2);
        m_pMSF->SetAlternativeDuration(m_cbAlternativeDuration.GetCheck());

#ifdef STANDALONE_FILTER
        CString str = _T("");
        m_edtAudioLanguageOrder.GetWindowText(str);
        m_pMSF->SetAudioLanguageOrder(str.GetBuffer());
        m_edtSubtitlesLanguageOrder.GetWindowText(str);
        m_pMSF->SetSubtitlesLanguageOrder(str.GetBuffer());
#endif
        m_pMSF->Apply();
    }

    return true;
}

BEGIN_MESSAGE_MAP(CMpegSplitterSettingsWnd, CInternalPropertyPageWnd)
END_MESSAGE_MAP()
