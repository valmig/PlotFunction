/***************************************************************
 * Name:      PlotFunctionApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Miguel Valbuena ()
 * Created:   2018-09-28
 * Copyright: Miguel Valbuena ()
 * License:
 **************************************************************/

#include "PlotFunctionApp.h"

//(*AppHeaders
#include "PlotFunctionMain.h"
#include <wx/image.h>
//*)
#include <iostream>
#include "PlotFunction.h"

void program_info()
{
 std::cout<<"\nCommand: PlotFunction function(s) [x1;x2] [y1;y2] ";
 std::cout<<"\n=======";
 std::cout<<"\nPlots functions in the optional range [x1;x2] [y1;y2]\n";
}


IMPLEMENT_APP(PlotFunctionApp)

bool PlotFunctionApp::OnInit()
{
    /*
    if (argc==1) {
        program_info();
        exit(0);
    }
    */
    if (argc>=2) {
        fstring=std::string(argv[1]);
        if (fstring == "-h" || fstring == "--help") {
            program_info();
            exit(0);
        }
    }
    if (argc>=3) {
        xstring=std::string(argv[2]);
    }
    if (argc>=4) {
        ystring=std::string(argv[3]);
    }
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	PlotFunctionFrame* Frame = new PlotFunctionFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
