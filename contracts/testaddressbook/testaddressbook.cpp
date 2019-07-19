#include <eosio/eosio.hpp>

using namespace eosio;

CONTRACT testaddressbook: public contract{
    public: 
        using contract::contract;

       ACTION findage(uint64_t count){

            address_index addresses(get_self(), get_self().value);
            auto forSecondary = addresses.get_index<"bycount"_n>();

            auto itr = forSecondary.require_find(count, "no count");

            if(itr != forSecondary.end(){

            }

            print(itr->user, " ", itr->count);

        }
        /* 
       ACTION insert(name user){
            
            require_auth(user);  
            check(user == get_self(),"?");  //require_auth(get_self());

            address_index forInsert(get_self(), get_self().value);
            auto itr=forInsert.find(user.value);

            check(itr == forInsert.end(), "already exists"); 

            forInsert.emplace(get_self(), [&](auto& row){ 
                row.user = user;
                row.count++;
            }); 

            print("insert success");
        }
        */
        ACTION upsert(name user) {
            require_auth(get_self());

            address_index forUpsert(get_self(), get_self().value);

            auto itr = forUpsert.find(user.value);

            if(itr==forUpsert.end())
            {
                forUpsert.emplace(user,[&](auto& row){
                row.user = user;
                row.count=1;
            });
            }
            else{
                forUpsert.modify(itr,user,[&](auto& row){
                    
                    row.count++;
                });
            }
            print("upsert success");

        }

        ACTION eraseall() {
            require_auth(get_self());

            counts forEraseall(get_self(),get_self().value);
            // address_index forEraseall(get_self(), get_self().value); 
            //auto itr = forEraseall.require_find(get_self().value,"no account"); 
            auto itr = forEraseall.begin();
            while( itr != forEraseAll.end() ) {
                itr = forEraseAll.erase( itr );
            }

            /* 
            forEraseall.erase(itr);

            forEraseall.modify(get_self(), [&](auto& row){ 
                row.count++;
            }); 
            print("eraseall success");
            */
        }

         ACTION remove(name user) {
            require_auth(user);
            check(user == get_self(),"?");

            address_index forRemove(get_self(), get_self().value); 
            auto itr = forRemove.require_find(user.value,"no account"); 
            forRemove.erase(itr);
            print("erase success");
        }

    private:
        struct[[eosio::table]] allowance{ 
            name user;
            uint64_t count;

            uint64_t primary_key() const {return user.value;}
            uint64_t by_count() const {return count;}
        };

        typedef multi_index < "allowances"_n, allowance, indexed_by <"bycount"_n, const_mem_fun < allowance ,uint64_t, &allowance::by_count>> > address_index;
};


        /* ACTION hi(name user){
            require_auth(user);
            address_index forHi(get_self(), getself().value);
            auto itr = forHi.find(user.value,"Push!!");
            if( itr == forHi.end() ){
            print("Get OUT!!!");
        }else {
            print("Hello, ", user);
        }
        }
        */

       /*   ACTION insert(name user){
            
            require_auth(user);  
            check(user == get_self(),"?");  //require_auth(get_self());

            address_index forInsert(get_self(), get_self().value);
            auto itr=forInsert.find(user.value);

            check(itr == forInsert.end(), "already exists"); 

            forInsert.emplace(get_self(), [&](auto& row){ 
                row.user = user;
            }); 

            print("insert success");
        }
        */

