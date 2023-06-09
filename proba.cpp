#include <iostream>
#include <vector>
#include <array>
#include <chrono>


constexpr inline std::pair<int, int> go_left(int x, int y) {
	return {y, x - 1};
}

constexpr inline std::pair<int, int> go_right(int x, int y) {
	return {y, x + 1};
}

constexpr inline std::pair<int, int> go_up(int x, int y) {
	return {y - 1, x};
}

constexpr inline std::pair<int, int> go_down(int x, int y) {
	return {y + 1, x};
}

std::istream& operator>>(std::istream& is, std::pair<int, int>& p) {
	is >> p.first >> p.second;
	return is;
}

std::ostream& operator<<(std::ostream& os, const std::pair<int, int>& p) {
	os << '(' << p.first << ", " << p.second << ')';
	return os;
}

std::pair<int, int> operator+(const std::pair<int, int>&& x, const std::pair<int, int>&& y) {
	return {x.first + y.first, x.second + y.second};
}

std::pair<int, int> operator+=(std::pair<int, int>& x, const std::pair<int, int>&& y) {
	x.first += y.first;
	x.second += y.second;
	return x;
}

std::pair<int, int> operator*(int a, const std::pair<int, int>& x) {
	return {a * x.first, a * x.second};
}

bool operator==(std::pair<int, int>& s, std::pair<int, int>& t) {
	return s.first == t.first && s.second == t.second;
}

std::pair<int, int> path_2_m(int m, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	if(s.second < t.second) {
		// od s idemo skroz levo
		// onda menjamo traku, pa se vracamo desno do s
		// onda cik-cak nadesno do ispod/iznad t
		// pa skroz desno
		// pa menjamo traku i onda levo do t
		
		if(x > 0 && x <= s.second) {
			if(y == s.first) return {y, x - 1};
			else return {y, x + 1};
		} else if(x == 0) {
			if(y == s.first) return {1 - y, 0};
			else return {y, x + 1};
		} else if(x > s.second && x < t.second - 1) {
			// ako je x parno u odnosu na s.second
			// i y = s.first, onda idemo desno
			// ako je x parno, a y != s.first, menjamo traku
			// ako je x neparno, onda obrnuto
			// (A && !B) || (!A && B)
			// !(A => B) || !(B => A)
			// to je A xor B
			// A = (x - s.second) % 2
			// B = y != s.first;
			if(((x - s.second) % 2) ^ (y == s.first))
				return {1 - y, x};
			else return {y, x + 1};
		} else if(x == t.second - 1) {
			// ako je y == t.first, onda na gore
			// a inace desno
			if(y == t.first) return {1 - y, x};
			else return {y, x + 1};
		} else if(x > t.second && x < m - 1) {
			if(y == t.first) return {y, x - 1};
			else return {y, x + 1};
		} else if(x == t.second) {
			if(y == t.first) return {-1, -1};
			if(t.second == m - 1) return t;
			else return {y, x + 1};
		} else {
			if(y == t.first) return {y, x - 1};
			else return {1 - y, x};
		}
		
	} else if(s.second > t.second) {		
		// ako posmatramo zdesna nalevo, s je levo, a t je desno
		// treba nove koordinate su m-1-s.second i m-1-t.second
		// samo treba oduzeti ih i dodati posle
		std::pair<int, int> s1 = {s.first, m - 1 - s.second}, t1 = {t.first, m - 1 - t.second},
		ret = path_2_m(m, m - 1 - x, y, s1, t1);
		if(ret.first == -1 || ret.second == -1) return {-1, -1};
		ret.second = m - 1 - ret.second;
		return ret;
		
	} else {
		// ako su oba na 0, onda desno, pa dole/gore, pa levo
		// ako su oba na m-1, onda levo, pa dole/gore, pa desno
		if(!s.second) {
			// za y == s.first, idemo desno (y, x + 1) ako je x < m - 1
			// za x == m - 1 i y == s.first, menjamo traku (1 - y, x)
			// za y != s.first, idemo levo (y, x - 1) ako je x > 0
			// za y != s.first i x == 0, {-1, -1}
			// 1 - y: x == m - 1 && y == s.first
			bool Fy = x == m - 1 && y == s.first;
			bool A = y != s.first && !x;
			// x: x == m - 1 && y == s.first
			// x - 1: !(!x) && y != s.first
			bool Fx = x == m - 1 && y == s.first;
			bool Bx = !(!x) && y != s.first;
			return {Fy*(1 - y) - A + (!Fy && !A)*y, Fx*x + Bx*(x - 1) - A + (!Fx && !Bx && !A)*(x + 1)};
		} else if(s.second == m - 1) {
			// za y == s.first, idemo levo (y, x - 1) ako je x > 0
			// za y == s.first i x == 0, menjamo traku (1 - y, x)
			// za y != s.first i x < m - 1, idemo desno
			// za y != s.first i x == m - 1, {-1, -1}
			// 1 - y: x == 0 && y == s.first
			// x: y == s.first && !x
			// x - 1: !(!x) && y == s.first
			bool Fy = !x && y == s.first;
			bool A = y == t.first && x == t.second;
			bool Fx = !x && y == s.first;
			bool Bx = !(!x) && y == s.first; 
			return {Fy*(1 - y) - A + (!Fy && !A)*y, Fx*x + Bx*(x - 1) - A + (!Fx && !Bx && !A)*(x + 1)};;	
		} else {
			return {-1, -1};
		}
	}
}

std::pair<int, int> path_n_2(int n, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	// ako zamenimo mesta x i y, m i n i okrenemo koordinate s i t, dobijamo slucaj 2xm
	std::pair<int, int> s1 = {s.second, s.first};
	std::pair<int, int> t1 = {t.second, t.first};
	auto p = path_2_m(n, y, x, s1, t1);
	return {p.second, p.first};
}


// resavamo trisekciju u M5
std::pair<int, int> horizontal_trisection(int m, int n, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	// postoje 2 slucaja - s je gore ili je t gore
	// podrazumevamo da su s i t antipodi, jer inace ovo ne radi
	// podrazumevamo i n >= 4, jer inace nema horizontalne trisekcije
	// takodje se podrazumeva da je m >= 2
	
	if(s.first < t.first) {
		// trazimo tacku p
		// p.first == 1
		// dakle, p.second = (m - 1)*((s.first + s.second) % 2)
		std::pair<int, int> p = {1, (m - 1)*((s.first + s.second) % 2)};
		// spajamo s sa p
		// ako je tacka u traci [0 - 1]x[0 - m-1] i nije jednaka p
		if(x == p.second && y == 1) return {2, p.second};
		if(y < 2) return path_2_m(m, x, y, s, p);
		// spajamo p sa q
		// idemo cik-cak
		// ako je p levo, onda na parnim y idemo desno
		// ako je p desno, na parnim y idemo levo
		if(2 <= y && y < n - 2) {
			// levo idemo ako je p levo i y neparno
			// ili ako je p desno, a y parno
			// p je levo ako je p.first == 0, a inace je desno
			// imamo (!p && neparno) || (p && parno)
			// (!A i B) ili (A i !B)
			// to je A xor B
			// ili !A xor !B
			// A = p.first, B = neparno
			// jedino treba da pazimo na x
			// ako idemo levo, x mora biti > 0 (y, x - 1)
			// ako idemo desno, x mora biti < m - 1 (y, x + 1)
			// za x == 0 i idemo levo, spustamo se dole (y + 1, x)
			// za x == m - 1 i idemo desno, spustamo se dole (y + 1, x)
			// y, y + 1 su opcije
			// y: (!(!x) && ((y % 2) ^ !!p.first)) || (x < m - 1 || ((y % 2)  == !!p.first))
			// Fy + !F(y + 1) == y + !F
			// x - 1, x i x + 1
			// x - 1: (!(!x) && ((y % 2) ^ !p.first))
			// x + 1: (x < m - 1 && ((y % 2) == !p.first))
			// A(x - 1) + B(x + 1) + (!A && !B)x
			// F == A || B
			bool A = (!!x && ((y % 2) ^ !!p.second));
			bool B = (x < m - 1 && !((y % 2) ^ !!p.second));
			return {y + !(A || B), A*(x - 1) + B*(x + 1) + !(A || B)*x};
		} else {
			// spajamo q sa t
			// ako je n - 2 parno i p je levo, isli smo desno
			// ako je n - 2 parno i p je desno, isli smo levo
			// ako je n - 2 neparno i p je levo, isli smo levo
			// ako je n - 2 neparno i p desno, isli smo desno
			// to znaci da je x-koordinata od q ili 0 ili m - 1 s obzirom na ove uslove
			// ona je m - 1 ako je n - 2 parno i p je levo ili ako je n - 2 neparno i p je desno
			// (parno && !p) || (neparno && p)
			// opet A xor B, uzmimo A = neparno, B = !p
			std::pair<int, int> q = {0, !(n % 2)*(!!(p.second) * (m - 1)) + (n % 2)*(!p.second * (m - 1))};
			// sada povezujemo q sa t
			// prvo smanjimo koordinate, pa ih povecamo
			// visina 0 je isto sto i n - 2
			std::pair<int, int> t1 = {t.first - n + 2, t.second}, ret = path_2_m(m, x, y - n + 2, q, t1);
			if(ret.first == -1 || ret.second == -1) return {-1, -1};
			ret.first += n - 2;
			return ret;
			
			}
	} else {
		// mozemo krenuti od n-1
		std::pair<int, int> s1 = {n - 1 - s.first, s.second}, t1 = {n - 1 - t.first, t.second};
		std::pair<int, int> ret = horizontal_trisection(m, n, x, n - 1 - y, s1, t1);
		if(ret.first == -1 || ret.second == -1) return {-1, -1};
		ret.first = n - 1 - ret.first;
		return ret;
	}
}

std::pair<int, int> vertical_trisection(int m, int n, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>&t) {
	// mozemo rotirati i uraditi horizontalnu
	std::pair<int, int> s1 = {s.second, s.first}, t1 = {t.second, t.first};
	std::pair<int, int> ret = horizontal_trisection(n, m, y, x, s1, t1);
	return {ret.second, ret.first};
}

typedef struct peeling {
	int r1, r2, r3, r4;

	friend std::ostream& operator<<(std::ostream& os, peeling& p) {
		os
		<< "r1: " << p.r1
		<< ", r2: " << p.r2
		<< ", r3: " << p.r3
		<< ", r4: " << p.r4
		<< '\n';
		return os;
	}

} peeling;

inline bool out_of_bounds(int x, int l, int r) {
	return (x < l && x > r);
}

inline bool is_corner_vertex(int x, int m) {
	// samo ako je x == 0 ili x == m - 1
	// ako je x == m - 1, onda je x - m - 1 == 0
	return !x or !(x - m + 1);
}

inline bool connected_by_non_boundary_edge(const std::pair<int, int>& s, const std::pair<int, int>& t, int m, int n) {
	// ovo implicira da je matrica 2xm ili nx2
	// za 2xm je ovo ispunjeno ako su ime iste x-koordinate, razlicite y-koordinate i x > 0 i x < m - 1
	// za nx2 slicno
	// s.first == t.first, s.first, t.first su iz {0, 1}
	// s.first - t.first == 0
	// s.first xor t.first
	return (n == 2 and s.second == t.second and !is_corner_vertex(s.second, m) and (s.first ^ t.first))
	or (m == 2 and s.first == t.first and !is_corner_vertex(s.first, n) and (s.second ^ t.second));
}

bool color_compatible(const std::pair<int, int>& v1, const std::pair<int, int>& v2, int width, int height) {
	int parity = (width % 2) && (height % 2);
	return (!parity && ((v1.first + v1.second + v2.first + v2.second) % 2))
		|| (parity && !((v1.first + v1.second) % 2) && !((v2.first + v2.second) % 2));
}

bool are_antipodes(const std::pair<int, int>& v1, const std::pair<int, int>& v2, int width, int height) {
	return std::min(v1.first, v2.first) <= 1
		&& std::min(v1.second, v2.second) <= 1
		&& std::max(v1.first, v2.first) >= height - 2
		&& std::max(v1.second, v2.second) >= width - 2;
}

bool has_hamiltonian_cycle(int width, int height) {
	return !((width % 2) && (height % 2)) && (width ^ 1) && (height ^ 1);
}

bool has_hamiltonian_path(std::pair<int, int> s, std::pair<int, int> t, int m, int n) {

	if(out_of_bounds(s.first, 0, n)
		|| out_of_bounds(s.second, 0, m)
		|| out_of_bounds(t.first, 0, n)
		|| out_of_bounds(t.second, 0, m)) {
	throw "Error: either s or t are outside the matrix";
}
	return color_compatible(s, t, m, n) and
		!( (n == 1 and (!is_corner_vertex(s.second, m) or !is_corner_vertex(t.second, m)))
		 or (m == 1 and (!is_corner_vertex(s.first, n) or !is_corner_vertex(t.first, n)))
		 or connected_by_non_boundary_edge(s, t, m, n)
		 // n == 3, m parno...u radu stoji da treba biti izomorfno sa s crna, s.x < t.x (za sy == 1) ili t.x > s.x + 1 (za sy != 1)
		 // to znaci da mora biti ovo da vazi i kad rotiramo sliku i kad rotiramo uloge za s i t
		 // znaci, potrebno je obraditi slucaj kada se slika zarotira oko y-ose
		 // ali tada se menjaju boje
		 // tako da je potrebno samo razmotriti ovaj slucaj kada je s crna ili kada je t crna 
		 or (n == 3 and !(m % 2) and ((s.first + s.second) % 2) and ((s.first == 1 and s.second < t.second) or (s.first != 1 and t.second > s.second + 1)))
		 or (n == 3 and !(m % 2) and ((t.first + t.second) % 2) and ((t.first == 1 and t.second < s.second) or (t.first != 1 and s.second > t.second + 1)))
		 or (m == 3 and !(n % 2) and ((s.second + s.first) % 2) and ((s.second == 1 and s.first < t.first) or (s.second != 1 and t.first > s.first + 1)))
		 or (m == 3 and !(n % 2) and ((t.first + t.second) % 2) and ((t.second == 1 and t.first < s.first) or (t.second != 1 and s.first > t.first + 1)))
		);
}


std::pair<int, int> H_C_M1_M3_CCW(int m, int n, int x, int y) {
	// leva ili gornja strana ce biti konkavne
	// desna mora biti ravna zbog M1
	// donja mora biti ravna zbog M3
	if((n % 2) && (m % 2)) return {-1 , -1};

	if(!(n % 2)) {
		// za y parno, idemo levo
		// za y neparno, idemo desno
		// na koloni m-1 idemo gore

		if(y % 2) {
			// idemo desno za x < m - 2
			// za x == m - 2, spustamo se dole ako nije y == n - 1
			// ako je x == m - 2 i y == n - 1, onda idemo desno
			// za x == m - 1, idemo na gore
			return {y + (x == m - 2 && y < n - 1) - (x == m - 1), x + (x < m - 2) + (x == m - 2 && y == n - 1)};
		} else {
			// idemo levo za x > 0 i x < m - 1
			// x > 0 <-> !(!x) == 1 && x >= 0
			// za x == 0, spustamo se dole, osim kada je y == n - 1 (tada idemo desno)
			// za x == m - 1 idemo na gore kada nije y == 0
			// za x == m - 1 i y == 0 idemo levo
			return {y + (!x && y < n - 1) - (x == m - 1 && !(!y)), x - ((!(!x) && x < m - 1) || (!y && x == m - 1)) + (!x && y == n - 1)};

		}


	} else {
			// od donjeg desnog ugla na gore, pa cik-cak po kolonama na levo
			// za x neparno, idemo na gore
			// za parno x, idemo na dole
			// pazimo na y == 0, y == n - 2 i y == n - 1
		if(x % 2) {
			// za y == 0, skrecemo levo (x - 1, y)
			// za y == n - 1 idemo desno (x + 1, y), osim za x == m - 1 (tada idemo gore)
			// inace idemo samo gore (x, y - 1)
			// x koordinatu resava funkcija (x - 1)*!y + (x + 1)*(y == n - 1 && x != m - 1) + x*(y > 0 && y < n - 1) + x*(y == n - 1 && x == m - 1)
			// imamo x - !y + (y == n - 1 && x != m - 1)
			// ako ubacimo y == 0, imamo x - 1 + 0 = x - 1
			// y == n - 1, dobijamo x - 0 + 1 = x + 1
			// a inace x - 0 + 0 = x
			return {y - (!(!y) && y < n - 1) - (y == n - 1 && x == m - 1), x - !y + (y == n - 1 && x != m - 1)};
		} else {
			// za y < n - 2, idemo dole (x, y + 1)
			// za y == n - 2, idemo levo za x > 0 (x - 1, y), a inace na dole
			// za y == n - 1, idemo desno (x + 1, y)
			// x koordinata: x - (y == n - 2) + (y == n - 1)
			// y koordinata: y + (y > 0 && y < n - 2)
			return {y + (y < n - 2) + (!x && y == n - 2), x - (y == n - 2 && !(!x)) + (y == n - 1)};
		}

	}
}

// CW za M1 dobijam tako sto okrenem matricu oko x-ose
// pa pozovem funkciju i vratim koordinate
std::pair<int, int> H_C_M1_CW(int m, int n, int x, int y) {
	auto ret = H_C_M1_M3_CCW(m, n, x, n - 1 - y);
	ret.first = n - 1 - ret.first;
	return ret;
}

// CW za M3 dobijam tako sto rotiam matricu oko y-ose
// pa pozovem funkciju i vratim koordinate

std::pair<int, int> H_C_M3_CW(int m, int n, int x, int y) {
	auto ret = H_C_M1_M3_CCW(m, n, m - 1 - x, y);
	ret.second = m - 1 - ret.second;
	return ret;
}

// za M2 je bitno da leva bude ravna
// za M4 da bude to gornja
// H_C_M1_M3_CCW obezbedjuje da su donja i desna ravne
// ako je rotiramo po obe ose, ostajemo na CCW
// ali sada za M2 i M4

std::pair<int, int> H_C_M2_M4_CCW(int m, int n, int x, int y) {
	auto ret = H_C_M1_M3_CCW(m, n, m - 1 - x, n - 1 - y);
	ret.second = m - 1 - ret.second;
	ret.first = n - 1 - ret.first;
	return ret;
}


// CW za M4 dobijam tako sto rotiam matricu oko y-ose
// pa pozovem funkciju i vratim koordinate

std::pair<int, int> H_C_M4_CW(int m, int n, int x, int y) {
	auto ret = H_C_M2_M4_CCW(m, n, m - 1 - x, y);
	ret.second = m - 1 - ret.second;
	return ret;
}

// CW za M2 dobijam tako sto okrenem matricu oko x-ose
// pa pozovem funkciju i vratim koordinate
std::pair<int, int> H_C_M2_CW(int m, int n, int x, int y) {
	auto ret = H_C_M2_M4_CCW(m, n, x, n - 1 - y);
	ret.first = n - 1 - ret.first;
	return ret;
}

peeling peel(const std::pair<int, int>& s, const std::pair<int, int>& t, int m, int n) {
	peeling r;

	r.r1 = std::min(s.first, t.first) - 1;
	r.r2 = std::max(s.first, t.first);
	r.r3 = std::min(s.second, t.second) - 1;
	r.r4 = std::max(s.second, t.second);

	// if(!(r.r1 % 2)) r.r1--;
	r.r1 -= !(r.r1 % 2);
	// if(!(_height - r.r2) % 2) r.r2++;
	r.r2 += !((n - r.r2) % 2);
	// if(!(r.r3 % 2)) r.r3--;
	r.r3 -= !(r.r3 % 2);
	// if(!(_width - r.r4) % 2) r.r4++;
	r.r4 += !((m - r.r4) % 2);
	return r;
}

constexpr inline int f(int y, int x) {
	return x + 3*y;
}

constexpr inline std::pair<int, int> f_inv(int c) {
	return {c / 3, c % 3};
}

inline std::pair<int, int> reflect_over_x(int x, int y, int n) {
	return {n - 1 - y, x};
}

inline std::pair<int, int> reflect_over_y(int x, int y, int m) {
	return {y, m - 1 - x};
}

inline std::pair<int, int> reflect_over_xy(int x, int y, int m, int n) {
	return {n - 1 - y, m  - 1 - x};
}


std::pair<int, int> find_path_m5(int m, int n, int x, int y, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	if(n >= 4 && (m > 3 || (m == 3 && !((s.first + s.second) % 2) ))) return horizontal_trisection(m, n, x, y, s, t);
	
	if(n >= 4) {
		// za m == 1 i m == 2, imamo vec dole obradjene slucajeve
		// za m == 3, resavamo ovde
		if(m == 3) {
			// tada znamo da je s crna, jer nije prosao gornji slucaj
			// ovde je junction vertex na sredini
			// ako je t iznad s, onda okrenemo graf po x-osi
			
			if(t.second < s.second) {
				auto s1 = reflect_over_x(s.second, s.first, n);
				auto t1 = reflect_over_x(t.second, t.first, n);
				auto ret = find_path_m5(m, n, x, n - 1 - y, s1, t1);
				if(ret.first == -1 || ret.second == -1) return {-1, -1};
				return reflect_over_x(ret.second, ret.first, n);
			}
			
			// sada je s gore
			// spajamo s sa (1, 1), (1, 1) sa (2, 1), a (2, 1) sa t
			if(y <= 1) {
				if(x == 1 && y == 1) return {2, 1};
				std::pair<int, int> p = {1, 1};
				return path_2_m(3, x, y, s, p);
			} else {
				std::pair<int, int> q = {0, 1}, t1 = {t.first - 2, t.second};
				auto ret = path_2_m(3, x, y - 2, q, t1);
				if(ret.first == -1 || ret.second == -1) return {-1, -1};
				ret.first += 2;
				return ret;
			}
		}
	}
	
	if(m >= 4 && (n > 3 || (n == 3 && !((s.first + s.second) % 2) ))) return vertical_trisection(m, n, x, y, s, t);
	
	if(m >= 4) {
		if(n == 3) {
			if(t.first < s.first) {
				auto s1 = reflect_over_y(s.second, s.first, m);
				auto t1 = reflect_over_y(t.second, t.first, m);
				auto ret = find_path_m5(m, n, m - 1 - x, y, s1, t1);
				if(ret.first == -1 || ret.second == -1) return {-1, -1};
				return reflect_over_y(ret.second, ret.first, m);
			}
			
			// spajamo s sa (1, 1), (1, 1) sa (1, 2), a (1, 2) sa t
			if(x <= 1) {
				if(x == 1 && y == 1) return {1, 2};
				std::pair<int, int> p = {1, 1};
				return path_n_2(3, x, y, s, p);
			} else {
				std::pair<int, int> q = {1, 0}, t1 = {t.first, t.second - 2};
				auto ret = path_n_2(3, x - 2, y, q, t1);
				if(ret.first == -1 || ret.second == -1) return {-1, -1};
				ret.second += 2;
				return ret;
			}
		}
	
	}
	
	
	// ovde je m < 4 i n < 4
	// imamo slucajeve:
	
	// 1x2, 1x3, 2x1, 3x1
	if(m == 1) {
		// vracamo onaj dole/gore (ako on nije poslednji)
		if(!s.first && y < n - 1) return {y + 1, x};
		if(s.first && y > 0) return {y - 1, x};
		else return {-1, -1};
	}
	
	if(n == 1) {
		if(!s.second && x < m - 1) return {y, x + 1};
		if(s.second && x > 0) return {y, x - 1};
		else return {-1, -1};
	}
	
	// 2x2, 2x3, 3x2 --> pozovemo funkciju za 2xm i nx2
	if(n == 2) return path_2_m(m, x, y, s, t);
	if(m == 2) return path_n_2(n, x, y, s, t);
	
	// 3x3
	// s i t su beli, jer inace nema puta
	// imamo 4 slucaja:
	// 1) (0, 0) --> (2, 2) i varijacije
	// 2) (0, 0) --> (2, 0) i varijacije
	// 3) (0, 0) --> (1, 1) i varijacije
	// 4) (1, 1) --> (0, 0) i varijacije
	// konstruisemo 4 liste da obuhvate te slucajeve
	// ostale svodimo izomorfnim transformacijama na ta 4 slucaja
	// a[i] predstavlja sledeci element od {x, y} koji odgovara i
	// numerisemo {x, y} brojevima od 0 do 8
	// {0, 0} --> 0, {1, 0} --> 1, {2, 2} --> 8
	// f(x, y) = x + 3y
	std::array<int, 9> a1 = {f(1, 0), f(0, 2), f(1, 2), f(2, 0), f(0, 1), f(2, 2), f(2, 1), f(1, 1), f(0, -1)};
	std::array<int, 9> a2 = {f(1, 0), f(0, 2), f(1, 2), f(1, 1), f(0, 1), f(2, 2), f(0, -1), f(2, 0), f(2, 1)};
	std::array<int, 9> a3 = {f(1, 0), f(1, 1), f(0, 1), f(2, 0), f(0, -1), f(0, 2), f(2, 1), f(2, 2), f(1, 2)};
	std::array<int, 9> a4 = {f(0, -1), f(0, 0), f(0, 1), f(2, 0), f(1, 0), f(0, 2), f(2, 1), f(2, 2), f(1, 2)};
	
	
	if(!s.first && !s.second) {
		// s je (0, 0)
		// granamo po t
		// t moze biti (0, 2), (2, 0), (1, 1) i (2, 2)
		if(!t.second) {
			// t je (2, 0)
			// vracamo sledeci iz liste a2
			return f_inv(a2[f(y, x)]);
		}
		
		if(!t.first) {
			// rotiramo listu a2
			// (0, 0) ide u (1, 0), a trebalo bi u (0, 1)
			// (1, 1) ide u (0, 1), a trebalo bi u (1, 0)
			// (2, 2) ide u (2, 1), a trebalo bi u (1, 2)
			// znaci, samo rotiramo, pa vadimo iz a2, pa rotiramo
			auto ret = f_inv(a2[f(x, y)]);
			if(ret.first == -1 || ret.second == -1) return {-1, -1};
			return {ret.second, ret.first};
		}
		
		if(t.first == 1 && t.second == 1) {
			// vratimo sledeci iz a3
			return f_inv(a3[f(y, x)]);
		}
		
		if(t.first == 2 && t.second == 2) {
			// vratimo sledeci iz a1
			return f_inv(a1[f(y, x)]);
		}
		
	} else if(!s.first) {
		// s je (0, 2)
		// t moze biti (0, 0), (1, 1), (2, 0) i (2, 2)
		if(!t.first && !t.second) {
			// t je (0, 0)
			// ovo je kompozicija refleksije po y-osi i rotacije slucaja a2
			// refleksijom se dobija s' = (0, 0), t' = (0, 2)
			// a rotacijom s" = (0, 0), t" = (0, 0)
			auto step1 = reflect_over_y(x, y, 3);
			auto ret = f_inv(a2[f(step1.second, step1.first)]);
			// opet rotiramo, pa reflektujemo
			if(ret.first == -1 || ret.second == -1) return {-1, -1};
			return reflect_over_y(ret.first, ret.second, 3);
		}
		
		if(t.first == 2 && !t.second) {
			// t je (2, 0)
			// ovo je kompozicija rotacije i refleksije po x-osi slucaja a1
			// rotacija daje s' = (2, 0), t' = (0, 2)
			// refleksija daje s" = (0, 0), t"= (2, 2)
			auto step1 = reflect_over_x(y, x, 3);
			auto step2 = f_inv(a1[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			auto ret = reflect_over_x(step2.second, step2.first, 3);
			return {ret.second, ret.first};
		}
		
		if(t.first == 1 && t.second == 1) {
			// ovo je refleksija po y-osi slucaja a3
			// s' = (0, 0), t' = (1, 1)
			auto step1 = reflect_over_y(x, y, 3);
			auto step2 = f_inv(a3[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_y(step2.second, step2.first, 3);
		}
		
		if(t.first == 2 && t.second == 2) {
			// ovo je refleksija po y-osi slucaja a2
			// s' = (0, 0), t' = (2, 0)
			auto step1 = reflect_over_y(x, y, 3);
			auto step2 = f_inv(a2[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_y(step2.second, step2.first, 3);
		}
		
	} else if(!s.second) {
		// s je (2, 0)
		// t moze biti (0, 0), (1, 1), (0, 2), (2, 2)
		
		if(!t.first && !t.second) {
			// refleksija po x-osi slucaja a2
			auto step1 = reflect_over_x(x, y, 3);
			auto step2 = f_inv(a2[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_x(step2.second, step2.first, 3);
		}
		
		if(!t.first && t.second == 2) {
			// refleksija po x-osi slucaja a1
			auto step1 = reflect_over_x(x, y, 3);
			auto step2 = f_inv(a1[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_x(step2.second, step2.first, 3);
		}
		
		if(t.first == 1 && t.second == 1) {
			// refleksija po x-osi slucaja a3
			auto step1 = reflect_over_x(x, y, 3);
			auto step2 = f_inv(a3[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_x(step2.second, step2.first, 3);
		}
		
		if(t.first == 2 && t.second == 2) {
			// refleksija po x-osi, pa onda rotacija slucaja a2
			auto step1 = reflect_over_x(x, y, 3);
			auto ret = f_inv(a2[f(step1.second, step1.first)]);
			if(ret.first == -1 || ret.second == -1) return {-1, -1};
			// opet rotiramo, pa reflektujemo
			return reflect_over_x(ret.first, ret.second, 3);
		}
		
	} else if(s.first == 1) {
		// s je (1, 1)
		// t moze biti (0, 0), (2, 0), (0, 2) i (2, 2)
		
		if(!t.first && !t.second) {
			// slucaj a4
			return f_inv(a4[f(y, x)]);
		}
		
		if(!t.first) {
			// refleksija po x-osi slucaja a4
			auto step1 = reflect_over_y(x, y, 3);
			auto step2 = f_inv(a4[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_y(step2.second, step2.first, 3);
		}
		
		if(!t.second) {
			// refleksija po y-osi slucaja a4
			auto step1 = reflect_over_x(x, y, 3);
			auto step2 = f_inv(a4[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_x(step2.second, step2.first, 3);
		}
		
		if(t.first && t.second) {
			// refleksija po x i refleksija po y-osi slucaja a4
			// to je zapravo centralna simetrija
			auto step1 = reflect_over_xy(x, y, 3, 3);
			auto step2 = f_inv(a4[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_xy(step2.second, step2.first, 3, 3);
		}
		
	} else {
		// s je (2, 2)
		// t moze biti (0, 0), (0, 2), (2, 0), (1, 1)
		if(!t.first && !t.second) {
			// centralna simetrija od a1
			auto step1 = reflect_over_xy(x, y, 3, 3);
			auto step2 = f_inv(a1[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_xy(step2.second, step2.first, 3, 3);
		}
		
		if(!t.first) {
			// centralna simetrija od a2
			auto step1 = reflect_over_xy(x, y, 3, 3);
			auto step2 = f_inv(a2[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_xy(step2.second, step2.first, 3, 3);
		}
		
		if(!t.second) {
			// centralna simetrija, pa rotacija slucaja a2
			auto step1 = reflect_over_xy(x, y, 3, 3);
			auto ret = f_inv(a2[f(step1.second, step1.first)]);
			if(ret.first == -1 || ret.second == -1) return {-1, -1};
			// opet rotiramo, pa reflektujemo
			return reflect_over_xy(ret.first, ret.second, 3, 3);
		}
		
		if(t.first && t.second) {
			// centralna simetrija slucaja a3
			auto step1 = reflect_over_xy(x, y, 3, 3);
			auto step2 = f_inv(a3[f(step1.first, step1.second)]);
			if(step2.first == -1 || step2.second == -1) return {-1, -1};
			return reflect_over_xy(step2.second, step2.first, 3, 3);
		}
	}
	
	return {-2, -2};
}

inline bool connectable_left(int m, int n, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	// moze se povezati samo ako nije nesto od sledeceg:
	// 1) 1xm
	// 2) 2x2 i s == (0, 0) i t == (1, 0) ili t == (0, 0) i s == (1, 0)
	return !(n == 1 || (m == 2 && n == 2 && !s.first && !s.second && t.first && !t.second)
	|| (m == 2 && n == 2 && !t.first && !t.second && s.first && !s.second));
}

inline bool connectable_top(int m, int n, std::pair<int, int>& s, std::pair<int, int>& t) {
	// svodimo na funkciju iznad preko rotacije
	std::pair<int, int> s1 = {s.second, s.first}, t1 = {t.second, t.first};
	return connectable_left(m, n, s1, t1);
}

inline bool connectable_right(int m, int n, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	auto s1 = reflect_over_y(s.second, s.first, m);
	auto t1 = reflect_over_y(t.second, t.first, m);
	return connectable_left(m, n, s1, t1);
}


inline bool connectable_bottom(int m, int n, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	auto s1 = reflect_over_x(s.second, s.first, n);
	auto t1 = reflect_over_x(t.second, t.first, n);
	return connectable_top(m, n, s1, t1);
}

inline bool m1_exists(const peeling& r) {
	return r.r3 > 0;	
}

inline bool m2_exists(const peeling& r, int m) {
	return r.r4 + 1 < m - 1;
}

inline bool m3_exists(const peeling &r) {
	return r.r1 > 0;
}

inline bool m4_exists(const peeling &r, int n) {
	return n - 1 > r.r2 + 1;
}


// trazenje orijentacije pomocu M1
std::pair<bool, std::pair<int, int>> find_orientation_m1(int m, int n, const peeling& r, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	std::pair<int, int> s1 = {s.first - r.r1 - 1, s.second - r.r3 - 1};
	std::pair<int, int> t1 = {t.first - r.r1 - 1, t.second - r.r3 - 1};
	int m1 = r.r4 - r.r3;
	int n1 = r.r2 - r.r1;
	// proveravamo da li (0, 0) ide dole
	if(auto ret = find_path_m5(m1, n1, 0, 0, s1, t1); !ret.second) {
		// tacka (0, 0) ide dole
		// CCW je orijentacija
		// tacka (0, 0) ide levo od sebe
		return {true, {r.r1 + 1, r.r3}};
	} else {
		// proveravamo (1, 0)
		ret = find_path_m5(m1, n1, 0, 1, s1, t1);
		if(!ret.second) {
			// ide na gore
			// CW je orijentacija
			return {ret.first, {r.r1 + 2, r.r3}};
		} else {
			// proveravamo (2, 0)
			ret = find_path_m5(m1, n1, 0, 2, s1, t1);
			if(!ret.second and ret.first == 1) {
				return {false, {r.r1 + 3, r.r3}};
			} else {
				return {true, {r.r1 + 3, r.r3}};
			}
		}
	}
}

std::pair<bool, std::pair<int, int>> find_orientation_m2(int m, int n, const peeling& r, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	// refleksijom svodimo na slucaj sa M1
	auto s2 = reflect_over_y(s.second, s.first, m);
	auto t2 = reflect_over_y(t.second, t.first, m);
	peeling R = {r.r1, r.r2, m - r.r4 - 2, m - r.r3 - 2};
	auto ret = find_orientation_m1(m, n, R, s2, t2);
	return {!ret.first, reflect_over_y(ret.second.second, ret.second.first, m)};
}

// povezujemo M5 sa M1
std::pair<int, int> connect_m5_m1(int m, int n, int x, int y, const peeling& r, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	// dohvatimo orijentaciju
	
	auto T = find_orientation_m1(m, n, r, s, t);
	auto ret = T.second;
	if(y == ret.first && x == ret.second + 1) {
		return go_left(x, y);
	} else {
		std::pair<int, int> s1 = {s.first - r.r1 - 1, s.second - r.r3 - 1};
		std::pair<int, int> t1 = {t.first - r.r1 - 1, t.second - r.r3 - 1};
		int m1 = r.r4 - r.r3;
		int n1 = r.r2 - r.r1;
		ret = find_path_m5(m1, n1, x - r.r3 - 1, y - r.r1 - 1, s1, t1);
		if(ret.first == -1 || ret.second == -1) return {-1, -1};
		ret.first += r.r1 + 1;
		ret.second += r.r3 + 1;
		return ret;
	}
}

// povezujemo M5 sa M2
std::pair<int, int> connect_m5_m2(int m, int n, int x, int y, const peeling& r, const std::pair<int, int>& s, const std::pair<int, int>& t) {
	// svodimo na M1 refleksijom
	auto s2 = reflect_over_y(s.second, s.first, m);
	auto t2 = reflect_over_y(t.second, t.first, m);
	peeling R = {r.r1, r.r2, m - r.r4 - 2, m - r.r3 - 2};
	auto ret = connect_m5_m1(m, n, m - 1 - x, y, R, s2, t2);
	if(ret.first == -1 or ret.second == -1) return {-1, -1};
	return reflect_over_y(ret.second, ret.first, m);
}

// funkcija prima parametre vezane za mrezu,
// dve bool vrednosti kojima odredjuje kada je u kriticnoj tacki
// i da li se povezuje sa susedima
// i prima dve funkcije da obrade ta dva slucaja (f1, f2),
// i prima dve funkcije da ispisu ciklus inace
template<typename F, typename C>
inline std::pair<int, int> connect_block(int m, int n, int x, int y, const peeling& r, const std::pair<int, int>& s, const std::pair<int, int>& t, bool b1, bool b2, bool orientation, F& f1, F& f2, C& cycle1, C& cycle2, int m1, int n1, int tx, int ty) {
	
	if(b1) {
		return f1(m, n, r, orientation);
	}
	
	if(b2) {
		return f2(m, n, r, orientation);
	}
	
	if(orientation) {
		return cycle1(m1, n1, x - tx, y - ty) + std::pair{ty, tx};
	} else {
		return cycle2(m1, n1, x - tx, y - ty) + std::pair{ty, tx};
	}	
}

inline std::pair<int, int> connect_m1_m3(int m, int n, const peeling& r, bool orientation) {
	if(orientation) {
		return {1, r.r3 + 1};
	} else {
		return {0, r.r3 + 1};
	}
}

inline std::pair<int, int> connect_m1_m4(int m, int n, const peeling& r, bool orientation) {
	if(orientation) {
		return {n - 1, r.r3 + 1};
	} else {
		return {n - 2, r.r3 + 1};
	}
}

inline std::pair<int, int> connect_m3_m1(int m, int n, const peeling& r, bool orientation) {
	if(orientation) {
		return {0, r.r3};
	} else {
		return {1, r.r3};
	}
}

inline std::pair<int, int> connect_m4_m1(int m, int n, const peeling& r, bool orientation) {
	if(orientation) {
		return {n - 2, r.r3};
	} else {
		return {n - 1, r.r3};
	}
}

inline std::pair<int, int> connect_m2_m3(int m, int n, const peeling& r, bool orientation) {
	if(orientation) {
		return {0, r.r4};
	} else {
		return {1, r.r4};
	}
}

inline std::pair<int, int> connect_m2_m4(int m, int n, const peeling& r, bool orientation) {
	if(orientation) {
		return {n - 2, r.r4};
	} else {
		return {n - 1, r.r4};
	}
}

inline std::pair<int, int> connect_m3_m2(int m, int n, const peeling& r, bool orientation) {
	if(orientation) {
		return {1, r.r4 + 1};
	} else {
		return {0, r.r4 + 1};
	}
}

inline std::pair<int, int> connect_m4_m2(int m, int n, const peeling& r, bool orientation) {
	if(orientation) {
		return {n - 1, r.r4 + 1};
	} else {
		return {n - 2, r.r4 + 1};
	}
}

inline bool connect_m1_m5_ccw(int x, int y, std::pair<int, int>& ret) {
	return x == ret.second and y == ret.first + 1;
}

inline bool connect_m1_m5_cw(int x, int y, std::pair<int, int>& ret) {
	return x == ret.second and y == ret.first - 1;
}

inline bool connect_m2_m5_ccw(int x, int y, std::pair<int, int>& ret) {
	return x == ret.second and y == ret.first - 1;
}

inline bool connect_m2_m5_cw(int x, int y, std::pair<int, int>& ret) {
	return x == ret.second and y == ret.first + 1;
}

inline bool connect_m3_m5_ccw(int x, int y, std::pair<int, int>& ret) {
	return x == ret.second - 1 and y == ret.first;
}

inline bool connect_m3_m5_cw(int x, int y, std::pair<int, int>& ret) {
	return x == ret.second + 1 and y == ret.first;
}

inline bool connect_m4_m5_ccw(int x, int y, std::pair<int, int>& ret) {
	return x == ret.second + 1 and y == ret.first;
}

inline bool connect_m4_m5_cw(int x, int y, std::pair<int, int>& ret) {
	return x == ret.second - 1 and y == ret.first;
}

inline bool not_connect_m5_ccw(int x, int y, std::pair<int, int>& ret) {
	return false;
}

inline bool not_connect_m5_cw(int x, int y, std::pair<int, int>& ret) {
	return false;
}


template<typename F, typename C, typename G, typename K, typename Cycle>
inline std::pair<int, int> connect_to_path(int m, int n, int x, int y, const peeling& r, const std::pair<int, int>& s, const std::pair<int, int>& t, F& orient, C& connect_m5_ccw, C& connect_m5_cw, G& go, bool c1, bool c2, bool c3, bool c4, K& connect1, K& connect2, Cycle& cycle_ccw, Cycle& cycle_cw, int m1, int n1, int tx, int ty) {
	// treba odrediti orijentaciju
	// a onda povezati sa M5
	auto T = orient(m, n, r, s, t);
	auto orientation = T.first;
	auto ret = T.second;
	
	if(orientation) {
		// ako je CCW, onda povezujemo onu ispod ret desno
		// if(x == ret.second and y == ret.first + 1) return go_right(x, y);
		
		if(connect_m5_ccw(x, y, ret)) {
			return go(x, y);
		}
		
		return connect_block(m, n, x, y, r, s, t, c1, c2, true, connect1, connect2, cycle_ccw, cycle_cw, m1, n1, tx, ty);
	
	} else {
		// ako je CW, onda povezujemo onu iznad desno
		//if(x == ret.second && y == ret.first - 1) return go_right(x, y);
		if(connect_m5_cw(x, y, ret)) {
			return go(x, y);
		}
		
		return connect_block(m, n, x, y, r, s, t, c3, c4, false, connect1, connect2, cycle_ccw, cycle_cw, m1, n1, tx, ty);
	}
}

std::pair<int, int> hamiltonian_path_util(int m, int n, int x, int y, std::pair<int, int>& s, std::pair<int, int>& t, peeling& r) {
	std::pair<int, int> s1 = {s.first - r.r1 - 1, s.second - r.r3 - 1};
	std::pair<int, int> t1 = {t.first - r.r1 - 1, t.second - r.r3 - 1};
	int m1 = r.r4 - r.r3;
	int n1 = r.r2 - r.r1;
	// ako postoji u M5 i tacka je u M5, idi u M5
	// inace specijalan slucaj...
	
	if(has_hamiltonian_path(s1, t1, m1, n1) and n1 >= 2 and m1 >= 2) {
		if(m1_exists(r) and connectable_left(m1, n1, s1, t1)) {
			// obradjujemo posebno svaki blok
			if(x <= r.r3) {
				// u M1 smo
				return connect_to_path(m, n, x, y, r, s, t, find_orientation_m1, connect_m1_m5_ccw, connect_m1_m5_cw, go_right,
				m3_exists(r) and x == r.r3 and y == 1,
				m4_exists(r, n) and (!m2_exists(r, m) or !m3_exists(r)) and x == r.r3 and y == n - 1,
				m3_exists(r) and x == r.r3 and !y,
				m4_exists(r, n) and (!m2_exists(r, m) or !m3_exists(r)) and x == r.r3 and y == n - 2,
				connect_m1_m3, connect_m1_m4, H_C_M1_M3_CCW, H_C_M1_CW, r.r3 + 1, n, 0, 0);
			} else if(x > r.r4) {
				// u M2 smo
				// vezemo se za put samo ako M3 i M4 ne postoje
				if(!m3_exists(r) and !m4_exists(r, n)) {
					return connect_to_path(m, n, x, y, r, s, t, find_orientation_m2, connect_m2_m5_ccw, connect_m2_m5_cw, go_left,
					false,
					false,
					false,
					false,
					connect_m2_m3, connect_m2_m4, H_C_M2_M4_CCW, H_C_M2_CW, m - r.r4 - 1, n, r.r4 + 1, 0);
				} else {
					return connect_to_path(m, n, x, y, r, s, t, find_orientation_m1, not_connect_m5_ccw, not_connect_m5_cw, go_left,
					m3_exists(r) and x == r.r4 + 1 and !y,
					m4_exists(r, n) and x == r.r4 + 1 and y == n - 2,
					m3_exists(r) and x == r.r4 + 1 and y == 1,
					m4_exists(r, n) and x == r.r4 + 1 and y == n - 1,
					connect_m2_m3, connect_m2_m4, H_C_M2_M4_CCW, H_C_M2_CW, m - r.r4 - 1, n, r.r4 + 1, 0);
				}
			} else {
				if(y <= r.r1) {
					// u M3 smo
					return connect_to_path(m, n, x, y, r, s, t, find_orientation_m1, not_connect_m5_ccw, not_connect_m5_cw, go_down,
					m1_exists(r) and x == r.r3 + 1 and !y,
					m2_exists(r, m) and x == r.r4 and y == 1,
					m1_exists(r) and x == r.r3 + 1 and y == 1,
					m2_exists(r, m) and x == r.r4 and !y,
					connect_m3_m1, connect_m3_m2, H_C_M1_M3_CCW, H_C_M3_CW, r.r4 - r.r3, r.r1 + 1, r.r3 + 1, 0);
				} else if(y > r.r2) {
					// u M4 smo
					return connect_to_path(m, n, x, y, r, s, t, find_orientation_m1, not_connect_m5_ccw, not_connect_m5_cw, go_up,
					m2_exists(r, m) and x == r.r4 and y == n - 1,
					m1_exists(r) and (!m2_exists(r, m) or !m3_exists(r)) and x == r.r3 + 1 and y == n - 2,
					m2_exists(r, m) and x == r.r4 and y == n - 2,
					m1_exists(r) and (!m2_exists(r, m) or !m3_exists(r)) and x == r.r3 + 1 and y == n - 1,
					connect_m4_m2, connect_m4_m1, H_C_M2_M4_CCW, H_C_M4_CW, r.r4 - r.r3, n - r.r2 - 1, r.r3 + 1, r.r2 + 1);
					
				} else {
					// u M5 smo
					// treba paziti na slucaj kada M3 i M4 ne postoje
					if(!m3_exists(r) and !m4_exists(r, n) and x == r.r4)
						return connect_m5_m2(m, n, x, y, r, s, t);
					return connect_m5_m1(m, n, x, y, r, s, t);
				}
			}
		
		} else if(m3_exists(r) and connectable_top(m1, n1, s1, t1)) {
			// svodimo na slucaj sa M1
			peeling R = {r.r3, r.r4, r.r1, r.r2};
			std::pair<int, int> s2 = {s.second, s.first}, t2 = {t.second, t.first};
			auto ret = hamiltonian_path_util(n, m, y, x, s2, t2, R);
			return {ret.second, ret.first};
		} else if(m2_exists(r, m) and connectable_right(m1, n1, s1, t1)) {
			// svodimo na M1 pomocu refleksije po y-osi
			auto s2 = reflect_over_y(s.second, s.first, m);
			auto t2 = reflect_over_y(t.second, t.first, m);
			peeling R = {r.r1, r.r2, m - r.r4 - 2, m - r.r3 - 2};
			auto ret = hamiltonian_path_util(m, n, m - x - 1, y, s2, t2, R);
			if(ret.first == -1 or ret.second == -1) return {-1, -1};
			return reflect_over_y(ret.second, ret.first, m);
		} else {
			// povezujemo sa M4
			// svodimo na M1 pomocu refleksije po x-osi i rotacije
			auto s2 = reflect_over_x(s.second, s.first, n);
			auto t2 = reflect_over_x(t.second, t.first, n);
			s2 = {s2.second, s2.first};
			t2 = {t2.second, t2.first};
			peeling R = {r.r3, r.r4, n - 2 - r.r2, n - 2 - r.r1};
			auto ret = hamiltonian_path_util(n, m, n - y - 1, x, s2, t2, R);
			if(ret.first == -1 or ret.second == -1) return {-1, -1};
			ret = {ret.second, ret.first};
			return reflect_over_x(ret.second, ret.first, n);
		}
		
		
	} else if(has_hamiltonian_path(s1, t1, m1, n1)) {
		// slucaj kada je n1 = 1 ili m1 = 1
		if(n1 == 1) {
			// pokusavamo redom da spojimo sa M1, M2, M3 i M4
			// spajanje postoji akko postoji odgovarajuci blok
			// i nije t sa te strane
			if(s1.second) {
				// reflektujemo sve po y-osi da bi s bilo levo
				auto s2 = reflect_over_y(s.second, s.first, m);
				auto t2 = reflect_over_y(t.second, t.first, m);
				// r.r4 + 1 postaje R.r3
				// m - 1 - r.r4 - 1 = m - r.r4 - 2
				// r.r3 + 1 postaje R.r4
				// m - 1 - r.r3 - 1 = R.r4
				peeling R = {r.r1, r.r2, m - r.r4 - 2, m - r.r3 - 2};
				auto ret = hamiltonian_path_util(m, n, m - x - 1, y, s2, t2, R);
				if(ret.first == -1 or ret.second == -1) return {-1, -1};
				return reflect_over_y(ret.second, ret.first, m);
			}
			if(m1_exists(r)) {
				// s saljemo levo
				// orijentacija zavisi od toga da li postoje
				// M3 i M4
				// ako postoji M3, idemo ka njemu
				if(m3_exists(r)) {
					// ideja: M5 --> M1 --> M3 --> M2 --> M4 --> M2 --> M3 --> M5
					if(x <= r.r3) {
						// u M1 smo
						// prosetamo se kroz ceo M1
						// tacku (r.r3, 1) spajamo sa M3
						// spajamo se sa M4 ako M2 ne postoji
						// spajanje je u tacki (r.r3, n - 2)
						if(x == r.r3 and (y == 1 or (!m2_exists(r, m) and y == n - 2))) return go_right(x, y);
						return H_C_M1_CW(r.r3 + 1, n, x, y);
					
					} else if(x > r.r4) {
						// u M2 smo
						// (r.r4 + 1, 1) spajamo sa M3
						// (r.r4 + 1, n - 1) spajamo sa M4 ako M4 postoji
						if(x == r.r4 + 1 and (y == 1 or (m4_exists(r, n) and y == n - 1))) return go_left(x, y);
						return H_C_M2_CW(m - r.r4 - 1, n, x - r.r4 - 1, y) + std::pair{0, r.r4 + 1};
					
					} else {
						if(y <= r.r1) {
							// u M3 smo
							// spajamo se sa M2 na (r.r4, 0) ako M2 postoji
							// spajamo se sa M5 na (r.r3 + 2, r.r1)
							if(m2_exists(r, m) and x == r.r4 and !y) return go_right(x, y);
							if(x == r.r3 + 2 and y == r.r1) return go_down(x, y);
							return H_C_M3_CW(r.r4 - r.r3, r.r1 + 1, x - r.r3 - 1, y) + std::pair{0, r.r3 + 1};
							
						} else if(y > r.r2) {
							// u M4 smo
							// spajamo se sa M2 ako postoji
							// a inace se spajamo sa M1
							if(m2_exists(r, m) and x == r.r4 and y == n - 2) return go_right(x, y);
							if(!m2_exists(r, m) and x == r.r3 + 1 and y == n - 1) go_left(x, y);
							return H_C_M4_CW(r.r4 - r.r3, n - r.r2 - 1, x - r.r3 - 1, y - r.r2 - 1) + std::pair{r.r2 + 1, r.r3 + 1};
						} else {
							// u M5 smo
							// spajamo (r.r3 + 1, 0) sa M1
							// inace idemo normalno
							if(x == r.r3 + 1 and y == r.r1 + 1) return go_left(x, y);
							auto ret = find_path_m5(m1, n1, x - r.r3 - 1, y - r.r1 - 1, s1, t1);
							if(ret.first == -1 or ret.second == -1) return {-1, -1};
							ret.first += r.r1 + 1;
							ret.second += r.r3 + 1;
							return ret;
						}
					}
				} else {
					// reflektujemo po x-osi i M4 postaje M3
					auto s2 = reflect_over_x(s.second, s.first, n);
					auto t2 = reflect_over_x(t.second, t.first, n);
					peeling R = {n - 2 - r.r2, n - 2 - r.r1, r.r3, r.r4};
					auto ret = hamiltonian_path_util(m, n, x, n - y - 1, s2, t2, R);
					if(ret.first == -1 or ret.second == -1) return {-1, -1};
					return reflect_over_x(ret.second, ret.first, n);
				}
			} else if(m3_exists(r)) {
				// ne mozemo ovde da rotiramo, jer se menja dimenzija
				// a u tom slucaju se oper radi rotacija i tu bi bila
				// beskonacna rekurzija
				// ostaje jedino da se razresi ovaj slucaj posebno
				if(x > r.r4) {
					// u M2 smo
					// (r.r4 + 1, 1) spajamo sa M3
					// (r.r4 + 1, n - 1) spajamo sa M4 ako M4 postoji
					if(x == r.r4 + 1 and (y == 1 or (m4_exists(r, n) and y == n - 1))) return go_left(x, y);
					return H_C_M2_CW(m - r.r4 - 1, n, x - r.r4 - 1, y) + std::pair{0, r.r4 + 1};
				} else {
					if(y <= r.r1) {
						// u M3 smo
						// u M3 smo
						// spajamo se sa M2 na (r.r4, 0) ako M2 postoji
						// spajamo se sa M5 na (r.r3 + 2, r.r1)
						if(m2_exists(r, m) and x == r.r4 and !y) return go_right(x, y);
						if(x == 1 and y == r.r1) return go_down(x, y);
						return H_C_M3_CW(r.r4 - r.r3, r.r1 + 1, x - r.r3 - 1, y) + std::pair{0, r.r3 + 1};
					} else if(y > r.r2) {
						// u M4 smo
						// spajamo samo sa M2 ako postoji
						// ako ne postoji, onda M3 zauzima (1, r.r1 + 1)
						// zato mi uzimamo (2, r.r1 + 1)
						if(m2_exists(r, m) and x == r.r4 and y == n - 2) return go_right(x, y);
						if(!m2_exists(r, m) and x == 2 and y == r.r2 + 1) return go_up(x, y);
						return ((m2_exists(r, m))?
							H_C_M4_CW(r.r4 - r.r3, n - r.r2 - 1, x - r.r3 - 1, y - r.r2 - 1) :
							H_C_M2_M4_CCW(r.r4 - r.r3, n - r.r2 - 1, x - r.r3 - 1, y - r.r2 - 1))
							+ std::pair{r.r2 + 1, r.r3 + 1};
					} else {
						// u M5 smo
						// povezujemo se sa M3 na (r.r3 + 1, r.r1 + 1)
						if(x == r.r3 + 1 and y == r.r1 + 1) return go_up(x, y);
						if(!m2_exists(r, m) and m4_exists(r, n) and x == r.r3 + 2 and y == r.r1 + 1) return go_down(x, y);
						auto ret = find_path_m5(m1, n1, x - r.r3 - 1, y - r.r1 - 1, s1, t1);
						if(ret.first == -1 or ret.second == -1) return {-1, -1};
						ret.first += r.r1 + 1;
						ret.second += r.r3 + 1;
						return ret;
					}
				
				}
			} else {
				// gledamo M4 i M2
				// ako reflektujemo i po x i po y
				// dobicemo M4 --> M3, M2 --> M1
				auto s2 = reflect_over_xy(s.second, s.first, m, n);
				auto t2 = reflect_over_xy(t.second, t.first, m, n);
				peeling R = {n - 2 - r.r2, n - 2 - r.r1, m - 2 - r.r4, m - 2 - r.r3};
				auto ret = hamiltonian_path_util(m, n, m - x - 1, n - y - 1, s2, t2, R);
				if(ret.first == -1 or ret.second == -1) return {-1, -1};
				return reflect_over_xy(ret.second, ret.first, m, n);
			}
		
		
		} else {
			// onda je m1 == 1
			// rotiramo matricu i vracamo
			std::pair<int, int> s2 = {s.second, s.first}, t2 = {t.second, t.first};
			peeling R = {r.r3, r.r4, r.r1, r.r2};
			auto ret = hamiltonian_path_util(n, m, y, x, s2, t2, R);
			return {ret.second, ret.first};
		}
	
	
	
	
	} else {
		// pretpostavicemo da je M4 neprazan (jer M3 ili M4 je neprazan)
		// ako je prazan, reflektujemo po x-osi
		if(m4_exists(r, n)) {
			// proverimo zasto se desilo da nema puta u M5
			if(n1 == 1) {
				// prvi slucaj
				if(n - 1 - r.r2 == 2 and r.r1 + 1 == 2) {
					// specijalni slucaj
					// tada sigurno postoji i M3
				} else {
					// ako M4 nema bar 3 reda, reflektujemo po x-osi
					if(n - 1 - r.r2 == 2) {
						auto s2 = reflect_over_x(s.second, s.first, n);
						auto t2 = reflect_over_x(t.second, t.first, n);
						peeling R = {n - 2 - r.r2, n - 2 - r.r1, r.r3, r.r4};
						auto ret = hamiltonian_path_util(m, n, x, n - y - 1, s2, t2, R);
						if(ret.first == -1 or ret.second == -1) return {-1, -1};
						return reflect_over_x(ret.second, ret.first, n);
					}
					// spustamo r.r2 za 1
					++r.r2;
					return hamiltonian_path_util(m, n, x, y, s, t, r);
				}
			} else if(m1 == 1) {
				// svodimo na prvi slucaj rotacijom
				std::pair<int, int> s2 = {s.second, s.first}, t2 = {t.second, t.first};
				peeling R = {r.r3, r.r4, r.r1, r.r2};
				auto ret = hamiltonian_path_util(n, m, y, x, s2, t2, R);
				return {ret.second, ret.first};
			} else if(n1 == 2) {
				// drugi slucaj
				// r.r4 postaje s.second
				r.r4 = s.second;
				return hamiltonian_path_util(m, n, x, y, s, t, r);
			} else if(m1 == 2) {
				// r.r2 postaje s.first
				r.r2 = s.first;
				return hamiltonian_path_util(m, n, x, y, s, t, r);
			} else if(n1 == 3) {
				// treci slucaj
			} else {
				// onda je m1 == 3
				// svodimo na treci slucaj rotacijom
				std::pair<int, int> s2 = {s.second, s.first}, t2 = {t.second, t.first};
				peeling R = {r.r3, r.r4, r.r1, r.r2};
				auto ret = hamiltonian_path_util(n, m, y, x, s2, t2, R);
				return {ret.second, ret.first};
			}
		} else {
			// reflektujemo po x-osi
			auto s2 = reflect_over_x(s.second, s.first, n);
			auto t2 = reflect_over_x(t.second, t.first, n);
			peeling R = {n - 2 - r.r2, n - 2 - r.r1, r.r3, r.r4};
			auto ret = hamiltonian_path_util(m, n, x, n - y - 1, s2, t2, R);
			if(ret.first == -1 or ret.second == -1) return {-1, -1};
			return reflect_over_x(ret.second, ret.first, n);
		}
	
	}
	
	return {-1, -1};
}

std::pair<int, int> find_hamiltonian_path(int m, int n, int x, int y, std::pair<int, int>& s, std::pair<int, int>& t) {
	// ako su s i t ista tacka, gledamo ima li ciklusa
	// ako ima, nalazimo ga
	if(s == t) {
		if(!has_hamiltonian_cycle(m, n)) return {-1, -1};
		return H_C_M1_M3_CCW(m, n, x, y);
	}
	
	// ako nema puta, batali
	if(!has_hamiltonian_path(s, t, m, n)) return {-1, -1};
	
	if(are_antipodes(s, t, m, n)) return find_path_m5(m, n, x, y, s, t);
	
	peeling r = peel(s, t, m, n);
	
	
	// util funkcija ce da obradi posebne slucajeve
	// recimo, korisno je da se kod ponovi ako se u
	// odredjenom slucaju menjaju granice za peeling
	// zato util funkcija prima peeling kao argument
	return hamiltonian_path_util(m, n, x, y, s, t, r);
}


int main() {

	std::pair<int, int> s, t;
	int n, m;
	
	std::cout << "Unesi s: ";
	std::cin >> s;
	std::cout << "Unesi t: ";
	std::cin >> t;
	std::cout << "Unesi m: ";
	std::cin >> m;
	std::cout << "Unesi n: ";
	std::cin >> n;
	
	// find_hamiltonian_path(int m, int n, int x, int y, std::pair<int, int>& s, std::pair<int, int>& t)
	auto x = s;
	
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < m; j++) {
			std::cout << find_hamiltonian_path(m, n, j, i, s, t) << " "; 
		}
		std::cout << '\n';
	}
	
	
	for(int i = 0; i < n*m; i++) {
		x = find_hamiltonian_path(m, n, x.second, x.first, s, t);
	}
	
	
	std::cout << x << '\n';

	return 0;
}
