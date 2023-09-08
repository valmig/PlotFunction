#include "calculus.h"
#include "PlotFunction.h"
#include <Glist.h>
#include <d_array.h>
#include <fraction.h>
#include <valfunction.h>
#include <rational.h>
#include <val_basics.h>
#include <val_utils.h>
#include <vector.h>
#include <analysis.h>
#include <polfactor.h>
#include <pol_arithmetic.h>
#include <LA.h>


namespace val
{


std::istream& operator >>(std::istream& is,valfunction &f)
{
	std::string s;
	is >> s;
	f = valfunction(s);
	return is;
}

std::ostream& operator <<(std::ostream& os,valfunction &f)
{
	os << f.getinfixnotation();
	return os;
}


int operator ==(const val::valfunction &f, const val::valfunction &g)
{
	return (f-g).is_zero();
}

int operator !=(const val::valfunction &f, const val::valfunction &g)
{
	return !(f == g);
}

valfunction& operator *=(valfunction &f, const valfunction &g)
{
	f = f * g;
	return f;
}

valfunction& operator /=(valfunction &f, const valfunction &g)
{
	f = f / g;
	return f;
}



void set_unity_element(valfunction &f)
{
	f = valfunction("1");
}

} // end namespace val



// Computes greatest nat. number  m, with m<=sqrt(n), by bisection-method in [a,b].
val::integer sqrt(const val::integer& n)
{
 using namespace val;
 integer a,a1,c,d,b=n;

 while ((d=(b-a))>1) {
	 a1=a+d/integer(2);
	 c=a1*a1;
	 if (c>n) b=std::move(a1);
	 else a=std::move(a1);
 }
 if ((b*b)>n) return a;
 else return b;
}


int sortfactors(val::d_array<val::pol<val::rational>> &factors, int &r, int &s)
{
    using namespace val;
    r = s = 0;
    for (const auto & f : factors) {
        if (f.degree() == 1) ++r;
        else if (f.degree() == 2) ++s;
        else return 0;
    }
    if (s == 0 || r == 0) return 1;
    int n = factors.length(),j;
    d_array<pol<rational>> hfactors(n);

    j=0;
    for (auto &f : factors) {
        if (f.degree() == 1) {
            hfactors[j] = std::move(f);
            ++j;
        }
    }
    j=0;
    for (auto &f : factors) {
        if (f.degree() == 2) {
            hfactors[j+r] = std::move(f);
            ++j;
        }
    }
    factors = std::move(hfactors);

    return 1;
}



int isexprational(const val::valfunction &f)
{
    if (f.is_zero()) return 0;

    std::string op = f.getfirstoperator();

    if (op=="exp" && f.getfirstargument().isrationalfunction()) return 1;
    if (op=="*" && f.getfirstargument().isrationalfunction()) return isexprational(f.getsecondargument());
    if (op=="m") return isexprational(f.getfirstargument());
    if (op=="/") {
        return (isexprational(f.getfirstargument()) && f.getsecondargument().isrationalfunction());
    }

    return 0;
}

std::string factorize(const val::pol<val::rational> &f)
{
    if (f.degree()==0 || f.iszero()) return val::ToString(f.LC());
    if (f.degree()==1) return val::PolToString(f);
    std::string s,s_prefix;
    val::d_array<val::pol<val::rational>> factors = val::polfactor(f);
    val::pol<val::rational> h;
    val::rational cont = val::content(f);
    int e,brackets,r,signf,sign;

    signf = f.LC().signum();
    sign = cont.signum();

    //for (const auto& g : factors) sign*=g.LC().signum();

    h = f;
    //::WriteText(val::ToString(factors.length()));
    r=factors.length();
    for (int i=0;i<r;++i) {
        e=0;
        //gr=val::toRationalPolynom(std::move(g));

        while ((h%factors[i]).iszero())
        {
            //val::divrest(h,factors[i],q,r);
            //::WriteText("\n" + val::ToString(q));
            h/=factors[i];
            ++e;
            sign*= factors[i].LC().signum();
            //h=std::move(q);
        }
        brackets=0;
        if (factors[i].length()>1) {
            s+="(";
            brackets=1;
        }
        s+=val::PolToString(factors[i]);
        if (brackets) s+=")";
        if (e>1) {
            s+="^"+val::ToString(e);
        }
        if (i<r-1) s+=" ";
    }

    if (sign != signf) cont.changesign();
    if (cont == val::rational(-1)) s_prefix += "-";
    else if (cont != val::rational(1)) {
        if (cont.denominator() != val::integer(1)) s_prefix += "(" + val::ToString(cont) + ")";
        else s_prefix += val::ToString(cont);
    }
    if (factors.length()>0 && s_prefix!="" && s_prefix!="-") s_prefix+="*";

    s = s_prefix + s;

    return s;
}



std::string PolfractionToString(const val::rationalfunction &F)
{
	int bracket=0;
	std::string sf;
	val::pol<val::rational> zero,one(1,0),minusone(-1,0);

	if (F.nominator()==zero) return "0";
	if (F.denominator()==one) return factorize(F.nominator());  //val::PolToString(F.nominator());
	if (F.denominator()==minusone) return factorize(-F.nominator());  //(val::PolToString(-F.nominator()));
	if (F.denominator().degree() == 0) return factorize(val::rational(1,F.denominator().LC()) * F.nominator());

	val::pol<val::integer> nom,denom;
	val::rational cont,c1;

	val::primitivpart(F.nominator(),nom,cont);
	val::primitivpart(F.denominator(),denom,c1);
	cont/=c1;
	nom*=val::nominator(cont);
	denom*=val::denominator(cont);

	if (F.nominator().length()>1) {sf+='(';bracket=1;}
	//sf+=val::PolToString(val::toRationalPolynom(nom));
	sf+=factorize(val::toRationalPolynom(nom));
	if (bracket) sf+=')';
	bracket=0;
	if (F.denominator()!=one) {
		sf+='/';
		if (F.denominator().length()>1) {
			bracket=1;
			sf+='(';
		}
		//sf+=val::PolToString(val::toRationalPolynom(denom));
		sf+=factorize(val::toRationalPolynom(denom));
		if (bracket) sf+=')';
	}

	return sf;
}



int partialfraction(const val::valfunction& f, val::rational &cont, val::pol<val::rational> &fp,val::d_array<val::pol<val::rational>> &numpol,
                    val::d_array<val::pol<val::rational>> &denompol, val::d_array<int> &denumexpo, int comment)
{
    using namespace val;
    numpol.del(); denompol.del(); denumexpo.del(); fp.del();

    if (!f.isrationalfunction()) return 0;
    rationalfunction fR = f.getrationalfunction();
    pol<rational> p = fR.nominator(), q = fR.denominator(), h, hdiv;

    if (deg(p) >= deg(q)) {
        pol<rational> rpol;
        divrem(p,q,fp,rpol);
        if (p != q*fp + rpol) {
            Error::error("\n Error by computation of division with remainder!");
        }
        p = std::move(rpol);
    }

    cont = content(p)/content(q);
    p = toRationalPolynom(primitivpart(p));
    q = toRationalPolynom(primitivpart(q));

    d_array<pol<rational>> qfactors = polfactor(q);
    int i, j, k, n = qfactors.length(), r, s, m = 0, nqvar = 0, kb;
    d_array<int> qexponents(0,n);
    d_array<int> islinear(0,n);

    // set linear factors first, r is number of linear factors, s of quadratic factors.
    if (!sortfactors(qfactors,r,s)) return 0;

    h = q;
    for (i = 0; i < n; ++i) {
        while ((h%qfactors[i]).iszero()) {
           ++qexponents[i];
           h /= qfactors[i];
        }
        if (i<r) m+=qexponents[i];
        else {
            m+=2*qexponents[i];
            nqvar += qexponents[i];
        }
    }
    if (comment) std::cout<<"\n After factorization: m = "<<m<<", h = "<<PolToString(h)<< ", cont = "<<ToString(cont);
    /*
    std::cout<<std::endl;
    for (const auto& v : qfactors) std::cout<<PolToString(v)<<std::endl;
    */
    d_array<d_array<pol<rational>>> linq(r), quadq(s);
    d_array<d_array<int>> a_var(r), alpha_var(s), beta_var(s);
    for (i = 0; i < r; ++i) {
        linq[i] = d_array<pol<rational>>(qexponents[i]);
        a_var[i] = d_array<int>(qexponents[i]);
    }
    for (i = 0; i < s; ++i) {
        quadq[i] = d_array<pol<rational>>(qexponents[i+r]);
        alpha_var[i] = d_array<int>(qexponents[i+r]);
        beta_var[i] = d_array<int>(qexponents[i+r]);
    }
    k = 0;

    for (i = 0; i < r; ++i) {
        hdiv = pol<rational>(rational(1),0);
        for (j = 0; j < qexponents[i]; ++j) {
            hdiv *= qfactors[i];
            linq[i][j] = q/hdiv;
            a_var[i][j] = k;
            ++k;
        }
    }
    for (i = 0; i < s; ++i) {
        hdiv = pol<rational>(rational(1),0);
        for (j = 0; j < qexponents[i+r]; ++j) {
            hdiv *= qfactors[i+r];
            quadq[i][j] = q/hdiv;
            alpha_var[i][j] = k;
            beta_var[i][j] = k + nqvar;
            ++k;
        }
    }
    //if (k+ nqvar != m) std::cout<<"\n k+nqvar != m! k + nqvar = "<<k+nqvar;

    // Set LES:
    matrix<rational> A(m,m+1);

    for (int d = 0; d < m; ++d) {
        A(d,m) = p[d];  // last column:
        // a_var
        for (i = 0; i < r; ++i) {
            for (j = 0; j < qexponents[i]; ++j) {
                k = a_var[i][j];
                A(d,k) = linq[i][j][d];
            }
        }
        // alpha_var, beta_var
        for (i = 0; i < s; ++i) {
            for (j = 0; j < qexponents[i+r]; ++j) {
                k = alpha_var[i][j];
                if (d > 0) A(d,k) = quadq[i][j][d-1];
                k = beta_var[i][j];
                A(d,k) = quadq[i][j][d];
            }
        }
    }

    if (comment) {
        for (i = 0; i < m; ++i) {
            std::cout<<std::endl;
            for (j = 0; j <= m; ++j) std::cout<<A(i,j)<<"  ";
        }
    }
    matrix<rational> X;
    rational det;
    les(A,X,det);
    if (X.numberofrows() != 1) return 0;

    numpol.resize(m - nqvar); denompol.resize(m - nqvar); denumexpo.resize(m -nqvar);


    for (i = 0; i < r; ++i) {
        for (j = 0; j < qexponents[i]; ++j) {
            k = a_var[i][j];
            numpol[k] = pol<rational>(X(0,k),0);
            denompol[k] = qfactors[i];
            denumexpo[k] = j+1;
        }
    }
    for (i = 0; i < s; ++i) {
        for (j = 0; j < qexponents[i+r]; ++j) {
            k = alpha_var[i][j];
            kb = beta_var[i][j];
            numpol[k] = pol<rational>({GPair<rational,int>(X(0,k),1),GPair<rational,int>(X(0,kb),0) });
            denompol[k] = qfactors[i+r];
            denumexpo[k] =  j+1;
        }
    }

    //delete 0-entries:
    n = 0 , m = numpol.length();
    for (i = 0; i < m; ++i) if (!numpol[i].iszero()) ++n;
    d_array<pol<rational>> hnumpol(n), hdenompol(n);
    d_array<int> hdenumexpo(n);

    j = 0;
    for (i = 0; i < m; ++i) {
        if (!numpol[i].iszero()) {
            hnumpol[j] = std::move(numpol[i]);
            hdenompol[j] = std::move(denompol[i]);
            hdenumexpo[j] = denumexpo[i];
            ++j;
        }
    }

    numpol = std::move(hnumpol);
    denompol = std::move(hdenompol);
    denumexpo = std::move(hdenumexpo);

    return 1;
}

namespace hintegral
{

val::d_array<std::string> operators({"exp","sin","cos","log","tan","sqrt","arcsin","arccos","arctan"}), escop({"exp","sin","cos","log"});
enum ops{EXP,SIN,COS,LOG,TAN,SQRT,ARCSIN,ARCCOS,ARCTAN};


val::valfunction rational_integral(const val::valfunction &f, int k);
// -------------------------------------------------------------------------------------------------------------


int is_polynomial(const val::valfunction &f, int k = 1)
{
	if (f.is_zero()) return 1;
	if (f.isconst(k)) {
		return 1;
	}
	std::string svar = "x" , sf = f.getinfixnotation();

	if (f.numberofvariables() > 1) svar += val::ToString(k);
	if (sf == svar) return 1;
	if (k == 1 && sf == "x") return 1;
	if (k == 2 && sf == "y") return 1;
	if (k == 3 && sf == "z") return 1;

	std::string oper = f.getfirstoperator();
	val::valfunction g,h;

	if (val::isinContainer(oper,operators)) {
		if (!f.getfirstargument().isconst(k)) return 0;
		else return 1;
	}
	if (oper == "m") return (is_polynomial(f.getfirstargument(),k));
	else if (oper == "+" || oper == "-" || oper == "*") return(is_polynomial(f.getfirstargument(),k) && is_polynomial(f.getsecondargument(),k));
	else if (oper == "/") {
		if (!f.getsecondargument().isconst(k)) return 0;
		else return is_polynomial(f.getfirstargument(),k);
	}
	else if (oper ==  "^") {
		h = f.getsecondargument();
		if (!h.isconst(k)) return 0;
		if (!f.getfirstargument().isconst(k)) {
			std::string s_h = h.getinfixnotation();
			if (!val::isinteger(s_h)) return 0;
			int n = val::FromString<int>(s_h);
			if (n < 0) return 0;
			return (is_polynomial(f.getfirstargument(),k));
		}
		else return 1;
	}
	return 1;
}


// if f is polynomial in x_k, then this computes the polynomial.
val::pol<val::valfunction> getpolynomial(const val::valfunction &f, int k = 1)
{
	using namespace val;

	std::string oper = f.getfirstoperator(), sf = f.getinfixnotation(), svar = "x" + val::ToString(k);
	int nvar = f.numberofvariables();

	if (f.is_zero()) return pol<valfunction>();
	else if (f.isconst(k)) {
		return pol<valfunction>(f);
	}
	else if (k == 1 && nvar == 1 && sf == "x") return pol<valfunction>(valfunction("1"),1);
	else if (k == 2 &&  sf == "y") return pol<valfunction>(valfunction("1"),1);
	else if (k == 3 &&  sf == "z") return pol<valfunction>(valfunction("1"),1);
	else if (sf == svar) return pol<valfunction>(valfunction("1"),1);
	else if (oper == "m") return -getpolynomial(f.getfirstargument(),k);
	else if (oper == "+") {
		return getpolynomial(f.getfirstargument(),k) + getpolynomial(f.getsecondargument(),k);
	}
	else if (oper == "-") return getpolynomial(f.getfirstargument(),k) - getpolynomial(f.getsecondargument(),k);
	else if (oper == "*") {
		return getpolynomial(f.getfirstargument(),k) * getpolynomial(f.getsecondargument(),k);
	}
	else if (oper == "/") return getpolynomial(f.getfirstargument(),k) / getpolynomial(f.getsecondargument(),k);
	else return (power(getpolynomial(f.getfirstargument(),k),FromString<int>(f.getsecondargument().getinfixnotation())));
}


int is_rational(const val::valfunction &f, int k = 1)
{
	if (f.is_zero()) return 1;
	if (f.isconst(k)) {
		return 1;
	}
	std::string svar = "x" , sf = f.getinfixnotation();

	if (f.numberofvariables() > 1) svar += val::ToString(k);
	if (sf == svar) return 1;
	if (k == 1 && sf == "x") return 1;
	if (k == 2 && sf == "y") return 1;
	if (k == 3 && sf == "z") return 1;

	std::string oper = f.getfirstoperator();
	val::valfunction g,h;

	if (val::isinContainer(oper,operators)) {
		if (!f.getfirstargument().isconst(k)) return 0;
		else return 1;
	}
	if (oper == "m") return (is_rational(f.getfirstargument(),k));
	else if (oper == "+" || oper == "-" || oper == "*") return(is_rational(f.getfirstargument(),k) && is_rational(f.getsecondargument(),k));
	else if (oper == "/") return (is_rational(f.getfirstargument(),k) && is_rational(f.getsecondargument(),k));
	else if (oper ==  "^") {
		h = f.getsecondargument();
		if (!h.isconst(k)) return 0;
		if (!f.getfirstargument().isconst(k)) {
			std::string s_h = h.getinfixnotation();
			if (!val::isinteger(s_h)) return 0;
			int n = val::FromString<int>(s_h);
			if (n < 0) return 0;
			return (is_rational(f.getfirstargument(),k));
		}
		else return 1;
	}
	return 1;
}


// if f is polynomial in x_k, then this computes the polynomial.
val::fraction<val::pol<val::valfunction>> getrationalfunction(const val::valfunction &f, int k = 1)
{
	using namespace val;

	std::string oper = f.getfirstoperator(), sf = f.getinfixnotation(), svar = "x" + val::ToString(k);
	int nvar = f.numberofvariables();

	if (f.is_zero()) return fraction<pol<valfunction>>();
	else if (f.isconst(k)) {
		return fraction<pol<valfunction>>(f);
	}
	else if (k == 1 && nvar == 1 && sf == "x") return fraction<pol<valfunction>>(pol<valfunction>(valfunction("1"),1));
	else if (k == 2 &&  sf == "y") return fraction<pol<valfunction>>(pol<valfunction>(valfunction("1"),1));
	else if (k == 3 &&  sf == "z") return fraction<pol<valfunction>>(pol<valfunction>(valfunction("1"),1));
	else if (sf == svar) return fraction<pol<valfunction>>(pol<valfunction>(valfunction("1"),1));
	else if (oper == "m") return -getrationalfunction(f.getfirstargument(),k);
	else if (oper == "+") {
		return getrationalfunction(f.getfirstargument(),k) + getpolynomial(f.getsecondargument(),k);
	}
	else if (oper == "-") return getrationalfunction(f.getfirstargument(),k) - getrationalfunction(f.getsecondargument(),k);
	else if (oper == "*") {
		return getrationalfunction(f.getfirstargument(),k) * getrationalfunction(f.getsecondargument(),k);
	}
	else if (oper == "/") return getrationalfunction(f.getfirstargument(),k) / getrationalfunction(f.getsecondargument(),k);
	else return (power(getrationalfunction(f.getfirstargument(),k),FromString<int>(f.getsecondargument().getinfixnotation())));
}


//

int isquadratic(const val::rational& r, val::rational &root)
{
    val::integer p = r.nominator(), q = r.denominator(), sp, sq;
    root = val::rational(0);
    if (q.iszero()) return 0;
    if (p.iszero()) return 1;
    if (p.signum() < 0) return 0;
    sp = sqrt(p);
    sq = sqrt(q);
    root = val::rational(sp,sq);
    if (r == root * root) return 1;
    else return 0;
}



// ------------------------------------------------------------------------------------------------------------------------------------------


val::pol<val::valfunction> integral(const val::pol<val::valfunction> &f)
{
	using namespace val;
	pol<valfunction> F;
	int d;
	for (const auto& m : f) {
		d = m.actualdegree() + 1;
		F.insert(m()/valfunction(ToString(d)),d);
	}
	return F;
}


int isoppolynomial(const val::valfunction &f, int k)
{
    if (f.is_zero()) return -1;
    int n = escop.length();

    std::string op = f.getfirstoperator();


    for (int i = 0; i < n; ++i) {
		if (op == escop[i] && is_polynomial(f.getfirstargument(),k)) {
			//std::cout << "Here: i = " << i << "; escop[i] = " << escop[i] << std::endl;
			return i;
		}
	}

    if (op=="*" && is_polynomial(f.getfirstargument(),k)) return isoppolynomial(f.getsecondargument(),k);

    if (op=="m") return isoppolynomial(f.getfirstargument(),k);

	return -1;
}


val::valfunction getrationalfrom_oprat(const val::valfunction &f, const std::string& oper = "exp")
{
    val::valfunction g;
    if (f.is_zero()) return g;

    std::string op=f.getfirstoperator();
    if (op==oper) return val::valfunction("1");
    if (op=="m") return -getrationalfrom_oprat(-f,oper);
    if (op=="*") return f.getfirstargument()*getrationalfrom_oprat(f.getsecondargument(),oper);
    if (op=="/") return (getrationalfrom_oprat(f.getfirstargument(),oper)/f.getsecondargument());

    return g;
}

val::valfunction getopargumentfrom_oprat(const val::valfunction& f, const std::string &oper = "exp")
{
    val::valfunction g;
    if (f.is_zero()) return g;

    std::string op=f.getfirstoperator();
    if (op==oper) return f.getfirstargument();
    if (op=="m") return getopargumentfrom_oprat(f.getfirstargument(),oper);
    if (op=="*") return getopargumentfrom_oprat(f.getsecondargument(),oper);
    if (op=="/") return getopargumentfrom_oprat(f.getfirstargument(),oper);

    return g;
}


// computes polynomial f(x): integral(cx^n exp (ax+b)) = f(x) * exp(ax+b):
val::pol<val::valfunction> getpolfromintegralexppol(int n,const val::valfunction &a, const val::valfunction& c)
{
	using namespace val;
	if (a.is_zero() || c.is_zero()) return pol<valfunction>();
	valfunction coeff = c/a;
	pol<valfunction> f(coeff,n);
	//if (n == 0) return f;
	for (int k = 1; k <= n; ++k) {
		coeff = -coeff;
		coeff *= valfunction(ToString(n+1-k));
		coeff = coeff / a;
		f.insert(coeff,n-k);
	}
	//std::cout<<"\n n = "<<n<<" f_n = "<<val::PolToString(f);
	return f;
}


void getpolsincos(int n,const val::valfunction& a, const val::valfunction &c,const std::string &oper,val::pol<val::valfunction> &f,val::pol<val::valfunction>& g)
{
	f.del(); g.del();
	if (a.is_zero() || c.is_zero()) return;
	val::valfunction coeff = c/a;
	val::valfunction sf("1"), sg("1");

	if (oper == "sin") {
		sg = val::valfunction("-1"); sf = val::valfunction("1");
	}

	g.insert(sg*coeff,n);

	for (int i = 1; i <= n; ++i) {
		coeff *= val::valfunction(val::ToString("n-i+1"));
		coeff = coeff / a;
		if (i%2) {
			f.insert(sf*coeff,n-i);
			sf = -sf;
		}
		else {
			sg = -sg;
			g.insert(sg*coeff,n-i);
		}
	}
	return;
}


// Tries to compute integral(g*h) or integral(g/h) via substitution, h != const, g != const
val::valfunction integral_product_subst(const val::valfunction &g, const val::valfunction& h, int k,const std::string &oper = "*")
{
    using namespace val;

    if (g.is_zero() || h.is_zero()) return valfunction();

    val::valfunction F, g1 = g.derive(k), h1 = h.derive(k), hdivg = h1/g, gdivh = g1/h, z;
    std::string sf;

    if (hdivg.isconst(k)) {
		//std::cout << "\nHere! g = " << g.derive(2).getinfixnotation() << std::endl;
        if (oper == "*") sf = "1/(2*(" + hdivg.getinfixnotation() + ")) * (" + h.getinfixnotation() + ")^2";
        else sf = "1/(" + hdivg.getinfixnotation() + ") * log(abs(" + h.getinfixnotation() + "))";
        return valfunction(sf);
    }
    else if (gdivh.isconst(k) && oper == "*") {
        sf = "1/(2*(" + gdivh.getinfixnotation() + ")) * (" + g.getinfixnotation() + ")^2";
        return valfunction(sf);
    }

    if (oper == "/") return F;


    valfunction z1, z1div;
    const valfunction *ph = nullptr;
	std::string firstop = h.getfirstoperator();
	int divisconst = 0;

	//std::cout<<"\n firtsop = "<<firstop<<std::endl;

    z = h.getfirstargument(); z1 = z.derive(k); z1div = z1/g;
    if (z1div.isconst(k)) {
        ph = &h;
        divisconst = 1;
    }
    else {
        z = g.getfirstargument(); z1 = z.derive(k); z1div = z1/h;
        firstop = g.getfirstoperator();
        if (z1div.isconst(k)) {
            divisconst = 1;
            ph = &g;
        }
    }


    if (divisconst) {
        valfunction H;
        if (isinContainer(firstop,operators)) {
            H = ::integral(valfunction(firstop + "(x)"),1);
        }
        else if (firstop == "^") {
            std::string sop = ph->getsecondargument().getinfixnotation();
            H = ::integral(valfunction("x^("  + sop + ")" ),1);
        }
        H = H(z);
        if (z1div.is_zero()) return F;
        sf = "1/(" + z1div.getinfixnotation() + ") * (" + H.getinfixnotation() + ")";
        return (valfunction(sf));
    }

    return F;
}


// Recursive computation of integral(num/denum^m), deg(num) <= 1, denum = x^2+ ax +b, 4b -a^2 != 0.
val::valfunction quadratic_integral(const val::pol<val::rational> &num, const val::pol<val::rational> &denum, int m, int k)
{
    using namespace val;
    valfunction F;
    if (num.degree() > 1 || denum.degree() != 2) return F;
    rational one(1);
    if (num.degree() == 0 && num.LC() != one) return F;
    rational a = denum[1], b =denum[0], c = rational(4) * b - a*a, d;
    if (c.iszero()) return F;
    std::string sF = PolToString(denum), sF1, sF2;
    valfunction F1, F2;

    if (num.degree() == 0) {
        sF1 = "1/(" + sF + ")";
        if (m==1) return rational_integral(valfunction(sF1),k);
        else {
            sF1 = "(2x + " + ToString(a) + ")/((" + ToString(m-1) + ")*(" + ToString(c) + ")*(" + sF + ")^" + ToString(m-1) +")";
            F1 = valfunction(sF1);
            d = rational(2*(2*m - 3),(m-1)); d /= c;
            F2 = valfunction(ToString(d));
            return (F1 + F2 * quadratic_integral(num,denum,m-1,k));
        }
    }
    else {
        sF1 = "(" + PolToString(num) + ")/(" + sF + ")";
        if (m == 1) return rational_integral(valfunction(sF1),k);
        else {
            const rational &alpha = num[1], &beta = num[0];
            d = rational(2*m -2);
            sF1 = "- (" + ToString(alpha) + ")/(("  + ToString(d) + ")*(" + sF + ")^" + ToString(m-1) +")";
            F1 = valfunction(sF1);
            d = beta; d -= alpha*a/rational(2);
            F2 = valfunction(ToString(d));
            pol<rational> num1(one,0);
            return (F1 + F2 * quadratic_integral(num1,denum,m,k));
        }

    }
    return F;
}


// f has to be a rational function:
val::valfunction rational_integral(const val::valfunction &f, int k)
{
    using namespace val;
    valfunction F, X("x" + ToString(k));
    if (f.is_zero()) return F;
    fraction<pol<valfunction>> f_rational_vf = getrationalfunction(f,k);
    std::string sF;


    if (f_rational_vf.numerator().degree() <= 1 && f_rational_vf.denominator().degree() == 1) {
        pol<valfunction> q, fnum, fdenom = f_rational_vf.denominator();;
        divrem(f_rational_vf.numerator(),fdenom,q,fnum);

        valfunction c = fnum[0]/fdenom[1];
        sF = "(" + c.getinfixnotation() + ")*" + "log(abs(" + fdenom(X).getinfixnotation() + "))";
        return ::integral(q(X),k) +  valfunction(sF);
    }

    //if (!f.isrationalfunction()) return F;

    valfunction ffnum = f_rational_vf.nominator()(X), ffdenom = f_rational_vf.denominator()(X);
    //rationalfunction f_rational = f.getrationalfunction();
    if (!ffdenom.ispolynomialfunction()) return F;


    pol<rational> fdenom = ffdenom.getpolynomial();

    if (f_rational_vf.nominator().degree() == 0 && fdenom.degree() == 2) {
        rational cont = 1/fdenom.LC(), a;
        fdenom /= fdenom.LC();                // compute integral(1/x^2 +ax + b)
        a = fdenom[1];
        rational b = fdenom[0], d = rational(4)*b - a*a, rd, zero(0);
        std::string sF1;

        if (d > zero) {
            int issquare = isquadratic(d,rd);
            if (issquare) {
                sF1 = ToString(rd);
            }
            else sF1 = "sqrt(" + ToString(d) + ")";
            sF = "(" + ToString(cont) + ")*(2/" + sF1 + ")*arctan((2x + " + ToString(a) + ")/" + sF1 +")";
            F = ffnum * valfunction(sF)(X);
            return F;
        }
        else if (d < zero) {
            d.changesign(); // => d = a^2 - 4b > 0
            if (isquadratic(d,rd)) sF1 = ToString(rd);
            else sF1 = "sqrt(" + ToString(d) + ")";
            std::string sF2 = "(2x + " + ToString(a) + " - " + sF1 + ")/(2x + " + ToString(a) + " + " + sF1 + ")";
            sF = "(" + ToString(cont) + ")*(1/" + sF1 + ")*log(abs(" + sF2 + "))";
            F = ffnum * valfunction(sF)(X);
            return F;
        }
        else {     // a^2 == 4b
            cont *= rational(-2);
            sF  = "(" + ToString(cont) + ")* 1/(2x +" + ToString(a) + ")";
            F = ffnum * valfunction(sF)(X);
            return F;
        }
    }

    if (!ffnum.ispolynomialfunction()) return F;

    pol<rational> fnum = ffnum.getpolynomial();
    rational a,c, cont = content(fnum)/content(fdenom);

    fnum = toRationalPolynom(primitivpart(fnum));
    fdenom = toRationalPolynom(primitivpart(fdenom));


    if (fnum.degree() == 1 && fdenom.degree() == 2) {
        rational alpha = fnum[1], beta = fnum[0];
        cont /= fdenom.LC(); fdenom /= fdenom.LC();
        a = fdenom[1];
        rational b = fdenom[0];
        alpha /= rational(2);
        std::string sF1 = "(" + ToString(alpha*cont) + ")*log(abs(" + PolToString(fdenom) + "))", sF2 = "1/(" + PolToString(fdenom) + ")";
        beta -= alpha * a;
        valfunction F1(ToString(cont*beta));
        F = (valfunction(sF1) + F1 * rational_integral(valfunction(sF2),k));
        return F(X);
    }
    else {
        pol<rational> fp;
        d_array<pol<rational>> numpol, denumpol;
        d_array<int> denumexpo;
        if (!partialfraction(f,c,fp,numpol,denumpol,denumexpo)) return F;
        valfunction Fp = ::integral(valfunction(PolToString(fp)));
        int i, n = numpol.length(), e;
        std::string sc = ToString(c), sc2;
        //d_array<valfunction> rF(n);

        F += Fp;

        //std::cout<<"\n F = "<<F.getinfixnotation();

        for (i = 0; i < n; ++i) {
            e = denumexpo[i];
            sF = "(" + PolToString(numpol[i]) + ")/(" + PolToString(denumpol[i]) +")";
            if (e == 1) {
                //std::cout<<"\nc = "<<c<<" , numpol[i] = "<<PolToString(numpol[i])<<" , denumpol[i] = "<<PolToString(denumpol[i]);
                sF = sc + "*" + sF;
                F += rational_integral(valfunction(sF),k);
                //std::cout<<"\n e = 1, F = "<<F.getinfixnotation();
            }
            else if (denumpol[i].degree() == 1 ){
                sc2 = ToString(c/rational(-e+1));
                sF = sc2 + "*" + sF + "^(" + ToString(e-1) + ")";
                F += valfunction(sF);
            }
            else {
                if (e > 5) return F;
                pol<rational> num = numpol[i], dnum = denumpol[i];
                c *= num.LC(); num /= num.LC();
                c /= power(dnum.LC(),e); dnum /= dnum.LC();
                valfunction F1(ToString(c)), F2 = quadratic_integral(num,dnum,e,k);
                F += F1*F2;
            }
        }
    }


    return F(X);
}

val::valfunction integral_sqrt_qpolynom(val::pol<val::valfunction> &Pg, int k)
{
	using namespace val;
	if (Pg.degree() != 2) return valfunction();

	valfunction F;
	valfunction a = Pg.LC();
	std::string sc , sa, sx, root, sf;
	rational ra, re;
	int eisrational = 0, isconst_a, isconst_e;

	Pg /= a;
	valfunction d = Pg[1]/valfunction("2"), e = Pg[0] - d*d; //  gp = a[(x+d)^2 +e]
	eisrational = val::isrationalnumber(e.getinfixnotation());

	if (val::isrationalnumber(sa = a.getinfixnotation())) {
		if (hintegral::isquadratic(abs(FromString<rational>(sa)),ra)) sc = ToString(ra);
		else sc = "sqrt(" + sa + ")";

	}
	else {
		if (sa[0] == '-') sa = (-a).getinfixnotation();
		sc = "sqrt(" + sa + ")";
	}

	sx = "(x" + ToString(k) + " + " + d.getinfixnotation() + ")";

	isconst_a = a.isconst() ; isconst_e = e.isconst();

	a.setparameter(1); e.setparameter(1);

	if (isconst_a && isconst_e) {
		double da = a(0), de = e(0);
		//std::cout << "\n da = " << da << " , de = " << de <<std::endl;
		if (da < 0.0 && de >= 0.0) return F;
		else if (da > 0.0) {
			root = "sqrt(" + sx + "^2 + " + e.getinfixnotation() + ")";
			if (de > 0.0) {
				sf = sc + "((" + sx + "/2) * " + root + " + ((" + e.getinfixnotation() + ")/2) * log(" + sx + " + " + root + "))";
			}
			else sf = sc + "((" + sx + "/2) * " + root + " + ((" + e.getinfixnotation() + ")/2) * log(abs(" + sx + " + " + root + ")))";
			return valfunction(sf);
		}
		else if (de < 0.0){   // a < 0, e >0:
			std::string se;
			e = -e;
			if (eisrational && isquadratic(FromString<rational>(e.getinfixnotation()),re)) se = ToString(re);
			else se = "sqrt("+ e.getinfixnotation() + ")";
			root = "sqrt(" + e.getinfixnotation() + " - " + sx + "^2)";
			sf = sc + "((" + sx + "/2) * " + root + " + ((" + e.getinfixnotation() + ")/2) * arcsin((" + sx + ")/(" + se + ")))";
			return valfunction(sf);
		}
	}
	else if (isconst_a) {
		//std::cout<<"\n a = " << a.getinfixnotation() << " , e = " << e.getinfixnotation() << std::endl;
			std::string se = e.getinfixnotation();
		if (a(0) < 0.0) {
			if (se.length() && se[0] == '-') return F;
			se = "sqrt("+ se + ")";
			root = "sqrt(" + e.getinfixnotation() + " - " + sx + "^2)";
			sf = sc + "((" + sx + "/2) * " + root + " + ((" + e.getinfixnotation() + ")/2) * arcsin((" + sx + ")/(" + se + ")))";
			return valfunction(sf);
		}
		else {
			root = "sqrt(" + sx + "^2 + " + se + ")";
			if (se.length() && se[0] == '-') sf = sc + "((" + sx + "/2) * " + root + " + (" + se + "/2) * log(abs(" + sx + " + " + root + ")))";
			else sf = sc + "((" + sx + "/2) * " + root + " + (" + se + "/2) * log(" + sx + " + " + root + "))";
			//std::cout << "\n Here!, sf = " << sf << std::endl;
			return valfunction(sf);
		}
	}
	else if (isconst_e) {
		if (a.getinfixnotation()[0] == '-') {
			if (e(0) < 0.0) return F;
			else {
				std::string se;
				e = -e;
				if (eisrational && isquadratic(FromString<rational>(e.getinfixnotation()),re)) se = ToString(re);
				else se = "sqrt("+ e.getinfixnotation() + ")";
				root = "sqrt(" + e.getinfixnotation() + " - " + sx + "^2)";
				sf = sc + "((" + sx + "/2) * " + root + " + ((" + e.getinfixnotation() + ")/2) * arcsin((" + sx + ")/(" + se + ")))";
				return valfunction(sf);
			}
		}
		else {
			root = "sqrt(" + sx + "^2 + " + e.getinfixnotation() + ")";
			if (e(0) < 0.0) {
				sf = sc + "((" + sx + "/2) * " + root + " + ((" + e.getinfixnotation() + ")/2) * log(abs(" + sx + " + " + root + ")))";
			}
			else sf = sc + "((" + sx + "/2) * " + root + " + ((" + e.getinfixnotation() + ")/2) * log(" + sx + " + " + root + "))";
			return valfunction(sf);
		}

	}
	else {
		if (a.getinfixnotation()[0] == '-') {
			if (e.getinfixnotation().length() > 0 && e.getinfixnotation()[0] == '-') return F;
			std::string se = "sqrt("+ e.getinfixnotation() + ")";
			root = "sqrt(" + e.getinfixnotation() + " - " + sx + "^2)";
			sf = sc + "((" + sx + "/2) * " + root + " + ((" + e.getinfixnotation() + ")/2) * arcsin((" + sx + ")/(" + se + ")))";
		}
		else {
			root = "sqrt(" + sx + "^2 + " + e.getinfixnotation() + ")";
			if ( e.getinfixnotation().length() > 0 && e.getinfixnotation()[0] == '-') {
				sf = sc + "((" + sx + "/2) * " + root + " + ((" + e.getinfixnotation() + ")/2) * log(abs(" + sx + " + " + root + ")))";
			}
			else {
				sf = sc + "((" + sx + "/2) * " + root + " + ((" + e.getinfixnotation() + ")/2) * log(" + sx + " + " + root + "))";
				//std::cout << "\n Here!, sf = " << sf << std::endl;
			}
		}
		return valfunction(sf);
	}


	return F;
}


} // end namespace hintegral



val::valfunction integral(const val::valfunction &f, int k)
{
	using namespace val;

	std::string svar = "x" + ToString(k);

	if (hintegral::is_polynomial(f,k)) {
		pol<valfunction> P = hintegral::getpolynomial(f,k), Q = hintegral::integral(P);
		return Q(valfunction(svar));
	}

	valfunction F, g = f.getfirstargument();
	std::string firstop = f.getfirstoperator();
	pol<valfunction> Pg;
	int op = -1, operindex;

	for (int i = 0; i < hintegral::operators.length(); ++i) {
		if (firstop == hintegral::operators[i]) {
			op = i;
			break;
		}
	}

	if (op != -1) {
		if (!hintegral::is_polynomial(g,k)) return F;
		Pg = hintegral::getpolynomial(g,k);
		if (Pg.degree() > 1 && op != hintegral::SQRT) return F;
	}

	switch (op) {
		case hintegral::EXP: {
			return f/Pg.LC();
		}
		break;
		case hintegral::SIN: {
			valfunction c("-cos(x)");
			return c(g)/Pg.LC();
		}
		break;
		case hintegral::COS: {
			valfunction c("sin(x)");
			return c(g)/Pg.LC();
		}
		break;
		case hintegral::TAN: {
			valfunction h1("log(x)"), h2("abs(cos(x))");
			h2 = h2(g);
			h1 = h1(h2);
			return h2/Pg.LC();
		}
		break;
		case hintegral::LOG: {
			return (g*f - g)/Pg.LC();
		}
		break;
		case hintegral::ARCSIN: {
			valfunction hF("arcsin(x)"), hG("sqrt(1 - x^2)");
			hF = g * hF(g) + hG(g);
			return hF/Pg.LC();
		}
		break;
		case hintegral::ARCCOS: {
			valfunction hF("arccos(x)"), hG("sqrt(1 - x^2)");
			hF = g * hF(g) - hG(g);
			return hF/Pg.LC();
		}
		break;
		case hintegral::ARCTAN: {
			valfunction hF("arctan(x)"), hG("1/2 * log(1 + x^2)");
			hF = g * hF(g) - hG(g);
			return hF/Pg.LC();
		}
		break;
		case hintegral::SQRT: {
			if (Pg.degree() == 1) {
				std::string sf = "(" + g.getinfixnotation() + ")^3";
				F = valfunction("2/3") * valfunction("sqrt(x)")(valfunction(sf))/Pg.LC();
				return F;
			}
			else if (Pg.degree() == 2) return hintegral::integral_sqrt_qpolynom(Pg,k);
		}
		break;
	}

	if (firstop == "+") {
		return integral(g,k) + integral(f.getsecondargument(),k);
	}
	else if (firstop == "-") return integral(g,k) - integral(f.getsecondargument(),k);
	else if (firstop == "m") return -integral(g,k);
	else if ((operindex = hintegral::isoppolynomial(f,k)) != -1) {  // case f = polynomial * oper(ax +b)
		valfunction h = hintegral::getrationalfrom_oprat(f,hintegral::escop[operindex]), h1 = hintegral::getopargumentfrom_oprat(f,hintegral::escop[operindex]);
		pol<valfunction> pfactor = hintegral::getpolynomial(h,k), parg = hintegral::getpolynomial(h1,k);
		int n = parg.degree();
		if (n <= 1) {
			if (operindex == 0) { // exp
				pol<valfunction> P;
				for (int i = 0; i <= n; ++i) P += hintegral::getpolfromintegralexppol(i,parg.LC(),pfactor[i]);
				h = P(valfunction(svar));
				h1 = valfunction("exp(x)")(h1);
				return h * h1;
			}
			else if (operindex <= 2) {    // sin or cos:
				pol<valfunction> fh,gh,P,Q;
				for (int i = 0; i <= n; ++i) {
					hintegral::getpolsincos(i,parg.LC(),pfactor[i],hintegral::escop[operindex],fh,gh);
					P += fh; Q += gh;
				}
				std::string oper1 = "sin(x)", oper2 = "cos(x)";
				if (operindex == 2) {
					oper1 = "cos(x)"; oper2 = "sin(x)";
				}
				F = P(valfunction(svar)) * (valfunction(oper1)(h1)) +
				    Q(valfunction(svar)) * (valfunction(oper2)(h1));
			}
			else {    // log:
                std::string  sparg = parg(valfunction(svar)).getinfixnotation(), sF1 = "log(" + sparg + ")", sF2 = pfactor(valfunction(svar)).getinfixnotation() , sF3;
                valfunction F1(sF1), F2 = integral(valfunction(sF2)), F3;
                sF3 = parg.LC().getinfixnotation() + "*(" + F2.getinfixnotation() + ")/(" + sparg + ")";
                F3 = integral(valfunction(sF3));
                if (!F3.is_zero()) return (F1 * F2 - F3);
			}
		}
		else if (firstop == "*") {
            valfunction h = f.getsecondargument();
            //std::cout << "\n g.nvar = " << g.numberofvariables();
            if (!g.isconst(k) && !h.isconst(k)) F = hintegral::integral_product_subst(g,h,k);
		}
	}
	else if (hintegral::is_rational(f,k)) {
        fraction<pol<valfunction>> R = hintegral::getrationalfunction(f,k);
        //std::cout << "\n f.num = \n" << R.numerator();
        //std::cout << "\n f.denum = \n" << R.denominator();
        //std::cout << "\n f.num/f.denum = \n" << R.numerator()/R.denominator();
        //std::cout << "\n f.num%f.denum = \n" << R.numerator()%R.denominator();

        return hintegral::rational_integral(f,k);
	}
	else if (firstop == "*" || firstop == "/") {
        valfunction h = f.getsecondargument();
        int g_const = g.isconst(k), h_const = h.isconst(k);
        if (!g_const && !h_const) {
            F = hintegral::integral_product_subst(g,h,k,firstop);
        }
        else if (g_const) {
            if (firstop == "*") return g * integral(h,k);
        }
        else if (h_const) {
            if (firstop == "*") return h * integral(g,k);
            else return integral(g,k)/h;
        }
	}
	else if (firstop == "^") {
		valfunction h = f.getsecondargument();
		if (!h.isconst(k)) {
			std::string sf = "exp(log(" + g.getinfixnotation() + ") * (" + h.getinfixnotation() + "))";
			return integral(valfunction(sf),k);
		}
		if (g.getfirstoperator() == "sqrt") {
			g = g.getfirstargument();
			h *= valfunction("1/2");
		}
		if (hintegral::is_polynomial(g,k)) {
			pol<valfunction> Pg = hintegral::getpolynomial(g,k);
			if (Pg.degree() > 1) return F;
			valfunction a = Pg.LC();
			if (h.getinfixnotation() == "-1") {
				F = valfunction("log(abs(x))");
				return F(g)/a;
			}
			h += valfunction("1");
			std::string sh = h.getinfixnotation(),
						sf = "1/((" + sh + ") * " + a.getinfixnotation() + ") * (" + g.getinfixnotation() + ")^(" + sh + ")";
			return valfunction(sf);
		}
	}

	return F;
}



void computeintegral(const myfunction& f,val::rational x1,val::rational x2,double delta,int n,int dez,int arclength)
{
    using namespace val;
    MyThreadEvent event(MY_EVENT,IdIntegral);
    int k=isderived(f),ispol=0,exact=0;
    double a=double(x1),b=double(x2),wert,exwert;
    val::rational r_wert;
    //val::DoubleFunction g;
    val::valfunction g;
    std::string name="";

    if (arclength) {
        if (f.isdifferentiable()) {
            val::valfunction h("sqrt(x^2 + 1)"),g(f.getinfixnotation());
            g=g.derive(); //g=g*g; g+=val::valfunction("1");
            h=h(g);
            //name += h.getinfixnotation() + "\n";
            //name = h.getinfixnotation() + "  ;";
            wert=integral(h,a,b,n,delta);
        }
        else {
            //val::DoubleFunction g1=val::DoubleFunction(val::doublefunction(std::bind(std::cref(f),std::placeholders::_1))).derive();
            val::valfunction g1(val::valfunction(f.getinfixnotation()).derive());
            g1 = g1*g1;
            //g1 += val::DoubleFunction(1);
            g1 += val::valfunction("1");
            //g = DoubleFunction(sqrt)(g1);
            g = val::valfunction("sqrt")(g1);
            wert = integral(g,a,b,n,delta);
        }
        name+="arclength( ";
        //g = val::DoubleFunction(sqrt) (val::DoubleFunction(val::doublefunction(std::bind(std::cref(f),std::placeholders::_1))).derive() + val::DoubleFunction(1));
    }
    else if (f.numberofvariables()<=1 && f.ispolynomialfunction()) {
        ispol=1;
        val::pol<val::rational> p=f.getpol(val::rational(0),'y');
        r_wert=p.integrate(x1,x2);
    }
    if (!arclength) {
        if (k) {
            wert=derive(f,b,k-1) - derive(f,a,k-1);
        }
        else wert=integral(f,a,b,n,delta);
        {
            val::valfunction F= integral(val::valfunction(f.getinfixnotation()));
            if (!F.is_zero()) {
                name += "integral("+f.getinfixnotation()+") \n= " + F.getinfixnotation() + "\n\n";
                exact = 1;
                exwert = F(b) - F(a);
            }

        }
        name+="integral( ";
    }

    int wprec = intdigits(wert) + dez, eprec = intdigits(exwert) + dez;
    wprec = val::Min(wprec,val::MaxPrec);
    eprec = val::Min(eprec,val::MaxPrec);

    tablestring = name + f.getinfixnotation() + " ; " + ToString(x1) + " ; " + ToString(x2) + " ) =\n     ";
    if (ispol) tablestring+= ToString(r_wert) + "\n\ndouble:   ";
    else if (exact) tablestring += ToString(val::round(exwert,dez),eprec) + " (over stammfunction), \n\ndouble:   ";
    tablestring+=ToString(val::round(wert,dez),wprec);
    if (k==0) tablestring+= "\nPrecision : " + ToString(delta) + " , Round to decimal: " + ToString(dez) + "\nIterations : " + ToString(n);

    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
}



std::string compute_partialfraction(const val::valfunction &f)
{
    using namespace val;
    std::string os;
    integer i_one(1);
    rational cont, one(1), lc;
    pol<rational> fp;
    d_array<int> denomexp;
    d_array<pol<rational>> numpol, denompol;
    if (!partialfraction(f,cont,fp,numpol,denompol,denomexp)) {
        //std::cout<<"\nPartial fraction decomposition failed!";
        return os;
    }
    int n = numpol.length(), contpar = 0, i;

    if (!fp.iszero()) {
        os += PolToString(fp);
        if (n > 0) os += " + ";
    }
    if (!n) return os;
    if (cont == -one) {
        os += "-";
        if (n>1) {
            os += "(";
            contpar = 1;
        }
    }
    else if (cont != one) {
        os += ToString(cont) + " * ";
        if (n>1) {
            os +="(";
            contpar = 1;
        }
    }

    for (i = 0; i < n; ++i) {
        if (numpol[i].degree() == 0) {
            lc = numpol[i].LC();
            if (lc.signum() < 0 || lc.denominator() != i_one) os += "(" + ToString(lc) + ")";
            else os += ToString(lc);
            //os += "(" + ToString(numpol[i][0]) +") 1";
        }
        else if (numpol[i].length() > 1) {
            os += "(" + PolToString(numpol[i]) + ")";
        }
        else {
            if (numpol[i][0] != one) os += PolToString(numpol[i]);
            os += "1";
        }
        os += "/(" + PolToString(denompol[i]) + ")";
        if (denomexp[i] > 1) os += "^" + ToString(denomexp[i]);
        if (i != n-1) os += " + ";
    }
    if (contpar) os += ")";
    return os;
}


void analize_rationalfunction(val::valfunction& F,const double& eps,int dec)
{

    int N=0,N_r=0,rat=1,klammer=0, digits;

    //val::rationalfunction::setreduced(0);
    val::vector<double> zeros;
    val::d_array<val::rational> r_zeros;
    val::valfunction FF;
    val::rationalfunction F_r = F.getrationalfunction(0), FF_r=F_r;
    //val::Glist<int> &Primelist = val::Primes;
    //std::string os="";
    val::pol<val::rational> one(1,0);

    FF_r.reducethis();
    F.simplify();

    if (F.is_zero()) return;

    if (F_r.denominator()==one && F_r.nominator().degree()<=0) return;

    if (F_r.denominator()!=one && F_r.nominator().length()>1) klammer=1;
    analyze_output[0]+="f(x) = ";
    if (F_r.denominator()!=one && F_r.denominator().degree() == 0) {
        //analyze_output[0] += "(" + val::ToString(val::rational(1)/F_r.denominator().LC()) + ")*";
        analyze_output[0] += val::PolToString((val::rational(1)/val::rational(F_r.denominator().LC())) * F_r.nominator());
    }
    else {
        if (klammer) analyze_output[0]+="(";
        analyze_output[0]+=val::PolToString(F_r.nominator());
        if (klammer) analyze_output[0]+=")";
        if (F_r.denominator().degree() > 0) {
            analyze_output[0]+="/("+val::PolToString(F_r.denominator()) + ")";
        }
    }
    analyze_output[0]+="\nFactorization of f in Q[x]:\nf(x) = " + PolfractionToString(FF_r) + "\n";
    {
        val::valfunction iF = integral(F);
        if (FF_r.denominator().degree() <= 5) { // partial fraction decomposition
            std::string spf = compute_partialfraction(F);
            if (spf != "") {
                analyze_output[0] += "f in partial fractions: \n f(x) = " + spf + "\n";
            }
        }
        if (!iF.is_zero()) analyze_output[0] += "Stammfunction: \n F(x) = " + iF.getinfixnotation() + "\n";
    }

    //analyze_output[0]+="\nFactorization of f in Q[x]:\nf(x) = " + F.getinfixnotation() + "\n";
    //os+=factorize(F_r) + "\n";

    // Definitionslücken:
    val::pol<val::rational> f_r = F_r.denominator();
    f_r /= val::gcd(f_r,f_r.derive());
    val::pol<double> f_d = val::ToDoublePolynom(f_r),f_dred;// = val::ToDoublePolynom(F_r.nominator());

    //val::rationalfunction::setreduced(1);
    F_r.reducethis();
    FF = F;
    //F = val::valfunction(PolfractionToString(F_r));
    //FF = val::valfunction(PolfractionToString(F_r));

    f_dred = val::ToDoublePolynom(F_r.denominator());

    val::realRoots(f_d,zeros,eps);
    N=zeros.dimension();
    if (N) {
        zeros.sort();
        analyze_output[0]+="Number of gaps of definition: " + val::ToString(N) + "\n";
        for (int i=0;i<N;++i) {
            if (zeros[i]==0) zeros[i] = 0.0;
            digits = intdigits(zeros[i]) + dec;
            digits = val::Min(digits,val::MaxPrec);
            analyze_output[0] += "  " + val::ToString(val::round(zeros[i],dec),digits);
        }

        if (rat) {
            //CreatePrimlist(Primelist);
            if (val::Primes.isempty()) analyze_output[0]+="\nNo primes found!";
            r_zeros=val::rational_roots(f_r);
            N_r = r_zeros.length();
            r_zeros.sort();
            analyze_output[0]+="\nNumber of rational gaps " + val::ToString(N_r) + ":\n";
            for (int i=0;i<N_r;++i) analyze_output[0]+= "  " + val::ToString(val::round(r_zeros[i],dec),val::MaxPrec);
        }
        analyze_output[0]+="\nBehavior :\n";
        for (int i = 0;i<N;++i) {
            digits = intdigits(zeros[i]) + dec;
            digits = val::Min(digits,val::MaxPrec);
            if (f_dred(zeros[i] + eps) * f_dred(zeros[i] - eps) > 0.0) {   //(val::abs(f_dred(zeros[i])) > 5*eps) {
                analyze_output[0]+="lim f("+val::ToString(val::round(zeros[i],dec),digits)+") = " + val::ToString(val::round(F(zeros[i]),dec),digits) + "\n";
                continue;
            }
            if (F(zeros[i]-eps)>0) {
                analyze_output[0]+="lim f("+val::ToString(val::round(zeros[i],dec),digits)+"-) = +inf,  ";
            }
            else analyze_output[0]+="lim f("+val::ToString(val::round(zeros[i],dec),digits)+"-) = -inf,  ";
            if (F(zeros[i]+eps)>0) {
                analyze_output[0]+="lim f("+val::ToString(val::round(zeros[i],dec),digits)+"+) = +inf,  ";
            }
            else analyze_output[0]+="lim f("+val::ToString(val::round(zeros[i],dec),digits)+"+) = -inf,  ";
            analyze_output[0]+="\n";
        }
        if (F_r.nominator().degree()>=F_r.denominator().degree()) {
            analyze_output[0]+="\n Asymptotic function:\n g(x) = "+val::PolToString(F_r.nominator()/F_r.denominator());
        }
    }

    // reelle Nullstellen:
    r_zeros.del();
    f_r = F_r.nominator();
    if (!f_r.iszero()) f_r /= gcd(f_r,f_r.derive());
    f_d = val::ToDoublePolynom(f_r);
    val::realRoots(f_d,zeros,eps);
    N=zeros.dimension();
    if (!N) analyze_output[1]+="No real zeros.\n";
    else {
        Points[0].reserve(N);
        zeros.sort();
        analyze_output[1]+="Number of real zeros: "+ val::ToString(N) + "\n";
        for (int i=0;i<N;++i) {
            if (zeros[i]==0) zeros[i] = 0;
            digits = intdigits(zeros[i]) + dec;
            digits = val::Min(digits,val::MaxPrec);
            analyze_output[1]+= "  " + val::ToString(val::round(zeros[i],dec),digits);
            Points[0].push_back(val::GPair<double>(zeros[i],0.0));
        }

        if (rat) {
            r_zeros=val::rational_roots(f_r);
            N_r = r_zeros.length();
            r_zeros.sort();
            analyze_output[1]+="\nNumber of rational zeros: " + val::ToString(N_r) + "\n";
            for (int i=0;i<N_r;++i) analyze_output[1]+= "  " + val::ToString(r_zeros[i]);
            analyze_output[1]+="\n";
        }
    }

    // Extrema:
    val::d_array<int> vzw;
    int pm=0,mp=0;

    F_r=derive(F_r);
    F = val::valfunction(PolfractionToString(F_r));
    f_r = F_r.nominator();
    if (!f_r.iszero()) f_r /= val::gcd(f_r,f_r.derive());
    f_d = val::ToDoublePolynom(f_r);
    r_zeros.del();
    val::realRoots(f_d,zeros,eps);
    N=zeros.dimension();
    //if (!N) os+="\nKeine Extrema.";
    if (F_r.denominator().degree() == 0) {
        analyze_output[2] += "f'(x) = " + val::PolToString((val::rational(1)/F_r.denominator().LC())*F_r.nominator()) + "\n";
        analyze_output[2] += "      = " + PolfractionToString(F_r) + "\n";
    }
    else analyze_output[2]+="f'(x) = " +PolfractionToString(F_r) + "\n";
    //os+="\n\nf'(x) = " + factorize(F_r) + "\n";
    if (N) {
        int ydigits;
        vzw.resize(N);
        zeros.sort();
        for (int i=0;i<N;++i) {
            vzw[i]=0;
            if (zeros[i]==0) zeros[i] = 0;
            //os+=val::ToString(zeros[i]) + " : "+val::ToString(F(zeros[i]-eps)) + " , " + val::ToString(F(zeros[i]+eps)) + " ; ";
            if (F(zeros[i]-eps) >0 && F(zeros[i]+eps)<0) {
                vzw[i]=1;
                pm++;
            }
            if (F(zeros[i]-eps)<0 && F(zeros[i]+eps)>0) {
                vzw[i]=-1;
                mp++;
            }
        }
        if (pm+mp) {
            double y;
            analyze_output[2]+="\nNumber of extrema: " + val::ToString(pm+mp);
            Points[1].reserve(pm+mp);
            if (rat) {
                r_zeros=val::rational_roots(f_r);
                N_r = r_zeros.length();
                r_zeros.sort();
            }
            if (pm) {
                analyze_output[2]+="\nNumber of maxima: " + val::ToString(pm) + "\n";
                for (int i=0;i<N;++i) {
                    if (vzw[i]==1) {
            			digits = intdigits(zeros[i]) + dec;
            			digits = val::Min(digits,val::MaxPrec);
                        y = FF(zeros[i]);
                        ydigits = intdigits(y) + dec;
                        ydigits = val::Min(ydigits,val::MaxPrec);
                        analyze_output[2]+="( " + val::ToString(val::round(zeros[i],dec),digits) + " | " + val::ToString(val::round(y,dec),ydigits) + " )  ";
                        Points[1].push_back(val::GPair<double>(zeros[i],y));
                        for (const auto& x : r_zeros) {
                            if (val::abs(double(x)-zeros[i])<eps) {
                                analyze_output[2]+= " [ = ( " + val::ToString(x) + " | " + val::ToString(eval(FF_r,x)) + " )  ]\n";
                                break;
                            }
                        }
                    }
                }
            }
            if (mp) {
                analyze_output[2]+="\nNumber of minima: " + val::ToString(mp) + "\n";
                for (int i=0;i<N;++i) {
                    if (vzw[i]==-1) {
            			digits = intdigits(zeros[i]) + dec;
            			digits = val::Min(digits,val::MaxPrec);
                        y = FF(zeros[i]);
                        ydigits = intdigits(y) + dec;
                        ydigits = val::Min(ydigits,val::MaxPrec);
                        analyze_output[2]+="( " + val::ToString(val::round(zeros[i],dec),digits) + " | " + val::ToString(val::round(y,dec),ydigits) + " )  ";
                        Points[1].push_back(val::GPair<double>(zeros[i],y));
                        for (const auto& x : r_zeros) {
                            if (val::abs(double(x)-zeros[i])<eps) {
                                analyze_output[2]+= " [ = ( " + val::ToString(x) + " | " + val::ToString(eval(FF_r,x)) + " )  ]\n";
                                break;
                            }
                        }
                    }
                }
            }

        }
    }
    else analyze_output[2]+="No extrema.";
    analyze_output[2]+="\n";

    //Wendepunkte:
    vzw.del();pm=0;mp=0;
    F_r = derive(F_r);
    F = val::valfunction(PolfractionToString(F_r));
    f_r = F_r.nominator();
    if (!f_r.iszero()) f_r /= val::gcd(f_r,f_r.derive());
    f_d = val::ToDoublePolynom(f_r);
    r_zeros.del();
    val::realRoots(f_d,zeros,eps);
    N = zeros.dimension();
    //if (!N) os+="\nKeine Extrema.";
    analyze_output[3]+="f''(x) = " +PolfractionToString(F_r) + "\n";
    //analyze_output[3]+="f''(x) = (" + val::PolToString(F_r.nominator()) + ")/" + val::PolToString(F_r.denominator()) + "\n";
    //os+="\n\nf''(x) = " + factorize(F_r) + "\n";
    if (N) {
        int ydigits;
        vzw.resize(N);
        zeros.sort();
        for (int i=0;i<N;++i) {
            vzw[i]=0;
            if (zeros[i]==0) zeros[i] = 0;
            //os+=val::ToString(zeros[i]) + " : "+val::ToString(F(zeros[i]-eps)) + " , " + val::ToString(F(zeros[i]+eps)) + " ; ";
            if (F(zeros[i]-eps) >0 && F(zeros[i]+eps)<0) {
                vzw[i]=1;
                pm++;
            }
            if (F(zeros[i]-eps)<0 && F(zeros[i]+eps)>0) {
                vzw[i]=-1;
                mp++;
            }
        }
        if (pm+mp) {
            double y;
            Points[2].reserve(pm+mp);
            analyze_output[3]+="\nNumber of inflection points (IP): " + val::ToString(pm+mp);
            if (rat) {
                r_zeros=val::rational_roots(f_r);
                N_r = r_zeros.length();
                r_zeros.sort();
            }
            if (pm) {
                analyze_output[3]+="\nNumber of  LR-IP: " + val::ToString(pm) + "\n";
                for (int i=0;i<N;++i) {
                    if (vzw[i]==1) {
            			digits = intdigits(zeros[i]) + dec;
            			digits = val::Min(digits,val::MaxPrec);
                        y = FF(zeros[i]);
                        ydigits = intdigits(y) + dec;
                        ydigits = val::Min(ydigits,val::MaxPrec);
                        analyze_output[3]+="( " + val::ToString(val::round(zeros[i],dec),digits) + " | " + val::ToString(val::round(y,dec),ydigits) + " )  ";
                        Points[2].push_back(val::GPair<double>(zeros[i],y));
                        for (const auto& x : r_zeros) {
                            if (val::abs(double(x)-zeros[i])<eps) {
                                analyze_output[3]+= " [ = ( " + val::ToString(x) + " | " + val::ToString(eval(FF_r,x)) + " )  ]\n";
                                break;
                            }
                        }
                    }
                }
            }
            if (mp) {
                analyze_output[3]+="\nNumber of RL-IP: " + val::ToString(mp) + "\n";
                for (int i=0;i<N;++i) {
                    if (vzw[i]==-1) {
            			digits = intdigits(zeros[i]) + dec;
            			digits = val::Min(digits,val::MaxPrec);
                        y = FF(zeros[i]);
                        ydigits = intdigits(y) + dec;
                        ydigits = val::Min(ydigits,val::MaxPrec);
                        analyze_output[3]+="( " + val::ToString(val::round(zeros[i],dec),digits) + " | " + val::ToString(val::round(y,dec),ydigits) + " )  ";
                        Points[2].push_back(val::GPair<double>(zeros[i],y));
                        for (const auto& x : r_zeros) {
                            if (val::abs(double(x)-zeros[i])<eps) {
                                analyze_output[3]+= " [ = ( " + val::ToString(x) + " | " + val::ToString(eval(FF_r,x)) + " )  ]\n";
                                break;
                            }
                        }
                    }
                }
            }

        }
    }
    else analyze_output[3]+="No inflection points.";
    analyze_output[3]+="\n";


    MyThreadEvent event(MY_EVENT,IdAnalyze);
    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
    return;
}


void analyze_exprationalfunction(const val::valfunction &f,const double &eps=1e-9,int dec=4)
{
    if (f.is_zero()) return;

    int i,pm=0,mp=0,N=0,N_r=0,ispol=0,rat=1, digits, ydigits;
    double y;
    val::rational r;
    val::valfunction F,Exp;
    val::rationalfunction FF_r,F_r,Exp_r;
    val::vector<double> zeros;
    val::d_array<val::rational> r_zeros;
    val::pol<val::rational> one(1,0),f_r;
    val::pol<double> f_d;
    val::d_array<int> vzw;

    F_r = hintegral::getrationalfrom_oprat(f,"exp").getrationalfunction();
    FF_r = F_r;
    if (F_r.nominator()==one) ispol=1;

    analyze_output[0]="f(x) = ";
    if (ispol) analyze_output[0] += f.getinfixnotation();
    else {
        F_r = hintegral::getrationalfrom_oprat(f,"exp").getrationalfunction();
        analyze_output[0] += "(" + PolfractionToString(F_r) + ")*" + "exp(" + hintegral::getopargumentfrom_oprat(f,"exp").getinfixnotation() + ").";
    }
    {
        val::valfunction iF = integral(f);
        if (!iF.is_zero()) analyze_output[0] += "\n Stammfunction: \n F(x) = " + iF.getinfixnotation();
    }

    Exp=hintegral::getopargumentfrom_oprat(f,"exp");
    if (rat) Exp_r= Exp.getrationalfunction();
    //Definitionslücken:
    //
    // Nullstellen:
    f_r = F_r.nominator();
    f_r /= val::gcd(f_r,f_r.derive());
    f_d = val::ToDoublePolynom(f_r);
    val::realRoots(f_d,zeros,eps);
    N = zeros.dimension();
    if (N) {
        Points[0].reserve(N);
        zeros.sort();
        analyze_output[1] += "\nNumber of zeros: " + val::ToString(N) + "\n";
        for (i=0;i<N;++i) {
            digits = intdigits(zeros[i]) + dec;
            digits = val::Min(digits,val::MaxPrec);
            analyze_output[1] += " " + val::ToString(val::round(zeros[i],dec),digits);
            Points[0].push_back(val::GPair<double>(zeros[i],0.0));
        }
        if (rat) {
            r_zeros=val::rational_roots(f_r);
            N_r = r_zeros.length();
            r_zeros.sort();
            analyze_output[1]+="\nNumber of rational zeros: " + val::ToString(N_r) + "\n";
            for (int i=0;i<N_r;++i) analyze_output[1]+= "  " + val::ToString(r_zeros[i]);
        }
    }
    else analyze_output[1] += "\nNo real zeros.";
    analyze_output[1] += "\n";

    //

    //Extrema:
    r_zeros.del();
    F=f.derive();
    F_r = hintegral::getrationalfrom_oprat(F,"exp").getrationalfunction();
    f_r = F_r.nominator();
    analyze_output[2] = "f'(x) = ";
    //if (F_r.denominator()==one) analyze_output[2] += F.getinfixnotation() + "\n";
    if (F_r.denominator().degree() < 1) analyze_output[2] += F.getinfixnotation() + "\n";
    else {
        analyze_output[2] += PolfractionToString(F_r) + "*" + "exp(" + Exp.getinfixnotation() + ").\n";
    }


    f_r /= val::gcd(f_r,f_r.derive());
    f_d = val::ToDoublePolynom(f_r);
    val::realRoots(f_d,zeros,eps);
    N = zeros.dimension();
    zeros.sort();
    vzw.resize(N);
    for (i=0;i<N;++i) {
        vzw[i]=0;
        if (zeros[i]==0) zeros[i] = 0;
        if (F(zeros[i]-eps) >0 && F(zeros[i]+eps)<0) {
            vzw[i]=1;
            pm++;
        }
        if (F(zeros[i]-eps)<0 && F(zeros[i]+eps)>0) {
            vzw[i]=-1;
            mp++;
        }
    }
    if (pm+mp>0) {
        analyze_output[2]+="\nNumber of extrema: " + val::ToString(pm+mp);
        Points[1].reserve(pm+mp);
        if (rat) r_zeros=val::rational_roots(f_r);
        if (pm) {
            analyze_output[2]+="\nNumber of maxima: " + val::ToString(pm) + "\n";
            for (i=0;i<N;++i) {
                if (vzw[i]==1) {
            		digits = intdigits(zeros[i]) + dec;
            		digits = val::Min(digits,val::MaxPrec);
                    y = f(zeros[i]);
                    ydigits = intdigits(y) + dec;
                    ydigits = val::Min(ydigits,val::MaxPrec);
                    analyze_output[2]+="( " + val::ToString(val::round(zeros[i],dec),digits) + " | " + val::ToString(val::round(y,dec),ydigits) + " )  ";
                    Points[1].push_back(val::GPair<double>(zeros[i],y));
                    for (const auto& x : r_zeros) {
                        if (val::abs(double(x)-zeros[i])<eps) {
                            r = eval(Exp_r,x);
                            analyze_output[2]+= " [ = ( " + val::ToString(x) + " | " + val::ToString(eval(FF_r,x));
                            if (!r.iszero()) analyze_output[2]+= "exp("+ val::ToString(eval(Exp_r,x)) + ") )  ]\n";
                            else analyze_output[2] += ") )  ]\n";
                            break;
                        }
                    }
                }
            }
        }
        if (mp) {
            analyze_output[2]+="\nNumber of minima: " + val::ToString(mp) + "\n";
            for (int i=0;i<N;++i) {
                if (vzw[i]==-1) {
            		digits = intdigits(zeros[i]) + dec;
            		digits = val::Min(digits,val::MaxPrec);
                    y = f(zeros[i]);
                    ydigits = intdigits(y) + dec;
                    ydigits = val::Min(ydigits,val::MaxPrec);
                    analyze_output[2]+="( " + val::ToString(val::round(zeros[i],dec),digits) + " | " + val::ToString(val::round(y,dec),ydigits) + " )  ";
                    Points[1].push_back(val::GPair<double>(zeros[i],y));
                    for (const auto& x : r_zeros) {
                        if (val::abs(double(x)-zeros[i])<eps) {
                            r = eval(Exp_r,x);
                            analyze_output[2]+= " [ = ( " + val::ToString(x) + " | " + val::ToString(eval(FF_r,x));
                            if (!r.iszero()) analyze_output[2]+= "exp("+ val::ToString(eval(Exp_r,x)) + ") )  ]\n";
                            else analyze_output[2] += ") )  ]\n";
                            break;
                        }
                    }
                }
            }
        }
    }
    else analyze_output[2] += "\nNo extrema.";
    analyze_output[2]+="\n";

    //
    // Wendepunkte:
    r_zeros.del();
    F = F.derive();
    F_r = hintegral::getrationalfrom_oprat(F,"exp").getrationalfunction();
    f_r = F_r.nominator();
    analyze_output[3] = "f''(x) = ";
    //if (F_r.denominator()==one) analyze_output[3] += F.getinfixnotation() + "\n";
    if (F_r.denominator().degree() < 1) analyze_output[3] += F.getinfixnotation() + "\n";
    else {
        analyze_output[3] += PolfractionToString(F_r) + "*" + "exp(" + Exp.getinfixnotation() + ").\n";
    }

    pm=mp=0;
    f_r /= val::gcd(f_r,f_r.derive());
    f_d = val::ToDoublePolynom(f_r);
    val::realRoots(f_d,zeros,eps);
    N = zeros.dimension();
    N = zeros.dimension();
    zeros.sort();
    vzw.resize(N);
    for (i=0;i<N;++i) {
        vzw[i]=0;
        if (zeros[i]==0) zeros[i] = 0;
        if (F(zeros[i]-eps) >0 && F(zeros[i]+eps)<0) {
            vzw[i]=1;
            pm++;
        }
        if (F(zeros[i]-eps)<0 && F(zeros[i]+eps)>0) {
            vzw[i]=-1;
            mp++;
        }
    }
    if (pm+mp>0) {
        analyze_output[3]+="\nNumber of inflection points (IP): " + val::ToString(pm+mp);
        Points[2].reserve(pm+mp);
        if (rat) r_zeros = val::rational_roots(f_r);
        if (pm) {
            analyze_output[3]+="\nNumber of LR-IP: " + val::ToString(pm) + "\n";
            for (i=0;i<N;++i) {
                if (vzw[i]==1) {
            		digits = intdigits(zeros[i]) + dec;
            		digits = val::Min(digits,val::MaxPrec);
                    y = f(zeros[i]);
                    ydigits = intdigits(y) + dec;
                    ydigits = val::Min(ydigits,val::MaxPrec);
                    analyze_output[3]+="( " + val::ToString(val::round(zeros[i],dec),digits) + " | " + val::ToString(val::round(y,dec),ydigits) + " )  ";
                    Points[2].push_back(val::GPair<double>(zeros[i],y));
                    for (const auto& x : r_zeros) {
                        if (val::abs(double(x)-zeros[i])<eps) {
                            r = eval(Exp_r,x);
                            analyze_output[3]+= " [ = ( " + val::ToString(x) + " | " + val::ToString(eval(FF_r,x));
                            if (!r.iszero()) analyze_output[3]+= "exp("+ val::ToString(eval(Exp_r,x)) + ") )  ]\n";
                            else analyze_output[3] += ") )  ]\n";
                            break;
                        }
                    }
                }
            }
        }
        if (mp) {
            analyze_output[3]+="\nNumber of RL-IP: " + val::ToString(mp) + "\n";
            for (int i=0;i<N;++i) {
                if (vzw[i]==-1) {
            		digits = intdigits(zeros[i]) + dec;
            		digits = val::Min(digits,val::MaxPrec);
                    y = f(zeros[i]);
                    ydigits = intdigits(y) + dec;
                    ydigits = val::Min(ydigits,val::MaxPrec);
                    analyze_output[3]+="( " + val::ToString(val::round(zeros[i],dec),digits) + " | " + val::ToString(val::round(y,dec),ydigits) + " )  ";
                    Points[2].push_back(val::GPair<double>(zeros[i],y));
                    for (const auto& x : r_zeros) {
                        if (val::abs(double(x)-zeros[i])<eps) {
                            r = eval(Exp_r,x);
                            analyze_output[3]+= " [ = ( " + val::ToString(x) + " | " + val::ToString(eval(FF_r,x));
                            if (!r.iszero()) analyze_output[3]+= "exp("+ val::ToString(eval(Exp_r,x)) + ") )  ]\n";
                            else analyze_output[3] += ") )  ]\n";
                            break;
                        }
                    }
                }
            }
        }
    }
    else analyze_output[3] += "\nNo inflection points.";
    analyze_output[3] +="\n";

    //


    MyThreadEvent event(MY_EVENT,IdAnalyze);
    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
    return;
}


void analyzefunction(const myfunction &f,std::string input)
{
    if (f.iszero()) return;

    int i,iterations=1000,decimals=4,n;//=input.length();
    double x1=-5,x2=5,epsilon=1e-9;
    //std::string s_n="";

    analyze_output.resize(4);
    Points.resize(3);
    for (i=0;i<4;++i) analyze_output[i]="";
    for (i=0;i<3;++i) Points[i].del();

    //Get Parameters
    val::d_array<char> sep({';', '\n'});
    auto values = getwordsfromstring(input,sep);

    n = values.length();
    if (n>0) x1 = val::FromString<double>(values[0]);
    if (n>1) x2 = val::FromString<double>(values[1]);
    if (n>2) epsilon = val::FromString<double>(values[2]);
    if (n>3) iterations = val::FromString<int>(values[3]);
    if (n>4) decimals = val::FromString<int>(values[4]);

    if (x1>=x2) {
        x1 = -5; x2 = 5;
    }
    if (epsilon<0.0) epsilon = 1e-9;
    if (decimals<0 || decimals >10) decimals=2;

    // ------------------------------------------------------------

    val::valfunction F(f.getinfixnotation(),0);

    if (F.isrationalfunction()) {
        analize_rationalfunction(F,epsilon,decimals);
        return;
    }
    F.simplify();

    if (F.isconst()) {
        analyze_output[0] = "f(x) = " + F.getinfixnotation() + " = " + val::ToString(F(0.0));
        MyThreadEvent event(MY_EVENT,IdAnalyze);
        if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
        return;
    }

    if (isexprational(F)) {
        analyze_exprationalfunction(F,epsilon,decimals);
        return;
    }

    int pm=0,mp=0,N=0, digits, ydigits;
    double y;
    val::Glist<double> prezeros,zeros;
    val::d_array<int> vzw;
    val::valfunction FF;


    analyze_output[0]="f(x) = "+F.getinfixnotation()+".";
    analyze_output[0]+="\n\n x in [ "+ val::ToString(val::round(x1,decimals)) + " ; " + val::ToString(val::round(x2,decimals)) + " ]";
    analyze_output[0]+="\n Number of iterations: " + val::ToString(iterations);
    {
        val::valfunction iF = integral(F);
        if (!iF.is_zero()) analyze_output[0] += "\n Stammfunction: \n F(x) = " + iF.getinfixnotation();
    }

    // Definition-gaps:
    val::Glist<val::GPair<double >> intervals = F.get_undefined_intervals(x1,x2,iterations,epsilon);
    if (!intervals.isempty()) {
        val::Glist<double> undef_points;
        analyze_output[0]+= "\n\n f not  defined at:\n";
        for (const auto& P : intervals) {
            if (val::abs(P.x-P.y)<epsilon) {
                undef_points.push_back(P.x);
                continue;
            }
            if (isInf(P.x)) analyze_output[0]+="( ";
            else if (val::isNaN(F(P.x)) || isInf(F(P.x))) analyze_output[0]+="[ ";
            else analyze_output[0]+="( ";
            digits = intdigits(P.x) + decimals;
            digits = val::Min(digits,val::MaxPrec);
            ydigits = intdigits(P.y) + decimals;
            ydigits = val::Min(ydigits,val::MaxPrec);
            analyze_output[0]+=val::ToString(val::round(P.x,decimals),digits) + " , " + val::ToString(val::round(P.y,decimals),ydigits);
            if (isInf(P.y)) analyze_output[0]+= " )  ";
            else if (val::isNaN(F(P.y)) || isInf(F(P.y))) analyze_output[0]+= " ]  ";
            else analyze_output[0]+= " )  ";
        }
        if (!undef_points.isempty()) {
            analyze_output[0]+= " { ";
            for (const auto &x : undef_points) {
                digits = intdigits(x) + decimals;
                digits = val::Min(digits,val::MaxPrec);
                analyze_output[0] += val::ToString(val::round(x, decimals),digits) + " ";
            }
            analyze_output[0] += "}";
        }
    }
    //

    // Zeros:
    if (!F.isconst()) prezeros = F.double_roots(x1,x2,iterations,epsilon);
    for (const auto &z : prezeros) {
        if (!isInf(F(z)) && !val::isNaN(F(z))) zeros.push_back(z);
    }
    N=zeros.length();
    if (N) {
        Points[0].reserve(N);
        zeros.sort();
        analyze_output[1] += "\nNumber of zeros: " + val::ToString(N) + "\n";
        for (i=0;i<N;++i) {
            digits = intdigits(zeros[i]) + decimals;
            digits = val::Min(digits,val::MaxPrec);
            analyze_output[1] += "  " + val::ToString(val::round(zeros[i],decimals),digits);
            Points[0].push_back(val::GPair<double>(zeros[i],0.0));
        }
    }
    else analyze_output[1] += "\nNo real zeros.";
    analyze_output[1]+="\n";
    //

    // Extrema:
    FF = F.derive();
    prezeros = FF.double_roots(x1,x2,iterations,epsilon);
    zeros.dellist();
    for (const auto & z : prezeros) {
        if (!isInf(F(z)) && !val::isNaN(F(z))) zeros.push_back(z);
    }
    N=zeros.length();
    analyze_output[2]+="f'(x) = " + FF.getinfixnotation() + ".\n";
    vzw.resize(N);
    zeros.sort();
    for (int i=0;i<N;++i) {
        vzw[i]=0;
        if (zeros[i]==0) zeros[i] = 0;
        if (val::isNaN(F(zeros[i]))) continue;
        if (FF(zeros[i]-epsilon) >0 && FF(zeros[i]+epsilon)<0) {
            vzw[i]=1;
            pm++;
        }
        if (FF(zeros[i]-epsilon)<0 && FF(zeros[i]+epsilon)>0) {
            vzw[i]=-1;
            mp++;
        }
    }
    if (pm+mp) {
        analyze_output[2]+="\nNumber of extrema: " + val::ToString(pm+mp);
        Points[1].reserve(pm+mp);
        if (pm) {
            analyze_output[2]+="\nNumber of maxima: " + val::ToString(pm) + "\n";
            for (int i=0;i<N;++i) {
                if (vzw[i]==1) {
            		digits = intdigits(zeros[i]) + decimals;
           		digits = val::Min(digits,val::MaxPrec);
                    y = F(zeros[i]);
                    ydigits = intdigits(y) + decimals;
            		ydigits = val::Min(ydigits,val::MaxPrec);
                    analyze_output[2]+="( " + val::ToString(val::round(zeros[i],decimals),digits) + " | " + val::ToString(val::round(y,decimals),ydigits) + " )  ";
                    Points[1].push_back(val::GPair<double>(zeros[i],y));
                }
            }
        }
        if (mp) {
            analyze_output[2]+="\nNumber of minima: " + val::ToString(mp) + "\n";
            for (int i=0;i<N;++i) {
                if (vzw[i]==-1) {
            		digits = intdigits(zeros[i]) + decimals;
           		digits = val::Min(digits,val::MaxPrec);
                    y = F(zeros[i]);
                    ydigits = intdigits(y) + decimals;
            		ydigits = val::Min(ydigits,val::MaxPrec);
                    analyze_output[2]+="( " + val::ToString(val::round(zeros[i],decimals),digits) + " | " + val::ToString(val::round(y,decimals),ydigits) + " )  ";
                    Points[1].push_back(val::GPair<double>(zeros[i],y));
                }
            }
        }

    }
    else analyze_output[2]+="No extrema.";
    analyze_output[2]+="\n";

    // Inflection-Points:
    vzw.del();pm=0;mp=0;
    FF = FF.derive();
    prezeros = FF.double_roots(x1,x2,iterations,epsilon);
    zeros.dellist();
    for (const auto & z : prezeros) {
        if (!isInf(F(z)) && !val::isNaN(F(z))) zeros.push_back(z);
    }
    N=zeros.length();
    analyze_output[3]+="f''(x) = " + FF.getinfixnotation() + ".\n";
    vzw.resize(N);
    zeros.sort();
    for (int i=0;i<N;++i) {
        vzw[i]=0;
        if (zeros[i]==0) zeros[i] = 0;
        if (val::isNaN(F(zeros[i]))) continue;
        if (FF(zeros[i]-epsilon) >0 && FF(zeros[i]+epsilon)<0) {
                vzw[i]=1;
                pm++;
        }
        if (FF(zeros[i]-epsilon)<0 && FF(zeros[i]+epsilon)>0) {
            vzw[i]=-1;
            mp++;
        }
    }
    if (pm+mp) {
        Points[2].reserve(pm+mp);
        analyze_output[3]+="\nNumber of inflection points (IP): " + val::ToString(pm+mp);
        if (pm) {
            analyze_output[3]+="\nNumber of  LR-IP: " + val::ToString(pm) + "\n";
            for (int i=0;i<N;++i) {
                if (vzw[i]==1) {
            		digits = intdigits(zeros[i]) + decimals;
           		digits = val::Min(digits,val::MaxPrec);
                    y = F(zeros[i]);
                    ydigits = intdigits(y) + decimals;
            		ydigits = val::Min(ydigits,val::MaxPrec);
                    analyze_output[3]+="( " + val::ToString(val::round(zeros[i],decimals),digits) + " | " + val::ToString(val::round(y,decimals),ydigits) + " )  ";
                    Points[2].push_back(val::GPair<double>(zeros[i],y));
                }
            }
        }
        if (mp) {
            analyze_output[3]+="\nNumber of RL-IP: " + val::ToString(mp) + "\n";
            for (int i=0;i<N;++i) {
                if (vzw[i]==-1) {
            		digits = intdigits(zeros[i]) + decimals;
           		digits = val::Min(digits,val::MaxPrec);
                    y = F(zeros[i]);
                    ydigits = intdigits(y) + decimals;
            		ydigits = val::Min(ydigits,val::MaxPrec);
                    analyze_output[3]+="( " + val::ToString(val::round(zeros[i],decimals),digits) + " | " + val::ToString(val::round(y,decimals),ydigits) + " )  ";
                    Points[2].push_back(val::GPair<double>(zeros[i],y));
                }
            }
        }

    }
    else analyze_output[3]+="No inflection points.";
    analyze_output[3] += "\n";


    MyThreadEvent event(MY_EVENT,IdAnalyze);
    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
    return;
}


void intersection(const myfunction &f, const myfunction &g, std::string input)
{
    if (f.iszero() || g.iszero()) return;

    int i,iterations=1000,decimals=4,n, N = 0, digits, ydigits;//=input.length();
    double x1=-5,x2=5,epsilon=1e-9, y;
    val::vector<double> zeros;
    val::d_array<double> places;
    val::d_array<val::rational> r_places;
    val::rationalfunction h_r;
    val::pol<val::rational> h_p;
    //std::string s_n="";

    analyze_output.resize(2);
    Points.resize(1);
    for (i=0;i<2;++i) analyze_output[i]="";
    Points[0].del();

    //Get Parameters
    val::d_array<char> sep({';', '\n'});
    auto values = getwordsfromstring(input,sep);

    n = values.length();
    if (n>0) x1 = val::FromString<double>(values[0]);
    if (n>1) x2 = val::FromString<double>(values[1]);
    if (n>2) epsilon = val::FromString<double>(values[2]);
    if (n>3) iterations = val::FromString<int>(values[3]);
    if (n>4) decimals = val::FromString<int>(values[4]);

    if (x1>=x2) {
        x1 = -5; x2 = 5;
    }
    if (epsilon<0.0) epsilon = 1e-9;
    if (decimals<0 || decimals >10) decimals=2;

    analyze_output[0] = "Intersection points of\n f(x) = " + f.getinfixnotation() + "\n g(x) = " + g.getinfixnotation();
    analyze_output[0] += "\n\n x in [ "+ val::ToString(val::round(x1,decimals)) + " ; " + val::ToString(val::round(x2,decimals)) + " ]";
    analyze_output[0] += "\n Number of iterations: " + val::ToString(iterations);
    // ------------------------------------------------------------

    val::valfunction F(f.getinfixnotation(),0), G(g.getinfixnotation()), h = F-G;
    h.simplify(1);

    if (h.isrationalfunction()) {
        h_r = h.getrationalfunction();
        h_p = h_r.nominator();
        h_p /= val::gcd(h_p,h_p.derive());
        val::pol<double> h_d = val::ToDoublePolynom(h_p);
        val::realRoots(h_d,zeros,epsilon);
    }
    else if (isexprational(h)) {
        h_r = hintegral::getrationalfrom_oprat(h,"exp").getrationalfunction();
        h_p = h_r.nominator();
        h_p /= val::gcd(h_p,h_p.derive());
        val::pol<double> h_d = val::ToDoublePolynom(h_p);
        val::realRoots(h_d,zeros,epsilon);
    }
    else {
        val::Glist<double> prezeros;
        if (!h.isconst()) prezeros = h.double_roots(x1,x2,iterations,epsilon);
        n = prezeros.length();
        zeros = val::vector<double>(n);
        for (i = 0; i < n; ++i) zeros[i] = prezeros[i];
    }

    n = zeros.dimension();
    if (n) {
        places.reserve(n);
        for (i = 0; i < n; ++i) {
            if (!isInf(f(zeros[i])) && !val::isNaN(f(zeros[i])) && !isInf(g(zeros[i])) && !val::isNaN(g(zeros[i]))) places.push_back(zeros[i]);
        }
        N = places.length();
    }

    if (!N) analyze_output[1]+="No real intersection points.\n";
    else {
        int rat;
        std::string xr,yr;
        Points[0].reserve(N);
   	places.sort();
	    analyze_output[1]+="Number of real intersection points: "+ val::ToString(N) + "\n";
        if (!h_p.iszero()) {
            r_places = val::rational_roots(h_p);
            r_places.sort();
        }
        for (auto &x : places) {
            if (x == 0) x = 0;
            y = f(x);
            digits = intdigits(x) + decimals;
            digits = val::Min(digits,val::MaxPrec);
            ydigits = intdigits(y) + decimals;
            ydigits = val::Min(digits,val::MaxPrec);
            analyze_output[1] += "( " + val::ToString(val::round(x,decimals),digits) + " | " + val::ToString(val::round(y,decimals),ydigits) + " )";
            Points[0].push_back(val::GPair<double>(val::round(x,decimals),val::round(y,decimals)));
            rat = 0;
            for (const auto& r : r_places) {
                if (val::abs(double(r)-x) < epsilon) {
                    xr = val::ToString(r); yr = F(val::valfunction(xr)).getinfixnotation();
                    rat = 1;
                    break;
                }
            }
            if (rat) {
                analyze_output[1] += "[ = ( " + xr + " | " + yr + " )]\n";
            }
            else analyze_output[1] += "\n";
        }
    }

    MyThreadEvent event(MY_EVENT,IdIntersection);
    if (MyFrame!=NULL) MyFrame->GetEventHandler()->QueueEvent(event.Clone() );
    return;
}
