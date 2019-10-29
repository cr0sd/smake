#include<stdio.h>
#include<string.h>
#include<vector>
#include<string>
#include<regex>
#define SLASH "\\"
#define PROG_NAME "smake"
#include"dir.cc"
#include"file.cc"

int main(int argc,char**argv)
{
	std::string dir=pwd();
	std::string fn=dir+SLASH+"makefile";
	std::string fs=filestring(fn.c_str());
	FILE*f=fopen(fn.c_str(),"r");

	std::string cur_tgt="";			// Current target
	std::string tgt="all";				// Active target

	bool found_tgt=false;

	// Get CLI arguments
	if(argc>=2) tgt=argv[1];

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
			//printf("\n\nNEW TARGET:'%s'\n",
				//cur_tgt.c_str());
			if(tgt==cur_tgt)
				found_tgt=true;
			else if(found_tgt)
				break; // Stop when finished
				//puts("\t\tcur_tgt == tgt!!");
		}

		else // Not a target line
		{
			// System call if found correct target
			if(found_tgt && cur_tgt==tgt)
			{
				printf("system('%s')\n",line.c_str());
				system(line.c_str());
			}
		}
	}

	// Alert if target not found
	if(!found_tgt)
		printf("%s: error: could not find target "
			"'%s'\n",PROG_NAME,tgt.c_str());
}
