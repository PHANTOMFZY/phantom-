/*
	phantom is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	phantom is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with phantom.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <utils/logger.h>
#include "general.h"
#include "private_key.h"
#include "key_store.h"
#include "argument.h"
#include <sstream>

namespace phantom {
	bool g_enable_ = true;
	bool g_ready_ = false;
	Argument::Argument() :
		help_modle_(false),
		drop_db_(false),
		peer_addr_(false),
		clear_peer_addresses_(false),
		clear_consensus_status_(false),
		log_dest_(-1),
		console_(false),
		create_hardfork_(false){}
	Argument::~Argument() {}

	bool Argument::Parse(int argc, char *argv[]) {
		if (argc > 1) {
			std::string s(argv[1]);
			if (s == "--console-with-cmd") {
				std::deque<std::vector<char>> params;
				std::string str_input;
                std::stringstream ss_input;
				static std::set<std::string> support_cmd = {
					"--sign-data",
					"--sign-data-with-keystore",
					"--check-address",
					"--check-keystore",
					"--check-signed-data",
					"--get-address",
					"--get-address-from-pubkey",
					"--get-privatekey-from-keystore",
					"--create-account",
					"--create-keystore",
					"--create-keystore-from-privatekey"
				};

				std::cout << "enter console command mode" << std::endl;
				
				do {
					params.clear();
					try
					{
						std::string input2str;
                        std::getline(std::cin, str_input);
                        ss_input.clear();
                        ss_input.str(str_input);
                        while (ss_input >> str_input) {
							utils::String::HexStringToBin(str_input, input2str);
							params.emplace_back(input2str.begin(), input2str.end());
							params.back().push_back('\0');
						}

						if (params.size() > 0 && support_cmd.find(std::string(params.front().data())) != support_cmd.end()) {
							// construct argc and argv
							std::vector<char*> new_argv;
							new_argv.emplace_back(argv[0]);
							for (auto& i : params)
								new_argv.push_back(i.data());

							Parse(new_argv.size(), new_argv.data());
						}
						else if (std::string(params.front().data()) == "exit") {
							break;
						}
						else {
							std::cout << "error" << std::endl;
						}
					}
					catch (std::exception e)
					{
						std::cout << "error" << std::endl;
					}
					
				} while (true);
				return true;
			}
			else if (s == "--dropdb") {
				drop_db_ = true;
			}
			else if (s == "--console") {
				console_ = true;
			}
			else if (s == "--hardware-address") {
				ShowHardwareAddress();
				return true;
			}
			else if (s == "--clear-peer-addresses") {
				clear_peer_addresses_ = true;
			}
			else if (s == "--aes-crypto") {
				if (argc > 2) {
					printf("%s\n", (utils::Aes::CryptoHex(argv[2], phantom::GetDataSecuretKey())).c_str());
				}
				else {
					printf("missing parameter, need crypto value \n");
				}
				return true;
			}
			//else if (s == "--aes-decrypt"  && argc > 2) {
			//	printf("%s\n", utils::Aes::HexDecrypto(argv[2], bubi::GetDataSecuretKey()).c_str());
			//	return true;
			//}
			else if (s == "--sm3" ) {
				if (argc > 2) {
					printf("%s\n", utils::String::BinToHexString(utils::Sm3::Crypto(argv[2])).c_str());
				}
				else {
					printf("missing parameter, need crypto value \n");
				}
				return true;
			}
			else if (s == "--sm3-hex" ) {
				if (argc > 2) {
					printf("%s\n", utils::String::BinToHexString(utils::Sm3::Crypto(utils::String::HexStringToBin(argv[2]))).c_str());
				}
				else {
					printf("missing parameter, need crypto value with hex format \n");
				}
				return true;
			}
			else if (s == "--peer-address") {
				ShowNodeId(argc, argv);
				return true;
			}
			else if (s == "--clear-consensus-status") {
				clear_consensus_status_ = true;
			}
			else if (s == "--create-hardfork") {
				create_hardfork_ = true;
			}
			else if (s == "--version") {
#ifdef SVNVERSION
				printf("%s,%u; " SVNVERSION "\n", General::PHANTOM_VERSION, General::LEDGER_VERSION);
#else
				printf("%s,%u\n", General::PHANTOM_VERSION, General::LEDGER_VERSION);
#endif 
				return true;
			}
			else if (s == "--help") {
				Usage();
				return true;
			}
			else if (s == "--check-signed-data" && argc > 4) {
				printf("%s\n", PublicKey::Verify(utils::String::HexStringToBin(argv[2]), utils::String::HexStringToBin(argv[3]), argv[4]) ? "true" : "false");
				return true;
			}
			else if (s == "--check-address" && argc > 2) {
				printf("%s\n", PublicKey::IsAddressValid(argv[2]) ? "ok" : "error");
				return true;
			}
			else if (s == "--sign-data" && argc > 3) {
				PrivateKey priv_key(argv[2]);
				std::string public_key = priv_key.GetEncPublicKey();
				std::string raw_data = utils::String::HexStringToBin(argv[3]);
				Json::Value result = Json::Value(Json::objectValue);
				
				result["data"] = argv[3];
				result["public_key"] = public_key;
				result["sign_data"] = utils::String::BinToHexString(priv_key.Sign(raw_data));
				printf("%s\n", result.toStyledString().c_str());
				return true;
			}
			else if (s == "--get-address" && argc > 2) {
				PrivateKey priv_key(argv[2]);
				std::string public_key = priv_key.GetEncPublicKey();
				std::string private_key = priv_key.GetEncPrivateKey();
				std::string public_address = priv_key.GetEncAddress();
				Json::Value result = Json::Value(Json::objectValue);

				result["public_key"] = public_key;
				result["private_key"] = private_key;
				result["private_key_aes"] = utils::Aes::CryptoHex(private_key, phantom::GetDataSecuretKey());
				result["address"] = public_address;
				result["public_key_raw"] = EncodePublicKey(priv_key.GetRawPublicKey());
				result["sign_type"] = GetSignTypeDesc(priv_key.GetSignType());
				printf("%s\n", result.toStyledString().c_str());
				return true;
			}

			else if (s == "--get-address-from-pubkey" && argc > 2) {
				phantom::PublicKey pub_key(argv[2]);
				std::string public_key_str = pub_key.GetEncPublicKey();
				std::string public_address = pub_key.GetEncAddress();
				Json::Value result = Json::Value(Json::objectValue);

				result["public_key"] = public_key_str;
				result["address"] = public_address;
				result["public_key_raw"] = EncodePublicKey(pub_key.GetRawPublicKey());
				result["sign_type"] = GetSignTypeDesc(pub_key.GetSignType());
				printf("%s\n", result.toStyledString().c_str());
				return true;
			}
			else if (s == "--create-account") {
				SignatureType type = SIGNTYPE_ED25519;
				if (argc > 2) {
					type = GetSignTypeByDesc(argv[2]);
					if (type == SIGNTYPE_NONE) {
						printf("parameter \"%s\" error, support ed25519 \n", argv[2]);
						return true;
					}
				}

				PrivateKey priv_key(type);
				if (!priv_key.IsValid()) {
					printf("Generate private key error");
					return true;
				}

				std::string public_key = priv_key.GetEncPublicKey();
				std::string private_key = priv_key.GetEncPrivateKey();
				std::string public_address = priv_key.GetEncAddress();

				LOG_TRACE("Creating account address:%s", public_address.c_str());
				Json::Value result = Json::Value(Json::objectValue);
				result["public_key"] = public_key;
				result["private_key"] = private_key;
				result["private_key_aes"] = utils::Aes::CryptoHex(private_key, phantom::GetDataSecuretKey());
				result["address"] = public_address;
				result["public_key_raw"] = EncodePublicKey(priv_key.GetRawPublicKey());
				result["sign_type"] = GetSignTypeDesc(priv_key.GetSignType());
				printf("%s\n", result.toStyledString().c_str());
				return true;
			}
			else if (s == "--create-keystore") {
				std::string password;
				if (argc <= 2) {
					password = utils::GetCinPassword("input the password:");
					std::cout << std::endl;
					if (password.empty()) {
						std::cout << "error, empty" << std::endl;
						return true;
					}
					std::string password1 = utils::GetCinPassword("input the password again:");
					std::cout << std::endl;
					if (password != password1) {
						std::cout << "error, not match" << std::endl;
						return true;
					}
				}
				else {
					password = argv[2];
				}

				KeyStore key_store;
				std::string new_priv_key;
				Json::Value key_store_json;
				bool ret = key_store.Generate(password, key_store_json, new_priv_key);
				if (ret) {
					printf("%s", key_store_json.toFastString().c_str());
				}
				else {
					printf("error");
				}
				return true;
			}

			else if (s == "--create-keystore-from-privatekey") {
				std::string password;
				if (argc <= 3) {
					password = utils::GetCinPassword("input the password:");
					std::cout << std::endl;
					if (password.empty()) {
						std::cout << "error, empty" << std::endl;
						return true;
					}
					std::string password1 = utils::GetCinPassword("input the password again:");
					std::cout << std::endl;
					if (password != password1) {
						std::cout << "error, not match" << std::endl;
						return true;
					}
				}
				else {
					password = argv[3];
				}

				PrivateKey key(argv[2]);
				if (!key.IsValid()) {
					printf("error, private key not valid");
					return true;
				} 

				KeyStore key_store;
				std::string new_priv_key = argv[2];
				Json::Value key_store_json;
				bool ret = key_store.Generate(password, key_store_json, new_priv_key);
				if (ret) {
					printf("%s", key_store_json.toFastString().c_str());
				}
				else {
					printf("error");
				}
				return true;
			}
			else if (s == "--check-keystore" && argc > 3) {
				KeyStore key_store;
				Json::Value key_store_json;
				key_store_json.fromString(argv[2]);
				std::string private_key;
				bool ret = key_store.From(key_store_json, argv[3], private_key);
				if (ret) {
					printf("ok");
				}
				else {
					printf("error");
				}
				return true;
			}
			else if (s == "--get-privatekey-from-keystore" && argc > 3) {
				KeyStore key_store;
				Json::Value key_store_json;
				key_store_json.fromString(argv[2]);
				std::string private_key;
				bool ret = key_store.From(key_store_json, argv[3], private_key);
				if (ret) {
					printf("%s", private_key.c_str());
				}
				else {
					printf("error");
				}
				return true;
			}
			else if (s == "--sign-data-with-keystore" && argc > 4) {
				KeyStore key_store;
				Json::Value key_store_json;
				key_store_json.fromString(argv[2]);
				std::string private_key;
				bool ret = key_store.From(key_store_json, argv[3], private_key);
				if (ret) {
					printf("%s", key_store_json.toFastString().c_str());
				}
				else {
					printf("error");
				}

				PrivateKey priv_key(private_key);
				std::string public_key = priv_key.GetEncPublicKey();
				std::string raw_data = utils::String::HexStringToBin(argv[4]);
				Json::Value result = Json::Value(Json::objectValue);

				result["data"] = argv[4];
				result["public_key"] = public_key;
				result["sign_data"] = utils::String::BinToHexString(priv_key.Sign(raw_data));
				printf("%s\n", result.toStyledString().c_str());
				return true;
			}
			else if (s == "--log-dest" && argc > 2) {
				utils::StringVector dests, levels;
				log_dest_ = utils::LOG_DEST_NONE;
				dests = utils::String::Strtok(argv[2], '+');

				for (auto &dest : dests) {
					std::string destitem = utils::String::ToUpper(dest);

					if (destitem == "ALL")         log_dest_ = utils::LOG_DEST_ALL;
					else if (destitem == "STDOUT") log_dest_ |= utils::LOG_DEST_OUT;
					else if (destitem == "STDERR") log_dest_ |= utils::LOG_DEST_ERR;
					else if (destitem == "FILE")   log_dest_ |= utils::LOG_DEST_FILE;
				}
				return false;
			}
			else if (s == "--dbtool") {
				printf("input database path:\n");
				std::string path;
				std::cin >> path;
				KeyValueDb* ledger_db_ = nullptr;
#ifdef WIN32
				ledger_db_ = new LevelDbDriver();
#else
				ledger_db_ = new RocksDbDriver();
#endif
				if (!ledger_db_->Open(path, -1)) {
					return false;
				}

				printf("1:list all key and values\n");
				printf("2:query one key\n");
				char ch;
				std::cin >> ch;
				if (ch == '1'){
#ifdef WIN32
					auto it = (leveldb::Iterator*)ledger_db_->NewIterator();
#else
					auto it = (rocksdb::Iterator*)ledger_db_->NewIterator();
#endif
					for (it->SeekToFirst(); it->Valid(); it->Next()){
						printf("%s:%s\n", utils::String::BinToHexString(it->key().ToString()).c_str(),
							utils::String::BinToHexString(it->value().ToString()).c_str());
					}
				}
				else if (ch == '2')
					while (true){
						printf("\ninput key(hex):");
						std::string hexkey, buff;
						std::cin >> hexkey;
						auto binkey = utils::String::HexStringToBin(hexkey);
						if (ledger_db_->Get(binkey, buff)){
							printf("%s", utils::String::BinToHexString(buff).c_str());
						}
						else{
							printf("%s", ledger_db_->error_desc().c_str());
						}
					}
				return true;
			}
			else {
				Usage();
				return true;
			}
		}

		return false;
	}

	void Argument::Usage() {
		printf(
			"Usage: phantom [OPTIONS]\n"
			"OPTIONS:\n"
			"  --dropdb                                                      clean up database\n"
			"  --peer-address <node-priv-key>                                get peer address from crypted node private key\n"
			"  --create-account [crypto]                                     create account, support ed25519\n"
			"  --get-address <node-priv-key>                                 get address from private key\n"
			"  --get-address-from-pubkey <public-key>                        get address from public key\n"
			"  --sign-data <node-priv-key> <blob data>                       sign blob data\n"
			"  --check-signed-data <blob data> <signed data> <public key>    check signed data\n"
			"  --check-address <address>                                     check address\n"
			"  --hardware-address                                            get local hardware address\n"
			"  --clear-consensus-status                                      delete consensus status\n"
			"  --sm3 <arg>                                                   generate sm3 hash \n"
			"  --sm3-hex <arg>                                               generate sm3 hash from hex format \n"
			"  --aes-crypto <value>                                          crypto value\n"
			"  --version                                                     display version information\n"
			"  --create-hardfork                                             create hard fork ledger\n"
			"  --clear-peer-addresses                                        clear peer list\n"
			"  --create-keystore <password>                                  create key store\n"
			"  --create-keystore-from-privatekey <private key> <password>    create key store from private key\n"
			"  --sign-data-with-keystore <keystore> <password> <blob data>   sign blob data with keystore\n"
			"  --check-keystore <keystore> <password>                        check password match the keystore\n"
			"  --get-privatekey-from-keystore <keystore> <password>          check password match the keystore\n"
			"  --log-dest <dest>                                             set log dest, LIKE FILE+STDOUT+STDERR\n"
			"  --help                                                        display this help\n"
			);
	}

	void Argument::ShowNodeId(int argc, char *argv[]) {
		if (argc < 3) {
			printf("missing parameter, need 1 parameter (the aes_crypto code of private key)\n");
			return;
		}

		if (!utils::String::IsHexString(argv[2])) {
			printf("the node_id of inputting is invalid, please check it!\n");
			return;
		}

		phantom::PrivateKey private_key(utils::Aes::HexDecrypto(argv[2], phantom::GetDataSecuretKey()));

        printf("local peer address (%s)\n", private_key.GetEncAddress().c_str());
	}

	void Argument::ShowHardwareAddress() {
		std::string hard_address = "";
		utils::System system;
		char out_msg[256] = { 0 };
		if (system.GetHardwareAddress(hard_address, out_msg))
			printf("local hardware address (%s)\n", hard_address.c_str());
		else
			printf("%s\n", out_msg);
	}

	void SignalFunc(int32_t code) {
		fprintf(stderr, "Get quit signal(%d)\n", code);
		g_enable_ = false;
	}

	void InstallSignal() {
		signal(SIGHUP, SignalFunc);
		signal(SIGQUIT, SignalFunc);
		signal(SIGINT, SignalFunc);
		signal(SIGTERM, SignalFunc);
#ifndef WIN32
		signal(SIGPIPE, SIG_IGN);
#endif
	}

}