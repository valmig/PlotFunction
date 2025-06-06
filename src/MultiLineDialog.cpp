#include "MultiLineDialog.h"
#include <d_array.h>
#include <val_basics.h>
#include <wx/accel.h>
#include <wx/string.h>
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


AnalysisDialog::AnalysisDialog(wxWindow *parent,int &nchild,const val::d_array<wxString> &output,const val::d_array<val::d_array<val::GPair<double>>> &Point,
                                const wxSize &Size,const wxPoint &Pos,int fonts, const std::string &title, int dtype) : Parent(parent), N_child(&nchild) , Points(&Point), fontsize(fonts), dialogtype(dtype)
{
    Create(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxRESIZE_BORDER|wxDEFAULT_DIALOG_STYLE, _T("wxID_ANY"));
    ++(*N_child);
    Move(Pos);
    int n_point=Point.length(),i,n_output=output.length();
    if (n_output > 4 || n_output < 2 || n_point > 3 || n_point < 1) return;

    surface = new wxPanel(this,100);

    surface->SetBackgroundColour(surface->GetBackgroundColour());

    TextEdit = val::d_array<wxTextCtrl*>(nullptr,n_output);
    val::d_array<wxBoxSizer*> BoxSizerText(nullptr,n_output);
    val::d_array<wxBoxSizer*> BoxSizerButton(nullptr,n_point);
    val::d_array<wxStaticText*> Text(nullptr,n_point);
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
    if (n_point == 3 && dialogtype == anadialog_type::function_type) {
        Text[0] = new wxStaticText(surface,11,_T("Zeros:"));
        Text[1] = new wxStaticText(surface,12,_T("Extrema:"));
        Text[2] = new wxStaticText(surface,13,_T("Inflection Points:"));
    }
    else if (n_point == 1 && dialogtype == anadialog_type::intersection_type) {
        Text[0] = new wxStaticText(surface,11,_T("Intersection points:"));
    }
    else if (dialogtype == anadialog_type::triangle_type) {
        Text[0] = new wxStaticText(surface,11,_T("Circumcircle:"));
        Text[1] = new wxStaticText(surface,12,_T("Incircle:"));
        Text[2] = new wxStaticText(surface,12,_T("Centroid:"));
    }
    if (n_point && Text[0] != nullptr) {
        wxFont font = Text[0]->GetFont();
        font.SetPointSize(fontsize + 1);
        for (i = 0; i < n_point; ++i) {
            if (Text[i] != nullptr) Text[i]->SetFont(font);
        }
    }
	 //
    /*
    for (i=1;i<n_output;++i) {
        BoxSizerText[i]->Add(Text[i-1],0,wxALL|wxALIGN_LEFT,5);
    }
    */
    for (i=0;i<n_point;++i) {
        BoxSizerButton[i] = new wxBoxSizer(wxHORIZONTAL);
        BoxSizerButton[i]->Add(Text[i],1,wxLEFT|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL,10);
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
    //wxCommandEvent s_event(val_EVENT_SWITCH,2000+i);
    //wxPostEvent(this,s_event);
    //ProcessWindowEvent(s_event);
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
    std::string sf;
    if (dialogtype == anadialog_type::function_type || dialogtype == anadialog_type::intersection_type) {
        sf="points";
        for (const auto &P : (*Points)[i]) {
            if (val::isNaN(P.x) || val::isNaN(P.y)) continue;
            sf+=" " + val::ToString(P.x) + " "+ val::ToString(P.y);
            ++n;
        }
    }
    else if (dialogtype == anadialog_type::triangle_type) {
        if (i <= 1) sf = "circle " + val::ToString((*Points)[i][0].x) + " " + val::ToString((*Points)[i][0].y) + " " + val::ToString((*Points)[i][1].x) + " 0 360 -1";
        else sf = "points " + val::ToString((*Points)[i][0].x) + " " + val::ToString((*Points)[i][0].y);
        n = 3;
    }
    if (n==0) return;
    p_event.SetPointString(sf);

    wxPostEvent(Parent,p_event);
}

// ------------------------------------------------------------------------------------------------------------------------

InputFunctionDialog::InputFunctionDialog(wxWindow *parent,const val::d_array<wxString> &Wlist,const wxString &value,
                                         const wxString &s_text, const wxString& title, const wxSize &size,int fonts)
                                            : BaseDialog(parent,0,wxDefaultPosition,title),
                                            input(new val::CompleteTextCtrl(surface,30,Wlist,value,size,wxDefaultPosition,wxTE_MULTILINE|wxVSCROLL|wxHSCROLL))
{
    Build(s_text,fonts);
}

InputFunctionDialog::InputFunctionDialog(wxWindow *parent,const val::trie_type<wxString> &Wlist,const wxString &value,
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



InputDialog::InputDialog(wxWindow *parent, wxWindowID id, const val::trie_type<wxString> &list, const wxString& value,
                         const wxSize &size, const wxPoint& pos, int fonts) : wxDialog(parent,id,"",wxDefaultPosition,wxDefaultSize,wxBORDER_RAISED)
{
    Move(pos);
#ifdef _WIN32
    input = new val::CompleteTextCtrl(this,101,list,value,size,wxDefaultPosition,wxTE_MULTILINE|wxTE_PROCESS_ENTER); // _WIN32
#else
    input = new val::CompleteTextCtrl(this,101,list,value,size,wxDefaultPosition,wxTE_PROCESS_ENTER);
#endif // _WIN32

    input->SetCloseBrackets(true);

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
    Bind(wxEVT_COMMAND_MENU_SELECTED,&InputDialog::OnEnterhit,this,104);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&InputDialog::OnEnterhit,this,105);

    wxAcceleratorEntry entries[4];
    entries[0].Set(wxACCEL_NORMAL,WXK_ESCAPE,102);
    entries[1].Set(wxACCEL_CTRL,(int) 'H',103);
    entries[2].Set(wxACCEL_NORMAL,WXK_UP,104);
    entries[3].Set(wxACCEL_NORMAL,WXK_DOWN,105);
    wxAcceleratorTable accel(4,entries);
    SetAcceleratorTable(accel);

    SetSizer(BoxSizer);
	BoxSizer->Fit(this);
	BoxSizer->SetSizeHints(this);
}

void InputDialog::OnEnterhit(wxCommandEvent &event)
{
    int id = event.GetId();
    //event.Skip();
    if (id == 104 || id == 105) {
        if (history == nullptr || history->isempty()) return;
        if (id == 104) --ahistory;
        else ++ahistory;
        ahistory = val::Max(-1, ahistory);
        ahistory = val::Min(history->length(), ahistory);
        if ((ahistory >= 0) && (ahistory < history->length())) {
            bool stat = input->GetCompleteEnabled();
            input->SetCompleteEnabled(false);
            input->SetValue((*history)[ahistory]);
            input->SetInsertionPointEnd();
            input->SetCompleteEnabled(stat);
        }
        else SetTextValue("");
        return;
    }
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

void InputDialog::OnHelp(wxCommandEvent &event)
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
            wxString s = (*SettingsParList)[com];
            if (com < (*SettingsCurrent).length()) {
                if (!(*SettingsCurrent)[com].isempty()) {
                    s += "\nCurrent: ";
                    for (const auto &v : (*SettingsCurrent)[com]) {
                        s += *v + "  ";
                    }
                }
            }
            Style.SetFontStyle(wxFONTSTYLE_ITALIC);
            tooltip->SetDefaultStyle(Style);
            tooltip->SetValue(s);
        }
        else return;
    }

    tooltip->Show();
    input->SetFocus();
    input->SelectNone();
    BoxSizer->Fit(this);
    BoxSizer->SetSizeHints(this);
}


// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ListDialog::ListDialog(wxWindow* parent,const val::Glist<std::string> &Choices,const std::string &title,const std::string &Entry,
                       int sx,int sy,int fontsize, long style, int max_choices)
{
	if (sx<100) sx=100;
	if (sx>400) sx=400;
	if (sy<10) sy=10;
	if (sy>300) sy=300;
	//*Initialize(MultiLineDialog)
	wxBoxSizer *BoxSizer1, *BoxSizer2;

	Create(parent, wxID_ANY, title,wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);

	if (!Choices.isempty()) {
        wxStaticText *StaticText1 = new wxStaticText(this,41,_T("Select Function:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
        BoxSizer1->Add(StaticText1, 0, wxALL | wxALIGN_LEFT, 10);
        listbox = new val::ListBox(this,30,Choices,style,max_choices);
        BoxSizer1->Add(listbox, 1, wxALL | wxEXPAND, 5);
	}
    wxStaticText *StaticText2 = new wxStaticText(this,42,_T("Set Interval/Precision/Iterations/Decimals:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    BoxSizer1->Add(StaticText2, 0, wxALL | wxALIGN_LEFT, 10);

	TextCtrl1 = new wxTextCtrl(this,20,Entry,wxDefaultPosition, wxSize(sx,sy), wxTE_MULTILINE|wxVSCROLL|wxHSCROLL);
    BoxSizer1->Add(TextCtrl1, 1, wxALL | wxEXPAND, 5);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	CancelButton = new wxButton(this, 51, _("Cancel"));
	BoxSizer2->Add(CancelButton, 1, wxALL, 5);
	OKButton = new wxButton(this, 52, _("OK"));
	BoxSizer2->Add(OKButton, 1, wxALL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_RIGHT, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&ListDialog::OnButtonClick,this, 52);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&ListDialog::OnButtonClick,this, 51);
	Bind(wxEVT_LISTBOX_DCLICK,&ListDialog::OnListBoxEvent,this,30);


    wxAcceleratorEntry entries[3];
    entries[0].Set(wxACCEL_CTRL, (int) 'O', 52);
    //entries[1].Set(wxACCEL_CTRL, (int) 'C',ID_BUTTON1);
    entries[1].Set(wxACCEL_NORMAL,WXK_ESCAPE, 51);
    entries[2].Set(wxACCEL_CTRL,WXK_RETURN, 52);
    wxAcceleratorTable accel(3,entries);
    SetAcceleratorTable(accel);

    {
        wxFont myfont(TextCtrl1->GetFont());
        if (fontsize<10) fontsize=10;
        if (fontsize>16) fontsize=16;
        myfont.SetPointSize(fontsize);
        TextCtrl1->SetFont(myfont);
        listbox->SetFont(myfont);
        //StaticText1->SetFont(myfont);
    }
}


ListDialog::~ListDialog()
{

}

std::string ListDialog::GetText() const
{
    return std::string(TextCtrl1->GetValue());
    //return std::string();
}

int ListDialog::GetSelection() const
{
    if (listbox!=nullptr) return listbox->GetSelection();
    else return -1;
    //return 0;
}



void ListDialog::OnButtonClick(wxCommandEvent& event)
{
    if (event.GetId()==52) EndModal(wxID_OK);
    else EndModal(wxID_CANCEL);
}

void ListDialog::OnListBoxEvent(wxCommandEvent&)
{
    EndModal(wxID_OK);
}
