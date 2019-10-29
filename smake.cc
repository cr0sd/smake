#include<stdio.h>
#include<string.h>
#include<vector>
#include<string>
#include<regex>
#include"dir.cc"
#include"file.cc"
#define SLASH "\\"

int main(int argc,char**argv)
{
	std::string dir=pwd();
	std::string fn=dir+SLASH+"makefile";
	std::string fs=filestring(fn.c_str());
	FILE*f=fopen(fn.c_str(),"r");

	std::string cur_tgt="";			// Current target
	std::string tgt="all";				// Active target

	bool found_tgt=false;

	if(argc>=2)
		tgt=argv[1];

	// Parse file (using std::regex)
	while(!feof(f))
	{
		// Get line
		char str[512]={0};
		fgets(str,512,f);

		// stop if line is empty
		if(*str==0) continue;

		// Remove endlines
		if(str[strlen(str)-1]=='\n')
			str[strlen(str)-1]=0;

		// Copy line to std::string
		std::string line=str;
		std::string tok="";

		// Check if line matches target definition
		// Set target if so
		std::smatch match;
		std::regex reg
			("([a-zA-Z_]+[a-zA-Z0-9_]*):.*");

		if(std::regex_match(line,reg))
		{
			std::regex_search(line,match,reg);

			cur_tgt=match[1];
			printf("\n\nNEW TARGET:'%s'\n",
				cur_tgt.c_str());
			if(tgt==cur_tgt)
				found_tgt=true;
			else if(found_tgt)
				break; // Stop when finished
				//puts("\t\tcur_tgt == tgt!!");
		}

		// Tokenize
		printf("LINE:'%s'\n",line.c_str());
		char *ptr=strtok(str," \t");
		do
		{
			tok=ptr;
			printf("[%s] ",tok.c_str());
			//cmd+=tok;
		} while(ptr=strtok(NULL," \t\n"));
		puts("");
	}
	if(!found_tgt)
		printf("error: could not find target "
			"'%s'\n",tgt.c_str());
}
