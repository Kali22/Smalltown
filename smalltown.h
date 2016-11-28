#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <iostream>
#include <utility>

#include "citizen.h"
#include "monster.h"

constexpr std::array<int, 47> fibs = produce_fibs<47>();// = produce_fibs(1, 2, 2, 47, {});

template <class U>
constexpr int gen_fib_limit(U value) {
	int a = 0, b = 1;
	for (int i = 1; i <= n; ++i) {
		c = a + b;
		a = b;
		b = c;
	}
	return c;
}



template<typename U, int N, int FN, U F1, U F2, U... Values,
	typename = std::enable_if<FU == N>>
constexpr std::array gen_fibs() {
	return std::array(Values);
}

template<typename U, int N, int FN, U F1, U F2, U... Values,
	typename = std::enable_if<FN < N>>
constexpr std::array gen_fibs() {
	return gen_fibs<U, N, FN + 1, F1 + F2, F1, F2 ...Values>();
}


template<unsigned... Values> class fibs{};

template<unsigned int N, unsigned int... Is>
class gen_fibs : gen_fibs<N-1, Is...>{};

template<unsigned... Is>
class gen_fibs<0, Is...> : seq<Is...>{};

template<unsigned... Is>
constexpr Table MagicFunction(seq<Is>){
  return {{ whichCategory(Is)... }};
}

constexpr Table MagicFunction(){
  return MagicFunction(gen_seq<128>{});
}








template <int, typename ...elems>
constexpr produce_fibs<47, ...elems> (

template<int N> 
class produce_fibs { enum { value = produce_fibs<N-1>::value + produce_fibs<N-2>::value }; };
template<>
class produce_fibs<1> { enum { value = 1 }; }; 


template <typename M, typename U, U t0, U t1, typename... C>
class Smalltown {

private:
	M monster;
	std::tuple<typename... C> citizens;
	U time = t0; //mamy(?): t0, t1
	size_t allCitizens, livingCitizens;
	
	bool if_attack() {
		for (auto fib : fibs) {
			if (time == fib) return true;
		}
		return false;
	}
	
	template <int N, typename = typename std::enable_if<N = allCitizens> >
	void attackAll(Monster& m) {}
	
	template <int N, typename = typename std::enable_if<N < allCitizens> >
	void attackAll(Monster& m) {
		Citizen& C = std::get<N>(citizens); //TODO check
		if(C.getHealth() > 0) (m, C);
		attackAll<N + 1>(m);
	}
	

public:
	Smalltown(M monst, C... cits) 
	: monster(monst)
	, citizens(std::make_tuple(cits))
	//, allCitizens(std::tuple_size<decltype(cits)>::value)
	, allCitizens(sizeof...(C)) //TODO check
	, livingCitizens(allCitizens) {
	    static_assert(std::is_arithmetic<U>::value, 
	                  "U is not an arithmetic type.");
		static_assert(t1 >= t0 && t0 >= 0, "Wrong init times");
		//static_assert(livingCitizens >= 0, "No living citizens"); so what?
	}
	
	std::tuple<string, T, size_t> getStatus() {
		return std::make_tuple(
			monster.getTypeName(); //TODO check result
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
		if (if_attack()) attackAll<0>(monster);
		time += timeStep;
		time %= t1;
	}
};

#endif //SMALLTOWN_H