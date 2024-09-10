#include "valDialogs.h"
#include <d_array.h>
#include <wx/event.h>
//(*InternalHeaders(MultiLineDialog)
#include <wx/intl.h>
#include <wx/string.h>
//*)

namespace val
{

enum {BaseDialog_Esc_Id=32600, BaseDialog_Return_Id, MultiLineDialog_OK_Id, MultiLineDialog_Cancel_Id,
        InfoWindow_Esc_Id, ListDialog_OK_Id, ListDialog_Cancel_Id};

// ----------------------------------------------------------------------------------------------------------------------------

BaseDialog::BaseDialog(wxWindow *parent,wxWindowID id,const wxPoint& point,const wxString& title) : wxDialog(parent,id,title,point,wxDefaultSize,wxRESIZE_BORDER|wxCAPTION)
{
    wxButton *CancelButton = new wxButton(this,BaseDialog_Esc_Id,_("Cancel"));
    wxButton *OKButton = new wxButton(this,BaseDialog_Return_Id,_("OK"));
    BoxSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *PanelSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *ButtonSizer = new wxBoxSizer(wxHORIZONTAL);

    PanelSizer->Add(surface,1,wxALL|wxEXPAND,0);
    ButtonSizer->Add(CancelButton,1,wxALL,5);
    ButtonSizer->Add(OKButton,1,wxALL,5);

    BoxSizer->Add(PanelSizer,1,wxALL|wxEXPAND,0);
    BoxSizer->Add(ButtonSizer,0,wxALL|wxALIGN_RIGHT,5);

    Bind(wxEVT_COMMAND_BUTTON_CLICKED,&BaseDialog::OnButtonClicked,this,BaseDialog_Esc_Id);
    Bind(wxEVT_COMMAND_BUTTON_CLICKED,&BaseDialog::OnButtonClicked,this,BaseDialog_Return_Id);

    AccEntry.push_back(wxAcceleratorEntry(wxACCEL_NORMAL,WXK_ESCAPE,BaseDialog_Esc_Id));
    AccEntry.push_back(wxAcceleratorEntry(wxACCEL_CTRL,WXK_RETURN,BaseDialog_Return_Id));

    SetSizer(BoxSizer);
	BoxSizer->Fit(this);
	BoxSizer->SetSizeHints(this);

}


void BaseDialog::SetAccelerator()
{
    int n = AccEntry.length();
    wxAcceleratorEntry *entries = new wxAcceleratorEntry[n];

    for (int i=0;i<n;++i) entries[i] = AccEntry[i];
    wxAcceleratorTable accel(n,entries);
    SetAcceleratorTable(accel);

    delete[] entries;
}

void BaseDialog::OnButtonClicked(wxCommandEvent &event)
{
    if (event.GetId() == BaseDialog_Return_Id) return EndModal(wxID_OK);
    else return EndModal(wxID_CANCEL);
}

// --------------------------------------------------------- MultiLineDialog --------------------------------------------------------------


//IdInit(MultiLineDialog)
const long MultiLineDialog::ID_STATICTEXT1 = wxNewId();
const long MultiLineDialog::ID_TEXTCTRL1 = wxNewId();
//const long MultiLineDialog::ID_BUTTON1 = wxNewId();
//const long MultiLineDialog::ID_BUTTON2 = wxNewId();
//


MultiLineDialog::MultiLineDialog(wxWindow* parent,const wxString &Entry,const wxString &SText,int sx,int sy,
                  const wxString &title,int fontsize,int singleline,int style) : Style(style)
{
	if (sx<100) sx=100;
	if (sx>400) sx=400;
	if (sy<10) sy=10;
	if (sy>300) sy=300;
	//*Initialize(MultiLineDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;

	Create(parent, wxID_ANY, title,wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);
	BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1,SText, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	BoxSizer2->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_LEFT, 5);
	BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
	if (!singleline) TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1,Entry, wxDefaultPosition, wxSize(sx,sy), wxTE_MULTILINE|wxVSCROLL|wxHSCROLL|Style, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	else TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1,Entry, wxDefaultPosition, wxSize(sx,-1),Style, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	BoxSizer3->Add(TextCtrl1, 1, wxALL|wxEXPAND, 5);
	BoxSizer1->Add(BoxSizer3, 1, wxALL|wxEXPAND, 5);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	CancelButton = new wxButton(this,MultiLineDialog_Cancel_Id, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
	BoxSizer4->Add(CancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	OKButton = new wxButton(this, MultiLineDialog_OK_Id, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
	BoxSizer4->Add(OKButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer4, 0, wxALL|wxALIGN_RIGHT, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	//Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MultiLineDialog::OnCancelButtonClick);
	//Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&MultiLineDialog::OnOKButtonClick);
	//*)
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&MultiLineDialog::OnOKButtonClick,this,MultiLineDialog_OK_Id);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&MultiLineDialog::OnCancelButtonClick,this,MultiLineDialog_Cancel_Id);
    if (singleline) {
        wxAcceleratorEntry entries[4];
        entries[0].Set(wxACCEL_CTRL, (int) 'O',MultiLineDialog_OK_Id);
        //entries[1].Set(wxACCEL_CTRL, (int) 'C',ID_BUTTON1);
        entries[1].Set(wxACCEL_NORMAL,WXK_ESCAPE,MultiLineDialog_Cancel_Id);
        entries[2].Set(wxACCEL_CTRL,WXK_RETURN,MultiLineDialog_OK_Id);
        entries[3].Set(wxACCEL_NORMAL,WXK_RETURN,MultiLineDialog_OK_Id);
        wxAcceleratorTable accel(4,entries);
        SetAcceleratorTable(accel);
    }
    else {
        wxAcceleratorEntry entries[3];
        entries[0].Set(wxACCEL_CTRL, (int) 'O',MultiLineDialog_OK_Id);
        //entries[1].Set(wxACCEL_CTRL, (int) 'C',ID_BUTTON1);
        entries[1].Set(wxACCEL_NORMAL,WXK_ESCAPE,MultiLineDialog_Cancel_Id);
        entries[2].Set(wxACCEL_CTRL,WXK_RETURN,MultiLineDialog_OK_Id);
        wxAcceleratorTable accel(3,entries);
        SetAcceleratorTable(accel);
    }
    {
        wxFont myfont(TextCtrl1->GetFont());
        if (fontsize<10) fontsize=10;
        if (fontsize>16) fontsize=16;
        myfont.SetPointSize(fontsize);
        TextCtrl1->SetFont(myfont);
        //StaticText1->SetFont(myfont);
    }
}

MultiLineDialog::~MultiLineDialog()
{
	//(*Destroy(MultiLineDialog)
	//*)
}


void MultiLineDialog::OnOKButtonClick(wxCommandEvent& event)
{
    EndModal(wxID_OK);
}

void MultiLineDialog::OnCancelButtonClick(wxCommandEvent& event)
{
    EndModal(wxID_CANCEL);
}


wxString MultiLineDialog::GetSettingsText() const
{
    return TextCtrl1->GetValue();
}

wxString MultiLineDialog::GetValue() const
{
	if (TextCtrl1 != nullptr) return TextCtrl1->GetValue();
	else return _("");
}

// ---------------------------------------------------------------------------------------------------------------------


// ------------------------------------------InfoWindow---------------------------------------------------------------------------------

InfoWindow::InfoWindow(wxWindow* parent,const wxString &Entry,const wxPoint &Position,const wxSize &Size,
                  const std::string &title,int fontsize, int multiline, long InfoStyle)
{
    Create(parent, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, InfoStyle, _T("wxID_ANY"));
    wxBoxSizer *BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    if (multiline) {
        int style = wxVSCROLL | wxHSCROLL | wxTE_READONLY | wxTE_MULTILINE;
        wxTextCtrl *textctrl= new wxTextCtrl(this,1000,Entry, wxDefaultPosition, wxDefaultSize,style, wxDefaultValidator);
        wxFont myfont(textctrl->GetFont());
        if (fontsize<10) fontsize=10;
        if (fontsize>18) fontsize=18;
        myfont.SetPointSize(fontsize);
        textctrl->SetFont(myfont);
        BoxSizer1->Add(textctrl, 1, wxALL|wxEXPAND, 5);
    }
    else {
        wxStaticText *textctrl = new wxStaticText(this,1000,Entry);
        wxFont myfont(textctrl->GetFont());
        if (fontsize<10) fontsize=10;
        if (fontsize>18) fontsize=18;
        myfont.SetPointSize(fontsize);
        textctrl->SetFont(myfont);
        BoxSizer1->Add(textctrl, 0, wxALL|wxALIGN_CENTER, 30);
    }
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);
	Move(Position);
	SetClientSize(Size);
	//number=&nchild;
	//++(*number);
    //OKButton = new wxButton(this,3000, _("OK"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator,"3000");
	//OKButton->Hide();
	Bind(wxEVT_CLOSE_WINDOW,&InfoWindow::OnClose,this);
	Bind(wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&InfoWindow::OnClose,this,InfoWindow_Esc_Id);
    wxAcceleratorEntry entries[1];

    entries[0].Set(wxACCEL_NORMAL,WXK_ESCAPE,InfoWindow_Esc_Id);
    wxAcceleratorTable accel(1,entries);
    SetAcceleratorTable(accel);
}

InfoWindow::~InfoWindow()
{
    if (number!=nullptr) *number = *number -1;
}


void InfoWindow::OnClose(wxCloseEvent &event)
{
    Destroy();
}

// ----------------------------------------------------------------------------------------------------------------------------------------------------

d_array<int> FromwxArray(const wxArrayInt& wA)
{
    int n;
    d_array<int> a(n=wA.size());
    for (int i=0;i<n;++i) a[i] = wA[i];
    return a;
}

void MultiChoiceDialog::SetAccelerators()
{
    Bind(wxEVT_COMMAND_BUTTON_CLICKED,&MultiChoiceDialog::OnKB,this,10); // uncheck all
    Bind(wxEVT_COMMAND_BUTTON_CLICKED,&MultiChoiceDialog::OnKB,this,11); // check all

    wxAcceleratorEntry entries[2];
    entries[0].Set(wxACCEL_CTRL,WXK_DELETE,10);
    entries[1].Set(wxACCEL_CTRL,(int) 'a',11);
    wxAcceleratorTable accel(2,entries);
    SetAcceleratorTable(accel);
}


void MultiChoiceDialog::OnKB(wxCommandEvent &event)
{
    if (event.GetId() == 10) {   // uncheck all
        val::d_array<int> sel;
        SetSelections(sel);
    }
    else { // check all
        val::d_array<int> sel(len);
        for (int i = 0; i < len; ++i) sel[i] = i;
        SetSelections(sel);
    }
}

// --------------------------------------------------------------------------------------------------------------------------------------------------


d_array<int> ListBox::GetSelections() const
{
    d_array<int> sel;
    sel.reserve(len);

    for (int i = 0; i < len ; ++i) {
        if (IsSelected(i)) sel.push_back(i);
    }
    return sel;
}
// --------------------------------------------------------------------------------------------------------------------------------------------------


ListDialog::ListDialog(wxWindow* parent,const val::Glist<std::string> &Choices,const std::string &title,const std::string &Entry,
                       int sx,int sy,int fontsize, long style, int max_choices)
{
	if (sx<100) sx=100;
	if (sx>400) sx=400;
	if (sy<10) sy=10;
	if (sy>300) sy=300;
	//*Initialize(MultiLineDialog)
	wxBoxSizer* BoxSizer1;
	wxBoxSizer* BoxSizer2;
	wxBoxSizer* BoxSizer3;
	wxBoxSizer* BoxSizer4;
	wxBoxSizer* BoxSizer41;
	wxBoxSizer* BoxSizer5;

	Create(parent, wxID_ANY, title,wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER, _T("wxID_ANY"));
	BoxSizer1 = new wxBoxSizer(wxVERTICAL);

	if (!Choices.isempty()) {
        BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
        wxStaticText *StaticText1 = new wxStaticText(this,41,_T("Select Function:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
        BoxSizer2->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
        BoxSizer1->Add(BoxSizer2, 0, wxALL|wxALIGN_LEFT, 5);
        listbox = new ListBox(this,30,Choices,style,max_choices);
        BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
        //BoxSizer3->Add(listbox,1,wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL,5);
        BoxSizer3->Add(listbox,1,wxALL|wxEXPAND,5);
        BoxSizer1->Add(BoxSizer3,1,wxALL|wxEXPAND,5);
	}
    BoxSizer41 = new wxBoxSizer(wxHORIZONTAL);
    wxStaticText *StaticText2 = new wxStaticText(this,42,_T("Set Interval/Precision/Iterations/Decimals:"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    BoxSizer41->Add(StaticText2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer1->Add(BoxSizer41, 0, wxALL|wxALIGN_LEFT, 5);

	TextCtrl1 = new wxTextCtrl(this,20,Entry,wxDefaultPosition, wxSize(sx,sy), wxTE_MULTILINE|wxVSCROLL|wxHSCROLL);
	BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
	BoxSizer4->Add(TextCtrl1,1,wxALL|wxEXPAND,5);
	BoxSizer1->Add(BoxSizer4,1,wxALL|wxEXPAND,5);
	BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
	CancelButton = new wxButton(this,ListDialog_Cancel_Id, _("Cancel"));
	BoxSizer5->Add(CancelButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	OKButton = new wxButton(this,ListDialog_OK_Id, _("OK"));
	BoxSizer5->Add(OKButton, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
	BoxSizer1->Add(BoxSizer5, 0, wxALL|wxALIGN_RIGHT, 5);
	SetSizer(BoxSizer1);
	BoxSizer1->Fit(this);
	BoxSizer1->SetSizeHints(this);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&ListDialog::OnButtonClick,this,ListDialog_OK_Id);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&ListDialog::OnButtonClick,this,ListDialog_Cancel_Id);
	Bind(wxEVT_LISTBOX_DCLICK,&ListDialog::OnListBoxEvent,this,30);


    wxAcceleratorEntry entries[3];
    entries[0].Set(wxACCEL_CTRL, (int) 'O',ListDialog_OK_Id);
    //entries[1].Set(wxACCEL_CTRL, (int) 'C',ID_BUTTON1);
    entries[1].Set(wxACCEL_NORMAL,WXK_ESCAPE,ListDialog_Cancel_Id);
    entries[2].Set(wxACCEL_CTRL,WXK_RETURN,ListDialog_OK_Id);
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
    if (event.GetId()==ListDialog_OK_Id) EndModal(wxID_OK);
    else EndModal(wxID_CANCEL);
}

void ListDialog::OnListBoxEvent(wxCommandEvent&)
{
    EndModal(wxID_OK);
}



// ----------------------------------------------------------------------------------------------------------------------------------------------


}  // end namespace val
