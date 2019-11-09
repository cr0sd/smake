#include<stdio.h>
#include<string.h>
#include<sstream>
#include<vector>
#include<string>
#include<regex>
#include<stack>
#include<map>

#define VERSION "0.5.3"
#define PROG_NAME "smake"
#define DEFAULT_MAKEFILE "SMakefile"


//#define WINDOWS
//#define GNULINUX
#if defined(WINDOWS)
	#define SLASH "\\"
	#include<windows.h>
	#define getcwd(x,y) GetCurrentDirectory(y,x)
	#define OS_MACRO "Windows_NT"
#elif defined(GNULINUX)
	#define SLASH "/"
	#include<unistd.h> // For getcwd
	#define OS_MACRO "GNU/Linux"
#else
	#error What OS are you even on even?
#endif


#define VERSTRING		PROG_NAME " " VERSION " \n" \
						"Build " __DATE__ " " __TIME__

#define HELPSTRING		"usage: smake [options] [target]\n" \
						"-h, --help\t\t\tThis help\n" \
						"-v, --version\t\t\tShow version info\n" \
						"-f, --file FILE\t\t\tUse FILE as makefile\n" \
						"-c\t\t\t\tSet target to 'clean'\n" \
						"-p\t\t\t\tPrint internal data\n" \
						"-P\t\t\t\tAlias to '-np'\n" \
						"-R, --no-builtin-variables\tDisable built-in macros\n" \
						"-n, --just-print\t\tPrint rules (do not execute)"

std::string cwd()
{
	char b[512];
	getcwd(b,512);
	std::string s=b;
	return s;
}


// Expand macros (e.g., '$(OS)' ~~> 'Windows_NT')
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

	std::stack<bool>cond_stack;				// Conditional operation (nestable)

	std::string cur_tgt="";					// Current target
	std::string act_tgt="";					// Active target

	bool found_tgt=false;					// Specified target is found?
	bool list_targets=false;				// Print fetched target info
	bool print_only=false;					// Print rules only
	bool print_database=false;				// '-p': print all internal data
	bool exec=true;							// Will build applicable targets
	bool no_builtin=false;					// Disable built-in macros

	int cur_line=0;							// Line number for errors




	// PART 0 -----
	// Parse argv
	if(argc>=2)
	{
		for(int i=1;i<argc;++i)
		{
			bool custom_makefile=false;	// Flag that user requested file name
			if(strcmp(argv[i],"--help")==0)
				puts(HELPSTRING),
				exit(0);
			else if(strcmp(argv[i],"--file")==0)
				custom_makefile=true;
			else if(strcmp(argv[i],"--version")==0)
				puts(VERSTRING),
				exit(0);
			else if(strcmp(argv[i],"--no-builtin-variables")==0)
			{
				no_builtin=true;
				continue;
			}
			else if(strcmp(argv[i],"--just-print")==0)
			{
				print_only=true;
				exec=false;
				continue;
			}
			else if(strlen(argv[i])>1 && argv[i][0]=='-')
			{
				// Process short flags (i.e., -X)
				if(argv[i][1]!='-')
				{

					for(int j=1;j<(int)strlen(argv[i]);++j)
					{
						if(argv[i][j]=='h')
							puts(HELPSTRING),
							exit(0);
						if(argv[i][j]=='v')
							puts(VERSTRING),
							exit(0);
						if(argv[i][j]=='n')
							print_only=true,
							exec=false;
						else if(argv[i][j]=='c')
							act_tgt="clean";
						else if(argv[i][j]=='p')
							print_database=true,
							list_targets=true;
						else if(argv[i][j]=='P')
							print_database=true,
							list_targets=true,
							print_only=true,
							exec=false;
						else if(argv[i][j]=='R')
							no_builtin=true;
						else if(argv[i][j]=='f')
							custom_makefile=true;
						else
							printf(PROG_NAME ": error: unknown option '%c'\n",argv[i][j]),
							exit(1);
					}
				}
				if(!custom_makefile)continue;
			}

			if(custom_makefile)
			{
				if(++i>=argc)
				{
					puts(PROG_NAME ": error: expected FILE");
					exit(1);
				}
				mkfn=argv[i];
				//custom_makefile=false;
				continue;
			}

			/** DEFAULT OPTION: Makefile name **/
			// No control flow here: Use continue before here to skip
			act_tgt=argv[i];
		}
	}




	// Builtin macros!!!
	if(!no_builtin)
	{
		macro_map["CC"]="cc";
		macro_map["CXX"]="c++";
		macro_map["LD"]="ld";
		macro_map["AS"]="as";
		macro_map["AR"]="ar";
		macro_map["ARFLAGS"]="rv";
		macro_map["CPP"]="cc -E";
		macro_map["PWD"]=dir;
		macro_map["RM"]="rm -f"; // GNU Make uses the '-f' flag (so we do too)
		macro_map["OS"]=OS_MACRO;
		macro_map["SMAKE"]=VERSION; // So we know this is Smake
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
		const char *trythese[]={	"Smakefile",
									"smakefile",
									"Makefile",
									"makefile"};

		// Try default name (SMakefile)
		fn=dir+SLASH+DEFAULT_MAKEFILE;
		f=fopen(fn.c_str(),"r");	

		// Otherwise try these
		for(auto that:trythese)
		{
			if(f)break;
			fn=dir+SLASH+that,
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
	 *	C. Comment
	 *	D. (Default case) Rule/command
	 *	I. If conditionals...
	 *	I-. Endif
	 *	V. Variable assignment
	 *	V+. Variable concatenation
	 * 
	 ***************/
	while(!feof(f) && f)
	{
		std::smatch match;
		std::regex reg;
		std::string line="";
		char str[512];
		bool concat_line=false;

		// Get line
		// Repeat and concatenate for /\\\n/ sequence
		do
		{
			// Get line
			fgets(str,512,f);
			if(feof(f))*str=0;
			++cur_line;

			// Check for concat char
			{
				char*p=strstr(str,"\\\n");
				if(p && !feof(f))
					*p=0,
					//strcpy(p,""),
					concat_line=true;
					//puts(str);
				else
					concat_line=false;
			}


			// Copy line to std::string
			if(str[0]=='\t')line+=str+1;
			else line+=str;
			//puts(str);

			//line=std::regex_replace(line,reg="\\\n","");
			line=std::regex_replace(line,reg="[ \t]*\n","");	// Remove space at end
			line=std::regex_replace(line,reg="^[ \t]","");		// Remove initial space

		} while(!feof(f) && concat_line);

		if(feof(f) && concat_line)
			printf(PROG_NAME": %d: error: line concatenation at EOF",cur_line),
			exit(1);

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


		/*** Pattern I#1 ***/
		// ifdef
		else if(std::regex_match(line,
			reg=R"([ \t]*ifdef[ \t]+\([ \t]*(\$\([a-zA-Z_\.]*\)|[a-zA-Z_\.]*)[ \t]*\)[ \t]*)"))
		{
			std::regex_search(line,match,reg);
			bool cond=replace_macros(match[1],macro_map).empty()==false;
			cond_stack.push(cond && (cond_stack.size()==0 || cond_stack.top()));
		}



		/*** Pattern I#2 ***/
		// ifeq
		else if(std::regex_match(line,
			reg=R"([ \t]*ifeq[ \t]+\([ \t]*(\$\([a-zA-Z_\.]*\)|[a-zA-Z_\.]*)[ \t]*,[ \t]*(\$\([a-zA-Z_\.]*\)|[a-zA-Z_\.]*)[ \t]*\)[ \t]*)"))
		{
			std::regex_search(line,match,reg);
			bool cond=replace_macros(match[1],macro_map)==
				replace_macros(match[2],macro_map);
			cond_stack.push(cond && (cond_stack.size()==0 || cond_stack.top()));
		}



		/*** Pattern I- ***/
		else if(std::regex_match(line,reg=R"([ \t]*endif[ \t]*)"))
		{
			if(cond_stack.empty()==false)
				cond_stack.pop();
			else
			{
				printf(PROG_NAME": %d: error: unexpected endif\n",cur_line);
				exit(1);
			}
		}

		/*** Pattern E ***/
		else if(std::regex_match(line,reg="[ \t]*else[ \t]*[#.*]*"))
		{
			// Do not change nesting level, just Complement
			// the bool condition at top level of stack
			// Important: Make sure the previous level is also
			// TRUE!

			bool nv=cond_stack.top();
			cond_stack.pop();
			cond_stack.push(!nv && 
				(cond_stack.size()==0 || cond_stack.top()));
		}


		/*** Pattern C ***/
		// Comment y'all
		///// Copy comments to rules, but not anything else
		///// Maybe handle comments per each pattern
		else if(std::regex_match(line,reg="[ \t]*#.*"))
			{
				if(dep_map.size()==0)continue;
				// Strip leading whitespace
				std::regex r("[\t ]*(.*)");
				std::regex_search(line,match,r);
				std::string t=match[1];

				// Push command onto TGT=>RULE vector
				rule_map[cur_tgt].push_back(t);
			}


		/*** Pattern V ***/
		// Line matches VAR = VALUE pattern
		// Assign internal make macros
		else if(std::regex_match(line,reg="([a-zA-Z_]*) *= *(.*)"))
		{
			// PARSE ASSIGNMENTS
			// We want to parse assignments here

			if(cond_stack.size()!=0 && cond_stack.top()==false)
				continue;

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
		else if(std::regex_match(line,reg="([a-zA-Z_]*) +\\+= *(.*)"))
		{

			if(cond_stack.size()!=0 && cond_stack.top()==false)
				continue;

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
				printf(PROG_NAME": %d: error: rule/command before first target\n",cur_line);
				exit(1);
			}

			// Create dep_map and rule_map
			// Skip if -l is used
			//if(exec)
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


	// Check that conditionals were closed correctly
	// (i.e., count(if*) == count(endif))
	if(cond_stack.size()!=0)
		puts(PROG_NAME": error: conditional has no matching endif"),
		exit(1);


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
				if(exec && s[0]!='#')
					system(s.c_str());
			}
			dep_order.pop();
		}
	}


	// Print internal database if '-p' used
	if(print_database)
	{
		if(exec)puts("");
		printf("# macros found in '%s' (or builtin):\n",fn.c_str());
		for(auto p:macro_map)
		{
			if(p.first.empty())
			{
				puts("invalid assignment");
				continue;
			}
			printf("# %s=%s\n",p.first.c_str(),p.second.c_str());
		}
		puts("");
	}

	// List targets if '-p' used
	if(list_targets)
	{
		printf("# %d targets found in '%s':\n",(int)(dep_map.size()),fn.c_str());
		for(auto x:dep_map)
		{
			printf("# %s",x.first.c_str());
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
