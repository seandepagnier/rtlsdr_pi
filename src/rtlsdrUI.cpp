///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "rtlsdrUI.h"

///////////////////////////////////////////////////////////////////////////

rtlsdrDialogBase::rtlsdrDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxSize( -1,-1 ) );
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer8->AddGrowableCol( 0 );
	fgSizer8->AddGrowableRow( 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbEnabled = new wxCheckBox( this, wxID_ANY, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( m_cbEnabled, 0, wxALL, 5 );
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Messages") ), wxVERTICAL );
	
	m_tMessages = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	sbSizer10->Add( m_tMessages, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer25;
	fgSizer25 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer25->SetFlexibleDirection( wxBOTH );
	fgSizer25->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bClear = new wxButton( this, wxID_ANY, _("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer25->Add( m_bClear, 0, wxALL, 5 );
	
	m_bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer25->Add( m_bClose, 0, wxALL, 5 );
	
	
	sbSizer10->Add( fgSizer25, 1, wxEXPAND, 5 );
	
	
	fgSizer8->Add( sbSizer10, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer8 );
	this->Layout();
	fgSizer8->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_cbEnabled->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnEnable ), NULL, this );
	m_bClear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnClear ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnClose ), NULL, this );
}

rtlsdrDialogBase::~rtlsdrDialogBase()
{
	// Disconnect Events
	m_cbEnabled->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnEnable ), NULL, this );
	m_bClear->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnClear ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnClose ), NULL, this );
	
}

rtlsdrPrefsBase::rtlsdrPrefsBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer24->SetFlexibleDirection( wxBOTH );
	fgSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText29 = new wxStaticText( this, wxID_ANY, _("Sample Rate (khz)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText29->Wrap( -1 );
	fgSizer24->Add( m_staticText29, 0, wxALL, 5 );
	
	m_sSampleRate = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 16384, 256 );
	fgSizer24->Add( m_sSampleRate, 0, wxALL, 5 );
	
	
	fgSizer12->Add( fgSizer24, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Error Correction") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText30 = new wxStaticText( this, wxID_ANY, _("Error Correction"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer10->Add( m_staticText30, 0, wxALL, 5 );
	
	m_sError = new wxSpinCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -256, 256, 50 );
	fgSizer10->Add( m_sError, 0, wxALL, 5 );
	
	m_bLaunchCompanion = new wxButton( this, wxID_ANY, _("Launch gnuradio-companion"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_bLaunchCompanion, 0, wxALL, 5 );
	
	m_bInfo = new wxButton( this, wxID_ANY, _("Info"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_bInfo, 0, wxALL, 5 );
	
	
	sbSizer9->Add( fgSizer10, 1, wxEXPAND, 5 );
	
	
	fgSizer12->Add( sbSizer9, 1, wxEXPAND, 5 );
	
	
	bSizer2->Add( fgSizer12, 1, wxEXPAND, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	
	bSizer2->Add( m_sdbSizer1, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );
	
	
	this->SetSizer( bSizer2 );
	this->Layout();
	bSizer2->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bLaunchCompanion->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnLaunchGnuRadioCompanion ), NULL, this );
	m_bInfo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnInfo ), NULL, this );
}

rtlsdrPrefsBase::~rtlsdrPrefsBase()
{
	// Disconnect Events
	m_bLaunchCompanion->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnLaunchGnuRadioCompanion ), NULL, this );
	m_bInfo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnInfo ), NULL, this );
	
}
