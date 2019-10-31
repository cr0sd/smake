#include<stdio.h>
#include<string.h>
#include<vector>
#include<string>
#include<regex>
#include<stack>

//#define WINDOWS
#define PROG_NAME "smake"
#include"platform.h"
#include"dir.cc"
#include"file.cc"

int main(int argc,char**argv)
{
	std::string dir=cwd();					// Absolute CWD path
	std::string fn=dir+SLASH+"makefile";	// Absolute makefile path
	FILE*f=fopen(fn.c_str(),"r");			// Makefile

	std::map<std::string,std::vector<std::string>>
		dep_map; 							// Dependencies map
	std::map<std::string,std::vector<std::string>>
		rule_map;							// Rule map

	std::string cur_tgt="";					// Current target
	std::string act_tgt="all";				// Active target
	std::vector<std::string>cur_rule;		// Current rule
	std::vector<std::string>act_rule;		// Active rule

	bool found_tgt=false;					// Specified target is found?
	bool list_targets=false;				// '-l' option
	bool print_only=false;					// Print only

	// Parse argv
	if(argc>=2)
	{
		for(int i=1;i<argc;++i)
		{
			if(strcmp(argv[i],"--help")==0 || strcmp(argv[i],"-h")==0)
				puts(	"usage: smake [OPTIONS]\n"
						"--help, -h\tThis help\n"
						"-l\t\tList targets (do not execute)\n"
						"-n\t\tPrint rules (do not execute)"),
				exit(0);
			else if(strcmp(argv[i],"-l")==0)
				list_targets=true;
			else if(strcmp(argv[i],"-n")==0)
				print_only=true;
			else // Default: use arg as target
				act_tgt=argv[i];
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
		//std::string tok="";
		std::string deps="";

		// Check if line matches target definition
		// Set target if so
		std::smatch match;
		std::regex reg
			("([a-zA-Z_\\-\\.]+):(.*)");
		if(std::regex_match(line,reg))
		{
			std::regex_search(line,match,reg);

			cur_tgt=match[1];
			deps=match[2];

			// Map TARGET => Dependencies

			{
				// Split deps by space
				std::string tmp; 
				std::stringstream ss(deps);
				std::vector<std::string>v;

				while(std::getline(ss,tmp,' '))
					v.push_back(tmp);
				dep_map[cur_tgt]=v;
			}

			if(act_tgt==cur_tgt)
				found_tgt=true;
		}

		else // Not a target line
		{
			// System call if found correct target
			// Skip if -l is used
			if(found_tgt && /*cur_tgt==act_tgt &&*/ !list_targets)
			{
				// Strip leading whitespace
				std::regex r("[\t ]*(.*)");
				std::regex_search(line,match,r);
				std::string t=match[1];


				// Push onto correct rule
				if(act_tgt==cur_tgt)
					act_rule.push_back(t);
				else
					cur_rule.push_back(t);
			}
		}
	}

	// Process rules
	if(!list_targets)
	{
		//puts(act_tgt.c_str());
		for(auto s:act_rule)
		{
			// Strip leading '@', print rule as appropriate
			if(s.front()=='@')
			{
				s.erase(0,1);
				if(print_only)
					puts(s.c_str());
			}
			else
				puts(s.c_str());

			// Print and/or execute line
			//printf(s.c_str());
			if(!print_only)system(s.c_str());
		}
	}

	// List targets if '-l' used
	if(list_targets)
	{
		printf("%d targets in '%s':\n",(int)(dep_map.size()),fn.c_str());
		for(auto x:dep_map)
		{
			printf(x.first.c_str());
			if(!x.second.empty())
			{
				printf(": ");
				for(auto s:x.second)
					printf("%s, ",s.c_str());
			}
			puts("");
		}
	}

	// Alert if target not found
	if(!found_tgt)
		printf("%s: error: could not find target "
			"'%s'\n",PROG_NAME,act_tgt.c_str());
}
