#ifndef MULTILINEDIALOG_H
#define MULTILINEDIALOG_H

#include "valDialogs.h"
#include "valControls.h"
#include <Glist.h>
#include <wx/event.h>
#include <wx/string.h>
#include <string>


#include <wx/clrpicker.h>
#include <wx/colordlg.h>
#include <wx/fontpicker.h>


// -----------------------FunctionColorDialog ------------------------------------------------------------


class FunctionColorDialog : public wxDialog
{
public:
	FunctionColorDialog(wxWindow *parent,const wxColour& color= *wxBLACK, const wxColour& backgroundcolor = *wxWHITE, const int linewidth=2);
	virtual ~FunctionColorDialog();
	const wxColor& GetColor() const {return Color;}
	int GetLineWith() const {return LineWidth;}
private:
    wxSpinCtrl *ColorSizeCtrl=nullptr;
    wxPanel *DrawPanel=nullptr;
    //wxColourPickerCtrl *ColorPicker=nullptr;
    wxButton *ColorButton=nullptr;
    wxColourData *ColorData=nullptr;
    wxColor Color, BackgroundColor;
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

enum anadialog_type{function_type, intersection_type, triangle_type};

class AnalysisDialog : public wxDialog
{
public:
    AnalysisDialog(wxWindow *parent,int &nchild,const val::d_array<wxString> &output,const val::d_array<val::d_array<val::GPair<double>>> &Point,
                   const wxSize &Size = wxSize(250,80), const wxPoint &Pos=wxDefaultPosition,int fonts=10, const std::string &title = "Analyze Function",
                   int dtype = anadialog_type::function_type);
    virtual ~AnalysisDialog();
private:
    wxWindow *Parent=nullptr;
    int *N_child=nullptr;
    const val::d_array<val::d_array<val::GPair<double>>> *Points=nullptr;
    //val::d_array<wxCheckBox*> checkbox{nullptr,3};
    val::d_array<val::SwitchCtrl*> checkbox{nullptr,3};
    val::d_array<wxTextCtrl*> TextEdit;
    int fontsize=10;
    int dialogtype = anadialog_type::function_type;
    wxPanel *surface = nullptr;
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
    InputFunctionDialog(wxWindow *parent,const val::d_array<wxString> &Wlist,const wxString &value="", const wxString &s_text="", const wxString& title="",
                        const wxSize &size = wxDefaultSize,int fonts = 10);
    InputFunctionDialog(wxWindow *parent,const val::trie_type<wxString> &Wlist,const wxString &value="", const wxString &s_text="", const wxString& title="",
                        const wxSize &size = wxDefaultSize,int fonts = 10);

    ~InputFunctionDialog() {}
    wxString GetValue() const {return input->GetValue();}
    void SetCloseBrackets(bool v) {input->SetCloseBrackets(v);}
    bool GetCloseBrackets() const {return input->GetCloseBrackets();}
    wxSize GetInputSize() const {return input->GetSize();}
    void SetMinInputSize(const wxSize& s) {input->SetMinSize(s);}
private:
    void Build(const wxString &s_text,int fonts);
    void OnShortcuts(wxCommandEvent &);
    val::CompleteTextCtrl *input = nullptr;
};


// -------------------------------------------------------------------------------------------------------------------------------------------

class InputDialog : public wxDialog
{
public:
    InputDialog(wxWindow *parent,wxWindowID id,const val::trie_type<wxString> &list,const wxString &value = "",const wxSize &size = wxDefaultSize,
                    const wxPoint &pos = wxDefaultPosition, int fonts = 10);
    ~InputDialog() {}
    void Clear() {input->Clear();}
    void SetSize(int x,int y) {input->SetSize(x,y);Fit();}
    wxFont GetFont() const {return input->GetFont();}
    bool SetFont(const wxFont &font) {return input->SetFont(font);}
    void SetFocus() {input->SetFocus();}
    void SetTextValue(const wxString& s) {input->SetValue(s);}
    const wxString GetTextValue() const {return input->GetValue();}
    void SetComLists(const val::d_array<wxString> &CL, const val::d_array<wxString> &CPL) {CommandsList = &CL; CommandsParList = &CPL;}
    void SetParLists(const val::d_array<wxString> &SL, const val::d_array<wxString> &SPL, const val::d_array<val::d_array<const std::string*>> &SC) {SettingsList = &SL; SettingsParList = &SPL; SettingsCurrent = &SC;}
    void SetHistory(const val::Glist<wxString> &SH) {history = &SH; ahistory = history->length();}

private:
    //wxWindow *Parent = nullptr;
    //wxWindowID  Identity = 0;
    val::CompleteTextCtrl *input = nullptr;
    wxTextCtrl *tooltip = nullptr;
    wxBoxSizer *BoxSizer = nullptr;
    int fontsize, ahistory;
    const val::d_array<wxString> *CommandsParList = nullptr, *CommandsList = nullptr, *SettingsList = nullptr, *SettingsParList = nullptr;
    const val::d_array<val::d_array<const std::string*>> *SettingsCurrent = nullptr;
    const val::Glist<wxString> *history = nullptr;
    //wxTextCtrl *input = nullptr;
    void OnEnterhit(wxCommandEvent&);
    void OnHelp(wxCommandEvent&);
};


// -------------------------------------------------------------------------------
class DLL_PUBLIC ListDialog : public wxDialog
{
public:
    ListDialog(wxWindow* parent,const val::Glist<std::string> &Choices=val::Glist<std::string>(),const std::string &title="",const std::string &Entry="",
               int sx=240,int sy=100,int fontsize=10, long style = 0, int max_choices = -1);
    virtual ~ListDialog();
    std::string GetText() const;
    int GetSelection() const;
    void SetSelection(int n) {if (listbox != nullptr) listbox->SetSelection(n);}
    val::d_array<int> GetSelections() const {if (listbox != nullptr) return listbox->GetSelections(); else return val::d_array<int>();}
	template <template <typename> class C> void SetSelections(const C<int> &List) {if (listbox!= nullptr) listbox->SetSelections(List);}
private:
    val::ListBox *listbox=nullptr;
    wxTextCtrl *TextCtrl1=nullptr;
    wxButton *CancelButton=nullptr,*OKButton=nullptr;
    void OnButtonClick(wxCommandEvent& event);
    void OnListBoxEvent(wxCommandEvent&);
};

#endif
