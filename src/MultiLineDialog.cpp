#include "MultiLineDialog.h"
#include "wx/accel.h"
#include <wx/msgdlg.h>
#include <analysis.h>
#include "valDialogs.cpp"
#include "valControls.cpp"



// --------------------------------------------------------------------------------------------------------------------------





// ----------------------------- FunctionColorDialog-----------------------------------------------------------------------------------------------------




FunctionColorDialog::FunctionColorDialog(wxWindow *parent,const wxColour& color,const wxColour& backgroundcolor
                                         ,const int linewidth) : Color(color), BackgroundColor(backgroundcolor), LineWidth(linewidth)
{
    Create(parent,wxID_ANY,_T("Choose color and width"));
    //
    wxBoxSizer *TopSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *ToolSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *PreviewSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *ColorSizer = new wxBoxSizer(wxVERTICAL);
    //
    wxButton *CancelButton = new wxButton(this,101,_T("Cancel"));
    wxButton *OKButton = new wxButton(this,102,_T("OK"));
    ButtonSizer->Add(CancelButton,1,wxALL,5);
    ButtonSizer->Add(OKButton,1,wxALL,5);
    //
    wxStaticText *text = new wxStaticText(this,11,_T("Preview:"));
    PreviewSizer->Add(text,0,wxALL|wxALIGN_LEFT);
    DrawPanel = new wxPanel(this,12,wxDefaultPosition,wxSize(sx,sy));
    DrawPanel->SetBackgroundColour(wxColour(255,255,255));
    PreviewSizer->AddSpacer(20);
    PreviewSizer->Add(DrawPanel,0,wxALL | wxALIGN_LEFT);
    ToolSizer->Add(PreviewSizer,0,wxALL|wxALIGN_LEFT,5);
    ToolSizer->AddSpacer(10);
    //
    wxStaticText *text2 = new wxStaticText(this,13,_T("Width/Color:"));
    ColorSizeCtrl = new wxSpinCtrl(this,14,val::ToString(LineWidth));
    //ColorPicker = new wxColourPickerCtrl(this,15,Color);
    //ColorPicker->SetToolTip(_T("Click to change color!"));
    ColorButton = new wxButton(this,15);
    ColorButton->SetBackgroundColour(Color);
    ColorButton->SetToolTip(_T("Click to change color!"));
    ColorSizer->Add(text2,0,wxALL,0);
    ColorSizer->Add(ColorSizeCtrl,0,wxALL,2);
    ColorSizer->Add(ColorButton,0,wxALL|wxEXPAND,2);
    ToolSizer->Add(ColorSizer,0,wxALL,5);
    //
    TopSizer->Add(ToolSizer,0,wxALL|wxALIGN_CENTER);
    TopSizer->Add(ButtonSizer,0,wxALL|wxALIGN_RIGHT);
	SetSizer(TopSizer);
	TopSizer->Fit(this);
	TopSizer->SetSizeHints(this);
    //
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FunctionColorDialog::OnButtonClick,this,101);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FunctionColorDialog::OnButtonClick,this,102);
	DrawPanel->Bind(wxEVT_PAINT,&FunctionColorDialog::OnDrawPanelPaint,this);
	//Bind(wxEVT_COLOURPICKER_CHANGED,&FunctionColorDialog::OnColorChanged,this);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FunctionColorDialog::OnColorButton,this,15);
	Bind(wxEVT_SPINCTRL,&FunctionColorDialog::OnWidthChanged,this);
	//
	ColorData = new wxColourData();
	//
    wxAcceleratorEntry entries[3];

    entries[0].Set(wxACCEL_CTRL, (int) 'O',102);
    entries[1].Set(wxACCEL_NORMAL,WXK_ESCAPE,101);
    entries[2].Set(wxACCEL_CTRL,WXK_RETURN,102);
    wxAcceleratorTable accel(3,entries);
    SetAcceleratorTable(accel);
    Paint();
}



FunctionColorDialog::~FunctionColorDialog()
{
    delete ColorData;
}

void FunctionColorDialog::OnButtonClick(wxCommandEvent& event)
{
    if (event.GetId()==101) EndModal(wxID_CANCEL);
    else EndModal(wxID_OK);
}

void FunctionColorDialog::Paint() const
{
    wxClientDC dc(DrawPanel);
    dc.SetBackground(BackgroundColor);
    dc.Clear();
    dc.SetPen(wxPen(Color,LineWidth));
    dc.DrawLine(10,sy/2,sx-10,sy/2);
}

void FunctionColorDialog::OnDrawPanelPaint(wxPaintEvent&)
{
    Paint();
}

/*
void FunctionColorDialog::OnColorChanged(wxColourPickerEvent&)
{
    Color=ColorPicker->GetColour();
    Paint();
}
*/

void FunctionColorDialog::OnColorButton(wxCommandEvent&)
{
    //wxColourData data; //= new wxColourData();
    ColorData->SetColour(Color);
    wxColourDialog dialog(this,ColorData);
    if (dialog.ShowModal()==wxID_OK) {
        Color = dialog.GetColourData().GetColour();
        ColorButton->SetBackgroundColour(Color);
        ColorButton->Refresh();
        Paint();
    }
}

void FunctionColorDialog::OnWidthChanged(wxSpinEvent&)
{
    LineWidth=ColorSizeCtrl->GetValue();
    Paint();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------


// -------------------------------------------TextFontsDialog----------------------------------------------------------------------------------


TextFontsDialog::TextFontsDialog(wxWindow * parent,const wxFont& textfont,const wxColour& color) : Color(color) , TextFont(textfont)
{
    Create(parent,wxID_ANY,_T("Choose Font and Color"));
    //
    wxBoxSizer *TopSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *ToolSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *PreviewSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *ColorSizer = new wxBoxSizer(wxVERTICAL);
    //
    //
    wxButton *CancelButton = new wxButton(this,101,_T("Cancel"));
    wxButton *OKButton = new wxButton(this,102,_T("OK"));
    ButtonSizer->Add(CancelButton,1,wxALL,5);
    ButtonSizer->Add(OKButton,1,wxALL,5);
    //
    //
    wxStaticText *text = new wxStaticText(this,11,_T("Preview:"));
    PreviewSizer->Add(text,0,wxALL|wxALIGN_LEFT,5);
    PreviewText = new wxTextCtrl(this,12,wxEmptyString,wxDefaultPosition,wxSize(200,40));
    PreviewText->SetBackgroundColour(*wxWHITE);
    PreviewSizer->AddSpacer(15);
    PreviewSizer->Add(PreviewText,0,wxALL|wxALIGN_LEFT);
    ToolSizer->Add(PreviewSizer,0,wxALL|wxALIGN_LEFT,5);
    ToolSizer->AddSpacer(8);
    //
    wxStaticText *text2 = new wxStaticText(this,13,_T(" Font/Color:"));
    FontPicker = new wxFontPickerCtrl(this,14,TextFont,wxDefaultPosition,wxSize(220,40));
    FontPicker->SetToolTip(_T("Click to change font"));
    //ColorPicker = new wxColourPickerCtrl(this,15,Color);
    //ColorPicker->SetToolTip(_T("Click to change color"));
    ColorButton = new wxButton(this,15);
    ColorButton->SetBackgroundColour(Color);
    ColorButton->SetToolTip(_T("Click to change color!"));
    ColorSizer->Add(text2,0,wxALL,0);
    ColorSizer->Add(FontPicker,0,wxALL,2);
    ColorSizer->Add(ColorButton,0,wxALL|wxEXPAND,2);
    ToolSizer->Add(ColorSizer,0,wxALL,5);
    //
    TopSizer->Add(ToolSizer,0,wxALL|wxALIGN_CENTER);
    TopSizer->AddSpacer(10);
    TopSizer->Add(ButtonSizer,0,wxALL|wxALIGN_RIGHT);
    SetSizer(TopSizer);
	TopSizer->Fit(this);
	TopSizer->SetSizeHints(this);
    //
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&TextFontsDialog::OnButtonClick,this,101);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&TextFontsDialog::OnButtonClick,this,102);
	//Bind(wxEVT_COLOURPICKER_CHANGED,&TextFontsDialog::OnColorChanged,this);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&TextFontsDialog::OnColorButton,this,15);
	Bind(wxEVT_FONTPICKER_CHANGED,&TextFontsDialog::OnFontChanged,this);
	//
    ColorData = new wxColourData();
	//
    wxAcceleratorEntry entries[3];

    entries[0].Set(wxACCEL_CTRL, (int) 'O',102);
    entries[1].Set(wxACCEL_NORMAL,WXK_ESCAPE,101);
    entries[2].Set(wxACCEL_CTRL,WXK_RETURN,102);
    wxAcceleratorTable accel(3,entries);
    SetAcceleratorTable(accel);
    //
    Write();
}


TextFontsDialog::~TextFontsDialog()
{

}

void TextFontsDialog::OnButtonClick(wxCommandEvent& event)
{
    if (event.GetId()==101) EndModal(wxID_CANCEL);
    else EndModal(wxID_OK);
}

/*
void TextFontsDialog::OnColorChanged(wxColourPickerEvent&)
{
    Color=ColorPicker->GetColour();
    Write();
}
*/

void TextFontsDialog::OnColorButton(wxCommandEvent&)
{
    //wxColourData data; //= new wxColourData();
    ColorData->SetColour(Color);
    wxColourDialog dialog(this,ColorData);
    if (dialog.ShowModal()==wxID_OK) {
        Color = dialog.GetColourData().GetColour();
        ColorButton->SetBackgroundColour(Color);
        ColorButton->Refresh();
        Write();
    }
}


void  TextFontsDialog::OnFontChanged(wxFontPickerEvent&)
{
    TextFont = FontPicker->GetSelectedFont();
    Write();
}

void TextFontsDialog::Write() const
{
    PreviewText->SetForegroundColour(Color);
    PreviewText->SetFont(TextFont);
    PreviewText->SetValue(_T("This is a sample text!"));
}

// ----------------------------------------------------------------------------------------------------------------------------------------------


//  -----------------------------------------------------------------------------------------------

wxDEFINE_EVENT(P_EVENT,ParentEvent);

// --------------------------------------------------------------------------------------------


AnalysisDialog::AnalysisDialog(wxWindow *parent,int &nchild,const val::d_array<std::string> &output,const val::d_array<val::d_array<val::GPair<double>>> &Point,
                                const wxSize &Size,const wxPoint &Pos,int fonts) : Parent(parent), N_child(&nchild) , Points(&Point), fontsize(fonts)
{
    Create(parent, wxID_ANY,_T("Analyze function"), wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER|wxCAPTION, _T("wxID_ANY"));
    ++(*N_child);
    Move(Pos);
    int n_point=Point.length(),i,n_output=output.length();
    if (n_output!=4 || n_point!=3) return;

    surface = new wxPanel(this,100);

    TextEdit = val::d_array<wxTextCtrl*>(nullptr,n_output);
    val::d_array<wxBoxSizer*> BoxSizerText(nullptr,n_output);
    val::d_array<wxBoxSizer*> BoxSizerButton(nullptr,n_point);
    wxStaticText* Text[3];
    //val::d_array<wxButton*> Button(nullptr,n_point);
    wxBoxSizer *BoxSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *UpBoxSizer = new wxBoxSizer(wxVERTICAL);

    UpBoxSizer->Add(surface,1,wxALL | wxEXPAND, 0);
    wxFont font;

    for (i=0;i<n_output;++i) {
        TextEdit[i] = new wxTextCtrl(surface,1000,output[i], wxDefaultPosition,Size, wxTE_MULTILINE|wxVSCROLL|wxHSCROLL|wxTE_READONLY);
        font = TextEdit[i]->GetFont();
        font.SetPointSize(fontsize);
        TextEdit[i]->SetFont(font);
        TextEdit[i]->SetInsertionPoint(0);
        BoxSizerText[i] = new wxBoxSizer(wxVERTICAL);
        //BoxSizerText[i]->Add(TextEdit[i],1,wxALL|wxEXPAND,5);
    }
    Text[0] = new wxStaticText(surface,11,_T("Zeros:"));
    Text[1] = new wxStaticText(surface,11,_T("Extrema:"));
    Text[2] = new wxStaticText(surface,11,_T("Inflection Points:"));
    //
    /*
    for (i=1;i<n_output;++i) {
        BoxSizerText[i]->Add(Text[i-1],0,wxALL|wxALIGN_LEFT,5);
    }
    */
    for (i=0;i<n_point;++i) {
        BoxSizerButton[i] = new wxBoxSizer(wxHORIZONTAL);
        BoxSizerButton[i]->Add(Text[i],1,wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,0);
    }
    for (i=0;i<n_output;++i) BoxSizerText[i]->Add(TextEdit[i],1,wxALL|wxEXPAND,5);

    for (i=0;i<n_point;++i) {
        if (!Point[i].isempty()) {
            //BoxSizerButton[i] = new wxBoxSizer(wxVERTICAL);
            //Button[i] = new wxButton(this,2000+i,_T("Show/Hide"));
            //BoxSizerButton[i]->Add(Button[i],1,wxALL,0);
            //checkbox[i] = new wxCheckBox(this,2000+i,_T("Show"));
            checkbox[i] = new val::SwitchCtrl(surface,2000+i);
            checkbox[i]->SetValue(false);
            BoxSizerButton[i]->AddStretchSpacer(1);
            BoxSizerButton[i]->Add(checkbox[i],0,wxALL,5);
            //BoxSizerButton[i]->Add(checkbox[i],1,wxALL,0);
            //Bind(wxEVT_CHECKBOX,&AnalysisDialog::OnButtonClicked,this,2000+i);
            Bind(val_EVENT_SWITCH,&AnalysisDialog::OnButtonClicked,this,2000+i);
#ifdef _WIN32
            Bind(wxEVT_COMMAND_MENU_SELECTED,&AnalysisDialog::OnShortCuts,this,4000+i);
#else
            Bind(wxEVT_COMMAND_BUTTON_CLICKED,&AnalysisDialog::OnShortCuts,this,4000+i);
#endif // _WIN32
        }
    }


    for (i=0;i<n_output;++i) {
        BoxSizer->Add(BoxSizerText[i],1,wxALL|wxEXPAND,5);
        /*
        if (i>0 && BoxSizerButton[i-1]!=nullptr) {
            BoxSizer->Add(BoxSizerButton[i-1],0,wxALL|wxALIGN_LEFT,5);
        }
        */
        if (i<n_point) BoxSizer->Add(BoxSizerButton[i],0,wxALL|wxEXPAND,5);

    }
    Bind(wxEVT_CLOSE_WINDOW,&AnalysisDialog::OnClose,this);
#ifdef _WIN32
    Bind(wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&AnalysisDialog::OnClose,this,3000);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&AnalysisDialog::OnZoom,this,5001);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&AnalysisDialog::OnZoom,this,5002);
#else
    Bind(wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&AnalysisDialog::OnClose,this,3000);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED,&AnalysisDialog::OnZoom,this,5001);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED,&AnalysisDialog::OnZoom,this,5002);
#endif // _WIN32

    surface->SetSizer(BoxSizer);

	SetSizer(UpBoxSizer);
	UpBoxSizer->Fit(this);
	UpBoxSizer->SetSizeHints(this);



    wxAcceleratorEntry entries[6];

    entries[0].Set(wxACCEL_NORMAL,WXK_ESCAPE,3000);
    entries[1].Set(wxACCEL_ALT,(int) 'Z',4000);
    entries[2].Set(wxACCEL_ALT,(int) 'E',4001);
    entries[3].Set(wxACCEL_ALT,(int) 'I',4002);
    entries[4].Set(wxACCEL_CTRL,(int) '+',5001);
    entries[5].Set(wxACCEL_CTRL,(int) '-',5002);
    wxAcceleratorTable accel(6,entries);
    SetAcceleratorTable(accel);

}


AnalysisDialog::~AnalysisDialog()
{

}

void AnalysisDialog::OnClose(wxCloseEvent&)
{
    if (N_child!=nullptr) --(*N_child);

    /*
    if (Parent!=nullptr) {
        ParentEvent p_event(P_EVENT,IdClose);

        p_event.SetPointString("N_child = " + val::ToString(*N_child));
        wxPostEvent(Parent,p_event);
    }
    */

    Destroy();
}

void AnalysisDialog::OnButtonClicked(wxCommandEvent &event)
{
    int i = event.GetId()%2000;
    //wxMessageBox(val::ToString(i));
    Paint(i);
}

void AnalysisDialog::OnShortCuts(wxCommandEvent &event)
{
    int i =event.GetId()%4000;
    if (checkbox[i]->GetValue()) checkbox[i]->SetValue(false);
    else checkbox[i]->SetValue(true);
    //Paint(i);
}


void AnalysisDialog::OnZoom(wxCommandEvent &event)
{
    if (event.GetId()==5001) ++fontsize;
    else --fontsize;
    if (TextEdit.isempty()) return;
    if (fontsize<6) fontsize=6;
    if (fontsize>20) fontsize=20;
    wxFont font;
    int inp;
    for (auto &text : TextEdit) {
        if (text==nullptr) continue;
        inp=text->GetInsertionPoint();
        font = text->GetFont();
        font.SetPointSize(fontsize);
        text->SetFont(font);
        text->SetInsertionPoint(inp);
        //text->SetValue(text->GetValue());
    }
}


void AnalysisDialog::Paint(int i)
{
    if (Parent==nullptr) return;
    int n=0;
    ParentEvent p_event(P_EVENT);
    std::string sf="points";
    for (const auto &P : (*Points)[i]) {
        if (val::isNaN(P.x) || val::isNaN(P.y)) continue;
        sf+=" " + val::ToString(P.x) + " "+ val::ToString(P.y);
        ++n;
    }
    if (n==0) return;
    p_event.SetPointString(sf);

    wxPostEvent(Parent,p_event);
}

// ------------------------------------------------------------------------------------------------------------------------

InputFunctionDialog::InputFunctionDialog(wxWindow *parent,const val::d_array<std::string> &Wlist,const wxString &value,
                                         const wxString &s_text, const wxString& title, const wxSize &size,int fonts)
                                            : BaseDialog(parent,0,wxDefaultPosition,title),
                                            input(new val::CompleteTextCtrl(this,30,Wlist,value,size,wxDefaultPosition,wxTE_MULTILINE|wxVSCROLL|wxHSCROLL))
{
    Build(s_text,fonts);
}

InputFunctionDialog::InputFunctionDialog(wxWindow *parent,const val::trie_type<std::string> &Wlist,const wxString &value,
                                         const wxString &s_text, const wxString& title, const wxSize &size,int fonts)
                                            : BaseDialog(parent,0,wxDefaultPosition,title),
                                            input(new val::CompleteTextCtrl(surface,30,Wlist,value,size,wxDefaultPosition,wxTE_MULTILINE|wxVSCROLL|wxHSCROLL))
{
    Build(s_text,fonts);
}


void InputFunctionDialog::Build(const wxString& s_text, int fonts)
{
    wxBoxSizer *PBoxSizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText *st_text = new wxStaticText(surface,20,s_text);
    PBoxSizer->Add(st_text,0,wxALL,5);
    //input = new val::CompleteTextCtrl(this,30,Wlist,value,size,wxDefaultPosition,wxTE_MULTILINE|wxVSCROLL|wxHSCROLL);

    if (fonts < 10) fonts = 10;
    if (fonts > 20) fonts = 20;
    wxFont myfont = input->GetFont();
    myfont.SetPointSize(fonts);
    input->SetFont(myfont);
    input->SetCloseBrackets(true);
#ifdef _WIN32
    input->SetInsertionPoint(input->GetLastPosition());
#endif // _WIN32
    input->SetFocus();
    PBoxSizer->Add(input,1,wxALL|wxEXPAND,10);
	surface->SetSizer(PBoxSizer);


    SetAccelerator();
    //n_SetAccelerator();

	BoxSizer->SetSizeHints(this);
}





// ---------------------------------------------------------------------------------------------------------------------------------------------------



InputDialog::InputDialog(wxWindow *parent, wxWindowID id, const val::trie_type<std::string> &list, const wxString& value,
                         const wxSize &size, const wxPoint& pos, int fonts) : wxDialog(parent,id,"",wxDefaultPosition,wxDefaultSize,wxBORDER_RAISED)
{
    Move(pos);
#ifdef _WIN32
    input = new val::CompleteTextCtrl(this,101,list,value,size,wxDefaultPosition,wxTE_MULTILINE|wxTE_PROCESS_ENTER); // _WIN32
#else
    input = new val::CompleteTextCtrl(this,101,list,value,size,wxDefaultPosition,wxTE_PROCESS_ENTER);
#endif // _WIN32

    if (fonts < 10) fonts = 10;
    if (fonts > 20) fonts = 20;
    fontsize = fonts;
    wxFont myfont = input->GetFont();
    myfont.SetPointSize(fonts);
    input->SetFont(myfont);

    tooltip = new wxTextCtrl(this,201,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
    tooltip->SetFont(myfont);


    BoxSizer = new wxBoxSizer(wxVERTICAL);
    //wxBoxSizer *TextSizer = new wxBoxSizer(wxHORIZONTAL);
    //TextSizer->Add(input,1,wxEXPAND|wxALL,0);
    BoxSizer->Add(input,0,wxALL|wxEXPAND,0);
    BoxSizer->Add(tooltip,0,wxALL|wxEXPAND,5);
    tooltip->Hide();
    //input = new wxTextCtrl(this,101,value,pos,size);
    Bind(wxEVT_TEXT_ENTER,&InputDialog::OnEnterhit,this,101);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&InputDialog::OnEnterhit,this,102);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&InputDialog::OnHelp,this,103);

    wxAcceleratorEntry entries[2];
    entries[0].Set(wxACCEL_NORMAL,WXK_ESCAPE,102);
    entries[1].Set(wxACCEL_CTRL,(int) 'H',103);
    wxAcceleratorTable accel(2,entries);
    SetAcceleratorTable(accel);

    SetSizer(BoxSizer);
	BoxSizer->Fit(this);
	BoxSizer->SetSizeHints(this);
}

void InputDialog::OnEnterhit(wxCommandEvent &event)
{
    int id = event.GetId();
    event.Skip();
    if (id == 101) {
        EndModal(wxID_OK);
    }
    else if (tooltip->IsShown()) {
        tooltip->Hide();
        BoxSizer->Fit(this);
        BoxSizer->SetSizeHints(this);
    }
    else EndModal(wxID_CANCEL);
}

void InputDialog::OnHelp(wxCommandEvent &)
{
    if (tooltip->IsShown()) return;
    if (CommandsParList == nullptr || CommandsList == nullptr || SettingsList == nullptr || SettingsParList == nullptr) return;

    wxTextAttr Style = tooltip->GetDefaultStyle();
    std::string firstword, text = std::string(input->GetValue());
    int n = text.length(), i, com = -1;

    Style.SetFontSize(fontsize);

    // get first word:
    for (i = 0; i < n; ++i) {
        if (text[i] == ' ') break;
        firstword += text[i];
    }

    // match firstword in lists:
    for (i = 0; i < CommandsList->length(); ++i) {
        if (firstword == (*CommandsList)[i]) {
            com = i;
            break;
        }
    }
    if (com != -1) {
        Style.SetFontStyle(wxFONTSTYLE_ITALIC);
        tooltip->SetDefaultStyle(Style);
        tooltip->SetValue((*CommandsParList)[com]);
    }
    else {
        for (i = 0; i < SettingsList->length(); ++i) {
            if (firstword == (*SettingsList)[i]) {
                com = i;
                break;
            }
        }
        if (com != -1) {
            Style.SetFontStyle(wxFONTSTYLE_ITALIC);
            tooltip->SetDefaultStyle(Style);
            tooltip->SetValue((*SettingsParList)[com]);
        }
    }


    //text = input->GetValue();

    tooltip->Show();
    input->SetFocus();
    BoxSizer->Fit(this);
    BoxSizer->SetSizeHints(this);

}


