#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <iostream>
#include <array>

#include "citizen.h"
#include "monster.h"

#include <tuple>

//constexpr std::array<int, 47> fibs = produce_fibs<47>();// = produce_fibs(1, 2, 2, 47, {});

template <typename M, typename U, U t0, U t1, typename... C>
class SmallTown {

private:
	
	//TODO const expr count limit for biggest F_n in U
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
	/*template<size_t Limit, size_t FN, U F1, U F2, U... Values>
		//typename = std::enable_if<(FN == N)>>
	constexpr static std::array<U, Limit> gen_fibs() {
		if (FN == Limit) return {Values...};//std::array<U, fib_limit>(Values...); //TODO
		return gen_fibs<U, Limit, FN + 1, F1 + F2, F1, Values..., F2>();
	}

	//template<typename U, int N, int FN, U F1, U F2, U... Values,
	//	typename = std::enable_if<(FN < N)>>
	//constexpr std::array gen_fibs() {
	//	//if (N == FN) 
	//	return gen_fibs<U, N, FN + 1, F1 + F2, F1, ...Values, F2>(); //TODO check folding f2 into values of next stage
	//}

	//template <typename U>
	constexpr static std::array<U, fib_limit>fibs = 
		gen_fibs<fib_limit, (size_t)1, 1, 1>();
	*/
	constexpr static std::array<U, fib_limit>fibs{};
	
	template<size_t Limit, size_t FN, U F1, U F2>
		//typename = std::enable_if<(FN == N)>>
	constexpr static void gen_fibs() {
		if (FN == Limit) return;
		fibs[FN] = F1;//{Values...};//std::array<U, fib_limit>(Values...); //TODO
		gen_fibs<U, Limit, FN + 1, F1 + F2, F1>();
	}
	
	M monster;
	std::tuple<C...> citizens;
	U time = t0; 	//we've got: t0, t1
	static const size_t citizensCount = sizeof...(C);
	size_t livingCitizens = citizensCount;
	
	bool ifAttack() {
		for(size_t i = 0; i < fib_limit; i++) {
			if (time == fibs[i]) return true;
		}
		return false;
	}
	
	//template <size_t N, size_t citizensCount, typename = std::enable_if<(N == citizensCount)>>
	//void attackAll(M& monster) {}
	
	//template <size_t i>
	//void attackAll();
	
	template <size_t i>//, typename = std::enable_if<(i < citizensCount)>>
	void attackAll() {
		if(i < citizensCount) {
			if ((std::get<i>(citizens)).getHealth() > 0) {
				attack(monster, std::get<i>(citizens));//TODO check
				if ((std::get<i>(citizens)).getHealth() == 0)
					livingCitizens--;
			}
			attackAll<i + 1>();
		}
	}

public:
	SmallTown(M monst, C... cits) 
	: monster(monst)
	, citizens(cits...)
	//, allCitizens(std::tuple_size<decltype(cits)>::value)
	//, allCitizens() //TODO check
	//, livingCitizens(allCitizens) 
	{
	    static_assert(std::is_arithmetic<U>::value, 
	                  "U is not an arithmetic type.");
		static_assert(t1 >= t0 && t0 >= 0, "Wrong init times");
		//static_assert(livingCitizens >= 0, "No living citizens"); so what?
	}
	
	//	std::tuple<std::string, T, size_t>
	auto getStatus() const {
		return std::forward_as_tuple( //TODO make_tuple, not auto return type
			monster.getTypeName(), //TODO check result
			monster.getHealth(),
			livingCitizens);
	}
	
	void tick(U timeStep) {
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