#ifndef CONTROLS_H_INCLUDED
#define CONTROLS_H_INCLUDED

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/popupwin.h>
#include <val_basics.h>
#include <d_array.h>
#include <string>
#include <trie.h>


//-------------------------------------------------------------------------------------------------------------------------------------

//class ISliderEvent;


//wxDECLARE_EVENT(IS_EVENT,ISliderEvent);
DLL_PUBLIC wxDECLARE_EVENT(val_EVENT_SLIDER,wxCommandEvent);

DLL_PUBLIC wxDECLARE_EVENT(val_EVENT_SWITCH,wxCommandEvent);

wxDECLARE_EVENT(val_EVENT_COMPLETE,wxCommandEvent);


/*
class ISliderEvent : public wxCommandEvent
{
public:
    ISliderEvent(wxEventType commandtype=IS_EVENT,int lv=0,int rv=0) : wxCommandEvent(commandtype),leftvalue(lv),rightvalue(rv) {}
    ISliderEvent(const ISliderEvent &event) : wxCommandEvent(event),leftvalue(event.leftvalue),rightvalue(event.rightvalue) {}
    wxEvent* Clone() const {return new ISliderEvent(*this);}
    int getleftvalue() const {return leftvalue;}
    int getrightvalue() const {return rightvalue;}
private:
    int leftvalue=0,rightvalue=0;
};

*/

namespace val
{

// -------------------------------------------------------------------------------------------------------------------------------------

class DLL_PUBLIC SpinCtrl : public wxSpinCtrlDouble
{
public:
    SpinCtrl(wxWindow *parent,wxWindowID id,const double& minval=0,const double& maxval=100,const double &dec=1,const double &dvalue=0);
    ~SpinCtrl() {}
};

// --------------------------------------------------------------------------------------------------------------------

class DLL_PUBLIC Slider : public wxControl
{
public:
    Slider(wxWindow *parent,wxWindowID id,const double& dval=0,const double& minv=0,const double &maxv=100,const double& step=1,
           const wxPoint &pos = wxDefaultPosition,const wxSize& Size = wxSize(150,50),const int style = wxHORIZONTAL);
    ~Slider() {}
    const double& GetValue() const {return value;}
    void SetValue(const double& nvalue);
private:
    wxWindow *Parent = nullptr;
    int identity = 0,Style = wxHORIZONTAL;
    double value=0,Minvalue=0,Maxvalue=100,Step=1;
    int sizex=150, sizey=50;
    int digits=3,decimals=0,svalue=0,range=100,slidereventnotready=0;
    std::string s_min,s_max;
    wxPanel *DrawPanel = nullptr;
    wxSlider *wxslider = nullptr;
    wxColour backgroundcolor = GetBackgroundColour();//wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
    wxColour textcolor = wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT);
    wxColour passivtext = wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT);
    //
    void SetLimits();
    void SetSliderValues();
    void OnSliderEvent(wxCommandEvent&);
    void Paint();
    void OnDrawPanelPaint(wxPaintEvent&);
    void OnDrawPanelResize(wxSizeEvent&);
    void SendEvent();
};

// ------------------------------------------------------------------------------------------------------------------------------


class DLL_PUBLIC IntervalSlider : public wxControl
{
public:
    IntervalSlider(wxWindow *parent, wxWindowID id,int p_size = 250,int b_height=8,int lv=0,int uv=100);
    ~IntervalSlider();
    int getleastvalue() const {return lvalue;}
    int getuppervalue() const {return uvalue;}
    int getleftvalue() const {return leftvalue;}
    int getrightvalue() const {return rightvalue;}
    void setlimits(int l_val,int r_val);
    void setvalues(int l, int r);
    void setallvalues(int leftlimit, int rightlimit, int l_value, int r_value);
private:
    wxWindow *Parent=nullptr;
    int identity=0;
    int minsize=250,panelsize=250,barheight=8;
    int lvalue=0,uvalue=100,leftvalue,rightvalue;
    wxSpinCtrl *LSpin=nullptr, *RSpin=nullptr;
    wxPanel *DrawPanel=nullptr;
    wxColor barcolor=wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
    wxColor bracketcolor=wxSystemSettings::GetColour(wxSYS_COLOUR_SCROLLBAR);
    wxColor backgroundcolor=wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW );
    wxColor textcolor=wxSystemSettings::GetColour(wxSYS_COLOUR_MENUTEXT);
    wxColor pointcolor;

    int height,MouseCaptured=0,ispainted=1,dx=5,dy=6,barwidth,x1,x2,leftmoved,rightmoved;
    wxString s_lvalue,s_uvalue;

    void plotbar(wxDC &dc);
    void Paint();
    void OnDrawPanelPaint(wxPaintEvent& event);
    void OnDrawPanelResize(wxSizeEvent&);
    void OnSpinCtrl(wxSpinEvent &event);
    void OnArrowsPressed(wxCommandEvent &event);
    void OnMouseCaptured(wxMouseEvent& event);
    void OnMouseReleased(wxMouseEvent& event);
    void OnMouseMoved(wxMouseEvent& event);
    void OnLostMouse(wxMouseCaptureLostEvent &event);
    void SendSliderEvent();
};
//--------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------

class DLL_PUBLIC SwitchCtrl : public wxControl
{
public:
    SwitchCtrl(wxWindow *parent,wxWindowID id,bool active=false);
    ~SwitchCtrl() {}
    bool GetValue() const {return is_active;}
    void SetValue(bool value);
    void ChangeValue(bool value);
private:
    wxWindow *Parent=nullptr;
    int identity=0;
    int plcolor = 50;
    bool is_active=false;
    wxPanel *DrawPanel = nullptr;
    wxColor backgroundcolor;//= wxSystemSettings::GetColour(wxSYS_COLOUR_MENU);
    wxColour barcolor_a = wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT);
    wxColour barcolor_p;//= wxSystemSettings::GetColour(wxSYS_COLOUR_BTNHIGHLIGHT);
    wxColour fg_color = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
    wxString checkmark = L"\u2714", xmark = "x";

    void Paint();
    void OnDrawPanelPaint(wxPaintEvent&);
    void OnMouseClicked(wxMouseEvent&);
    void SendEvent();
};

// ---------------------------------------------------------------------------------------------------------------------------

class DLL_PUBLIC TopListbox : public wxPopupWindow
{
public:
    TopListbox(wxWindow *parent) : wxPopupWindow(parent) {listbox = new wxListBox(this,11);}
    ~TopListbox() {}
    void Clear() {listbox->Clear();}
    void SetSize(int x,int y) {listbox->SetSize(x,y);Fit();}
    void SetSelection(int n) {listbox->SetSelection(n);}
    wxFont GetFont() const {return listbox->GetFont();}
    bool SetFont(const wxFont &font) {return listbox->SetFont(font);}
    void SetFocus() {listbox->SetFocus();}
    void Append(const wxString & value) {listbox->Append(value);}
private:
    wxListBox *listbox=nullptr;
};

// ----------------------------------------------------------------------------------------------------------------------------
// replaced by class CompleteTextCtrl
/*
class DLL_PUBLIC CompleteCtrl : public wxControl
{
public:
    CompleteCtrl(wxWindow *parent,wxWindowID id,const val::d_array<std::string> &list,const wxString &value = "",const wxSize &size = wxDefaultSize,
                 const wxPoint &pos = wxDefaultPosition,long style = 0);
    ~CompleteCtrl();
    wxString GetValue() const {return input->GetValue();}
    void SetValue(const wxString &value) {input->SetValue(value);}
    void SetFocus() {input->SetFocus();}
    wxFont GetFont() const {return input->GetFont();}
    bool SetFont(const wxFont& font);
    void SetCloseBrackets(bool v = true) {closebrackets = v;}
    bool GetCloseBrackets() const {return closebrackets;}
private:
    wxTextCtrl *input = nullptr;
    //wxListBox *listbox = nullptr;
    TopListbox *listbox = nullptr;
    wxAcceleratorTable *parenttable = nullptr, *accel = nullptr;
    val::d_array<std::string> WordList,CandList;
    std::string actualword;
    int selection = 0, isactiv=0, n_candidates = 0, fontsize, actuallength = 0, formerlength=0;
    bool closebrackets = false;
    //void OnFocusEvent(wxFocusEvent &event);
    //void OnLostFocusEvent(wxFocusEvent &event);
    void OnInputChanged(wxCommandEvent &);
    void OnListBoxSelected(wxCommandEvent &);
    void OnCompleteBrackets(wxCommandEvent &);
    void CompleteWord();
    void CompleteBrackets();
    void BindAll();
    void UnbindAll();
};
*/
// -----------------------------------------------------------------------------------------------------------------------------------------------

class DLL_PUBLIC CompleteTextCtrl : public wxTextCtrl
{
public:
    CompleteTextCtrl(wxWindow *parent,wxWindowID id,const val::d_array<wxString> &list,const wxString &value = "",const wxSize &size = wxDefaultSize,
                    const wxPoint &pos = wxDefaultPosition,long style = 0);

    CompleteTextCtrl(wxWindow *parent,wxWindowID id,const val::trie_type<wxString> &list,const wxString &value = "",const wxSize &size = wxDefaultSize,
                    const wxPoint &pos = wxDefaultPosition,long style = 0);
    ~CompleteTextCtrl();
    bool SetFont(const wxFont& font);
    void SetCloseBrackets(bool v = true) {closebrackets = v;}
    bool GetCloseBrackets() const {return closebrackets;}
    void SetCompleteEnabled(bool v = true) {enablecomplete = v;}
    bool GetCompleteEnabled() const {return enablecomplete;}
private:
    wxWindow *Parent = nullptr;
    wxWindowID identity;
    TopListbox *listbox = nullptr;
    wxMemoryDC dc;
    val::d_array<wxString> CandList;
    val::trie_type<wxString> WordList;
    const val::d_array<wxChar> o_brackets{'(', '{', '[', '"'}, c_brackets{')', '}', ']', '"'};
    char beg = 65, end = 122;
    const val::trie_type<wxString> *WordListPointer = nullptr;
    wxString actualword;
    wxString s_text;
    long from = 0, to = 0;
    int selection = 0, isactiv=0, n_candidates = 0, fontsize, actuallength = 0, formerlength=0, textisselected = 0;
    bool closebrackets = false, enablecomplete = true;
    wxString bracket = ")";
    wxAcceleratorTable *parenttable = nullptr, *accel = nullptr, *taccel = nullptr;
    void BuildObject();
    int isalphabetical(wxChar s) const;
    wxString findword(const wxString &s, int pos) const;
    void OnInputChanged(wxCommandEvent &event);
    void OnListBoxSelected(wxCommandEvent &event);
    void OnCompleteBrackets(wxCommandEvent &);
    void OnShortCuts(wxCommandEvent &);
    void CompleteWord();
    void DuplicateLine();
    void CompleteBrackets();
    void BindAll();
    void UnbindAll();
    void OnKeyDown(wxKeyEvent &event);
    void OnKeyChar(wxKeyEvent &event);
};




} // end namespace val

#endif // CONTROLS_H_INCLUDED


