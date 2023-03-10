#ifndef MULTILINEDIALOG_H
#define MULTILINEDIALOG_H

#include "valDialogs.h"
#include "valControls.h"


#include <wx/clrpicker.h>
#include <wx/colordlg.h>
#include <wx/fontpicker.h>


// -----------------------FunctionColorDialog ------------------------------------------------------------


class FunctionColorDialog : public wxDialog
{
public:
	FunctionColorDialog(wxWindow *parent,const wxColour& color= *wxBLACK,const int linewidth=2);
	virtual ~FunctionColorDialog();
	const wxColor& GetColor() const {return Color;}
	int GetLineWith() const {return LineWidth;}
private:
    wxSpinCtrl *ColorSizeCtrl=nullptr;
    wxPanel *DrawPanel=nullptr;
    //wxColourPickerCtrl *ColorPicker=nullptr;
    wxButton *ColorButton=nullptr;
    wxColourData *ColorData=nullptr;
    wxColor Color;
    int LineWidth;
    void OnButtonClick(wxCommandEvent& event);
    void OnDrawPanelPaint(wxPaintEvent& event);
    //void OnColorChanged(wxColourPickerEvent& event);
    void OnColorButton(wxCommandEvent &event);
    void OnWidthChanged(wxSpinEvent& event);
    void Paint() const;
    int sx=150,sy=30;
};

// ------------------------------------------------------------------------------------------------------------

// ------------------------------------------ TextFontsDialog -------------------------------------------------

class TextFontsDialog : public wxDialog
{
public:
    TextFontsDialog(wxWindow * parent,const wxFont& textfont,const wxColour& color= *wxBLACK);
    virtual ~TextFontsDialog();
    const wxColour& GetColor() const {return Color;}
    const wxFont& GetFont() const {return TextFont;}
private:
    wxColour Color;
    wxFont TextFont;
    //wxColourPickerCtrl *ColorPicker=nullptr;
    wxTextCtrl *PreviewText=nullptr;
    wxButton *ColorButton=nullptr;
    wxColourData *ColorData=nullptr;
    wxFontPickerCtrl *FontPicker=nullptr;
    void OnButtonClick(wxCommandEvent& event);
    //void OnColorChanged(wxColourPickerEvent& event);
    void OnColorButton(wxCommandEvent &event);
    void OnFontChanged(wxFontPickerEvent& event);
    void Write() const;
};

// --------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------

class ParentEvent;

wxDECLARE_EVENT(P_EVENT,ParentEvent);

enum{IdPaintPoints};

class ParentEvent : public wxCommandEvent
{
public:
    ParentEvent(wxEventType commandtype=P_EVENT,int id=IdPaintPoints) : wxCommandEvent(commandtype,id) { }
    ParentEvent(const ParentEvent& event) : wxCommandEvent(event) {this->SetPointString(event.GetPointString());}
    wxEvent* Clone() const {return new ParentEvent(*this);}
    void SetPointString(const std::string& s) {p_string=s;}
    const std::string& GetPointString() const {return p_string;}
private:
    std::string p_string="";
};

// -----------------------------------------------------------------------------------------------------------


class AnalysisDialog : public wxDialog
{
public:
    AnalysisDialog(wxWindow *parent,int &nchild,const val::d_array<std::string> &output,const val::d_array<val::d_array<val::GPair<double>>> &Point,
                   const wxSize &Size = wxSize(250,80), const wxPoint &Pos=wxDefaultPosition,int fonts=10);
    virtual ~AnalysisDialog();
private:
    wxWindow *Parent=nullptr;
    int *N_child=nullptr;
    const val::d_array<val::d_array<val::GPair<double>>> *Points=nullptr;
    //val::d_array<wxCheckBox*> checkbox{nullptr,3};
    val::d_array<val::SwitchCtrl*> checkbox{nullptr,3};
    val::d_array<wxTextCtrl*> TextEdit;
    int fontsize=10;
    //
    void OnClose(wxCloseEvent &event);
    void OnButtonClicked(wxCommandEvent &event);
    void OnShortCuts(wxCommandEvent &event);
    void OnZoom(wxCommandEvent &event);
    void Paint(int i);
};


// -----------------------------------------------------------------------------------------------------------------------------------


class InputFunctionDialog : public val::BaseDialog
{
public:
    InputFunctionDialog(wxWindow *parent,const val::d_array<std::string> &Wlist,const wxString &value="", const wxString &s_text="", const wxString& title="",
                        const wxSize &size = wxDefaultSize,int fonts = 10);

    ~InputFunctionDialog() {}
    wxString GetValue() const {return input->GetValue();}
    void SetCloseBrackets(bool v) {input->SetCloseBrackets(v);}
    bool GetCloseBrackets() const {return input->GetCloseBrackets();}
    wxSize GetInputSize() const {return input->GetSize();}
    void SetMinInputSize(const wxSize& s) {input->SetMinSize(s);}
private:
    val::CompleteTextCtrl *input = nullptr;
};


#endif
