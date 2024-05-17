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
class myfunction;


extern wxFrame *MyFrame;
extern wxString tablestring;
extern std::string fstring,xstring,ystring,filesep,filedir,sizestring,settingsfile,settingsdir,valdir,iconpath,savefiledir,
                   openfiledir,handcursor,RecentFilesPath,alticonpath, RecentCommandsPath, errorfile, ansexpr;

extern std::mutex compute_mutex;
extern myfunction global_function;
extern val::d_array<val::Glist<val::GPair<double>>> critpoints;//,undef_intervals;
extern val::d_array<val::d_array<double>> critx;
extern val::d_array<wxString> analyze_output;
extern val::d_array<val::d_array<val::GPair<double>>> Points;
extern const val::d_array<wxString> greek_literals;
extern const val::d_array<wxString> greek_letters;
extern const val::d_array<wxColour> defaultcolors;
extern const val::d_array<std::string> defaultcolornames;
extern const val::d_array<std::string> CommandsList;
extern const val::d_array<std::string> SettingsList;
extern const val::d_array<std::string> SettingsParList;
extern const val::d_array<std::string> CommandsParList;
extern val::Glist<std::string> recentcommands;
//extern const val::d_array<std::string> CommandsManList;

extern const val::d_array<std::string> WordList;
extern const val::trie_type<std::string> WordTree;
extern const val::trie_type<std::string> InputDialogTree;


//static std::mutex computemutex;


enum val_colors{BLUE,RED,GREEN,LBLUE,ORANGE,VIOLET,GREY,WHITE,BlACK,LGREY,YELLOW,DGREEN,BEIGE,BROWN,DBLUE,PINK,SKYBLUE,LGREEN};

enum val_settings{AXIS_SCALE,AXIS_COLOR,GRID_SCALE,GRID_COLOR,VALUES_NUMBER,AXIS_RANGE,SHOW_X_AXIS,SHOW_Y_AXIS,SHOW_GRID,SHOW_X_SCALE,
                    SHOW_Y_SCALE,RESET_COLORS,FONT_SIZE,FUNCTION_COLOR,PANEL_SIZE,AXIS_NAMES,REGRESSION_DEGREE,POINT_DECIMALS,
                    SHOW_FUNCTION,BACKGROND_COLOR,PARAMETER_VALUES,FUNCTION_SIZE,MARGIN,AXIS_FONTSIZE,FUNCTION_SETTINGS,MOVEINC};

enum val_commands{DERIVE,ANALYZE,TANGENT,NORMAL,INTERPOLATION,REGRESSION,TABLE,INTEGRAL,ARCLENGTH,ZERO_ITERATION,MOVE,EVALUATE,INTERSECTION,CALCULATE};

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

int isderived(const std::string &s);

int isderived(const myfunction &f);

int islinearfunction(const myfunction &F);

int isinarray(const val::rational& value,const val::d_array<val::rational> & x);

double derive(const myfunction &f,const double& x);

val::rationalfunction derive(const val::rationalfunction &f);

val::rational eval(const val::rationalfunction &F,const val::rational &x);

void gettangentvalues(const myfunction &f,const double &x,double &m,double &b,int tangent=1);

std::string extractstringfrombrackets(std::string &sf,const char lb, const char rb);

std::string getstringfrombrackets(const std::string &sf,const char lb, const char rb);

// Returns color index if specified, -1 else.
int getfunctionfromstring(std::string &fstring,std::string& f_s,double &x1,double &x2);

val::Glist<double> getdoublevaluesfromstring(const std::string &sf,const val::d_array<char>& separators, int emptystring = 0);

val::Glist<std::string> getwordsfromstring(const std::string &sf,const val::d_array<char>& separators, int emptywords = 0,
                                           const val::d_array<char> &ignore = val::d_array<char>());

val::matrix<val::rational> set_les(const std::string &s);

val::pol<val::rational> interpolation(const std::string &s);

std::string delcharfromstring(const std::string& s,const char z=' ');

void computepoints(const val::Glist<myfunction> &F,val::d_array<val::d_array<double>> &farray,val::d_array<val::d_array<val::d_array<double> > > &carray,
                   int points,const double &x1,const double &x2,double &ymax,double &ymin,int activef,int comppoints);

void computetable(const myfunction& f,double x1,double x2,double dx);

void computetable_rat(const myfunction& f,val::rational x1,val::rational x2,val::rational dx);

void computeevaluation(const myfunction& f, double par);

void calculate(std::string s);

void computeinterpolation(std::string input,std::string &Fstring);

void computezeroiteration(const myfunction& f,double x1,double x2,double eps,int n,int dez);

void computerotation(const val::d_array<myfunction*> F,std::string input);

void computeregression(const myfunction& f,int degree=1);

void computepointstatistics(const myfunction& f, std::string input);

void analyze_triangle(const myfunction &f, std::string input);

void computetangent(std::string sf,const myfunction& f,double x1,double x2,int tangent=1);

template <class T>
double kepler_simpson_sum(const T& f,const double& a,const double& b,int n);

template <class T>
double integral(const T& f,const double &a,const double &b,int iter = 50, const double& delta=1e-8);

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


class myfunction
{
public:
    myfunction() = default;
    explicit myfunction(const std::string &s) {infix_to_postfix(s);}
    myfunction(const myfunction&);
    myfunction(myfunction &&f) {Gdat=std::move(f.Gdat);t=std::move(f.t);s_infix=std::move(f.s_infix);textdata=std::move(f.textdata);TextWords=std::move(f.TextWords);nvar=f.nvar;withpar=f.withpar;isrational=f.isrational;isline=f.isline;istext=f.istext;mode=f.mode;}// Gdat(std::move(f.Gdat)), t(std::move(f.t)), s_infix(std::move(f.s_infix)) {nvar=f.nvar;}//{Gdat=std::move(f.Gdat);t=f.t;}
    const myfunction& operator=(const myfunction&) = delete;
    const myfunction& operator= (myfunction&& f) {Gdat=std::move(f.Gdat);s_infix=std::move(f.s_infix);textdata=std::move(f.textdata);TextWords=std::move(f.TextWords);nvar=f.nvar;withpar=f.withpar;isrational=f.isrational;isline=f.isline;istext=f.istext;mode=f.mode;return *this;}
    const myfunction& infix_to_postfix(const std::string &s);
    template <class T> T operator() (const T&) const;
    template <class T> val::pol<T> getpol(const T& a,const char var='x') const; // Polynom p(Y) = f(a,Y);
    val::rationalfunction getrationalfuncion() const;
    void getLinePoints(double& x1,double &y1,double &x2,double &y2,double &arrow) const;
    void getTrianglePoints(double &x1,double &y1,double &x2,double &y2,double &x3,double &y3) const;
    void getTextPoint(double &x1,double &y1) const;
    void getCirclePoints(double& x1,double&y1,double &r,double &angle1, double &angle2,int &slice) const;
    void getFillPoints(double& x,double& y, double& transparancy) const;
    val::d_array<double> getPolygonPoints() const;
    const wxString& getTextData() const {return textdata;}
    const val::Glist<drawingword>& getTextWords() const {return TextWords;}
    void setparameter(const double &a) {t=a;}
    const double& getparameter() const {return t;}
    const std::string& getinfixnotation() const {return s_infix;}
    int numberofvariables() const {return nvar;}
    int iszero() const {return Gdat.isempty();}
    int iswithparameter() const {return withpar;}
    int israt() const {return isrational;}
    int isrationalfunction() const;
    int ispolynomialfunction() const;
    int isdifferentiable() const;
    int IsLine() const {return (mode==LINE);}
    int IsText() const {return (mode==TEXT);}
    int IsCircle() const {return (mode==CIRCLE);}
    int IsRectangle() const {return (mode==RECTANGLE);}
    int IsTriangle() const {return (mode==TRIANGLE);}
    int IsFill() const {return (mode==FILL);}
    int IsPolygon() const {return mode==POLYGON;}
    int IsPoints() const {return mode==POINTS;}
    enum modetype {FUNCTION,LINE,TEXT,CIRCLE,RECTANGLE,TRIANGLE,FILL,POLYGON,POINTS};
    modetype getmode() const {return mode;}
    //void print() const;
private:
    struct token {
        std::string data="";
        int type=0;
        token() = default;
        token(const std::string &s,const int t) : data(s),type(t) {}
    };
    struct s_stack {
        std::string data="";
        enum data_type {NUMBER,VARIABLE,OPERATOR,LBRACKET,RBRACKET};
        data_type type=NUMBER;
        int precedence=0;
        int leftassociativ=0;
        s_stack() = default;
        s_stack(const std::string& s,data_type t,int p=0,int l=1) : data(s), type(t), precedence(p),leftassociativ(l) {}
    };
    double t=0;
    val::Glist<token> Gdat;
    std::string s_infix="";
    wxString textdata="";
	val::Glist<drawingword> TextWords;
    int nvar=1,withpar=0,isrational=1,isline=0,istext=0;
    modetype mode=FUNCTION;
    void settextdata();
};

template <>
double myfunction::operator() (const double &x) const;


template <class T>
T myfunction::operator() (const T& x) const
{
    using namespace val;
    T v2,value=val::zero_element<T>();
    if (Gdat.isempty()) return value;

    GlistIterator<myfunction::token> iT;
    Glist<T> G;


    for (iT=Gdat;iT;iT++) {
        G.resetactual();
        if (iT().type==0) {
            if (iT().data=="t") G.inserttohead(T(t));
            else G.inserttohead(val::FromString<T>(iT().data));
        }
        else if (iT().type==1) {G.inserttohead(x);} //std::cout<<"  variable ";}
        else {
            value=val::zero_element<T>();
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
                    value=val::power(value,int(val::integer(val::rational(v2))));
                }
                G.inserttohead(value);
            }
            else if (iT().data=="abs") {
                if (!G.isempty()) G.actualvalue()=val::abs(G.actualvalue());
            }
        }
    }
    G.resetactual();
    value=val::zero_element<T>();
    if (!G.isempty()) value=G.actualvalue();
    return value;
}

template <class T>
val::pol<T> myfunction::getpol(const T& a,const char var) const
{
    using namespace val;
    pol<T> Y(T(1),1),value,v2;  //
    int exponent;

    if (Gdat.isempty()) return value;

    GlistIterator<myfunction::token> iT;
    Glist<pol<T>> G;


    for (iT=Gdat;iT;iT++) {
        G.resetactual();
        if (iT().type==0) {
            if (iT().data=="t") G.inserttohead(pol<T>(T(t),0));
            else G.inserttohead(pol<T>(val::FromString<T>(iT().data),0));
        }
        else if (iT().type==1) {
            if (iT().data==var) G.inserttohead(pol<T>(a,0));
            else G.inserttohead(Y);
        } //std::cout<<"  variable ";
        else {
            value.del();  // value=0
            if (iT().data=="+") {   //case "+":
                if (!G.isempty()) {value=std::move(G.actualvalue());G.skiphead();}
                if (!G.isempty()) {value+=G.actualvalue();G.skiphead();}
                G.inserttohead(value);
            }
            else if (iT().data=="-") {  // case "-":
                if (!G.isempty()) {v2=G.actualvalue();G.skiphead();}
                if (!G.isempty()) {value=G.actualvalue();G.skiphead();value-=v2;}
                G.inserttohead(std::move(value));
            }
            else if (iT().data=="m") {
                if (!G.isempty()) G.actualvalue()*=T(-1);
            }
            else if (iT().data=="*") {  //case "*":
                if (!G.isempty()) {value=std::move(G.actualvalue());G.skiphead();}
                if (!G.isempty()) {value*=G.actualvalue();G.skiphead();}
                G.inserttohead(std::move(value));
            }
            else if (iT().data=="/") {  //case "/":
                if (!G.isempty()) {v2=std::move(G.actualvalue());G.skiphead();}
                if (!G.isempty()) {value=G.actualvalue();G.skiphead();value/=v2;}
                G.inserttohead(std::move(value));
            }
            else if (iT().data=="^") { //case "^":
                if (!G.isempty()) {v2=std::move(G.actualvalue());G.skiphead();}
                exponent=int(v2.leader());
                if (!G.isempty()) {
                    value=G.actualvalue();
                    G.skiphead();
                    //value=val::power(value,int(val::integer(val::rational(v2))));
                    value=val::power(value,exponent);
                }
                G.inserttohead(std::move(value));
            }
        }
    }
    G.resetactual();
    value.del();
    if (!G.isempty()) value=std::move(G.actualvalue());
    return value;
}


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

#endif // PLOTFUNCTION_H_INCLUDED
