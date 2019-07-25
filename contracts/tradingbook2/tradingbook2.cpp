#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT tradingbook2: public contract {
    public:
    using contract::contract;

    ACTION erase(uint64_t scope) {
            require_auth(get_self());

            Sendscope senderase(get_self(), get_self().value);
            auto sends = senderase.begin();
            while(sends != senderase.end()){
                Send allsend(get_self(), sends->scope); // 영역을 찾는다. 
                auto allsends = allsend.begin();

                while(allsends != allsend.end()){
                    allsend.erase(allsends); // send 또 recive의 값을 지운다.
                }

                senderase.erase(sends); //scope value : scope table값을 지운다
            };
                
            
            print("erase success");


            Recivescope reciverase(get_self(), get_self().value);
            auto itr = reciverase.begin();
            while(itr != reciverase.end()){
                Receive allrecive(get_self(), itr->scope);
                auto recives = allrecive.begin();
                while(recives != allrecive.end()){
                    allrecive.erase(recives);
                };
                reciverase.erase(itr);
            };
            
            print("erase success");
        }

        
// user.value 값을 변경하게다는 말
    [[eosio::on_notify("eosio.token::transfer")]]
    void ontransfer(name from, name to, asset quantity, std::string memo) {
        if(from == get_self()) {
        Recivescope forSaveScope(get_self(), get_self().value);
        auto itr = forSaveScope.find(to value);


        Receive forReceiver(get_self(), from.value);  //find는 primary에 값을 비교하여 가져온다.
        forReceiver.emplace(get_self(), [&](auto& row) {
            row.mykey = forReceiver.available_primary_key();
            row.user = from;
            row.balance = quantity;
            });
        Recivescope forSendscope(get_self(), get_self().value);
        forSendscope.emplace(get_self(), [&](auto& row){
            row.scope = from.value;
        });
        } else {
        Send forSender(get_self(), to.value);
        forSender.emplace(get_self(), [&](auto& row) {    
            row.mykey = forSender.available_primary_key();
            row.user = to;
            row.balance = quantity;
            });

        Sendscope forSendscope(get_self(), get_self().value);
        forSendscope.emplace(get_self(), [&](auto& row){
            row.scope = to.value;   // scope 마다 계정을 하나 할당 받는 느낌 
        });
        }
    }

    private:
    TABLE tradingbook_struct {
        uint64_t mykey;
        name user;
        asset balance;

        uint64_t primary_key() const { return mykey; }
        };

    TABLE Scopebook_struct{
        uint64_t scope;  // 저장 될때  숫자로 들어온다
        
        uint64_t primary_key() const {return scope;}
    };

    typedef multi_index<"sendbooks"_n, tradingbook_struct> Send;
    typedef multi_index<"receivebooks"_n, tradingbook_struct> Receive;

    typedef multi_index<"sendscope"_n, Scopebook_struct> Sendscope;
    typedef multi_index<"receivescope"_n, Scopebook_struct> Recivescope;
    
};