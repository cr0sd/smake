#include<stdio.h>
#include<string.h>
#include<vector>
#include<string>
#include<regex>
#define WINDOWS
#include"platform.h"
#define PROG_NAME "smake"
#include"dir.cc"
#include"file.cc"

int main(int argc,char**argv)
{
	std::string dir=cwd();
	std::string fn=dir+SLASH+"makefile";
	std::string fs=filestring(fn.c_str());
	FILE*f=fopen(fn.c_str(),"r");

	std::string cur_tgt="";			// Current target
	std::string tgt="all";				// Active target

	bool found_tgt=false;
	bool list_targets=false;
	std::string targets="";
	int n_targets=0;

	// Get CLI arguments
	if(argc>=2)
	{
		for(int i=1;i<argc;++i)
		{
			if(strcmp(argv[i],"--help")==0 || strcmp(argv[i],"-h")==0)
				puts(	"usage: smake [OPTIONS]\n"
						"--help, -h\tThis help\n"
						"-l\t\tList targets (do not execute)"),
				exit(0);
			else if(strcmp(argv[i],"-l")==0)
				//puts("LIST TARGETS:"),
				list_targets=true;
			else // Default: use arg as target
				tgt=argv[i];
		}
	}

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
			("([a-zA-Z_\\-\\.]+):.*");
		if(std::regex_match(line,reg))
		{
			std::regex_search(line,match,reg);

			cur_tgt=match[1];
			targets+="\n"+cur_tgt;
			++n_targets;
			//printf("\n\nNEW TARGET:'%s'\n",
				//cur_tgt.c_str());
			if(tgt==cur_tgt)
				found_tgt=true;
			else if(found_tgt && !list_targets)
				break; // Stop when finished (and not counting n_targets)
				//puts("\t\tcur_tgt == tgt!!");
		}

		else // Not a target line
		{
			// System call if found correct target
			// Skip if -l is used
			if(found_tgt && cur_tgt==tgt && !list_targets)
			{
				// Remove leading whitespace
				std::regex r("[\t ]*(.*)");
				std::regex_search(line,match,r);
				std::string t=match[1];
				//printf("system('%s')\n",t.c_str());
				puts(t.c_str());
				system(line.c_str());
			}
		}
	}

	// List targets if '-l' used
	if(list_targets)
		printf("Targets in '%s':\n",fn.c_str()),
		printf("NUMBER TARGETS:%d\n%s",n_targets,targets.c_str());

	// Alert if target not found
	if(!found_tgt)
		printf("%s: error: could not find target "
			"'%s'\n",PROG_NAME,tgt.c_str());
}
