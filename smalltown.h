//Batosz Burny, Maciej Gontar
#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <iostream>
#include <array>
#include <tuple>
#include "citizen.h"
#include "monster.h"

/* template for SmallTown, with given type of monster,
 * type of time, starting time, number of hours in a day
 * and list of types of citizens
 */
template<typename M, typename U, U t0, U t1, typename... C>
class SmallTown {

private:
    //CONSTEXPR class components
    //count limit for biggest distinct fibonacci number no bigger than t1 ({1} for t1 = 0)
    constexpr static size_t genFibLimit() {
        U a = 1, b = 1;
        for (size_t i = 1;; ++i) {
            U c = a + b;
            if (c > t1 || c < a) return i;
            a = b;
            b = c;
        }
    }
    
    constexpr static size_t fibLimit = genFibLimit();
    
    //generating fibonacci numbers up to (fib_limit)-th distinct one
    template<size_t N, U F1, U F2, U... temp>
    constexpr static typename std::enable_if_t<(N == fibLimit),
            const std::array<U, fibLimit>> genFibs() {
        return std::array<U, fibLimit>{{temp..., F1}};
    }
    
    template<size_t N, U F1, U F2, U... temp>
    constexpr static typename std::enable_if_t<(N != fibLimit),
            std::array<U, fibLimit>> genFibs() {
        return genFibs<N + 1, F1 + F2, F1, temp..., F1>();
    }
    
    //an alias for comfort of usage
    constexpr static const std::array<U, fibLimit> &fibs = genFibs<1, 1, 1>();
    
    
    //normal class components
    M monster;
    std::tuple<C...> citizens;
    U time = t0;
    static const size_t citizensCount = sizeof...(C);
    size_t livingCitizens = citizensCount;
    
    //helper function for determining if monster's attack should occur
    bool ifAttack() const {
        return std::binary_search(fibs.begin(), fibs.end(), time);
    }
    
    //template of helper function to attack all (alive) citizens in the town
    template<size_t i>
    void attackAll() {
        auto &cit = std::get<i>(citizens);
        if (cit.getHealth() > 0) {
            attack(monster, cit);
            if (cit.getHealth() == 0)
                livingCitizens--;
        }
        if (i < citizensCount - 1) {
            attackAll<std::min(i + 1, citizensCount - 1)>();
        }
    }

public:
    //constructor with asserts
    SmallTown(M monst, C... cits)
            : monster(monst), citizens(cits...) {
        static_assert(std::is_arithmetic<U>::value,
                      "U is not an arithmetic type.");
        static_assert(t1 >= t0 && t0 >= 0, "Wrong init times");
    }
    
    auto getStatus() const {
        return std::make_tuple(
                std::string(typeid(monster).name()),
                monster.getHealth(),
                livingCitizens
        );
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
        
        if (ifAttack() && monster.getHealth() > 0) attackAll<0>();
        time += timeStep;
        //special case for an overflow of unsigned types (to avoid dividing by 0)
        if (t1 + 1 > t1) {
            time %= (t1 + 1);
        }
    }
};

#endif //SMALLTOWN_H
