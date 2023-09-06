/***************************************************************
 * Name:      PlotFunctionMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Miguel Valbuena ()
 * Created:   2018-09-28
 * License:   GPL v3.0
 **************************************************************/

#include "PlotFunctionMain.h"
#include <functional>
#include <string>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include "PlotFunction.h"
#include "calculus.h"
#include <thread>
#include <val_utils.h>
#include <analysis.h>
#include <rational.h>
#include <wx/bitmap.h>
#include <wx/dcmemory.h>
#include <wx/colordlg.h>
#include "MultiLineDialog.h"
#include "d_array.h"
#include "val_wx/valDialogs.h"
#include "wx/gdicmn.h"
#include <fstream>
#include <wx/fontdlg.h>
#include <pol_arithmetic.h>
#include <function_parser.h>
#include <wx/utils.h>
#include <wx/display.h>
#include <wx/dcgraph.h>

//InternalHeaders(PlotFunctionFrame)
#include <wx/bitmap.h>
#include <wx/icon.h>
#include <wx/image.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/clipbrd.h>
#include <wx/rawbmp.h>
//#include <wx/dataobj.h>
//#include <wx/base64.h>
//#include <wx/mstream.h>
//#include <wx/sstream.h>
//#include <wx/textfile.h>
//



//*IdInit(PlotFunctionFrame)
const long PlotFunctionFrame::ID_PANEL1 = 30001;//wxNewId();
const long PlotFunctionFrame::idMenuQuit = 30002;//wxNewId();
const long PlotFunctionFrame::ID_MENUITEM4 = 30003;//wxNewId();
const long PlotFunctionFrame::ID_MENUITEM1 = 30004;//wxNewId();
const long PlotFunctionFrame::ID_MENUITEM2 = 30005;//wxNewId();
const long PlotFunctionFrame::ID_MENUITEM3 = 30006;//wxNewId();
const long PlotFunctionFrame::ID_MENUITEM7 = 30007;//wxNewId();
const long PlotFunctionFrame::ID_MENUITEM5 = 30008;//wxNewId();
const long PlotFunctionFrame::ID_MENUITEM6 = 30009;//wxNewId();
const long PlotFunctionFrame::idMenuAbout = 30010;//wxNewId();
const long PlotFunctionFrame::ID_STATUSBAR1 = 30011;//wxNewId();
//*)


struct quadruple
{
    int x1, x2, y, dy;
    quadruple() = default;
    quadruple(int ix1, int ix2, int iy, int idy) : x1(ix1), x2(ix2), y(iy), dy(idy) {}
};




PlotFunctionFrame::PlotFunctionFrame(wxWindow* parent,wxWindowID id)
{
    //*Initialize(PlotFunctionFrame)
    //wxBoxSizer* BoxSizer2;
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, id,Program_Name, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));

    if (val::FileExists(iconpath)) {
        wxIcon FrameIcon;
        FrameIcon.CopyFromBitmap(wxBitmap(wxImage(iconpath)));
        SetIcon(FrameIcon);
    }
    else if(val::FileExists(alticonpath)) {
        wxIcon FrameIcon;
        FrameIcon.CopyFromBitmap(wxBitmap(wxImage(alticonpath)));
        SetIcon(FrameIcon);
    }
    //
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    DrawPanel = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxSize(400,300), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    DrawPanel->SetMinSize(wxSize(-1,-1));
    DrawPanel->SetMaxSize(wxSize(-1,-1));
    DrawPanel->SetForegroundColour(wxColour(255,255,255));
    DrawPanel->SetBackgroundColour(wxColour(255,255,255));

    SideText = new val::CompleteTextCtrl(this,3002,WordTree,wxEmptyString,wxSize(widthSideText,100),wxDefaultPosition,wxTE_MULTILINE|wxVSCROLL|wxHSCROLL|wxTE_RICH);


    BoxSizer2->Add(SideText, 0, wxALL|wxEXPAND, 5);
    BoxSizer2->Add(DrawPanel, 1, wxALL|wxEXPAND, 5);

    BoxSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
    SetSizer(BoxSizer1);
    //
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
#ifdef __APPLE__
    Menu1->Append(101,_("New\tRawCtrl-N"));
#else
    Menu1->Append(101,_("New\tCtrl-N"));
#endif // __APPLE__
    Menu1->Append(102,_("Open file...\tCtrl-O"));
    MenuRecentfiles = new wxMenu();
    MenuRecentfiles->AppendSeparator();
    MenuRecentfiles->Append(601,_("Clear history"));
    Menu1->Append(600,_("Recent files"),MenuRecentfiles);
    Menu1->Append(103,_("Save file\tCtrl-S"));
    Menu1->Append(104,_("Save file as..."));
    MenuSave = new wxMenuItem(Menu1, ID_MENUITEM4, _("Export Graphic as...\tCtrl-E"), wxEmptyString, wxITEM_NORMAL);
    Menu1->Append(MenuSave);
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    Menu3 = new wxMenu();
    MenuItem3 = new wxMenuItem(Menu3, ID_MENUITEM1, _("Main Settings...\tAlt-M"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem3);
    submenuaxis = new wxMenu();
    Menu_xAxis = new wxMenuItem(submenuaxis, ID_MENUITEM2, _("x-Axis\tAlt-X"), wxEmptyString, wxITEM_CHECK);
    submenuaxis->Append(Menu_xAxis);
    Menu_xAxis->Check(true);
    Menu_yAxis = new wxMenuItem(submenuaxis, ID_MENUITEM3, _("y-Axis\tAlt-Y"), wxEmptyString, wxITEM_CHECK);
    submenuaxis->Append(Menu_yAxis);
    Menu_yAxis->Check(true);
    Menu3->Append(ID_MENUITEM7, _("Axis"), submenuaxis, wxEmptyString);
    MenuSize = new wxMenuItem(Menu3, ID_MENUITEM5, _("Size for png-Graphic"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuSize);
    MenuItem4 = new wxMenuItem(Menu3, ID_MENUITEM6, _("Font Size\tCtrl-F"), wxEmptyString, wxITEM_NORMAL);
    Menu3->Append(MenuItem4);
    wxMenu* viewMenu = new wxMenu();
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    SetMenuBar(MenuBar1);
    //
    sidemenuview = new wxMenuItem(viewMenu,30101,_("Side Bar \tF2"),wxEmptyString,wxITEM_CHECK);
    viewMenu->Append(sidemenuview);
    sidemenuview->Check(false);
    //
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[2] = { -3,-1 };
    //int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(2,__wxStatusBarWidths_1);
    //StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    //StatusBar1->SetFieldsCount(2);
    SetStatusBar(StatusBar1);

    //DrawPanel->Connect(wxEVT_PAINT,(wxObjectEventFunction)&PlotFunctionFrame::OnDrawPanelPaint,0,this);
    DrawPanel->Bind(wxEVT_PAINT,&PlotFunctionFrame::OnDrawPanelPaint,this);
    DrawPanel->Bind(wxEVT_SIZE,&PlotFunctionFrame::OnDrawPanelResize,this);
    //DrawPanel->Connect(wxEVT_SIZE,(wxObjectEventFunction)&PlotFunctionFrame::OnDrawPanelResize,0,this);

    SideText->Bind(wxEVT_KILL_FOCUS,&PlotFunctionFrame::OnLostFocus,this);
    SideText->Bind(wxEVT_SET_FOCUS,&PlotFunctionFrame::OnLostFocus,this);
    SideText->Hide();

    //Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PlotFunctionFrame::OnQuit);
    //Connect(ID_MENUITEM4,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PlotFunctionFrame::OnMenuSaveSelected);
    //Connect(ID_MENUITEM1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PlotFunctionFrame::OnAllSettingsSelected);
    //Connect(ID_MENUITEM2,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PlotFunctionFrame::OnMenu_xAxisSelected);
    //Connect(ID_MENUITEM3,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PlotFunctionFrame::OnMenu_xAxisSelected);
    //Connect(ID_MENUITEM5,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PlotFunctionFrame::OnMenuSizeSelected);
    //Connect(ID_MENUITEM6,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PlotFunctionFrame::OnMenuFontSize);
    //Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PlotFunctionFrame::OnAbout);

    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnQuit,this,idMenuQuit);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuSaveSelected,this,ID_MENUITEM4);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnAllSettingsSelected,this,ID_MENUITEM1);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenu_xAxisSelected,this,ID_MENUITEM2);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenu_xAxisSelected,this,ID_MENUITEM3);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuSizeSelected,this,ID_MENUITEM5);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFontSize,this,ID_MENUITEM6);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnAbout,this,idMenuAbout);
    //*)
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnSideBarCheck,this,30101);
    //
    colorsubmenu = new wxMenu();
    multicolormenu = new wxMenuItem(colorsubmenu,111,"Multiple Colors \tAlt-C",wxEmptyString,wxITEM_CHECK);
    colorsubmenu->Append(multicolormenu);
    multicolormenu->Check(true);
    colorsubmenu->Append(4400,_T("Background Color...\tCtrl-Shift-B"));
    colorsubmenu->Append(4401,_T("Axis Color...\tCtrl-Shift-A"));
    colorsubmenu->Append(4402,_T("Grid Color...\tCtrl-Shift-G"));
    colorsubmenu->Append(4403,_T("Default Function Color...\tAlt-Shift-C"));
    colorsubmenu->AppendSeparator();
    Menu3->Insert(3,3,"Colors",colorsubmenu);
    wxMenuItem *resetcoloursmenu=new wxMenuItem(Menu3,4,"Reset Colours \tCtrl-Alt-R", wxEmptyString);
    Menu3->Insert(4,resetcoloursmenu);
    //
    wxMenuItem *ChangeParameterMenu= new wxMenuItem(Menu3,5,"Change Parameter Values...\tCtrl-P",wxEmptyString);
    Menu3->Insert(3,ChangeParameterMenu);
    wxMenuItem *DefaultFontSetting= new wxMenuItem(Menu3,21,"Default Draw Text Settings...\tCtrl-Alt-F");
    Menu3->Append(DefaultFontSetting);
    wxMenuItem *SetCSize= new wxMenuItem(Menu3,22,"Set Panel Size...\tF8");
    Menu3->Append(SetCSize);
    Menu3->Append(23,_T("Regression Degree...\tShift-Alt-A"));
    Menu3->Append(24,_T("Round-decimals for points...\tCtrl-D"));
    Menu3->Append(25,_T("Move Increments...\tCtrl-M"));
    //
    //wxMenuItem *xrangemenu=new wxMenuItem(Menu3,6,"x-Axis Range...\tCtrl-X",wxEmptyString);
    //wxMenuItem *yrangemenu=new wxMenuItem(Menu3,7,"y-Axis Range...\tCtrl-Y",wxEmptyString);
    //Menu3->Insert(3,xrangemenu);
    //Menu3->Insert(4,yrangemenu);
    wxMenuItem *xrangemenu=new wxMenuItem(submenuaxis,6,"x-Axis Range...\tCtrl-X",wxEmptyString);
    wxMenuItem *yrangemenu=new wxMenuItem(submenuaxis,7,"y-Axis Range...\tCtrl-Y",wxEmptyString);
    x_scaleactiv= new wxMenuItem(submenuaxis,8,"Show x-scale \tShift-Alt-X",wxEmptyString,wxITEM_CHECK);
    y_scaleactiv= new wxMenuItem(submenuaxis,9,"Show y-scale \tShift-Alt-Y",wxEmptyString,wxITEM_CHECK);
    wxMenuItem *scalemenu=new wxMenuItem(submenuaxis,10,"Scale Settings...\tShift-Alt-S",wxEmptyString);
    gridactiv= new wxMenuItem(submenuaxis,11,"Grid \tAlt-G",wxEmptyString,wxITEM_CHECK);
    wxMenuItem *gridmenu=new wxMenuItem(submenuaxis,12,"Grid Settings...\tCtrl-G",wxEmptyString);
    wxMenuItem *axisfontmenu=new wxMenuItem(submenuaxis,13,"Axis Font Size\tCtrl-Shift-F",wxEmptyString);
    submenuaxis->Append(xrangemenu);
    submenuaxis->Append(yrangemenu);
    submenuaxis->Append(x_scaleactiv);
    submenuaxis->Append(y_scaleactiv);
    submenuaxis->Append(scalemenu);
    submenuaxis->Append(gridactiv);
    submenuaxis->Append(gridmenu);
    submenuaxis->Append(axisfontmenu);
    x_scaleactiv->Check(); y_scaleactiv->Check();
    //
#ifdef __APPLE__
    addfunction=new wxMenuItem(Menu_functions,3000,"Add/Remove new functions...\tCtrl-N", wxEmptyString);
#else
    addfunction=new wxMenuItem(Menu_functions,3000,"Add/Remove new functions...\tAlt-N", wxEmptyString);
#endif // __APPLE__
    hideallmenu= new wxMenuItem(Menu_functions,3001,"Hide All \tAlt-H", wxEmptyString);
    showallmenu= new wxMenuItem(Menu_functions,3002,"Show All \tAlt-S", wxEmptyString);
    deletelastmenu = new wxMenuItem(Menu_functions,3003,"Delete Last \tAlt-Del", wxEmptyString);
    deleteallmenu = new wxMenuItem(Menu_functions,3004,"Delete All \tCtrl-Del", wxEmptyString);
    undomenu = new wxMenuItem(Menu_functions,3005,_T("undo \tCtrl-Z"), wxEmptyString);
    redomenu = new wxMenuItem(Menu_functions,3006,_T("redo \tShift-Ctrl-Z"), wxEmptyString);
    //
    Menu_functions= new wxMenu();
    Menu_functions->AppendSeparator();
    Menu_functions->Append(addfunction);
    Menu_functions->Append(undomenu);
    Menu_functions->Append(redomenu);
    Menu_functions->Append(hideallmenu);
    Menu_functions->Append(showallmenu);
    Menu_functions->Append(deletelastmenu);
    Menu_functions->Append(deleteallmenu);
    //
    wxMenu *Menu_Tools=new wxMenu();
    wxMenuItem *Menuparameter = new wxMenuItem(Menu_Tools,7000,"Parameter Values...\tAlt-P",wxEmptyString);
    wxMenuItem *Menutangent = new wxMenuItem(Menu_Tools,7001,"Tangent...\tAlt-T",wxEmptyString);
    wxMenuItem *Menudiff = new wxMenuItem(Menu_Tools,7002,"Derivate \tAlt-D",wxEmptyString);
    wxMenuItem *Menutable = new wxMenuItem(Menu_Tools,7003,"Table... \tCtrl-T",wxEmptyString);
    wxMenuItem *Menuintegral = new wxMenuItem(Menu_Tools,7004,"Integrate... \tAlt-I",wxEmptyString);
    wxMenuItem *MenuArcLength = new wxMenuItem(Menu_Tools,7005,"Arc Length... \tShift-Alt-I",wxEmptyString);
    wxMenuItem *MenuInterpolation = new wxMenuItem(Menu_Tools,7006,"Interpolation...\tCtrl-I");
    wxMenuItem *Menunormal = new wxMenuItem(Menu_Tools,7007,"Normal...\tShift-Alt-N",wxEmptyString);
    Menu_Tools->Append(Menuparameter);
    Menu_Tools->Append(Menutangent);
    Menu_Tools->Append(Menunormal);
    Menu_Tools->Append(Menudiff);
    Menu_Tools->Append(Menutable);
    Menu_Tools->Append(Menuintegral);
    Menu_Tools->Append(MenuArcLength);
    Menu_Tools->Append(MenuInterpolation);
    Menu_Tools->Append(7008,_("Zero-iteration...\tAlt-Z"));
    Menu_Tools->Append(7009,_("Rotate...\tAlt-R"));
    Menu_Tools->Append(7010,_("Regression \tAlt-A"));
    Menu_Tools->Append(7011,_("Analyze function... \tCtrl-A"));
    Menu_Tools->Append(7012,_("Intersection... \tShift-Ctrl-I"));

    MenuBar1->Append(Menu1, _("&File"));
    MenuBar1->Append(viewMenu,_("&View"));
    MenuBar1->Append(Menu3, _("Settings"));
    MenuBar1->Append(Menu_functions, _("Functions"));
    MenuBar1->Append(Menu_Tools,"Tools");
    MenuBar1->Append(Menu2, _("Help"));


    //
    rightclickmenu = new wxMenu();
    rightclickmenu->Append(1001,_T("Fill Area"));
    rightclickmenu->Append(1002,_T("Choose Color and Fill Area..."));
    rightclickmenu->Append(1003,_T("circle..."));
    rightclickmenu->Append(1004,_T("text..."));
    addpointsmenu = new wxMenuItem(rightclickmenu,1005,_T("Add Points \tShift-Alt-P"),wxEmptyString,wxITEM_CHECK);
    //addpointsmenu->Check(false);
    rightclickmenu->Append(addpointsmenu);
    linepointsmenu = new wxMenuItem(rightclickmenu,1007,_T("Draw Line \tShift-Ctrl-L"),wxEmptyString,wxITEM_CHECK);
    rightclickmenu->Append(linepointsmenu);
    polygonpointsmenu = new wxMenuItem(rightclickmenu,1006,_T("Draw Polygon \tShift-Ctrl-P"),wxEmptyString,wxITEM_CHECK);
    rightclickmenu->Append(polygonpointsmenu);
    rightclickmenu->AppendSeparator();
    rightclickmenu->Append(1008,_T("Copy Image\tCtrl-C"));
    rightclickmenu->Append(1009,_T("Paste Image to Background\tCtrl-V"));
    //
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnFileMenu,this,101);          // New
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnFileMenu,this,102);          // Open
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnFileMenu,this,103);          // Save File
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnFileMenu,this,104);          // Save File as
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuRecent,this,601);        // Clear History
    //
    //Bind(MY_EVENT,&PlotFunctionFrame::OnPaint,this,IdPaint);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuResetColours,this,4);    // Reset Colors
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuParameter,this,7000);    // Parameter Values
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuTools,this,7001);      // Tangent
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuTools,this,7002);      // Derive
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuTools,this,7003);      // Table
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuTools,this,7004);      // Intgrate
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuTools,this,7005);      // Arc length
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuTools,this,7006);      // Interpolation
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuTools,this,7007);      // Normal
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuTools,this,7008);      // Zero Iteration
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuTools,this,7009);      // Rotate
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuTools,this,7010);      // Regression
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuTools,this,7011);      // Analyze
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuTools,this,7012);      // Intersection
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnChangeParmeterMenu,this,5);  // Change Parameter Values
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnChangeParmeterMenu,this,23); // Regression Degree
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnChangeParmeterMenu,this,24); // Round-decimal for points
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMoveMenu,this,25);           // Move Increments
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnRangeMenu,this,6);           // x-Axis Range
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnRangeMenu,this,7);           // y-Axis Range
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenu_xAxisSelected,this,8);  // Show x-scale
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenu_xAxisSelected,this,9);  // Show y-scale
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnScaleMenu,this,10);          // Scale Settings
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenu_xAxisSelected,this,11); // Grid
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnGridMenu,this,12);           // Grid Settings
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFontSize,this,13);       // Axis Fontsize
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuColours,this,21);        // Default Draw Text Settings
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuColours,this,4400);      // Background Color
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuColours,this,4401);      // Axis Color
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuColours,this,4402);      // Grid Color
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuColours,this,4403);      // Default Function Color
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuSizeSelected,this,22);   // Set Panel Size
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenunewfunction,this,3000);  // Add/Remove new functions
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenunewfunction,this,3001);  // Hide All
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenunewfunction,this,3002);  // Show All
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenunewfunction,this,3003);  // Delete Last
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenunewfunction,this,3004);  // Delete All
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenunewfunction,this,3005);  // undo
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenunewfunction,this,3006);  // redo
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1001);         // Fill Area
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1002);         // Choose Color and Fill Area
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1003);         // circle
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1004);         // text
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1005);         // Add Points
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1006);         // DrawPolygon
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1007);         // DrawLine
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1008);         // Copy to Clipboard
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1009);         // Paste from Clipboard
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnInputDialog,this,1010);      // Opens InputDialog
    Bind(MY_EVENT,&PlotFunctionFrame::OnMyEvent,this);
    Bind(P_EVENT,&PlotFunctionFrame::OnParentEvent,this);
    // Mouse-Events:
    DrawPanel->Bind(wxEVT_RIGHT_DOWN,&PlotFunctionFrame::OnMouseDown,this);
    DrawPanel->Bind(wxEVT_MOUSEWHEEL,&PlotFunctionFrame::OnMouseWheel,this);
    DrawPanel->Bind(wxEVT_LEFT_DOWN,&PlotFunctionFrame::OnMouseCaptured,this);
    DrawPanel->Bind(wxEVT_LEFT_DCLICK,&PlotFunctionFrame::OnMouseDouble,this);
    DrawPanel->Bind(wxEVT_LEFT_UP,&PlotFunctionFrame::OnMouseReleased,this);
    DrawPanel->Bind(wxEVT_MOUSE_CAPTURE_LOST,&PlotFunctionFrame::OnLostMouse,this);
    //
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnZoom,this,20001);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnZoom,this,20002);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMove,this,20003);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMove,this,20004);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMove,this,20005);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMove,this,20006);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnSelectActiveFunction,this,20007);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnSelectActiveFunction,this,20008);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnSelectActiveFunction,this,20011);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMove,this,20009);
    Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMove,this,20010);
    //
    wxAcceleratorEntry entries[19];
    entries[0].Set(wxACCEL_CTRL, (int) '+',20001);
    entries[1].Set(wxACCEL_CTRL, (int) '-',20002);
    entries[2].Set(wxACCEL_CTRL,WXK_RIGHT,20003);
    entries[3].Set(wxACCEL_CTRL,WXK_LEFT,20004);
    entries[4].Set(wxACCEL_CTRL,WXK_UP,20005);
    entries[5].Set(wxACCEL_CTRL,WXK_DOWN,20006);
    entries[6].Set(wxACCEL_SHIFT|wxACCEL_ALT,(int) 'P',1005);
    entries[7].Set(wxACCEL_CTRL,WXK_PAGEDOWN,20007);
    entries[8].Set(wxACCEL_NORMAL,WXK_ESCAPE,20008);
    entries[9].Set(wxACCEL_SHIFT|wxACCEL_CTRL,(int) 'P',1006);
    entries[10].Set(wxACCEL_SHIFT|wxACCEL_CTRL,(int) 'L',1007);
    entries[11].Set(wxACCEL_CTRL,(int) 'C',1008);
    entries[12].Set(wxACCEL_CTRL,WXK_INSERT,1008);
    entries[13].Set(wxACCEL_CTRL,(int) 'V',1009);
    entries[14].Set(wxACCEL_SHIFT,WXK_INSERT,1009);
#ifdef __APPLE__
    entries[15].Set(wxACCEL_ALT,WXK_SPACE,1010);
#else
    entries[15].Set(wxACCEL_CTRL,WXK_SPACE,1010);
#endif // __APPLE__
    entries[16].Set(wxACCEL_ALT,WXK_RIGHT,20009);
    entries[17].Set(wxACCEL_ALT,WXK_LEFT,20010);
    entries[18].Set(wxACCEL_CTRL,WXK_PAGEUP,20011);
    wxAcceleratorTable accel(19,entries);
    SetAcceleratorTable(accel);
    //


    defaultFont=this->GetFont();
    Color.resize(7);

    DrawPanel->SetMinSize(wxSize(100,10));

    SideText->SetCloseBrackets(true);

    GetSizeSettings();
    if (settings) {
        wxSize TextSize = SideText->GetSize();
        TextSize.x = widthSideText;
        SideText->SetSize(TextSize);
        TextSize.y = -1;
        SideText->SetMinSize(TextSize);

        clientsize_x += 20;
        clientsize_y += 20;
        if (SideText_isshown) {
            clientsize_x +=  widthSideText + 10;
            sidemenuview->Check(true);
            SideText->Show();
            SideText->SetFocus();
            CheckFocus();
        }
        Move(wxPoint(Posx,Posy));
    }
    else {
        clientsize_x = clientsize_y = 500;
    }

    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);
    SetClientSize(wxSize(clientsize_x,clientsize_y));
    SetMinClientSize(wxSize(-1,80));
    actualPanelsize = DrawPanel->GetSize();
    MyFrame=this;
    DrawPanel->SetCursor(wxCursor (wxCURSOR_HAND));
    ResetColours();
    GetSettings();
    //WordList.sort();
    Compute();
}

PlotFunctionFrame::~PlotFunctionFrame()
{
    using namespace std;
    //std::ofstream file(val::GetExeDir()+filesep+"settings.txt",ios::out | ios::trunc);
    std::ofstream file(settingsfile,ios::out | ios::trunc);
    if (file) {
        wxSize MySize=DrawPanel->GetSize();
        wxPoint MyPoint=GetPosition();
        file<<MyPoint.x<<endl;
        file<<MyPoint.y<<endl;
        file<<MySize.x<<endl;
        file<<MySize.y<<endl;
        file<<fontsize<<endl;
        file<<axis_fontsize<<endl;
        file<<filedir<<endl;
        file<<openfiledir<<endl;
        file<<savefiledir<<endl;
#ifndef __LINUX__
        file<<defaultFont.GetNativeFontInfoUserDesc()<<endl;
#else
        file<<defaultFont.GetNativeFontInfoDesc()<<endl;
#endif // __LINUX__
        file<<SideText_isshown<<endl;
        file<<widthSideText<<endl<<endl;
        file.close();
    }
    std::ofstream file3(RecentFilesPath,std::ios::out | std::ios::trunc);
    for (const auto& v : recentfiles) {
        file3<<v<<std::endl;
    }
    file3.close();

    if (DrawPanel->HasCapture()) DrawPanel->ReleaseMouse();
    //(*Destroy(PlotFunctionFrame)
    //*)
}

void PlotFunctionFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}


void PlotFunctionFrame::OnFileMenu(wxCommandEvent& event)
{
    int id=event.GetId();
    std::string dirname,filename;
#ifdef _WIN32
    wxString filetype = _("PLF files (*.plf)|*.plf");
#else
    wxString filetype = _("all files (*)|*");
#endif // _WIN32

    if (drawpoints) changedrawpoints();
    if (drawpolygon) changedrawpolygon();
    if (drawline) changedrawline();

    if (id==101) {   //new
        val::rational rzero;
        actual_filename="";
        ResetColours();
        SetLabel(Program_Name);
        pi_scale_x=rzero; pi_scale_y=rzero; pi_factor_x=rzero; pi_factor_y=rzero; g_pi_factor_x=rzero; g_pi_factor_y=rzero;
        abst=10; x_scale=1; y_scale=1; gx_scale = gy_scale = 0.5;
        x_axis="x"; y_axis="y"; sx_scale="1"; sy_scale="1";// sgx_scale="1"; sgy_scale="1";
        //a_fstring=0;
        //n_fstring=1;
        fstring="";
        xstring="-5;5";
        ystring="-5;5";
        pen.del();
        active_function = -1;
        pointactive = 0;
        refreshfunctionstring();
        bitmapbackground = 0;
        actualBitmapBackground = wxBitmap();
        BackgroundImage.Destroy();
        GetSettings();
        Compute();
    }
    else if (id == 103 || id == 104) { // Save, Save as:
        if (id==104 || actual_filename=="") {
            wxFileDialog Dialog(this, _("Save File As"), wxEmptyString, wxEmptyString, filetype,wxFD_SAVE|wxFD_OVERWRITE_PROMPT,wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
            Dialog.SetDirectory(savefiledir);
            if (Dialog.ShowModal()==wxID_OK) {
                dirname=Dialog.GetDirectory();
                filename= Dialog.GetFilename();
            }
            else return;
        }
        else {dirname=actual_dirname;filename=actual_filename;}
        savefile(dirname,filename);
    }
    else if (id == 102) { // Open
        wxFileDialog Dialog(this,_("Open File"),wxEmptyString,wxEmptyString,filetype,wxFD_OPEN);
        std::string fullname,line;

        Dialog.SetDirectory(openfiledir);
        if (Dialog.ShowModal()==wxID_OK) {
            dirname=Dialog.GetDirectory();
            filename= Dialog.GetFilename();
        }
        else return;
        fullname = dirname + filesep + filename;
        if (!val::FileExists(fullname)) {
            wxMessageBox(fullname + "\ndoes not exist!");
            return;
        }
        std::fstream file(fullname,std::ios::in);
        if (!file) {
            wxMessageBox("Cannot open File\n " + fullname);
            return;
        }
        getline(file,line);
        if (line!="plotfunctionfile") {
            wxMessageBox("Wrong Type of file!");
            return;
        }
        bitmapbackground = 0;
        actualBitmapBackground = wxBitmap();
        BackgroundImage.Destroy();
        appendrecentfiles(fullname);
        openfile(dirname,filename);
        active_function = -1;
        pointactive = 0;
    }
}


void PlotFunctionFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("programmed by Miguel Valbuena\nLocation: " + val::GetExeDir(),"PlotFunction");
}

void PlotFunctionFrame::GetSizeSettings()
{
#ifdef _WIN32
    closeHand = new wxCursor(wxCURSOR_HAND);
#else
    if (val::FileExists(handcursor)) {
        closeHand= new wxCursor(handcursor,wxBITMAP_TYPE_PNG);
    }
    else closeHand = new wxCursor(wxCURSOR_HAND);
#endif // _WIN32

    std::ifstream file(settingsfile,std::ios::in);
    if (!file) {
        if (!val::DirExists(valdir)) {
            if (!val::CreateDir(valdir)) wxMessageBox("Cannot Create\n" + valdir);
        }
        if (!val::DirExists(settingsdir)) {
             if (!val::CreateDir(settingsdir)) wxMessageBox("Cannot Create\n" + settingsdir);
        }
        return;
    }
    int i=0;
    std::string line;
    settings=1;
    file>>Posx>>Posy>>clientsize_x>>clientsize_y>>fontsize>>axis_fontsize;
	if (clientsize_x == 0) clientsize_x = 400;
	if (clientsize_y == 0) clientsize_y = 400;
	if (fontsize < 8) fontsize = 8;
	file.clear();
	file.seekg(0,std::ios::beg);
	while (file && i<=5) {
        getline(file,line);
        ++i;
	}
	if (file) {getline(file,line);filedir=line;}
	if (file) {getline(file,line);openfiledir=line;}
	if (file) {getline(file,line);savefiledir=line;}
	if (file) {getline(file,line);defaultFont.SetNativeFontInfoUserDesc(wxString(line));}
	if (file) {getline(file,line);SideText_isshown = val::FromString<int>(line);}
	if (file) {getline(file,line);widthSideText = val::FromString<int>(line);}
	//if (file) {getline(file,line);defaultFont.SetNativeFontInfo(wxString(line));}
    file.close();

    if (widthSideText > 200) widthSideText = 200;
    if (widthSideText < 100) widthSideText = 100;

    //
    //wxMessageBox(val::ToString(SideText_isshown));
    std::fstream file1(RecentFilesPath,std::ios::in);
    while (file1) {
        getline(file1,line);
        if (line=="") break;
        else recentfiles.push_back(line);
    }

    int n = recentfiles.length();
    for (int i=0;i<n;++i) {
        recent_menu.push_back(nullptr);
        recent_menu[i] = new wxMenuItem(MenuRecentfiles,6000+i,recentfiles[i]);
        MenuRecentfiles->Insert(i,recent_menu[i]);
        Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuRecent,this,6000+i);
    }
    file1.close();
}


void PlotFunctionFrame::ResetColours()
{
    defaultpaintcolor = defaultcolors[BLUE]; //wxColour(0,0,255);  // blue
    Color[0] = defaultcolors[BLUE]; //wxColour(0,0,255);// blue
    Color[1] = defaultcolors[RED]; //wxColour(255,0,0); // red
    Color[2] = defaultcolors[GREEN]; //wxColour(0,255,0); // green
    Color[3] = defaultcolors[LBLUE]; //wxColour(0,230,246); // light blue
    Color[4] = defaultcolors[ORANGE]; //wxColour(255,116,0); // orange
    Color[5] = defaultcolors[VIOLET];  //wxColour(238,0,255); // violet
    Color[6] = defaultcolors[GREY]; //wxColour(125,125,125);  // grey
    BackgroundColor = defaultcolors[WHITE]; //wxColour(255,255,255);  // white
    axis_color = defaultcolors[BlACK]; //wxColour(0,0,0);
    grid_color = defaultcolors[LGREY]; //wxColour(191,191,191);
    axis_pen=2;grid_pen=1;
    DrawPanel->SetBackgroundColour(BackgroundColor);
    for (int i=0;i<pen.length();++i) pen[i] =2;
    bitmapbackground = 0;
    actualBitmapBackground = wxBitmap();
    BackgroundImage.Destroy();
}


void PlotFunctionFrame::GetSettings()
{

    ispainted=0;
    F.dellist();
    x_range.dellist();
    critpoints.del();
    //undef_intervals.del();
    critx.del();
    islinear.del();

    int n,i,j,nmenu=0, oldN = N;
    val::rational par;
    val::d_array<int> ischecked;
    std::string s="",f_s,t="",menuitemstring,helpstring;
    val::GPair<double> xr;
    myfunction f;

    N = 0;

#ifdef _WIN32
    Menu_functions->Remove(addfunction);
    Menu_functions->Remove(undomenu);
    Menu_functions->Remove(redomenu);
    Menu_functions->Remove(hideallmenu);
    Menu_functions->Remove(showallmenu);
    Menu_functions->Remove(deletelastmenu);
    Menu_functions->Remove(deleteallmenu);
#endif // _WIN32

    if (!f_menu.isempty()) {
        nmenu=f_menu.length();
        ischecked.resize(nmenu);
        for (i=0;i<nmenu;++i) {
            if (f_menu[i]->IsChecked()) ischecked[i]=1;
            else ischecked[i]=0;
            Menu_functions->Remove(2000+i);
            //Disconnect(2000+i,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PlotFunctionFrame::OnMenu_xAxisSelected);
            Unbind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenu_xAxisSelected,this,2000+i);
            delete f_menu[i];
            //Disconnect(4000+i,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PlotFunctionFrame::OnMenuColours);
            Unbind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuColours,this,4000+i);
            colorsubmenu->Delete(4000+i);
        }
        f_menu.del();
        c_menu.del();
    }

    val::d_array<char> separators({';'});
    val::Glist<std::string> svalues;

    if (xstring=="") {
        x1=-5;x2=5;
        xstring = "-5;5";
    }
    else {
        x1 = x2 = 0;
        svalues = getwordsfromstring(xstring,separators);
        n = svalues.length();
        if (n > 0) x1 = double(val::FromString<val::rational>(svalues[0]));
        if (n > 1) x2 = double(val::FromString<val::rational>(svalues[1]));
        /*
        n=xstring.size();
        s="";
        for (i=0;i<n;++i) {
            if (xstring[i]==';') {
                x1=double(val::FromString<val::rational>(s));
                //i++;
                s="";
            }
            else s+=xstring[i];
        }
        x2=double(val::FromString<val::rational>(s));
        */
        if (x1>=x2) {
            x1=-5;x2=5;
        }
    }
    if (ystring=="") {yset=0;}
    else {
        y1 = y2 = 0;
        yset=1;
        svalues = getwordsfromstring(ystring,separators);
        n = svalues.length();
        if (n > 0) y1 = double(val::FromString<val::rational>(svalues[0]));
        if (n > 1) y2 = double(val::FromString<val::rational>(svalues[1]));
        /*
        n=ystring.size();
        s="";
        for (i=0;i<n;++i) {
            if (ystring[i]==';') {
                y1=double(val::FromString<val::rational>(s));
                //i++;
                //break;
                s="";
            }
            else s+=ystring[i];
        }
        y2=double(val::FromString<val::rational>(s));
        */
        if (y1>=y2) yset=0;
    }


    s="";
    if (fstring=="") {setfunctionsmenu(); WriteText(); return;}
    val::d_array<char> ignore({'\n'});
    val::Glist<int> colorindezes;
    int cindex;

    svalues = getwordsfromstring(fstring,separators,0,ignore);
    for (auto& v : svalues) {
        cindex = getfunctionfromstring(v,f_s,xr.x,xr.y);
        f = myfunction(f_s);
        if (f.numberofvariables() > 1) {
            val::valfunction g(f.getinfixnotation());
            f = myfunction(g.getinfixnotation());
        }
        if (!f.iszero()) {
            F.push_back(std::move(f));
            x_range.push_back(xr);
            colorindezes.push_back(cindex);
            N++;
        }
    }

    /*

    n=fstring.size();

    for (i=0;i<n;i++) {
        if (fstring[i]=='\n') continue;
        if (fstring[i]==';') {
            getfunctionfromstring(s,f_s,xr.x,xr.y);
            //f=myfunction(s);
            f=myfunction(f_s);
            if (!f.iszero()) {
                F.inserttoend(std::move(f));
                x_range.inserttoend(xr);
                N++;
            }
            s="";
        }
        else s+=fstring[i];
    }
    getfunctionfromstring(s,f_s,xr.x,xr.y);
    f=myfunction(f_s);
    if (!f.iszero()) {
        F.inserttoend(std::move(f));
        x_range.inserttoend(xr);
        N++;
    }
    */

    if (N==0) {
        fstring="";
        setfunctionsmenu();
        WriteText();
        return;
    }
    f_menu.resize(N);
    c_menu.resize(N);
    if (N>Color.length())  {
        int i,l=Color.length();
        Color.resize(N);
        for (i=l;i<N;++i) {
            Color[i] = Color[i%7];
        }
    }

    i = 0;
    for (const auto &v : colorindezes) {
        if (v != -1) Color[i] = defaultcolors[v];
        ++i;
    }

    if (N>Font.length()) {
        int i,l=Font.length();
        Font.resize(N);
        for (i=l;i<N;++i) Font[i]=defaultFont;
    }
    if (N>pen.length()) {
        int i,l=pen.length();
        pen.resize(N);

        for (i=l;i<N;++i) {
            pen[i] = 2;
        }

    }
    n=Parameter.length();
    islinear.reserve(N);
    for (i=0;i<N;++i) {
        // mit menuItems:
        s="";
        islinear[i] = 0;
        if (F[i].getmode() == myfunction::FUNCTION) {
            if (islinearfunction(F[i])) islinear[i] = 1;
        }
        if (F[i].iswithparameter()) {
            if (n) {
                j=val::Min(i,n-1);
                par=Parameter[j];
            }
            else par=val::rational(0);
            F[i].setparameter(double(par));
            s= "  [" + val::ToString(par) + "]";
        }
        if (i<=8) t="\tAlt-" + val::ToString(i+1);
        else t="";

        menuitemstring = val::headofstring(F[i].getinfixnotation(),40);
        if (F[i].getinfixnotation().length()>40) menuitemstring+="...";
        helpstring = val::headofstring(F[i].getinfixnotation(),120);
        if (F[i].getinfixnotation().length()>120) helpstring+="...";
        f_menu[i]=new wxMenuItem(Menu_functions,2000+i,menuitemstring +s+t,helpstring +s, wxITEM_CHECK);
#ifdef _WIN32
        f_menu[i]->SetTextColour(Color[i]);
#endif // _WIN32
        // Only Windows : _menu[i]->SetTextColour(Color[i%Color.length()]);
        Menu_functions->Insert(i,f_menu[i]);
        if (i<nmenu) {
            if (ischecked[i]) f_menu[i]->Check(true);
            else f_menu[i]->Check(false);
        }
        else f_menu[i]->Check(true);
        Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenu_xAxisSelected,this,2000+i);
        if (i<=8) t="\tCtrl-" + val::ToString(i+1);
        else t="";
        {
            menuitemstring = val::headofstring(F[i].getinfixnotation(),40);
            if (F[i].getinfixnotation().length()>40) menuitemstring+="...";
            c_menu[i] = new wxMenuItem(colorsubmenu,4000+i,menuitemstring +t);
            colorsubmenu->Append(c_menu[i]);
        }
        Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuColours,this,4000+i);
    }

    fstring="";
    for (i=0;i<N;++i) {
        fstring+=F[i].getinfixnotation();
        if (i >= oldN && !multicolormenu->IsChecked()) Color[i] = defaultpaintcolor;
        if (x_range[i].x == x_range[i].y) {
            fstring+=";\n";
            continue;
        }
        if (x_range[i].y <= x_range[i].x) {x_range[i].x=x1; x_range[i].y=x2;}
        //fstring+=F[i].getinfixnotation();
        if (x_range[i].x!=x1 || x_range[i].y!=x2) fstring+= "  [ "+ val::ToString(x_range[i].x) +" , " + val::ToString(x_range[i].y) + " ];\n";
        else fstring+=";\n";
    }
    setfunctionsmenu();
    WriteText();
}


/*
void PlotFunctionFrame::valFloodFill(wxMemoryDC& dc, int x, int y)
{
    wxColour pcolor, bgc;
    int xmin = abst, xmax = sizex + abst, ymin = abst, ymax = sizey + abst, paint;
    int x1, x2, xstart, dy;
    quadruple s;
    val::Glist<quadruple> S;

    dc.GetPixel(x,y,&bgc);

    S.push_back(quadruple(x,x,y,1));
    S.push_back(quadruple(x,x,y-1,-1));

    while (!S.isempty()) {
        s = S.getelement(); S.skiphead();
        x = x1 = s.x1; x2 = s.x2; y = s.y; dy = s.dy;
        paint = 0;
        dc.GetPixel(x,y,&pcolor);
        if (x <= xmax && x>=xmin && y <= ymax && y >= ymin && pcolor == bgc) {
            while (x-1 >= xmin) {
                dc.GetPixel(x-1,y,&pcolor);
                if (pcolor != bgc) break;
                if (!paint) {
                    paint = 1;
                    xstart = x -1;
                }
                --x;
            }
            if (paint) dc.DrawLine(x,y,xstart,y);
            if (x < x1) S.push_back(quadruple(x, x1-1, y-dy, -dy));
        }
        while (x1 <= x2) {
            paint = 0;
            while (1) {
                if (x1 < xmin || x1 > xmax || y < ymin || y > ymax) break;
                dc.GetPixel(x1,y,&pcolor);
                if (pcolor != bgc) break;
                if (!paint) {
                    paint = 1;
                    xstart = x1;
                }
                ++x1;
            }
            if (paint) dc.DrawLine(xstart,y,x1,y);
            S.push_back(quadruple(x, x1 - 1, y+dy, dy));
            if (x1 - 1 > x2) S.push_back(quadruple(x2 + 1, x1 - 1, y-dy, -dy));
            ++x1;
            while (x1 < x2) {
                if (x1 > xmax || x1 < xmin || y < ymin || y > ymax) break;
                dc.GetPixel(x1,y,&pcolor);
                if (pcolor == bgc) break;
                ++x1;
            }

            x = x1;
        }
    }
}

*/





void PlotFunctionFrame::valFloodFill(wxMemoryDC& dc, int x, int y, const wxColour &fgc)
{
    if (cpaper == nullptr) return;
    dc.SelectObject(wxNullBitmap);
    wxNativePixelData data(*cpaper);
    //wxNativePixelData data(dc.GetSelectedBitmap());

    if (!data) {
        dc.SelectObject(*cpaper);
        return;
    }

    wxNativePixelData::Iterator p(data);
    wxColour pcolor, bgc;
    int xmin = abst, xmax = sizex + abst, ymin = abst, ymax = sizey + abst;
    int x1, x2, dy;
    char red = fgc.Red(), green = fgc.Green(), blue = fgc.Blue();
    quadruple s;
    val::Glist<quadruple> S;

    //dc.GetPixel(x,y,&bgc);
    p.MoveTo(data,x,y);
    bgc = wxColour(p.Red(),p.Green(),p.Blue());

    S.push_back(quadruple(x,x,y,1));
    S.push_back(quadruple(x,x,y-1,-1));

    //std::cout << int(bgc.Red()) << " , " << int(bgc.Green()) << " , " << int(bgc.Blue()) << std::endl;
    //std::cout << xmin << " , " << xmax << std::endl;
    //return;

    while (!S.isempty()) {
        s = S.getelement(); S.skiphead();
        x = x1 = s.x1; x2 = s.x2; y = s.y; dy = s.dy;
        //paint = 0;
        //dc.GetPixel(x,y,&pcolor);
        p.MoveTo(data,x,y); pcolor = wxColour(p.Red(),p.Green(),p.Blue());

        if (x <= xmax && x>=xmin && y <= ymax && y >= ymin && pcolor == bgc) {
            while (x-1 >= xmin) {
                //dc.GetPixel(x-1,y,&pcolor);
                p.MoveTo(data,x-1,y); pcolor = wxColour(p.Red(),p.Green(),p.Blue());
                if (pcolor != bgc) break;
                p.Red() = red; p.Green() = green; p.Blue() = blue;
                /*
                if (!paint) {
                    paint = 1;
                    xstart = x -1;
                }
                */

                --x;
            }
            //if (paint) dc.DrawLine(x,y,xstart,y);
            if (x < x1) S.push_back(quadruple(x, x1-1, y-dy, -dy));
        }
        while (x1 <= x2) {
            //paint = 0;
            while (1) {
                if (x1 < xmin || x1 > xmax || y < ymin || y > ymax) break;
                //dc.GetPixel(x1,y,&pcolor);
                p.MoveTo(data,x1,y); pcolor = wxColour(p.Red(),p.Green(),p.Blue());
                if (pcolor != bgc) break;
                /*
                if (!paint) {
                    paint = 1;
                    xstart = x1;
                }
                */
                p.Green() = green; p.Red() = red; p.Blue() = blue;
                ++x1;
            }
            //if (paint) dc.DrawLine(xstart,y,x1,y);
            S.push_back(quadruple(x, x1 - 1, y+dy, dy));
            if (x1 - 1 > x2) S.push_back(quadruple(x2 + 1, x1 - 1, y-dy, -dy));
            ++x1;
            while (x1 < x2) {
                if (x1 > xmax || x1 < xmin || y < ymin || y > ymax) break;
                //dc.GetPixel(x1,y,&pcolor);
                p.MoveTo(data,x1,y); pcolor = wxColour(p.Red(),p.Green(),p.Blue());
                if (pcolor == bgc) break;
                ++x1;
            }

            x = x1;
        }
    }
    dc.SelectObject(*cpaper);
}




void PlotFunctionFrame::plotvertices(wxDC& dc)
{
    dc.SetTextForeground(axis_color);

    int ypainted = 0, xpainted = 0;
    wxFont normalfont=dc.GetFont(), italicfont=dc.GetFont();
    int normalsize = normalfont.GetPointSize(), nsize=normalsize , dscale =3,dx=4,dy=6,pen=axis_pen;

    nsize = axis_fontsize;

    italicfont.MakeItalic();
    italicfont.MakeBold();
    wxString pi = L"\u03C0",swert;
    val::rational factor,one(1),minusoune(-1);

    wxSize sz=dc.GetSize();

    sizex=sz.x -2*abst;
    sizey=sz.y-2*abst;

    if (sizex <320) {nsize-=2;dscale--;dx--;dy--;}
    if (sizex < 220) {nsize -=2;dscale--;dx--;dy--;pen--;}
    if (nsize < 2) nsize =2;
    italicfont.SetPointSize(nsize);

    decimalx = decimal((x2-x1)/double(sizex));
    decimaly = decimal((y2-y1)/double(sizey));
    yzero=abst+ int(val::round(double(sizey-1)*(y2/(y2-y1)),0));

    if (gridactiv->IsChecked() && !fillfunctions && yset && ((y2-y1)/gy_scale)<=50 && ((x2-x1)/gx_scale)<=50) {
        dc.SetPen(wxPen(grid_color,grid_pen)); // grey
        int f=int(x1/gx_scale),ix,iy;
        for (double x= double(f)*gx_scale;x<=x2;x+=gx_scale) {
            if (x<x1) continue;
            ix=abst+ int(double(sizex-1)*((x-x1)/(x2-x1)));
            dc.DrawLine(ix,abst,ix,abst+sizey);
        }
        f=int(y1/gy_scale);
        for (double y= double(f)*gy_scale;y <= y2;y+=gy_scale) {
            if (y<y1) continue;
            iy=abst-1 + sizey - int(val::round(double(sizey-1)*((y-y1)/(y2-y1)),0));
            dc.DrawLine(abst,iy,abst+sizex,iy);
        }
    }

    dc.SetPen( wxPen( axis_color, pen ) );

    if (y1*y2<=0) {
        if (Menu_xAxis->IsChecked()) {
            xpainted=1;
            dc.DrawLine(abst,yzero,abst+sizex,yzero);
            dc.DrawLine(abst+sizex-dy,yzero-dx,abst+sizex,yzero);
            dc.DrawLine(abst+sizex-dy,yzero+dx,abst+sizex,yzero);
            dc.SetFont(italicfont);
            dc.DrawText(x_axis,abst+sizex-5,yzero+5);
            dc.SetFont(normalfont);
        }
    }
    if (x1*x2<=0) {
        int x = abst+1+ int(double(sizex-1)*((-x1)/(x2-x1)));
        if (Menu_yAxis->IsChecked()) {
            ypainted=1;
            dc.DrawLine(x,abst,x,abst+sizey);
            dc.DrawLine(x-dx,abst+dy,x,abst);
            dc.DrawLine(x+dx,abst+dy,x,abst);
            dc.SetFont(italicfont);
            dc.DrawText(y_axis,x+10,abst-10);
            dc.SetFont(normalfont);
        }
    }
    if (x_scaleactiv->IsChecked()) {
        if (((x2-x1)/x_scale) <=50.0) {
            normalfont.SetPointSize(nsize);
            dc.SetFont(normalfont);
            int ix, ch = dc.GetCharHeight(), cw = dc.GetCharWidth();

            int f=int(x1/x_scale);
            for (double x= double(f)*x_scale;x<=x2;x+=x_scale,++f) {
                if (x<x1) continue;
                if (x==0 && ypainted) continue;
                ix=abst+ int(double(sizex-1)*((x-x1)/(x2-x1)));
                if (pi_scale_x) {
                    swert="";
                    factor = val::rational(f) * pi_factor_x;
                    if (factor==minusoune) swert="-";
                    else if (factor != one) swert=val::ToString(factor);
                    if (factor.iszero()) swert="0";
                    else swert += pi;
                }
                else swert = val::ToString(x);
                if (xpainted) {
                    if (x==x2) break;
                    dc.DrawLine(ix,yzero+dscale,ix,yzero-dscale);
                    dc.DrawText(swert,ix-(swert.size()*cw)/2,yzero+3);
                }
                else {
                    dc.DrawText(swert,ix-(swert.size()*cw)/2,abst + sizey-ch+3);
                }
            }
            normalfont.SetPointSize(normalsize);
            dc.SetFont(normalfont);
        }
    }
    if (y_scaleactiv->IsChecked() && yset) {
        if (((y2-y1)/y_scale) <=50) {
            normalfont.SetPointSize(nsize);
            dc.SetFont(normalfont);
            int cw = dc.GetCharWidth(), ch = dc.GetCharHeight()/2;
            int iy,xzero=abst+1+ int(double(sizex-1)*((-x1)/(x2-x1)));

            //if (nsize >= 15) {cw -= 1;}
            //if (nsize >= 20) {cw -= 2;}
            //dyaxis = nsize;

            int f=int(y1/y_scale);
            for (double y= double(f)*y_scale;y <= y2;y+=y_scale,++f) {
                if (y<y1) continue;
                if (y==0 && xpainted) continue;
                iy=abst-1 + sizey - int(double(sizey-1)*((y-y1)/(y2-y1)));
                if (pi_scale_y) {
                    swert="";
                    factor = val::rational(f) * pi_factor_y;
                    if (factor==minusoune) swert="-";
                    else if (factor != one) swert=val::ToString(factor);
                    if (factor.iszero()) swert="0";
                    else swert += pi;
                }
                else swert = val::ToString(y);
                if (ypainted) {
                    if (y==y2) break;
                    dc.DrawLine(xzero-dscale,iy,xzero+dscale,iy);
                    dc.DrawText(swert,xzero - (swert.size())*cw - 5,iy-ch);
                }
                else {
                    if (y==y1) continue;
                    dc.DrawText(swert,0,iy-ch);
                }
            }
            normalfont.SetPointSize(normalsize);
            dc.SetFont(normalfont);
        }
    }
}



void PlotFunctionFrame::plotfunction(wxDC& dc,const val::d_array<double> &f,int colour)
{
    int ix0,ix1,iy0,iy1,index,ylimit=abst+sizey-1,aw=0,ew=sizex, ready = 0, i, k;
    double faktor_x,faktor_y,xr1=x_range[colour].x,xr2=x_range[colour].y, yvalue = 0, yold = val::Inf, dyzero(yzero);

    if (active_function == colour) dc.SetPen(wxPen(Color[colour],pen[colour]+3));
    else dc.SetPen(wxPen(Color[colour],pen[colour]));

    if (x_range[colour].x == x_range[colour].y) {
        aw=0;
        ew=sizex;
    }
    else {
        xr1=val::Max(x1,xr1);
        xr2=val::Min(x2,xr2);
        aw=int(val::round(double(sizex-1)*((xr1-x1)/(x2-x1)),0));
        ew=sizex - int(val::round(double(sizex-1)*((x2-xr2)/(x2-x1)),0));
    }
    if (aw<0) aw=0;
    if (ew>sizex) ew=sizex;

    faktor_x=double(points-1)/double(sizex-1);
    faktor_y=double(sizey-1)/(y2-y1);

    if (islinear[colour]) {
        i = aw;
        do {
            index = int (val::round(double(i)*faktor_x,0));
            iy0 = yzero-int(val::round(faktor_y * f[index],0));
            ++i;
            if (i>=ew) return;
        }
        while (iy0 < abst || iy0 > ylimit);
        if (i > ew) return;
        ix0 = i-1 + abst;

        i = ew - 1;
        do {
            index = int (val::round(double(i)*faktor_x,0));
            iy1 = yzero-int(val::round(faktor_y * f[index],0));
            --i;
            if (i<aw) return;
        }
        while (iy1 < abst || iy1 > ylimit);
        ix1 = i + 1 + abst;
        dc.DrawLine(ix0,iy0,ix1,iy1);
        return;
    }

    //wxPoint *s_points = new wxPoint[ew];
    //int j = 0;

    i = aw;
    do {
        k = 0;
        do { // search first point:
            ++k;
            yold = yvalue;
            ix0 = i + abst;
            index = int (val::round(double(i)*faktor_x,0));
            if (index<0 || index >= points) return;
            yvalue = f[index];
            //iy0=yzero-int(val::round(faktor_y * yvalue,0));
            iy0 = int(val::round(dyzero - faktor_y*yvalue,0));//yzero-int(val::round(faktor_y * yvalue,0));
            ++i;
            if (i >= ew) return;
        }
        while (isInf(yvalue) || val::isNaN(yvalue) || (iy0 > ylimit) || (iy0 < abst));

        if ((k > 1) && !isInf(yold) && !val::isNaN(yold)) {
            --i; --ix0;
            //iy0 = yzero - int(val::round(faktor_y * yold ,0));
            iy0 = int(val::round(dyzero - faktor_y*yold,0));//yzero - int(val::round(faktor_y * yold ,0));
            if (iy0 > ylimit) iy0 = ylimit;
            if (iy0 < abst) iy0 = abst;
        }

        for (; i < ew; ++i, ix0 = ix1, iy0 = iy1) {
            ix1=abst+i;
            index=int (val::round(double(i)*faktor_x,0));
            if (index<0 || index >=points) return;
            yvalue = f[index];
            //iy1=yzero -int(val::round(faktor_y* yvalue,0));
            iy1= int(val::round(dyzero - faktor_y*yvalue,0)); //yzero -int(val::round(faktor_y* yvalue,0));
            if (isInf(yvalue) || val::isNaN(yvalue)) { // || (iy1 > ylimit) || (iy1 < abst)) {
                ++i;
                break;
            }
            if (iy1 > ylimit || iy1 < abst) {
                iy1 = val::Max(iy1,abst);
                iy1 = val::Min(iy1,ylimit);
                dc.DrawLine(ix0,iy0,ix1,iy1);
                ++i;
                break;
            }
            dc.DrawLine(ix0,iy0,ix1,iy1);
            //s_points[j].x = ix1;
            //s_points[j].y = iy1;
            //++j;
        }
        if (i >= ew) ready = 1;
    }
    while (!ready);

    //dc.DrawSpline(ew,s_points);
    //delete[] s_points;
}


void PlotFunctionFrame::plotline(wxDC& dc,const val::d_array<double> &f,int colour)
{
    if (!yset) return;

    int ix0,iy0,ix1,iy1;

    if (active_function == colour) dc.SetPen(wxPen(Color[colour],pen[colour]+3));
    else dc.SetPen(wxPen(Color[colour],pen[colour]));

    val::d_array<double> g(f);
    double inf=val::Inf,infm=-val::Inf;

    for (int i=0;i<4;i+=2) {
        if (val::isNaN(g[i]) || val::isNaN(g[i+1])) return;
        if (g[i]==infm) g[i] = x1;
        if (g[i]==inf) g[i] = x2;
        if (g[i+1]==infm) g[i+1] = y1;
        if (g[i+1]==inf) g[i+1] = y2;
    }

    ix0=abst+int(double(sizex-1)*((g[0]-x1)/(x2-x1)));
    iy0=yzero -int((double(sizey-1)/double(y2-y1)) * g[1]);
    ix1=abst+int(double(sizex-1)*((g[2]-x1)/(x2-x1)));
    iy1=yzero -int((double(sizey-1)/double(y2-y1)) * g[3]);

    if (n_linepoints && colour == N-1) {
        if (polygonline) {
            dc.SetPen(wxPen(Color[colour],1));
            dc.DrawLine(wxPoint(ix0,iy0),actualpolygonpoint);
            polygonline = 0;
        }
		int r = val::Max(1,(pen[colour]+2)/2);
        dc.SetPen(wxPen(Color[colour],pen[colour]+2));
        dc.SetBrush(wxBrush(Color[colour]));
        dc.DrawCircle(ix0,iy0,r);
        //dc.SetPen(wxPen(Color[colour],8));
        //dc.DrawPoint(ix0,iy0);
        return;
    }

    dc.DrawLine(ix0,iy0,ix1,iy1);
}


void PlotFunctionFrame::plotcircle(wxDC& dc,const val::d_array<double> &f,int colour)
{
    if (!yset || sizex <= 10 || sizey <=10 ) return;

    int ix0,iy0,ix1,iy1;
    double x,y,r,a1,a2,lx,ly;
    int slice;
    F[colour].getCirclePoints(x,y,r,a1,a2,slice);

    if (r <= 0) return;


    if (active_function == colour) dc.SetPen(wxPen(Color[colour],pen[colour]+3));
    else dc.SetPen(wxPen(Color[colour],pen[colour]));
    wxBrush brush(Color[colour]);
    if (slice < 2) brush.SetStyle(wxBrushStyle::wxBRUSHSTYLE_TRANSPARENT);
    dc.SetBrush(brush);

    ix0=abst+int(double(sizex-1)*((f[0]-x1)/(x2-x1)));
    iy0=yzero -int((double(sizey-1)/double(y2-y1)) * f[1]);
    ix1=abst+int(double(sizex-1)*((f[2]-x1)/(x2-x1)));
    iy1=yzero -int((double(sizey-1)/double(y2-y1)) * f[3]);
    ix1-=ix0;
    iy1-=iy0;

    dc.DrawEllipticArc(ix0,iy0,ix1,iy1,f[4],f[5]);

    if (slice == 1) {
        ix0=abst+int(double(sizex-1)*((x-x1)/(x2-x1)));
        iy0=yzero -int((double(sizey-1)/double(y2-y1)) * y);
        lx = x + r * val::cos(a1 * val::PI/180.0);
        ly = y + r * val::sin(a1 * val::PI/180.0);
        ix1 = abst+int(double(sizex-1)*((lx-x1)/(x2-x1)));
        iy1=yzero -int((double(sizey-1)/double(y2-y1)) * ly);
        dc.DrawLine(ix0,iy0,ix1,iy1);
        lx = x + r * val::cos(a2 * val::PI/180.0);
        ly = y + r * val::sin(a2 * val::PI/180.0);
        ix1 = abst+int(double(sizex-1)*((lx-x1)/(x2-x1)));
        iy1=yzero -int((double(sizey-1)/double(y2-y1)) * ly);
        dc.DrawLine(ix0,iy0,ix1,iy1);
    }


    if (active_function == colour) {
        //double x,y,r,a1,a2;
        //int slice;
        //F[colour].getCirclePoints(x,y,r,a1,a2,slice);
        ix0=abst+int(double(sizex-1)*((x-x1)/(x2-x1)));
        iy0=yzero -int((double(sizey-1)/double(y2-y1)) * y);
        dc.DrawPoint(ix0,iy0);
    }
}


void PlotFunctionFrame::plotrectangle(wxDC& dc,const val::d_array<double> &f,int colour)
{
    if (!yset) return;
    if (f[0]<x1 || f[0]>x2 || f[1]<y1 || f[1]>y2 || f[2]<x1 || f[2]>x2 || f[3]<y1 || f[3]>y2) return;

    int ix0,iy0,ix1,iy1;

    if (active_function == colour) dc.SetPen(wxPen(Color[colour],pen[colour]+3));
    else dc.SetPen(wxPen(Color[colour],pen[colour]));

    wxBrush brush;
    brush.SetStyle(wxBrushStyle::wxBRUSHSTYLE_TRANSPARENT);
    dc.SetBrush(brush);

    ix0=abst+int(double(sizex-1)*((f[0]-x1)/(x2-x1)));
    iy0=yzero -int((double(sizey-1)/double(y2-y1)) * f[1]);
    ix1=abst+int(double(sizex-1)*((f[2]-x1)/(x2-x1)));
    iy1=yzero -int((double(sizey-1)/double(y2-y1)) * f[3]);
    ix1-=ix0;
    iy1-=iy0;

    dc.DrawRectangle(ix0,iy0,ix1,iy1);
}

void PlotFunctionFrame::plottriangle(wxDC& dc,const val::d_array<double> &f,int colour)
{
    if (!yset) return;
    if (f[0]<x1 || f[0]>x2 || f[1]<y1 || f[1]>y2 || f[2]<x1 || f[2]>x2 || f[3]<y1 || f[3]>y2 || f[4]<x1 || f[4]>x2
            || f[5]<y1 || f[5]>y2) return;

    int ix0,iy0,ix1,iy1,ix2,iy2;

    if (active_function == colour) dc.SetPen(wxPen(Color[colour],pen[colour]+3));
    else dc.SetPen(wxPen(Color[colour],pen[colour]));

    ix0=abst+int(double(sizex-1)*((f[0]-x1)/(x2-x1)));
    iy0=yzero -int((double(sizey-1)/double(y2-y1)) * f[1]);
    ix1=abst+int(double(sizex-1)*((f[2]-x1)/(x2-x1)));
    iy1=yzero -int((double(sizey-1)/double(y2-y1)) * f[3]);
    ix2=abst+int(double(sizex-1)*((f[4]-x1)/(x2-x1)));
    iy2=yzero -int((double(sizey-1)/double(y2-y1)) * f[5]);

    dc.DrawLine(ix0,iy0,ix1,iy1);
    dc.DrawLine(ix1,iy1,ix2,iy2);
    dc.DrawLine(ix0,iy0,ix2,iy2);
}


void PlotFunctionFrame::plotfill(wxMemoryDC& dc,const val::d_array<double> &f,int colour)
{
    if (!yset || bitmapbackground) return;
#ifdef __APPLE__
    if (DrawPanel->HasCapture()) return;
#endif // __APPLE__

    if (f[0]<x1 || f[0]>x2 || f[1]<y1 || f[1]>y2) return;

    int ix,iy;
    wxColor &col =Color[colour], bgc;

    if (f[2]!=1.0) {
        char green=col.Green(),blue=col.Blue(),red=col.Red();
        int transp = int(val::round(f[2]*255,0));

        col = wxColour(red,green,blue,transp);
    }

    ix=abst+int(double(sizex-1)*((f[0]-x1)/(x2-x1)));
    iy=yzero -int((double(sizey-1)/double(y2-y1)) * f[1]);

    dc.SetPen(wxPen(col,1));
    valFloodFill(dc,ix,iy,col);
/*
#ifndef __APPLE__
    dc.GetPixel(ix,iy,&bgc);
    wxBrush brush(col);
    dc.SetBrush(brush);
    dc.FloodFill(ix,iy,bgc);
#else
    dc.SetPen(wxPen(col,1));
    valFloodFill(dc,ix,iy,col);
#endif // __APPLE
*/
}


void PlotFunctionFrame::plotpolygon(wxDC& dc,const val::d_array<double> &f,int colour)
{
    if (!yset) return;
    int i,n=f.length();

    //if (n<2) return;

    for (i=0;i<n;i+=2) {
        if (f[i]<x1 || f[i]>x2) return;
        if (f[i+1]<y1 || f[i+1]>y2) return;
    }

    int ix0,iy0,ix1,iy1;

    if (active_function == colour) dc.SetPen(wxPen(Color[colour],pen[colour]+3));
    else dc.SetPen(wxPen(Color[colour],pen[colour]));

    ix0=abst+int(double(sizex-1)*((f[0]-x1)/(x2-x1)));
    iy0=yzero -int((double(sizey-1)/double(y2-y1)) * f[1]);

    for (i=2;i<n;i+=2) {
        ix1=abst+int(double(sizex-1)*((f[i]-x1)/(x2-x1)));
        iy1=yzero -int((double(sizey-1)/double(y2-y1)) * f[i+1]);
        dc.DrawLine(ix0,iy0,ix1,iy1);
        ix0=ix1;
        iy0=iy1;
    }

    if (drawpolygon && colour == N-1) {
        if (polygonline) {
            dc.SetPen(wxPen(Color[colour],1));
            dc.DrawLine(wxPoint(ix0,iy0),actualpolygonpoint);
            polygonline = 0;
        }
        int r = val::Max(1,(pen[colour]+2)/2);
        dc.SetPen(wxPen(Color[colour],pen[colour]+2));
        dc.SetBrush(wxBrush(Color[colour]));
        dc.DrawCircle(ix0,iy0,r);
        //dc.SetPen(wxPen(Color[colour],8));
        //dc.DrawPoint(ix0,iy0);
        actualpolygonpoint = wxPoint(ix0,iy0);
    }
}

void PlotFunctionFrame::plotpoints(wxDC& dc,const val::d_array<double> &f,int colour)
{
    if (!yset) return;
    int i,n=f.length();
    if (n<2) return;

    int ix,iy, r = val::Max(1,(pen[colour]+2)/2);


    dc.SetPen(wxPen(Color[colour],pen[colour]+2));
    dc.SetBrush(wxBrush(Color[colour]));

    for (i=0;i<n;i+=2) {
        if (isInf(f[i]) || isInf(f[i+1]) || val::isNaN(f[i]) || val::isNaN(f[i+1])) continue;
        ix=abst+int(double(sizex-1)*((f[i]-x1)/(x2-x1)));
        iy=yzero -int((double(sizey-1)/double(y2-y1)) * f[i+1]);
        if (f[i]<x1 || f[i]>x2) continue;
        if (iy<abst || iy >(abst+sizey)) continue;
        if (active_function == colour && i == pointactive) {
            dc.SetPen(wxPen(Color[colour],pen[colour]+4));
        }
//#ifdef _WIN32
        dc.DrawCircle(ix,iy,r);
        //dc.FloodFill(ix,iy,dc.GetBackground().GetColour());
//#else
        //dc.DrawCircle(ix,iy,1);
        //dc.DrawPoint(ix,iy);
//#endif // _WIN32
        if (active_function == colour && i == pointactive) {
            dc.SetPen(wxPen(Color[colour],pen[colour]+2));
        }
    }
}



void PlotFunctionFrame::plottext(wxDC& dc,const val::d_array<double> &f,const wxString& text,int colour)
{
    if (!yset) return;
    if (f[0]<x1 || f[0]>x2 || f[1]<y1 || f[1]>y2) return;

    wxFont or_font(dc.GetFont()), font(Font[colour]);

    if (!font.IsNull()) {
        int p = font.GetPointSize();
        if (active_function == colour) font.SetPointSize(p+5);
        dc.SetFont(font);
    }

    int ix0,iy0;
    ix0=abst+int(double(sizex-1)*((f[0]-x1)/(x2-x1)));
    iy0=yzero -int((double(sizey-1)/double(y2-y1)) * f[1]);
    dc.SetTextForeground(Color[colour]);
    dc.DrawText(text,ix0,iy0);
    dc.SetFont(or_font);

}

void PlotFunctionFrame::plotcurve(wxDC& dc,const val::d_array<val::d_array<double> > &f,const val::Glist<val::GPair<double>>& cpoints,
                                  const val::d_array<double>& cx,int colour)
{
    int ix,x,y,ylimit=abst+sizey,index,n_critx=cx.length(),i_cx,i_cx0,j,m,k,oldx=abst;
    double xval=x1,dx=(x2-x1)/double(points-1);
    val::d_array<int> oldy,critindex;

    if (active_function == colour) {
        dc.SetPen(wxPen(Color[colour],pen[colour]+3));
    }
    else dc.SetPen(wxPen(Color[colour],pen[colour]));


    if (islinear[colour]) {
        int i = 0, ew = sizex, ix0, ix1, iy0, iy1;
        double faktor_x = double(points-1)/double(sizex-1) , faktor_y = double(sizey-1)/(y2-y1);

        do {
            index = int (val::round(double(i)*faktor_x,0));
            iy0 = yzero-int(val::round(faktor_y * f[index][0],0));
            ++i;
            if (i>=ew) return;
        }
        while (iy0 < abst || iy0 > ylimit);
        if (i > ew) return;
        ix0 = i-1 + abst;

        i = ew - 1;
        do {
            index = int (val::round(double(i)*faktor_x,0));
            iy1 = yzero-int(val::round(faktor_y * f[index][0],0));
            --i;
            if (i<0) return;
        }
        while (iy1 < abst || iy1 > ylimit);
        ix1 = i + 1 + abst;
        dc.DrawLine(ix0,iy0,ix1,iy1);
        return;
    }



    critindex.reserve(cx.length());
    for (i_cx=0;i_cx < n_critx;++i_cx) {
        if (x1<=cx[i_cx]) break;
    }
    i_cx0=i_cx;

    for (index=0;index<points;++index,xval+=dx) {
        ix= int(val::round(double(index)*double(sizex-1)/double(points-1),0));
        x=abst+ix;
        m = f[index].length();
        if (i_cx < n_critx && xval>cx[i_cx]) {
            critindex.push_back(index);
            oldy.resize(m);
            for (j=0;j<m;++j) {
                y=yzero -int(val::round(double(sizey-1)/double(y2-y1) * f[index][j],0));
                oldy[j] = y;
                if (y>=abst && y<=ylimit) dc.DrawPoint(x,y);
            }
            ++i_cx;
        }
        else {
            k=oldy.length();
            for (j=0;j<m;++j) {
                y=yzero -int(val::round(double(sizey-1)/double(y2-y1) * f[index][j],0));
                if (j>=k) {
                    oldy.resize(m);
                    if (y>=abst && y<=ylimit) dc.DrawPoint(x,y);
                }
                else {
                    if (y>=abst && y<=ylimit) dc.DrawLine(oldx,oldy[j],x,y);
                }
                oldy[j] = y;
            }
        }
        oldx=x;

        for (const auto &v : f[index]) {
            y=yzero -int(val::round(double(sizey-1)/double(y2-y1) * v,0));
            if (y<abst || y > ylimit) continue;
            dc.DrawPoint(x,y);
        }
    }

    if (cpoints.isempty()) return;

    val::Glist<val::GPair<int>> cand;
    val::GPair<int> q;
    int i,found,l,lim;

    k=critindex.length();

    // Plot critical points:
    for (const auto& p : cpoints) {
        cand.dellist();
        found=0;
        for (i=0;i<k;++i) {
            if (val::abs(p.x - cx[i+i_cx0])<1e-4) {
                found=1;break;
            }
        }
        if (!found) continue;
        if (critindex[i]==0) continue;
        x=abst+int(val::round(double(sizex-1)*((p.x-x1)/(x2-x1)),0));
        y=yzero -int(val::round(double(sizey-1)/double(y2-y1) * p.y,0));
        i=critindex[i];
        if (f[i-1].length()<2 && f[i].length()<2) lim =1;
        else lim = 2;
        // points left of critical point:
        if ((l=f[i-1].length())>=lim) {
            for (j=0;j<l;++j) {
                q.x=abst+ int(val::round(double(i-1)*double(sizex-1)/double(points-1),0));
                q.y=yzero -int(val::round(double(sizey-1)/double(y2-y1) * f[i-1][j],0));
                inserttocand(cand,q,y);
            }
        }
        // points right of critical point:
        if ((l=f[i].length())>=lim) {
            for (j=0;j<l;++j) {
                q.x=abst + int(val::round(double(i)*double(sizex-1)/double(points-1),0));
                q.y=yzero -int(val::round(double(sizey-1)/double(y2-y1) * f[i][j],0));
                inserttocand(cand,q,y);
            }
        }

        l=val::Min(2,cand.length());
        if (l==0 && p.x>=x1 && p.x <=x2 && y>=abst && y<=ylimit) {
            dc.DrawPoint(x,y);
            continue;
        }
        for (i=0;i<l;++i) {
            dc.DrawLine(x,y,cand[i].x,cand[i].y);
        }
    }
}


void PlotFunctionFrame::plotallfunctions(wxMemoryDC& dc)
{
    int i_f=0,i_c=0;// i_rf = 0;

    for (int i=0;i<N;++i) {
        if (F[i].numberofvariables()==1) {
            if (f_menu[i]->IsChecked()) {
                switch (F[i].getmode()) {
                    case myfunction::LINE :
                        plotline(dc,farray[i_f],i); break;
                    case myfunction::CIRCLE :
                        plotcircle(dc,farray[i_f],i); break;
                    case myfunction::TEXT :
                        plottext(dc,farray[i_f],F[i].getTextData(),i);break;
                    case myfunction::RECTANGLE :
                        plotrectangle(dc,farray[i_f],i);break;
                    case myfunction::TRIANGLE :
                        plottriangle(dc,farray[i_f],i);break;
                    case myfunction::POLYGON :
                        plotpolygon(dc,farray[i_f],i);break;
                    case myfunction::POINTS :
                        plotpoints(dc,farray[i_f],i);break;
                    case myfunction::FILL :
                        if (fillfunctions) {plotfill(dc,farray[i_f],i);} break;
                    default:
                        {
                            //wxGCDC gdc(dc);
                            plotfunction(dc,farray[i_f],i);
                        } break;
                }

            }
            ++i_f;

        }
        else { // alg. Kurve.
            if (f_menu[i]->IsChecked()) plotcurve(dc,curvearray[i_c],critpoints[i_c],critx[i_c],i);
            ++i_c;
        }
    }
}


void PlotFunctionFrame::Paint()
{
    if (!ispainted) return;

    ispainted=0;
    wxClientDC dc1(DrawPanel);

    wxBitmap paper = wxBitmap(DrawPanel->GetSize());

    cpaper = &paper;

    // Create a memory Device Context
    wxMemoryDC dc;
    // Tell memDC to write on “paper”.
    dc.SelectObject( paper );
    // Call Repin to draw our picture on memDC
    //dc.SetBackground(wxBrush(BackgroundColor));
    //dc.SetBackground(*wxTRANSPARENT_BRUSH);
    //dc.SetFont(dc1.GetFont());

    //dc.Clear();

    plottomemoryDc(dc);
    /*
    fillfunctions=0;
    for (int i=0;i<N;++i) {
        if (F[i].IsFill() && f_menu[i]->IsChecked()) ++fillfunctions;
    }

    plotvertices(dc);
    plotallfunctions(dc);
    if (fillfunctions && gridactiv->IsCheck()) {
        fillfunctions=0;
        plotvertices(dc);
        plotallfunctions(dc);
    }
    */

     // this frees up "paper" so that it can write itself to a file.
    dc.SelectObject( wxNullBitmap );
    dc1.DrawBitmap(paper,0,0);
    cpaper = nullptr;
    //delete paper;


    ispainted=1;
    //computemutex.unlock();
    iscomputing=0;
}


void PlotFunctionFrame::plottomemoryDc(wxMemoryDC &memDC)
{
    memDC.SetFont(DrawPanel->GetFont());

    if (bitmapbackground) {
        wxSize newsize = DrawPanel->GetSize();
        if (newsize != actualPanelsize) {
            actualPanelsize = newsize;
            wxImage image = BackgroundImage.Scale(newsize.x,newsize.y);
            actualBitmapBackground = wxBitmap(image);
        }
        memDC.DrawBitmap(actualBitmapBackground,wxPoint(0,0));
    }
    else {
        memDC.SetBackground(wxBrush(BackgroundColor));
        memDC.Clear();
    }

    fillfunctions=0;
    for (int i=0;i<N;++i) {
            if (F[i].IsFill() && f_menu[i]->IsChecked()) ++fillfunctions;
    }

    plotvertices(memDC);
    //wxGCDC gdc(memDC);
    plotallfunctions(memDC);
    //plotallfunctions(gdc);
    if (fillfunctions && gridactiv->IsCheck()) {
        fillfunctions=0;
        plotvertices(memDC);
        plotallfunctions(memDC);
    }
}


void PlotFunctionFrame::OnDrawPanelPaint(wxPaintEvent &event)
{
    if (!ispainted || iscomputing) return;
    npainted++;
#ifndef _WIN32
    iscomputing=1;
    Paint();
#endif // _WIN32
}


void PlotFunctionFrame::OnDrawPanelResize(wxSizeEvent &event)
{
    if (!ispainted || iscomputing) return;
#ifdef _WIN32
    iscomputing=1;
    Paint();
#endif // _WIN32
}


void PlotFunctionFrame::Compute(int i, int comppoints)
{
    iscomputing=1;
    std::thread t(computepoints,std::cref(F),std::ref(farray),std::ref(curvearray),points,std::cref(x1),
                  std::cref(x2),std::ref(ymax),std::ref(ymin),i,comppoints);
    t.detach();
}


void PlotFunctionFrame::OnAllSettingsSelected(wxCommandEvent& event)
{
    int i,n;
    std::string input,output,pstring="";

    input+=xstring;
    input+="\n" + ystring;
    input+="\n" + val::ToString(points);

    input+="\n" + fstring;

    val::MultiLineDialog dialog(this,input,"x values / y values / number of values / functions:",260,100,"Change Settings",fontsize);
#ifdef __APPLE__
    dialog.Centre();
#endif // __APPLE__
    if (dialog.ShowModal()==wxID_CANCEL) return;
    // else OK:
    output=dialog.GetSettingsText();
    if (input==output) return;
    // else change:

    xstring = "-5;5";
    ystring = "";
    fstring = "";

    val::d_array<char> separators{'\n'};
    val::Glist<std::string> words = getwordsfromstring(output,separators,1);

    n = words.length();

    if (n>0) xstring = words[0];
    if (n>1) ystring = words[1];
    if (n>2) pstring = words[2];

    for (i = 3; i < n; ++i) fstring += words[i] + "\n";

    refreshfunctionstring();

    points = val::FromString<int>(pstring);

    if (points<500) points=500;
    else if(points>6000) points=6000;

    GetSettings();
    Compute();
}


void PlotFunctionFrame::OnMenu_xAxisSelected(wxCommandEvent& event)
{
    active_function = -1;
    iscomputing = 1;
    Paint();
}


void PlotFunctionFrame::OnMenuSaveSelected(wxCommandEvent& event)
{
    if (!ispainted) return;
#ifdef _WIN32
    wxString filetype = _("PNG files (*.png)|*.png");
#else
    wxString filetype = _("all files (*)|*");
#endif // _WIN32
    wxFileDialog FileDialogSave1(this, _("Save Graphic As"), wxEmptyString, wxEmptyString, filetype,wxFD_SAVE|wxFD_OVERWRITE_PROMPT,wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
    FileDialogSave1.SetDirectory(filedir);
    if (FileDialogSave1.ShowModal()!=wxID_OK) return;

    std::string filename;

    filedir=FileDialogSave1.GetDirectory();
    filename=filedir+filesep+FileDialogSave1.GetFilename();

    /*
    int n = filename.length();
    if (n<4) {
        filename += ".png";
    }
    else if (filename[n-1] != 'g' || filename[n-2] != 'n' || filename[n-3] != 'p' || filename[n-4] != '.' ) filename += ".png";
    */


    wxSize size;
    if (defaultsize) size=DrawPanel->GetSize();
    else size = bitmapsize;
    wxBitmap paper(size);

    cpaper = &paper;

    // Create a memory Device Context
    wxMemoryDC memDC;
    // Tell memDC to write on “paper”.
    memDC.SelectObject( paper );
    //memDC.SetBackground(wxBrush(BackgroundColor));
    //memDC.Clear();
    //memDC.SetFont(DrawPanel->GetFont());

    plottomemoryDc(memDC);
    /*
    plotvertices(memDC);
    for (int i=0;i<N;++i) plotfunction(memDC,farray[i],i);

    fillfunctions=0;
    for (int i=0;i<N;++i) {
        if (F[i].IsFill() && f_menu[i]->IsChecked()) ++fillfunctions;
    }

    plotvertices(memDC);
    plotallfunctions(memDC);
    if (fillfunctions && gridactiv->IsCheck()) {
        fillfunctions=0;
        plotvertices(memDC);
        plotallfunctions(memDC);
    }
    */



    // Tell memDC to write on a fake bitmap;
    // this frees up "paper" so that it can write itself to a file.
    memDC.SelectObject( wxNullBitmap );

    // Put the contents of "paper” into a png and into a jpeg file.
    paper.SaveFile(filename, wxBITMAP_TYPE_PNG,(wxPalette*)NULL );
    cpaper = nullptr;
    //paper->SaveFile( _T("RedSquare.jpg"), wxBITMAP_TYPE_JPEG,(wxPalette*)NULL );
}


void PlotFunctionFrame::OnMenuSizeSelected(wxCommandEvent& event)
{

     wxSize Panelsize=DrawPanel->GetSize();
     std::string psz = val::ToString(Panelsize.x) + " , " + val::ToString(Panelsize.y) + " ) ] ";

     std::string title, stattext,hsizestring;
     int id = event.GetId();

     if (id == 22) {
        stattext = "Set Panel Size [ Current = ( " + psz;
        title = "Set Panel Size";
        hsizestring = "";
     }
     else {
        stattext = "Set Size of png-Graphic [empty string= ( " + psz;
        title = "Set Graphic Size";
        hsizestring = sizestring;
     }


     val::MultiLineDialog dialog(this,hsizestring,stattext,240,2*fontsize+6,title,fontsize,1,wxTE_RIGHT);
#ifdef __APPLE__
    dialog.Centre();
#endif // __APPLE__
     if (dialog.ShowModal()!=wxID_OK) return;
     std::string svalue = dialog.GetSettingsText();
     ChangeSettings(PANEL_SIZE,svalue,id);

     return;
}


void PlotFunctionFrame::OnMenunewfunction(wxCommandEvent &event)
{
    int id=event.GetId();

    if (id != 3005 && id != 3006) {
        if (drawpoints) changedrawpoints();
        if (drawpolygon) changedrawpolygon();
        if (drawline) changedrawline();
    }
    if (id==3000) {   // Add/removeFunction
        std::string input="",output="";
        input=fstring;
        int sx = 240, sy = 100;
#ifdef __APPLE__
        sx = 300, sy = 150;
#endif // __APPLE__

        InputFunctionDialog dialog(this,WordTree,input,"Enter or delete functions:","Add/Remove functions",wxSize(sx,sy),fontsize);
        dialog.SetCloseBrackets(closebrackets);
#ifdef __APPLE__
        dialog.Centre();
#endif // __APPLE__
        //dialog.SetSize(DialogInputSize);
        if (dialog.ShowModal()==wxID_CANCEL) return;
        closebrackets = dialog.GetCloseBrackets();
        // sonst OK:
        //output=dialog.GetSettingsText();
        //DialogInputSize = dialog.GetSize();
        output=dialog.GetValue();
        if (input==output) return;
        fstring=output;
        refreshfunctionstring();
        GetSettings();
        Compute();
    }
    else if (id==3001 || id==3002) { // Hide All / Show All
        if (N==0) return;
        int n=f_menu.length();
        if (n==0) return;
        for (int i=0;i<n;++i) {
            if (id==3001) f_menu[i]->Check(false);
            else f_menu[i]->Check(true);
        }
        Paint();
    }
    else if (id==3004) { // Delete All
        active_function = -1;
        fstring="";
        refreshfunctionstring();
        GetSettings();
        Compute();
    }
    else if (id==3005 || id==3006) { // undo/redo:
        if (id==3005) {
            a_fstring--;
            if (drawpoints) addingpoints--;
            if (drawpolygon) n_polygonpoints--;
            if (drawline) n_linepoints--;
        }
        else {
            a_fstring++;
            if (drawpoints) addingpoints++;
            if (drawpolygon) n_polygonpoints++;
            if (drawline) n_linepoints++;
        }

        if (drawpoints && addingpoints<=0) changedrawpoints();
        if (drawpolygon && n_polygonpoints<=0) changedrawpolygon();
        if (drawline && n_linepoints<=0) changedrawline();

        if (a_fstring<0) {
            a_fstring=0;
            return;
        }
        if (a_fstring>=n_fstring) {
            a_fstring--;
            return;
        }

        fstring= functionstring[a_fstring];
        GetSettings();
        Compute();
    }
    else { // Delete Last
        if (N==0) return;
        active_function = -1;
        if (N==1) {
            fstring="";
        }
        else {
            fstring="";
            for (int i=0;i<N-1;++i) {
                fstring+=F[i].getinfixnotation();
                if (x_range[i].x==x_range[i].y) fstring+=";\n";
                else if (x_range[i].x!=x1 || x_range[i].y!=x2) fstring+= "  [ "+ val::ToString(x_range[i].x) +" , " + val::ToString(x_range[i].y) + " ];\n";
                else fstring+=";\n";
            }
        }
        refreshfunctionstring();
        GetSettings();
        Compute();
    }
}


void PlotFunctionFrame::OnMenuColours(wxCommandEvent &event)
{
    int evid = event.GetId();

    if (evid==4400) {
        wxColourData data;
        data.SetColour(BackgroundColor);
        wxColourDialog dialog(this,&data);
        dialog.SetLabel(_("Choose background colour"));
#ifdef __APPLE__
        dialog.Centre();
#endif // __APPLE__
        if (dialog.ShowModal()==wxID_OK) {
            BackgroundColor = dialog.GetColourData().GetColour();
            DrawPanel->SetBackgroundColour(BackgroundColor);
            bitmapbackground = 0;
            actualBitmapBackground = wxBitmap();
            BackgroundImage.Destroy();
            Paint();
        }
        return;
    }
    if (evid==4401 || evid==4402) {
        wxColour col;
        int pen;

        if (evid==4401) { col = axis_color; pen = axis_pen;}
        else {col = grid_color; pen = grid_pen;}
        FunctionColorDialog Dialog(this,col,BackgroundColor,pen);
#ifdef __APPLE__
        Dialog.Centre();
#endif // __APPLE__
        if (Dialog.ShowModal()==wxID_OK) {
            col=Dialog.GetColor();
            pen=Dialog.GetLineWith();
        }
        if (evid==4401) {axis_color = col; axis_pen = pen;}
        else {grid_color = col; grid_pen = pen;}
        Paint();
        return;
    }
    if (evid == 4403) {
        wxColourData *cdata = new wxColourData();
        cdata->SetColour(defaultpaintcolor);
        wxColourDialog dialog(this,cdata);
#ifdef __APPLE__
        dialog.Centre();
#endif // __APPLE__
        if (dialog.ShowModal() == wxID_OK) defaultpaintcolor = dialog.GetColourData().GetColour();
        delete cdata;
        return;
    }
    if (evid==21) {
        wxFontData data;
        data.SetInitialFont(defaultFont);
        wxFontDialog dialog(this,data);
#ifdef __APPLE__
        dialog.Centre();
#endif // __APPLE__
        if (dialog.ShowModal()==wxID_OK) {
            data=dialog.GetFontData();
            defaultFont=data.GetChosenFont();
        }
        return;
    }

    int i=(evid%4000);

    if (F[i].IsText()) {
        TextFontsDialog Dialog(this,Font[i],Color[i]);
        if (Dialog.ShowModal()==wxID_OK) {
            Font[i]=Dialog.GetFont();
            Color[i]= Dialog.GetColor();
            Paint();
            WriteText();
        }
        return;
    }
    else {
        FunctionColorDialog Dialog(this,Color[i],BackgroundColor,pen[i]);
        if (Dialog.ShowModal()==wxID_OK) {
            Color[i]=Dialog.GetColor();
            pen[i]=Dialog.GetLineWith();
            Paint();
            WriteText();
        }
        return;
    }
}


void PlotFunctionFrame::OnMenuResetColours(wxCommandEvent &event)
{
    ResetColours();
    Paint();
    WriteText();
}


void PlotFunctionFrame::OnMenuFontSize(wxCommandEvent& event)
{
    int id = event.GetId();
    std::string title = "Set Font Size";
    int fs = fontsize, cmd = FONT_SIZE;

    if (id == 13) {                    // Axis Font Size else Fontsize
        title = "Set Axis Font Size";
        fs = axis_fontsize;
        cmd = AXIS_FONTSIZE;
    }

    val::MultiLineDialog fontsizedialog(this,val::ToString(fs),"Entry Size",240,2*fontsize+6,title,fontsize,1,wxTE_RIGHT);
#ifdef __APPLE__
    fontsizedialog.Centre();
#endif // __APPLE__
    if (fontsizedialog.ShowModal()==wxID_OK) {
        std::string svalue = fontsizedialog.GetSettingsText();
        ChangeSettings(cmd,svalue);
    }
}


void PlotFunctionFrame::OnMenuParameter(wxCommandEvent &event)
{
    if (N!=1) return;
    if (!F[0].iswithparameter()) return;

    int i,n,anz=0;
    std::string pars,hs,fs=F[0].getinfixnotation(),xrs="";
    val::MultiLineDialog pardialog(this,"","Entry Values separated by ;",240,2*fontsize+6,"Set Parameter Values",fontsize,1);
#ifdef __APPLE__
    pardialog.Centre();
#endif // __APPLE__

    if (pardialog.ShowModal()==wxID_CANCEL) return;
    Parameter.dellist();
    pars=pardialog.GetSettingsText();
    if (pars=="") return;
    n=pars.size();
    fstring="";
    hs="";
    if (x_range[0].x==x_range[0].y) xrs="";
    else if (x_range[0].x!=x1 || x_range[0].y!=x2) xrs=" [ " + val::ToString(x_range[0].x) + " , " + val::ToString(x_range[0].y) + " ] ";
    for (i=0;i<n;++i) {
        if (pars[i]==';') {
            anz++;
            Parameter.inserttoend(val::FromString<val::rational>(hs));
            hs="";
        }
        else hs+=pars[i];
    }
    if (hs!="") {
        anz++;
        Parameter.inserttoend(val::FromString<val::rational>(hs));
    }
    for (i=0;i<anz;++i) fstring+=fs + xrs + ";";

    GetSettings();
    Compute();
}


void PlotFunctionFrame::OnMenuTools(wxCommandEvent &event)
{
    using namespace val;
    int i,naktiv=0,j=0,id=event.GetId();

    if (drawpoints) changedrawpoints();
    if (drawpolygon) changedrawpolygon();
    if (drawline) changedrawline();

    if (id==7006) {  //interpolation
        MultiLineDialog dialog(this,"","Enter Points:",240,80,"Interpolation",fontsize);
#ifdef __APPLE__
        dialog.Centre();
#endif // __APPLE__
        if (dialog.ShowModal()==wxID_CANCEL) return;
        ExecuteCommand(INTERPOLATION,-1,dialog.GetSettingsText());
        return;
    }

    val::Glist<std::string>  List;
    val::Glist<int> indezes;

    if (id==7009) { //Rotate..
        val::d_array<int> ind;
        std::string s;

        for (i=0;i<f_menu.length();++i) {
            if (f_menu[i]->IsChecked() && F[i].numberofvariables()<=1 && (F[i].getmode()==myfunction::TRIANGLE || F[i].getmode()==myfunction::LINE
                        || F[i].getmode()==myfunction::POLYGON || F[i].getmode()==myfunction::POINTS)) {
                j=i; ++naktiv;
                s = F[i].getinfixnotation();
                if (s.length() >= 50) s.resize(47);
                s += "...";
                List.push_back(std::move(s));
                indezes.push_back(i);
            }
        }
        if (naktiv==0) return;
        if (naktiv>1) {
            //val::SingleChoiceDialog dialog(this,"Available functions:","Rotate...",List);
            val::MultiChoiceDialog dialog(this,"Available functions:","Rotate...",List);
#ifdef __APPLE__
            dialog.Centre();
#endif // __APPLE__
            if (dialog.ShowModal()==wxID_OK) {
                //j = indezes[dialog.GetSelection()];
                val::d_array<int> sel = dialog.GetSelections();
                if (sel.isempty()) return;
                for (int k : sel) ind.push_back(indezes[k]);
            }
            else return;
        }
        else ind.push_back(j);
        MultiLineDialog dialog(this,"","Enter alpha; x0;y0",240,80,"Rotate...",fontsize);
#ifdef __APPLE__
        dialog.Centre();
#endif // __APPLE__
        if (dialog.ShowModal()==wxID_CANCEL) return;

        val::d_array<myfunction*> H;
        for (int i : ind) H.push_back(&F[i]);
        //std::thread t(computerotation,std::cref(F[j]),dialog.GetSettingsText());
        std::thread t(computerotation,H,dialog.GetSettingsText());
        t.detach();
        //computerotation(H,dialog.GetSettingsText());
        //GetSettings();
        //Compute();
        return;
    }

    if (id==7010) {  // Regression:
        std::string s;
        for (i=0;i<f_menu.length();++i) {
            if (f_menu[i]->IsChecked() && (F[i].IsPoints() || F[i].IsPolygon())) {
                j=i; ++naktiv;
                s = F[i].getinfixnotation();
                if (s.length()>=50) {
                    s.resize(47);
                    s+="...";
                }
                List.push_back(s);
                indezes.push_back(i);
            }
        }
        if (naktiv==0) return;
        if (naktiv>1) {
            val::SingleChoiceDialog dialog(this,"Available functions:","Regression",List);
#ifdef __APPLE__
            dialog.Centre();
#endif // __APPLE__
            if (dialog.ShowModal()==wxID_OK) {
                j = indezes[dialog.GetSelection()];
            }
            else return;
        }
        //std::thread t(computeregression,std::cref(F[j]),regressiondegree);
        //t.detach();
        ExecuteCommand(REGRESSION,j);
        return;
    }

    for (i=0;i<f_menu.length();++i) {
        if (f_menu[i]->IsChecked() && F[i].getmode()==myfunction::FUNCTION) {
            if (id!=7001 && id!= 7007 && F[i].numberofvariables()>1) continue;
            j=i; ++naktiv; List.push_back(F[i].getinfixnotation());indezes.push_back(i);
        }
    }

    if (naktiv==0) return;

    if (id==7011) { // Analyze Function
        std::string Entry = val::ToString(x1) + " ; " + val::ToString(x2) + "\n";
        wxSize sz=DrawPanel->GetSize();
        int sizex=sz.x -2*abst,j;

        Entry += "1e-9\n" + val::ToString(sizex) + "\n" + "4";

        ListDialog ldialog(this,List,"Analyze Function",Entry,240,100,fontsize);
#ifndef __LINUX__
        ldialog.Centre();
        ldialog.SetSelection(0);
#endif // __LINUX___
        if (ldialog.ShowModal()==wxID_CANCEL) return;
        j=ldialog.GetSelection();
        if (j<0) j=0;
        j = indezes[j];
        Entry = ldialog.GetText();
        ExecuteCommand(ANALYZE,j,Entry);
        return;
    }
    if (id == 7012) { // Intersection
        if (List.length() < 2) return;
        std::string Entry = val::ToString(x1) + " ; " + val::ToString(x2) + "\n";
        wxSize sz=DrawPanel->GetSize();
        int sizex=sz.x -2*abst;

        Entry += "1e-9\n" + val::ToString(sizex) + "\n" + "4";
        ListDialog ldialog(this,List,"Intersection",Entry,240,100,fontsize,wxLB_MULTIPLE,2);
        ldialog.SetSelections(val::d_array<int>({0,1}));
        if (ldialog.ShowModal() == wxID_CANCEL) return;
        val::d_array<int> selections = ldialog.GetSelections();
        if (selections.length() != 2) return;
        Entry = "#" + val::ToString(indezes[selections[1]] + 1) + " ";
        Entry += ldialog.GetText();
        ExecuteCommand(INTERSECTION,indezes[selections[0]],Entry);
        return;
    }

    if (naktiv > 1) {
        std::string title;
        switch (id)
        {
            case 7001 : title = "Tangent..." ; break;
            case 7002 : title = "Derivate..." ; break;
            case 7003 : title = "Table..." ; break;
            case 7004 : title = "Integrate..." ; break;
            case 7005 : title = "Arc lenght..." ; break;
            case 7007 : title = "Normal..." ; break;
            case 7008 : title = "Zero iteration..." ; break;
            default: break;
        }
        val::SingleChoiceDialog dialog(this,"Available functions:",title,List);
#ifdef __APPLE__
        dialog.Centre();
#endif // __APPLE__
        if (dialog.ShowModal()==wxID_OK) {
            j = indezes[dialog.GetSelection()];
        }
        else return;
    }

    //if (F[j].numberofvariables()>1 || F[j].getmode()!=myfunction::FUNCTION) return;
    //if (isderived(F[j])) return;

    //int n=fstring.size()-1;
    if (id==7001 || id ==7007) { // Tangente, Normale
        //int tangent=1;
        std::string type,input;
        if (id==7001) type = "tangent";
        else {
            type = "normal";
            //tangent=0;
        }
        MultiLineDialog tangentdialog(this,"","Entry x-value or point",240,2*fontsize+6,"Set Point for " + type,fontsize,1);
#ifdef __APPLE__
        tangentdialog.Centre();
#endif // __APPLE__
        if (tangentdialog.ShowModal()==wxID_CANCEL) return;
        input=tangentdialog.GetSettingsText();
        //input+=" ";
        ExecuteCommand(TANGENT,j,input,id);
        return;
    }
    else if (id==7002) { // Ableitung
        ExecuteCommand(DERIVE,j);
        return;
    }
    else if (id==7003) { // Table
        if (nchildwindows) return;

        MultiLineDialog tabledialog(this,xstring + " ; 0.5" ,"Entry x1,x2,dx:",240,2*fontsize+6,"Set Values for Table",fontsize,1);
#ifdef __APPLE__
        tabledialog.Centre();
#endif // __APPLE__
        if (tabledialog.ShowModal()==wxID_CANCEL) return;
        ExecuteCommand(TABLE,j,tabledialog.GetSettingsText());
        return;
    }
    else if (id==7004 || id==7005 || id==7008)    { // Integral + Iteration:
        if (id==7005 && isderived(F[j])) return;

        std::string title,param;
        wxPoint Point  = this->GetPosition();

        if (id==7004) title ="Integral";
        else if (id==7005) {title = "Arc Length";}
        else title="zero-iteration";

        param="Entry round-dec / iteration / precision / x1;x2";

        std::string text=val::ToString(dez) + "\n" + val::ToString(iter) + "\n" + val::ToString(delta) + "\n";

        MultiLineDialog integraldialog(this,text,param,240,100,title,fontsize);
#ifdef __APPLE__
        integraldialog.Centre();
#endif // __APPLE__

        integraldialog.SetPosition(wxPoint(Point.x,Point.y+10));

        if (integraldialog.ShowModal()==wxID_CANCEL) return;
        ExecuteCommand(INTEGRAL,j,integraldialog.GetSettingsText(),id);
        return;
    }
    GetSettings();
    Compute();
}


void PlotFunctionFrame::OnChangeParmeterMenu(wxCommandEvent & event)
{
    int id=event.GetId();

    if (id==23) {// Set Regression Degree:
        val::MultiLineDialog dialog(this,val::ToString(regressiondegree),"Entry Regression Degree:",240,2*fontsize+6,"Set Regression Degree",fontsize,1,wxTE_RIGHT);
#ifdef __APPLE__
        dialog.Centre();
#endif // __APPLE__
        if (dialog.ShowModal()==wxID_CANCEL) return;
        std::string svalue = dialog.GetSettingsText();
        ChangeSettings(REGRESSION_DEGREE,svalue);
        return;
    }

    if (id==24) { // Round-decimals for points
        val::MultiLineDialog dialog(this,val::ToString(rounddrawingpoints),"Entry round-decimals: (-1 = no rounding)",240,2*fontsize+6,"Set Round-decimals",fontsize,1,wxTE_RIGHT);
#ifdef __APPLE__
        dialog.Centre();
#endif // __APPLE__
        if (dialog.ShowModal()==wxID_CANCEL) return;
        std::string svalue = dialog.GetSettingsText();
        ChangeSettings(POINT_DECIMALS,svalue);
        return;
    }

    std::string pars,hs;
    int i,n=Parameter.length();

    if (Parameter.isempty()) pars="1";
    else {
        for (i=0;i<n;++i) pars+=val::ToString(Parameter[i]) + ";";
    }

    val::MultiLineDialog pardialog(this,pars,"Entry Values separated by ;",240,2*fontsize+6,"Set Parameter Values",fontsize,1);
#ifdef __APPLE__
    pardialog.Centre();
#endif // __APPLE__
    if (pardialog.ShowModal()==wxID_CANCEL) return;
    //Parameter.dellist();

    pars=pardialog.GetSettingsText();
    if (pars == "") pars = "1";
    ChangeSettings(PARAMETER_VALUES,pars);
}


void PlotFunctionFrame::OnRangeMenu(wxCommandEvent &event)
{
    int id=event.GetId();
    //std::string entry,info,title,svalue;
    std::string entry,info,title,svalue;

    if (id==6) {
        entry=xstring;info="Entry x-Values(empty string = default";title="Set x-Range Values";
    }
    else {
        entry=ystring;info="Entry y-Values(empty string = default";title="Set y-Range Values";
    }
    val::MultiLineDialog rangedialog(this,entry,info,240,2*fontsize+6,title,fontsize,1,wxTE_RIGHT);
#ifdef __APPLE__
    rangedialog.Centre();
#endif // __APPLE__
    if (rangedialog.ShowModal()==wxID_CANCEL) return;
    svalue = rangedialog.GetSettingsText();

    ChangeSettings(AXIS_RANGE,svalue,id);

    //svalue=rangedialog.GetSettingsText();
}



void PlotFunctionFrame::OnScaleMenu(wxCommandEvent &event)
{
    std::string svalue = val::ToString(x_scale) +";\n" + val::ToString(y_scale) + ";\n" + val::ToString(abst) +
                        ";\n" + x_axis + "\n" + y_axis;
    val::MultiLineDialog scaledialog(this,svalue,"Entry x-scale-, y-scale-units, margin",240,100,"Scale-Settings",fontsize);
#ifdef __APPLE__
    scaledialog.Centre();
#endif // __APPLE__
    if (scaledialog.ShowModal()==wxID_CANCEL) return;
    svalue=scaledialog.GetSettingsText();
    if (svalue=="") return;
    ChangeSettings(AXIS_SCALE,svalue);

}


void PlotFunctionFrame::OnGridMenu(wxCommandEvent &event)
{
    std::string svalue = sgx_scale +";" + sgy_scale;
    val::MultiLineDialog griddialog(this,svalue,"Entry x-Grid-scale-, y-Grid-scale-units",240,2*fontsize+6,"Grid-Settings",fontsize,1);
#ifdef __APPLE__
    griddialog.Centre();
#endif // __APPLE__
    if (griddialog.ShowModal()==wxID_CANCEL) return;
    svalue=griddialog.GetSettingsText();
    if (svalue=="") return;

    ChangeSettings(GRID_SCALE,svalue);
}



void PlotFunctionFrame::OnInputDialog(wxCommandEvent&)
{
    if (drawpoints) changedrawpoints();
    if (drawpolygon) changedrawpolygon();
    if (drawline) changedrawline();

    wxSize size = GetSize();

    wxPoint point = GetPosition();
    point.y += 60;
    point.x += 10;
    size.y = 30;
    size.x -= 20;

#ifdef __APPLE__
    point.y -= 30;
#endif // __APPLE__


    if (SideText_isshown) {
        size.x -= (widthSideText + 10);
        point.x += widthSideText +10;
    }
    InputDialog input(this,1,InputDialogTree,"",size,point,fontsize);
    input.SetComLists(CommandsList,CommandsParList);
    input.SetParLists(SettingsList,SettingsParList);
    input.SetHistory(recentcommands);
#ifdef __APPLE__
    StatusBar1->SetStatusText(_T("Type Cmd-H for help"),1);
#else
    StatusBar1->SetStatusText(_T("Type Ctrl-H for help"),1);
#endif // __APPLE__
    if (input.ShowModal() == wxID_CANCEL) {
        StatusBar1->SetStatusText(_T(""),1);
        return;
    }
    StatusBar1->SetStatusText(_T(""),1);

    val::d_array<char> separators({' ', '\n'});
    std::string svalue(input.GetTextValue()), command;
    int n = svalue.length();

    if (svalue[n-1] == '\n') {
        svalue.resize(n-1);
        --n;
    }


    if (!n) return;

    {
        int l = recentcommands.length();
        if (l && (recentcommands[l-1] != svalue)) recentcommands.push_back(svalue);
        else recentcommands.push_back(svalue);
        if (l >= 100) recentcommands.skiphead();
    }


    command = val::getfirstwordofstring(svalue,separators);

    int command_number = -1, id = 1;

    // Extract first word from svalue:
    svalue = val::tailofstring(svalue,svalue.length()-command.length() -1);

    // Extract function number from svalue
    if (svalue.length() > 1 && svalue[0] == '#') {
        std::string s_id = val::getfirstwordofstring(svalue,separators);
        svalue = val::tailofstring(svalue,svalue.length()-s_id.length() -1);
        s_id = val::tailofstring(s_id,s_id.length() - 1);
        id = val::FromString<int>(s_id);
    }

    // Check if command is settingsList:
    for (int i = 0; i < SettingsList.length(); ++i) {
        if (command == SettingsList[i]) {
            command_number = i;
            break;
        }
    }
    if (command_number == AXIS_RANGE) id = 6;
    else if (command_number == PANEL_SIZE) id = 22;
    if (command_number != -1) {
        //wxMessageBox(val::ToString(command_number));
        ChangeSettings(command_number,svalue,id);
        return;
    }

    int f_nr = id -1;

    command_number = -1;
    for (int i = 0; i < CommandsList.length(); ++i) {
        if (command == CommandsList[i]) {
            command_number = i;
            break;
        }
    }

    if (command_number != -1) {
        //wxMessageBox(val::ToString(f_nr));
        ExecuteCommand(command_number,f_nr,svalue,id);
        return;
    }

}



void PlotFunctionFrame::ChangeSettings(int command, const std::string &svalue, int id)
{
    switch (command)
    {
    case PANEL_SIZE:
        {
             val::d_array<char> separators{' ', ',', ';', ':'};
             val::Glist<std::string> words = getwordsfromstring(svalue,separators);

             if (words.isempty()) {
                if ( id != 22) {
                    defaultsize=1;
                    sizestring = "";
                }
                return;
             }

             int y, x = val::FromString<int>(words[0]);
             if (words.length()>1) y = val::FromString<int>(words[1]);
             else y = x;

             if (id == 22) {
                x +=  20; y+= 20;
                if (x < 50) x = 50;
                if (y < 50) y = 50;
                if (SideText_isshown) x += widthSideText + 10;
                SetClientSize(x,y);
                Update();
             }
             else {
                if (x < 50) x = 50;
                if (y < 50) y = 50;
                defaultsize=0;
                bitmapsize=wxSize(x,y);
                sizestring=val::ToString(x) + "," + val::ToString(y);
             }
        }
        break;
    case FONT_SIZE: case AXIS_FONTSIZE:
        {
            if (svalue == "") return;
            int fsize = val::FromString<int>(svalue);
            if (command == FONT_SIZE) {
                fontsize = fsize;
                if (fontsize<10) fontsize=10;
                if (fontsize>16) fontsize=16;
                WriteText();
            }
            else {
                axis_fontsize = fsize;
                if (axis_fontsize < 2) axis_fontsize = 2;
                if (axis_fontsize > 30) axis_fontsize = 30;
                Paint();
            }
        }
        break;
    case REGRESSION_DEGREE:
        {
            regressiondegree = val::FromString<int>(svalue);
            if (regressiondegree<1) regressiondegree = 1;
            if (regressiondegree>4) regressiondegree = 4;
        }
        break;
    case POINT_DECIMALS:
        {
            rounddrawingpoints=val::FromString<int>(svalue);
            if (rounddrawingpoints<-2) rounddrawingpoints=-2;
            if (rounddrawingpoints>9) rounddrawingpoints=9;
        }
        break;
    case PARAMETER_VALUES:
        {
            val::d_array<char> sep({';', ' '});
            val::Glist<std::string> s_values = getwordsfromstring(svalue,sep);

            Parameter.dellist();

            for (const auto& v : s_values) Parameter.push_back(val::FromString<val::rational>(v));

            if (Parameter.isempty()) Parameter.push_back(val::rational(1));

            GetSettings();
            Compute();
        }
        break;
    case AXIS_RANGE:
        {
            val::d_array<char> separators{':'};
            val::Glist<std::string> s_values=getwordsfromstring(svalue,separators,1);
            val::Glist<double> values;
            double d1 = 0 , d2 = 0;

            separators[0] = ';';
            separators.push_back(' ');
            if (!s_values.isempty())  {
                values = getdoublevaluesfromstring(s_values[0],separators,0);
                if (values.length() >= 2) {
                    d1 = values[0]; d2 = values[1];
                }
                else if (values.length() >= 1) {
                    d2 = val::abs(values[0]);
                    d1 = -d2;
                }
            }

            if (d1 >= d2) {
                d1 = -5;
                d2 = 5;
            }

            if (s_values.length()<=1) {
                if (s_values.isempty() || s_values[0] == "") {
                    if (id == 6) {
                        x1  = -5; x2 = 5;
                        xstring = "-5;5";
                    }
                    else {
                        ystring ="";
                        yset = 0;
                    }
                    Compute();
                    return;
                }
                else if (id == 6) {
                    x1 = d1; x2 = d2;
                }
                else {
                    y1 = d1; y2 = d2;
                    yset = 1;
                }
            }
            else {
                x1 = d1; x2 = d2;
                if (s_values[1] == "") {
                    yset = 0;
                    xstring = val::ToString(x1) + ";" + val::ToString(x2);
                    Compute();
                    return;
                }
                values = getdoublevaluesfromstring(s_values[1],separators,0);
                if (values.length() >= 2) {
                    d1 = values[0]; d2 = values[1];
                }
                else if (values.length() >= 1) {
                    d2 = val::abs(values[0]);
                    d1 = -d2;
                }

                if (d1 >= d2) {
                    d1 = -5;
                    d2 = 5;
                }
                y1 = d1; y2 = d2;
                yset = 1;
            }
            xstring = val::ToString(x1) + ";" + val::ToString(x2);
            ystring = val::ToString(y1) + ";" + val::ToString(y2);
            Compute();

        }
        break;
    case AXIS_SCALE:
        {
            int n;

            val::d_array<char> separators{' ', ';', '\n'};
            val::Glist<std::string> words = getwordsfromstring(svalue,separators);

            n = words.length();
            //x_axis=y_axis="";

            if (n>0) {
                sx_scale = words[0];
                if (!(pi_scale_x = getpiscale(words[0],pi_factor_x,x_scale))) {
                    x_scale=val::FromString<double>(words[0]);
                    if (x_scale<=0.0) x_scale=1.0;
                }
            }
            if (n>1) {
                sy_scale = words[1];
                if (!(pi_scale_y = getpiscale(words[1],pi_factor_x,y_scale))) {
                    y_scale=val::FromString<double>(words[1]);
                    if (y_scale<=0.0) y_scale=1.0;
                }
            }
            else {
                y_scale = x_scale;
            }
            if (n>2) {
                abst = val::FromString<int>(words[2]);
                if (abst<10) abst=10;
                if (abst>60) abst=60;
            }
            if (n>3) x_axis = words[3];
            if (n>4) y_axis = words[4];

            Paint();
        }
        break;
    case GRID_SCALE:
        {
            int i,n=svalue.size();
            std::string swert;
            for (i=0;i<n;++i) {
                if (svalue[i]==';' || svalue[i] == ' ' || i==n-1) {
                    if (i==n-1) swert+=svalue[i];
                    sgx_scale=swert;
                    if (!getpiscale(swert,g_pi_factor_x,gx_scale)) {
                        gx_scale=val::FromString<double>(swert);
                        if (gx_scale<=0.0) gx_scale=1.0;
                    }
                    ++i;
                    break;
                }
                else swert+=svalue[i];
            }
            swert="";
            if (i>=n) {
                gy_scale=gx_scale;
                sgy_scale=sgx_scale;
                Paint();
                return;
            }

            for (;i<n;++i)
                if (svalue[i]=='\n') continue;
                else swert+=svalue[i];
            sgy_scale=swert;
            if (!getpiscale(swert,g_pi_factor_y,gy_scale)) {
                gy_scale=val::FromString<double>(swert);
                if (gy_scale<=0) gy_scale=1.0;
            }
            Paint();
        }
        break;
    case RESET_COLORS:
        {
            ResetColours();
            Paint();
            WriteText();
        }
        break;
    case AXIS_COLOR: case GRID_COLOR: case FUNCTION_COLOR: case BACKGROND_COLOR:
        {
            if (svalue.length() == 0) return;
            val::d_array<char> separators({' ', ',' , ';'});
            val::Glist<std::string> s_values = getwordsfromstring(svalue,separators);
            if (s_values.isempty()) return;


            wxColour color;
            if (s_values.length() == 1) {
                int colorindex = -1;
                for (int i = 0; i < defaultcolornames.length(); ++i) {
                    if (s_values[0] == defaultcolornames[i]) {
                        colorindex = i;
                        break;
                    }
                }
                if (colorindex == -1) return;
                else color = defaultcolors[colorindex];
            }
            else if (s_values.length() == 3) {
                int r = val::FromString<int>(s_values[0]), g = val::FromString<int>(s_values[1]), b = val::FromString<int>(s_values[2]);
                color = wxColour(r,g,b);
            }
            else return;
            if (command == AXIS_COLOR) axis_color = color;
            else if (command == GRID_COLOR) grid_color = color;
            else if (command == FUNCTION_COLOR) {
                if (id > N) return;
                Color[id-1] = color;
            }
            else {
                BackgroundColor = color;
                DrawPanel->SetBackgroundColour(BackgroundColor);
                bitmapbackground = 0;
                actualBitmapBackground = wxBitmap();
                BackgroundImage.Destroy();

            }
            Paint();
            WriteText();
        }
        break;
    case SHOW_X_AXIS: case SHOW_Y_AXIS: case SHOW_GRID: case SHOW_X_SCALE: case SHOW_Y_SCALE: case SHOW_FUNCTION:
        {
            bool show;
            wxMenuItem *menu = nullptr;

            if (val::FromString<int>(svalue) != 0) show = true;
            else show = false;
            switch (command)
            {
            case SHOW_X_AXIS: menu = Menu_xAxis; break;
            case SHOW_Y_AXIS: menu = Menu_yAxis; break;
            case SHOW_GRID: menu = gridactiv; break;
            case SHOW_X_SCALE: menu = x_scaleactiv; break;
            case SHOW_Y_SCALE: menu = y_scaleactiv; break;
            case SHOW_FUNCTION:
                {
                    if (id > N) return;
                    menu = f_menu[id-1];
                }
                break;
            default:
                break;
            }
            menu->Check(show);
        }
        break;
    case VALUES_NUMBER:
        {
            points = val::FromString<int>(svalue);
            if (points < 500) points = 500;
            if (points > 6000) points = 6000;
            Compute();
        }
        break;
    case FUNCTION_SIZE:
        {
            if (id > N) return;
            pen[id-1] = val::FromString<int>(svalue);
            if (pen[id-1] <= 0) pen[id-1] = 1;
            Paint();
        }
        break;
    case AXIS_NAMES:
        {
            val::d_array<char> separators({' ', ';', ','});
            val::Glist<std::string> s_values = getwordsfromstring(svalue,separators);
            if (s_values.length() > 0) x_axis = s_values[0];
            if (s_values.length() > 1) y_axis = s_values[1];
            Paint();
        }
        break;
    case MARGIN:
        {
            abst = val::FromString<int>(svalue);
            if (abst < 10) abst = 10;
            if (abst > 60) abst = 60;
            Paint();
        }
        break;
    default:
        break;
    }
}


void PlotFunctionFrame::ExecuteCommand(int command, int f_nr, const std::string &svalue, int id)
{
    if ((command != REGRESSION && command != INTERPOLATION)  && (f_nr < 0 || f_nr >= N)) return;
    switch (command)
    {
    case DERIVE:
        {
            std::string sf=F[f_nr].getinfixnotation();
            int n = fstring.length()-1, m = sf.size()-1;
            if (m<0) return;

            if (fstring[n]!=';') fstring+=';';
            if (sf[m]=='\'') fstring += sf + "\'";
            else if (F[f_nr].numberofvariables()==1 && F[f_nr].isdifferentiable()) {
                val::valfunction f(F[f_nr].getinfixnotation()), g = f.derive();
                g.simplify(1);
                fstring += g.getinfixnotation();
            }
            else fstring += "(" + sf + ")" + "\'";
            if (F[f_nr].iswithparameter()) {
                Parameter.inserttoend(val::rational(F[f_nr].getparameter()));
            }
            if (x_range[f_nr].x==x_range[f_nr].y) fstring+=";";
            else if (x_range[f_nr].x!=x1 || x_range[f_nr].y!=x2)
                fstring+="  [ "+val::ToString(x_range[f_nr].x) + " , " + val::ToString(x_range[f_nr].y) + " ]";
            refreshfunctionstring();
            }
        break;
    case ANALYZE: case INTERSECTION:
        {
            if (F[f_nr].getmode() != myfunction::FUNCTION || F[f_nr].numberofvariables() > 1) return;
            val::d_array<char> separators{' ', ';', '\n'};
            val::Glist<std::string> s_values = getwordsfromstring(svalue,separators);
            int n = s_values.length(), nr2 = 1;
            std:: string nvalue = "";

            if (command == INTERSECTION) {
                if (n && s_values[0][0] == '#') {
                    nvalue = s_values[0];
                    s_values.skiphead();
                    nvalue = val::tailofstring(nvalue, nvalue.length()-1);
                    nr2 = val::FromString<int>(nvalue) - 1;
                }
                if (nr2 == f_nr || nr2 < 0 || nr2 >= N) return;
                if (F[nr2].getmode() != myfunction::FUNCTION || F[nr2].numberofvariables() > 1) return;
                n = s_values.length();
                nvalue = "";
            }
            if (n < 1) nvalue += val::ToString(x1);
            else nvalue = s_values[0];
            if (n < 2) nvalue += ";" + val::ToString(x2);
            else nvalue += ";" + s_values[1];
            if (n < 3) nvalue += "\n1e-9";
            else nvalue += "\n" + s_values[2];
            if (n < 4) nvalue += "\n" + val::ToString(sizex);
            else nvalue += "\n" + s_values[3];
            if (n < 5) nvalue += "\n4";
            else nvalue += "\n" + s_values[4];

            if (command == ANALYZE) {
                std::thread t(analyzefunction,std::cref(F[f_nr]),nvalue);
                t.detach();
            }
            else {
                std::thread t(intersection,std::cref(F[f_nr]),std::cref(F[nr2]),nvalue);
                t.detach();
            }
            return;
        }
        break;
    case TANGENT: case NORMAL:
        {
            int tangent = 1;

            if (command == NORMAL || id == 7007) tangent = 0;
            std::thread t(computetangent,svalue,std::cref(F[f_nr]),x1,x2,tangent);
            t.detach();
            return;
        }
        break;
    case INTERPOLATION:
        {
            std::string nvalue = svalue;
            if (svalue == "") {
                if (f_nr >=0 && f_nr < N && (F[f_nr].IsPoints() || F[f_nr].IsPolygon()) ) {
                    val::d_array<double> Points = F[f_nr].getPolygonPoints();
                    for (const auto &v : Points) nvalue += val::ToString(v) + " ";
                }
                else return;
            }
            std::thread t(computeinterpolation,nvalue,std::ref(fstring));
            t.detach();
            return;
        }
        break;
    case REGRESSION:
        {
            val::d_array<char> sep{' ', ';'};
            val::Glist<std::string> s_values = getwordsfromstring(svalue,sep);
            std::string s = "";
            int n = s_values.length() , deg = regressiondegree;

            if (n%2) {
                deg = val::FromString<int>(s_values[n-1]);
                deg = val::Max(1,deg);
                deg = val::Min(10,deg);
                for (int i = 0; i < n-1; ++i) s += s_values[i] + " ";
            }
            else s = svalue;


            if (s != "") {
                global_function = myfunction("points " + s);
                std::thread t(computeregression,std::cref(global_function),deg);
                t.detach();
                return;
            }
            else {
                if (f_nr < 0 || f_nr >= N) return;
                std::thread t(computeregression,std::cref(F[f_nr]),deg);
                t.detach();
                return;
            }
        }
        break;
    case TABLE:
        {
            if (nchildwindows) return;

            val::rational x_1(x1),x_2(x2),d_x(0.5);
            int rat=0;

            //if (svalue=="") return;
            int n=svalue.size();

            if (n > 0 && svalue[n-1] == ';') rat = 1;

            val::d_array<char> separ{';', ' '};
            val::Glist<std::string> s_values = getwordsfromstring(svalue,separ);

            n = s_values.length();
            //if (n == 0) return;
            if (n > 0) x_1 = val::FromString<val::rational>(s_values[0]);
            if (n > 1) x_2 = val::FromString<val::rational>(s_values[1]);
            if (n > 2) d_x = val::FromString<val::rational>(s_values[2]);

            if (x_1>x_2) return;
            if (d_x<=val::rational(0)) return;

            //wxMessageBox(val::ToString(x1) + " ; " + val::ToString(x2) + " ; " + val::ToString(dx));
            if (!F[f_nr].israt() || !rat || isderived(F[f_nr])) {
                std::thread t(computetable,std::cref(F[f_nr]),double(x_1),double(x_2),double(d_x));
                t.detach();
                return;
            }
            else {
                std::thread t(computetable_rat,std::cref(F[f_nr]),x_1,x_2,d_x);
                t.detach();
                return;
            }

        }
        break;
    case EVALUATE:
        {
            if ((F[f_nr].getmode() != myfunction::FUNCTION) || F[f_nr].numberofvariables() > 1 || isderived(F[f_nr])) return;
            tablestring = svalue;
            double par = 1.0;

            if (!Parameter.isempty()) {
                int i = val::Min(f_nr,Parameter.length()-1);
                par = double(Parameter[i]);
            }
            std::thread t(computeevaluation,std::cref(F[f_nr]),par);
            t.detach();
            return;
        }
        break;
    case INTEGRAL: case ARCLENGTH: case ZERO_ITERATION:
        {
            int arclength = 0;

            if (id == 7005 || command == ARCLENGTH) {
                if (isderived(F[f_nr])) return;
                arclength = 1;
            }
            if (id == 7004 || id == 7005) command = INTEGRAL;
            if (id == 7008) command = ZERO_ITERATION;

            val::rational xr1,xr2;

            val::d_array<char> sep({'\n', ';', ' '});
            val::Glist<std::string> s_values = getwordsfromstring(svalue,sep);

            int n = s_values.length();

            if (n == 2 && id == 1) {
                s_values.push(val::ToString(delta));
                s_values.push(val::ToString(iter));
                s_values.push(val::ToString(dez));
                n = 5;
            }

            //if (n != 5) return;

            if (n>0) dez = val::FromString<int>(s_values[0]);
            if (n>1) iter = val::FromString<int>(s_values[1]);
            if (n>2) delta = val::FromString<double>(s_values[2]);
            if (n>3) xr1 = val::FromString<val::rational>(s_values[3]);
            if (n>4) xr2 = val::FromString<val::rational>(s_values[4]);

            if (dez<0) dez = 0;
            if (dez>10) dez=10;
            if (iter<40) iter=40;
            if (iter>500) iter=500;
            if (delta<1e-9) delta=1e-9;
            if (delta>0.1) delta=0.1;

            if (command == INTEGRAL || command == ARCLENGTH) {
                std::thread t(computeintegral,std::cref(F[f_nr]),xr1,xr2,delta,iter,dez,arclength);
                t.detach();
                return;
            }
            else {
                std::thread t(computezeroiteration,std::cref(F[f_nr]),xr1,xr2,delta,iter,dez);
                t.detach();
                return;
            }
        }
        break;
    case MOVE:
        {
            double dx = 0, dy = 0;
            val::d_array<char> separ{' ', ';' };
            val::Glist<std::string> s_values = getwordsfromstring(svalue,separ);
            int n = s_values.length();

            if (n > 0) dx = val::FromString<double>(s_values[0]);
            if (n >1 ) dy = val::FromString<double>(s_values[1]);

            displacefunction(f_nr,dx,dy);
            fstring = "";
            int i = 0;
            for (const auto & v : F) {
                if (x_range[i].x == x_range[i].y) fstring += v.getinfixnotation() + ";\n";
                else fstring += v.getinfixnotation() + "[" + val::ToString(x_range[i].x) + " , " + val::ToString(x_range[i].y) + "];\n";
                ++i;
            }
            refreshfunctionstring();
            WriteText();
            // Change menu-text
            {
                std::string tf = "", tc = "", s = F[f_nr].getinfixnotation();
                if (f_nr<=8) {
                    tf = "\tAlt-" + val::ToString(f_nr+1);
                    tc = "\tCtrl-" + val::ToString(f_nr+1);
                }
                //else t="";
                if (s.length()>40) {
                    s.resize(40);
                    s+="...";
                }
                //s+=t;
                f_menu[f_nr]->SetItemLabel(s + tf);
                c_menu[f_nr]->SetItemLabel(s + tc);
            }

            iscomputing = 1;
            Compute(f_nr);
            return;
        }
        break;
    default:
        break;
    }
    GetSettings();
    Compute();
}



void PlotFunctionFrame::OnMoveMenu(wxCommandEvent &)
{
    std::string svalue = val::ToString(movedx);
    if (moveinpointsx) svalue +=" p";
    svalue += " ; " + val::ToString(movedy);
    if (moveinpointsy) svalue += " p";
    val::MultiLineDialog movedialog(this,svalue,"Entry x-increment ; y-increment [p for pixels]",240,2*fontsize+6,"Move Increments",fontsize,1);
#ifdef __APPLE__
    movedialog.Centre();
#endif // __APPLE__
    if (movedialog.ShowModal() == wxID_CANCEL) return;
    svalue=movedialog.GetSettingsText();
    if (svalue=="") return;

    int i,n=svalue.size() , nfound = 0, m;
    std::string swert;

    moveinpointsx = moveinpointsy = 0;
    movedy = 0;
    for (i=0 ; i<n ; ++i) {
        if (svalue[i] == ';' || i == n-1) {
            if (i==n-1) {
                swert += svalue[n-1];
                //wxMessageBox(swert);
            }
            for (m=swert.length();m>0;--m) {
                if (swert[m-1] == ' ' || swert[m-1] == '\n') continue;
                else if (swert[m-1] == 'p') {
                    if (nfound == 0 ) moveinpointsx = 1;
                    else moveinpointsy = 1;
                }
                else break;
            }
            swert.resize(m);
            if (nfound == 0) movedx = val::FromString<double>(swert);
            else movedy = val::FromString<double>(swert);
            swert ="";
            nfound++;
        }
        else swert += svalue[i];
    }

    if (movedy == 0) {
        movedy = movedx;
        moveinpointsy = moveinpointsx;
    }


    if (movedx <= 0) movedx = 0.1;
    if (movedy <= 0) movedy = 0.1;
}


void PlotFunctionFrame::OnMyEvent(MyThreadEvent& event)
{
    int id = event.GetId();
    if (id == IdInfo) {
        wxMessageBox(event.GetMessage());
        return;
    }

    if (id == IdPaint) {
        if (!yset)  {y1=ymin;y2=ymax;}
        if (isInf(y1)) y1=-5;
        if (isInf(y2)) y2 = 5;

        std::string s="";

        s += "x1= " + val::ToString(val::round(x1)) + "; x2= " + val::ToString(val::round(x2));
        if (y1!=ymin) s+="; y1= " + val::ToString(val::round(y1));
        if (y2!=ymax) s+="; y2= " + val::ToString(val::round(y2));
        if (active_function == -1) StatusBar1->SetStatusText(s + "; ymin= "+val::ToString(val::round(ymin))+ "; ymax= "+val::ToString(val::round(ymax)));
        else StatusBar1->SetStatusText(F[active_function].getinfixnotation());

        //wxMessageBox("Anzahl algebraische Kurven: " + val::ToString(curvearray.length()));

        ispainted=1;
        Paint();
        return;
    }

    if (id == IdRefresh) {
        refreshfunctionstring();
        GetSettings();
        Compute();
        return;
    }

    using namespace val;
    wxPoint Point  = this->GetPosition();
    wxSize  Size= this->GetClientSize();
    wxDisplay display(unsigned(0));
    int x=Point.x,y=Point.y,dx=Size.GetWidth(), dy = Size.GetHeight(),sx,sy,maxx=display.GetGeometry().GetWidth();


    if (id == IdTable || id == IdEval) { //
        //std::string s = ToString(x) + " , " + ToString(y) + " ; " + ToString(dx) + " , " + ToString(dy);
        std::string title;

        if (id  == IdTable) title = "Table";
        else title = "Evaluation";
        sx=200;
        sy=dy;
        x+=dx;
        if (x+sx+23>maxx) {
            //wxMessageBox(val::ToString(maxx));
            x-=maxx-(x+sx+23);
        }

        Point.x = x; Point.y = y;
        Size.SetWidth(sx);
        Size.SetHeight(sy);
        //wxMessageBox(s);
        InfoWindow *tablewindow = new InfoWindow(this,nchildwindows,tablestring,Point,Size,title,fontsize);
        tablewindow->Show();
    }
    else if (id == IdAnalyze || id == IdIntersection) {
        std::string title;

        if (id == IdAnalyze) {
            title = "Analyze Function";
        }
        else title = "Intersection Points";
        sx=250;
        sy=110;

        x+=dx;
        if (x+sx+23>maxx) {
            //wxMessageBox(val::ToString(maxx));
            x-=maxx-(x+sx+23);
        }
        //x+=dx - sx;
        Point.x = x; Point.y = y;
        AnalysisDialog *adialog = new AnalysisDialog(this,nanalyzewindows,analyze_output,Points,wxSize(sx,sy),Point,fontsize,title);
        adialog->Show();
    }
    else if (id == IdIntegral) { // Integral
        y=y+dy-200;
        Point.x = x; Point.y = y;
        Size.SetWidth(300);
        Size.SetHeight(200);
        InfoWindow *tablewindow = new InfoWindow(this,nchildwindows,tablestring,wxDefaultPosition,Size,"Integral",fontsize);
        tablewindow->Show();
        //wxMessageBox("Integral");
    }
    else { // zero-iteration:
        y+=dy-200;
        Point.x = x; Point.y = y;
        Size.SetWidth(300);
        Size.SetHeight(205);
        InfoWindow *tablewindow = new InfoWindow(this,nchildwindows,tablestring,wxDefaultPosition,Size,"zero iteration",fontsize);
        tablewindow->Show();
    }
}


void PlotFunctionFrame::OnParentEvent(ParentEvent &event)
{
    const std::string &sp = event.GetPointString();
    int i,found=0;
    for (i=0;i<N;++i) {
        if (F[i].getinfixnotation()==sp) {
            found = 1;
            break;
        }
    }
    if (!found) {
        int n = fstring.length();
        if (n>0 && fstring[n-1]!=';') fstring+=";";
        if (n>0) fstring+="\n";
        fstring+=sp;
        refreshfunctionstring();
        GetSettings();
        Compute();
    }
    else {
        if (f_menu[i]->IsChecked()) f_menu[i]->Check(false);
        else f_menu[i]->Check(true);
        Paint();
    }
}


void PlotFunctionFrame::OnMenuFill(wxCommandEvent &event)
{
    int n=fstring.length()-1, id = event.GetId();
    double x,y;

    if (id == 1009) {
        if (wxTheClipboard->Open()) {
            if (wxTheClipboard->IsSupported( wxDF_BITMAP )) {
                actualPanelsize = DrawPanel->GetSize();
                wxBitmapDataObject data;
                wxTheClipboard->GetData( data );
                actualBitmapBackground = data.GetBitmap();
                BackgroundImage = actualBitmapBackground.ConvertToImage();
                wxImage image = BackgroundImage.Scale(actualPanelsize.x,actualPanelsize.y);
                actualBitmapBackground = wxBitmap(image);
                bitmapbackground = 1;
                Paint();
            }
            wxTheClipboard->Close();
        }
        return;
    }
    if (id == 1008) {
        wxBitmap *paper = new wxBitmap(DrawPanel->GetSize());

        // Create a memory Device Context
        wxMemoryDC memDC;
        // Tell memDC to write on “paper”.
        memDC.SelectObject( *paper );
        //memDC.SetBackground(wxBrush(BackgroundColor));
        //memDC.Clear();
        //memDC.SetFont(DrawPanel->GetFont());

        plottomemoryDc(memDC);
        /*
        plotvertices(memDC);
        for (int i=0;i<N;++i) plotfunction(memDC,farray[i],i);

        fillfunctions=0;
        for (int i=0;i<N;++i) {
            if (F[i].IsFill() && f_menu[i]->IsChecked()) ++fillfunctions;
        }

        plotvertices(memDC);
        plotallfunctions(memDC);
        if (fillfunctions && gridactiv->IsCheck()) {
            fillfunctions=0;
            plotvertices(memDC);
            plotallfunctions(memDC);
        }
        */

        // Tell memDC to write on a fake bitmap;
        // this frees up "paper" so that it can write itself to a file.
        memDC.SelectObject( wxNullBitmap );

        if (wxTheClipboard->Open()) {
            wxTheClipboard->SetData( new wxBitmapDataObject(*paper) );
            wxTheClipboard->Close();
        }

        delete paper;

        return;
    }

    if (id == 1007) { // right click menu draw line
        changedrawline();
        return;
    }

    if (id == 1006) { // right click menu drawpolygon
        changedrawpolygon();
        return;
    }

    if (id==1005) {
        changedrawpoints();
        return;
    }

    if (id==1003 ||  id==1004) {

        std::string entry,title,descr;

        x=(x2-x1)*double(mouse_x1-abst) / double (sizex -1) + x1;
        y=double(yzero-mouse_y1)*(y2-y1)/double(sizey -1);

        if (rounddrawingpoints>=0) {
            x=val::round(x,rounddrawingpoints); y=val::round(y,rounddrawingpoints);
        }
        else if (rounddrawingpoints==-2) {
            int fx,fy;
            double gx1,gx2,gy1,gy2;

            fx = int(x/gx_scale); fy = int(y/gy_scale);
            if (fx < 0 ) fx--;
            if (fy < 0 ) fy--;
            gx1=double(fx)*gx_scale; gy1=double(fy)*gy_scale;
            gx2=gx1+gx_scale; gy2=gy1+gy_scale;
            if (val::abs(x-gx1)<=val::abs(x-gx2)) x=gx1;
            else x=gx2;
            if (val::abs(y-gy1)<=val::abs(y-gy2)) y=gy1;
            else y=gy2;
        }

        if (id==1003) {
            entry ="circle "  + val::ToString(x) + " " + val::ToString(y) + " ";
            title = "Draw circle";
            descr = "Entry radius [start-angle end-angle]";
        }
        else {
            entry ="";
            title = "Draw Text";
            descr = "Entry Text";
        }

        val::MultiLineDialog cdialog(this,entry,descr,240,fontsize+15,title,fontsize,1);
#ifdef __APPLE__
        cdialog.Centre();
#endif // __APPLE__

        if (cdialog.ShowModal()==wxID_OK) {
            if (fstring[n]!=';') fstring+=";";
            if (id == 1003) fstring+=cdialog.GetSettingsText();
            else fstring += "text {" + cdialog.GetSettingsText() + "} " + val::ToString(x) + " " + val::ToString(y) + " ";
            refreshfunctionstring();
            GetSettings();
            Compute();
        }
        return;
    }
    if (id==1002) {
        if (N==Color.length()) {
            Color.resize(N+1);
            Color[N] = Color[0];
        }
        wxColourData ColorData;
        ColorData.SetColour(Color[N]);
        wxColourDialog dialog(this,&ColorData);
#ifdef __APPLE__
        dialog.Centre();
#endif // __APPLE__
        if (dialog.ShowModal()==wxID_OK) {
            //ColorData=dialog.GetColourData();
            Color[N] = dialog.GetColourData().GetColour();
        }
        else return;
    }

    x=(x2-x1)*double(mouse_x1-abst) / double (sizex -1) + x1;
    y=double(yzero-mouse_y1)*(y2-y1)/double(sizey -1);
    if (fstring[n]!=';')  fstring+=';';
    fstring += "fill " + val::ToString(x) + " " + val::ToString(y);
    refreshfunctionstring();
    GetSettings();
    Compute();
}


void PlotFunctionFrame::OnMouseDown(wxMouseEvent &event)
{
    if (!yset) return;

    if (DrawPanel->HasCapture()) return;

    mouse_x1=event.GetX();mouse_y1=event.GetY();
    PopupMenu(rightclickmenu);
    //mouse_x1=event.GetX();mouse_y1=event.GetY();
    //StatusBar1->SetStatusText(val::ToString(mouse_x1) + " " + val::ToString(mouse_y1));
}


void PlotFunctionFrame::OnMouseWheel(wxMouseEvent &event)
{
    if (!yset) return;

    if (event.GetWheelRotation()< 0) zoom*=0.96;
    else zoom*=1.04;


    //if (f<0) return;
    if (!iscomputing) {
        iscomputing=1;
        x1*=zoom;x2*=zoom;y1*=zoom;y2*=zoom;
        zoom=1.0;
        xstring=val::ToString(x1) + ";" + val::ToString(x2);
        ystring=val::ToString(y1) + ";" + val::ToString(y2);
        //StatusBar1->SetStatusText(val::ToString(a) + "  " + val::ToString(f));
        Compute();
        //Paint();
    }
   // }
}



void PlotFunctionFrame::refreshfunctionstring()
{
    if (a_fstring == 29) {
        for (int i=0;i<29;++i) functionstring[i]=functionstring[i+1];
        functionstring[29] = fstring;
    }
    else {
        a_fstring++;
        n_fstring=a_fstring+1;
        functionstring[a_fstring] = fstring;
    }
}


void PlotFunctionFrame::savefile(const std::string &dirname,const std::string &filename)
{
        std::ofstream file(dirname + filesep + filename,std::ios::out | std::ios::trunc);
        if (!file) {
            wxMessageBox("Cannot write to:\n" + filename);
            return;
        }
        actual_dirname=savefiledir=dirname; actual_filename=filename;
        SetLabel(Program_Name + " " +actual_filename);
        wxSize MySize = DrawPanel->GetSize();
        int i,m=0;

        file<<"plotfunctionfile"<<std::endl;
        file<<MySize.x<<std::endl;
        file<<MySize.y<<std::endl;
        file<<points<<std::endl;
        file<<BackgroundColor.GetRGB()<<std::endl;
        // Axis:
        file<<abst<<' '<<x_scale<<' '<<y_scale<<' '<<pi_scale_x<<' '<<pi_scale_y<<std::endl;
        file<<pi_factor_x<<' '<<pi_factor_y<<std::endl;
        file<<sx_scale<<std::endl; file<<sy_scale<<std::endl; file<<x_axis<<std::endl; file<<y_axis<<std::endl;
        file<<axis_pen<<' '<<axis_color.GetRGB()<<std::endl;
        // Grid:
        file<<gx_scale<<' '<<gy_scale<<' '<<g_pi_factor_x<<' '<<g_pi_factor_y<<' '<<grid_pen<<' '<<grid_color.GetRGB()<<std::endl;
        //file<<grid_pen<<' '<<grid_color.GetRGB()<<std::endl;
        file<<sgx_scale<<std::endl<<sgy_scale<<std::endl;
        // Range:
        file<<xstring<<std::endl<<ystring<<std::endl;
        // Functions:
        file<<N<<std::endl<<fstring<<std::endl;
        for (i=0;i<N;++i) {
            file<<pen[i]<<std::endl;
        }
        for (i=0;i<N;++i) {
            file<<Color[i].GetRGB()<<std::endl;
        }
        m=Font.length();
        file<<m<<std::endl;
#ifndef __LINUX__
        for (i=0;i<m;++i) {
            file<<Font[i].GetNativeFontInfoDesc()<<std::endl;
        }
#else
        for (i=0;i<m;++i) {
            file<<Font[i].GetNativeFontInfoDesc()<<std::endl;
        }
#endif // __LINUX__
        //

        if (bitmapbackground) {
            /*
            wxMemoryOutputStream mos;
            if ( BackgroundImage.SaveFile(mos, wxBITMAP_TYPE_PNG) ) {
                const wxStreamBuffer* buff = mos.GetOutputStreamBuffer();
                const wxString encoded = wxBase64Encode(buff->GetBufferStart(), buff->GetBufferSize());
               // wxLogDebug(encoded);
                file<<'1'<<std::endl;
                file<<encoded<<std::endl;
            }
            else wxMessageBox("Save Background failed!");
            */
            //
            file<<1<<std::endl;
            std::string nfilename = dirname + filesep +filename;
#ifdef _WIN32
            int n = filename.length();
            if (n<4 || filename[n-1] != 'f' || filename[n-2] != 'l' || filename[n-3] != 'p' || filename[n-4] != '.') nfilename += ".plf";
#endif // _WIN32
            nfilename += ".png";
            BackgroundImage.SaveFile(nfilename);
        }
        else file<<0<<std::endl;

        file.close();
}

void PlotFunctionFrame::openfile(const std::string &dirname,const std::string &filename)
{
    std::string name = dirname + filesep + filename,line;

    if (drawpoints) changedrawpoints();
    if (drawpolygon) changedrawpolygon();
    if (drawline) changedrawline();
    /*
    if (!val::FileExists(name)) {
        wxMessageBox("File " + name + "\ndoes not exist!");
        return;
    }
    */

    std::ifstream file(name,std::ios::in);

    if (!file) {
        wxMessageBox("Cannot open File\n " + name);
        return;
    }
    ispainted = 0;


    actual_dirname=openfiledir=dirname; actual_filename=filename;
    SetLabel(Program_Name + " " +actual_filename);

    /*
    getline(file,line);
    if (line!="plotfunctionfile") {
        wxMessageBox("Wrong Type of file!");
        return;
    }
    */
    getline(file,line);


    int xsize,ysize,iwert,i,m=0;

    file>>xsize>>ysize>>points;
    file>>iwert; BackgroundColor.SetRGB(iwert);
    //Axis:
    file>>abst>>x_scale>>y_scale>>pi_scale_x>>pi_scale_y>>pi_factor_x>>pi_factor_y;
    getline(file,sx_scale); getline(file,sy_scale); getline(file,x_axis); getline(file,y_axis);
    //wxMessageBox(sx_scale +"\n" + sy_scale + "\n" + x_axis + "\n" + y_axis);
    file>>axis_pen>>iwert; axis_color.SetRGB(iwert);
    //Grid:
    file>>gx_scale>>gy_scale>>g_pi_factor_x>>g_pi_factor_y>>grid_pen>>iwert; grid_color.SetRGB(iwert);
    //file>>grid_pen>>iwert; grid_color.SetRGB(iwert);
    getline(file,line);getline(file,sgx_scale); getline(file,sgy_scale);
    //Range:
    getline(file,xstring); getline(file,ystring);
    //wxMessageBox(xstring + " " + ystring);
    //Functions:
    file>>N;
    getline(file,line);
    fstring="";
    for (i=0;i<N;++i) {
        line="";
        getline(file,line);
        fstring+=line + ";";
    }
    pen.resize(N);
    if (N>Color.length()) Color.resize(N);
    for (i=0;i<N;++i) file>>pen[i];
    for (i=0;i<N;++i) {
        file>>iwert; Color[i].SetRGB(iwert);
    }
    file>>m;
    Font.resize(m);
    for (i=0;i<m;++i) {
        line="";
        getline(file,line);
        Font[i] = defaultFont;
        Font[i].SetNativeFontInfoUserDesc(wxString(line));
    }

    bitmapbackground = 0;

    int bm=0;
    if (file) getline(file,line);
    if (file) getline(file,line);
    bm = val::FromString<int>(line);
    if (bm == 1) {
        /*
        file.close();
        wxTextFile wxfile(name);
        if (!wxfile.Open()) {
            wxMessageBox("cannot open file");
        }
        else {
            wxString is = wxfile.GetLastLine();
            wxMessageBox(is);
            wxStringInputStream istrm(is);
            if (BackgroundImage.LoadFile(istrm)) {
                actualPanelsize = DrawPanel->GetSize();
                wxImage image = BackgroundImage.Scale(actualPanelsize.x,actualPanelsize.y);
                actualBitmapBackground = wxBitmap(image);
                bitmapbackground = 1;
            }
            else wxMessageBox("Cannot load Background!");
        }
        //
        */
        name += ".png";
        if (BackgroundImage.LoadFile(name,wxBITMAP_TYPE_PNG)) {
            //wxMessageBox("loaded: " + name);
            actualPanelsize = DrawPanel->GetSize();
            wxImage image = BackgroundImage.Scale(actualPanelsize.x,actualPanelsize.y);
            actualBitmapBackground = wxBitmap(image);
            bitmapbackground = 1;
        }
        else wxMessageBox("Cannot load Background!");
    }


    file.close();
    xsize +=  20; ysize += 20;
    if (xsize < 50) xsize = 50;
    if (ysize < 50) ysize = 50;
    if (SideText_isshown) xsize += widthSideText + 10;


#ifdef _WIN32
    // DrawPanel->Disconnect(wxEVT_SIZE,(wxObjectEventFunction)&PlotFunctionFrame::OnDrawPanelResize,0,this);
    DrawPanel->Unbind(wxEVT_SIZE,&PlotFunctionFrame::OnDrawPanelResize,this);
    SetClientSize(wxSize(xsize,ysize));
    // DrawPanel->Connect(wxEVT_SIZE,(wxObjectEventFunction)&PlotFunctionFrame::OnDrawPanelResize,0,this);
    DrawPanel->Bind(wxEVT_SIZE,&PlotFunctionFrame::OnDrawPanelResize,this);
    Update();
#else
    SetClientSize(wxSize(xsize,ysize));
#endif
    ispainted = 1;
    DrawPanel->SetBackgroundColour(BackgroundColor);
    refreshfunctionstring();
    GetSettings();
    Compute();
}


int PlotFunctionFrame::findactivefunction(int x, int y)
{
    int i, i_f=0, i_c=0, ix, endx, iy, index;
    double faktor_y = double(sizey) / double(y2-y1), faktor_x=double(points)/double(sizex), fy;

    endx = val::Min(sizex+abst,x+10);


    for (i=0; i<N; ++i) {
        if (!f_menu[i]->IsChecked()) {
            if (F[i].numberofvariables() > 1) i_c++;
            else i_f++;
            continue;
        }
        switch (F[i].getmode())
        {
        case myfunction::FUNCTION :
            {
                //wxMessageBox(val::ToString(i) + " , " + val::ToString(i_f));
                //wxMessageBox(val::ToString(ix) + ", " + val::ToString(endx));
                if (F[i].numberofvariables()>1) {
                    for (ix = val::Max(abst,x); ix < endx; ++ix) {
                        index = int (val::round(double(ix-abst)*faktor_x,0));
                        for (const auto & cy : curvearray[i_c][index]) {
                            iy = yzero - int(val::round(faktor_y*cy,0));
                            if (val::abs(y-iy) < 10) {
                                critpoints[i_c].dellist();
                                critx[i_c].del();
                                return i;
                            }
                        }
                    }
                    i_c++;
                    break;
                }
                for (ix = val::Max(abst,x); ix < endx; ++ix) {
                    index = int (val::round(double(ix-abst)*faktor_x,0));
                    fy = farray[i_f][index];
                    iy = yzero - int(val::round(faktor_y*fy,0));
                    //wxMessageBox(val::ToString(ix) + " , " + val::ToString(index) + " , " + val::ToString(fy) + " , " + val::ToString(y) + " , " + val::ToString(iy));
                    if (val::abs(y-iy) <10) {
                        //undef_intervals[i_rf].dellist();
                        return i;
                    }
                }
                //++i_rf;
                ++i_f;
            }
            break;
        case myfunction::TEXT :
            {
                ix = val::Max(abst,x);
                int ix0=abst+int(double(sizex)*((farray[i_f][0]-x1)/(x2-x1)));
                iy=yzero -int((double(sizey)/double(y2-y1)) * farray[i_f][1]);
                if (val::abs(ix-ix0)<15 && val::abs(y-iy) < 15 ) return i;
                if (val::abs(endx-ix0)<15 && val::abs(y-iy) < 15 ) return i;
                ++i_f;
            }
            break;
        case myfunction::CIRCLE :
            {
                double mx,my,r,a1,a2,angle,px,py;
                val::GPair<double> D;
                int dx, dy,slice;

                px = (x2-x1)*double(x-abst)/double(sizex -1) + x1;
                py = double(yzero-y)*(y2-y1)/double(sizey-1);
                F[i].getCirclePoints(mx,my,r,a1,a2,slice);
                angle = degree_angle(px-mx,py-my);
                if (angle < a1 -3 || angle >a2 +3) {++i_f; break;}
                D = coordinatesdistance(mx,my,px,py,r,angle);
                dx = int(val::round(double(sizex) * D.x / (x2-x1), 0));
                dy = int(val::round(double(sizey) * D.y / (y2-y1), 0));
                //wxMessageBox(val::ToString(D.x) + " , " + val::ToString(D.y));
                if (dx < 10 && dy < 10) return i;
                ++i_f;
            }
            break;
        case myfunction::FILL :
            {
                ++i_f;
            }
            break;
        case myfunction::POINTS :
            {
                int ix0 = 0, n = farray[i_f].length(), j;
                for (j = 0; j < n; ++j) {
                    if (j%2 == 0) {
                        ix0 = abst + int(double(sizex)*((farray[i_f][j]-x1)/(x2-x1)));
                    }
                    else {
                        iy = yzero -int((double(sizey)/double(y2-y1)) * farray[i_f][j]);
                        if (val::abs(ix0-x) < 10 && val::abs(y-iy) < 10) {
                            pointactive = j-1;
                            return i;
                        }
                    }
                }
                ++i_f;
            }
            break;
        case myfunction::RECTANGLE :
            {
                int ix0,iy0,ix1,iy1;
                wxPoint l1,l2,p(x,y);
                const val::d_array<double> &g = farray[i_f];

                ix0 = abst+int(double(sizex)*((g[0]-x1)/(x2-x1)));
                iy0 = yzero -int((double(sizey)/double(y2-y1)) * g[1]);
                ix1 = abst+int(double(sizex)*((g[2]-x1)/(x2-x1)));
                iy1 = yzero -int((double(sizey)/double(y2-y1)) * g[3]);

                l1.x = ix0; l1.y = iy0; l2.x = ix1; l2.y = iy0;
                if (squaredistance(l1,l2,p)<100) return i;
                l1.x = ix1; l1.y = iy1;
                if (squaredistance(l1,l2,p)<100) return i;
                l2.x = ix0; l2.y = iy1;
                if (squaredistance(l1,l2,p)<100) return i;
                l1.x = ix0; l1.y = iy0;
                if (squaredistance(l1,l2,p)<100) return i;

                ++i_f;
            }
            break;
        default:  //myfunction::LINE , myfunction::TRIANGLE, myfunction::POLYGON
            {
                int ix0,j;
                double inf(val::Inf);
                val::d_array<double> g(farray[i_f]);
                wxPoint l0,l1,l2,p(x,y);

                for (j = 0; j < g.length(); ++j) {
                    if (j%2 == 0) {
                        if (g[j] == inf) g[j] = x2;
                        if (g[j] == -inf) g[j] = x1;
                    }
                    else {
                        if (g[j] == inf) g[j] = y2;
                        if (g[j] == -inf) g[j] = y1;
                    }
                }

                //ix = val::Max(abst,x);

                ix0 = abst+int(double(sizex)*((g[0]-x1)/(x2-x1)));
                iy = yzero -int((double(sizey)/double(y2-y1)) * g[1]);
                l0.x = l1.x = ix0; l0.y = l1.y = iy;
                //wxMessageBox(val::ToString(farray[i_f][0]) + " , " + val::ToString(farray[i_f][1]));
                //wxMessageBox(val::ToString(l1.x) + " , " + val::ToString(l1.y));

                for (j = 2; j < g.length(); ++j) {
                    if (j%2 == 0) ix0 = abst+int(double(sizex)*((g[j]-x1)/(x2-x1)));
                    else {
                        iy = yzero -int((double(sizey)/double(y2-y1)) * g[j]);
                        l2.x = ix0; l2.y = iy;
                        //wxMessageBox(val::ToString(l2.x) + " , " + val::ToString(l2.y));
                        //wxMessageBox(val::ToString(farray[i_f][j-1]) + " , " + val::ToString(farray[i_f][j]));
                        //wxMessageBox(val::ToString(squaredistance(l1,l2,p)));
                        if (squaredistance(l1,l2,p)<100) return i;
                        l1 = l2;
                    }
                }
                if (F[i].getmode() == myfunction::TRIANGLE && squaredistance(l0,l2,p)<100) return i;
                 ++i_f;
            }
            break;
        }

    }

    return -1;
}


// mouse - left - down
void PlotFunctionFrame::OnMouseCaptured(wxMouseEvent &event)
{
    DrawPanel->SetFocus();
    if (!yset) return;

    mouse_x1=event.GetX(); mouse_y1=event.GetY();


    if (drawpoints || drawpolygon || drawline) {
        //mouse_x1=event.GetX();mouse_y1=event.GetY();
        double x,y,gx1,gx2,gy1,gy2;
        int fx,fy;

        x=(x2-x1)*double(mouse_x1-abst) / double (sizex -1) + x1;
        y=double(yzero-mouse_y1)*(y2-y1)/double(sizey -1);
        if (rounddrawingpoints>=0) {
            x=val::round(x,rounddrawingpoints); y=val::round(y,rounddrawingpoints);
        }
        else if (rounddrawingpoints==-2) {
            fx = int(x/gx_scale); fy = int(y/gy_scale);
            if (x < 0 ) fx--;
            if (y < 0 ) fy--;
            gx1=double(fx)*gx_scale; gy1=double(fy)*gy_scale;
            gx2=gx1+gx_scale; gy2=gy1+gy_scale;
            if (val::abs(x-gx1)<=val::abs(x-gx2)) x=gx1;
            else x=gx2;
            if (val::abs(y-gy1)<=val::abs(y-gy2)) y=gy1;
            else y=gy2;
        }
        if (drawpoints && !addingpoints) {
            fstring+="points";
        }
        else if (drawpolygon && !n_polygonpoints) {
            fstring+="polygon";
        }
        else if (drawline && !n_linepoints) {
            fstring += "line";
        }
        else {
            int n = fstring.length();
            for (--n;n>=0;--n) if (fstring[n]==';') break;
            fstring.resize(n);
        }
        mx1=x; my1=y;
        fstring+="  " + val::ToString(x) + " " + val::ToString(y);
        if (drawpoints) addingpoints++;
        else if (drawpolygon) {
            n_polygonpoints++;
        }
        else {
            n_linepoints++;
            if (n_linepoints == 2) n_linepoints = 0;
        }
        if (!n_linepoints) refreshfunctionstring();
        GetSettings();
        Compute();

        return;
    }

    if (wxGetKeyState(WXK_CONTROL)) {
        active_function = findactivefunction(mouse_x1,mouse_y1);
        if (active_function == -1) return;
    }

    //mousecaptured=1;
    DrawPanel->CaptureMouse();
    //DrawPanel->SetCursor(wxCursor (wxCURSOR_HAND));
    DrawPanel->SetCursor(*closeHand);
    //mouse_x1=event.GetX(); mouse_y1=event.GetY();

    if (active_function != -1) {
        iscomputing = 1;
        //computemutex.lock();
        Paint();
    }

    if (mouse_x1<abst) return;//mouse_x1=abst;
    if (mouse_x1>sizex + abst) return;//mouse_x1 = sizex + abst;
    if (mouse_y1<abst) return;//mouse_y1=abst;
    if (mouse_y1 > sizey + abst) return;//mouse_y1 = sizey + abst;

    mx1=x1;mx2=x2;my1=y1;my2=y2;
    DrawPanel->Bind(wxEVT_MOTION,&PlotFunctionFrame::OnMouseMoved,this);
}


void PlotFunctionFrame::OnMouseMoved(wxMouseEvent &event)
{
    doubleclicked = 0;
    if (iscomputing) return;
    /*
    if (drawpolygon) {
        if (!n_polygonpoints) return;
        // Todo
        return;
    }
    */
    if (drawpoints || drawpolygon || drawline) {
        //if (!addingpoints) return;
        int xm=event.GetX(),ym=event.GetY();
        double x,y,dxm,dym,d=0;
        std::string text;

        dxm=(x2-x1)*double(xm-abst) / double (sizex -1) + x1;
        dym=double(yzero-ym)*(y2-y1)/double(sizey -1);
        text = "( " +val::ToString(dxm) + " , " + val::ToString(dym) + " )";
        if (addingpoints || n_polygonpoints || n_linepoints) {
            x=mx1; y = my1;
            x-=dxm;y-=dym;
            d=val::sqrt(x*x + y*y);
            text+= " ; " + val::ToString(d);
        }
        StatusBar1->SetStatusText(text,0);
        if (n_polygonpoints || n_linepoints) {
            polygonline = 1;
            iscomputing = 1;
            actualpolygonpoint.x = xm; actualpolygonpoint.y = ym;
            Paint();
        }
        return;
    }


    //wxSize panelsize = DrawPanel->GetSize();
    int mousex=event.GetX(),mousey = event.GetY();
    if (mousex<abst) mousex = abst;
    if (mousex>sizex+abst) mousex = sizex-abst;
    if (mousey<abst) mousey = abst;
    if (mousey>sizey+abst) mousey = sizey+abst;

    int dx=mousex - mouse_x1, dy = mousey-mouse_y1;
    double dx1,dy1;

    dx1 = (x2-x1)*double(dx)/double(sizex);
    dy1 = (y2-y1)*double(dy)/double(sizey);
    //x1-=dx1; x2-=dx1; y1+=dy1; y2+=dy1;
    if (active_function != -1) {
        displacefunction(active_function,dx1,-dy1);
        mouse_x1=mousex;
        mouse_y1=mousey;
        iscomputing = 1;
        Compute(active_function,0);
    }
    else {
        x1=mx1-dx1; x2=mx2-dx1; y1=my1+dy1; y2=my2+dy1;
        xstring=val::ToString(x1) + ";" + val::ToString(x2);
        ystring=val::ToString(y1) + ";" + val::ToString(y2);
        iscomputing=1;
        Compute(-1,0);
    }
    //computemutex.lock();
    //StatusBar1->SetStatusText(val::ToString(dx) + " , " + val::ToString(dy));

}

// mouse - left -up
void PlotFunctionFrame::OnMouseReleased(wxMouseEvent &event)
{
    if (drawpoints || drawpolygon || drawline) return;
    if (doubleclicked) {
        doubleclicked = 0;
        return;
    }

    DrawPanel->Unbind(wxEVT_MOTION,&PlotFunctionFrame::OnMouseMoved,this);
    //DrawPanel->SetCursor(*wxSTANDARD_CURSOR);
    DrawPanel->SetCursor(wxCursor (wxCURSOR_HAND));
    if (DrawPanel->HasCapture()) DrawPanel->ReleaseMouse();
    //if (iscomputing) return;

    if (active_function != -1) {
        //computemutex.lock();

        while (iscomputing) {
            wxYield();
        }

        fstring = "";
        int i = 0;
        for (const auto & v : F) {
            if (x_range[i].x == x_range[i].y) fstring += v.getinfixnotation() + ";\n";
            else fstring += v.getinfixnotation() + "[" + val::ToString(x_range[i].x) + " , " + val::ToString(x_range[i].y) + "];\n";
            ++i;
        }
        refreshfunctionstring();
        iscomputing=1;
        active_function=-1;
        GetSettings();
        Compute();
    }
#ifdef __APPLE__
    else {
        while (iscomputing) {
            wxYield();
        }
        Paint();
    }
#endif // __APPLE__
}


void PlotFunctionFrame::OnMouseDouble(wxMouseEvent &event)
{
    if (drawpolygon) {
        //changedrawpolygon();
        n_polygonpoints = 0;
        drawpolygon = 0;
        iscomputing = 1;
        Paint();
        drawpolygon = 1;
    }
    else if (drawpoints) {
        addingpoints = 0;
    }
    else {
        mouse_x1=event.GetX(); mouse_y1=event.GetY();
        //wxMessageBox(val::ToString(mouse_x1) + " " + val::ToString(mouse_y1));
        active_function = findactivefunction(mouse_x1,mouse_y1);
        if (active_function == -1) return;
        doubleclicked = 1;
        WriteText();
        Paint();
    }
}



void PlotFunctionFrame::OnLostMouse(wxMouseCaptureLostEvent &event)
{

}


void PlotFunctionFrame::displacefunction(int i,const double &dx1,const double &dy1)
{
    if (i<0 || i>=N) return;

    double dx = val::round(dx1,decimalx) , dy = val::round(dy1,decimaly);

    switch (F[i].getmode())
    {
        case myfunction::FUNCTION :
            {
                if (F[i].numberofvariables() == 1) {
                    val::valfunction f(F[i].getinfixnotation()), g("x"),h(val::ToString(dx)), d = g - h, y(val::ToString(dy));
                    f = f(d) + y;
                    F[i].infix_to_postfix(f.getinfixnotation());
                }
                else {
                    std::string sf = F[i].getinfixnotation() , sx, sy, nf = "";
                    int n = sf.length();
                    if (dx > 0) sx = "(x - " + val::ToString(dx) + ")";
                    else sx = "(x + " + val::ToString(val::abs(dx)) + ")";
                    if (dy > 0) sy = "(y - " + val::ToString(dy) + ")";
                    else sy = "(y + " + val::ToString(val::abs(dy)) + ")";
                    for (int j=0; j<n ; ++j) {
                        if (sf[j] == 'x') nf += sx;
                        else if (sf[j] == 'y') nf += sy;
                        else nf +=sf[j];
                    }
                    val::valfunction f(nf);
                    F[i].infix_to_postfix(f.getinfixnotation());
                }

            }
            break;
        case myfunction::TEXT :
            {
                int j,k=0;
                std::string sf = F[i].getinfixnotation(), nf="";
                int n = sf.length();
                for (j = 0; j < n; ++j) {
                    nf += sf[j];
                    if (sf[j] == '}') break;
                }
                for (j = 0; j < i; ++j ) {
                    if (F[j].numberofvariables() == 1) k++;
                }
                nf += " " + val::ToString(farray[k][0] + dx) + " " + val::ToString(farray[k][1] + dy);
                F[i].infix_to_postfix(nf);
                //wxMessageBox(F[i].getTextData());
            }
            break;
        case myfunction::CIRCLE :
            {
                double x,y,r,a1,a2;
                int slice;
                std::string nf = "circle ";

                F[i].getCirclePoints(x,y,r,a1,a2,slice);
                nf += val::ToString(x+dx) + " " + val::ToString(y+dy) + " " + val::ToString(r) + " " + val::ToString(a1) + " " + val::ToString(a2) + " " + val::ToString(slice);
                F[i].infix_to_postfix(nf);
            }
            break;
        case myfunction::POINTS :
            {
                int j,k=0;
                std::string nf = "points";

                //wxMessageBox(val::ToString(pointactive)); m
                for (j=0;j<i;++j) {
                    if (F[j].numberofvariables() == 1) ++k;
                }
                for (j = 0; j < farray[k].length(); ++j) {
                    if (j == pointactive) {
                        nf += " " + val::ToString(farray[k][j] + dx) + " " + val::ToString(farray[k][j+1] + dy);
                        ++j;
                    }
                    else nf += " " + val::ToString(farray[k][j]);
                }
                F[i].infix_to_postfix(nf);
            }
            break;
        case myfunction::FILL :
            {
            }
            break;
        case myfunction::RECTANGLE :
            {
                int j, k = 0;
                std::string nf = "rectangle";

                for (j=0;j<i;++j) {
                    if (F[j].numberofvariables() == 1) ++k;
                }
                for (j = 0; j < 4; ++j) {
                    if (j%2 == 0) nf += " " + val::ToString(farray[k][j] + dx);
                    else nf += " " + val::ToString(farray[k][j] + dy);
                }

                F[i].infix_to_postfix(nf);
            }
            break;
        default:  //myfunction::LINE , myfunction::TRIANGLE, myfunction::POLYGON
            {
                std::string nf = "";
                int k = 0, j;
                switch (F[i].getmode())
                {
                case myfunction::LINE :
                    nf = "line";
                    break;
                case myfunction::TRIANGLE :
                    nf = "triangle";
                    break;
                case myfunction::POLYGON :
                    nf = "polygon";
                    break;
                default:
                    break;
                }
                for (j = 0; j < i; ++j ) {
                    if (F[j].numberofvariables() == 1) k++;
                }
                for (j = 0; j < farray[k].length(); ++j) {
                    if (j%2 == 0) {
                        nf += " " + val::ToString(farray[k][j] + dx);
                    }
                    else nf += " " + val::ToString(farray[k][j] + dy);
                }
                F[i].infix_to_postfix(nf);
            }
            break;
    }
}


void PlotFunctionFrame::changedrawpoints()
{
    if (!drawpoints) {
            if (drawline) changedrawline();
            if (drawpolygon) changedrawpolygon();

            SendNotification("Insert Points: ON");
            StatusBar1->SetStatusText(_T("Insert Points"),1);
            addpointsmenu->Check(true);
            DrawPanel->SetCursor(wxCursor (wxCURSOR_ARROW));
            DrawPanel->Bind(wxEVT_MOTION,&PlotFunctionFrame::OnMouseMoved,this);
            //DrawPanel->CaptureMouse();
    }
    else {
             //wxNotificationMessage message(_T("Mouse Mode"),_T("Drawing Points: OFF"),this);

             //message.Show(1);
             SendNotification("Insert Points: OFF");
             StatusBar1->SetStatusText(_T(""),1);
             addpointsmenu->Check(false);
             DrawPanel->SetCursor(wxCursor (wxCURSOR_HAND));
             DrawPanel->Unbind(wxEVT_MOTION,&PlotFunctionFrame::OnMouseMoved,this);
             if (DrawPanel->HasCapture()) DrawPanel->ReleaseMouse();
             addingpoints=0;
    }
    drawpoints=!drawpoints;
}


void PlotFunctionFrame::changedrawpolygon()
{
    if (!drawpolygon) {
             if (drawpoints) changedrawpoints();
             if (drawline) changedrawline();

             SendNotification("Insert Polygon: ON");
             StatusBar1->SetStatusText(_T("Insert Polygon!"),1);
             polygonpointsmenu->Check(true);
             DrawPanel->SetCursor(wxCursor (wxCURSOR_ARROW));
             DrawPanel->Bind(wxEVT_MOTION,&PlotFunctionFrame::OnMouseMoved,this);
             drawpolygon = 1;
    }
    else {
             SendNotification("Insert Polygon: OFF");
             StatusBar1->SetStatusText(_T(""),1);
             polygonpointsmenu->Check(false);
             DrawPanel->SetCursor(wxCursor (wxCURSOR_HAND));
             DrawPanel->Unbind(wxEVT_MOTION,&PlotFunctionFrame::OnMouseMoved,this);
             if (DrawPanel->HasCapture()) DrawPanel->ReleaseMouse();
             n_polygonpoints=0;
             drawpolygon = 0;
             iscomputing = 1;
             Paint();
    }
}

void PlotFunctionFrame::changedrawline()
{
    if (!drawline) {
             if (drawpolygon) changedrawpolygon();
             if (drawpoints) changedrawpoints();

             SendNotification("Insert Line: ON");
             StatusBar1->SetStatusText(_T("Insert Line!"),1);
             linepointsmenu->Check(true);
             DrawPanel->SetCursor(wxCursor (wxCURSOR_ARROW));
             DrawPanel->Bind(wxEVT_MOTION,&PlotFunctionFrame::OnMouseMoved,this);
             drawline = 1;
    }
    else {
             SendNotification("Insert Line: OFF");
             StatusBar1->SetStatusText(_T(""),1);
             linepointsmenu->Check(false);
             DrawPanel->SetCursor(wxCursor (wxCURSOR_HAND));
             DrawPanel->Unbind(wxEVT_MOTION,&PlotFunctionFrame::OnMouseMoved,this);
             if (DrawPanel->HasCapture()) DrawPanel->ReleaseMouse();
             n_linepoints=0;
             drawline = 0;
             iscomputing = 1;
             Paint();
    }
}




void PlotFunctionFrame::SendNotification(const std::string& s)
{
    using namespace std::chrono_literals;
    val::InfoWindow *notification = new val::InfoWindow(this,nchildwindows,s,wxDefaultPosition,wxDefaultSize,"Info",20,0);
    wxPoint pos = GetPosition();
    pos.x += 10; pos.y += 50;
    if (SideText_isshown) pos.x += widthSideText;
    notification->Move(pos);
    notification->Show();
    for (int i=0;i<5000;++i) wxYield();
    std::this_thread::sleep_for(500ms);
    notification->Destroy();
}


void PlotFunctionFrame::OnZoom(wxCommandEvent &event)
{
    int id = event.GetId();
    if (active_function != -1) {
        if (id == 20001) ++pen[active_function];
        else --pen[active_function];
        if (pen[active_function] < 0) pen[active_function] = 0;
        if (pen[active_function] > 100) pen[active_function] = 100;
        if (F[active_function].IsText()) {
            int fs = Font[active_function].GetPointSize();
            if (id == 20001) ++fs;
            else --fs;
            if (fs < 0) fs = 0;
            if (fs >100) fs = 100;
            Font[active_function].SetPointSize(fs);
        }
        Paint();
        return;
    }
    if (!yset) return;
    if (id == 20001) zoom*=0.96;
    else zoom*=1.04;
    if (!iscomputing) {
        iscomputing=1;
        x1*=zoom;x2*=zoom;y1*=zoom;y2*=zoom;
        zoom=1.0;
        xstring=val::ToString(x1) + ";" + val::ToString(x2);
        ystring=val::ToString(y1) + ";" + val::ToString(y2);
        //StatusBar1->SetStatusText(val::ToString(a) + "  " + val::ToString(f));
        Compute();
        //Paint();
    }
}

void PlotFunctionFrame::OnMove(wxCommandEvent &event)
{

    if (iscomputing) return;

    int id = event.GetId();

    if (id == 20009 || id == 20010) {
        if (!SideText_isshown) return;
        if (id == 20009) widthSideText += 5;
        else widthSideText -= 5;
        //if (widthSideText > 200) widthSideText = 200;
        if (widthSideText < 100) widthSideText = 100;
        wxSize Size = DrawPanel->GetSize(), TextSize = SideText->GetSize();

        TextSize.x = widthSideText;
        SideText->SetSize(TextSize);
        TextSize.y = -1;
        SideText->SetMinSize(TextSize);

        widthSideText = SideText->GetSize().x;
        Size.x += 30 + widthSideText;
        Size.y += 20;
        //DrawPanel->Hide();

        //wxMessageBox(val::ToString(widthSideText));
        //BoxSizer2->Clear();
        //BoxSizer2->Add(SideText, 0, wxALL|wxEXPAND, 5);
        //BoxSizer2->Add(DrawPanel, 1, wxALL|wxEXPAND, 5);

        //BoxSizer2->Fit(SideText);

        BoxSizer1->Fit(this);
        BoxSizer1->SetSizeHints(this);
        SetClientSize(Size);

        return;
    }

    if (!yset) return;
    //double dx = 0.1, dy = 0.1;

    switch (id)
    {
    case 20003 : {movex += movedx;} break;
    case 20004 : {movex -= movedx;} break;
    case 20005 : {movey += movedy;} break;
    case 20006 : {movey -= movedy;} break;
    default:
        break;
    }

    if (moveinpointsx) movex = (x2 - x1)*movex/double(sizex);
    if (moveinpointsy) movey = (y2 - y1)*movey/double(sizey);

    if (active_function != -1) {
        std::string svalue = val::ToString(movex) + " " + val::ToString(movey);
        ExecuteCommand(MOVE,active_function,svalue);
        movex = movey = 0;
    }
    else {
        x1 -= movex; x2 -= movex; y1 -= movey; y2 -= movey;
        xstring=val::ToString(x1) + ";" + val::ToString(x2);
        ystring=val::ToString(y1) + ";" + val::ToString(y2);
        movex=movey=0;
        iscomputing=1;
        Compute();
    }

}


void PlotFunctionFrame::OnSelectActiveFunction(wxCommandEvent &event)
{
    if (iscomputing) return;
    //wxMessageBox("active Function");
    int id = event.GetId();
    if (id == 20007 || id == 20011) {
        if (N==0) return;
        int i, j = active_function,n;

        if (j != -1 && F[j].IsPoints()) {
            int i_f = 0;
            for (i = 0; i < j; ++i) {
                if (F[i].numberofvariables() == 1) ++i_f;
            }
            n = farray[i_f].length();
            //wxMessageBox(val::ToString(n));
            if (id == 20007) pointactive += 2;
            else pointactive -= 2;
            if (pointactive < n && pointactive >= 0) {
                iscomputing = 1;
                Paint();
                return;
            }
            else pointactive = 0;
        }

        //j = active_function;


        val::d_array<int> checked;

        checked.reserve(N);
        for (i=0;i<N;++i) {
            if (f_menu[i]->IsChecked() && !F[i].IsFill()) checked.push_back(i);
        }
        n = checked.length();
        if (n == 0) return;
        for (i = 0; i < n ; ++i) {
            if (checked[i] == active_function) j = i;
        }
        if (id == 20007) ++j;
        else --j;
        j %= n;
        if (j < 0) j = n-1;
        /*
        for (i = 0; i < n; ++i) {
            if (j <= checked[i]) break;
        }
        */
        //active_function = checked[i%n];
        active_function = checked[j];
        StatusBar1->SetStatusText(F[active_function].getinfixnotation());
        iscomputing = 1;
        WriteText();
        Paint();
    }
    else {
        if (drawpoints) {
            changedrawpoints();
            return;
        }
        if (drawpolygon) {
            changedrawpolygon();
            return;
        }
        if (drawline) {
            changedrawline();
            return;
        }
        if (active_function == -1) return;
        active_function = -1;
        pointactive = 0;
        iscomputing = 1;
        doubleclicked = 0;
        WriteText();
        Paint();
    }
}


#ifdef _WIN32
void PlotFunctionFrame::setfunctionsmenu()
{
    Menu_functions->Append(addfunction);
    Menu_functions->Append(undomenu);
    Menu_functions->Append(redomenu);
    Menu_functions->Append(hideallmenu);
    Menu_functions->Append(showallmenu);
    Menu_functions->Append(deletelastmenu);
    Menu_functions->Append(deleteallmenu);
}
#else
void PlotFunctionFrame::setfunctionsmenu()
{
    return;
}
#endif // _WIN32





void PlotFunctionFrame::OnMenuRecent(wxCommandEvent& event)
{
    if (event.GetId()==601) {
        int n=recentfiles.length();
        recentfiles.dellist();
        for (int i=0;i<n;++i) {
            MenuRecentfiles->Remove(6000+i);
            Unbind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuRecent,this,6000+i);
            delete recent_menu[i];
        }
        recent_menu.dellist();
        return;
    }

    int m=event.GetId()-6000,isvalid=1;
    std::string f,dirname,filename;


    f=recentfiles[m];
    if (!val::FileExists(f)) {
        isvalid=0;
        wxMessageBox(f + " does not exist!");
    }
    else {
        std::fstream file(f,std::ios::in);
        std::string line;
        std::getline(file,line);
        if (line!="plotfunctionfile") {
            wxMessageBox("Wrong Type of file!");
            isvalid=0;
        }
    }

    if (!isvalid) {
        int n=recentfiles.length();
        MenuRecentfiles->Remove(6000+n-1);
        Unbind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuRecent,this,6000+n-1);
        delete recent_menu[n-1];
        recent_menu.delelement(n-1);
        recentfiles.delelement(m);
        n--;
        for (int i=0;i<n;++i) recent_menu[i]->SetItemLabel(recentfiles[i]);
        return;
    }

    checkinrecent(f);
    // Extract filename:
    int i,j,n=f.length();
    j=n;
    char fs = filesep[0];
    for (i=n-1;i>=0;i--) {
        if (f[i]==fs) {
            j=i;
            break;
        }
    }
    for (i=0;i<j;++i) dirname+=f[i];

    for (i=j+1;i<n;++i) filename+=f[i];
    openfile(dirname,filename);
}

int PlotFunctionFrame::checkinrecent(const std::string& filename)
{
    int i=0,n=recentfiles.length();
    std::string s;

    for (i=0;i<n;++i) {
        s=recentfiles[i];
        if (s==filename) {
            if (i==0) return 1;
            int j;
            for (j=i;j>0;--j) recentfiles[j]=recentfiles[j-1];
            recentfiles[0]=filename;
            for (j=0;j<n;++j) recent_menu[j]->SetItemLabel(recentfiles[j]);
            return 1;
        }
    }
    return 0;
}


void PlotFunctionFrame::appendrecentfiles(const std::string& filename)
{
    if (checkinrecent(filename)) return;

    int n=recentfiles.length(),i;
    if (n==MaxRecent) {
        for (i=n-1;i>0;--i) recentfiles[i] = recentfiles[i-1];
        recentfiles[0] = filename;
    }
    else {
        recentfiles.push(filename);
        recent_menu.push_back(nullptr);
        recent_menu[n] = new wxMenuItem(MenuRecentfiles,6000+n,recentfiles[0]);
        MenuRecentfiles->Insert(n,recent_menu[n]);
        Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuRecent,this,6000+n);
        ++n;
    }
    for (i=0;i<n;++i) recent_menu[i]->SetItemLabel(recentfiles[i]);
}


void PlotFunctionFrame::WriteText()
{
    //if (!SideText_isshown) return;
    SideText->SetValue("");
    wxColour def_color = SideText->GetForegroundColour();
    wxFont myfont = SideText->GetFont();
    myfont.SetPointSize(fontsize);
    SideText->SetFont(myfont);

    val::d_array<char> separfunc{';'};
    val::Glist<std::string> s_functions = getwordsfromstring(fstring,separfunc,0,val::d_array<char>{'\n'});
    int n = val::Min(s_functions.length(),N);
    wxTextAttr Style = SideText->GetDefaultStyle();
    Style.SetFontSize(fontsize);

    for (int i = 0; i < n; ++i) {
        Style.SetTextColour(Color[i]);
        if (i == active_function) {
            Style.SetFontWeight(wxFONTWEIGHT_BOLD);
            Style.SetFontUnderlined(true);
        }
        else {
            Style.SetFontWeight(wxFONTWEIGHT_NORMAL);
            Style.SetFontUnderlined(false);
        }
        SideText->SetDefaultStyle(Style);
        SideText->WriteText("#" + val::ToString(i+1) + ": ");
        Style.SetTextColour(def_color);
        SideText->SetDefaultStyle(Style);
        SideText->WriteText(s_functions[i] + ";\n");
    }
    SideText_Word = SideText->GetValue();
}


void PlotFunctionFrame::CompareSideTextInput()
{
    std::string O_Word;
    O_Word = SideText->GetValue();
    if (O_Word == SideText_Word) {
        return;
    }
    val::d_array<char> sep{';'}, numsep{':'};
    val::Glist<std::string> words = getwordsfromstring(O_Word,sep,0,val::d_array<char>{'\n'});
    std::string s_func, first_word;
    int m;

    fstring = "";
    for (const auto &v : words) {
        first_word = val::getfirstwordofstring(v,numsep);
        //wxMessageBox(first_word);
        if ( (m = first_word.length() > 0) && first_word[0] == '#') {
            s_func = val::tailofstring(v,v.length()-m-3);
        }
        else s_func = v;
        fstring += s_func + ";\n";
    }
    refreshfunctionstring();
    GetSettings();
    Compute();
    //WriteText();
}


void PlotFunctionFrame::OnLostFocus(wxFocusEvent &event)
{
    //wxMessageBox("Focus on Text lost!");
    CompareSideTextInput();
    CheckFocus();
    event.Skip();
}


void PlotFunctionFrame::OnSideBarCheck(wxCommandEvent&)
{
    wxSize Size = DrawPanel->GetSize();
    //wxPoint pos = GetPosition();
    //wxMessageBox(val::ToString(Size.x) + ";" + val::ToString(Size.y));
    Size.x += 20;
    Size.y += 20;
    if (SideText_isshown) {
        //pos.x += widthSideText + 10;
        //Move(pos);
        SideText_isshown = 0;
        SideText->Hide();
        closebrackets = SideText->GetCloseBrackets();
        DrawPanel->SetFocus();
#ifdef _WIN32
        CheckFocus();
        SetClientSize(Size);
        Update();
        Paint();
        return;
#endif // _WIN32
    }
    else {
        //pos.x -= (widthSideText + 10);
        //Move(pos);
        SideText_isshown = 1;
        Size.x += widthSideText + 10;
        WriteText();
        SideText->Show();
        SideText->SetFocus();
        SideText->SetCloseBrackets(closebrackets);
    }
    //Move(pos);
    CheckFocus();
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);
    SetClientSize(Size);
    //Move(pos);
}


void PlotFunctionFrame::CheckFocus()
{
#ifdef __APPLE__
    return;
#endif // __APPLE__

    if (SideText->HasFocus()) {
        Unbind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1008);         // Copy to Clipboard
        Unbind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1009);         // Paste from Clipboard
    }
    else {
        Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1008);         // Copy to Clipboard
        Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenuFill,this,1009);         // Paste from Clipboard
    }
}
