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

	bool found_tgt=false;					// Specified target is found?
	bool list_targets=false;				// '-l' option
	bool print_only=false;					// Print only


	// PART 0 -----
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


	// PART 1 -----
	// Parse file (using std::regex)
	// Fill in targets, dependencies, and rules
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

		// Check if line matches target definition
		// Set target if so
		std::smatch match;
		std::regex reg("([a-zA-Z_\\-\\.]+):(.*)");
		// Target format: "(name_of_target): (dependencies)"
		//                 Group 1           Group 2
		if(std::regex_match(line,reg))
		{
			std::regex_search(line,match,reg);

			cur_tgt=match[1];
			//deps=match[2];

			// Map TARGET => Dependencies

			// Split deps by space
			std::string tmp; 
			std::stringstream ss(match[2]);

			while(std::getline(ss,tmp,' '))
				dep_map[cur_tgt].push_back(tmp);

			if(match[2].str().empty())
				dep_map[cur_tgt].clear();

			if(act_tgt==cur_tgt)
				found_tgt=true;
		}

		else // Not a target line
		{
			// Create dep_map and rule_map
			// Skip if -l is used
			if(found_tgt && /*cur_tgt==act_tgt &&*/ !list_targets)
			{
				// Strip leading whitespace
				std::regex r("[\t ]*(.*)");
				std::regex_search(line,match,r);
				std::string t=match[1];


				// Push onto correct rule
				rule_map[cur_tgt].push_back(t);

				//if(act_tgt==cur_tgt)
					//rule_map[cur_tgt].push_back(t);
					////act_rule.push_back(t);
				//else
					//rule_map[cur_tgt].push_back(t);
					////cur_rule.push_back(t);
			}
		}
	}



	// PART 2 -----
	// Now execute/print rules

	//printf("%s: ",act_tgt.c_str());
	//if(dep_map[act_tgt].empty()==false)
		//puts(dep_map[act_tgt][0].c_str());
	//else
		//puts("");

	// TEST: Display rule_map
	{
		for(auto p:rule_map)
		{
			// NOTE: All seems well -- now we should construct
			// a stack/queue with targets+dependencies in correct sequence
			printf("''%s'':\n",p.first.c_str());
			for(auto s:p.second)
				printf("\t''%s''\n",s.c_str());
		}
		
	}

	// Process rules
	if(!list_targets)
	{
		for(auto s:rule_map[act_tgt])
		{
			// Strip leading '@', print rule as appropriate
			if(s.front()=='@') // @ Silences a rule unless print_only
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
		printf("%d targets found in '%s':\n",(int)(dep_map.size()),fn.c_str());
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
