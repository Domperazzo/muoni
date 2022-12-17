//libreria per la gestione degli argomenti

/*	IDEA
passo a una funzione parse tutti gli argomenti esistenti.
la funzione parse divide fra i vari comandi identificati da -l kkjdc

nel main devo definire quali argomenti esistono e a che elemento associare il contenuto del comando.
inoltre potrei definire che tipo di comando è (booleano, testo, numero...)

dovrebbe anche essere definibile il comando help in automatico


*/


#ifndef ARGS_H
#define ARGS_H

#include <iostream>
#include <string>
#include <string.h> 
#include <vector>

class arg{
	private:
		std::string arg_value;
		std::string arg_command;
		std::string description;
		bool is_present;

	public:
		arg(int argc, char * argv[], std::string com): arg_command(com){	// costruttore
			int num = 0;
			for(int i = 1; i < argc; i++){
				if(strcmp(argv[i], com.c_str()) == 0){
					num = i;
					break;
				}
			}

			if(strcmp(com.c_str(), "-h") == 0 && num != 0){
				is_present = true;
				return;
			}
			if (argc == num + 1){
				is_present = false;
				return;
			}
			else if(num == 0 || argv[num+1][0] == '-'){ 
				is_present = false;
			}
			else{
				is_present = true;
				arg_value = argv[num + 1];
			}
		}

		std::string GetValue(){
			if(is_present) return arg_value;
			else return "null";
		}

		bool GetValid(){
			return is_present;
		}

		std::string GetCom(){
			if(is_present) return arg_command;
			else return "null";
		}

		void Set_description(const std::string & des){
			description = des;
			return;
		}

		void Print_help(){
			std::cout << arg_command << " :\t\t" << description << std::endl;
		}
};

class argomenti{
	private:
		int argc;
		char ** argv;
		std::vector<arg*> arguments;
		std::string help;

	public:
		argomenti(int o_argc, char * o_argv[]){
			argc = o_argc;
			argv = o_argv;
		}

		void NewArg(std::string com, std::string desc){
			arguments.push_back(new arg(argc, argv, com));
			arguments[arguments.size() - 1] -> Set_description(desc);
		}

		void Help(){
			for(int i = 0; i < arguments.size(); i++){
				arguments[i] -> Print_help();
			}
		}

		std::string GetValue(const std::string & com){
			for(int i = 0; i < arguments.size(); i++){
				if(arguments[i] -> GetCom() == com){
					return arguments[i] -> GetValue();
				}
			}
			return "null";
		}

		bool GetValid(const std::string & com){
			for(int i = 0; i < arguments.size(); i++){
				if(arguments[i] -> GetCom() == com){
					return arguments[i] -> GetValid();
				}
			}
			return false;
		}
	
};


#define START_ARGS	argomenti * args = new argomenti(argc, argv);	\
					args -> NewArg("-h", "help");					


#define HELP_ARGS	if(args -> GetValid("-h")){						\
						args -> Help();								\
						return 1; 									\
					};


#endif
