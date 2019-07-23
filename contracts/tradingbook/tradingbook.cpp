#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT tradingbook: public contract {
    public:
    using contract::contract;

    ACTION dummy() {}

    [[eosio::on_notify("eosio.token::transfer")]]
    void ontransfer(name from, name to, asset quantity, std::string memo) {
        if(from == get_self()) {
        Receive forReceiver(get_self(), to.value);
        forReceiver.emplace(get_self(), [&](auto& row) {
            row.mykey = forReceiver.available_primary_key();
            row.user = to;
            row.balance = quantity;
            });
        } else {
        Send forSender(get_self(), from.value);
        forSender.emplace(get_self(), [&](auto& row) {
            row.mykey = forSender.available_primary_key();
            row.user = from;
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

    typedef multi_index<"sendbooks"_n, tradingbook_struct> Send;
    typedef multi_index<"receivebooks"_n, tradingbook_struct> Receive;

};