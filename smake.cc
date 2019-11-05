#include<stdio.h>
#include<string.h>
#include<vector>
#include<string>
#include<regex>
#include<stack>
#include<map>

#define PROG_NAME "smake"
#define WINDOWS
#define DEFAULT_MAKEFILE "SMakefile"

#if defined(WINDOWS)
	#define SLASH "\\"
	#include<windows.h>
	#define getcwd(x,y) GetCurrentDirectory(y,x)
#else
	#define SLASH "/"
	#include<unistd.h> // For getcwd
#endif

std::string cwd()
{
	char b[512];
	getcwd(b,512);
	std::string s=b;
	return s;
}

std::string
replace_macros(std::string s,
	std::map<std::string,std::string>macro_map)
{
	//printf("ORIGINAL:\t'%s'\n",s.c_str());
	// Replace matches in s from macro_map
	std::smatch m;
	do
	{
		std::regex_search(s,m,std::regex(R"(\$\( *([^\(\)]*) *\))"));
		for(int i=1;i<(int)m.size();++i)
		{
			std::string replace=macro_map[m[i].str()];
			s=std::regex_replace(s,
				std::regex("\\$\\("+m[i].str()+"\\)"),
				replace);
		}
	} while(m.size()>0);

	//printf("NEW:\t\t'%s'\n",s.c_str());
	return s;
}

int main(int argc,char**argv)
{
	std::string dir=cwd();					// Absolute CWD path
	std::string mkfn=DEFAULT_MAKEFILE;		// Concatenated with fn
	std::string fn=dir+SLASH+mkfn;			// Absolute makefile path
	FILE*f;//=fopen(fn.c_str(),"r");		// Makefile

	std::map<std::string,std::vector<std::string>>
		dep_map; 							// Dependencies map
	std::map<std::string,std::vector<std::string>>
		rule_map;							// Rule map
	std::map<std::string,std::string>
		macro_map;							// Associative macro list (MACNAME => VALUE)

	std::string cur_tgt="";					// Current target
	std::string act_tgt="";					// Active target

	bool found_tgt=false;					// Specified target is found?
	bool list_targets=false;				// '-l' option
	bool print_only=false;					// Print only
	bool print_database=false;				// '-p': print internal data
	bool exec=true;							// Will build applicable targets
	bool no_builtin=false;					// Disable built-in macros

	int cur_line=0;							// Line number for errors




	// PART 0 -----
	// Parse argv
	if(argc>=2)
	{
		for(int i=1;i<argc;++i)
		{
			if(strcmp(argv[i],"--help")==0 || strcmp(argv[i],"-h")==0)
				puts(	PROG_NAME " " __DATE__ " " __TIME__ "\n"
						"usage: smake [OPTIONS]\n"
						"-h, --help\tThis help\n"
						"-f, --file FILE\tUse FILE as makefile\n"
						"-c\t\tSet target to 'clean'\n"
						"-p\t\tPrint internal data\n"
						"-R\t\tDisable built-in macros\n"
						"-n\t\tPrint rules (do not execute)"),
				exit(0);
			else if(strcmp(argv[i],"-n")==0 || strcmp(argv[i],"--print-only")==0)
				print_only=true,
				exec=false;
			else if(strcmp(argv[i],"-c")==0)
				act_tgt="clean";
			else if(strcmp(argv[i],"-p")==0)
				print_database=true,
				list_targets=true;
			else if(strcmp(argv[i],"-R")==0)
				no_builtin=true;
			else if(strcmp(argv[i],"-f")==0 || strcmp(argv[i],"--file")==0)
			{
				if(++i>=argc)
				{
					puts(PROG_NAME ": error: expected FILE");
					exit(1);
				}
				mkfn=argv[i];
			}
			else // Default: use arg as target
				act_tgt=argv[i];
		}
	}




	// Builtin macros!!!
	if(!no_builtin)
	{
		macro_map["CC"]="cc";
		macro_map["CXX"]="c++";
		macro_map["LD"]="ld";
		macro_map["RM"]="rm -f"; // GNU Make uses the '-f' flag (so we do too)
	}


	// Open custom makefile name if specified
	//if(strcmp(mkfn.c_str(),DEFAULT_MAKEFILE)!=0)

	// Custom smakefile
	if(mkfn!=DEFAULT_MAKEFILE)
	{
		fn=dir+SLASH+mkfn;
		f=fopen(fn.c_str(),"r");
	}

	// Default smakefiles
	else
	{
		fn=dir+SLASH+DEFAULT_MAKEFILE;
		f=fopen(fn.c_str(),"r");	// Try default name (SMakefile)
		if(!f)
		{
			fn=dir+SLASH+"Smakefile";
			f=fopen(fn.c_str(),"r");
		}
		if(!f)
		{
			fn=dir+SLASH+"smakefile";
			f=fopen(fn.c_str(),"r");
		}
		if(!f)
		{
			// Otherwise try 'makefile'
			fn=dir+SLASH+"makefile";
			f=fopen(fn.c_str(),"r");
		}
	}


	// Check file is good (we didn't do this before
	// in case user typed '--help'
	if(!f)
	{
		printf(PROG_NAME ": error: could not open file "
			"'%s'\n",fn.c_str());
		exit(1);
	}


	// PART 1 -----
	// Parse file (using std::regex)
	// Fill in targets, dependencies, and rules
	/***************
	 * 
	 * Patterns:
	 *	A. Target: dependencies
	 *	D. (Default case) Rule/command
	 *	V. Variable assignment
	 *	V+. Variable concatenation
	 * 
	 ***************/
	while(!feof(f) && f)
	{
		std::smatch match;
		std::regex reg;
		std::string line;
		char str[512]={0};

		// Get line
		fgets(str,512,f);
		++cur_line;

		// Copy line to std::string
		line=str;

		// Remove endline (from fgets)
		//std::replace(line.begin(),line.end(),'\n','');

		line=std::regex_replace(line,reg="[ \t]*\n","");	// Remove space at end
		line=std::regex_replace(line,reg="^[ \t]","");		// Remove initial space
		//puts(line.c_str());

		// Check for empty line
		//if(std::regex_match(line,reg="[ \t\n]*")) 
			//continue;
		if(line.empty())continue;



		/*** Pattern A ***/
		// Check if line matches target definition
		// Set target if so
		reg="([a-zA-Z_\\-\\.]+)[ \\t]*:(.*)";
		// Target format: "(name_of_target): (dependencies)"
		//                 Group 1           Group 2
		if(std::regex_match(line,reg))
		{
			std::regex_search(line,match,reg);

			cur_tgt=match[1];

			// act_tgt assumes first valid target in makefile
			if(act_tgt=="")act_tgt=cur_tgt;
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


		/*** Pattern C ***/
		// Comment y'all
		else if(std::regex_match(line,reg="#.*"))
			continue;


		/*** Pattern V ***/
		// Line matches VAR = VALUE pattern
		// Assign internal make macros
		else if(std::regex_match(line,reg="([a-zA-Z_]*) *= *(.*)"))
		{
			// PARSE ASSIGNMENTS
			// We want to parse assignments here

			std::regex_search(line,match,reg);

			if(match[1].str().empty())
			{
				printf(PROG_NAME": %d: error: empty macro name",cur_line);
				exit(1);
			}

			// Map VARIABLE => VALUE
			macro_map[match[1].str()]=match[2].str();

			//printf("$(%s) == $(%s)\n",
				//match[1].str().c_str(),
				//match[2].str().c_str());
		}


		/*** Pattern V+ ***/
		// Line matches VAR += VALUE
		// Concatenate VALUE to end of VAR
		else if(std::regex_match(line,reg="([a-zA-Z_]*) +\\+=  *(.*)"))
		{
			std::regex_search(line,match,reg);

			if(match[1].str().empty())
			{
				printf(PROG_NAME": %d: error: empty macro name",cur_line);
				exit(1);
			}

			macro_map[match[1]]+=" "+match[2].str();
		}

		/*** Pattern D ***/
		// Default pattern
		// Rule/command
		else
		{
			if(dep_map.empty())
			{
				printf(PROG_NAME": %d: error: recipe before first target\n",cur_line);
				exit(1);
			}
			// Create dep_map and rule_map
			// Skip if -l is used
			if(exec)
			{
				// Strip leading whitespace
				std::regex r("[\t ]*(.*)");
				std::regex_search(line,match,r);
				std::string t=match[1];

				// Push command onto TGT=>RULE vector
				rule_map[cur_tgt].push_back(t);
			}
		}
	}
	fclose(f);



	// PART 2 -----
	// Now execute/print rules

	// Create stack of targets in order
	// of 'leaf nodes' first, up to active target
	std::stack<std::string>dep_order;
	if(found_tgt)
	{
		std::stack<std::string>st;
		st.push(act_tgt);
		while(st.size()>0)
		{
			std::string str=st.top();
			st.pop();
			dep_order.push(str);
			for(auto s:dep_map[str])
				st.push(s);
		}
	}

	// Read/process rules
	// If we are executing them or printing them
	if(exec || print_only)
	{
		while(dep_order.size()>0)
		{
			for(std::string s:rule_map[(dep_order.top())])
			//for(std::string s:v)
			{
				// Expand macros
				s=replace_macros(s,macro_map).c_str();

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
				if(exec)
					system(s.c_str());
			}
			dep_order.pop();
		}
	}


	// Print internal database if '-p' used
	if(print_database)
	{
		if(exec)puts("");
		printf("# macros found in '%s':\n",fn.c_str());
		for(auto p:macro_map)
		{
			if(p.first.empty())
			{
				puts("invalid assignment");
				continue;
			}
			printf("$(%s)=='%s'\n",p.first.c_str(),p.second.c_str());
		}
		puts("");
	}

	// List targets if '-p' used
	if(list_targets)
	{
		printf("# %d targets found in '%s':\n",(int)(dep_map.size()),fn.c_str());
		for(auto x:dep_map)
		{
			printf(x.first.c_str());
			if(!x.second.empty())
			{
				printf(": ");
				for(size_t i=0;i<x.second.size();++i)
				{
					printf("%s",x.second[i].c_str());
					if(i<x.second.size()-1)
						printf(", ");
				}
			}
			puts("");
		}
	}

	// Alert if target not found
	if(!found_tgt)
		printf(PROG_NAME": error: could not find target "
			"'%s'\n",act_tgt.c_str());
}
