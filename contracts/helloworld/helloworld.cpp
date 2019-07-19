#include <eosiolib/eosio.hpp>

using namespace eosio;

CONTRACT helloworld: public contract {
    public:
        using contract::contract;
        ACTION hi(name user){
            //require_auth(get_self()||"bschild12345"_n);
            //require_auth("rowsrowsrows"_n);
            const rum = get_self() || has_auth("bschild12345"_n);
            check(rum, "you aint a contract");
            print("hello," , user);
            }
    private:
};