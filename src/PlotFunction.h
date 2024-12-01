#ifndef PLOTFUNCTION_H_INCLUDED
#define PLOTFUNCTION_H_INCLUDED



#include <wx/frame.h>
#include <string>
#include <d_array.h>
#include <Glist.h>
#include <analysis.h>
#include <val_utils.h>
#include <val_basics.h>
#include <pol.h>
#include <val_filesys.h>
#include <matrix.h>
#include <fraction.h>
#include <MyTime.h>
#include <valfunction.h>
#include <mutex>
#include <trie.h>


class MyThreadEvent;
//class myfunction;
struct plotobject;


extern wxFrame *MyFrame;
extern wxString tablestring;
extern std::string fstring,xstring,ystring,filesep,filedir,sizestring,settingsfile,settingsdir,valdir,iconpath,savefiledir,
                   openfiledir,handcursor,RecentFilesPath,alticonpath, RecentCommandsPath, errorfile, ansexpr;

extern std::mutex compute_mutex;
extern plotobject global_function;
//extern val::d_array<val::Glist<val::GPair<double>>> critpoints;//,undef_intervals;
//extern val::d_array<val::d_array<double>> critx;
extern val::d_array<wxString> analyze_output;
extern val::d_array<val::d_array<val::GPair<double>>> Points;
extern const val::d_array<wxString> greek_literals;
extern const val::d_array<wxString> greek_letters;
extern const val::d_array<wxColour> defaultcolors;
extern const val::d_array<wxString> defaultcolornames;
extern const val::d_array<wxString> CommandsList;
extern const val::d_array<wxString> SettingsList;
extern const val::d_array<wxString> SettingsParList;
extern const val::d_array<wxString> CommandsParList;
extern val::Glist<wxString> recentcommands;
extern const val::d_array<wxString> WordList;
extern const val::trie_type<wxString> WordTree;
extern const val::trie_type<wxString> InputDialogTree;


enum val_colors{BLUE,RED,GREEN,LBLUE,ORANGE,VIOLET,GREY,WHITE,BlACK,LGREY,YELLOW,DGREEN,BEIGE,BROWN,DBLUE,PINK,SKYBLUE,LGREEN,MAGENTA,OLIVE,PURPLE,GOLD,CYAN};

enum val_settings{AXIS_SCALE,AXIS_COLOR,GRID_SCALE,GRID_COLOR,VALUES_NUMBER,AXIS_RANGE,SHOW_X_AXIS,SHOW_Y_AXIS,SHOW_GRID,SHOW_X_SCALE,
                    SHOW_Y_SCALE,RESET_COLORS,FONT_SIZE,FUNCTION_COLOR,PANEL_SIZE,AXIS_NAMES,REGRESSION_DEGREE,POINT_DECIMALS,
                    SHOW_FUNCTION,BACKGROND_COLOR,PARAMETER_VALUES,FUNCTION_SIZE,MARGIN,AXIS_FONTSIZE,FUNCTION_SETTINGS,MOVEINC};

enum val_commands{DERIVE,ANALYZE,TANGENT,NORMAL,INTERPOLATION,REGRESSION,TABLE,INTEGRAL,ARCLENGTH,ZERO_ITERATION,MOVE,EVALUATE,INTERSECTION,CALCULATE,ROTATE};

wxDECLARE_EVENT(MY_EVENT, MyThreadEvent);

class MyThreadEvent: public wxThreadEvent
{
public:
        MyThreadEvent(wxEventType commandType, int id = 0)
                    :  wxThreadEvent(commandType, id) { }

        // You *must* copy here the data to be transported
        MyThreadEvent(const MyThreadEvent& event)
                    :  wxThreadEvent(event) { this->SetMessage(event.GetMessage()); }

        // Required for sending with wxPostEvent()
        wxEvent* Clone() const { return new MyThreadEvent(*this); }

        std::string GetMessage() const { return message; }
        void SetMessage(const std::string &s) { message = s; }
private:
        std::string message;
};

enum myevent_id{IdPaint,IdTable,IdIntegral,IdRefresh,IdIteration,IdAnalyze,IdInfo,IdEval, IdIntersection, IdCalculate, IdPointStat, IdTriangle};


void inserttocand(val::Glist<val::GPair<int>> &cand,const val::GPair<int> &q,int y);

namespace val
{
val::rational abs(const val::rational&);
int operator <(const val::GPair<double>& p,const val::GPair<double>& q);
const int MaxPrec = 19;
}

double squaredistance(const wxPoint& l1, const wxPoint &l2, const wxPoint &p); // square distance between line-segment def. by l1, l2 and point p
double squaredistance(const wxPoint& m, const wxPoint &p, int rx, int ry, int angle);
val::GPair<double> coordinatesdistance(const double &mx, const double &my, const double &px, const double &py,const double &r, int angle);
double degree_angle(const double &a, const double &b); // angle in degree of vector (a,b) and (1,0)

int decimal(const double& x);

int intdigits(double x);

int getpiscale(const std::string& s,val::rational &factor,double &scale,int pos = 1);

int isInf(const double& a);

//int isderived(const std::string &s);

//int isderived(const myfunction &f);

//int islinearfunction(const myfunction &F);

int isinarray(const val::rational& value,const val::d_array<val::rational> & x);

double derive(const plotobject &f,const double& x);

val::rationalfunction derive(const val::rationalfunction &f);

val::rational eval(const val::rationalfunction &F,const val::rational &x);

void gettangentvalues(const plotobject &f,const double &x,double &m,double &b,int tangent=1);

std::string extractstringfrombrackets(std::string &sf,const char lb, const char rb);

std::string getstringfrombrackets(const std::string &sf,const char lb, const char rb);

// Returns color index if specified, -1 else.
int getfunctionfromstring(std::string &fstring, plotobject &f);

val::Glist<double> getdoublevaluesfromstring(const std::string &sf,const val::d_array<char>& separators, int emptystring = 0);

val::Glist<std::string> getwordsfromstring(const std::string &sf,const val::d_array<char>& separators, int emptywords = 0,
                                           const val::d_array<char> &ignore = val::d_array<char>());

val::matrix<val::rational> set_les(const std::string &s);

val::pol<val::rational> interpolation(const std::string &s);

std::string delcharfromstring(const std::string& s,const char z=' ');

val::Glist<char> substitutepar(std::string &s);

void back_substitutepar(std::string &s, const val::Glist<char> &VarList, int nvariables);


void computepoints(val::Glist<plotobject> &F, int points,const double &x1,const double &x2,double &ymax,double &ymin,int activef,int comppoints);

void computetable(const plotobject& f,double x1,double x2,double dx);

void computetable_rat(const plotobject& f,val::rational x1,val::rational x2,val::rational dx);

void computeevaluation(const plotobject& f, double par);

void calculate(std::string s);

void computeinterpolation(std::string input,std::string &Fstring);

void computezeroiteration(const plotobject& f,double x1,double x2,double eps,int n,int dez);

void computerotation(const val::d_array<plotobject*> F,std::string input);

void computeregression(const plotobject& f,int degree=1);

void computepointstatistics(const plotobject& f, std::string input);

void analyze_triangle(const plotobject &f, std::string input);

void computetangent(std::string sf,const plotobject& f,double x1,double x2,int tangent=1);

template <class T>
double kepler_simpson_sum(const T& f,const double& a,const double& b,int n);


template <class T>
double integral(const T& f,const double &a,const double &b,int iter = 50, const double& delta=1e-8);

// Newton-Iteration.
// return : -2 method failed, number of iterations not reached:
//          -1 number of iterations reached, but |f(x) > eps|; iteration failed
//          i = number of required iterations; iteration succesful.
// f1 = f'
template<class T>
int NewtonIteration(const T& f, const T &f1, double &x, const double& eps = 1e-9, int n = 15);


// Iterationsverfahren zur Bestimmung einer Nullstelle nach der Sekanten-Methode:
// Rückgabe: -2 Iterationsgrenze wird nicht erreicht, Methode fehlgeschlagen.
//           -1 Iterationsgrenze erreicht aber |f(x1)>eps|
//            i = Anzahl der Iterationen,  |f(x1)| <= eps.
//template <class T>
//int SecantMethod(const T& f,double& x0,double& x1,const double& eps,int n);

//----------------------------------------------------------------------------------------------------------------------------------------

struct drawingword
{
    wxString word, sub_word, sup_word;
};

//-----------------------------   class myfunction ----------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------------------------------------------------


template <class T>
double kepler_simpson_sum(const T& f,const double& a,const double& b,int n)
{
    if (n<=0) return 0.0;
    if (a==b) return 0.0;

    double h=(b-a)/(double(2*n)),wert1=0.0,wert2=0.0,x=a;
    int i,m=2*n;

    for (i=2;i<m;i+=2) {
        x+= h;
        wert1+=f(x);
        x+=h;
        wert2+=f(x);
    }
    x+=h;
    wert1+=f(x);
    wert1 *= 4.0;
    wert2 *= 2.0;
    wert1+=wert2+f(a)+f(b);
    wert1*=(h*0.333333333333333333333);
    return wert1;
}


template <class T>
double integral(const T& f,const double &a,const double &b,int iter, const double& delta)
{
    int n=7;
    double wert1,wert2;

    wert1=kepler_simpson_sum(f,a,b,n);

    do {
        n++;
        wert2=kepler_simpson_sum(f,a,b,n);
        if (val::abs(wert2-wert1)<delta) break;
        wert1=wert2;
    } while (n<=iter);

    return wert2;
}

template<class T>
int NewtonIteration(const T& f, const T &f1, double &x, const double& eps, int n)
{
    int i;
    double y1, y;
    for (i = 0; i < n+1; ++i) {
        y = f(x);
        if (val::abs(y) < eps) return i;
        y1 = f1(x);
        if (val::abs(y1) < eps) return -2;
        x -= y/y1;
    }
    return -1;
}

// ---------------------------------------------------------------------------------------------------------------------------------------------------

struct plotobject
{
    enum modetype {LINE,TEXT,CIRCLE,RECTANGLE,TRIANGLE,FILL,POLYGON,POINTS,HISTOGRAM,BITMAP,PARCURVE,ALGCURVE,FUNCTION};
    //
    val::valfunction f,g,x1,x2;
    wxBitmap bitmap;
    val::d_array<double> farray, critx;
    val::d_array<val::d_array<double> > curvearray;
    val::Glist<val::GPair<double>> critpoints;//,undef_intervals;
    val::Glist<drawingword> TextWords;
    wxString textdata;
    std::string s_infix;
    val::GPair<double> x_range{0.0,0.0};
    int objectype = FUNCTION, islinear = 0;
    //
    static const val::d_array<std::string> s_object_type;
    static const val::d_array<int> defnpoints;               // default number number of points for each objectype
    static val::Glist<wxImage> image;           // replace myimage with wxImage
    //
    // Constructors:
    plotobject() = default;
    explicit plotobject(const std::string &);
    //
    const std::string& getinfixnotation() const {return s_infix;}
    int is_empty() const {return s_infix == "";}
    int getmode() const {return objectype;}
    int iswithparameter() const;
    int IsLine() const {return (objectype==LINE);}
    int IsText() const {return (objectype==TEXT);}
    int IsCircle() const {return (objectype==CIRCLE);}
    int IsRectangle() const {return (objectype==RECTANGLE);}
    int IsTriangle() const {return (objectype==TRIANGLE);}
    int IsFill() const {return (objectype==FILL);}
    int IsPolygon() const {return objectype==POLYGON;}
    int IsPoints() const {return objectype==POINTS;}
    int IsHistogram() const {return objectype==HISTOGRAM;}
    int IsParcurve() const {return objectype==PARCURVE;}
    int IsFunction() const {return objectype==FUNCTION;}
    int IsAlgCurve() const {return objectype==ALGCURVE;}
    //
    val::pol<double> getpol(const double& x) const;
    //
    void setdrawingwords(const wxString &s);    // replace std::string with wxString
};



#endif // PLOTFUNCTION_H_INCLUDED
