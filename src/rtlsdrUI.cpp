///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May 19 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
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
	
	wxStaticBoxSizer* sbSizer10;
	sbSizer10 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Messages") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer13;
	fgSizer13 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer13->AddGrowableCol( 0 );
	fgSizer13->AddGrowableRow( 0 );
	fgSizer13->SetFlexibleDirection( wxBOTH );
	fgSizer13->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_tMessages = new wxTextCtrl( sbSizer10->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 420,70 ), wxTE_MULTILINE|wxTE_READONLY );
	fgSizer13->Add( m_tMessages, 0, wxALL|wxEXPAND, 5 );
	
	
	sbSizer10->Add( fgSizer13, 1, wxEXPAND, 5 );
	
	
	fgSizer8->Add( sbSizer10, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer25;
	fgSizer25 = new wxFlexGridSizer( 0, 4, 0, 0 );
	fgSizer25->SetFlexibleDirection( wxBOTH );
	fgSizer25->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbEnabled = new wxCheckBox( this, wxID_ANY, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer25->Add( m_cbEnabled, 0, wxALL, 5 );
	
	m_bPreferences = new wxButton( this, wxID_ANY, _("Preferences"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer25->Add( m_bPreferences, 0, wxALL, 5 );
	
	m_bClear = new wxButton( this, wxID_ANY, _("Clear"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer25->Add( m_bClear, 0, wxALL, 5 );
	
	m_bClose = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer25->Add( m_bClose, 0, wxALL, 5 );
	
	
	fgSizer8->Add( fgSizer25, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer8 );
	this->Layout();
	fgSizer8->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_cbEnabled->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnEnable ), NULL, this );
	m_bPreferences->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnPreferences ), NULL, this );
	m_bClear->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnClear ), NULL, this );
	m_bClose->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnClose ), NULL, this );
}

rtlsdrDialogBase::~rtlsdrDialogBase()
{
	// Disconnect Events
	m_cbEnabled->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnEnable ), NULL, this );
	m_bPreferences->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnPreferences ), NULL, this );
	m_bClear->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnClear ), NULL, this );
	m_bClose->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrDialogBase::OnClose ), NULL, this );
	
}

FlightsDialogBase::FlightsDialogBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer17;
	fgSizer17 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer17->AddGrowableCol( 0 );
	fgSizer17->AddGrowableRow( 0 );
	fgSizer17->SetFlexibleDirection( wxBOTH );
	fgSizer17->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_lFlights = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_HRULES|wxLC_REPORT );
	fgSizer17->Add( m_lFlights, 0, wxALL|wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer20;
	fgSizer20 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer20->AddGrowableCol( 2 );
	fgSizer20->SetFlexibleDirection( wxBOTH );
	fgSizer20->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_stConnected = new wxStaticText( this, wxID_ANY, _("N/A"), wxDefaultPosition, wxSize( 180,-1 ), 0 );
	m_stConnected->Wrap( -1 );
	fgSizer20->Add( m_stConnected, 0, wxALL|wxEXPAND, 5 );
	
	m_bGoto = new wxButton( this, wxID_ANY, _("Goto"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer20->Add( m_bGoto, 0, wxALL, 5 );
	
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2->Realize();
	
	fgSizer20->Add( m_sdbSizer2, 1, wxEXPAND, 5 );
	
	
	fgSizer17->Add( fgSizer20, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer17 );
	this->Layout();
	fgSizer17->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_bGoto->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlightsDialogBase::OnGoto ), NULL, this );
}

FlightsDialogBase::~FlightsDialogBase()
{
	// Disconnect Events
	m_bGoto->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FlightsDialogBase::OnGoto ), NULL, this );
	
}

rtlsdrPrefsBase::rtlsdrPrefsBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer14;
	fgSizer14 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer14->AddGrowableCol( 0 );
	fgSizer14->SetFlexibleDirection( wxBOTH );
	fgSizer14->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cMode = new wxChoicebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxCHB_DEFAULT );
	m_panel1 = new wxPanel( m_cMode, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer12;
	fgSizer12 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer12->AddGrowableCol( 0 );
	fgSizer12->SetFlexibleDirection( wxBOTH );
	fgSizer12->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxFlexGridSizer* fgSizer121;
	fgSizer121 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer121->SetFlexibleDirection( wxBOTH );
	fgSizer121->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText5 = new wxStaticText( m_panel1, wxID_ANY, _("Program"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer121->Add( m_staticText5, 0, wxALL, 5 );
	
	wxString m_cAISProgramChoices[] = { _("builtin rtl_ais"), _("rtl_ais"), _("rtl_fm | aisdecoder"), _("soft_fm | aisdecoder"), _("ais_rx  (gnuradio)") };
	int m_cAISProgramNChoices = sizeof( m_cAISProgramChoices ) / sizeof( wxString );
	m_cAISProgram = new wxChoice( m_panel1, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cAISProgramNChoices, m_cAISProgramChoices, 0 );
	m_cAISProgram->SetSelection( 0 );
	fgSizer121->Add( m_cAISProgram, 0, wxALL, 5 );
	
	
	fgSizer12->Add( fgSizer121, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer71;
	sbSizer71 = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, _("extra args (blank default)") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer15;
	fgSizer15 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer15->AddGrowableCol( 1 );
	fgSizer15->SetFlexibleDirection( wxBOTH );
	fgSizer15->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_stP1args = new wxStaticText( sbSizer71->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_stP1args->Wrap( -1 );
	fgSizer15->Add( m_stP1args, 0, wxALL, 5 );
	
	m_tP1args = new wxTextCtrl( sbSizer71->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer15->Add( m_tP1args, 0, wxALL|wxEXPAND, 5 );
	
	m_stP2args = new wxStaticText( sbSizer71->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_stP2args->Wrap( -1 );
	fgSizer15->Add( m_stP2args, 0, wxALL, 5 );
	
	m_tP2args = new wxTextCtrl( sbSizer71->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer15->Add( m_tP2args, 0, wxALL|wxEXPAND, 5 );
	
	
	sbSizer71->Add( fgSizer15, 1, wxEXPAND, 5 );
	
	
	fgSizer12->Add( sbSizer71, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer24;
	fgSizer24 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer24->SetFlexibleDirection( wxBOTH );
	fgSizer24->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText29 = new wxStaticText( m_panel1, wxID_ANY, _("Sample Rate (khz)"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText29->Wrap( -1 );
	fgSizer24->Add( m_staticText29, 0, wxALL, 5 );
	
	m_sAISSampleRate = new wxSpinCtrl( m_panel1, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 16384, 251 );
	fgSizer24->Add( m_sAISSampleRate, 0, wxALL, 5 );
	
	
	fgSizer12->Add( fgSizer24, 1, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer9;
	sbSizer9 = new wxStaticBoxSizer( new wxStaticBox( m_panel1, wxID_ANY, _("Error Correction Calibration") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer10;
	fgSizer10 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer10->SetFlexibleDirection( wxBOTH );
	fgSizer10->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText30 = new wxStaticText( sbSizer9->GetStaticBox(), wxID_ANY, _("Error Correction"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText30->Wrap( -1 );
	fgSizer10->Add( m_staticText30, 0, wxALL, 5 );
	
	m_sAISError = new wxSpinCtrl( sbSizer9->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -256, 256, 50 );
	fgSizer10->Add( m_sAISError, 0, wxALL, 5 );
	
	m_button7 = new wxButton( sbSizer9->GetStaticBox(), wxID_ANY, _("Auto Calibrate"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_button7, 0, wxALL, 5 );
	
	m_bLaunchCompanion = new wxButton( sbSizer9->GetStaticBox(), wxID_ANY, _("Manual via grc"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_bLaunchCompanion, 0, wxALL, 5 );
	
	m_bInfo = new wxButton( sbSizer9->GetStaticBox(), wxID_ANY, _("Info"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer10->Add( m_bInfo, 0, wxALL, 5 );
	
	
	sbSizer9->Add( fgSizer10, 1, wxEXPAND, 5 );
	
	
	fgSizer12->Add( sbSizer9, 1, wxEXPAND, 5 );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	
	fgSizer12->Add( fgSizer11, 1, wxEXPAND, 5 );
	
	
	m_panel1->SetSizer( fgSizer12 );
	m_panel1->Layout();
	fgSizer12->Fit( m_panel1 );
	m_cMode->AddPage( m_panel1, _("AIS"), false );
	m_panel2 = new wxPanel( m_cMode, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sbSizer81 = new wxStaticBoxSizer( new wxStaticBox( m_panel2, wxID_ANY, _("Plotting Planes (ADSB)") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer18;
	fgSizer18 = new wxFlexGridSizer( 0, 1, 0, 0 );
	fgSizer18->AddGrowableCol( 0 );
	fgSizer18->SetFlexibleDirection( wxBOTH );
	fgSizer18->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_cbEnableFlights = new wxCheckBox( sbSizer81->GetStaticBox(), wxID_ANY, _("Enable"), wxDefaultPosition, wxDefaultSize, 0 );
	m_cbEnableFlights->SetValue(true); 
	fgSizer18->Add( m_cbEnableFlights, 0, wxALL, 5 );
	
	wxFlexGridSizer* fgSizer19;
	fgSizer19 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer19->AddGrowableCol( 1 );
	fgSizer19->SetFlexibleDirection( wxBOTH );
	fgSizer19->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText10 = new wxStaticText( sbSizer81->GetStaticBox(), wxID_ANY, _("dump1090 Server"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	fgSizer19->Add( m_staticText10, 0, wxALL, 5 );
	
	m_tDump1090Server = new wxTextCtrl( sbSizer81->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer19->Add( m_tDump1090Server, 0, wxALL|wxEXPAND, 5 );
	
	m_bAboutDump109Server = new wxButton( sbSizer81->GetStaticBox(), wxID_ANY, _("?"), wxDefaultPosition, wxSize( 30,-1 ), 0 );
	fgSizer19->Add( m_bAboutDump109Server, 0, wxALL, 5 );
	
	
	fgSizer18->Add( fgSizer19, 1, wxEXPAND, 5 );
	
	
	sbSizer81->Add( fgSizer18, 1, wxEXPAND, 5 );
	
	
	m_panel2->SetSizer( sbSizer81 );
	m_panel2->Layout();
	sbSizer81->Fit( m_panel2 );
	m_cMode->AddPage( m_panel2, _("ADSB"), false );
	m_panel3 = new wxPanel( m_cMode, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_tFMFrequency = new wxTextCtrl( m_panel3, wxID_ANY, _("100.4"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( m_tFMFrequency, 0, wxALL, 5 );
	
	m_staticText4 = new wxStaticText( m_panel3, wxID_ANY, _("Mhz"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer8->Add( m_staticText4, 0, wxALL, 5 );
	
	
	m_panel3->SetSizer( fgSizer8 );
	m_panel3->Layout();
	fgSizer8->Fit( m_panel3 );
	m_cMode->AddPage( m_panel3, _("FM Radio"), false );
	m_panel4 = new wxPanel( m_cMode, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgSizer9;
	fgSizer9 = new wxFlexGridSizer( 0, 3, 0, 0 );
	fgSizer9->SetFlexibleDirection( wxBOTH );
	fgSizer9->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_tVHFChannel = new wxTextCtrl( m_panel4, wxID_ANY, _("16"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_tVHFChannel, 0, wxALL, 5 );
	
	m_cbVHFWX = new wxCheckBox( m_panel4, wxID_ANY, _("WX"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer9->Add( m_cbVHFWX, 0, wxALL, 5 );
	
	m_staticText7 = new wxStaticText( m_panel4, wxID_ANY, _("Squelch"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer9->Add( m_staticText7, 0, wxALL, 5 );
	
	m_sVHFSquelch = new wxSpinCtrl( m_panel4, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 29 );
	fgSizer9->Add( m_sVHFSquelch, 0, wxALL, 5 );
	
	wxString m_cVHFSetChoices[] = { _("International"), _("US") };
	int m_cVHFSetNChoices = sizeof( m_cVHFSetChoices ) / sizeof( wxString );
	m_cVHFSet = new wxChoice( m_panel4, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_cVHFSetNChoices, m_cVHFSetChoices, 0 );
	m_cVHFSet->SetSelection( 1 );
	fgSizer9->Add( m_cVHFSet, 0, wxALL, 5 );
	
	
	m_panel4->SetSizer( fgSizer9 );
	m_panel4->Layout();
	fgSizer9->Fit( m_panel4 );
	m_cMode->AddPage( m_panel4, _("VHF Audio"), false );
	fgSizer14->Add( m_cMode, 1, wxEXPAND | wxALL, 5 );
	
	wxFlexGridSizer* fgSizer101;
	fgSizer101 = new wxFlexGridSizer( 1, 0, 0, 0 );
	fgSizer101->AddGrowableCol( 2 );
	fgSizer101->SetFlexibleDirection( wxBOTH );
	fgSizer101->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bAboutAuthor = new wxButton( this, wxID_ANY, _("About Author"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer101->Add( m_bAboutAuthor, 0, wxALL, 5 );
	
	m_button8 = new wxButton( this, wxID_ANY, _("Information"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer101->Add( m_button8, 0, wxALL, 5 );
	
	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();
	
	fgSizer101->Add( m_sdbSizer1, 0, wxBOTTOM|wxEXPAND|wxRIGHT|wxTOP, 5 );
	
	
	fgSizer14->Add( fgSizer101, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer14 );
	this->Layout();
	fgSizer14->Fit( this );
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_cAISProgram->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( rtlsdrPrefsBase::OnAISProgram ), NULL, this );
	m_button7->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnAutoCalibrate ), NULL, this );
	m_bLaunchCompanion->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnLaunchGnuRadioCompanion ), NULL, this );
	m_bInfo->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnInfo ), NULL, this );
	m_bAboutDump109Server->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnAboutDump1090Server ), NULL, this );
	m_bAboutAuthor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnAboutAuthor ), NULL, this );
	m_button8->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnInformation ), NULL, this );
}

rtlsdrPrefsBase::~rtlsdrPrefsBase()
{
	// Disconnect Events
	m_cAISProgram->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( rtlsdrPrefsBase::OnAISProgram ), NULL, this );
	m_button7->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnAutoCalibrate ), NULL, this );
	m_bLaunchCompanion->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnLaunchGnuRadioCompanion ), NULL, this );
	m_bInfo->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnInfo ), NULL, this );
	m_bAboutDump109Server->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnAboutDump1090Server ), NULL, this );
	m_bAboutAuthor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnAboutAuthor ), NULL, this );
	m_button8->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( rtlsdrPrefsBase::OnInformation ), NULL, this );
	
}
