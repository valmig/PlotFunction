/***************************************************************
 * Name:      PlotFunctionMain.h
 * Purpose:   Defines Application Frame
 * Author:    Miguel Valbuena ()
 * Created:   2018-09-28
 * License:   GPL v3.0
 **************************************************************/


#ifndef PLOTFUNCTIONMAIN_H
#define PLOTFUNCTIONMAIN_H

//(*Headers(PlotFunctionFrame)
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
//*)
#include <wx/notebook.h>
#include <wx/dcclient.h>
#include <d_array.h>
#include <pol.h>
#include <Glist.h>
#include <val_basics.h>
#include <rational.h>
#include <string>
#include "valControls.h"
#include "PlotFunction.h"

class MyThreadEvent;
class ParentEvent;
//class myfunction;
struct plotobject;


enum viewnum{VGRID, VXAXIS, VYAXIS, VSCALEX, VSCALEY};
enum settingsnum{XRANGE, YRANGE, ASCALE, GSCALE, ANAME, AFSIZE, PNSIZE, SMARGIN, PDEC, MOVINC, PARVAL, REGDEG};


void custom_error(const char* c);


class PlotFunctionFrame: public wxFrame
{
    public:

        PlotFunctionFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~PlotFunctionFrame();

    private:
        enum insert_type{NORMAL_I, POINTS_I, POLYGON_I, LINE_I, RECTANGLE_I, CIRCLE_I, TRIANGLE_I};

        //(*Identifiers(PlotFunctionFrame)
        static const long ID_PANEL1;
        //static const long idMenuQuit;
        //static const long ID_MENUITEM4;
        static const long ID_MENUITEM1;
        static const long ID_MENUITEM2;
        static const long ID_MENUITEM3;
        static const long ID_MENUITEM7;
        static const long ID_MENUITEM5;
        static const long ID_MENUITEM6;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(PlotFunctionFrame)
        //wxMenu* MenuSettings;
        //wxMenu* submenuaxis;
        //wxMenuItem* MenuItem4;
        //wxMenuItem* MenuSave;
        //wxMenuItem* MenuSize;
        wxMenuItem* Menu_xAxis;
        wxMenuItem* Menu_yAxis;
        wxPanel* DrawPanel;
        wxStatusBar* StatusBar1;
        //*)
        wxBoxSizer *BoxSizer1; // *BoxSizer2;
        val::CompleteTextCtrl *SideText = nullptr;

        wxMenu *Menu_functions,*colorsubmenu,*rightclickmenu, *rightclickfunctionsmenu;
        wxMenuItem *hideallmenu,*showallmenu,*deletelastmenu,*deleteallmenu,*x_scaleactiv,*y_scaleactiv,*gridactiv,*undomenu,*redomenu,*addfunction,
                    *addpointsmenu, *polygonpointsmenu, *linepointsmenu, *rectanglemenu , *circlemenu, *multicolormenu, *trianglemenu;
        int ispainted=0,N=0,points=1000,abst=10,sizex,sizey,yzero,yset=1,defaultsize=1,fontsize=10,nchildwindows=0;
        int settings=0,Posx=0,Posy=0,clientsize_x=0,clientsize_y=0,iter=50,dez=4,mouse_x1,mouse_y1,fillfunctions=0,pi_scale_x=0,pi_scale_y=0,histogrames = 0;//mouse_x2,mouse_y2;
        int n_fstring=1,a_fstring=0,axis_pen=2,grid_pen=1,iscomputing=0,MaxRecent=10, doubleclicked = 0;
        int regressiondegree=1,rounddrawingpoints=-2,nanalyzewindows=0,active_function=-1,pointactive=0,decimalx=0,decimaly=0;
        int moveinpointsx = 0, moveinpointsy = 0;
        int bitmapbackground = 0, axis_fontsize = 10, SideText_isshown = 0, widthSideText = 200;
        int notebook_isshown = 0, widthNoteBookPanel = 200, plusw = 12, dpanelinsertmode = NORMAL_I, n_points = 0, wraptext = 0, lastfocusedwindow = 1;
        long InfoStyle = wxRESIZE_BORDER;
        bool closebrackets = true, computedefaultobject = false;
        wxPoint actuallinepoint,actualpolygonpoint;
        val::rational pi_factor_x,pi_factor_y,g_pi_factor_x,g_pi_factor_y;
        double x1=-5,x2=5,y1=-5,y2=5,ymin=-5,ymax=5,x_scale=1,y_scale=1,gx_scale=0.5,gy_scale=0.5,delta=1e-8,zoom=1.0,mx1,mx2,my1,my2,movedx = 0.1, movedy = 0.1;
        double movex = 0, movey = 0;
        std::string Program_Name="PlotFunction",x_axis="x",y_axis="y",sx_scale="1",sy_scale="1",sgx_scale="0.5",sgy_scale="0.5",actual_filename="",actual_dirname="", sabst = "10",
                    saxis_fontsize = "10", sfontsize = "10", spoints = "1000", srounddrawingspoints = "-2", sregressiondegree = "1", sPanelx, sPanely,
                    smovedx = "0.1", smovedy = "0.1", sactive_function = "0";
        wxString SideText_Word;
        wxFont defaultFont;
        wxSize bitmapsize,actualPanelsize; //DialogInputSize{wxDefaultSize};
        wxBitmap actualBitmapBackground, *cpaper = nullptr;
        wxImage BackgroundImage;
        wxColour axis_color ,grid_color, defaultpaintcolor;
        //val::d_array<val::d_array<double> > farray;
        //val::d_array<val::d_array<val::d_array<double> > > curvearray;
        wxColour BackgroundColor = wxColour(255,255,255);
        plotobject defaultplotobject;
        val::d_array<wxColor> Color;
        val::d_array<wxFont> Font;
        val::d_array<wxMenuItem*> f_menu,c_menu;
        val::d_array<int> pen;//islinear;
        val::Glist<plotobject> F;
        val::Glist<val::rational> Parameter{val::rational(1)};
        //val::Glist<val::GPair<double> > x_range;
        val::d_array<std::string> functionstring{30};
        val::d_array<std::string> dpanelnotification{"Points", "Polygon", "Line", "Rectangle", "Circle", "Triangle"};
        //
        const val::d_array<val::d_array<const std::string*>> SettingsCurrent{{&sx_scale, &sy_scale}, {}, {&sgx_scale, &sgy_scale}, {}, {&spoints}, {&xstring, &ystring},
                                                                             {}, {}, {}, {}, {}, {}, {&sfontsize}, {}, {&sPanelx, &sPanely}, {&x_axis, &y_axis}, {&sregressiondegree}, {&srounddrawingspoints},
                                                                             {}, {}, {}, {}, {&sabst}, {&saxis_fontsize}, {}, {&smovedx, &smovedy}, {&sactive_function}};
        //
        wxMenu* MenuRecentfiles;
        val::Glist<wxMenuItem*> recent_menu;
        val::Glist<std::string> recentfiles;
        //
        wxCursor *closeHand=nullptr;
        //
        wxNotebook *notebook = nullptr;
        val::d_array<val::SwitchCtrl*> ViewSwitches{nullptr,12};
        val::d_array<wxTextCtrl*> Set_TextEdit{nullptr,12};

        //
        void ChangeSettings(int command, const std::string &svalue="", int id = 0);
        void ExecuteCommand(int command, int f_nr = 0, const std::string &svalue="", int id = 0);
        int findactivefunction(int x,int y);
        void displacefunction(int i,const std::string& dx,const std::string& dy);
        void changefunctionsettings(int i);
        void changedpanelinsertmode(int mode = 0);
        void SendNotification(const std::string& s);
        //
        void Paint();
        void render(wxDC &);
        void Compute(int i=-1, int computepoints = 1);
        void ResetColours();
        void GetSettings();

        void SetAccelerators(const val::Glist<wxAcceleratorEntry> &Accel);
        void GetSizeSettings();
        void valFloodFill(wxMemoryDC& dc, int x, int y, const wxColour &fgc);
        void plotvertices(wxDC& dc);
        /*
        void plotfunction(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plotline(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plotcircle(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plotrectangle(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plottriangle(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plotfill(wxMemoryDC& dc,const val::d_array<double> &f,int colour=0);
        void plotpolygon(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plotpoints(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plottext(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plotcurve(wxDC& dc,const val::d_array<val::d_array<double> > &c,const val::Glist<val::GPair<double>>& critpoints
                       ,const val::d_array<double> &critx,int colour=0);
        */
        void plotfunction(wxDC& dc,int colour);
        void plotline(wxDC& dc,int colour);
        void plotcircle(wxDC& dc,int colour);
        void plotrectangle(wxDC& dc,int colour);
        void plothistogram(wxDC& dc, int colour, const double& solid = 0.0);
        void plottriangle(wxDC& dc,int colour);
        void plotfill(wxMemoryDC& dc,int colour);
        void plotpolygon(wxDC& dc,int colour);
        void plotpoints(wxDC& dc,int colour);
        void plottext(wxDC& dc,int colour);
        void plotbitmap(wxDC& dc,int colour);
        void plotcurve(wxDC& dc,int colour);
        void plotparcurve(wxDC& dc,int colour);
        void plotallfunctions(wxMemoryDC& dc);
        void plottomemoryDc(wxMemoryDC &memDC);
        void refreshfunctionstring();
        void savefile(const std::string& dirname,const std::string& filename);
        void openfile(const std::string& dirname,const std::string& filename);
        void setfunctionsmenu();
        //
        int checkinrecent(const std::string& filename);
        void appendrecentfiles(const std::string& filename);
        //
        void CompareSideTextInput();
        void WriteText();
        void OnFocusChanged(wxFocusEvent&);
        void OnSideBarCheck(wxCommandEvent&);
        void OnSideBarEvaluate(wxCommandEvent&);
        void ChangeSideTextStyle(wxCommandEvent&);
        void CreateNoteBook();
        void OnNoteBookSwitch(wxCommandEvent &event);
        void OnNoteBookButtons(wxCommandEvent &event);
        void OnNoteBookEnter(wxCommandEvent& event);
        void OnTexLostFocus(wxFocusEvent&);
        void ProcessText_Edit(int id);
        void switchrefresh();
        void Text_Editrefresh();

        //*Handlers(PlotFunctionFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnDrawPanelPaint(wxPaintEvent& event);
        void OnDrawPanelResize(wxSizeEvent& event);
        void OnAllSettingsSelected(wxCommandEvent& event);
        void OnMenu_xAxisSelected(wxCommandEvent& event);
        void OnMenuExportSelected(wxCommandEvent& event);
        void OnMenuSizeSelected(wxCommandEvent& event);
        void OnMenuFontSize(wxCommandEvent& event);
        void OnFileMenu(wxCommandEvent &event);
        void OnMenunewfunction(wxCommandEvent &event);
        void OnMenuColours(wxCommandEvent &event);
        void OnMenuResetColours(wxCommandEvent &event);
        void OnMenuParameter(wxCommandEvent &event);
        void OnMenuTools(wxCommandEvent &event);
        void OnChangeParmeterMenu(wxCommandEvent & event);
        void OnRangeMenu(wxCommandEvent &event);
        void OnScaleMenu(wxCommandEvent &event);
        void OnGridMenu(wxCommandEvent &event);
        void OnMoveMenu(wxCommandEvent &);
        void OnMenuFill(wxCommandEvent &event);
        void OnInputDialog(wxCommandEvent &event);
        void OnMenuButton(wxCommandEvent&);
        void OnMenuRecent(wxCommandEvent& event);
        // Mouse - Handler
        void OnMouseDown(wxMouseEvent &event);
        void OnMouseWheel(wxMouseEvent &event);
        void OnMouseCaptured(wxMouseEvent& event);
        void OnMouseReleased(wxMouseEvent& event);
        void OnMouseMoved(wxMouseEvent& event);
        void OnMouseDouble(wxMouseEvent& event);
        void OnLostMouse(wxMouseCaptureLostEvent &event);
        //
        void OnZoom(wxCommandEvent &event);
        void OnMove(wxCommandEvent &event);
        void OnSelectActiveFunction(wxCommandEvent &event);
        //
        void OnMyEvent(MyThreadEvent& event);
        void OnParentEvent(ParentEvent& event);
        //*)
};

#endif // PLOTFUNCTIONMAIN_H
