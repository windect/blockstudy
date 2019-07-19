const { Api, JsonRpc, RpcError } = require('eosjs');
const { JsSignatureProvider } = require('eosjs/dist/eosjs-jssig');
const { PrivateKey, PublicKey, Signature, Aes, key_utils, config } = require('eosjs-ecc');
const { TextEncoder, TextDecoder } = require('util');
const fetch = require('node-fetch');
require('dotenv').config({ path: '.env' });

let privateKeys = ['5K14Vw6UhSGKFRpvM2FSqGztKNxxs9upHB6DocM8hjJEHM8Vpir'];
const rpc = new JsonRpc('http://127.0.0.1:8011', { fetch });
const signatureProvider = new JsSignatureProvider(privateKeys);
const api = new Api({ rpc, signatureProvider, textDecoder: new TextDecoder(), textEncoder: new TextEncoder() });

async function main() {
    const result2 = await rpc.get_currency_balace('eosio_token', 'towstowstows','EOS');
    console.log(result2);
    const result = await api.transact({
        actions: [{
          account: 'eosio.token',
          name: 'transfer',
          authorization: [{
            actor: 'towstowstows',
            permission: 'active',
          }],
          data: {
            from: 'towstowstows',
            to: 'baekseokinit',
            quantity: '1.0000 EOS',
            memo: '1.0000 EOS',
          },
        }]
      }, {
        blocksBehind: 3,
        expireSeconds: 30,
      });
    

}

main();

   /*
    const result = await api.transact({
        actions: [{
          account: 'eosio.token',
          name: 'trasfer',
          authorization: [{
            actor: 'towstowstows',
            permission: 'active',
          }],
          data: {
            payer: 'useraaaaaaaa',
            receiver: 'useraaaaaaaa',
            bytes: 8192,
          },
        }]

    const result = await api.transact({
  actions: [{
    account: 'eosio',
    name: 'delegatebw',
    authorization: [{
      actor: 'useraaaaaaaa',
      permission: 'active',
    }],
    data: {
      from: 'useraaaaaaaa',
      receiver: 'useraaaaaaaa',
      stake_net_quantity: '1.0000 SYS',
      stake_cpu_quantity: '1.0000 SYS',
      transfer: false,
    }
  }] 
}, {
  blocksBehind: 3,
  expireSeconds: 30,
});
      }, {
        blocksBehind: 3,
        expireSeconds: 30,
      });
      */