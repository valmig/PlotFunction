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
val::Glist<std::string> recentcommands;


const val::d_array<wxString> greek_literals{"\\alpha", "\\beta", "\\gamma", "\\delta", "\\epsilon", "\\zeta", "\\eta", "\\theta", "\\iota", "\\kappa", "\\lambda", "\\mu", "\\nu",
                                            "\\xi", "\\omicron", "\\pi", "\\rho", "\\sigma", "\\tau", "\\phi", "\\chi", "\\psi", "\\omega", "\\Gamma", "\\Delta", "\\Pi",
                                            "\\Sigma", "\\Phi", "\\Psi", "\\Omega"};

const val::d_array<wxString> greek_letters{L"\u03B1", L"\u03B2", L"\u03B3", L"\u03B4", L"\u03B5",  L"\u03B6", L"\u03B7", L"\u03B8", L"\u03B9", L"\u03BA", L"\u03BB", L"\u03BC", L"\u03BD",
                                           L"\u03BE", L"\u03BF", L"\u03C0", L"\u03C1", L"\u03C3", L"\u03C4", L"\u03C6", L"\u03C7", L"\u03C8", L"\u03C9", L"\u0393", L"\u0394", L"\u03A0",
                                           L"\u03A3", L"\u03A6", L"\u03A8", L"\u03A9"};

const val::d_array<std::string> WordList{"PI", "exp", "log", "line", "sinh", "sqrt", "cosh", "circle", "tanh", "text", "triangle", "polygon", "points",
                                   "inf", "fill", "abs", "arcsin", "arccos", "arctan", "rectangle", "sin", "cos", "tan",
                                   "arsinh", "arcosh", "artanh" };


val::d_array<std::string> sfunctionlist({"sqrt", "exp", "log", "abs", "sinh", "cosh", "tanh", "arsinh", "arcosh", "artanh",
										"sin", "cos", "tan", "arcsin", "arccos", "arctan", "inf", "PI", "line", });

val::d_array<val::GPair<std::string,val::d_function*>>
functionpairs ({ {"sqrt",val::sqrt}, {"abs",val::abs}, {"exp", val::exp}, {"log", val::log}, {"sin", val::sin}, {"cos", val::cos},
            {"tan", val::tan}, {"arcsin", val::arcsin}, {"arccos", val::arccos}, {"arctan", val::arctan}, {"sinh", val::sinh},
            {"cosh", val::cosh}, {"tanh", val::tanh}, {"arsinh", val::arsinh}, {"arcosh", val::arcosh}, {"artanh", val::artanh}});


const val::d_array<wxColour> defaultcolors{wxColour(0,0,255), wxColour(255,0,0), wxColour(0,255,0), wxColour(173,216,230),
                                      wxColour(255,116,0), wxColour(238,0,255), wxColour(125,125,125), wxColour(255,255,255),
                                      wxColour(0,0,0), wxColour(191,191,191), wxColour(255,255,0), wxColour(0,128,0), wxColour(245,245,220), wxColour(165,42,42),
                                      wxColour(0,0,128), wxColour(255,192,203), wxColour(135,206,235), wxColour(144,238,144)};

const val::d_array<std::string> defaultcolornames{"blue", "red", "green", "lblue", "orange", "violet", "grey", "white", "black",
                                "lgrey", "yellow", "dgreen", "beige", "brown", "dblue", "pink", "skyblue", "lgreen" };

const val::d_array<std::string> SettingsList({"axis-scale", "axis-color", "grid-scale", "grid-color", "values-number", "axis-range", "show-x-axis",
                                "show-y-axis", "show-grid" , "show-x-scale", "show-y-scale" , "reset-colors", "font-size", "function-color", "panel-size", "axis-names", "regression-degree",
                                "point-decimals", "show-function", "background-color", "parameter-values", "function-size", "margin",
                                "axis-fontsize", "function-settings", "move-increment" });

const val::d_array<std::string> SettingsParList({"axis-scale sx [sy]	<Shift-Alt-S>",
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

const val::d_array<std::string> CommandsList({"derive", "analyze", "tangent", "normal", "interpolation", "regression", "table", "integral",
                                             "arclength", "zero-iteration", "move", "evaluate", "intersection", "calculate" });

const val::d_array<std::string> CommandsParList({"derive [#nr = 1]",
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
                                                 "calculate arithmetic expression"
                                                 });



//const val::d_array<std::string> InputDialogList(SettingsList);

const val::trie_type<std::string> WordTree(WordList + defaultcolornames, 58, int('A'));


const val::trie_type<std::string> InputDialogTree(SettingsList + CommandsList + defaultcolornames, 78, int('-'));


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


// -------------------------------------------------------------------------------------------------------------------------------

std::string getstringfunction(const std::string &s, int j)
{
	int n = s.length() - j, m, found, i;

	for (const auto& sf : WordList) {
		m = sf.length();
		found = 0;
		if (n >= m) {
			found = 1;
			for (i = 0; i < m; ++i) {
				if (sf[i] != s[i+j]) {
					found = 0;
					break;
				}
			}
		}
		if (found) return sf;
	}

	return "";
}

int getindexoffunction(const std::string &sf)
{
	if (sf =="") return -1;
	int i = 0;

	for (const auto &f : functionpairs) {
		if (f.x == sf) return i;
		++i;
	}

	return -1;
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

void computezeroiteration(const plotobject&f,double x1,double x2,double eps,int n,int dez)
{
    using namespace val;
    MyThreadEvent event(MY_EVENT,IdIteration);
    int res, precision;
    tablestring="Zero approximation of:\n f(x) = " + f.getinfixnotation() + ".\n\n";

    tablestring+="Start-interval:  [ " + ToString(x1) + " ; " + ToString(x2) + " ]\nMax. number of Iterations: " + ToString(n) +
        "\nPrecision: " + ToString(eps) + "\nRound to decimal: " + ToString(dez) + "\n";
    res = SecantMethod(f.f,x1,x2,eps,n);
    precision = intdigits(x2) + dez;
    precision = val::Min(precision, val::MaxPrec);
    if (res>=0) {
        tablestring+="\nSecant-method successful!\nNumber of Iterations: " + val::ToString(res) + "\n x = " + ToString(val::round(x2,dez),precision);
    }
    else if (res==-1) {
        tablestring+= "\nSecant-method failed!\n x = " + ToString(val::round(x2,dez),precision);
    }
    else tablestring+="\nSecant-method failed!";

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
    double x0=0,y0=0,alpha=0;

    val::d_array<char> separ({';',' '});
    //val::Glist<double>
    auto values = getdoublevaluesfromstring(input,separ);

    n = values.length();
    if (n == 0) return;
    if (n > 0) alpha = values[0];
    if (n > 1) x0 = values[1];
    if (n > 2) y0 = values[2];


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
            case plotobject::PARCURVE: {
                val::valfunction A00(val::ToString(A(0,0))), A01(val::ToString(A(0,1))), A10(val::ToString(A(1,0))), A11(val::ToString(A(1,1)));
                val::valfunction m0(val::ToString(m(0))), m1(val::ToString(m(1))), f1, g1;
                f1 = A00*(f->f-m0) + A01*(f->g -m1) + m0;
                g1 = A10*(f->f-m0) + A11*(f->g -m1) + m1;
                sf += "( " + f1.getinfixnotation() + " , " + g1.getinfixnotation() + " )  [ " + val::ToString(f->x_range.x) + " , " + val::ToString(f->x_range.y) + " ]";
            }

            default: break;
        }
        if (fmode != plotobject::PARCURVE) {
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
    double  a, b , c, a2, b2, c2, d, cr2, cr, ir2, ir, u, alpha, beta, gamma;
    val::vector<double> A(2), B(2), C(2), AB(2), AC(2), BC(2), I(2), U(2);
    Points.resize(2);

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

    // compute circumcenter and circumradius:
    d = 2.0*(AB(0)*AC(1) - AB(1)*AC(0));
    U(0) = (AC(1)*(AB(0)*AB(0) + AB(1)*AB(1)) - AB(1)*(AC(0)*AC(0) + AC(1)*AC(1)))/d;
    U(1) = (AB(0)*(AC(0)*AC(0) + AC(1)*AC(1)) - AC(0)*(AB(0)*AB(0) + AB(1)*AB(1)))/d;
    cr2 = U*U; cr = val::sqrt(cr2);
    U += A;
    U(0) = val::round(U(0),decimals); U(1) = val::round(U(1),decimals);
    Points[0].resize(2);
    Points[0][0].x = U(0); Points[0][0].y = U(1);

    // compute incenter and inradius
    I = ((a/u)*A + (b/u)*B + (c/u)*C);
    I(0) = val::round(I(0),decimals); I(1) = val::round(I(1),decimals);
    ir2 = ((b+c-a)*(a+c-b)*(a+b-c))/(4*u); ir = val::sqrt(ir2);
    Points[1].resize(2);
    Points[1][0].x = I(0); Points[1][0].y = I(1);

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

    analyze_output.resize(3);
    for (int i = 0; i < 3; ++i) analyze_output[i] = "";
    analyze_output[0] = "Points of triangle:\n A(" + val::ToString( A(0)) + "," + val::ToString(A(1)) +"); B(" +
        val::ToString(B(0)) + "," + val::ToString(B(1)) + "); C(" + val::ToString(C(0)) + "," + val::ToString(C(1)) + ").\n";
    analyze_output[0] += "|AB| = " + val::ToString(c) + "; |AC| = " + val::ToString(b) + "; |BC| = " + val::ToString(a) + ".\n";
    analyze_output[0] += "\n " + greek_letters[0] + " = " + val::ToString(alpha) + _T("° , ")+ greek_letters[1] +  " = " + val::ToString(beta) + _T("° , ") +
                          greek_letters[2] + " = " + val::ToString(gamma) + _T("°.");

    analyze_output[1] = "Circumcenter: (" + val::ToString(U(0)) + " , " + val::ToString(U(1)) + ")\nCircumradius: " + val::ToString(cr);
    analyze_output[2] = "Incenter: (" + val::ToString(I(0)) + " , " + val::ToString(I(1)) + ")\nInradius: " + val::ToString(ir);

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

    val::valfunction F(f.getinfixnotation());
    F.setparameter(f.f.getparameter());

    if (isfunction) {
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



// --------------------------- Funktionen zu myfunction ----------------------------------

/*
int isinteger(const double &a,const double &epsilon=1e-9)
{
    val::rational r = val::rational(a);
    if (r.denominator().iszero()) return 0;
    int h=int (val::integer(r));
    double d_h=double(h);
    //std::cout<<"\n d_h , a "<<d_h<<" , "<<a<<std::endl;
    if (val::abs(d_h-a)<epsilon) return 1;
    else return 0;
}



int checkbrackets(const std::string& s)
{
    int i=0,n=s.size(), k=0;

    for (i=0;i<n;++i) {
        if (s[i] == '(') k++;
        else if (s[i]==')') k--;
        else continue;
        if (k<0) return 0;
    }

    if (k!=0) return 0;

    return 1;
}


std::string findnumber(const std::string &s, int &i)
{
    int n=s.size(),e_set=0,p_set=0;
    std::string out;

    for (;i<n;i++) {
        if (s[i]>='0'  && s[i]<='9') out+=s[i];
        else if (s[i]=='e' && i<n-1 && s[i+1]!='x') {
            if (e_set) {i++; break;}
            //if (i==n-1) {i++;break;}
            if (s[i+1] == '-' || s[i+1]=='+') {
                out+=s[i];i++;out+=s[i];e_set=1;
            }
            else {out+=s[i];e_set=1;}
        }
        else if (s[i]==',' || s[i]=='.') {
            if (p_set) {i++;break;}
            out+=s[i];
            p_set=1;
        }
        else {break;}
    }

    return out;
}

int foundpattern(const std::string &s,const std::string &pattern, int& i)
{
    int is=1,n=s.size(),m=pattern.size(),j;

    if (i+m>n) return 0;

    for(j=0;i<n && j<m;++i,++j) {
        if (s[i]!=pattern[j]) {return 0;}
    }

    return is;
}


val::Glist<drawingword> getdrawingwords(const wxString &s)
{
    val::Glist<drawingword> G;
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
            if (s_ready) continue;
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
            G.push_back(lword);
            lword.sub_word = lword.sup_word = lword.word = "";
            s_ready = 0;
        }
    }
    if (lword.word != "") G.push_back(lword);
    return G;
}




//  --------------------------------------

myfunction::myfunction(const myfunction& f)
{
    val::GlistIterator<token> it;
    for (it=f.Gdat;it;it++) Gdat.inserttoend(it());
    t=f.t;
    s_infix=f.s_infix;
    textdata=f.textdata;
    TextWords = f.TextWords;
    nvar=f.nvar;
    withpar=f.withpar;
    isrational=f.isrational;
    isline=f.isline;
    istext=f.istext;
    mode=f.mode;
}

template <>
double myfunction::operator()<> (const double& x) const
{
    using namespace val;
    double value=0.0,v2;
    if (Gdat.isempty()) return 0.0;

    GlistIterator<myfunction::token> iT;
    Glist<double> G;
    int i;


    for (iT=Gdat;iT;iT++) {
        G.resetactual();
        if (iT().type==0) {
            if (iT().data=="PI") G.inserttohead(val::PI);
            else if (iT().data=="t") G.inserttohead(t);
            else G.inserttohead(val::FromString<double>(iT().data));
        }
        else if (iT().type==1) {G.inserttohead(x);} //std::cout<<"  variable ";}
        else {
            value=0.0;
            if (iT().data=="+") {   //case "+":
                if (!G.isempty()) {value=G.actualvalue();G.skiphead();}
                if (!G.isempty()) {value+=G.actualvalue();G.skiphead();}
                G.inserttohead(value);
            }
            else if (iT().data=="-") {  // case "-":
                if (!G.isempty()) {v2=G.actualvalue();G.skiphead();}
                if (!G.isempty()) {value=G.actualvalue();G.skiphead();value-=v2;}
                G.inserttohead(value);
            }
            else if (iT().data=="m") {
                if (!G.isempty()) G.actualvalue()*=-1.0;
            }
            else if (iT().data=="*") {  //case "*":
                if (!G.isempty()) {value=G.actualvalue();G.skiphead();}
                if (!G.isempty()) {value*=G.actualvalue();G.skiphead();}
                G.inserttohead(value);
            }
            else if (iT().data=="/") {  //case "/":
                if (!G.isempty()) {v2=G.actualvalue();G.skiphead();}
                if (!G.isempty()) {value=G.actualvalue();G.skiphead();value/=v2;}
                G.inserttohead(value);
            }
            else if (iT().data=="^") { //case "^":
                if (!G.isempty()) {v2=G.actualvalue();G.skiphead();}
                if (!G.isempty()) {
                    value=G.actualvalue();
                    G.skiphead();
                    if (value<0) {
                        if (isinteger(v2)) value=val::power(value,int(val::integer(val::rational(v2))));
                        else value=val::exp(value,v2);
                    }
                    else value=val::exp(value,v2);
                }
                G.inserttohead(value);
            }

            else if ((i = getindexoffunction(iT().data)) != -1) {
                if (!G.isempty()) G.actualvalue()=functionpairs[i].y(G.actualvalue());
            }
        }
    }
    G.resetactual();
    value=0.0;
    if (!G.isempty()) value=G.actualvalue();
    return value;
}


val::rationalfunction myfunction::getrationalfuncion() const
{
    using namespace val;
    rationalfunction v2,value=val::zero_element<rationalfunction>();
    if (Gdat.isempty()) return value;

    GlistIterator<myfunction::token> iT;
    Glist<rationalfunction> G;


    for (iT=Gdat;iT;iT++) {
        G.resetactual();
        if (iT().type==0) {
            if (iT().data=="t") G.inserttohead(rationalfunction(pol<rational>(rational(t),0)));
            else G.inserttohead(rationalfunction(pol<rational>(val::FromString<rational>(iT().data),0)));
        }
        else if (iT().type==1) {G.inserttohead(rationalfunction(pol<rational>(1,1)));} //std::cout<<"  variable ";}
        else {
            value=val::zero_element<rationalfunction>();
            if (iT().data=="+") {   //case "+":
                if (!G.isempty()) {value=G.actualvalue();G.skiphead();}
                if (!G.isempty()) {value+=G.actualvalue();G.skiphead();}
                G.inserttohead(value);
            }
            else if (iT().data=="-") {  // case "-":
                if (!G.isempty()) {v2=G.actualvalue();G.skiphead();}
                if (!G.isempty()) {value=G.actualvalue();G.skiphead();value-=v2;}
                G.inserttohead(value);
            }
            else if (iT().data=="m") {
                if (!G.isempty()) G.actualvalue()=-G.actualvalue();
            }
            else if (iT().data=="*") {  //case "*":
                if (!G.isempty()) {value=G.actualvalue();G.skiphead();}
                if (!G.isempty()) {value*=G.actualvalue();G.skiphead();}
                G.inserttohead(value);
            }
            else if (iT().data=="/") {  //case "/":
                if (!G.isempty()) {v2=G.actualvalue();G.skiphead();}
                if (!G.isempty()) {value=G.actualvalue();G.skiphead();value/=v2;}
                G.inserttohead(value);
            }
            else if (iT().data=="^") { //case "^":
                if (!G.isempty()) {v2=G.actualvalue();G.skiphead();}
                if (!G.isempty()) {
                    value=G.actualvalue();
                    G.skiphead();
                    value=val::power(value,int(val::integer(v2.nominator().LC())));
                }
                G.inserttohead(value);
            }
        }
    }
    G.resetactual();
    value=val::zero_element<rationalfunction>();
    if (!G.isempty()) value=G.actualvalue();
    return value;
}


// Anwendung des shunting - yard - Algortihmus:
const myfunction& myfunction::infix_to_postfix(const std::string &s)
{
    using namespace val;
    int i=0,n=s.size(),nG=0,j,failed=1;//syntax=0;
    std::string out="", sf;
    Glist<s_stack> G,OP;
    s_stack t,*tlast;
    s_stack::data_type tp;
    int prec;

    //

    //

    if (!Gdat.isempty()) Gdat.dellist();
    s_infix="";

    if (!checkbrackets(s)) return *this;//out;  //Fehler bei Klammerung;
    s_infix=s;
    for (i=n;i>0;--i) {
        if (s_infix[i-1]!=' ') break;
    }
    s_infix.resize(i);
    i=0;
    // Setze G in infix -Notation;

    while (i<n) {
        failed=0;
        //syntax=1;
        if (s[i]=='+') {
            t = s_stack("+",s_stack::OPERATOR,2);//G.inserttoend(s_stack("+",s_stack::OPERATOR,2));
            //nG++;
            i++;
        }
        else if (s[i]=='-') {
            if (G.isempty()) t = s_stack("m",s_stack::OPERATOR,2);//G.inserttoend(s_stack("m",s_stack::OPERATOR,2));
            else if (G[nG-1].data==")" || G[nG-1].type==0 || G[nG-1].type==1) t = s_stack("-",s_stack::OPERATOR,2);//G.inserttoend(s_stack("-",s_stack::OPERATOR,2));
            else t = s_stack("m",s_stack::OPERATOR,4);//G.inserttoend(s_stack("m",s_stack::OPERATOR,4));
            //nG++;
            i++;
        }
        else if (s[i]=='*' || s[i]=='/') {
            out="";
            out+=s[i];
            t = s_stack(out,s_stack::OPERATOR,3);//G.inserttoend(s_stack(out,s_stack::OPERATOR,3));
            //nG++;
            i++;
        }
        else if (s[i]=='^') {
            t = s_stack("^",s_stack::OPERATOR,5,0);//G.inserttoend(s_stack("^",s_stack::OPERATOR,5,0));
            //nG++;
            i++;
            int j=i;
            std::string out=findnumber(s,j);
            if (out=="") isrational=0;
            else {
                val::rational w=val::FromString<val::rational>(out);
                if (val::denominator(w)!=val::integer(1)) isrational=0;
            }
        }
        else if (s[i]=='(') {
            t = s_stack("(",s_stack::LBRACKET);//G.inserttoend(s_stack("(",s_stack::LBRACKET));
            //nG++;
            i++;
        }
        else if (s[i]==')') {
            t = s_stack(")",s_stack::RBRACKET);//G.inserttoend(s_stack(")",s_stack::RBRACKET));
            //nG++;
            i++;
        }
        else if (s[i]>='0' && s[i] <='9') {
            t = s_stack(findnumber(s,i),s_stack::NUMBER);//G.inserttoend(s_stack(findnumber(s,i),s_stack::NUMBER));
            //nG++;
        }
        else if (s[i]=='x') {
            t = s_stack("x",s_stack::VARIABLE);//G.inserttoend(s_stack("x",s_stack::VARIABLE));
            i++;
            //nG++;
        }
        else if (s[i]=='y') {
            t = s_stack("y",s_stack::VARIABLE);//G.inserttoend(s_stack("y",s_stack::VARIABLE));
            ++i;
            //nG++;
            nvar=2;
        }


        else if (s[i] == 't' && (i == n-1 || ( i < n-1 && s[i+1] != 'a' && s[i+1] != 'e' && s[i+1] != 'r'))) {
			t = s_stack("t",s_stack::NUMBER);
            withpar=1;
			++i;
		}
        else if ((sf = getstringfunction(s,i)) != "") {
			isrational = 0;
			if (sf == "PI") {
                t = s_stack(val::ToString(val::PI,20),s_stack::NUMBER);
                i += sf.length();
			}
			else {
                tp = s_stack::OPERATOR;
                prec = 5;
                if (sf == "line") {
                    isline=1;
                    mode=LINE;
                }
                else if (sf == "inf") {
                    tp = s_stack::NUMBER;
                    prec = 0;
                }
                else if (sf == "circle") {
                    mode = CIRCLE;
                }
                else if (sf == "text") {
                    istext=1;
                    mode=TEXT;
                }
                else if (sf == "triangle") {
                    mode=TRIANGLE;
                }
                else if (sf == "polygon") {
                    mode = POLYGON;
                }
                else if (sf == "points") mode = POINTS;
                else if (sf == "fill") mode = FILL;
                else if (sf == "rectangle") mode = RECTANGLE;

                t = s_stack(sf,tp,prec);
                if (sf == "text") {
                    j = i;
                    int nbrackets = 0;
                    for (i=j;i<n;++i) {
                        if (s[i] == '{') ++nbrackets;
                        else if (s[i]=='}') {
                            --nbrackets;
                            if (!nbrackets) {
                                ++i;
                                break;
                            }
                        }
                    }
                }
                else i+= sf.length();
			}
		}
        else {i++;failed=1;}
        if (!failed) {
            if (!G.isempty() && mode==FUNCTION) {
                tlast = &(G[nG-1]);
                if (t.data=="(") {
                    if (tlast->type==s_stack::NUMBER || tlast->data==")" || tlast->type==s_stack::VARIABLE) {
                        G.inserttoend(s_stack("*",s_stack::OPERATOR,3));
                        nG++;
                    }
                }
                else if (t.type==s_stack::OPERATOR && t.precedence==5 && t.data!="^") {
                    if (tlast->type==s_stack::NUMBER || tlast->data==")" || tlast->type==s_stack::VARIABLE) {
                        G.inserttoend(s_stack("*",s_stack::OPERATOR,3));
                        nG++;
                    }
                }
                else if (t.type==s_stack::VARIABLE) {
                    if (tlast->type==s_stack::NUMBER || tlast->data==")" || tlast->type==s_stack::VARIABLE) {
                        G.inserttoend(s_stack("*",s_stack::OPERATOR,3));
                        nG++;
                    }
                }
                else if (t.type==s_stack::NUMBER) {
                    if (tlast->type==s_stack::NUMBER || tlast->data==")" || tlast->type==s_stack::VARIABLE) {
                        G.inserttoend(s_stack("*",s_stack::OPERATOR,3));
                        nG++;
                    }
                }
            }
            G.inserttoend(t);
            nG++;
        }
    }

    // Nun hunting-yard-algorithmus:

    G.resetactual();
    //i=0;
    //if (foundpattern(s_infix,"line",i)) isline=1;
    if (isline || istext || mode==CIRCLE || mode==RECTANGLE || mode==TRIANGLE || mode==FILL || mode==POLYGON || mode==POINTS) {
        if (istext) settextdata();
        std::string s="",op;
        int n;
        double x,y;
        while (!G.isempty()) {
            t = G.actualvalue(); G.skiphead();G.resetactual();
            if (t.data=="m" || t.data=="-") {s="-";continue;}
            if (t.data=="*" || t.data=="/") {
                n=Gdat.length();
                if (!n) continue;
                if (G.isempty()) break;
                op=t.data;
                t = G.actualvalue(); G.skiphead();G.resetactual();
                x = FromString<double>(Gdat[n-1].data);
                y = FromString<double>(t.data);
                if (op=="*") x*=y;
                else x/=y;
                Gdat[n-1] = myfunction::token(val::ToString(x),0);
                //Gdat.inserttoend(myfunction::token(val::ToString(x),0));
                continue;
            }
            else if (t.type!=s_stack::NUMBER) continue;
            Gdat.inserttoend(myfunction::token(s+t.data,0));
            s="";
        }

        return *this;
    }

    // Hunting - yard algorithmus
    while (!G.isempty()) {
         t = G.actualvalue(); G.skiphead();G.resetactual();
         if (t.type==s_stack::NUMBER || t.type==s_stack::VARIABLE) Gdat.inserttoend(myfunction::token(t.data,int(t.type))); //out+= t.data + " ";
         else if (t.type==s_stack::OPERATOR) {
            while (!OP.isempty() && OP.actualvalue().type==s_stack::OPERATOR) {
                if (OP.actualvalue().precedence>t.precedence  || (OP.actualvalue().precedence==t.precedence && OP.actualvalue().leftassociativ)) {
                    Gdat.inserttoend(myfunction::token(OP.actualvalue().data,2));//out+=OP.actualvalue().data + " ";
                    OP.skiphead(); OP.resetactual();
                }
                else break;
            }
            OP.inserttohead(t); OP.resetactual();
         }
         else if (t.type==s_stack::LBRACKET) {OP.inserttohead(t);OP.resetactual();}
         else if (t.type==s_stack::RBRACKET) {
            while (!OP.isempty() && OP.actualvalue().type!=s_stack::LBRACKET) {
                Gdat.inserttoend(myfunction::token(OP.actualvalue().data,int(OP.actualvalue().type)));//out+= OP.actualvalue().data + " ";
                OP.skiphead();OP.resetactual();
            }
            if (!OP.isempty()) {OP.skiphead();OP.resetactual();}
         }
    }

    while (!OP.isempty()) {
        Gdat.inserttoend(myfunction::token(OP.actualvalue().data,int(OP.actualvalue().type)));//out+=OP.actualvalue().data + " ";
        OP.skiphead();OP.resetactual();
    }

    return *this;
}


int myfunction::isrationalfunction() const
{
    int n = s_infix.length(),nklammerauf=0,nklammerzu=0;

    if (n==0) return 1;
    if (!isrational) return 0;

    for (int i=0;i<n;++i) {
        if (s_infix[i]=='a') {
            if (foundpattern(s_infix,"abs",i)) {
                for (;i<n;++i) {
                    if (s_infix[i]=='(') nklammerauf++;
                    if (s_infix[i]=='x') return 0;
                    if (s_infix[i]==')') nklammerzu++;
                    if (nklammerauf==nklammerzu) break;
                }
            }
        }
    }

    return 1;
}


int myfunction::ispolynomialfunction() const
{
    int n = s_infix.length(),nklammerauf=0,nklammerzu=0;

    if (n==0) return 1;
    if (!isrationalfunction()) return 0;

    for (int i=0;i<n;++i) {
        if (s_infix[i]=='/') {
            if (i==n-1) return 1;
            ++i;
            for (;i<n;++i) {
                if (s_infix[i]=='(') nklammerauf++;
                if (s_infix[i]=='x') return 0;
                if (s_infix[i]==')') nklammerzu++;
                if (nklammerauf==nklammerzu) break;
            }
        }
    }
    return 1;
}




int myfunction::isdifferentiable() const
{
    int n = Gdat.length();
    for (int i = 0; i < n; ++i) {
        if (Gdat[i].data == "abs") {
            if (i < n-1 && Gdat[i+1].data == "log") continue;
            else return 0;
        }
    }
    return 1;
}



void myfunction::getLinePoints(double &x1,double &y1,double &x2,double &y2,double &arrow) const
{
    using namespace val;
    GlistIterator<myfunction::token> iT;

    iT=Gdat;
    x1=x2=y1=y2=0;
    arrow = 0.0;


    while  (iT.actualvalid()) {
        if (iT().type==0) {
            x1=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            y1=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            x2=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }

    while  (iT.actualvalid()) {
        if (iT().type==0) {
            y2=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }

    while  (iT.actualvalid()) {
        if (iT().type==0) {
            arrow = FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
}

void myfunction::getTrianglePoints(double &x1,double &y1,double &x2,double &y2,double &x3,double &y3) const
{
    using namespace val;
    GlistIterator<myfunction::token> iT;

    iT=Gdat;
    x1=x2=y1=y2=x3=y3=0;
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            x1=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            y1=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            x2=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }

    while  (iT.actualvalid()) {
        if (iT().type==0) {
            y2=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            x3=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }

    while  (iT.actualvalid()) {
        if (iT().type==0) {
            y3=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
}


void myfunction::getCirclePoints(double& x1,double&y1,double &r,double &angle1, double &angle2, int &slice) const
{
    using namespace val;
    GlistIterator<myfunction::token> iT;

    iT=Gdat;
    x1=y1=angle1=0;angle2=360;
    r=1.0;
    slice = 0;


    while  (iT.actualvalid()) {
        if (iT().type==0) {
            x1=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            y1=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            r=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }

    while  (iT.actualvalid()) {
        if (iT().type==0) {
            angle1=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            angle2=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            slice = FromString<int>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }

}


void myfunction::getTextPoint(double &x1,double &y1) const
{
    using namespace val;
    GlistIterator<myfunction::token> iT;

    iT=Gdat;
    x1=y1=0;
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            x1=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            y1=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
}


void myfunction::getFillPoints(double& x,double& y, double& transparancy) const
{
    using namespace val;
    GlistIterator<myfunction::token> iT;

    iT=Gdat;
    x=y=0;
    transparancy=1;
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            x=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            y=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
    while  (iT.actualvalid()) {
        if (iT().type==0) {
            transparancy=FromString<double>(iT().data);
            iT.moveactual();
            break;
        }
        iT.moveactual();
    }
}

val::d_array<double> myfunction::getPolygonPoints() const
{
    using namespace val;
    val::Glist<double> P;

    for (const auto& iT : Gdat) {
        if (iT.type==0) {
            P.push_back(FromString<double>(iT.data));
        }
    }

    int n=P.length(),i=0;
    if (n%2) n--;
    if (n<=0) return d_array<double>();
    d_array<double> points(n);

    for (const auto& x : P) {
        points[i]=x;
        ++i;
        if (i==n) break;
    }
    return points;
}

void myfunction::settextdata()
{
    textdata = getstringfrombrackets(s_infix, '{', '}');

    for (int i = 0; i < greek_literals.length(); ++i) {
        //if (textdata.Find(greek_literals[i]) != wxNOT_FOUND) textdata.Replace(greek_literals[i], greek_letters[i]);
        textdata.Replace(greek_literals[i], greek_letters[i]);
    }
    TextWords = getdrawingwords(textdata);

    return;
}
*/

// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------

const val::d_array<std::string> plotobject::s_object_type{"line", "text", "circle", "rectangle", "triangle", "fill", "polygon", "points"};
const val::d_array<int> plotobject::defnpoints{5,2,6,4,6,3,2,2};

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
        if (!getpiscale(values[0],factor,x_range.x,0))  x_range.x = val::FromString<double>(values[0]);
        x_range.y = x_range.x;
    }
    if (n > 1) {
        if (!getpiscale(values[1],factor,x_range.y,0)) x_range.y = val::FromString<double>(values[1]);
    }

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
