#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT testAutokey: public contract {
    public:
    using contract::contract;

    ACTION dummy() {}

    [[eosio::on_notify("eosio.token::transfer")]]
    void ontransfer(name from, name to, asset quantity, std::string memo) {
        if(from == get_self()) {
        Receive forReceiver(get_self(), get_self().value);
        forReceiver.emplace(get_self(), [&](auto& row) {
            row.mykey = forReceiver.available_primary_key();
            row.user = from;
            row.balance = quantity;
            });
        } else {
        Send forSender(get_self(), get_self().value);
        forSender.emplace(get_self(), [&](auto& row) {
            row.mykey = forSender.available_primary_key();
            row.user = to;
            row.balance = quantity;
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

    typedef multi_index<"senderbook"_n, tradingbook_struct> Send;
    typedef multi_index<"receiverbook"_n, tradingbook_struct> Receive;

};





/*
using namespace eosio;

CONTRACT testAutokey: public contract {
    public:
        using contract::contract;

        ACTION dummy() {
            require_auth(user)
            action(permisiion_level(user, "active"_n), //실행 권한 active_n
                //permission = 액션의 주체 설정
                permission_level(user,)
                //contract name,
                get_self()
                //action name,
                "dummytwo"_n
                //parmeters
                std::make_tuple(user)
            
            ).send();
        }
        // require_auth(get_self());
        [[eosio::on_notify("eosio.token::transfer")]]
        void aototransfer(name from, name to, asset quantity, std::string memo){
            if (from == get_self()){
                Reciver reciver(get_self(), get_self().value);
                    reciver.emplace(get_self(), [&](auto& row){
                        row.mykey = reciver.available_primary_key();
                        row.user = to;
                        row.balance = quantity;
                    });
            }else{
                Sender sender(get_self(), get_self().value);
                sender.emplace(get_self(), [&](auto& row) {
                row.mykey = sender.available_primary_key();
                row.user = from;
                row.balance = quantity;
                });
            }

        }

    private:
        TABLE example_struct {
        uint64_t mykey;
        name user;
        asset balance;

        uint64_t primary_key() const { return mykey; }
        };

        typedef multi_index<"senderbook"_n, example_struct> Sender;
        typedef multi_index<"receivebook"_n, example_struct> Reciver;
};

 */