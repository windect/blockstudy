#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT geteos: public contract {
    public:
        using contract::contract;

        ACTION getbalance( name user, asset token ) {    //user로 들어온 사용자의 EOS잔액 출력하는 action, asset token을 입력하고 아래 symbol sym에서 받아온다.
            accounts mytoken( "eosio.token"_n, user.value );   //
            symbol sym = token.symbol;
            auto itr = mytoken.require_find( sym.code().raw(), "no balance" );

            print( itr->balance );
        }

    private:
        TABLE account {

            asset    balance;   // 토큰타입


            uint64_t primary_key()const { return balance.symbol.code().raw(); }
        };

        TABLE currency_stats {
            asset    supply;
            asset    max_supply;
            name     issuer;

            uint64_t primary_key()const { return supply.symbol.code().raw(); }
        };

        typedef multi_index< "accounts"_n, account > accounts;  //accounts로 사용하겠다는 의미
        typedef multi_index< "stat"_n, currency_stats > stats;  //stats으로 사용하겠다는 의미
};

