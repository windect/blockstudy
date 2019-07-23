#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>

using namespace eosio;

CONTRACT onnotify2: public contract {
    public:
        using contract::contract;

        ACTION dummy() {}

        [[eosio::on_notify("eosio.token::transfer")]]
        void ontransfer(name from, name to, asset quantity, std::string memo) {
        if(from == get_self()) {
        outers myTable(get_self(), get_self().value);
        //auto itr = recive.find(to.value);
            if(myTable.begin() == myTable.end()) /* itr == myTable.end() */ {

                myTable.emplace(to, [&](auto& row) {
                    row.user = to;
                    row.balance = quantity;
                });

            } else {

                auto itr = myTable.begin();
                myTable.modify(itr, to, [&](auto& row) {
                    row.balance += quantity;
                });

            }
        }
            else if(to == get_self()) {
            iners myTable(get_self(), get_self().value);
            if(myTable.begin() == myTable.end()) {

                myTable.emplace(from, [&](auto& row) {
                    row.user = from;
                    row.balance = quantity;
                });

            } else {

                auto itr = myTable.begin();
                myTable.modify(itr, from, [&](auto& row) {
                row.balance += quantity;
                });

            }
            }
        } 

    private:
        TABLE outstructs {
        name user;
        asset balance;

        uint64_t primary_key() const { return user.value; } //balance가 아닌 이유는  balance simbol이 eos 1개 밖에 없기 때문이다.
        };

        

        typedef multi_index<"outes"_n, outstructs> outers;
        typedef multi_index<"iners"_n, outstructs> iners;
};

//정답
/*
#include
#include

using namespace eosio;

CONTRACT onnotify: public contract {
public:
using contract::contract;

ACTION dummy() {}

[[eosio::on_notify("eosio.token::transfer")]]
void ontransfer(name from, name to, asset quantity, std::string memo) {
if(from == get_self()) {
Receiver receiver(get_self(), get_self().value);
auto itr = receiver.find(to.value); //find를 사용하여 같은 값이 없으면 마지막 포인트를 항한다.
if(itr == receiver.end()) {
receiver.emplace(from, [&](auto& row) {
row.user = to;
row.balance = quantity;
});
} else {
receiver.modify(itr, from, [&](auto& row) {
row.balance += quantity;
});
}
} else {
Sender sender(get_self(), get_self().value);
auto itr = sender.find(from.value);
if(itr == sender.end()) {
sender.emplace(to, [&](auto& row) {
row.user = from;
row.balance = quantity;
});
} else {
sender.modify(itr, to, [&](auto& row) {
row.balance += quantity;
});
}
}
}
private:
TABLE trxlist {
name user;
asset balance;

uint64_t primary_key() const { return user.value; }
};

typedef multi_index<"eossender"_n, trxlist> Sender;
typedef multi_index<"eosreceiver"_n, trxlist> Receiver;
};

 */