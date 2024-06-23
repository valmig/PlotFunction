#include "PlotFunction.h"
#include <Glist.h>
#include <fraction.h>
#include <valfunction.h>
#include <val_basics.h>
#include <val_utils.h>
#include <rational.h>
#include <vector.h>
#include <wx/msgdlg.h>
#include <LA.h>
#include <polfactor.h>
#include <ideal_roots.h>
#include <analysis.h>
#include <pol_arithmetic.h>


wxFrame *MyFrame=nullptr;
wxString tablestring="";
std::string fstring="",xstring="-5;5",ystring="-5;5",sizestring="",openfiledir= val::CurrentHomeDir(),
            savefiledir=val::CurrentHomeDir(), ansexpr = "";

#ifdef _WIN32
std::string filesep="\\", filedir=val::CurrentHomeDir(), valdir = val::CurrentHomeDir()+"\\AppData\\Roaming\\MVPrograms",
            settingsdir = valdir + "\\PlotFuncion", settingsfile=settingsdir + "\\settings.txt",
            iconpath="C:\\gccprogrammes\\Plot\\PlotFunction\\icon\\MV_plot.xpm",
            handcursor="C:\\gccprogrammes\\Plot\\PlotFunction\\icon\\hand.png",
            alticonpath = val::GetExeDir() + "\\MV_plot.xpm",
    		errorfile = settingsdir + "\\error.log";
#endif // _WIN32
#ifdef __LINUX__
std::string filesep="/",filedir=val::CurrentHomeDir(), valdir= val::CurrentHomeDir() + "/.config/MVPrograms",
            settingsdir = valdir + "/PlotFunction", settingsfile=settingsdir + "/settings.conf",
            iconpath = val::CurrentHomeDir() + "/.local/share/icons/MVPrograms/MV_Plot.xpm",
            handcursor = val::CurrentHomeDir() + "/.local/share/icons/MVPrograms/hand.png",
            alticonpath = val::CurrentHomeDir() + "/.local/share/icons/MVPrograms/MV_Plot.xpm",
    		errorfile = settingsdir + "/error.log";
#endif
#ifdef __APPLE__
std::string filesep="/",filedir=val::CurrentHomeDir(), valdir= val::CurrentHomeDir() + "/Library/Application Support",
            settingsdir = valdir + "/PlotFunction", settingsfile=settingsdir + "/settings.conf",
            iconpath = val::CurrentHomeDir() + "/.local/share/icons/MVPrograms/MV_Plot.xpm",
            handcursor = val::CurrentHomeDir() + "/.local/share/icons/MVPrograms/hand.png",
            alticonpath = val::GetExeDir() + "/../Resources/MV_Plot.xpm",
    		errorfile = settingsdir + "/error.log";
#endif //

std::string RecentFilesPath = settingsdir + filesep + "recentfiles.txt";
std::string RecentCommandsPath = settingsdir + filesep + "recentcommands.txt";

std::mutex compute_mutex;
//std::atomic<int> iscomputing(0);

plotobject global_function;

//val::d_array<val::Glist<val::GPair<double>>> critpoints;// undef_intervals;
//val::d_array<val::d_array<double>> critx;
val::d_array<wxString> analyze_output(4);
val::d_array<val::d_array<val::GPair<double>>> Points(3);
val::Glist<wxString> recentcommands;


const val::d_array<wxString> greek_literals{"\\alpha", "\\beta", "\\gamma", "\\delta", "\\epsilon", "\\zeta", "\\eta", "\\theta", "\\iota", "\\kappa", "\\lambda", "\\mu", "\\nu",
                                            "\\xi", "\\omicron", "\\pi", "\\rho", "\\sigma", "\\tau", "\\phi", "\\chi", "\\psi", "\\omega", "\\Gamma", "\\Delta", "\\Pi",
                                            "\\Sigma", "\\Phi", "\\Psi", "\\Omega"};

const val::d_array<wxString> greek_letters{L"\u03B1", L"\u03B2", L"\u03B3", L"\u03B4", L"\u03B5",  L"\u03B6", L"\u03B7", L"\u03B8", L"\u03B9", L"\u03BA", L"\u03BB", L"\u03BC", L"\u03BD",
                                           L"\u03BE", L"\u03BF", L"\u03C0", L"\u03C1", L"\u03C3", L"\u03C4", L"\u03C6", L"\u03C7", L"\u03C8", L"\u03C9", L"\u0393", L"\u0394", L"\u03A0",
                                           L"\u03A3", L"\u03A6", L"\u03A8", L"\u03A9"};

const val::d_array<wxString> WordList{"PI", "exp", "log", "line", "sinh", "sqrt", "cosh", "circle", "tanh", "text", "triangle", "polygon", "points",
                                   "inf", "fill", "abs", "arcsin", "arccos", "arctan", "rectangle", "sin", "cos", "tan",
                                   "arsinh", "arcosh", "artanh", "bitmap"};

/*
val::d_array<std::string> sfunctionlist({"sqrt", "exp", "log", "abs", "sinh", "cosh", "tanh", "arsinh", "arcosh", "artanh",
										"sin", "cos", "tan", "arcsin", "arccos", "arctan", "inf", "PI", "line", });
*/
/*
val::d_array<val::GPair<std::string,val::d_function*>>
functionpairs ({ {"sqrt",val::sqrt}, {"abs",val::abs}, {"exp", val::exp}, {"log", val::log}, {"sin", val::sin}, {"cos", val::cos},
            {"tan", val::tan}, {"arcsin", val::arcsin}, {"arccos", val::arccos}, {"arctan", val::arctan}, {"sinh", val::sinh},
            {"cosh", val::cosh}, {"tanh", val::tanh}, {"arsinh", val::arsinh}, {"arcosh", val::arcosh}, {"artanh", val::artanh}});
*/

const val::d_array<wxColour> defaultcolors{wxColour(0,0,255), wxColour(255,0,0), wxColour(0,255,0), wxColour(173,216,230),
                                      wxColour(255,116,0), wxColour(238,0,255), wxColour(125,125,125), wxColour(255,255,255),
                                      wxColour(0,0,0), wxColour(191,191,191), wxColour(255,255,0), wxColour(0,128,0), wxColour(245,245,220), wxColour(165,42,42),
                                      wxColour(0,0,128), wxColour(255,192,203), wxColour(135,206,235), wxColour(144,238,144)};

const val::d_array<wxString> defaultcolornames{"blue", "red", "green", "lblue", "orange", "violet", "grey", "white", "black",
                                "lgrey", "yellow", "dgreen", "beige", "brown", "dblue", "pink", "skyblue", "lgreen" };

const val::d_array<wxString> SettingsList({"axis-scale", "axis-color", "grid-scale", "grid-color", "values-number", "axis-range", "show-x-axis",
                                "show-y-axis", "show-grid" , "show-x-scale", "show-y-scale" , "reset-colors", "font-size", "function-color", "panel-size", "axis-names", "regression-degree",
                                "point-decimals", "show-function", "background-color", "parameter-values", "function-size", "margin",
                                "axis-fontsize", "function-settings", "move-increment" });

const val::d_array<wxString> SettingsParList({"axis-scale sx [sy]	<Shift-Alt-S>",
                                              "axis-color def. color / Red Green Blue	<Shift-Ctrl-A>",
                                              "grid-scale sx [sy]	<Ctrl-G>",
                                              "grid-color def. color / Red Green Blue	<Shift-Ctrl-G>",
                                              "values-number unsigned int",
                                              "axis-range x1 x2 [: y1 y2]		<Ctrl-X>/<Ctrl-Y>",
                                              "show-x-axis 0/1	<Alt-X>",
                                              "show-y-axis 0/1	<Alt-Y>",
                                              "show-grid 0/1	<Alt-G>",
                                              "show-x-scale 0/1		<Shift-Alt-X>",
                                              "show-y-scale 0/1		<Shift-Alt-Y",
                                              "reset-colors		<Ctrl-Alt-R>",
                                              "font-size unsigned int	<Ctrl-F>",
                                              "function-color [#nr = 1] def. color / Red Green Blue    <Ctrl-nr>",
                                              "panel-size sx [sy]    <F8>",
                                              "axis-names string string    <Shift-Alt-S>",
                                              "regression-degree unsigned int \t<Shift-Alt-A>",
                                              "point-decimals int    <Ctrl-D>",
                                              "show-function [#nr = 1] 0/1    <Alt-nr>",
                                              "background-color def. color / Red Green Blue    <Shift-Ctrl-B>",
                                              "parameter-values double ...    <Ctrl-P>",
                                              "function-size [#nr = 1] unsigned int    <Ctrl-nr>",
                                              "margin unsigned int    <Shift-Alt-S>",
                                              "axis-fontsize unsigned int    <Shift-Ctrl-F>",
                                              "function-settings [nr=1]",
                                              "move-increment dx [dy=dx] (p for pixels)    <Ctrl-M>"
                                             });

const val::d_array<wxString> CommandsList({"derive", "analyze", "tangent", "normal", "interpolation", "regression", "table", "integral",
                                             "arclength", "zero-iteration", "move", "evaluate", "intersection", "calculate", "rotate" });

const val::d_array<wxString> CommandsParList({"derive [#nr = 1]",
                                                 "analyze [#nr = 1] [x1 x2] [prec = 1e-09] [iterations] [decimals]    <Ctrl-A>",
                                                 "tangent [#nr = 1] x / x y    <Alt-T>",
                                                 "normal [#nr = 1] x / x y    <Shit-Alt-N>",
                                                 "interpolation #nr / points for f; [points for f']; [points for f'']    <Ctrl-I>" ,
                                                 "regression #nr [ = 1]/points [degree = regression-degree]    <Alt-A>",
                                                 "table [#nr = 1] [x1 x2] [dx = 0.5] [;]    <Ctrl-T>",
                                                 "integral [#nr = 1] [decimals iterations precision] [x1 x2]    <Alt-I>",
                                                 "arclength [#nr = 1] [decimals iterations precision] [x1 x2]    <Shift-Alt-I>",
                                                 "zero-iteration [#nr = 1] [decimals iterations precision] [x1 x2]    <Alt-Z>",
                                                 "move [#nr = 1] x y",
                                                 "evaluate [#nr = 1] expressions [ddecimals[ = 4 ]]",
                                                 "intersection [#nr1 = 1] #nr2 [x1 x2] [prec] [iterations] [decimals]",
                                                 "calculate arithmetic expression",
                                                 "rotate [#nr = 1] angle (in degrees ) x y (center)"
                                                 });



//const val::d_array<std::string> InputDialogList(SettingsList);

const val::trie_type<wxString> WordTree(WordList + defaultcolornames, 58, int('A'));


const val::trie_type<wxString> InputDialogTree(SettingsList + CommandsList + defaultcolornames, 78, int('-'));


wxDEFINE_EVENT(MY_EVENT,MyThreadEvent);

// ------------------------------------------------------------------------------------------------------------------------------
namespace val
{
// Functions for computation of critical pairs of plane algebraic curves:
int operator <(const val::GPair<double>& p,const val::GPair<double>& q)
{
    if (p.x<q.x) return 1;
    else if (p.x>q.x) return 0;
    else return (p.y<q.y);

}
}


// -----------------------------------------------------------------------------

void inserttocand(val::Glist<val::GPair<int>> &cand,const val::GPair<int> &q,int y)
{
    if (cand.isempty()) {
        cand.push(q);
    }
    else if (val::abs(q.y - y) < val::abs(cand[0].y - y)) cand.push(q);
    else if (cand.length()==1) cand.push_back(q);
    else if (val::abs(q.y - y) < val::abs(cand[1].y - y)) cand[1] = q;
    return;
}


// ------------------------------------------------------------------------------



int isinList(const val::Glist<val::GPair<double>> &P,const val::GPair<double> &q,const double& eps=1e-9)
{
    for (const auto p : P){
        if (val::abs(p.x-q.x)<eps && val::abs(p.y-q.y)<eps) return 1;
        else if (q<p) return 0;
    }
    return 0;
}


val::pol<double> eval_f(const val::s_polynom<val::integer>& f,const double &x,int i=1)
{
    val::pol<double> g;
    for (const auto& monom : f) {
        if (i==0) g.insert(double(monom.actualcoef())*val::power(x,monom.actualterm()[0]),monom.actualterm()[1]);
        else g.insert(double(monom.actualcoef())*val::power(x,monom.actualterm()[1]),monom.actualterm()[0]);
    }
    return g;
}

val::Glist<val::GPair<double>> critical_points_pl_alg_curve(const val::valfunction& f)
{
    val::Glist<val::GPair<double>> crit_points;
    if (f.is_zero() || f.numberofvariables()!=2) return crit_points;

    val::valfunction g;
    int i,j,oldordn=val::n_expo::getordtype(), oldordns=val::s_expo::getordtype();
    val::Glist<val::s_polynom<val::integer>> G;
    val::s_polynom<val::integer> h,h1;
    val::matrix<int> OM_s=val::s_expo::getordmatrix(), OM_n=val::n_expo::getordmatrix();
    val::vector<double> roots,rootsy;
    val::GPair<double> Paar;

    val::n_polynom<val::rational>::setstaticexpodim(2);
    val::n_expo::setordtype(-1);
    val::s_expo::setordtype(-1);
    h=val::primitivpart(f.gets_polynom());
    g=f.derive(2);
    h1=val::primitivpart(g.gets_polynom());
    //h.reord();
    //h1.reord();
    G.sinsert(std::move(h));
    G.sinsert(std::move(h1));
    val::primitiv_groebner(G);
    // Check if is 0-dimensional!
    if (G.length()!=2) return crit_points;
    //
    val::realRoots(val::ToDoublePolynom(val::To_unipol(G[0],1)),rootsy,1e-9);
    for (i=0;i<rootsy.dimension();++i) {
        val::realRoots(eval_f(G[1],rootsy[i],1),roots,1e-9);
        for (j=0;j<roots.dimension();++j) {
            Paar.x=roots[j]; Paar.y=rootsy[i];
            if (!isinList(crit_points,Paar,1e-4)) crit_points.sinsert(Paar);
        }
    }


    val::s_expo::setordtype(oldordns);
    val::n_expo::setordtype(oldordn);
    val::n_expo::setordmatrix(OM_n);
    val::s_expo::setordmatrix(OM_s);
    return crit_points;
}

val::d_array<double> critical_x_values(const val::Glist<val::GPair<double>>& crit_pair)
{
    val::d_array<double> crit_x;
    int i;

    crit_x.reserve(crit_pair.length());
    i=0;
    for (const auto &p : crit_pair) {
        if (i>0 && val::abs(p.x-crit_x[i-1])<=1e-5)  continue;
        else {
            crit_x[i] = p.x;
            ++i;
        }
    }

    return crit_x;
}

// --------------------------------------------------------------------------------------------

// x > 0
int decimal(const double& x)
{
    int k=0;
    double factor = 1;

    while (x<factor) {
        k++;
        factor*=0.1;
    }
    return k;
}


int intdigits(double x)
{
    x = val::abs(x);
    if (x == 0.0) return 1;
    return int(val::log(10.0,x)) + 1;
}

std::string head(const std::string &s,int l)
{
    std::string output="";
    int n = val::Min(int(s.length()),l);
    if (n<=0) return output;
    for (int i =0;i<n;++i) output+=s[i];
    return output;
}


std::string delcharfromstring(const std::string& s,const char z)
{
    int n = s.length();
    std::string out="";

    for (int i=0;i<n;++i) {
        if (s[i]!=z) out+=s[i];
    }
    return out;
}


double squaredistance(const wxPoint& l1, const wxPoint &l2, const wxPoint &p)
{
    double a,b,c,d,distanceline=0,distancepoint=0;
    wxPoint minpoint, maxpoint;

    a = -(double(l2.y) -double(l1.y));
    b = double(l2.x) - double(l1.x);
    c = double(l1.x) * a + double(l1.y) * b;
    d = double(p.x) * a + double(p.y) * b;


    //wxMessageBox(val::ToString(a) + ", " + val::ToString(b) + ", " + val::ToString(c)+ ", " + val::ToString(d));
    //wxMessageBox(val::ToString(l1.x) + ", " + val::ToString(l1.y) + ", " + val::ToString(l2.x)+ ", " + val::ToString(l2.y));

    distanceline = ((c-d)*(c-d))/(a*a + b*b);

    // distance between p and endpoint:

    if (l1.x < l2.x) {
        if (p.x < l1.x) {
            a = p.x - l1.x; b = p.y - l1.y;
            distancepoint = a*a + b*b;
        }
        else if (p.x > l2.x) {
            a = p.x - l2.x; b = p.y - l2.y;
            distancepoint = a*a + b*b;
        }
    }
    else if (l1.x > l2.x) {
        if (p.x > l1.x) {
            a = p.x - l1.x; b = p.y - l1.y;
            distancepoint = a*a + b*b;
        }
        else if (p.x < l2.x) {
            a = p.x - l2.x; b = p.y - l2.y;
            distancepoint = a*a + b*b;
        }
    }
    else if (l1.y < l2.y) {
        if (p.y < l1.y) {
            a = p.x - l1.x; b = p.y - l1.y;
            distancepoint = a*a + b*b;
        }
        else if (p.y > l2.y) {
            a = p.x - l2.x; b = p.y - l2.y;
            distancepoint = a*a + b*b;
        }
    }
    else if (l1.y > l2.y) {
        if (p.y > l1.y) {
            a = p.x - l1.x; b = p.y - l1.y;
            distancepoint = a*a + b*b;
        }
        else if (p.y < l2.y) {
            a = p.x - l2.x; b = p.y - l2.y;
            distancepoint = a*a + b*b;
        }
    }
    return val::Max(distanceline,distancepoint);
}

double squaredistance(const wxPoint& m, const wxPoint &p, int rx, int ry, int angle)
{
    double alpha = double(angle) * val::PI / 180.0, x, y, dx, dy;
    x = double(m.x) + double(rx) * val::cos(alpha);
    y = double(m.y) - double(ry) * val::sin(alpha);
    wxMessageBox(val::ToString(angle) + " , " + val::ToString(alpha) + " , " + val::ToString(x) + " , " + val::ToString(y)
                  + " , " + val::ToString(p.x) + " , " + val::ToString(p.y));
    dx = double(p.x) -x;
    dy = double(p.y) -y;
    return dx*dx + dy*dy;
}


val::GPair<double> coordinatesdistance(const double &mx, const double &my, const double &px, const double &py,const double &r, int angle)
{
    double alpha = double(angle) * val::PI / 180.0, x, y;
    x = mx + r * val::cos(alpha);
    y = my + r * val::sin(alpha);
    return val::GPair<double>(val::abs(px-x),val::abs(py-y));
}


double degree_angle(const double &a, const double &b)
{
    if (b>=0) return val::arccos(a/val::sqrt(a*a + b*b)) * 180.0 / val::PI;
    else return  360 - val::arccos(a/val::sqrt(a*a + b*b)) * 180.0 / val::PI;
}



int getpiscale(const std::string& is,val::rational &factor,double &scale, int pos)
{
    int i,n=is.length(),found=0,neg=0;
    std::string s,sf;

    if (!n) return 0;
    if (is[0] == '-') {
        neg = 1;
        for (i = 1; i < n; ++i ) s += is[i];
    }
    else s = is;

    n = s.length();

    for (i=0;i<n-1;++i) {
        if (s[i]=='P' && s[i+1] == 'I') {
            found=1;
            break;
        }
    }
    if (!found) return 0;
    ++i;
    /*
    found=0;
    for (;i<n;++i) {
        if (s[i]=='I') {
            found=1;
            break;
        }
    }
    if (!found) return 0;
    */
    factor=val::rational(1);
    if (i==1 && n>=3) {
        for (i=3;i<n;++i) {
            sf+=s[i];
        }
        factor/=val::abs(val::FromString<val::rational>(sf));
    }
    else if (i>1) {
        n=i-1;
        for (i=0;i<n;++i) sf+=s[i];
        factor = val::abs(val::FromString<val::rational>(sf));
    }

    if (factor.iszero()) factor = val::rational(1);
    if (neg && !pos) factor.changesign();
    scale = double(factor) * val::PI;

    return 1;
}


int isInf(const double  &a)
{
    if (a==val::Inf || a== -val::Inf) return 1;
    else return 0;
}


void gettangentvalues(const plotobject &f,const double &x,double &m,double &b,int tangent)
{
    //val::DoubleFunction F(val::doublefunction(std::bind(std::cref(f),std::placeholders::_1)));
    //int n=isderived(f);
    //for (i=0;i<n;++i) F=F.derive();

    m= derive(f,x);  //F.derive(x);
    if (!tangent) {
        if (val::abs(m)<=1e-9) m = val::Inf;
        else m = -1/m;
    }
    b=derive(f,x) -m*x;
    return;
}


std::string extractstringfrombrackets(std::string &sf,const char lb, const char rb)
{
    std::string s = "", sb = "";
    int i, n = sf.length(), nbrackets = 0;

    if (n == 0) return sb;
    for (i = 0; i < n; ++i) {
        if (sf[i] == lb) {
            ++nbrackets;
            break;
        }
        s += sf[i];
    }
    for (++i; i < n ; ++i) {
        if (sf[i] == rb) {
            --nbrackets;
            if (!nbrackets) break;
        }
        if (sf[i] == lb) ++nbrackets;
        sb += sf[i];
    }
    for (++i; i < n; ++i) s += sf[i];
    sf = s;
    return sb;
}


std::string getstringfrombrackets(const std::string &sf, const char lb, const char rb)
{
    int n = sf.length(), i, nbrackets = 0;
    std::string sb ="";

    if (n == 0) return sb;
    for (i = 0; i < n; ++i) {
        if (sf[i] == lb) {
            ++nbrackets;
            break;
        }
    }
    for (++i; i < n; ++i) {
        if (sf[i] == rb) {
            --nbrackets;
            if (!nbrackets) break;
        }
        if (sf[i] == lb) ++nbrackets;
        sb += sf[i];
    }
    return sb;
}



int getfunctionfromstring(std::string &fstring, plotobject &f)
{
    val::rational factor;
    std::string ns="";
    int i, colorindex = -1;
    //int i, n, colorindex = -1;

    ns = extractstringfrombrackets(fstring,'<','>');

    i = 0;
    for (const auto& v : defaultcolornames) {
        if (v == ns) {
            colorindex = i;
            break;
        }
        ++i;
    }
    f = plotobject(fstring);

    return colorindex;
}


val::Glist<double> getdoublevaluesfromstring(const std::string &sf,const val::d_array<char>& separators, int emptystring)
{
    val::Glist<double> values;
    std::string s="";
    int n = sf.length();

    for (int i = 0; i < n ; ++i) {
        if (val::isinContainer(sf[i],separators)) {
            if (emptystring || s != "") values.push_back(double(val::FromString<val::rational>(s)));
            //if (s != "") values.push_back(double(val::FromString<val::rational>(s)));
            s = "";
        }
        else s += sf[i];
    }
    if (emptystring || s != "") values.push_back(double(val::FromString<val::rational>(s)));
    return values;
}


val::Glist<std::string> getwordsfromstring(const std::string &sf,const val::d_array<char>& separators,int emptywords,
                                           const val::d_array<char> &ignore)
{
    val::Glist<std::string> values;
    std::string s="";
    int n = sf.length();

    for (int i = 0; i < n ; ++i) {
        if (val::isinContainer(sf[i],ignore)) continue;
        if (val::isinContainer(sf[i],separators)) {
            if (emptywords || s != "") values.push_back(s);
            //else if (s != "") values.push_back(s);
            s = "";
        }
        else s += sf[i];
    }

    if (emptywords || s!= "") values.push_back(s);

    return values;
}



int isderived(const std::string &sf)
{
    if (sf=="") return 0;
    int n=sf.size()-1,anz=0;
    //if (sf[n]=='\'') return 1;
    //else return 0;
    while (n>=0 && (sf[n]=='\'' || sf[n]==' ')) {
        if (sf[n]=='\'') ++anz;
        --n;
    }

    return anz;
}

int isinarray(const val::rational& value,const val::d_array<val::rational> & x)
{
    for (const auto& v : x)
        if (v==value) return 1;
    return 0;
}


/*
int isderived(const plotobject &f)
{
    return isderived(f.getinfixnotation());
}


int islinearfunction(const plotobject &F)
{
    val::valfunction f(F.getinfixnotation());
    return f.islinearfunction();
}
*/


double derive(const plotobject &F,const double& x)
{
    //if (n<=0) return f(x);

    double xh1,xh2,hh,y;
    if (val::abs(x)>1) y=val::abs(x);
    else y=1.0;
    hh=1e-2*y;

    xh1=x+hh;
    xh2=x-hh;
    hh=xh1 - xh2;

    //if (n==1) {
        return (F.f(xh1)-F.f(xh2))/hh;
    //}
    //else return (derive(f,xh1,n-1)-derive(f,x,n-1))/(xh1-x);
}


val::rationalfunction derive(const val::rationalfunction &f)
{
    val::pol<val::rational> nom = f.nominator().derive()*f.denominator()-f.nominator()*f.denominator().derive(), denom = f.denominator() * f.denominator();
    val::rational contnom = val::content(nom), contdenom = val::content(denom);
    nom /= contnom; denom /= contdenom;
    contnom /= contdenom;
    nom *= contnom;
    return val::rationalfunction(nom,denom);
}


val::matrix<val::rational> set_les(const std::string &s)
{
    int n=s.length(),i=0,j,k=0,n1=0,n2=0,l;
    val::matrix<val::rational> A;
    if (n==0) return A;
    val::d_array<val::d_array<val::rational>> x(3),y(3);
    val::d_array<val::rational>::set_plus_cap(10);
    val::d_array<int> dim(0,3);
    std::string svalue="";
    val::rational value;

    j=0;
    for (i=0;i<n;++i) {
        if (s[i]==' ' || s[i]== '\n' || s[i] == ';') {
            if (s[i]==' ' || s[i] == '\n') {
                while (i<n && (s[i] == ' ' || s[i]=='\n')) ++i;
                --i;
            }
            j%=2;
            if (svalue!="") {
                value=val::FromString<val::rational>(svalue);
                svalue="";
                if (!j) {
                    if (!isinarray(value,x[k])) {
                        x[k].push_back(value);
                        ++j;
                    }
                }
                else {
                    y[k].push_back(value);
                    ++j;
                }
            }
            if (s[i]==';') {
                while (i<n && s[i]==';') {++k;++i;}
                --i;
                if (k>2) k=2;
            }
        }
        else svalue+=s[i];
    }

    value=val::FromString<val::rational>(svalue);
    if (!j) {
        if (!isinarray(value,x[k])) {
            x[k].push_back(value);
            ++j;
        }
    }
    else {
        y[k].push_back(value);
        ++j;
    }

    dim[1]=n1= val::Min(x[1].length(),y[1].length());
    dim[2]=n2= val::Min(x[2].length(),y[2].length());
    dim[0]=n= val::Min(x[0].length(),y[0].length());
    n+=n1+n2;
    if (n2) n = val::Max(n,n2+2);
    if (n1) n = val::Max(n,n1+1);
    if (n==0) return A;
    //std::cout<<n<<"  "<<dim[0]<<"  "<<dim[1]<<"  "<<dim[2]<<std::endl;
    A = val::matrix<val::rational>(n,n+1);
    for (k=0,l=0;k<3;++k) {
        for (i=0;i<dim[k];++i,++l) {
            for (j=k;j<n;++j) {
                A(l,j) = val::power(x[k][i],j-k);
                if (k>=1) A(l,j) *= val::rational(j);
                if (k==2) A(l,j) *= val::rational(j-1);
            }
            A(l,n) = y[k][i];
        }
    }
    return A;
}



val::pol<val::rational> interpolation(const std::string &s)
{
    val::matrix<val::rational> X,A=set_les(s);
    val::pol<val::rational> f;
    val::rational det;
    int dim;

    if (A.numberofcolumns()<2) return f;
    dim=val::les(A,X,det);
    if (dim==0) return f;
    for (int i=0;i<X.numberofcolumns();++i) f.insert(X(0,i),i);
    return f;
}

val::rational eval(const val::rationalfunction &F,const val::rational &x)
{
    return F.nominator()(x)/F.denominator()(x);
}



// --------------------------------------------------------------------------------------------------------------------------------------------------------



void computepoints(val::Glist<plotobject> &F,int points,const double &x1,const double &x2,double &ymax,double &ymin,int activef, int comppoints)
{
    MyThreadEvent event(MY_EVENT,IdPaint);

    if (F.isempty()) {
        if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
        return;
    }

    int m=F.length(),i,j;// nrf = 0, i_rf = 0;
    val::pol<double> p;
    val::vector<double> zeros;
    double delta=(x2-x1)/(double(points-1)),x,y;//deltay = (y2-y1)/(double(points-1));

    if (activef == -1) {
        ymax=-val::Inf;
        ymin=val::Inf;
    }

    //wxMessageBox("Fine!");
    for (i=0;i<m;++i) {
        if ((activef != -1) && (activef != i)) continue;
        if (F[i].getmode() == plotobject::PARCURVE && activef == -1) {
            int n = 2 * points;
            double dx;
            x = F[i].x_range.x;
            dx = (F[i].x_range.y - x)/double(points);
            F[i].farray = val::d_array<double>(n);
            for (int j = 0; j < n-1; j+=2, x+=dx) {
                F[i].farray[j] = F[i].f(x);
                y = F[i].farray[j+1] = F[i].g(x);
                ymax=val::Max(ymax,y);
                ymin=val::Min(ymin,y);
                //std::cout << fx << " " << y << " " << std::endl;
            }
            //std::cout << dx << std::endl;
        }
        if (F[i].getmode() != plotobject::FUNCTION) continue;
        if (F[i].f.numberofvariables()==1) {
            F[i].farray.reserve(points);
            for (x=x1,j=0;j<points;++j,x+=delta) {
                //y=farray[i_f][j]=F[i](val::round(x,dec));
                y=F[i].farray[j]=F[i].f(x);
                ymax=val::Max(ymax,y);
                ymin=val::Min(ymin,y);
            }
        }
        else { //algebraische Kurve.
                //std::ofstream file("/home/miguel/test/log",std::ios::out | std::ios::trunc);
            F[i].curvearray.del();
            F[i].curvearray.reserve(points);
            for (x=x1,j=0;j<points;++j,x+=delta) {
                p=F[i].f.getunivarpol(x,"x2");
                //p = F[i].getpol(x);
                val::realRoots(p,zeros,1e-9);
                zeros.sort();
                F[i].curvearray[j].reserve(zeros.dimension());
                    //file<<x<<":       ";
                for (int k=0;k<zeros.dimension();++k) {
                        //file<<zeros[k]<<" , ";
                    F[i].curvearray[j][k] = y = zeros[k];
                    ymax=val::Max(ymax,y);
                    ymin=val::Min(ymin,y);
                }
                    //file<<std::endl;
            }
            if (comppoints) { // || critpoints[i_c].isempty()) {
                val::valfunction f(F[i].getinfixnotation());
                F[i].critpoints = critical_points_pl_alg_curve(f);
                F[i].critx = critical_x_values(F[i].critpoints);
            }
        }
    }

    //wxMessageBox("Fine!");

    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
}


void computetable(const plotobject& g, double x1,double x2, double dx)
{
     using namespace val;
     MyThreadEvent event(MY_EVENT,IdTable);
     Glist<GPair<double>> MP,PM;
     Glist<double> Zeros;
     double y0=g.f(x1),y1,x0=x1,eps=1e-4;
     int k=4, digits, ydigits; //m=isderived(g);


     while (dx<eps) {
        eps/=10;
        ++k;
     }

     tablestring = "Table for f(x) = " + g.getinfixnotation() + "\n";
     tablestring += " x         f(x) \n";
     for (double x=x1;x<=x2;x+=dx) {
        x=val::round(x,k);
        y1 = g.f(x);  //derive(g,x); //f(x);
        if (val::abs(y1)<eps) Zeros.inserttoend(x);
        else if (y0<0 && y1>0) MP.inserttoend(GPair<double>(x0,x));
        else if (y0>0 && y1<0) PM.inserttoend(GPair<double>(x0,x));
        digits = intdigits(x) + k;
        digits = val::Min(digits,val::MaxPrec);
        ydigits = intdigits(y1) + k;
        ydigits = val::Min(ydigits,val::MaxPrec);
        if (!val::isNaN(y1)) tablestring += ToString(val::round(x,k),digits) + "          " + ToString(val::round(y1,k),ydigits) + "\n";
        else tablestring += ToString(val::round(x,k), digits) + "          undefined" + "\n";
        x0=x;
        y0=y1;
     }

     if (!Zeros.isempty()) {
        tablestring += "\nPossible Zeros at: \n";
        for (const auto& el : Zeros) {
            digits = intdigits(el) + k;
            digits = val::Min(digits,val::MaxPrec);
            tablestring += ToString(el,digits) + "\n";
        }
     }
     if (!MP.isempty()) {
        tablestring += "\nChange Sign -/+ at: \n";
        for (const auto& el : MP) {
            digits = intdigits(el.x) + k;
            digits = val::Min(digits,val::MaxPrec);
            ydigits = intdigits(el.y) + k;
            ydigits = val::Min(ydigits,val::MaxPrec);
            tablestring += ToString(el.x,digits) + " ; " + ToString(el.y,ydigits) +"\n";
        }
     }

     if (!PM.isempty()) {
        tablestring += "\nChange Sign +/- at: \n";
        for (const auto& el:PM) {
            digits = intdigits(el.x) + k;
            digits = val::Min(digits,val::MaxPrec);
            ydigits = intdigits(el.y) + k;
            ydigits = val::Min(ydigits,val::MaxPrec);
            tablestring += ToString(el.x,digits) + " ; " + ToString(el.y,ydigits) +"\n";
        }
     }

     if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
}


void computetable_rat(const plotobject& f,val::rational x1,val::rational x2,val::rational dx)
{
     using namespace val;
     MyThreadEvent event(MY_EVENT,IdTable);
     Glist<GPair<rational>> MP,PM;
     Glist<rational> Zeros;
     rational y0=f.f(x1),y1,x0=x1,zero;

     tablestring = "Table for f(x) = " + f.getinfixnotation() + "\n";
     tablestring += " x         f(x) \n";
     for (rational x=x1;x<=x2;x+=dx) {
        y1 = f.f.rationaleval(x);
        if (y1.iszero()) Zeros.inserttoend(x);
        else if (y0<zero && y1>zero) MP.inserttoend(GPair<rational>(x0,x));
        else if (y0>zero && y1<zero) PM.inserttoend(GPair<rational>(x0,x));
        tablestring += ToString(x) + "          " + ToString(y1) + "\n";
        x0=x;
        y0=y1;
     }

     if (!Zeros.isempty()) {
        tablestring += "\nZeros at: \n";
        for (const auto& el:Zeros) {
            tablestring += ToString(el) + "\n";
        }
     }
     if (!MP.isempty()) {
        tablestring += "\nChange Sign -/+ at: \n";
        for (const auto& el:MP) {
            tablestring += ToString(el.x) + " ; " + ToString(el.y) +"\n";
        }
     }

     if (!PM.isempty()) {
        tablestring += "\nChange Sign +/- at: \n";
        for (const auto& el:PM) {
            tablestring += ToString(el.x) + " ; " + ToString(el.y) +"\n";
        }
     }

     if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
}



void computeevaluation(const plotobject& f, double par)
{
    using namespace val;
    d_array<char> sep({';'});
    Glist<std::string> wlist = getwordsfromstring(std::string(tablestring),sep);

    if (wlist.isempty()) return;
    int n = wlist.length(), decimals = 4, m = wlist[n-1].length();

    for (int i = 0; i < m; ++i) {
        if (wlist[n-1][i] != ' ') {
            wlist[n-1] = val::tailofstring(wlist[n-1], m-i);
            break;
        }
    }

    if (!(wlist[n-1].empty()) && wlist[n-1][0] == 'd') {
        wlist[n-1] = val::tailofstring(wlist[n-1], wlist[n-1].length()-1);
        decimals = val::FromString<int>(wlist[n-1]);
        decimals = val::Max(decimals, 0);
        decimals = val::Min(decimals, 12);
        wlist.delelement(n-1);
    }

    if (wlist.isempty()) return;

    valfunction F(f.getinfixnotation()), g, h;
    double x, y, limit = 1e50;
    int precision = val::MaxPrec, yprecision = val::MaxPrec;

    //F.setparameter(par);

    tablestring = "f(x) = " + f.getinfixnotation() + "\n";

    for (auto & w : wlist) {
        replace<char>(w, "ans", ansexpr);
        g = valfunction(w);
        h = F(g);
        g.setparameter(par);
        h.setparameter(par);
        tablestring += "\n x = " + w + ":" + "\nSymbolic evaluation:\n f(" + w + ") = " + h.getinfixnotation();
        x = g(0); y = h(0);
        if (val::abs(x) < limit) {
            x = val::round(x,decimals);
            precision = intdigits(x) + decimals;
        }
        if (val::abs(y) < limit) {
            y = val::round(y,decimals);
            yprecision = intdigits(y) + decimals;
        }
        precision = val::Min(precision,val::MaxPrec); yprecision = val::Min(yprecision, val::MaxPrec);
        tablestring += "\n\nDouble evaluation:\n f(" + w + ") = " + ToString(y,yprecision);
        tablestring += "\nPoint in graph: \n" + ToString(x,precision) + "  " + ToString(y,yprecision) + "\n";
    }
    ansexpr = "(" + h.getinfixnotation() + ")";
    MyThreadEvent event(MY_EVENT,IdEval);
    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone());
}

void calculate(std::string s)
{
    val::replace<char>(s, "ans", ansexpr);
    val::valfunction f(s);

    tablestring = "Evaluation of:\n" + s +": \nSymbolic:\n" + f.getinfixnotation() + "\n\ndouble:\n" + val::ToString(f(0),8);
    ansexpr = "(" + f.getinfixnotation() + ")";
    MyThreadEvent event(MY_EVENT,IdCalculate);
    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
}

void computezeroiteration(const plotobject&F,double x1,double x2,double eps,int n,int dez)
{
    using namespace val;
    MyThreadEvent event(MY_EVENT,IdIteration);
    int res, precision;
    val::valfunction f(F.f.getinfixnotation());
    double x = 0.5*(x1 + x2);

    tablestring="Zero approximation of:\n f(x) = " + f.getinfixnotation() + ".\n\n";

    tablestring+="Start-interval:  [ " + ToString(x1) + " ; " + ToString(x2) + " ]\nMax. number of Iterations: " + ToString(n) +
        "\nPrecision: " + ToString(eps) + "\nRound to decimal: " + ToString(dez) + "\n";
    res = SecantMethod(f,x1,x2,eps,n);
    precision = intdigits(x2) + dez;
    precision = val::Min(precision, val::MaxPrec);
    if (res>=0) {
        tablestring+="\nSecant-method successful!\nNumber of Iterations: " + val::ToString(res) + "\n x = " + ToString(val::round(x2,dez),precision);
    }
    else if (res==-1) {
        tablestring+= "\nSecant-method failed!\n x = " + ToString(val::round(x2,dez),precision);
    }
    else tablestring+="\nSecant-method failed!";

    if (f.isdifferentiable()) {
        val::valfunction f1 = f.derive();
        res = NewtonIteration(f, f1, x,eps,n);
        tablestring += "\n\nNewton Iteration:";
        if (res >= 0) {
            tablestring += "\nNewton-Iteration succesful!\nNumber of Iterations: " + val::ToString(res) + "\n x = " + ToString(val::round(x,dez),precision);
        }
        else if (res == -1) {
            tablestring += "\nNewton-Teration failed!\n x = " + ToString(val::round(x,dez),precision);
        }
        else tablestring += "\nNewton-Iteration failed!";
    }

    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
}



void computerotation(const val::d_array<plotobject*> F,std::string input)
{
    MyThreadEvent event(MY_EVENT,IdRefresh);
    val::vector<double> x(2),m(2),y(2);
    val::matrix<double> A;
    int fmode;
    std::string sf,sval;
    val::d_array<double> d_f;
    int i,n;
    double x0=0,y0=0,alpha=0;// x1= 0, x2 = 0;
    std::string x1 ="0", x2 = "0";

    val::d_array<char> separ({';',' '});
    //val::Glist<double>
    auto values = getwordsfromstring(input,separ);

    n = values.length();
    if (n == 0) return;
    if (n > 0) alpha = val::FromString<double>(values[0]);
    if (n > 1) x0 = val::FromString<double>(values[1]);
    if (n > 2) y0 = val::FromString<double>(values[2]);
    if (n > 3) x1 = values[3];
    if (n > 4) x2 = values[4];


    A=val::rotationmatrix(val::PI*alpha/180.0);

    m(0) = x0; m(1) = y0;

    for (const auto & f: F) {
        d_f.del();
        fstring+=";\n";
        fmode = f->getmode();
        sf = "";
        switch (fmode)
        {
            case plotobject::LINE : {
                sf+="line";
                d_f = f->farray;
                break;
            }
            case plotobject::TRIANGLE:  {
                sf+="triangle";
                d_f = f->farray;
                break;
            }
            case plotobject::POLYGON: {
                sf+="polygon";
                d_f = f->farray;
                break;
            }
            case plotobject::POINTS: {
                sf+="points";
                d_f = f->farray;
                break;
            }
            case plotobject::PARCURVE: case plotobject::FUNCTION: {
                val::valfunction f1,f2;
                if (fmode == plotobject::FUNCTION) {
                    if (f->f.numberofvariables() > 1) return;
                    f1 = val::valfunction("x");
                    f2 = f->f;
                    if (f->x_range.x != f->x_range.y) {
                        x1 = f->x1.getinfixnotation();
                        x2 = f->x2.getinfixnotation();
                    }
                }
                else {
                    f1 = f->f; f2 = f->g;
                    x1 = f->x1.getinfixnotation(); x2 = f->x2.getinfixnotation();
                }
                val::valfunction A00(val::ToString(A(0,0))), A01(val::ToString(A(0,1))), A10(val::ToString(A(1,0))), A11(val::ToString(A(1,1)));
                val::valfunction m0(val::ToString(m(0))), m1(val::ToString(m(1))), g1, g2;
                g1 = A00*(f1-m0) + A01*(f2 -m1) + m0;
                g2 = A10*(f1-m0) + A11*(f2 -m1) + m1;
                sf += "( " + g1.getinfixnotation() + " , " + g2.getinfixnotation() + " )  [ " + x1 + " , " + x2 + " ]";
                break;
            }
            default: break;
        }
        if (fmode != plotobject::PARCURVE && fmode != plotobject::FUNCTION) {
            n = d_f.length();
            for (i =0;i<n;++i) {
                x(i%2) = d_f[i];
                if (i%2) {
                    y=A*(x-m) + m;
                    sf+= " " +  val::ToString(y(0)) + " " + val::ToString(y(1));
                }
            }
            if (fmode == plotobject::LINE && d_f[4] != 0) {
                sf += " " + val::ToString(d_f[4]);
            }
        }
        fstring+=sf;
    }
    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
}



void computeregression(const plotobject& f,int degree)
{
    if (!f.IsPoints() && !f.IsPolygon()) return;
    val::d_array<double> d_f = f.farray;
    double minx = val::Inf, maxx = -val::Inf;
    int n = d_f.length(), i, N = n/2;
    if (n<=0) return;

    if (n<=5) degree=1;

    for (i = 0; i < n; i += 2) {
        minx = val::Min(minx,d_f[i]);
        maxx = val::Max(maxx,d_f[i]);
    }

    MyThreadEvent event(MY_EVENT,IdRefresh);
    if (degree==1) {
        double y=0,x=0,m=0,q=0,b;

        for (i=0;i<n;i+=2) {
            x+=d_f[i]; y+=d_f[i+1];
        }
        x/=double(N); y/=double(N);

        for (i=0;i<n;i+=2) {
            m+=(d_f[i]-x)*(d_f[i+1]-y);
            q+=(d_f[i]-x)*(d_f[i]-x);
        }
        m/=q;
        if (val::isNaN(m) || val::abs(m)==val::Inf) return;
        b=y - m*x;
        fstring += ";\n" + val::ToString(m) + "*x";
        if (val::abs(b)>1e-9) {
            if (b>0) fstring += " + ";
            else fstring +=" - ";
            fstring += val::ToString(val::abs(b));
        }
    }
    else {
        int j;
        degree = val::Min(N,degree);
        val::d_array<val::vector<double> > g(degree+1);
        val::vector<double> y(N);
        double det;

        for (i=0;i<=degree;++i) g[i]=val::vector<double>(N);
        for (i=0;i<N;++i) g[0](i)=1.0;
        for (i=0;i<N;++i) {
            g[1](i) = d_f[2*i];
            y(i) = d_f[2*i+1];
        }
        for (i=2;i<=degree;++i) {
            for (j=0;j<N;++j) g[i](j) = val::power(g[1](j),i);
        }

        val::matrix<double> A(degree+1,degree+2),X;
        for (i=0;i<=degree;++i) {
            A(i,i) = g[i]*g[i];
            for (j=i+1;j<=degree;++j) A(i,j) = A(j,i) = g[i]*g[j];
            A(i,degree+1) = g[i]*y;
        }
        val::les_double(A,X,det);

        val::pol<double> f;
        for (i=0;i<=degree;++i) f.insert(X(0,i),i);
        if (f.iszero()) return;
        fstring += ";\n" + val::PolToString(f);
    }
    if (f.IsPolygon()) {
        fstring += "[" + val::ToString(minx) + " , " + val::ToString(maxx) + "]";
    }

    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
}


void computepointstatistics(const plotobject& f, std::string input)
{
    if (!f.IsPoints() && !f.IsPolygon()) return;
    val::d_array<double> Points = f.farray;
    int i, n = Points.length()/2, decimals = 4, m;

    if (n < 2) return;

    double Ex = 0, Ey = 0, Vx = 0, Vy = 0, Cxy = 0, rhoxy = 0, dn = double(n), v, w, sigmax, sigmay, s2x, s2y, epsilon = 1e-9;
    wxString sigma(L"\u03C3"), rho(L"\u03C1");
    val::d_array<char> sep({';', '\n'});
    auto values = getwordsfromstring(input, sep);

    m = values.length();

    if (m>2) epsilon = val::FromString<double>(values[2]);
    if (m>4) decimals = val::FromString<int>(values[4]);

    if (epsilon < 0) epsilon = 1e-9;
    if (decimals < 0 || decimals > 10) decimals = 4;

    for (i = 0; i < n; ++i) {
        Ex += Points[2*i];
        Ey += Points[2*i+1];
    }
    Ex /= dn; Ey /= dn;

    for (i = 0; i < n; ++i) {
        v = Points[2*i] - Ex;
        w = Points[2*i + 1] - Ey;
        Cxy += v*w;
        Vx += v*v;
        Vy += w*w;
    }
    s2x = Vx /(dn-1); s2y = Vy/(dn-1);
    Vx /= dn; Vy /= dn; Cxy /= dn;
    sigmax = val::sqrt(Vx); sigmay = val::sqrt(Vy);

    tablestring = "Number of Points, n = " + val::ToString(n) + "\n";
    tablestring += "\n E(X) = " +  val::ToString(val::round(Ex,decimals)) + ", V(X) = " + val::ToString(val::round(Vx,decimals)) + " , " + sigma + "(X) = " + val::ToString(val::round(sigmax,decimals));
    tablestring += "\n S2(X) = " + val::ToString(val::round(s2x,decimals)) + ", sx = " + val::ToString(val::round(val::sqrt(s2x),decimals)) + "\n";
    tablestring += "\n E(Y) = " + val::ToString(val::round(Ey,decimals)) + ", V(Y) = " + val::ToString(val::round(Vy,decimals)) + " , " + sigma + "(Y) = " + val::ToString(val::round(sigmay,decimals));
    tablestring += "\n S2(Y) = " + val::ToString(val::round(s2y,decimals)) + ", sy = " + val::ToString(val::round(val::sqrt(s2y),decimals)) + "\n";
    tablestring += "\n C(X,Y) = " + val::ToString(val::round(Cxy,decimals));

    if ((val::abs(Vx) > epsilon) && (val::abs(Vy) > epsilon)) {
        rhoxy = Cxy/val::sqrt(Vx*Vy);
        tablestring += " , " + rho + "(X,Y) = " + val::ToString(val::round(rhoxy,decimals));
    }
    MyThreadEvent event(MY_EVENT,IdPointStat);
    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone());
}


void analyze_triangle(const plotobject &f, std::string input)
{
    if (!f.IsTriangle()) return;
    int decimals = 4;
    double  a, b , c, a2, b2, c2, d, cr2, cr, ir2, ir, u, alpha, beta, gamma, Area, centrox, centroy;
    val::vector<double> A(2), B(2), C(2), AB(2), AC(2), BC(2), I(2), U(2);
    Points.resize(3);

    //Get Parameters
    val::d_array<char> sep({';', '\n'});
    auto values = getwordsfromstring(input,sep);
    if (values.length()>4) decimals = val::FromString<int>(values[4]);

    if (decimals < 0 || decimals > 10) decimals = 4;

    A(0) = f.farray[0]; A(1) = f.farray[1]; B(0) = f.farray[2]; B(1) = f.farray[3]; C(0) = f.farray[4]; C(1) = f.farray[5];
    AB = B - A; AC = C - A; BC = C - B;
    a2 = BC*BC; b2 = AC*AC; c2 = AB*AB;
    a = val::sqrt(a2); b = val::sqrt(b2); c = val::sqrt(c2);
    u = a + b + c;
    Area = val::sqrt((a+b-c)*(a-b+c)*(b+c-a)*(a+b+c))/4.0;

    // compute circumcenter and circumradius:
    d = 2.0*(AB(0)*AC(1) - AB(1)*AC(0));
    U(0) = (AC(1)*(AB(0)*AB(0) + AB(1)*AB(1)) - AB(1)*(AC(0)*AC(0) + AC(1)*AC(1)))/d;
    U(1) = (AB(0)*(AC(0)*AC(0) + AC(1)*AC(1)) - AC(0)*(AB(0)*AB(0) + AB(1)*AB(1)))/d;
    cr2 = U*U; cr = val::sqrt(cr2);
    U += A;
    U(0) = val::round(U(0),decimals); U(1) = val::round(U(1),decimals);
    Points[0].resize(2);
    Points[0][0].x = U(0); Points[0][0].y = U(1);

    // compute incenter, inradius and centroid
    I = ((a/u)*A + (b/u)*B + (c/u)*C);
    I(0) = val::round(I(0),decimals); I(1) = val::round(I(1),decimals);
    ir2 = ((b+c-a)*(a+c-b)*(a+b-c))/(4*u); ir = val::sqrt(ir2);
    Points[1].resize(2);
    Points[1][0].x = I(0); Points[1][0].y = I(1);
    //
    Points[2].resize(1);
    centrox = Points[2][0].x = val::round((A(0)+B(0)+C(0))/3,decimals); centroy = Points[2][0].y = val::round((A(1)+B(1)+C(1))/3,decimals);


    // Compute angles:
    alpha = val::arccos((b2+c2-a2)/(2*b*c)); beta = val::arccos((a2+c2-b2)/(2*a*c));
    // in degrees:
    alpha /= val::PI; alpha *= 180.0; beta /= val::PI; beta *= 180.0; gamma = 180 - (alpha + beta);

    // Round:
    a = val::round(a,decimals); b = val::round(b,decimals); c = val::round(c,decimals);
    alpha = val::round(alpha,2); beta = val::round(beta,2); gamma = val::round(gamma,2);
    cr = val::round(cr,decimals); ir = val::round(ir,decimals);
    Points[0][1].x = cr;
    Points[1][1].x = ir;
    //

    analyze_output.resize(4);
    for (int i = 0; i < 3; ++i) analyze_output[i] = "";
    analyze_output[0] = "Points of triangle:\n A(" + val::ToString( A(0)) + "," + val::ToString(A(1)) +"); B(" +
        val::ToString(B(0)) + "," + val::ToString(B(1)) + "); C(" + val::ToString(C(0)) + "," + val::ToString(C(1)) + ").\n";
    analyze_output[0] += "|AB| = " + val::ToString(c) + "; |AC| = " + val::ToString(b) + "; |BC| = " + val::ToString(a) + ".\n";
    analyze_output[0] += "\n " + greek_letters[0] + " = " + val::ToString(alpha) + _T("° , ")+ greek_letters[1] +  " = " + val::ToString(beta) + _T("° , ") +
                          greek_letters[2] + " = " + val::ToString(gamma) + _T("°.");
    analyze_output[0] += "\n Area = " + val::ToString(Area);

    analyze_output[1] = "Circumcenter: (" + val::ToString(U(0)) + " , " + val::ToString(U(1)) + ")\nCircumradius: " + val::ToString(cr);
    analyze_output[2] = "Incenter: (" + val::ToString(I(0)) + " , " + val::ToString(I(1)) + ")\nInradius: " + val::ToString(ir);
    analyze_output[3] = "Centroid: (" + val::ToString(centrox) + " , " + val::ToString(centroy) + ")";

    MyThreadEvent event(MY_EVENT,myevent_id::IdTriangle);
    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone());
}


void computetangent(std::string sf,const plotobject &f,double x1,double x2,int tangent)
{
    if (sf=="") return;
    //std::string svalue;
    int n=sf.length(),i,isingraph=0,isfunction=0;
    double x=0.0,y=0.0,m,b;

    val::d_array<char> separators{' ', ';'};
    val::Glist<double> values = getdoublevaluesfromstring(sf,separators);

    if (values.isempty()) return;
    x = values[0];
    if (values.length()>1) {
        y = values[1];
    }
    else isingraph = 1;

    if (f.f.numberofvariables()<=1) isfunction=1;

    val::valfunction F(f.f.getinfixnotation());
    F.setparameter(f.f.getparameter());

    if (f.getmode() == plotobject::PARCURVE) {
        val::valfunction G = f.g, f1 = f.f.derive(), g1 = f.g.derive(), h;

        if (isingraph) h = F - val::valfunction(val::ToString(x,15));
        else {
            val::valfunction h1, h2 = G - val::valfunction(val::ToString(y,15));
            if (tangent) {
                h1 = val::valfunction(val::ToString(x,15)) - F;
                h = h1*g1 + h2*f1;
            }
            else {
                h1 = F - val::valfunction(val::ToString(x,15));
                h = h1*f1 + h2*g1;
            }
        }
        //F -= val::valfunction(val::ToString(x));

        auto t_values = h.double_roots(f.x_range.x, f.x_range.y, 1000);
        if (t_values.isempty()) return;
        double x1, y1, b;
        std::string s_p = ";\npoints ";
        n = 0;
        for (const auto& t : t_values) {
            x1 = f1(t); y1 = g1(t);
            if (val::abs(x1) < 1e-9 && val::abs(y1) < 1e-9) continue;
            ++n;
            m = y1/x1;
            //std::cout <<"\nf1 = " << f1.getinfixnotation() << " ;t = " << t << "; x1 = "<< x1 << "; m = " << m << std::endl;
            if (tangent) {
                if (val::abs(x1) < 1e-9) m = val::Inf;
            }
            else {
                if (val::abs(m) < 1e-9) m = val::Inf;
                else m = -1/m;
            }
            if (isInf(m)) {
                fstring += ";\nline " + val::ToString(x) + " -inf " + val::ToString(x) + " inf";
            }
            else {
                if (val::abs(m) < 1e-9) m = 0.0;
                if (isingraph) b = f.g(t) - m*x;
                else b = y - m*x;
                h = val::valfunction(val::ToString(m) + "*x") + val::valfunction(val::ToString(b));
                fstring += ";\n" + h.getinfixnotation();
            }
            s_p += val::ToString(F(t)) + " " + val::ToString(G(t)) + " ";
        }
        if (n) fstring += s_p;
    }
    else if (isfunction) {
        int diffbar = 0;
        if (isingraph) {
            if (F.isdifferentiable()) {
                m=(F.derive())(x);
                diffbar = 1;
            }
            else {
                //n=isderived(f);
                m= derive(f,x);  //F.derive(x);
            }
            if (val::isNaN(m)) return;
            if (!tangent) {
                if (val::abs(m)<=1e-9) m=val::Inf; // m = inf
                else m = -1/m;
            }
            if (isInf(m)) {
               fstring+=";\nline "+val::ToString(x)+ " -inf " + val::ToString(x) + " inf";
            }
            else if (diffbar) {
                val::valfunction x_f(sf), m_f = (F.derive())(x_f);
                if (!tangent) m_f = val::valfunction("-1/(" + m_f.getinfixnotation() + ")");
                val::valfunction b_f = F(x_f) - m_f * x_f, g = m_f * val::valfunction("x") + b_f;
                fstring += ";\n" + g.getinfixnotation();
            }
            else {
                b=F(x) -m*x;
                fstring+=";\n" + val::ToString(m) + "*x";
                if (val::abs(b)>1e-9) {
                    if (b>=0) fstring+= " + ";
                    fstring+=val::ToString(b);
                }
            }
        }
        else {
            val::valfunction h,h1,F1;
            if (!F.isdifferentiable()) return;
            F1=F.derive();
            val::Glist<double> Roots;

            h1 = val::valfunction("x - " + val::ToString(x));
            if (tangent) h = F - val::valfunction(val::ToString(y)) - h1*F1;
            else h = F1 * (F - val::valfunction(val::ToString(y))) + h1;
            //fstring+=";\n" + h.getinfixnotation();
            Roots = h.double_roots(x1,x2,1000);
            for (const auto& z : Roots) {
                m=F1(z);
                if (!tangent) m = -1.0/m;
                if (val::isNaN(m)) continue;
                if (isInf(m)) {
                    fstring+=";\nline "+val::ToString(z)+ " -inf " + val::ToString(z) + " inf";
                    continue;
                }
                b = F(z) - m*z;
                fstring+=";\n" + val::ToString(m) + "*x";
                if (b>0) fstring+= " + ";
                fstring+=val::ToString(b);
            }
            if (!Roots.isempty()) fstring+=";\npoints";
            for (const auto& z : Roots) {
                fstring += " " + val::ToString(z)+ " " + val::ToString(F(z));
            }
        }
    }
    else {
        val::valfunction Fx =F.derive(1) ,Fy = F.derive(2);
        if (isingraph) {
            val::pol<double> p=f.getpol(x);
            val::vector<double> Roots;
            val::realRoots(p,Roots);

            if (!(n=Roots.dimension()))return;
            double a;
            val::vector<double> X(2);
            X(0) = x;
            for (int i=0;i<n;++i) {
                X(1) = Roots[i];
                a = Fx(X);
                b=Fy(X);  // Tangente: ax+by =0;
                if (val::abs(a)<1e-9) a=0.0;
                if (val::abs(b)<1e-9) b = 0.0;
                if (a==0.0 && b==0.0) continue;   // Singular point.
                //c = -a*x - b*Roots[i];
                if (b==0.0) {
                    if (tangent) {
                        fstring+=";\nline " + val::ToString(x) + " -inf " + val::ToString(x) + " inf";
                        continue;
                    }
                    else {
                        fstring+="\n"+ val::ToString(Roots[i]);
                        continue;
                    }
                }
                m=-a/b;
                if (!tangent) {
                   if (a==0.0) {
                       fstring+=";\nline " + val::ToString(x) + " -inf " + val::ToString(x) + " inf";
                       continue;
                   }
                   m = b/a;
                }
                b = Roots[i] - m*x;
                fstring+=";\n" + val::ToString(m) + "*x";
                if (val::abs(b)>1e-9) {
                    if (b>=0) fstring+= " + ";
                    fstring+=val::ToString(b);
                }
            }
        }
        else {
            if (F.numberofvariables()>2) return;
            int s_oldordtype=val::s_expo::getordtype(),n_oldordtype = val::n_expo::getordtype();
            val::valfunction g;//= Fx*val::valfunction("x - "+val::ToString(x)) + Fy*val::valfunction("y - "+val::ToString(y));
            val::s_expo::setordtype(-1);
            val::n_expo::setordtype(-1);
            val::n_polynom<val::rational>::setstaticexpodim(2);
            val::s_polynom<val::integer> g1,g2;
            val::pol<double> h,h1;
            val::Glist<val::s_polynom<val::integer>> G;
            val::vector<double> rootsy,roots, X(2);
            val::GPair<double> Paar;
            val::Glist<val::GPair<double>> candlist;

            if (tangent) g = Fx*val::valfunction("x - "+val::ToString(x)) + Fy*val::valfunction("y - "+val::ToString(y));
            else g = Fy*val::valfunction("x - "+val::ToString(x)) - Fx*val::valfunction("y - "+val::ToString(y));

            g1=val::primitivpart(F.gets_polynom()); g2 = val::primitivpart(g.gets_polynom());
            G.sinsert(std::move(g1)); G.sinsert(std::move(g2));
            val::primitiv_groebner(G);

            if (G.length()!=2) {
                MyThreadEvent infoevent(MY_EVENT,IdInfo);

                if (!val::iszerodimensional(G)) {
                    infoevent.SetMessage("No solution!");
                    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(infoevent.Clone() );
                    return;
                }
                else val::radical_ideal(G,3,0);

                if (G.length()!=2) {
                    infoevent.SetMessage("Groebner Basis has more than 2 elements!");
                    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(infoevent.Clone() );
                    return;
                }
            }
            //
            val::realRoots(val::ToDoublePolynom(val::To_unipol(G[0],1)),rootsy,1e-9);
            for (i=0;i<rootsy.dimension();++i) {
                val::realRoots(eval_f(G[1],rootsy[i],1),roots,1e-9);
                for (int j=0;j<roots.dimension();++j) {
                    X(0)=Paar.x=roots[j]; X(1)=Paar.y=rootsy[i];
                    if (!isinList(candlist,Paar,1e-4)) {
                        if (val::abs(Fx(X))<1e-9 && val::abs(Fy(X))<1e-9) continue;
                        candlist.sinsert(Paar);
                    }
                }
            }

            if (candlist.isempty())
            {
                MyThreadEvent infoevent(MY_EVENT,IdInfo);
                infoevent.SetMessage("No Points found!");
                if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(infoevent.Clone() );
                return;
            }



            for (const auto& P : candlist) {
                m=(P.y - y)/(P.x - x);
                if (val::isNaN(m)) continue;
                if (isInf(m)) {
                    //if (tangent) {
                    fstring+=";\nline " + val::ToString(P.x) + " -inf " + val::ToString(P.x) + " inf";
                    //}
                    //else fstring+=";\n" + val::ToString(P.y);
                    continue;
                }
                if (val::abs(m)<1e-9) {
                    //if (tangent)
                    fstring+=";\n" + val::ToString(P.y);
                    //else fstring+=";\nline " + val::ToString(P.x) + " -inf " + val::ToString(P.x) + " inf";
                    continue;
                }
                //if (!tangent) m = -1.0/m;
                b= P.y - m*P.x;
                fstring+=";\n" + val::ToString(m) + "*x";
                if (val::abs(b)>1e-9) {
                    if (b>=0) fstring+= " + ";
                    fstring+=val::ToString(b);
                }
            }


            if (!candlist.isempty()) fstring+=";\npoints";
            for (const auto& P : candlist) {
                fstring+= " " + val::ToString(P.x) + " " + val::ToString(P.y);
            }

            val::s_expo::setordtype(s_oldordtype);
            val::n_expo::setordtype(n_oldordtype);
        }
    }
    MyThreadEvent event(MY_EVENT,IdRefresh);
    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
}



void computeinterpolation(std::string input,std::string &Fstring)
{
    MyThreadEvent event(MY_EVENT,IdRefresh);
    int n=Fstring.length();
    val::pol<val::rational> f = interpolation(input);

    if (n > 1 && Fstring[n-1]!=';') Fstring+=';';
    Fstring+=PolToString(f);
    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
}



// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

const val::d_array<std::string> plotobject::s_object_type{"line", "text", "circle", "rectangle", "triangle", "fill", "polygon", "points"};
const val::d_array<int> plotobject::defnpoints{5,2,6,4,6,3,2,2,4};
val::Glist<wxImage> plotobject::image;

//replace std::tring with wxString
void plotobject::setdrawingwords(const wxString &s)
{
    TextWords.dellist();
    if (s.empty()) return;

    unsigned i, n = s.length(), s_ready = 0;
    drawingword lword;

    for (i = 0; i < n; ++i) {
        if (s[i] == '_' && i < n-1) {
            if (s[i+1] == '{') {
                for (i += 2; i < n; ++i ) {
                    if (s[i] == '}') {
                        s_ready = 1;
                        break;
                    }
                    else {
                        lword.sub_word += s[i];
                    }
                }
            }
            else {
                ++i;
                lword.sub_word += s[i];
                s_ready = 1;
            }
            //if (s_ready) continue;
        }
        else if (s[i] == '^' && i < n-1) {
            if (i < n-1 && s[i+1] == '{') {
                for (i += 2; i < n; ++i ) {
                    if (s[i] == '}') {
                        s_ready = 1;
                        break;
                    }
                    else {
                        lword.sup_word += s[i];
                    }
                }
            }
            else {
                ++i;
                lword.sup_word += s[i];
                s_ready = 1;
            }
        }
        else if (!s_ready) lword.word += s[i];
        if (s_ready) {
            TextWords.push_back(lword);
            lword.sub_word = lword.sup_word = lword.word = "";
            s_ready = 0;
        }
    }
    if (lword.word != "") TextWords.push_back(lword);
    return;
}




plotobject::plotobject(const std::string &sf)
{
    if (sf.empty()) return;
    int i, n, m;
    std::string s_f= sf, fw, ns;
    val::rational factor;

    islinear = 0;

    ns = extractstringfrombrackets(s_f, '[', ']');
    textdata = extractstringfrombrackets(s_f, '{', '}');
    fw = val::getfirstwordofstring<val::d_array>(s_f);

    val::Glist<std::string> values = getwordsfromstring(ns,val::d_array<char>({','}),0,val::d_array<char>({' '}));
    n = values.length();

    if (n > 0) {
        //if (!getpiscale(values[0],factor,x_range.x,0))  x_range.x = val::FromString<double>(values[0]);
        x1 = val::valfunction(values[0]);
        x2 = x1;
        x_range.x = x1(0);
        x_range.y = x_range.x;
    }
    if (n > 1) {
        //if (!getpiscale(values[1],factor,x_range.y,0)) x_range.y = val::FromString<double>(values[1]);
        x2 = val::valfunction(values[1]);
        x_range.y = x2(0);
    }
    if (x_range.x > x_range.y) {
        x1 = x2 = val::valfunction("0");
        x_range.x = x_range.y = 0.0;
    }

    // Check if object is a bitmap
    if (fw.find("bitmap") != std::string::npos) {
        if (image.isempty()) return;
        int pos = 1, l = fw.length(), m, sx, sy = 0;
        std::string s_pos;
        for (int i = 7; i < l; ++i) s_pos += fw[i];
        //std::cout << "\n s_pos = " << s_pos << std::endl;
        if (s_pos != "") pos = val::FromString<int>(s_pos);
        if (pos < 1 || pos > image.length()) return;
        //
        objectype = BITMAP;
        farray = val::d_array<double>(0.0,defnpoints[BITMAP]);
        values.dellist();
        values = getwordsfromstring(s_f, val::d_array<char>({' '}));
        values.skiphead();
        m = val::Min(farray.length(),values.length());
        for (int i = 0; i < m; ++i) farray[i] = val::FromString<double>(values[i]);
        // assign bitmap to bitmap;
        sx = int(farray[2]); sy = int(farray[3]);
        if (sy == 0) sy = sx;
        if (sx < 0) sx = image[pos-1].GetWidth();
        if (sy < 0) sy = image[pos-1].GetHeight();
        farray[2] = double(sx); farray[3] = double(sy);
        wxImage s_image = image[pos-1].Scale(sx,sy);
        bitmap = wxBitmap(s_image);
        //
        s_infix = "bitmap_" + val::ToString(pos);
        for (const auto &v: farray) s_infix += " " + val::ToString(v);
        return;
    }

    // Check for other object types:
    for (i = 0; i < s_object_type.length(); ++i) {
        if (fw == s_object_type[i]) {
            objectype = i;
            s_infix = fw;
            break;
        }
    }
    // Check if object is a parametric curve:
    if (s_f.find(",") != std::string::npos ) {
        s_f = extractstringfrombrackets(s_f, '(', ')');
        values.dellist();
        values = getwordsfromstring(s_f, val::d_array<char>({','}));
        if (values.length() < 2) return;
        objectype = PARCURVE;
        f = val::valfunction(values[0]);
        g = val::valfunction(values[1]);
        s_infix = "( " + f.getinfixnotation() + " , " + g.getinfixnotation() + " )";
        if (f.islinearfunction() && g.islinearfunction()) islinear = 1;
        return;
    }
    //
    if (objectype == FUNCTION) {
        f = val::valfunction(s_f,0);
        if (f.getinfixnotation() == "0" && s_f != "0") s_infix = "";
        else if (f.is_zero()) s_infix = "0";
        else s_infix = f.getinfixnotation();
        islinear = f.islinearfunction();
        return;
    }
    values.dellist();
    val::d_array<char> sep{' ', '\n'};
    values = getwordsfromstring(s_f, sep);
    if (values.isempty())  {
        objectype = FUNCTION;
        return;
    }
    s_infix = values[0];
    values.skiphead();

    n = values.length();

    switch (objectype) {
        case POINTS: case POLYGON:
        {
            if (n%2) --n;
            m = val::Max(defnpoints[POLYGON],n);
            farray = val::d_array<double>(0.0,m);
            for (int i = 0; i < n; ++i ) {
                farray[i] = val::FromString<double>(values[i]);
                s_infix += " " + values[i];
            }
        } break;
        case LINE: case CIRCLE: case RECTANGLE: case TRIANGLE: case FILL: case TEXT:
        {
            farray = val::d_array<double>(0.0,defnpoints[objectype]);
            m = val::Min(defnpoints[objectype], n);

            if (objectype == CIRCLE) farray[4] = 360.0;
            if (objectype == FILL) farray[2] = 1.0;
            if (objectype == TEXT) s_infix += " {" + textdata + "} ";
            for (int i = 0; i < m; ++i) {
                farray[i] = val::FromString<double>(values[i]);
                s_infix += " " + values[i];
            }

        } break;
        default: break;
    }
    if (objectype == TEXT) {
        // replace greek letters..
        for (int i = 0; i < greek_literals.length(); ++i) {
            textdata.Replace(greek_literals[i], greek_letters[i]);
        }
        setdrawingwords(textdata);
    }
}


int plotobject::iswithparameter() const
{
    if (objectype != FUNCTION) return 0;
	int n = s_infix.length(), is = 0;
	for (int i = 0; i < n-1; ++i) {
		//if (s[i] == 'P' && s[i+1] == 'I') is = 1;
		if (s_infix[i] == 'r' && s_infix[i+1] == 't') {  //  case sqrt
			++i;
			continue;
		}
		if (s_infix[i] == 't' && s_infix[i+1] != 'a') is = 1;
	}
	if (s_infix[n-1] == 't') is = 1;
	return is;
}


val::pol<double> plotobject::getpol(const double& x) const
{
    /*
    val::vector<val::valfunction> arg({val::valfunction(val::ToString(x)), val::valfunction("x")});
    val::valfunction g;
    g = f(arg);
    */
    std::string sf = s_infix, value = "(" + val::ToString(x) + ")", svarx = "x", svary = "y";
    val::replace(sf, svarx, value);
    val::replace(sf, svary, svarx);
    val::valfunction g(sf,0);
    val::pol<double> p = val::ToDoublePolynom(g.getpolynomial());
    return p;
}
