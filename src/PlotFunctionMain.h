/***************************************************************
 * Name:      PlotFunctionMain.h
 * Purpose:   Defines Application Frame
 * Author:    Miguel Valbuena ()
 * Created:   2018-09-28
 * Copyright: Miguel Valbuena ()
 * License:
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
#include <wx/dcclient.h>
#include <d_array.h>
#include <pol.h>
#include <Glist.h>
#include <val_basics.h>
#include <rational.h>
#include <string>
#include "valDialogs.h"
//#include <MyTime.h>
//#include <wx/fontdata.h>



class MyThreadEvent;
class ParentEvent;
class myfunction;
namespace val {class rational;}

class PlotFunctionFrame: public wxFrame
{
    public:

        PlotFunctionFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~PlotFunctionFrame();

    private:

        //*Handlers(PlotFunctionFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnDrawPanelPaint(wxPaintEvent& event);
        void OnDrawPanelResize(wxSizeEvent& event);
        void OnAllSettingsSelected(wxCommandEvent& event);
        void OnMenu_xAxisSelected(wxCommandEvent& event);
        void OnMenuSaveSelected(wxCommandEvent& event);
        void OnMenuSizeSelected(wxCommandEvent& event);
        void OnMenuFontSize(wxCommandEvent& event);
        //*)

        //(*Identifiers(PlotFunctionFrame)
        static const long ID_PANEL1;
        static const long idMenuQuit;
        static const long ID_MENUITEM4;
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
        wxMenu* Menu3;
        wxMenu* submenuaxis;
        wxMenuItem* MenuItem3;
        wxMenuItem* MenuItem4;
        wxMenuItem* MenuSave;
        wxMenuItem* MenuSize;
        wxMenuItem* Menu_xAxis;
        wxMenuItem* Menu_yAxis;
        wxPanel* DrawPanel;
        wxStatusBar* StatusBar1;
        //*)
        //wxBoxSizer* BoxSizer1;
        //wxBoxSizer* BoxSizer2;
        wxBoxSizer *BoxSizer1, *BoxSizer2;
        val::CompleteTextCtrl *SideText = nullptr;

        void GetSizeSettings();
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
        void ChangeSettings(int command, const std::string &svalue="", int id = 0);
        void ExecuteCommand(int command, int f_nr = 0, const std::string &svalue="", int id = 0);
        int findactivefunction(int x,int y);
        void displacefunction(int i,const double& dx,const double& dy);
        // Mouse - Handler
        void OnMouseDown(wxMouseEvent &event);
        void OnMouseWheel(wxMouseEvent &event);
        void OnMouseCaptured(wxMouseEvent& event);
        void OnMouseReleased(wxMouseEvent& event);
        void OnMouseMoved(wxMouseEvent& event);
        void OnMouseDouble(wxMouseEvent& event);
        //void OnMouseUp(wxMouseEvent &event);
        void OnLostMouse(wxMouseCaptureLostEvent &event);
        void changedrawpoints();
        void changedrawpolygon();
        void changedrawline();
        void SendNotification(const std::string& s);
        //
        void OnZoom(wxCommandEvent &event);
        void OnMove(wxCommandEvent &event);
        void OnSelectActiveFunction(wxCommandEvent &event);
        //
        wxMenu *Menu_functions,*colorsubmenu,*rightclickmenu;
        wxMenuItem *hideallmenu,*showallmenu,*deletelastmenu,*deleteallmenu,*x_scaleactiv,*y_scaleactiv,*gridactiv,*undomenu,*redomenu,*addfunction,
                    *addpointsmenu, *polygonpointsmenu, *linepointsmenu, *multicolormenu, *sidemenuview = nullptr;
        int ispainted=0,N=0,points=1000,abst=10,sizex,sizey,yzero,npainted=0,yset=1,ncolours=4,defaultsize=1,fontsize=10,nchildwindows=0;
        int settings=0,Posx=0,Posy=0,clientsize_x=0,clientsize_y=0,iter=50,dez=4,mouse_x1,mouse_y1,fillfunctions=0,pi_scale_x=0,pi_scale_y=0;//mouse_x2,mouse_y2;
        int n_fstring=1,a_fstring=0,axis_pen=2,grid_pen=1,iscomputing=0,mousecaptured=0,MaxRecent=10,addingpoints=0,drawpoints=0;
        int regressiondegree=1,rounddrawingpoints=-2,nanalyzewindows=0,active_function=-1,pointactive=0,decimalx=0,decimaly=0;
        int moveinpointsx = 0, moveinpointsy = 0, drawpolygon = 0, n_polygonpoints=0, polygonline = 0, drawline = 0, n_linepoints = 0;
        int bitmapbackground = 0, axis_fontsize = 10, SideText_isshown = 0, widthSideText = 200;
        wxPoint actuallinepoint,actualpolygonpoint;
        val::rational pi_factor_x,pi_factor_y,g_pi_factor_x,g_pi_factor_y;
        void Paint();
        void Compute(int i=-1, int computepoints = 1);
        double x1=-5,x2=5,y1=-5,y2=5,ymin=-5,ymax=5,x_scale=1,y_scale=1,gx_scale=0.5,gy_scale=0.5,delta=1e-8,zoom=1.0,mx1,mx2,my1,my2,movedx = 0.1, movedy = 0.1;
        double movex = 0, movey = 0;
        std::string Program_Name="Simple plot",x_axis="x",y_axis="y",sx_scale="1",sy_scale="1",sgx_scale="0.5",sgy_scale="0.5",actual_filename="",actual_dirname="";
        std::string SideText_Word;
        wxFont defaultFont;
        wxSize bitmapsize,actualPanelsize; //DialogInputSize{wxDefaultSize};
        wxBitmap actualBitmapBackground;
        wxImage BackgroundImage;
        wxColour axis_color ,grid_color, defaultpaintcolor;
        val::d_array<val::d_array<double> > farray;
        val::d_array<val::d_array<val::d_array<double> > > curvearray;
        //val::d_array<val::d_array<val::d_array<double> > > y_curvearray;
        wxColour BackgroundColor = wxColour(255,255,255);
        val::d_array<wxColor> Color;
        val::d_array<wxFont> Font;
        val::d_array<wxMenuItem*> f_menu,c_menu;
        val::d_array<int> pen,islinear;
        //val::d_array<val::pol<double>> F;
        val::Glist<myfunction> F;
        val::Glist<val::rational> Parameter{val::rational(1)};
        val::Glist<val::GPair<double> > x_range;
        val::d_array<std::string> functionstring{30};
        //
        wxMenu* MenuRecentfiles;
        val::Glist<wxMenuItem*> recent_menu;
        val::Glist<std::string> recentfiles;
        //
        wxCursor *closeHand=nullptr;//("/home/miguel/gccprogrammes/Plot/PlotFunction/icons/hand.png",wxBITMAP_TYPE_PNG);
        //val::ChronoClass Mtime;
        void ResetColours();
        void GetSettings();
        //void OnPaint(MyThreadEvent& event);
        //void OnTable(MyThreadEvent& event);
        //void OnRefresh(MyThreadEvent& event);
        void OnMyEvent(MyThreadEvent& event);
        void OnParentEvent(ParentEvent& event);

        void plotvertices(wxDC& dc);
        void plotfunction(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plotline(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plotcircle(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plotrectangle(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plottriangle(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plotfill(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plotpolygon(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plotpoints(wxDC& dc,const val::d_array<double> &f,int colour=0);
        void plottext(wxDC& dc,const val::d_array<double> &f,const wxString& text,int colour=0);
        //void plotcurve(wxDC& dc,const val::d_array<val::d_array<double> > &c,const val::d_array<val::d_array<double> > &y_c,int colour=0);
        void plotcurve(wxDC& dc,const val::d_array<val::d_array<double> > &c,const val::Glist<val::GPair<double>>& critpoints
                       ,const val::d_array<double> &critx,int colour=0);
        void plotallfunctions(wxDC& dc);
        void plottomemoryDc(wxMemoryDC &memDC);
        void refreshfunctionstring();
        void savefile(const std::string& dirname,const std::string& filename);
        void openfile(const std::string& dirname,const std::string& filename);
        void setfunctionsmenu();
        //
        void OnMenuRecent(wxCommandEvent& event);
        int checkinrecent(const std::string& filename);
        void appendrecentfiles(const std::string& filename);
        //
        void CompareSideTextInput();
        void WriteText();
        void OnLostFocus(wxFocusEvent&);
        void OnSideBarCheck(wxCommandEvent&);
        void CheckFocus();

        //DECLARE_EVENT_TABLE()
};

#endif // PLOTFUNCTIONMAIN_H
