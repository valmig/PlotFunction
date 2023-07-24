#ifndef VALDIALOGS_H_INCLUDED
#define VALDIALOGS_H_INCLUDED


#include <wx/string.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//#include <wx/frame.h>
#include <string>
#include <wx/choicdlg.h>
#include <val_utils.h>
#include <Glist.h>
#include <d_array.h>
#include <wx/dynarray.h>
#include <wx/listbox.h>
#include <wx/panel.h>


namespace val
{
// ------------------------------------------------------------------------------------------------------------------------------

// Base class for a Dialog with a Cancel- and OK-button.

class DLL_PUBLIC BaseDialog : public wxDialog
{
public:
    BaseDialog(wxWindow *parent,wxWindowID id,const wxPoint& point=wxDefaultPosition,const wxString& title="");
    virtual ~BaseDialog() {}
protected:
    // Panel to add elements of derived class
    wxPanel *surface=new wxPanel(this,100);
    wxBoxSizer *BoxSizer=nullptr;
    // List of accelerators. Add new accelerators in derived class
    val::Glist<wxAcceleratorEntry> AccEntry;
    // Call this function in derived class, when all the accelerators are in AccEntry.
    virtual void SetAccelerator();
private:
    void OnButtonClicked(wxCommandEvent &event);
};


//-----------------------------------------MultiLineDialog---------------------------------------------------------------------------------

class DLL_PUBLIC MultiLineDialog: public wxDialog
{
	public:

		MultiLineDialog(wxWindow* parent,const std::string &Entry="",const std::string &SText="Entry Settings",int sx=240,int sy=100,
                  const std::string &title="",int fontsize=10,int singleline=0,int style=0);
		virtual ~MultiLineDialog();

		/*
		//Declarations(MultiLineDialog)
		wxButton* CancelButton;
		wxButton* OKButton;
		wxStaticText* StaticText1;
		wxTextCtrl* TextCtrl1;
		//
		*/
		std::string GetSettingsText() const;
		wxString GetValue() const;

	protected:

		//(*Identifiers(MultiLineDialog)
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL1;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		//*)

	private:
		int Style = 0;

		//Declarations(MultiLineDialog)
		wxButton* CancelButton;
		wxButton* OKButton;
		wxStaticText* StaticText1;
		wxTextCtrl* TextCtrl1;
		//

		//Handlers(MultiLineDialog)
		void OnOKButtonClick(wxCommandEvent& event);
		void OnCancelButtonClick(wxCommandEvent& event);
		//
};

// ---------------------------------------------------------------------------------------------------------------------------------------


// ------------------------------------------------------- InfoWindow ---------------------------------------------------------------------

class DLL_PUBLIC InfoWindow: public wxDialog
{
	public:
		InfoWindow(wxWindow* parent,const std::string &Entry="",const wxPoint &Position=wxDefaultPosition,
                    const wxSize &Size=wxDefaultSize,const std::string &title="",int fontsize=10,int multilline = 1);
		InfoWindow(wxWindow* parent,int &nchild,const std::string &Entry="",const wxPoint &Position=wxDefaultPosition,
                    const wxSize &Size=wxDefaultSize,const std::string &title="",int fontsize=10, int multiline = 1) : InfoWindow(parent,Entry,Position,Size,title,fontsize,multiline)
                    {
                        number = &nchild;
                        ++(*number);
                    }
		virtual ~InfoWindow();

	protected:
	private:
	    void OnClose(wxCloseEvent &event);
	    int *number=nullptr;

};
// --------------------------------------------------------------------------------------------------------------------------


// ----------------------------------- SingleChoiceDialog -------------------------------------------------------------------


template <template <typename> class C>
wxString* getwxStringarray(const C<std::string> &G)
{
    wxString *List=nullptr;
    if (G.isempty()) return List;

    List= new wxString[G.length()];

    int i=0;

    for (const auto& v : G) List[i++] = v;
    return List;
}



class DLL_PUBLIC SingleChoiceDialog : public wxSingleChoiceDialog
{
private:
    wxString *List=nullptr;
    //const wxString* getlist(const val::Glist<std::string>& G);
    //const wxString* getlist(const val::d_array<std::string>& G);
public:
    SingleChoiceDialog(wxWindow *parent,const std::string& message,const std::string& title,const val::Glist<std::string> &G,
                    const wxPoint& Pos = wxDefaultPosition,const wxSize& Size =wxSize(250,250) )
                    : wxSingleChoiceDialog(parent,message,title,G.length(),(List=getwxStringarray(G))) {SetClientSize(Size); Move(Pos);}
    SingleChoiceDialog(wxWindow *parent,const std::string& message,const std::string& title,const d_array<std::string> &G,
                    const wxPoint& Pos = wxDefaultPosition,const wxSize& Size =wxSize(250,250) )
                    : wxSingleChoiceDialog(parent,message,title,G.length(),(List=val::getwxStringarray(G))) {SetClientSize(Size); Move(Pos);}
    virtual ~SingleChoiceDialog() {delete[] List;}
};

// ----------------------------------------------------------------------------


// ------- MultichoiceDialog ------------------------------------------------------------


DLL_PUBLIC d_array<int> FromwxArray(const wxArrayInt& wA);

template <template <typename>  class C>
wxArrayInt TowxArray(const C<int>& a)
{
    wxArrayInt wA;
    for (const auto& v : a) wA.Add(v);
    return wA;
}

class DLL_PUBLIC MultiChoiceDialog : public wxMultiChoiceDialog
{
private:
    wxString* List=nullptr;
    int len = 0;
    void SetAccelerators();
    void OnKB(wxCommandEvent &);
public:
    MultiChoiceDialog(wxWindow *parent,const std::string& message, const std::string& title,const val::Glist<std::string> &G,
                    const wxPoint& Pos = wxDefaultPosition)
                    :wxMultiChoiceDialog(parent,message,title,G.length(),(List=getwxStringarray(G))), len(G.length()) {SetAccelerators(); Move(Pos);}
    MultiChoiceDialog(wxWindow *parent,const std::string& message, const std::string& title,const val::d_array<std::string> &G,
                    const wxPoint& Pos = wxDefaultPosition)
                    :wxMultiChoiceDialog(parent,message,title,G.length(),(List=getwxStringarray(G))), len(G.length()) {SetAccelerators(); Move(Pos);}
    d_array<int> GetSelections() const {return FromwxArray(wxMultiChoiceDialog::GetSelections());}
    void SetSelections(const d_array<int>& A) {wxMultiChoiceDialog::SetSelections(TowxArray(A));}
    void SetSelections(const Glist<int>& A) {wxMultiChoiceDialog::SetSelections(TowxArray(A));}
    virtual ~MultiChoiceDialog() {delete[] List;}
};


// -------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------

class DLL_PUBLIC ListBox : public wxListBox
{
private:
    wxString *List=nullptr;
public:
    ListBox(wxWindow* parent,wxWindowID id,const Glist<std::string> &G)
            : wxListBox(parent,id,wxDefaultPosition,wxDefaultSize,G.length(),(List = getwxStringarray(G))) {}
    virtual ~ListBox() {delete[] List;}
};
// -------------------------------------------------------------------------------
class DLL_PUBLIC ListDialog : public wxDialog
{
public:
    ListDialog(wxWindow* parent,const val::Glist<std::string> &Choices=val::Glist<std::string>(),const std::string &title="",const std::string &Entry="",
               int sx=240,int sy=100,int fontsize=10);
    virtual ~ListDialog();
    std::string GetText() const;
    int GetSelection() const;
    void SetSelection(int n) {if (listbox != nullptr) listbox->SetSelection(n);}
private:
    ListBox *listbox=nullptr;
    wxTextCtrl *TextCtrl1=nullptr;
    wxButton *CancelButton=nullptr,*OKButton=nullptr;
    void OnButtonClick(wxCommandEvent& event);
    void OnListBoxEvent(wxCommandEvent&);

};

// ------------------------------------------------------------------------------------------------------------------------
} // end namespace val

#endif // VALDIALOGS_H_INCLUDED
