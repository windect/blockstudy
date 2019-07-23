#include <eosio/eosio.hpp>

using namespace eosio;


CONTRACT addressbook: public contract{
    public: 
        using contract::contract;

        ACTION findage(uint64_t age){
            address_index addresses(get_self(), get_self().value);
            auto forSecondary = addresses.get_index<"byage"_n>();

            auto itr = forSecondary.require_find(age, "no age");

            print(itr->user, " ", itr->age);

        }
        
        ACTION upsert(name user, std::string first_name, std::string last_name, uint64_t age) {
            require_auth(user);

            address_index forUpsert(get_self(), get_self().value);

            auto itr = forUpsert.find(user.value);

            if(itr==forUpsert.end())
            {
                forUpsert.emplace(user,[&](auto& row){
                row.user=user;
                row.first_name=first_name;
                row.last_name=last_name;
                row.age = age;
            });
            send_summary(user, " successfully emplaced record to addressbook");
            }
            else{
                forUpsert.modify(itr,user,[&](auto& row){
                    row.user=user;
                    row.first_name=first_name;
                    row.last_name=last_name;
                    row.age = age;
                });
                send_summary(user, " successfully modified record to addressbook");
            }
            print("upsert success");

        }  //upset modify
        ACTION insert(name user, std::string first_name, std::string last_name, uint64_t age){
            require_auth(user);

            address_index forInsert(get_self(), get_self().value); //multi index 개체에 존재하는 지를 확인
            auto itr=forInsert.find(user.value);

            check(itr == forInsert.end(), "already exists"); //forinsert table에 마지막 scope를 가르키면
            forInsert.emplace(user, [&](auto& row){  //콜백 함수->[&](auto& row) / ram을 지불하는 부분? / 입력받은 값을 table에 담아주는 부분
                row.user = user;
                row.first_name= first_name;
                row.last_name = last_name;
                row.age = age;
            }); 
            send_summary(user, " successfully emplaced record to addressbook");
            print("insert success");
        }
        ACTION erase(name user) {
            require_auth(user);

            address_index forErase(get_self()/*contract*/, get_self().value /*scope*/);  //address_index 변수로 된 저장공간 / value=scope 구분없이 저장(scope 통일)
            auto itr = forErase.require_find(user.value,"no account"); //auto => 뒤에 자료형에 맞취서 선언해주는 명령어 / 정보를 찾는 역활
            forErase.erase(itr);
            print("erase success");
            send_summary(user, " successfully erase success record to addressbook");
        }
            
        [[eosio::action]]
        void notify(name user, std::string msg) {
        require_auth(get_self());
        require_recipient(user);
        }
        
    private:
        struct[[eosio::table]] person{ //한사람이 갖는 정보 
            name user;
            std::string first_name;
            std::string last_name;
            uint64_t age;

            uint64_t primary_key() const {return user.value;} //primary_key()멤버 함수 user 안에 변수
            uint64_t by_age() const {return age;}
        };

        void send_summary(name user, std::string message) {
            action(
            permission_level{get_self(),"active"_n},
            get_self(),
            "notify"_n,
            std::make_tuple(user, name{user}.to_string() + message)
            ).send();
        };
        typedef multi_index < "peopletwo"_n, person, indexed_by <"byage"_n, const_mem_fun < person,uint64_t, &person::by_age>> > address_index; // 내가 만든 table 주소명
    
};

