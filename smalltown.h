#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <iostream>
#include <vector>
#include <tuple>
#include "citizen.h"
#include "monster.h"

template <typename M, typename U, U t0, U t1, typename... C>
class SmallTown {

private:
	
	//const expr count limit for biggest F_n in U
	constexpr static size_t gen_fib_limit() {
		U a = 0, b = 1, c = 1;
		for (size_t i = 1; ; ++i) {
			c = a + b;
			if (c >= t1 || c < 0) return i; //TODO check
			a = b;
			b = c;
			//std::cerr << c << "\n\n\n";
		}
	}
	
	constexpr static size_t fib_limit = gen_fib_limit(); 

	// type U of time, N - max fib index to count, FN - curr fib index, 
	// F1, F2 - last two fibs, Values - variadic table of counted fibs
	/*template<size_t FN, U F1, U F2, U... Values>
		//typename = std::enable_if<(FN == N)>>
	constexpr static std::array<U, fib_limit> gen_fibs() {
		if (FN == fib_limit) return U(Values)...;//std::array<U, fib_limit>(Values...);
		return gen_fibs<std::min(fib_limit, FN + 1), F1 + F2, F1, Values..., F2>();
	}*/
	
	
	//template<size_t FN, U F1, U F2, U... Values>
		//typename = std::enable_if<(FN == N)>>
	/*constexpr static void gen_fibs() {
		std::vector<U> temp;
		U a = 0, b = 1, c = 1;
		for (size_t i = 1; ; ++i) {
			c = a + b;
			if (c >= t1 || c < a) return;// temp;
			fibs.push_back(c);
			a = b;
			b = c;
			//std::cerr << c << "\n\n\n";
		}
		//if (FN == fib_limit) return U(Values)...;//std::array<U, fib_limit>(Values...);
		//return gen_fibs<std::min(fib_limit, FN + 1), F1 + F2, F1, Values..., F2>();
	}
	
	constexpr static std::vector<U>fibs{};// = gen_fibs();*/
	
	
	//template<typename U, int N, int FN, U F1, U F2, U... Values,
	//	typename = std::enable_if<(FN < N)>>
	//constexpr std::array gen_fibs() {
	//	//if (N == FN) 
	//	return gen_fibs<U, N, FN + 1, F1 + F2, F1, ...Values, F2>(); //TODO check folding f2 into values of next stage
	//}

	//constexpr static std::vector<U>fibs = gen_fibs();
	
	/*
	template<size_t FN, typename U, U F1, U F2, U ... args>
	constexpr struct array_{
		if (FN < fib_limit)
			array
			constexpr static U data[sizeof...(args)];
	};

	template<typename U, U ... args>
	U array_<fib_limit, U, 1, 1, args...>::data[fib_limit] = {U(args)...};
	*/
	
	
	
	
	
	/*
	template<size_t FN, U F1, U F2, U... temp>
	constexpr static std::array<U, fib_limit> gen_fibs() {
		if (FN == fib_limit) return {temp...};
		//fibs[FN] = F1;//{Values...};//std::array<U, fib_limit>(Values...); //TODO
		return gen_fibs<std::min(FN + 1, fib_limit), F1 + F2, F1, temp..., F2>();
	}
	
	
	constexpr static std::array<U, fib_limit>fibs = gen_fibs<1, 1, 1>();// = {1, 2};// = std::array::fill(0);
	*/
	
	
	M monster;
	std::tuple<C...> citizens;
	U time = t0;                                   //we've got: t0, t1
	static const size_t citizensCount = sizeof...(C);
	size_t livingCitizens = citizensCount;
	
	bool ifAttack() const {
		for(size_t i = 0; i < fib_limit; i++) {
			//if (time == fibs[i]) TODO uncomment
			return true;
		}
		return false;
	}
	
	//template <size_t N, size_t citizensCount, typename = std::enable_if<(N == citizensCount)>>
	//void attackAll(M& monster) {}
	
	//template <size_t i>
	//void attackAll();
	
	template <size_t i>//, typename = std::enable_if<(i < citizensCount)>>
	void attackAll() {
		auto& cit = std::get<i>(citizens);
		if (cit.getHealth() > 0) {
			attack(monster, cit);
			if (cit.getHealth() == 0)
				livingCitizens--;
		}
		if(i < citizensCount - 1) {
			attackAll<std::min(i + 1, citizensCount - 1)>();
		}
	}

public:
	SmallTown(M monst, C... cits) 
	: monster(monst)
	, citizens(cits...) {
	    static_assert(std::is_arithmetic<U>::value, 
	                  "U is not an arithmetic type.");
		static_assert(t1 >= t0 && t0 >= 0, "Wrong init times");
		//static_assert(livingCitizens >= 0, "No living citizens"); so what?
	}
	
	//	std::tuple<std::string, T, size_t>
	auto getStatus() const {
		return std::make_tuple(
			monster.getTypeName(),
			monster.getHealth(),
			livingCitizens);
	}
	
	void tick(U timeStep) {
		/*for(int i = 0; i < fib_limit; i++) {
			//std::cout << fibs[i] << ' ';
		}*/
		if (livingCitizens == 0) {
			if (monster.getHealth() > 0) {
				std::cout << "MONSTER WON\n";
			} else {
				std::cout << "DRAW\n";
			}
		} else if (monster.getHealth() == 0) {
			std::cout << "CITIZENS WON\n";
		}
		if (ifAttack()) attackAll<0>();
		time += timeStep;
		time %= t1;
	}
};

#endif //SMALLTOWN_H