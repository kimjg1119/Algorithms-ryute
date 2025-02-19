#include <bits/stdc++.h>

using namespace std;

template<int id>
struct modular_unfixed_base{
	static unsigned int _mod;
	static unsigned int &mod(){
		return _mod;
	}
	static unsigned long long inverse_mod;
	static void precalc_barrett(){
		inverse_mod = (unsigned long long)-1 / _mod + 1;
	}
	template<class istream = decltype(cin)>
	static void setup(istream &in = cin){
		in >> _mod;
		precalc_barrett();
	}
	template<class T>
	static vector<modular_unfixed_base> precalc_power(T base, int SZ){
		vector<modular_unfixed_base> res(SZ + 1, 1);
		for(auto i = 1; i <= SZ; ++ i) res[i] = res[i - 1] * base;
		return res;
	}
	static vector<modular_unfixed_base> _INV;
	static void precalc_inverse(int SZ){
		if(_INV.empty()) _INV.assign(2, 1);
		for(auto x = _INV.size(); x <= SZ; ++ x) _INV.push_back(_mod / x * -_INV[_mod % x]);
	}
	// _mod must be a prime
	static modular_unfixed_base _primitive_root;
	static modular_unfixed_base primitive_root(){
		if(_primitive_root) return _primitive_root;
		if(_mod == 2) return _primitive_root = 1;
		if(_mod == 998244353) return _primitive_root = 3;
		unsigned int divs[20] = {};
		divs[0] = 2;
		int cnt = 1;
		unsigned int x = (_mod - 1) / 2;
		while(x % 2 == 0) x /= 2;
		for(auto i = 3; 1LL * i * i <= x; i += 2){
			if(x % i == 0){
				divs[cnt ++] = i;
				while(x % i == 0) x /= i;
			}
		}
		if(x > 1) divs[cnt ++] = x;
		for(auto g = 2; ; ++ g){
			bool ok = true;
			for(auto i = 0; i < cnt; ++ i){
				if((modular_unfixed_base(g) ^ (_mod - 1) / divs[i]) == 1){
					ok = false;
					break;
				}
			}
			if(ok) return _primitive_root = g;
		}
	}
	constexpr modular_unfixed_base(): data(){ }
	modular_unfixed_base(const double &x){ data = normalize(llround(x)); }
	modular_unfixed_base(const long double &x){ data = normalize(llround(x)); }
	template<class T, typename enable_if<is_integral<T>::value>::type* = nullptr> modular_unfixed_base(const T &x){ data = normalize(x); }
	template<class T, typename enable_if<is_integral<T>::value>::type* = nullptr> static unsigned int normalize(const T &x){
		assert(inverse_mod);
		int sign = x >= 0 ? 1 : -1;
		unsigned int v = _mod <= sign * x ? sign * x - ((unsigned __int128)(sign * x) * inverse_mod >> 64) * _mod : sign * x;
		if(v >= _mod) v += _mod;
		if(sign == -1 && v) v = _mod - v;
		return v;
	}
	const unsigned int &operator()() const{ return data; }
	template<class T> operator T() const{ return data; }
	modular_unfixed_base &operator+=(const modular_unfixed_base &otr){ if((data += otr.data) >= _mod) data -= _mod; return *this; }
	modular_unfixed_base &operator-=(const modular_unfixed_base &otr){ if((data += _mod - otr.data) >= _mod) data -= _mod; return *this; }
	template<class T, typename enable_if<is_integral<T>::value>::type* = nullptr> modular_unfixed_base &operator+=(const T &otr){ return *this += modular_unfixed_base(otr); }
	template<class T, typename enable_if<is_integral<T>::value>::type* = nullptr> modular_unfixed_base &operator-=(const T &otr){ return *this -= modular_unfixed_base(otr); }
	modular_unfixed_base &operator++(){ return *this += 1; }
	modular_unfixed_base &operator--(){ return *this += _mod - 1; }
	modular_unfixed_base operator++(int){ modular_unfixed_base result(*this); *this += 1; return result; }
	modular_unfixed_base operator--(int){ modular_unfixed_base result(*this); *this += _mod - 1; return result; }
	modular_unfixed_base operator-() const{ return modular_unfixed_base(_mod - data); }
	modular_unfixed_base &operator*=(const modular_unfixed_base &rhs){
		data = normalize((unsigned long long)data * rhs.data);
		return *this;
	}
	template<class T, typename enable_if<is_integral<T>::value>::type* = nullptr>
	modular_unfixed_base &operator^=(T e){
		if(e < 0) *this = 1 / *this, e = -e;
		modular_unfixed_base res = 1;
		for(; e; *this *= *this, e >>= 1) if(e & 1) res *= *this;
		return *this = res;
	}
	template<class T, typename enable_if<is_integral<T>::value>::type* = nullptr>
	modular_unfixed_base operator^(T e) const{
		return modular_unfixed_base(*this) ^= e;
	}
	modular_unfixed_base &operator/=(const modular_unfixed_base &otr){
		int a = otr.data, m = _mod, u = 0, v = 1;
		if(a < _INV.size()) return *this *= _INV[a];
		while(a){
			int t = m / a;
			m -= t * a; swap(a, m);
			u -= t * v; swap(u, v);
		}
		assert(m == 1);
		return *this *= u;
	}
	unsigned int data;
};
template<int id> unsigned int modular_unfixed_base<id>::_mod;
template<int id> unsigned long long modular_unfixed_base<id>::inverse_mod;
template<int id> vector<modular_unfixed_base<id>> modular_unfixed_base<id>::_INV;
template<int id> modular_unfixed_base<id> modular_unfixed_base<id>::_primitive_root;
template<int id> bool operator==(const modular_unfixed_base<id> &lhs, const modular_unfixed_base<id> &rhs){ return lhs.data == rhs.data; }
template<int id, class T, typename enable_if<is_integral<T>::value>::type* = nullptr> bool operator==(const modular_unfixed_base<id> &lhs, T rhs){ return lhs == modular_unfixed_base<id>(rhs); }
template<int id, class T, typename enable_if<is_integral<T>::value>::type* = nullptr> bool operator==(T lhs, const modular_unfixed_base<id> &rhs){ return modular_unfixed_base<id>(lhs) == rhs; }
template<int id> bool operator!=(const modular_unfixed_base<id> &lhs, const modular_unfixed_base<id> &rhs){ return !(lhs == rhs); }
template<int id, class T, typename enable_if<is_integral<T>::value>::type* = nullptr> bool operator!=(const modular_unfixed_base<id> &lhs, T rhs){ return !(lhs == rhs); }
template<int id, class T, typename enable_if<is_integral<T>::value>::type* = nullptr> bool operator!=(T lhs, const modular_unfixed_base<id> &rhs){ return !(lhs == rhs); }
template<int id> bool operator<(const modular_unfixed_base<id> &lhs, const modular_unfixed_base<id> &rhs){ return lhs.data < rhs.data; }
template<int id> bool operator>(const modular_unfixed_base<id> &lhs, const modular_unfixed_base<id> &rhs){ return lhs.data > rhs.data; }
template<int id> bool operator<=(const modular_unfixed_base<id> &lhs, const modular_unfixed_base<id> &rhs){ return lhs.data <= rhs.data; }
template<int id> bool operator>=(const modular_unfixed_base<id> &lhs, const modular_unfixed_base<id> &rhs){ return lhs.data >= rhs.data; }
template<int id> modular_unfixed_base<id> operator+(const modular_unfixed_base<id> &lhs, const modular_unfixed_base<id> &rhs){ return modular_unfixed_base<id>(lhs) += rhs; }
template<int id, class T, typename enable_if<is_integral<T>::value>::type* = nullptr> modular_unfixed_base<id> operator+(const modular_unfixed_base<id> &lhs, T rhs){ return modular_unfixed_base<id>(lhs) += rhs; }
template<int id, class T, typename enable_if<is_integral<T>::value>::type* = nullptr> modular_unfixed_base<id> operator+(T lhs, const modular_unfixed_base<id> &rhs){ return modular_unfixed_base<id>(lhs) += rhs; }
template<int id> modular_unfixed_base<id> operator-(const modular_unfixed_base<id> &lhs, const modular_unfixed_base<id> &rhs){ return modular_unfixed_base<id>(lhs) -= rhs; }
template<int id, class T, typename enable_if<is_integral<T>::value>::type* = nullptr> modular_unfixed_base<id> operator-(const modular_unfixed_base<id> &lhs, T rhs){ return modular_unfixed_base<id>(lhs) -= rhs; }
template<int id, class T, typename enable_if<is_integral<T>::value>::type* = nullptr> modular_unfixed_base<id> operator-(T lhs, const modular_unfixed_base<id> &rhs){ return modular_unfixed_base<id>(lhs) -= rhs; }
template<int id> modular_unfixed_base<id> operator*(const modular_unfixed_base<id> &lhs, const modular_unfixed_base<id> &rhs){ return modular_unfixed_base<id>(lhs) *= rhs; }
template<int id, class T, typename enable_if<is_integral<T>::value>::type* = nullptr> modular_unfixed_base<id> operator*(const modular_unfixed_base<id> &lhs, T rhs){ return modular_unfixed_base<id>(lhs) *= rhs; }
template<int id, class T, typename enable_if<is_integral<T>::value>::type* = nullptr> modular_unfixed_base<id> operator*(T lhs, const modular_unfixed_base<id> &rhs){ return modular_unfixed_base<id>(lhs) *= rhs; }
template<int id> modular_unfixed_base<id> operator/(const modular_unfixed_base<id> &lhs, const modular_unfixed_base<id> &rhs) { return modular_unfixed_base<id>(lhs) /= rhs; }
template<int id, class T, typename enable_if<is_integral<T>::value>::type* = nullptr> modular_unfixed_base<id> operator/(const modular_unfixed_base<id> &lhs, T rhs) { return modular_unfixed_base<id>(lhs) /= rhs; }
template<int id, class T, typename enable_if<is_integral<T>::value>::type* = nullptr> modular_unfixed_base<id> operator/(T lhs, const modular_unfixed_base<id> &rhs) { return modular_unfixed_base<id>(lhs) /= rhs; }
template<int id> istream &operator>>(istream &in, modular_unfixed_base<id> &number){
	long long x;
	in >> x;
	number.data = modular_unfixed_base<id>::normalize(x);
	return in;
}
// #define _PRINT_AS_FRACTION
template<int id> ostream &operator<<(ostream &out, const modular_unfixed_base<id> &number){
#ifdef LOCAL
#ifdef _PRINT_AS_FRACTION
	out << number();
	cerr << "(";
	for(auto d = 1; ; ++ d){
		if((number * d).data <= 1000000){
			cerr << (number * d).data << "/" << d;
			break;
		}
		else if((-number * d).data <= 1000000){
			cerr << "-" << (-number * d).data << "/" << d;
			break;
		}
	}
	cerr << ")";
	return out;
#else
	return out << number();
#endif
#else
	return out << number();
#endif
}
#undef _PRINT_AS_FRACTION

using modular = modular_unfixed_base<0>;
