/***************************************************************
 * Name:      PlotFunctionMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Miguel Valbuena ()
 * Created:   2018-09-28
 * Copyright: Miguel Valbuena ()
 * License:
 **************************************************************/

#include "PlotFunctionMain.h"
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include "calculus.h"
#include <thread>
#include <val_utils.h>
#include <analysis.h>
#include <rational.h>
#include <wx/bitmap.h>
#include <wx/dcmemory.h>
#include <wx/colordlg.h>
#include "MultiLineDialog.h"
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




PlotFunctionFrame::PlotFunctionFrame(wxWindow* parent,wxWindowID id)
{
    //*Initialize(PlotFunctionFrame)
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer2;
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

    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    BoxSizer2 = new wxBoxSizer(wxHORIZONTAL);
    DrawPanel = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxSize(400,300), wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    DrawPanel->SetMinSize(wxSize(-1,-1));
    DrawPanel->SetMaxSize(wxSize(-1,-1));
    DrawPanel->SetForegroundColour(wxColour(255,255,255));
    DrawPanel->SetBackgroundColour(wxColour(255,255,255));
    BoxSizer2->Add(DrawPanel, 1, wxALL|wxEXPAND, 5);
    BoxSizer1->Add(BoxSizer2, 1, wxALL|wxEXPAND, 5);
    SetSizer(BoxSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    Menu1->Append(101,_("New\tCtrl-N"));
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
    MenuBar1->Append(Menu1, _("&File"));
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
    MenuBar1->Append(Menu3, _("Settings"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[2] = { -3,-1 };
    //int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(2,__wxStatusBarWidths_1);
    //StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    //StatusBar1->SetFieldsCount(2);
    SetStatusBar(StatusBar1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    //DrawPanel->Connect(wxEVT_PAINT,(wxObjectEventFunction)&PlotFunctionFrame::OnDrawPanelPaint,0,this);
    DrawPanel->Bind(wxEVT_PAINT,&PlotFunctionFrame::OnDrawPanelPaint,this);
    DrawPanel->Bind(wxEVT_SIZE,&PlotFunctionFrame::OnDrawPanelResize,this);
    //DrawPanel->Connect(wxEVT_SIZE,(wxObjectEventFunction)&PlotFunctionFrame::OnDrawPanelResize,0,this);

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
    wxMenuItem *SetCSize= new wxMenuItem(Menu3,22,"Set Panel Size...\tF2");
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
    submenuaxis->Append(xrangemenu);
    submenuaxis->Append(yrangemenu);
    submenuaxis->Append(x_scaleactiv);
    submenuaxis->Append(y_scaleactiv);
    submenuaxis->Append(scalemenu);
    submenuaxis->Append(gridactiv);
    submenuaxis->Append(gridmenu);
    x_scaleactiv->Check(); y_scaleactiv->Check();
    //
    addfunction=new wxMenuItem(Menu_functions,3000,"Add/Remove new functions...\tAlt-N", wxEmptyString);
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
    MenuBar1->Insert(2,Menu_functions, _("Functions"));
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
    MenuBar1->Insert(3,Menu_Tools,"Tools");
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
    //
    wxAcceleratorEntry entries[15];
    entries[0].Set(wxACCEL_CTRL, (int) '+',20001);
    entries[1].Set(wxACCEL_CTRL, (int) '-',20002);
    entries[2].Set(wxACCEL_CTRL,WXK_RIGHT,20003);
    entries[3].Set(wxACCEL_CTRL,WXK_LEFT,20004);
    entries[4].Set(wxACCEL_CTRL,WXK_UP,20005);
    entries[5].Set(wxACCEL_CTRL,WXK_DOWN,20006);
    entries[6].Set(wxACCEL_SHIFT|wxACCEL_ALT,(int) 'P',1005);
    entries[7].Set(wxACCEL_CTRL,WXK_TAB,20007);
    entries[8].Set(wxACCEL_NORMAL,WXK_ESCAPE,20008);
    entries[9].Set(wxACCEL_SHIFT|wxACCEL_CTRL,(int) 'P',1006);
    entries[10].Set(wxACCEL_SHIFT|wxACCEL_CTRL,(int) 'L',1007);
    entries[11].Set(wxACCEL_CTRL,(int) 'C',1008);
    entries[12].Set(wxACCEL_CTRL,WXK_INSERT,1008);
    entries[13].Set(wxACCEL_CTRL,(int) 'V',1009);
    entries[14].Set(wxACCEL_SHIFT,WXK_INSERT,1009);
    wxAcceleratorTable accel(15,entries);
    SetAcceleratorTable(accel);
    //

    defaultFont=this->GetFont();
    Color.resize(7);

    DrawPanel->SetMinSize(wxSize(100,10));
    GetSizeSettings();
    if (settings) {
        SetClientSize(wxSize(clientsize_x,clientsize_y));
        Move(wxPoint(Posx,Posy));
    }
    else SetClientSize(wxSize(500,500));
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
        wxSize MySize=GetClientSize();
        wxPoint MyPoint=GetPosition();
        file<<MyPoint.x<<endl;
        file<<MyPoint.y<<endl;
        file<<MySize.x<<endl;
        file<<MySize.y<<endl;
        file<<fontsize<<endl;
        file<<filedir<<endl;
        file<<openfiledir<<endl;
        file<<savefiledir<<endl;
#ifdef _WIN32
        file<<defaultFont.GetNativeFontInfoUserDesc()<<endl<<endl;
#else
        file<<defaultFont.GetNativeFontInfoDesc()<<endl<<endl;
#endif // _WIN32
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
        abst=10; x_scale=1; y_scale=1;// gx_scale=1; gy_scale=1;
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
    file>>Posx>>Posy>>clientsize_x>>clientsize_y>>fontsize;
	if (clientsize_x == 0) clientsize_x = 400;
	if (clientsize_y == 0) clientsize_y = 400;
	if (fontsize < 8) fontsize = 8;
	file.clear();
	file.seekg(0,std::ios::beg);
	while (file && i<=4) {
        getline(file,line);
        ++i;
	}
	if (file) {getline(file,line);filedir=line;}
	if (file) {getline(file,line);openfiledir=line;}
	if (file) {getline(file,line);savefiledir=line;}
	if (file) {getline(file,line);defaultFont.SetNativeFontInfoUserDesc(wxString(line));}
	//if (file) {getline(file,line);defaultFont.SetNativeFontInfo(wxString(line));}
    file.close();
    //
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
    defaultpaintcolor = wxColour(0,0,255);  // blue
    Color[0]=wxColour(0,0,255);// blue
    Color[1]=wxColour(255,0,0); // red
    Color[2]=wxColour(0,255,0); // green
    Color[3]=wxColour(0,230,246); // light blue
    Color[4]=wxColour(255,116,0); // orange
    Color[5]=wxColour(238,0,255); // violet
    Color[6]=wxColour(125,125,125);  // grey
    BackgroundColor = wxColour(255,255,255);  // white
    axis_color=wxColour(0,0,0);
    grid_color=wxColour(191,191,191);
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
    if (fstring=="") {setfunctionsmenu(); return;}
    val::d_array<char> ignore({'\n'});

    svalues = getwordsfromstring(fstring,separators,0,ignore);
    for (auto& v : svalues) {
        getfunctionfromstring(v,f_s,xr.x,xr.y);
        f = myfunction(f_s);
        if (!f.iszero()) {
            F.push_back(std::move(f));
            x_range.push_back(xr);
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
    if (N>Font.length()) {
        int i,l=Font.length();
        Font.resize(N);
        for (i=l;i<N;++i) Font[i]=defaultFont;
    }
    if (N>pen.length()) {
        int i,l=pen.length();
        pen.resize(N);

        for (i=l;i<N;++i) {
#ifdef _WIN32
            if (F[i].IsPoints()) pen[i]=6;
            else pen[i]=2;
#else
            if (F[i].IsPoints()) pen[i]=8;
            else pen[i]=2;
#endif // _WIN32
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
        //Connect(2000+i,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PlotFunctionFrame::OnMenu_xAxisSelected);
        Bind(wxEVT_COMMAND_MENU_SELECTED,&PlotFunctionFrame::OnMenu_xAxisSelected,this,2000+i);
        if (i<=8) t="\tCtrl-" + val::ToString(i+1);
        else t="";
        {
            menuitemstring = val::headofstring(F[i].getinfixnotation(),40);
            if (F[i].getinfixnotation().length()>40) menuitemstring+="...";
            c_menu[i] = new wxMenuItem(colorsubmenu,4000+i,menuitemstring +t);
            colorsubmenu->Append(c_menu[i]);
        }
        //Connect(4000+i,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&PlotFunctionFrame::OnMenuColours);
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
 }

void PlotFunctionFrame::plotvertices(wxDC& dc)
{
    dc.SetTextForeground(axis_color);

    int ypainted = 0, xpainted = 0, cw = dc.GetCharWidth(), dyaxis = 8;
    wxFont normalfont=dc.GetFont(), italicfont=dc.GetFont();
    int normalsize = normalfont.GetPointSize(), nsize=normalsize , dscale =3,dx=4,dy=6,pen=axis_pen;

    italicfont.MakeItalic();
    italicfont.MakeBold();
    wxString pi = L"\u03C0",swert;
    val::rational factor,one(1),minusoune(-1);

    wxSize sz=dc.GetSize();

    sizex=sz.x -2*abst;
    sizey=sz.y-2*abst;

    if (sizex <320) {nsize-=2;dscale--;dx--;dy--;cw-=2;dyaxis-=2;}
    if (sizex < 220) {nsize -=2;dscale--;dx--;dy--;pen--;cw-=1;dyaxis-=2;}
    if (nsize < 2) nsize =2;

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
            int ix;

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
                    dc.DrawText(swert,ix-swert.size()*4,yzero+2);
                }
                else {
                    dc.DrawText(swert,ix-swert.size()*4,abst + sizey-3);
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
            int iy,xzero=abst+1+ int(double(sizex-1)*((-x1)/(x2-x1)));

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
                    dc.DrawText(swert,xzero - (swert.size()+1)*cw,iy-dyaxis);
                }
                else {
                    if (y==y1) continue;
                    dc.DrawText(swert,0,iy-dyaxis);
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
        }
        if (i >= ew) ready = 1;
    }
    while (!ready);
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
        dc.SetPen(wxPen(Color[colour],8));
        dc.DrawPoint(ix0,iy0);
        return;
    }

    dc.DrawLine(ix0,iy0,ix1,iy1);
}


void PlotFunctionFrame::plotcircle(wxDC& dc,const val::d_array<double> &f,int colour)
{
    if (!yset || sizex <= 10 || sizey <=10 ) return;

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

    dc.DrawEllipticArc(ix0,iy0,ix1,iy1,f[4],f[5]);
    if (active_function == colour) {
        double x,y,r,a1,a2;
        F[colour].getCirclePoints(x,y,r,a1,a2);
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


void PlotFunctionFrame::plotfill(wxDC& dc,const val::d_array<double> &f,int colour)
{
    if (!yset) return;
    if (f[0]<x1 || f[0]>x2 || f[1]<y1 || f[1]>y2) return;

    int ix,iy;
    wxColor &col =Color[colour];

    if (f[2]!=1.0) {
        char green=col.Green(),blue=col.Blue(),red=col.Red();
        int transp = int(val::round(f[2]*255,0));

        col = wxColour(red,green,blue,transp);
    }
    wxBrush brush(col);
    dc.SetBrush(brush);

    ix=abst+int(double(sizex-1)*((f[0]-x1)/(x2-x1)));
    iy=yzero -int((double(sizey-1)/double(y2-y1)) * f[1]);

    dc.FloodFill(ix,iy,dc.GetBackground().GetColour());
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
        dc.SetPen(wxPen(Color[colour],8));
        dc.DrawPoint(ix0,iy0);
        actualpolygonpoint = wxPoint(ix0,iy0);
    }
}

void PlotFunctionFrame::plotpoints(wxDC& dc,const val::d_array<double> &f,int colour)
{
    if (!yset) return;
    int i,n=f.length();
    if (n<2) return;

    int ix,iy;

    dc.SetPen(wxPen(Color[colour],pen[colour]));

    for (i=0;i<n;i+=2) {
        if (isInf(f[i]) || isInf(f[i+1]) || val::isNaN(f[i]) || val::isNaN(f[i+1])) continue;
        ix=abst+int(double(sizex-1)*((f[i]-x1)/(x2-x1)));
        iy=yzero -int((double(sizey-1)/double(y2-y1)) * f[i+1]);
        if (f[i]<x1 || f[i]>x2) continue;
        if (iy<abst || iy >(abst+sizey)) continue;
        if (active_function == colour && i == pointactive) {
            dc.SetPen(wxPen(Color[colour],pen[colour]+3));
        }
#ifdef _WIN32
        dc.DrawCircle(ix,iy,1);
#else
        //dc.DrawCircle(ix,iy,1);
        dc.DrawPoint(ix,iy);
#endif // _WIN32
        if (active_function == colour && i == pointactive) {
            dc.SetPen(wxPen(Color[colour],pen[colour]));
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


void PlotFunctionFrame::plotallfunctions(wxDC& dc)
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
                        {plotfunction(dc,farray[i_f],i);} break;
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

    // Create a memory Device Context
    wxMemoryDC dc;
    // Tell memDC to write on ???paper???.
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
    //delete paper;


    ispainted=1;
    computemutex.unlock();
    iscomputing=0;
}


void PlotFunctionFrame::plottomemoryDc(wxMemoryDC &memDC)
{
    memDC.SetBackground(wxBrush(BackgroundColor));
    memDC.Clear();
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
    wxBitmap *paper = new wxBitmap(size);

    // Create a memory Device Context
    wxMemoryDC memDC;
    // Tell memDC to write on ???paper???.
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

    // Put the contents of "paper??? into a png and into a jpeg file.
    paper->SaveFile(filename, wxBITMAP_TYPE_PNG,(wxPalette*)NULL );
    //paper->SaveFile( _T("RedSquare.jpg"), wxBITMAP_TYPE_JPEG,(wxPalette*)NULL );
    delete paper;
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
     if (dialog.ShowModal()!=wxID_OK) return;

     val::d_array<char> separators{' ', ',', ';', ':'};
     val::Glist<std::string> words = getwordsfromstring(dialog.GetSettingsText(),separators);

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
        x += 20; y+= 20;
        if (x < 50) x = 50;
        if (y < 50) y = 50;
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
        InputFunctionDialog dialog(this,WordList,input,"Enter or delete functions:","Add/Remove functions",wxSize(240,100),fontsize);
        //dialog.SetSize(DialogInputSize);
        if (dialog.ShowModal()==wxID_CANCEL) return;
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
        FunctionColorDialog Dialog(this,col,pen);
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
        if (dialog.ShowModal() == wxID_OK) defaultpaintcolor = dialog.GetColourData().GetColour();
        delete cdata;
        return;
    }
    if (evid==21) {
        wxFontData data;
        data.SetInitialFont(defaultFont);
        wxFontDialog dialog(this,data);
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
        }
        return;
    }
    else {
        FunctionColorDialog Dialog(this,Color[i],pen[i]);
        if (Dialog.ShowModal()==wxID_OK) {
            Color[i]=Dialog.GetColor();
            pen[i]=Dialog.GetLineWith();
            Paint();
        }
        return;
    }
}


void PlotFunctionFrame::OnMenuResetColours(wxCommandEvent &event)
{
    ResetColours();
    Paint();
}


void PlotFunctionFrame::OnMenuFontSize(wxCommandEvent& event)
{
    val::MultiLineDialog fontsizedialog(this,val::ToString(fontsize),"Entry Size",240,2*fontsize+6,"Set Font Size",fontsize,1,wxTE_RIGHT);
    if (fontsizedialog.ShowModal()==wxID_OK) {
        fontsize=val::FromString<int>(fontsizedialog.GetSettingsText());
        if (fontsize<10) fontsize=10;
        if (fontsize>16) fontsize=16;
    }
}


void PlotFunctionFrame::OnMenuParameter(wxCommandEvent &event)
{
    if (N!=1) return;
    if (!F[0].iswithparameter()) return;

    int i,n,anz=0;
    std::string pars,hs,fs=F[0].getinfixnotation(),xrs="";
    val::MultiLineDialog pardialog(this,"","Entry Values separated by ;",240,2*fontsize+6,"Set Parameter Values",fontsize,1);

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
        if (dialog.ShowModal()==wxID_CANCEL) return;
        std::string input=dialog.GetSettingsText();
        std::thread t(computeinterpolation,input,std::ref(fstring));
        t.detach();
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
            if (dialog.ShowModal()==wxID_OK) {
                j = indezes[dialog.GetSelection()];
            }
            else return;
        }
        std::thread t(computeregression,std::cref(F[j]),regressiondegree);
        t.detach();
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
        if (ldialog.ShowModal()==wxID_CANCEL) return;
        j=ldialog.GetSelection();
        if (j<0) j=0;
        j = indezes[j];
        Entry = ldialog.GetText();
        std::thread t(analyzefunction,std::cref(F[j]),Entry);
        t.detach();
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
        if (dialog.ShowModal()==wxID_OK) {
            j = indezes[dialog.GetSelection()];
        }
        else return;
    }

    //if (F[j].numberofvariables()>1 || F[j].getmode()!=myfunction::FUNCTION) return;
    //if (isderived(F[j])) return;

    int n=fstring.size()-1;
    if (id==7001 || id ==7007) { // Tangente, Normale
        //double x,m,b;
        int tangent=1;
        std::string type,input;
        if (id==7001) type = "tangent";
        else {
            type = "normal";
            tangent=0;
        }
        MultiLineDialog tangentdialog(this,"","Entry x-value or point",240,2*fontsize+6,"Set Point for " + type,fontsize,1);
        if (tangentdialog.ShowModal()==wxID_CANCEL) return;
        input=tangentdialog.GetSettingsText();
        input+=" ";
        std::thread t(computetangent,input,std::cref(F[j]),x1,x2,tangent);
        t.detach();
        return;
    }
    else if (id==7002) { // Ableitung:
        std::string sf=F[j].getinfixnotation();
        int m=sf.size()-1;
        if (m<0) return;
        if (fstring[n]!=';') fstring+=';';
        if (sf[m]=='\'') fstring += sf + "\'";
        else if (F[j].numberofvariables()==1 && F[j].isdifferentiable()) {
            val::valfunction f(F[j].getinfixnotation()), g = f.derive();
            g.simplify(1);
            fstring += g.getinfixnotation();
        }
        else fstring += "(" + sf + ")" + "\'";
        if (F[j].iswithparameter()) {
            Parameter.inserttoend(val::rational(F[j].getparameter()));
        }
        if (x_range[j].x==x_range[j].y) fstring+=";";
        else if (x_range[j].x!=x1 || x_range[j].y!=x2)
            fstring+="  [ "+val::ToString(x_range[j].x) + " , " + val::ToString(x_range[j].y) + " ]";
        refreshfunctionstring();

    }
    else if (id==7003) { // Table
        if (nchildwindows) return;
        val::rational x1,x2,dx;
        int rat=0;
        MultiLineDialog tabledialog(this,xstring + " ; 0.5" ,"Entry x1,x2,dx:",240,2*fontsize+6,"Set Values for Table",fontsize,1);
        if (tabledialog.ShowModal()==wxID_CANCEL) return;
        std::string s=tabledialog.GetSettingsText();
        if (s=="") return;
        int n=s.size();

        if (s[n-1] == ';') rat = 1;

        val::d_array<char> separ{';', ' '};
        val::Glist<std::string> svalues = getwordsfromstring(s,separ);

        n = svalues.length();
        if (n == 0) return;
        if (n > 0) x1 = val::FromString<val::rational>(svalues[0]);
        if (n > 1) x2 = val::FromString<val::rational>(svalues[1]);
        if (n > 2) dx = val::FromString<val::rational>(svalues[2]);

        if (x1>x2) return;
        if (dx<=val::rational(0)) return;

        //wxMessageBox(val::ToString(x1) + " ; " + val::ToString(x2) + " ; " + val::ToString(dx));
        if (!F[j].israt() || !rat || isderived(F[j])) {
            std::thread t(computetable,std::cref(F[j]),double(x1),double(x2),double(dx));
            t.detach();
            return;
        }
        else {
            std::thread t(computetable_rat,std::cref(F[j]),x1,x2,dx);
            t.detach();
            return;
        }
    }
    else if (id==7004 || id==7005 || id==7008)    { // Integral + Iteration:
        if (id==7005 && isderived(F[j])) return;

        std::string title,param;
        int arclength=0;
        wxPoint Point  = this->GetPosition();

        if (id==7004) title ="Integral";
        else if (id==7005) {title = "Arc Length";arclength=1;}
        else title="zero-iteration";

        param="Entry round-dec / iteration / precision / x1;x2";

        std::string text=val::ToString(dez) + "\n" + val::ToString(iter) + "\n" + val::ToString(delta) + "\n";

        MultiLineDialog integraldialog(this,text,param,240,100,title,fontsize);

        integraldialog.SetPosition(wxPoint(Point.x,Point.y+10));

        if (integraldialog.ShowModal()==wxID_CANCEL) return;
        text=integraldialog.GetSettingsText();

        //int i, n=text.size();
        //std::string s="";
        val::rational xr1,xr2;

        val::d_array<char> sep({'\n', ';', ' '});
        val::Glist<std::string> svalues = getwordsfromstring(text,sep);

        int n = svalues.length();
        if (n != 5) return;
        dez = val::FromString<int>(svalues[0]);
        iter = val::FromString<int>(svalues[1]);
        delta = val::FromString<double>(svalues[2]);
        xr1 = val::FromString<val::rational>(svalues[3]);
        xr2 = val::FromString<val::rational>(svalues[4]);

        if (dez<0) dez = 0;
        if (dez>10) dez=10;
        if (iter<40) iter=40;
        if (iter>500) iter=500;
        if (delta<1e-9) delta=1e-9;
        if (delta>0.1) delta=0.1;

        if (id==7004 || id==7005) {
            std::thread t(computeintegral,std::cref(F[j]),xr1,xr2,delta,iter,dez,arclength);
            t.detach();
            return;
        }
        else {
            std::thread t(computezeroiteration,std::cref(F[j]),xr1,xr2,delta,iter,dez);
            t.detach();
            return;
        }
    }
    GetSettings();
    Compute();
}


void PlotFunctionFrame::OnChangeParmeterMenu(wxCommandEvent & event)
{
    int id=event.GetId();

    if (id==23) {// Set Regression Degree:
        val::MultiLineDialog dialog(this,val::ToString(regressiondegree),"Entry Regression Degree:",240,2*fontsize+6,"Set Regression Degree",fontsize,1,wxTE_RIGHT);
        if (dialog.ShowModal()==wxID_CANCEL) return;
        regressiondegree = val::FromString<int>(dialog.GetSettingsText());
        if (regressiondegree<1) regressiondegree = 1;
        if (regressiondegree>4) regressiondegree = 4;
        return;
    }

    if (id==24) { // Round-decimals for points
        val::MultiLineDialog dialog(this,val::ToString(rounddrawingpoints),"Entry round-decimals: (-1 = no rounding)",240,2*fontsize+6,"Set Round-decimals",fontsize,1,wxTE_RIGHT);
        if (dialog.ShowModal()==wxID_CANCEL) return;
        rounddrawingpoints=val::FromString<int>(dialog.GetSettingsText());
        if (rounddrawingpoints<-2) rounddrawingpoints=-2;
        if (rounddrawingpoints>9) rounddrawingpoints=9;
        return;
    }

    std::string pars,hs;
    int i,n=Parameter.length();

    if (Parameter.isempty()) pars="0";
    else {
        for (i=0;i<n;++i) pars+=val::ToString(Parameter[i]) + ";";
    }

    val::MultiLineDialog pardialog(this,pars,"Entry Values separated by ;",240,2*fontsize+6,"Set Parameter Values",fontsize,1);
    if (pardialog.ShowModal()==wxID_CANCEL) return;
    Parameter.dellist();
    pars=pardialog.GetSettingsText();
    if (pars=="") return;

    val::d_array<char> sep({';', ' '});
    val::Glist<std::string> svalues = getwordsfromstring(pars,sep);

    for (const auto& v : svalues) Parameter.push_back(val::FromString<val::rational>(v));

    GetSettings();
    Compute();
}


void PlotFunctionFrame::OnRangeMenu(wxCommandEvent &event)
{
    int id=event.GetId();
    //std::string entry,info,title,svalue;
    std::string entry,info,title; //svalue;

    if (id==6) {
        entry=xstring;info="Entry x-Values(empty string = default";title="Set x-Range Values";
    }
    else {
        entry=ystring;info="Entry y-Values(empty string = default";title="Set y-Range Values";
    }
    val::MultiLineDialog rangedialog(this,entry,info,240,2*fontsize+6,title,fontsize,1,wxTE_RIGHT);
    if (rangedialog.ShowModal()==wxID_CANCEL) return;

    //svalue=rangedialog.GetSettingsText();
    val::d_array<char> separators{':'};
    val::Glist<std::string> svalue=getwordsfromstring(rangedialog.GetSettingsText(),separators,1);
    val::Glist<double> values;
    double d1 = 0 , d2 = 0;

    separators[0] = ';';
    if (!svalue.isempty())  {
        values = getdoublevaluesfromstring(svalue[0],separators,1);
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

    if (svalue.length()<=1) {
        if (svalue.isempty() || svalue[0] == "") {
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
        if (svalue[1] == "") {
            yset = 0;
            xstring = val::ToString(x1) + ";" + val::ToString(x2);
            Compute();
            return;
        }
        values = getdoublevaluesfromstring(svalue[1],separators,1);
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



void PlotFunctionFrame::OnScaleMenu(wxCommandEvent &event)
{
    std::string svalue = sx_scale +";\n" +sy_scale + ";\n" + val::ToString(abst) +
                        ";\n" + x_axis + "\n" + y_axis;
    val::MultiLineDialog scaledialog(this,svalue,"Entry x-scale-, y-scale-units, margin",240,100,"Scale-Settings",fontsize);
    if (scaledialog.ShowModal()==wxID_CANCEL) return;
    svalue=scaledialog.GetSettingsText();
    if (svalue=="") return;

    int n;

    val::d_array<char> separators{';','\n'};
    val::Glist<std::string> words = getwordsfromstring(svalue,separators);

    n = words.length();
    x_axis=y_axis="";

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
    if (n>2) {
        abst = val::FromString<int>(words[2]);
        if (abst<10) abst=10;
        if (abst>60) abst=60;
    }
    if (n>3) x_axis = words[3];
    if (n>4) y_axis = words[4];

    Paint();
}

void PlotFunctionFrame::OnGridMenu(wxCommandEvent &event)
{
    std::string svalue = sgx_scale +";" + sgy_scale;
    val::MultiLineDialog griddialog(this,svalue,"Entry x-Grid-scale-, y-Grid-scale-units",240,2*fontsize+6,"Grid-Settings",fontsize,1);
    if (griddialog.ShowModal()==wxID_CANCEL) return;
    svalue=griddialog.GetSettingsText();
    if (svalue=="") return;

    int i,n=svalue.size();
    std::string swert;
    for (i=0;i<n;++i) {
        if (svalue[i]==';' || i==n-1) {
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


void PlotFunctionFrame::OnMoveMenu(wxCommandEvent &)
{
    std::string svalue = val::ToString(movedx);
    if (moveinpointsx) svalue +=" p";
    svalue += " ; " + val::ToString(movedy);
    if (moveinpointsy) svalue += " p";
    val::MultiLineDialog movedialog(this,svalue,"Entry x-increment ; y-increment [p for pixels]",240,2*fontsize+6,"Move Increments",fontsize,1);
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
    if (event.GetId()==IdInfo) {
        wxMessageBox(event.GetMessage());
        return;
    }

    if (event.GetId()==IdPaint) {
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

    if (event.GetId()==IdRefresh) {
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


    if (event.GetId()==IdTable) { // Tafel
        //std::string s = ToString(x) + " , " + ToString(y) + " ; " + ToString(dx) + " , " + ToString(dy);
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
        InfoWindow *tablewindow = new InfoWindow(this,nchildwindows,tablestring,Point,Size,"Table",fontsize);
        tablewindow->Show();
    }
    else if (event.GetId()==IdAnalyze) {
        sx=250;
        sy=110;

        x+=dx;
        if (x+sx+23>maxx) {
            //wxMessageBox(val::ToString(maxx));
            x-=maxx-(x+sx+23);
        }
        //x+=dx - sx;
        Point.x = x; Point.y = y;
        //Size.SetWidth(300);
        //Size.SetHeight(dy);
        //wxMessageBox("Hier");
        //InfoWindow *tablewindow = new InfoWindow(this,nchildwindows,tablestring,Point,Size,"Analyze function",fontsize);
        //tablewindow->Show();
        AnalysisDialog *adialog = new AnalysisDialog(this,nanalyzewindows,analyze_output,Points,wxSize(sx,sy),Point);
        adialog->Show();
    }
    else if (event.GetId()==IdIntegral) { // Integral
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
        // Tell memDC to write on ???paper???.
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
            entry ="text {} "  + val::ToString(x) + " " + val::ToString(y) + " ";
            title = "Draw Text";
            descr = "Entry Text";
        }

        val::MultiLineDialog cdialog(this,entry,descr,240,fontsize+15,title,fontsize,1);

        if (cdialog.ShowModal()==wxID_OK) {
            if (fstring[n]!=';') fstring+=";";
            fstring+=cdialog.GetSettingsText();
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
        wxSize MySize=GetClientSize();
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
#ifdef _WIN32
        for (i=0;i<m;++i) {
            file<<Font[i].GetNativeFontInfoDesc()<<std::endl;
        }
#else
        for (i=0;i<m;++i) {
            file<<Font[i].GetNativeFontInfoDesc()<<std::endl;
        }
#endif // _WIN32
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
                int dx, dy;

                px = (x2-x1)*double(x-abst)/double(sizex -1) + x1;
                py = double(yzero-y)*(y2-y1)/double(sizey-1);
                F[i].getCirclePoints(mx,my,r,a1,a2);
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
                std::string nf = "circle ";

                F[i].getCirclePoints(x,y,r,a1,a2);
                nf += val::ToString(x+dx) + " " + val::ToString(y+dy) + " " + val::ToString(r) + " " + val::ToString(a1) + " " + val::ToString(a2);
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
             polygonpointsmenu->Check(true);
             DrawPanel->SetCursor(wxCursor (wxCURSOR_ARROW));
             DrawPanel->Bind(wxEVT_MOTION,&PlotFunctionFrame::OnMouseMoved,this);
             drawline = 1;
    }
    else {
             SendNotification("Insert Line: OFF");
             StatusBar1->SetStatusText(_T(""),1);
             polygonpointsmenu->Check(false);
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
    val::InfoWindow *notification = new val::InfoWindow(this,nchildwindows,s,wxDefaultPosition,wxSize(300,100),"Info",20,0);
    notification->Show();
    for (int i=0;i<1000;++i) wxYield();
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
    if (!yset) return;
    if (iscomputing) return;

    //double dx = 0.1, dy = 0.1;

    switch (event.GetId())
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
        displacefunction(active_function,movex,movey);
        fstring = "";
        int i = 0;
        for (const auto & v : F) {
            if (x_range[i].x == x_range[i].y) fstring += v.getinfixnotation() + ";\n";
            else fstring += v.getinfixnotation() + "[" + val::ToString(x_range[i].x) + " , " + val::ToString(x_range[i].y) + "];\n";
            ++i;
        }
        refreshfunctionstring();
        movex = movey = 0;
        // Change menu-text
        {
            std::string tf = "", tc = "", s = F[active_function].getinfixnotation();
            if (active_function<=8) {
                tf = "\tAlt-" + val::ToString(active_function+1);
                tc = "\tCtrl-" + val::ToString(active_function+1);
            }
            //else t="";
            if (s.length()>40) {
                s.resize(40);
                s+="...";
            }
            //s+=t;
            f_menu[active_function]->SetItemLabel(s + tf);
            c_menu[active_function]->SetItemLabel(s + tc);
        }
        iscomputing=1;
        //std::cout<<"\n F = "<<F[active_function].getinfixnotation()<<std::endl;
        //GetSettings();
        Compute(active_function);
    }
    else {
        //double dx1 = (x2-x1)*double(movex)/double(sizex), dy1 = (y2-y1)*double(movey)/double(sizey);
        //x1-=dx1; x2-=dx1; y1+=dy1; y2+=dy1;
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
    if (event.GetId() == 20007) {
        if (N==0) return;
        int i, j = active_function,n;

        if (j != -1 && F[j].IsPoints()) {
            int i_f = 0;
            for (i = 0; i < j; ++i) {
                if (F[i].numberofvariables() == 1) ++i_f;
            }
            n = farray[i_f].length();
            //wxMessageBox(val::ToString(n));
            pointactive += 2;
            if (pointactive < n) {
                iscomputing = 1;
                Paint();
                return;
            }
            else pointactive = 0;
        }

        j = active_function;


        val::d_array<int> checked;
        checked.reserve(N);
        for (i=0;i<N;++i) {
            if (f_menu[i]->IsChecked() && !F[i].IsFill()) checked.push_back(i);
        }
        n = checked.length();
        if (n == 0) return;
        ++j; j %= N;
        for (i = 0; i < n; ++i) {
            if (j <= checked[i]) break;
        }
        active_function = checked[i%n];
        StatusBar1->SetStatusText(F[active_function].getinfixnotation());
        iscomputing = 1;
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



/*


void PlotFunctionFrame::OnMouseDown(wxMouseEvent &event)
{
    DrawPanel->CaptureMouse();
    mouse_x1=event.GetX();mouse_y1=event.GetY();
    //StatusBar1->SetStatusText(val::ToString(mouse_x1) + " , " + val::ToString(mouse_y1));
}


void PlotFunctionFrame::OnMouseUp(wxMouseEvent &event)
{
    mouse_x2=event.GetX();mouse_y2=event.GetY();

    //wxMessageBox(val::ToString(mouse_x1) + " , " + val::ToString(mouse_y1) + "     " + val::ToString(mouse_x2) + " , " + val::ToString(mouse_y2));
    StatusBar1->SetStatusText(val::ToString(mouse_x1) + " , " + val::ToString(mouse_y1) + "     " + val::ToString(mouse_x2) + " , " + val::ToString(mouse_y2));
    //ReleaseMouse();
    DrawPanel->ReleaseMouse();
}
*/


