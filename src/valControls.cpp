#include "valControls.h"
#include <wx/accel.h>
#include <wx/msgdlg.h>
#include <wx/string.h>
#include <val_utils.h>

//wxDEFINE_EVENT(IS_EVENT,ISliderEvent);
wxDEFINE_EVENT(val_EVENT_SLIDER,wxCommandEvent);
wxDEFINE_EVENT(val_EVENT_SWITCH,wxCommandEvent);
wxDEFINE_EVENT(val_EVENT_COMPLETE,wxCommandEvent);



namespace val
{

enum {IntervalSlider_UP_Id = 32500, IntervalSlider_DOWN_Id, IntervalSlider_ALTUP_id, IntervalSlider_ALTDOWN_Id,
        IntervalSlider_CTRLUP_Id, IntervalSlider_CTRLDOWN_Id, CompleteCtrl_UP_Id, CompleteCtrl_DOWN_Id,
        CompleteCtrl_RETURN_Id, CompleteCtrl_ESCAPE_Id, CompleteCtrl_TAB_Id, CompleteCtrl_BRACKETS_Id};

int isdarkcolor(const wxColour& color)
{
    double r = color.Red(), g = color.Green(), b = color.Blue(), hsp;

    hsp = 0.299 * (r * r) + 0.587 * (g * g) + 0.114 * (b * b);
    if (hsp <= 16256.25) return 1;
    else return 0;
}


int getnumberofdigits(double lvalue,double mvalue,const double& steps)
{
    int dl=1,dm=1,d=1;
    double w=1;

    if (lvalue<0) ++dl;
    lvalue= val::abs(lvalue);
    mvalue= val::abs(mvalue);
    while (w<lvalue) {
        w*=10;
        ++dl;
    }
    w=1;
    while (w<mvalue) {
        w*=10;
        ++dm;
    }
    d = val::Max(dl,dm);
    w=1;
    while (w>steps) {
        w*=0.1;
        ++d;
    }

    return d;
}


int getnumberofdecimals(const double& s)
{
    int dec=0;
    double w = 1;

    while (s<w) {
        w*=0.1;
        ++dec;
    }
    return dec;
}


int starts_with(const std::string &input, const std::string &pattern)
{
    int n = input.size(), m = pattern.size();

    if (m>n) return 0;
    for (int i = 0; i < m; ++i) {
        if (input[i]!=pattern[i]) return 0;
    }

    return 1;
}


val::d_array<std::string> getlist_of_starts_with(const val::d_array<std::string> &G, const std::string &pattern)
{
    val::d_array<std::string> H;
    H.reserve(G.length());
    if (G.isempty() || pattern == "") return H;

    for (const auto &s : G) {
        if (starts_with(s,pattern)) H.push_back(s);
    }

    return H;
}




// -----------------------------------------------------------------------------------------------------------------------

SpinCtrl::SpinCtrl(wxWindow *parent,wxWindowID id,const double& minvalue,const double& maxvalue,const double& dec,const double& dvalue)
            //wxSpinCtrlDouble(parent,id,"",wxDefaultPosition,wxDefaultSize,0x400|wxALIGN_RIGHT,minvalue,maxvalue,dvalue,dec)
{
    int digits = getnumberofdigits(minvalue,maxvalue,dec),x=(digits+1)*13;
    if (x<130) x= 130;
    //if (digits > 4) x += (digits-4)*13;
    Create(parent,id,"",wxDefaultPosition,wxSize(x,35),0x4000|wxALIGN_RIGHT,minvalue,maxvalue,dvalue,dec);
}

// ------------------------------------------------------------------------------------------------------------------------

Slider::Slider(wxWindow *parent,wxWindowID id,const double& dval,const double& minv,const double &maxv,const double& step,const wxPoint &pos,
        const wxSize &Size,const int style) : wxControl(parent,id,pos,wxDefaultSize,wxBORDER_NONE), Parent(parent), identity(id), Style(style),
            value(dval), Minvalue(minv), Maxvalue(maxv), Step(step),
            sizex(Size.x), sizey(Size.y)
{
    //if (Style != wxHORIZONTAL && Style != wxVERTICAL) Style = wxHORIZONTAL;
    if (Style & wxHORIZONTAL) {
        Style = wxSL_HORIZONTAL;
        sizey =50;
    }
    if (Style & wxVERTICAL) {
        Style = wxSL_VERTICAL | wxSL_INVERSE;
        if (sizex<50) sizex=50;
    }
    SetLimits();
    DrawPanel = new wxPanel(this,100,wxDefaultPosition,wxSize(sizex,sizey));
    wxslider = new wxSlider(DrawPanel,200,0,0,100,wxDefaultPosition,wxSize(sizex,sizey),Style);
    SetSliderValues();

    DrawPanel->Bind(wxEVT_PAINT,&Slider::OnDrawPanelPaint,this);
    DrawPanel->Bind(wxEVT_SIZE,&Slider::OnDrawPanelResize,this);
    Bind(wxEVT_SLIDER,&Slider::OnSliderEvent,this,200);

    Paint();
}


void Slider::SetLimits()
{
    if (Maxvalue<Minvalue) Maxvalue = Minvalue+1;
    if (Step<0.0001) Step = 0.0001;
    digits = getnumberofdigits(Minvalue,Maxvalue,Step);
    decimals = getnumberofdecimals(Step);
    if (value<Minvalue) value = Minvalue;
    if (value>Maxvalue) value = Maxvalue;
    s_min = val::ToString(Minvalue); s_max = val::ToString(Maxvalue);
}

void Slider::SetSliderValues()
{
    range = int(val::round((Maxvalue-Minvalue)/Step,0));
    svalue = int(val::round((value-Minvalue)/Step,0));
    if (svalue<0) svalue =0;
    if (svalue>range) svalue = range;
    wxslider->SetRange(0,range);
    wxslider->SetValue(svalue);
    //wxMessageBox(val::ToString(range));
}

void Slider::OnSliderEvent(wxCommandEvent&)
{
    if (slidereventnotready) return;
    //spinbutton->SetEvtHandlerEnabled(false);
    slidereventnotready=1;
    int nvalue = wxslider->GetValue();

    //if (nvalue>svalue) value+=Step;
    //else if (nvalue<svalue) value-=Step;

    value = Minvalue + double(nvalue)*Step;

    if (value>Maxvalue) {
        value=Maxvalue;
        svalue=range;
    }
    else if (value<Minvalue) {
        value = Minvalue;
        svalue = 0;
    }
    else svalue = nvalue;

    wxslider->SetValue(svalue);
    Paint();
    SendEvent();
    slidereventnotready=0;
    //spinbutton->SetEvtHandlerEnabled(true);

}


void Slider::Paint()
{
    wxClientDC dc(DrawPanel);
    std::string text(val::ToString(value));
    //int x = int(val::round(double(sizex)*double(svalue)/double(range),0));
    if (Style & wxHORIZONTAL) {
        int dx=9,x = 2+ int(val::round(double(sizex-2*dx)*double(svalue)/double(range),0)),stext=text.length();

        if (stext>2) x-=(stext-2)*10;
        if (x<0) x=0;

        //dc.SetBackground(wxBrush(backgroundcolor));
        dc.SetPen(wxPen(backgroundcolor,20));
        dc.SetTextForeground(passivtext);
        dc.DrawText(val::ToString(Minvalue),2,sizey-18);
        dc.DrawText(val::ToString(Maxvalue),sizex-10*val::ToString(Maxvalue).length(),sizey-18);
        dc.DrawLine(0,5,sizex,5);
        dc.SetTextForeground(textcolor);
        dc.DrawText(val::ToString(value),x,3);
    }
    else {
        int mid = sizex/2,dy=8, y = sizey-15 - int(val::round(double(sizey-2*dy)*double(svalue)/double(range),0));
        if (value==Maxvalue) y=0;
        dc.SetTextForeground(passivtext);
        dc.DrawText(s_min,mid-6 - 10*s_min.length(),sizey-15);
        dc.DrawText(s_max,mid-6 - 10*s_max.length(),0);
        dc.SetPen(wxPen(backgroundcolor,digits*10));
        dc.DrawLine(mid+20,0,mid+20,sizey);
        dc.SetTextForeground(textcolor);
        dc.DrawText(text,mid+8,y);
    }
}

void Slider::OnDrawPanelPaint(wxPaintEvent&)
{
    Paint();
}

void Slider::OnDrawPanelResize(wxSizeEvent&)
{
    Paint();
}


void Slider::SendEvent()
{
    //wxMessageBox("sending Event");
    if (Parent== nullptr) wxMessageBox("Parent is null");
    //ISliderEvent S_event(IS_EVENT,leftvalue,rightvalue);
    wxCommandEvent S_event(val_EVENT_SLIDER,identity);
    wxPostEvent(Parent,S_event);
}

void Slider::SetValue(const double& dval)
{
    if (dval < Minvalue) value = Minvalue;
    else if (dval > Maxvalue) value = Maxvalue;
    else value = val::round(dval,decimals);
    svalue = int(val::round((value-Minvalue)/Step,0));
    wxslider->SetValue(svalue);
    //SendEvent();
}


// -----------------------------------------------------------------------------------------------------------------------

IntervalSlider::IntervalSlider(wxWindow *parent,wxWindowID id,int p_size,int b_height,int lv,int uv) : wxControl(parent,id),
                Parent(parent),identity(id),panelsize(p_size),barheight(b_height),lvalue(lv),uvalue(uv),leftvalue(lv),rightvalue(uv)
{
    if (panelsize<minsize) panelsize=minsize;
    if (barheight<8) barheight=8;
    if (barheight > 20) barheight =20;
    dx=barheight-3;
    barwidth = panelsize-2*dx; //barheight=20 - 2*dy;
    height = barheight+22;
    s_lvalue = val::ToString(lvalue); s_uvalue = val::ToString(uvalue);

    {
        int r=barcolor.Red(),g=barcolor.Green(),b=barcolor.Blue();
        r-=20;g-=20;b-=20;
        r = val::Max(0,r); g = val::Max(0,g); b = val::Max(0,b);
        pointcolor = wxColour(r,g,b);
    }
    wxPanel *MPanel = new wxPanel(this,-1);
    wxBoxSizer *BoxSizer=new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *VBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    DrawPanel = new wxPanel(MPanel,100, wxDefaultPosition, wxSize(panelsize,height),wxBORDER_NONE);
    BoxSizer->Add(DrawPanel,0,wxALL|wxALIGN_LEFT,2);
    LSpin = new wxSpinCtrl(MPanel,201,val::ToString(leftvalue),wxDefaultPosition,wxSize(120,35),0x4000|wxALIGN_RIGHT,lvalue,uvalue,leftvalue);
    RSpin = new wxSpinCtrl(MPanel,202,val::ToString(rightvalue),wxDefaultPosition,wxSize(120,35),0x4000|wxALIGN_RIGHT,lvalue,uvalue,rightvalue);
    VBoxSizer->Add(LSpin,0,wxALL|wxALIGN_LEFT,2);
    VBoxSizer->AddStretchSpacer(1);
    //VBoxSizer->Add(RSpin,0,wxALL|wxALIGN_RIGHT,2);
    VBoxSizer->Add(RSpin,0,wxALL,2);
    BoxSizer->Add(VBoxSizer,0,wxALL|wxEXPAND);
    MPanel->SetSizer(BoxSizer);
    BoxSizer->Fit(MPanel);
    BoxSizer->SetSizeHints(this);

    DrawPanel->Bind(wxEVT_PAINT,(wxObjectEventFunction)&IntervalSlider::OnDrawPanelPaint,this);
    DrawPanel->Bind(wxEVT_SIZE,(wxObjectEventFunction)&IntervalSlider::OnDrawPanelResize,this);

    Bind(wxEVT_SPINCTRL,&IntervalSlider::OnSpinCtrl,this,201);
    Bind(wxEVT_SPINCTRL,&IntervalSlider::OnSpinCtrl,this,202);

    DrawPanel->Bind(wxEVT_LEFT_DOWN,&IntervalSlider::OnMouseCaptured,this);
    DrawPanel->Bind(wxEVT_LEFT_UP,&IntervalSlider::OnMouseReleased,this);
    DrawPanel->Bind(wxEVT_MOUSE_CAPTURE_LOST,&IntervalSlider::OnLostMouse,this);

    Bind(wxEVT_COMMAND_MENU_SELECTED,&IntervalSlider::OnArrowsPressed,this,IntervalSlider_UP_Id);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&IntervalSlider::OnArrowsPressed,this,IntervalSlider_DOWN_Id);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&IntervalSlider::OnArrowsPressed,this,IntervalSlider_ALTUP_id);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&IntervalSlider::OnArrowsPressed,this,IntervalSlider_ALTDOWN_Id);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&IntervalSlider::OnArrowsPressed,this,IntervalSlider_CTRLUP_Id);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&IntervalSlider::OnArrowsPressed,this,IntervalSlider_CTRLDOWN_Id);

    wxAcceleratorEntry entries[6];
    entries[0].Set(wxACCEL_NORMAL,WXK_UP,IntervalSlider_UP_Id);
    entries[1].Set(wxACCEL_NORMAL,WXK_DOWN,IntervalSlider_DOWN_Id);
    entries[2].Set(wxACCEL_ALT,WXK_UP,IntervalSlider_ALTUP_id);
    entries[3].Set(wxACCEL_ALT,WXK_DOWN,IntervalSlider_ALTDOWN_Id);
    entries[4].Set(wxACCEL_CTRL,WXK_UP,IntervalSlider_CTRLUP_Id);
    entries[5].Set(wxACCEL_CTRL,WXK_DOWN,IntervalSlider_CTRLDOWN_Id);
    //entries[6].Set(wxACCEL_ALT,WXK_RIGHT,20003);
    //entries[7].Set(wxACCEL_ALT,WXK_LEFT,20004);

    //entries[4].Set(wxACCEL_CTRL, (int) '+',20005);
    //entries[5].Set(wxACCEL_CTRL, (int) '-',20006);

    wxAcceleratorTable accel(6,entries);
    SetAcceleratorTable(accel);


    Paint();
}

void IntervalSlider::setlimits(int l_val,int r_val)
{
    if (l_val>=r_val) return;
    lvalue=l_val; uvalue=r_val;
    s_lvalue = val::ToString(lvalue); s_uvalue = val::ToString(uvalue);
    if (leftvalue < lvalue) leftvalue = lvalue;
    if (rightvalue > uvalue) rightvalue = uvalue;
    LSpin->SetValue(leftvalue); RSpin->SetValue(rightvalue);
    LSpin->SetRange(lvalue,uvalue);
    RSpin->SetRange(lvalue,uvalue);
    Paint();
    SendSliderEvent();
}

void IntervalSlider::setvalues(int l, int r)
{
    if (l >= r) return;
    if (l < lvalue) l = lvalue;
    if (r > uvalue) r = uvalue;
    leftvalue = l; rightvalue = r;
    LSpin->SetValue(leftvalue); RSpin->SetValue(rightvalue);
    Paint();
    SendSliderEvent();
}


void IntervalSlider::setallvalues(int l_l, int l_r, int l, int r)
{
    if ((l_l >= l_r) || (l >= r)) return;
    lvalue = l_l; uvalue = l_r;
    leftvalue = l; rightvalue = r;
    if (leftvalue < lvalue) leftvalue = lvalue;
    if (rightvalue > uvalue) rightvalue = uvalue;
    s_lvalue = val::ToString(lvalue); s_uvalue = val::ToString(uvalue);
    LSpin->SetValue(leftvalue); RSpin->SetValue(rightvalue);
    LSpin->SetRange(lvalue,uvalue);
    RSpin->SetRange(lvalue,uvalue);
    Paint();
    SendSliderEvent();
}

IntervalSlider::~IntervalSlider()
{
    if (MouseCaptured) return;
}


void IntervalSlider::plotbar(wxDC &dc)
{
    x1 = dx + int((double(leftvalue - lvalue)/double(uvalue-lvalue))*double(barwidth));
    x2 = dx + int((double(rightvalue - lvalue)/double(uvalue-lvalue))*double(barwidth));

    dc.SetTextForeground(textcolor);
    dc.SetPen(wxPen(bracketcolor,barheight));
    dc.DrawLine(dx,dy+dy/2,x1,dy+dy/2);
    dc.DrawLine(x2,dy+dy/2,dx+barwidth,dy+dy/2);
    dc.SetPen(wxPen(barcolor,barheight));
    dc.DrawLine(x1,dy+dy/2,x2,dy+dy/2);
    //dc.DrawText(s_lvalue,dx,2*dy);
    dc.DrawText(s_lvalue,dx,height-18);
    /*
    dc.SetPen(wxPen(pointcolor,barheight));
    dc.DrawPoint(x1,dy+dy/2);
    dc.DrawPoint(x2,dy+dy/2);
    */
    dc.SetPen(wxPen(pointcolor,1));
    dc.SetBrush(wxBrush(pointcolor));
    dc.DrawCircle(x1,dy+dy/2,barheight/2);
    dc.DrawCircle(x2,dy+dy/2,barheight/2);
    //dc.DrawText(s_uvalue,dx+barwidth-8*s_uvalue.length(),2*dy);
    dc.DrawText(s_uvalue,dx+barwidth-8*s_uvalue.length(),height-18);
    //dc.DrawLine(dx,dy+dy/2,dx+barwidth,dy+dy/2);
}

void IntervalSlider::Paint()
{
    if (!ispainted) return;
    ispainted=0;
    wxClientDC dc1(DrawPanel);


    wxBitmap paper = wxBitmap(DrawPanel->GetSize());

    // Create a memory Device Context
    wxMemoryDC dc;
    // Tell memDC to write on “paper”.
    dc.SelectObject( paper );
    // Call Repin to draw our picture on memDC
    dc.SetBackground(wxBrush(backgroundcolor));
    //dc.SetFont(dc1.GetFont());
    //dc.SetBackground(wxBrush(barcolor));

    dc.Clear();

    plotbar(dc);

    dc.SelectObject( wxNullBitmap );
    dc1.DrawBitmap(paper,0,0);


    ispainted=1;
}

void IntervalSlider::OnArrowsPressed(wxCommandEvent &event)
{
    int id = event.GetId();
    switch (id)
    {
    case IntervalSlider_UP_Id:
        {
            leftvalue++;
            if (leftvalue > rightvalue) leftvalue = rightvalue;
            LSpin->SetValue(leftvalue);
        }
        break;
    case IntervalSlider_DOWN_Id:
        {
            leftvalue--;
            if (leftvalue<lvalue) leftvalue++;
            LSpin->SetValue(leftvalue);
        }
        break;
    case IntervalSlider_ALTUP_id:
        {
            rightvalue++;
            if (rightvalue > uvalue) rightvalue--;
            RSpin->SetValue(rightvalue);
        }
        break;
    case IntervalSlider_ALTDOWN_Id:
        {
            rightvalue--;
            if (rightvalue<leftvalue) rightvalue++;
            RSpin->SetValue(rightvalue);
        }
        break;
	case IntervalSlider_CTRLUP_Id:
		{
			rightvalue++;
			leftvalue--;
			if (rightvalue > uvalue) rightvalue = uvalue;
			if (leftvalue < lvalue) leftvalue = lvalue;
			RSpin->SetValue(rightvalue);
			LSpin->SetValue(leftvalue);
		}
		break;
	case IntervalSlider_CTRLDOWN_Id:
		{
			if (rightvalue == leftvalue) break;
			rightvalue--;
			leftvalue++;
			RSpin->SetValue(rightvalue);
			LSpin->SetValue(leftvalue);
		}
        break;
    default:
        break;
    }
    Paint();
    SendSliderEvent();
}


void IntervalSlider::OnSpinCtrl(wxSpinEvent &event)
{
    //wxMessageBox("Spin-Event");
    int id = event.GetId(),value;
    if (id==201) {
        value = LSpin->GetValue();
        if (value>rightvalue) {
            value = rightvalue;
            LSpin->SetValue(value);
            return;
        }
        if (value<lvalue) {
            value = lvalue;
            LSpin->SetValue(value);
            return;
        }
        leftvalue=value;
    }
    else {
        value = RSpin->GetValue();
        if (value<leftvalue) {
            value = leftvalue;
            RSpin->SetValue(value);
            return;
        }
        if (value>uvalue) {
            value = uvalue;
            RSpin->SetValue(value);
            return;
        }
        rightvalue=value;
    }


    //wxMessageBox(val::ToString(x1)+ " , " + val::ToString(x2));

    Paint();
    SendSliderEvent();
    //wxMessageBox(val::ToString(leftvalue)+ " , " + val::ToString(rightvalue));
}


void IntervalSlider::OnMouseCaptured(wxMouseEvent& event)
{
    int mouse_x=event.GetX(); //mouse_y=event.GetY();
    leftmoved=rightmoved=0;
    if (mouse_x <= x1+10 && mouse_x >= x1-10) leftmoved=1;
    if (mouse_x <= x2+10 && mouse_x >= x2-10) rightmoved=1;
    if (!leftmoved && !rightmoved) return;
    if (leftmoved && rightmoved) {
        if (val::abs(mouse_x-x1) <= val::abs(mouse_x-x2)) rightmoved=0;
        else leftmoved=0;
    }
    DrawPanel->CaptureMouse();
    DrawPanel->Bind(wxEVT_MOTION,&IntervalSlider::OnMouseMoved,this);
    MouseCaptured=1;
}

void IntervalSlider::OnMouseReleased(wxMouseEvent& event)
{
    DrawPanel->Unbind(wxEVT_MOTION,&IntervalSlider::OnMouseMoved,this);
    if (DrawPanel->HasCapture()) DrawPanel->ReleaseMouse();
    MouseCaptured=0;
    SendSliderEvent();
}

void IntervalSlider::OnMouseMoved(wxMouseEvent& event)
{
    int x=event.GetX();//y=event.GetY(),dx = ;
    if (leftmoved) {
        //x1+=difx;
        x1=x;
        if (x1<dx) x1=dx;
        if (x1>x2) {
			x1 = x2;
			leftvalue=rightvalue;
		}
        else leftvalue = int(val::round(double((x1-dx)*(uvalue-lvalue))/double(barwidth),0)) + lvalue;
        LSpin->SetValue(leftvalue);
    }
    else {
        x2=x;
        if (x2>dx+barwidth) x2=dx + barwidth;
        if (x2 < x1) {
			x2 = x1;
			rightvalue=leftvalue;
		}
        else rightvalue = int(val::round(double((x2-dx)*(uvalue-lvalue))/double(barwidth),0)) + lvalue;
        RSpin->SetValue(rightvalue);
    }

    Paint();
}

void IntervalSlider::OnLostMouse(wxMouseCaptureLostEvent &event)
{

}



void IntervalSlider::OnDrawPanelPaint(wxPaintEvent &event)
{
    if (!ispainted) return;
    //StatusBar1->SetStatusText(val::ToString(npainted));
#ifndef _WIN32
    //wxPaintDC dummy(DrawPanel);
    Paint();
#endif // _WIN32
}

void IntervalSlider::OnDrawPanelResize(wxSizeEvent&)
{
    if (!ispainted) return;
    Paint();
}



void IntervalSlider::SendSliderEvent()
{
    //wxMessageBox("sending Event");
    if (Parent== nullptr) wxMessageBox("Parent is null");
    //ISliderEvent S_event(IS_EVENT,leftvalue,rightvalue);
    wxCommandEvent S_event(val_EVENT_SLIDER,identity);
    wxPostEvent(Parent,S_event);
}

// ---------------------------------------------------------------------------------------------------------------------------

// ---------------------------------------------------------------------------------------------------------------------------
SwitchCtrl::SwitchCtrl(wxWindow *parent,wxWindowID id,bool active) : wxControl(parent,id,wxDefaultPosition,wxDefaultSize,wxBORDER_NONE),
            Parent(parent), identity(id), is_active(active)
{
    //wxStaticText *text = new wxStaticText(this,10,_("switxh"));
    //wxBoxSizer *BoxSizer = new wxBoxSizer(wxHORIZONTAL);
    DrawPanel = new wxPanel(this,100,wxDefaultPosition,wxSize(52,24),wxNO_BORDER);
    //BoxSizer->Add(DrawPanel,0,wxALL|wxALIGN_LEFT);
    //BoxSizer->Add(text,0,wxALL|wxALIGN_LEFT);

    DrawPanel->Bind(wxEVT_PAINT,(wxObjectEventFunction)&SwitchCtrl::OnDrawPanelPaint,this);
    DrawPanel->Bind(wxEVT_LEFT_DOWN,&SwitchCtrl::OnMouseClicked,this);

    {
        int r = fg_color.Red(), g = fg_color.Green(), b = fg_color.Blue();

        if (isdarkcolor(fg_color)) {
            //wxMessageBox("Dark Color");
            r+=plcolor; g+=plcolor ; b+=plcolor;
            r = val::Min(r,255); g = val::Min(g,255); b = val::Min(b,255);
            //wxMessageBox(val::ToString(r) + " , " + val::ToString(g) + " , " + val::ToString(b));
        }
        else {
            r-=plcolor; g-=plcolor ; b-=plcolor;
            r = val::Max(r,0); g = val::Max(g,0); b = val::Max(b,0);
        }
        barcolor_p = wxColour(r,g,b);
        if (parent != nullptr) backgroundcolor = parent->GetBackgroundColour();
    }

//#ifdef _WIN32
    wxBoxSizer *BoxSizer = new wxBoxSizer(wxHORIZONTAL);
    BoxSizer->Add(DrawPanel,0,wxALL|wxALIGN_LEFT);
    SetSizer(BoxSizer);
    BoxSizer->Fit(this);
    BoxSizer->SetSizeHints(this);
//#endif // _WIN32

    Paint();
}

void SwitchCtrl::SetValue(bool value)
{
    if (is_active==value) return;
    is_active = value;
    Paint();
    SendEvent();
}

void SwitchCtrl::ChangeValue(bool value)
{
    if (is_active==value) return;
    is_active = value;
    Paint();
}




void SwitchCtrl::Paint()
{
    wxClientDC dc1(DrawPanel);


    wxBitmap paper = wxBitmap(DrawPanel->GetSize());

    // Create a memory Device Context
    wxMemoryDC dc;
    // Tell memDC to write on “paper”.
    dc.SelectObject( paper );
    // Call Repin to draw our picture on memDC

    dc.SetBackground(wxBrush(backgroundcolor));
    //dc.SetFont(dc1.GetFont());
    //dc.SetBackground(wxBrush(barcolor));
    //dc.SetBrush(wxBrush(fg_color,wxBRUSHSTYLE_TRANSPARENT));

    dc.Clear();

    dc.SetBrush(wxBrush(fg_color));


    dc.SetTextForeground(fg_color);
    if (is_active) {
        dc.SetPen(wxPen(barcolor_a,17));
        dc.DrawLine(12,11,40,11);
        dc.SetPen(wxPen(fg_color,1));
//#ifdef _WIN32
        dc.DrawCircle(38,11,8);
        //dc.FloodFill(38,11,backgroundcolor);
//#else
        //dc.DrawPoint(38,11);
//#endif // _WIN32
        dc.DrawText(checkmark,10,3);
    }
    else {
        dc.SetPen(wxPen(barcolor_p,17));
        dc.DrawLine(12,11,40,11);
        dc.SetPen(wxPen(fg_color,1));
//#ifdef _WIN32
        dc.DrawCircle(14,11,8);
//#else
        //dc.DrawPoint(14,11);
//#endif
        dc.DrawText(xmark,36,3);
    }

    dc.SelectObject( wxNullBitmap );
    dc1.DrawBitmap(paper,0,0);
}


void SwitchCtrl::OnMouseClicked(wxMouseEvent&)
{
    if (is_active) is_active=false;
    else is_active=true;

    Paint();
    SendEvent();
}

void SwitchCtrl::SendEvent()
{
    wxCommandEvent event(val_EVENT_SWITCH,identity);
    if (Parent) wxPostEvent(Parent,event);
}

void SwitchCtrl::OnDrawPanelPaint(wxPaintEvent& event)
{
    Paint();
}

// ---------------------------------------------------------------------------------------------------------------------------

/*
CompleteCtrl::CompleteCtrl(wxWindow *parent, wxWindowID id,const val::d_array<std::string> &list,const wxString &value, const wxSize &size,
                           const wxPoint &pos,long style) : wxControl(parent,id,pos), WordList(list)
{

    input = new wxTextCtrl(this,11,value,wxDefaultPosition,size,style);
    //listbox = new wxListBox(this,12);
    fontsize = input->GetFont().GetPointSize();
    listbox = new TopListbox(this);
    listbox->Show(false);
    //input->SetFocus();
    wxBoxSizer *boxsizer = new wxBoxSizer(wxVERTICAL);
    boxsizer->Add(input,1,wxALL|wxEXPAND,0);
    SetSizer(boxsizer);
    boxsizer->Fit(this);


    Bind(wxEVT_TEXT,&CompleteCtrl::OnInputChanged,this,11);
    Bind(val_EVENT_COMPLETE,&CompleteCtrl::OnCompleteBrackets,this);
    //Bind(wxEVT_SET_FOCUS,&CompleteCtrl::OnFocusEvent,this);
    //Bind(wxEVT_KILL_FOCUS,&CompleteCtrl::OnLostFocusEvent,this);
    //Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteCtrl::OnListoxSelected,this,101);
    //Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteCtrl::OnListoxSelected,this,102);
    //Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteCtrl::OnListoxSelected,this,103);



    wxAcceleratorEntry entries[4];
    entries[0].Set(wxACCEL_NORMAL,WXK_UP,CompleteCtrl_UP_Id);
    entries[1].Set(wxACCEL_NORMAL,WXK_DOWN,CompleteCtrl_DOWN_Id);
    entries[2].Set(wxACCEL_NORMAL,WXK_RETURN,CompleteCtrl_RETURN_Id);
    entries[3].Set(wxACCEL_NORMAL,WXK_ESCAPE,CompleteCtrl_ESCAPE_Id);
    accel = new wxAcceleratorTable(4,entries);
    //SetAcceleratorTable(*accel);

    parenttable = parent->GetAcceleratorTable();

}

bool CompleteCtrl::SetFont(const wxFont& font)
{
    fontsize = font.GetPointSize();
    wxFont lfont(font);
    lfont.SetPointSize(fontsize-1);
    listbox->SetFont(lfont);
    //wxMessageBox(val::ToString(listbox->GetFont().GetPointSize()));
    return input->SetFont(font);
}


CompleteCtrl::~CompleteCtrl()
{
    delete accel;
}




void CompleteCtrl::OnInputChanged(wxCommandEvent &)
{
    UnbindAll();
    listbox->Show(false);
    listbox->Clear();
    n_candidates = 0;
    isactiv = 0;
    selection = 0;
    std::string word = std::string(input->GetValue());
    int n = input->GetInsertionPoint();
    if (!input->IsSingleLine()) --n;
    //wxMessageBox(val::ToString(n));
    if (n<0) n = 0;

    formerlength = actuallength;
    actuallength = word.length();

    if (closebrackets && word[n] == '(' && actuallength > formerlength) {
        actualword = "";
		//if (n!=m && word[m+1] == ')') return;
		wxCommandEvent event(val_EVENT_COMPLETE,0);
		wxPostEvent(this,event);
		return;
	}


    //if (m!=n-1) wxMessageBox(findword(word,m-1,startpos));
    if (word[n] >= 97 && word[n] <= 122) {
        //actualword += word[n-1];
        actualword = findword(word,n);
        CandList = getlist_of_starts_with(WordList,actualword);
        n_candidates = CandList.length();
        if (CandList.isempty()) return;
        int anz = n_candidates;
        if (anz > 8) anz =8;
        isactiv = 1;
        for ( const auto & v : CandList) listbox->Append(v);
        wxPoint pos = input->PositionToCoords(n), screenpos = input->GetScreenPosition();
        //listbox->SetFont(input->GetFont());
        //fontsize = input->GetFont().GetPointSize();
        listbox->Move(screenpos.x+pos.x,screenpos.y+pos.y+20);
        listbox->SetSize(200,anz * (fontsize + 15));
        listbox->SetSelection(selection);
        listbox->Show(true);
        BindAll();
        //listbox->SetFocus();
    }
    else actualword = "";
}


void CompleteCtrl::OnListBoxSelected(wxCommandEvent &evt)
{
    if (!isactiv) return;
    int id = evt.GetId();
    switch (id)
    {
    case CompleteCtrl_UP_Id:
        {
            selection--;
            if (selection<0) selection = 0;
            listbox->SetSelection(selection);
        }
        break;
    case CompleteCtrl_DOWN_Id:
        {
            selection++;
            if (selection >= n_candidates) selection = n_candidates -1;
            listbox->SetSelection(selection);
        }
        break;
    case CompleteCtrl_RETURN_Id:
        {
            CompleteWord();
            listbox->Show(false);
            isactiv = 0;
            UnbindAll();
        }
        break;
    case CompleteCtrl_ESCAPE_Id:
        {
            listbox->Show(false);
            isactiv = 0;
            UnbindAll();
        }

    default:
        break;
    }

}

void CompleteCtrl::OnCompleteBrackets(wxCommandEvent&)
{
    CompleteBrackets();
}


void CompleteCtrl::CompleteWord()
{
    std::string &fullword = CandList[selection],suffix;
    int i,m = actualword.length(), n = fullword.length();

    for (i = m; i < n ; ++i) suffix += fullword[i];
    input->WriteText(suffix);
    actualword = "";
}


void CompleteCtrl::CompleteBrackets()
{
    input->WriteText(")");
    input->SetInsertionPoint(input->GetInsertionPoint()-1);
}

void CompleteCtrl::BindAll()
{
    if (parenttable != nullptr)  SetAcceleratorTable(*accel);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteCtrl::OnListBoxSelected,this,CompleteCtrl_UP_Id);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteCtrl::OnListBoxSelected,this,CompleteCtrl_DOWN_Id);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteCtrl::OnListBoxSelected,this,CompleteCtrl_RETURN_Id);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteCtrl::OnListBoxSelected,this,CompleteCtrl_ESCAPE_Id);
}

void CompleteCtrl::UnbindAll()
{
    if (parenttable!=nullptr)  SetAcceleratorTable(*parenttable);
    Unbind(wxEVT_COMMAND_MENU_SELECTED,&CompleteCtrl::OnListBoxSelected,this,CompleteCtrl_UP_Id);
    Unbind(wxEVT_COMMAND_MENU_SELECTED,&CompleteCtrl::OnListBoxSelected,this,CompleteCtrl_DOWN_Id);
    Unbind(wxEVT_COMMAND_MENU_SELECTED,&CompleteCtrl::OnListBoxSelected,this,CompleteCtrl_RETURN_Id);
    Unbind(wxEVT_COMMAND_MENU_SELECTED,&CompleteCtrl::OnListBoxSelected,this,CompleteCtrl_ESCAPE_Id);
}
*/

// ------------------------------------------------------------------------------------------------------------------------------------------


CompleteTextCtrl::CompleteTextCtrl(wxWindow *parent, wxWindowID id,const val::d_array<wxString> &list,const wxString &value, const wxSize &size,
                                   const wxPoint &pos,long style) : wxTextCtrl(parent,id,value,pos,size,style), Parent(parent), identity(id), WordList(list,58,int('A'))
{
	BuildObject();
}


CompleteTextCtrl::CompleteTextCtrl(wxWindow *parent, wxWindowID id,const val::trie_type<wxString> &list,const wxString &value, const wxSize &size,
                                   const wxPoint &pos,long style) : wxTextCtrl(parent,id,value,pos,size,style), Parent(parent), identity(id),
                                   beg(list.first_valid_char()), end(list.last_valid_char()), WordListPointer(&list)
{
	BuildObject();
}



void CompleteTextCtrl::BuildObject()
{
    fontsize = GetFont().GetPointSize();
    listbox = new TopListbox(this);
    listbox->Show(false);
    dc.SetFont(GetFont());


    Bind(wxEVT_TEXT,&CompleteTextCtrl::OnInputChanged,this);
    Bind(wxEVT_CHAR,&CompleteTextCtrl::OnKeyChar,this);
    Bind(wxEVT_KEY_DOWN,&CompleteTextCtrl::OnKeyDown,this);
    Bind(val_EVENT_COMPLETE,&CompleteTextCtrl::OnCompleteBrackets,this);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteTextCtrl::OnShortCuts,this,101);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteTextCtrl::OnShortCuts,this,102);

    wxAcceleratorEntry entries[5];
    entries[0].Set(wxACCEL_NORMAL,WXK_UP,CompleteCtrl_UP_Id);
    entries[1].Set(wxACCEL_NORMAL,WXK_DOWN,CompleteCtrl_DOWN_Id);
#ifdef _WIN32
    entries[2].Set(wxACCEL_NORMAL,WXK_RETURN,CompleteCtrl_RETURN_Id);
#else
    entries[2].Set(wxACCEL_NORMAL,WXK_RETURN,CompleteCtrl_RETURN_Id);
#endif // _WIN32
    entries[3].Set(wxACCEL_NORMAL,WXK_ESCAPE,CompleteCtrl_ESCAPE_Id);
    entries[4].Set(wxACCEL_NORMAL,WXK_TAB,CompleteCtrl_TAB_Id);
    accel = new wxAcceleratorTable(5,entries);

    wxAcceleratorEntry t_entries[2];
    t_entries[0].Set(wxACCEL_CTRL,(int) 'B',101);
    t_entries[1].Set(wxACCEL_CTRL,(int) 'D',102);
    taccel = new wxAcceleratorTable(2,t_entries);

    SetAcceleratorTable(*taccel);

    parenttable = Parent->GetAcceleratorTable();
}



int CompleteTextCtrl::isalphabetical(wxChar s) const
{
    //if ( (s >= 65 && s<= 90 ) || (s >= 97 && s <= 122 ) ) return 1;
    //else return 0;
    if (s >= beg && s <= end) return 1;
    else return 0;
}

wxString CompleteTextCtrl::findword(const wxString& input,int pos) const
{
    int start_position;
    wxString word = "";
    for (start_position = pos; start_position >= 0; --start_position) {
        //if (input[start_position]<97 || input[start_position] > 122) break;
        if (!(isalphabetical(input[start_position]))) break;
    }
    if (start_position == pos ) return word;
    start_position++;
    for (int i = start_position; i <=pos ; ++i) word += input[i];
    return word;
}



bool CompleteTextCtrl::SetFont(const wxFont& font)
{
    fontsize = font.GetPointSize();
    wxFont lfont(font);
    lfont.SetPointSize(fontsize-1);
    listbox->SetFont(lfont);
    //wxMessageBox(val::ToString(listbox->GetFont().GetPointSize()));
    dc.SetFont(font);
    return wxTextCtrl::SetFont(font);
}


CompleteTextCtrl::~CompleteTextCtrl()
{
    delete accel;
    delete taccel;
}


void CompleteTextCtrl::OnInputChanged(wxCommandEvent &tevent)
{
    //tevent.Skip();
    UnbindAll();
    listbox->Show(false);
    listbox->Clear();
    n_candidates = 0;
    isactiv = 0;
    selection = 0;
    wxString word = GetValue();
    int n = GetInsertionPoint();
    if (!IsSingleLine()) --n;
    //wxMessageBox(word + " , = " +val::ToString(n));
    if (n<0) n = 0;

    formerlength = actuallength;
    actuallength = word.length();
    if (n > actuallength - 1) n = actuallength - 1;
#ifdef _WIN32
    int line = 0;
    for (int i= 0; i<n; ++i) {
        if (word[i] == '\n') {
            line++;
            //n--;
            //if (n<0) n = 0;
            //break;
        }
    }
    if (n<0) n = 0;
#endif // _WIN32

    if (closebrackets && (word[n] == '(' || word[n] == '{' || word[n] == '[') && actuallength > formerlength) {
        if (word[n] == '(') bracket = ")";
        else if (word[n] == '[') bracket = "]";
        else bracket = "}";
        actualword = "";
        //if (n!=m && word[m+1] == ')') return;
        wxCommandEvent event(val_EVENT_COMPLETE,0);
        wxPostEvent(this,event);
        if (Parent != nullptr) {
            wxCommandEvent pevent(wxEVT_TEXT,identity);
            wxPostEvent(Parent,pevent);
        }
        return;
    }

    //if (m!=n-1) wxMessageBox(findword(word,m-1,startpos));
    //if (word[n] >= 97 && word[n] <= 122) {

    if (enablecomplete && isalphabetical(word[n])) {
        //actualword += word[n-1];
        actualword = findword(word,n);
        //std::cout << "\n" << actualword << std::endl;
        //CandList = getlist_of_starts_with(WordList,actualword);
        if (WordListPointer == nullptr) CandList = WordList.getmatchingprefix<d_array>(actualword);
        else CandList = WordListPointer->getmatchingprefix<d_array>(actualword);
        n_candidates = CandList.length();
        if (CandList.isempty()) return;
        int anz = n_candidates;
        if (anz > 8) anz =8;
        isactiv = 1;
        for ( const auto & v : CandList) listbox->Append(v);
        wxPoint pos, screenpos = GetScreenPosition();
#ifndef __APPLE__
        if (!IsSingleLine()) pos = PositionToCoords(n);
        else {
#endif // __APPLE__
        long x,y;
        int faktor = fontsize - 1;
        if (faktor < 1) faktor = 1;
        PositionToXY(n,&x,&y);
        {
            wxString line = GetLineText(y);
            line.resize(int(x) + 1);
            wxSize size = dc.GetTextExtent(line);
            pos.x = size.x;
        }
        wxSize sz = GetClientSize();
        pos.y = int(y)*(faktor + 8);
        if (pos.y > sz.y-20) pos.y = sz.y-20;
        if (pos.x > sz.x-10) pos.x = sz.x-10;
#ifndef __APPLE__
        }
#endif // __APPLE__

#ifdef _WIN32
        if (line && n>0 && word[n-1] == '\n') pos = PositionToCoords(n+line);
#endif // _WIN32
        if (IsSingleLine()) {
			pos.y = 5;
		}
        //listbox->SetFont(input->GetFont());
        //fontsize = input->GetFont().GetPointSize();
        int ps = 15;
        if (anz < 2) ps = 16;
        listbox->Move(screenpos.x+pos.x,screenpos.y+pos.y+20);
        listbox->SetSize(200,anz * (fontsize + ps));
        listbox->SetSelection(selection);
        listbox->Show(true);
        BindAll();
        //listbox->SetFocus();
    }
    else actualword = "";
    if (Parent != nullptr) {
        wxCommandEvent pevent(wxEVT_TEXT,identity);
        wxPostEvent(Parent,pevent);
    }
}


void CompleteTextCtrl::DuplicateLine()
{
    if (IsSingleLine()) return;
    wxString value = GetValue();
    if (value.length() == 0) return;
    long n = GetInsertionPoint(), x, y, linenumber = 1;

    PositionToXY(n, &x, &y);

    wxString line = GetLineText(y), nvalue = "";

    if (line.length() == 0) return;
    // Get line number;
    for (unsigned i = 0; i < value.length(); ++i) {
        if (value[i] == '\n') ++linenumber;
    }
    long i;
    //wxMessageBox(val::ToString(y) + " " + val::ToString(linenumber));
    for (i = 0; i <= y; ++i) nvalue += GetLineText(i) + "\n";
    nvalue += line;
    for (; i < linenumber; ++i) nvalue += "\n" + GetLineText(i);

    bool stat = enablecomplete;

    enablecomplete = false;
    SetValue(nvalue);
    SetInsertionPoint(XYToPosition(0,y+1));
    enablecomplete = stat;
}



void CompleteTextCtrl::OnShortCuts(wxCommandEvent &event)
{
    int id = event.GetId();
    if (id == 101) {
        if (closebrackets) closebrackets = false;
        else closebrackets = true;
    }
    else if (id == 102) {
    	DuplicateLine();
    }
}


void CompleteTextCtrl::OnListBoxSelected(wxCommandEvent &evt)
{
    if (!isactiv) return;
    int id = evt.GetId();
    switch (id)
    {
    case CompleteCtrl_UP_Id:
        {
            selection--;
            if (selection<0) selection = 0;
            listbox->SetSelection(selection);
        }
        break;
    case CompleteCtrl_DOWN_Id:
        {
            selection++;
            if (selection >= n_candidates) selection = n_candidates -1;
            listbox->SetSelection(selection);
        }
        break;
    case CompleteCtrl_RETURN_Id:
        {
            CompleteWord();
            listbox->Show(false);
            isactiv = 0;
            UnbindAll();
        }
        break;
    case CompleteCtrl_TAB_Id:
        {
            CompleteWord();
            listbox->Show(false);
            isactiv = 0;
            UnbindAll();
        }
        break;
    case CompleteCtrl_ESCAPE_Id:
        {
            listbox->Show(false);
            isactiv = 0;
            UnbindAll();
        }
        break;
    default:
        break;
    }

}

void CompleteTextCtrl::CompleteWord()
{
    wxString &fullword = CandList[selection],suffix;
    int i,m = actualword.length(), n = fullword.length();

    for (i = m; i < n ; ++i) suffix += fullword[i];
    WriteText(suffix);
    actualword = "";
}


void CompleteTextCtrl::OnCompleteBrackets(wxCommandEvent&)
{
    CompleteBrackets();
}

void CompleteTextCtrl::CompleteBrackets()
{
    WriteText(bracket);
    SetInsertionPoint(GetInsertionPoint()-1);
}




void CompleteTextCtrl::BindAll()
{
    if (parenttable != nullptr)  {
        //SetAcceleratorTable(wxNullAcceleratorTable);
        SetAcceleratorTable(*accel);
        //Parent->SetAcceleratorTable(wxNullAcceleratorTable);
    }
    Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteTextCtrl::OnListBoxSelected,this,CompleteCtrl_UP_Id);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteTextCtrl::OnListBoxSelected,this,CompleteCtrl_DOWN_Id);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteTextCtrl::OnListBoxSelected,this,CompleteCtrl_RETURN_Id);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteTextCtrl::OnListBoxSelected,this,CompleteCtrl_ESCAPE_Id);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&CompleteTextCtrl::OnListBoxSelected,this,CompleteCtrl_TAB_Id);
}

void CompleteTextCtrl::UnbindAll()
{

    SetAcceleratorTable(*taccel);
    if (parenttable!=nullptr)  {
        Parent->SetAcceleratorTable(*parenttable);
        //SetAcceleratorTable(*oldtable);
    }
    Unbind(wxEVT_COMMAND_MENU_SELECTED,&CompleteTextCtrl::OnListBoxSelected,this,CompleteCtrl_UP_Id);
    Unbind(wxEVT_COMMAND_MENU_SELECTED,&CompleteTextCtrl::OnListBoxSelected,this,CompleteCtrl_DOWN_Id);
    Unbind(wxEVT_COMMAND_MENU_SELECTED,&CompleteTextCtrl::OnListBoxSelected,this,CompleteCtrl_RETURN_Id);
    Unbind(wxEVT_COMMAND_MENU_SELECTED,&CompleteTextCtrl::OnListBoxSelected,this,CompleteCtrl_ESCAPE_Id);
    Unbind(wxEVT_COMMAND_MENU_SELECTED,&CompleteTextCtrl::OnListBoxSelected,this,CompleteCtrl_TAB_Id);
}


void CompleteTextCtrl::OnKeyDown(wxKeyEvent &event)
{
    GetSelection(&from, &to);

    if (from != to) textisselected = 1;
    else textisselected = 0;

    s_text = GetStringSelection();
    event.Skip();
}

void CompleteTextCtrl::OnKeyChar(wxKeyEvent &event)
{
    if (!textisselected) {
        event.Skip();
        return;
    }
    int j = -1;
    wxChar key = event.GetUnicodeKey();

    for (int i = 0; i < o_brackets.length(); ++i) {
        if (key == o_brackets[i] || key == c_brackets[i]) j = i;
    }
    if (j != -1) {
        //wxMessageBox(s_text);
        s_text = o_brackets[j] + s_text;
        s_text += c_brackets[j];
#ifndef __APPLE__
        to = from;
#endif
        Replace(from, to, s_text);
        textisselected = 0;
    }
    else event.Skip();
}

} // end namespace val
