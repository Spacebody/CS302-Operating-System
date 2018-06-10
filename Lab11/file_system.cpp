/***                                                                          
 *          .,:,,,                                        .::,,,::.          
 *        .::::,,;;,                                  .,;;:,,....:i:         
 *        :i,.::::,;i:.      ....,,:::::::::,....   .;i:,.  ......;i.        
 *        :;..:::;::::i;,,:::;:,,,,,,,,,,..,.,,:::iri:. .,:irsr:,.;i.        
 *        ;;..,::::;;;;ri,,,.                    ..,,:;s1s1ssrr;,.;r,        
 *        :;. ,::;ii;:,     . ...................     .;iirri;;;,,;i,        
 *        ,i. .;ri:.   ... ............................  .,,:;:,,,;i:        
 *        :s,.;r:... ....................................... .::;::s;        
 *        ,1r::. .............,,,.,,:,,........................,;iir;        
 *        ,s;...........     ..::.,;:,,.          ...............,;1s        
 *       :i,..,.              .,:,,::,.          .......... .......;1,       
 *      ir,....:rrssr;:,       ,,.,::.     .r5S9989398G95hr;. ....,.:s,      
 *     ;r,..,s9855513XHAG3i   .,,,,,,,.  ,S931,.,,.;s;s&BHHA8s.,..,..:r:     
 *    :r;..rGGh,  :SAG;;G@BS:.,,,,,,,,,.r83:      hHH1sXMBHHHM3..,,,,.ir.    
 *   ,si,.1GS,   sBMAAX&MBMB5,,,,,,:,,.:&8       3@HXHBMBHBBH#X,.,,,,,,rr    
 *   ;1:,,SH:   .A@&&B#&8H#BS,,,,,,,,,.,5XS,     3@MHABM&59M#As..,,,,:,is,   
 *  .rr,,,;9&1   hBHHBB&8AMGr,,,,,,,,,,,:h&&9s;   r9&BMHBHMB9:  . .,,,,;ri.  
 *  :1:....:5&XSi;r8BMBHHA9r:,......,,,,:ii19GG88899XHHH&GSr.      ...,:rs.  
 *  ;s.     .:sS8G8GG889hi.        ....,,:;:,.:irssrriii:,.        ...,,i1,  
 *  ;1,         ..,....,,isssi;,        .,,.                      ....,.i1,  
 *  ;h:               i9HHBMBBHAX9:         .                     ...,,,rs,  
 *  ,1i..            :A#MBBBBMHB##s                             ....,,,;si.  
 *  .r1,..        ,..;3BMBBBHBB#Bh.     ..                    ....,,,,,i1;   
 *   :h;..       .,..;,1XBMMMMBXs,.,, .. :: ,.               ....,,,,,,ss.   
 *    ih: ..    .;;;, ;;:s58A3i,..    ,. ,.:,,.             ...,,,,,:,s1,    
 *    .s1,....   .,;sh,  ,iSAXs;.    ,.  ,,.i85            ...,,,,,,:i1;     
 *     .rh: ...     rXG9XBBM#M#MHAX3hss13&&HHXr         .....,,,,,,,ih;      
 *      .s5: .....    i598X&&A&AAAAAA&XG851r:       ........,,,,:,,sh;       
 *      . ihr, ...  .         ..                    ........,,,,,;11:.       
 *         ,s1i. ...  ..,,,..,,,.,,.,,.,..       ........,,.,,.;s5i.         
 *          .:s1r,......................       ..............;shs,           
 *          . .:shr:.  ....                 ..............,ishs.             
 *              .,issr;,... ...........................,is1s;.               
 *                 .,is1si;:,....................,:;ir1sr;,                  
 *                    ..:isssssrrii;::::::;;iirsssssr;:..                    
 *                         .,::iiirsssssssssrri;;:.                      
 */		

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <list>
#include <algorithm>
#include <cstring>
#include <functional>

using namespace std;

// limit of strings
#define MAX_STRING_LENGTH 1024
#define MAX_LENGTH 4096

//return status of functions
#define SUCCEED 1
#define FAILED 0
#define ERROR -1

//file status
#define OPENED 2
#define CLOSED 3

//file modes
#define WRITE 'w'
#define READ 'r'
#define EXECUTABLE 'x'
#define NONE '-'

//default setting for root
#define ROOT_NAME "root"
#define ROOT_PASSWD 111

//type, file is 'f', and directory is 'd' 
#define _FILE 'f'
#define _DIR 'd'

//settings of workspace
#define HEAD "$"
#define TAIL "%"
#define DEFAULT_WORKSPACE "/"
#define SEPARATOR "/"

//yes or not
#define YES 1
#define NO 0

//define the color of output text
#define RESET "\033[0m"
#define BOLD_BLUE "\033[1m\033[34m"  //Bold blue for dirctory
#define BOLD_GREEN "\033[1m\033[32m" //Bold green for execuatble files

/* File system structure */
// file
typedef struct file_struct
{
	char file_name[MAX_STRING_LENGTH];
	int length;
	char *content;
	int position;
	char modes[3]; //r, w, x
	int modes_sum; //omode(0~7)
	const char property = _FILE; // f, means file
    int status; //opened or closed
    char creator[MAX_STRING_LENGTH]; //creator
    char opener[MAX_STRING_LENGTH];
} *file;

// directory
typedef struct directory_struct
{	
	char directory_name[MAX_STRING_LENGTH];
	int item_number; //the number of files and directories
	char modes[3]; //r, w, x
	const char property = _DIR; // d, means directory
	char creator[MAX_STRING_LENGTH]; //creator
	vector<struct directory_struct *> dirs;
	vector<file> files;
} *directory;

// user
typedef struct user_profile
{
	char user_name[MAX_STRING_LENGTH];
	int password;
} *user_info;


typedef vector<user_info> users_dict;

//multiple users, single file system
users_dict users; //record all users
users_dict::iterator user_iter; //iterator
char workspace[MAX_STRING_LENGTH]; //will change according to current workspace
directory root_dir = NULL; //root of the file system
vector<directory>::iterator dir_iter;//directory iterator
vector<file>::iterator file_iter; //file iterator
user_info current_user = NULL; //record current user
directory current_dir = NULL; //record current working directory
vector<pair<file, const char *>> opened_records; //record opened file
vector<pair<file, const char *>>::iterator opened_iter; //record iterator 

void init_fs();  //initialize file system
int wait_for_cmd(const user_info &user);
//commands
void _man();
void _pwd();
int _mkdir(directory &dir, const int has_path, char *args);
int _dedir(directory &dir, const int has_path, char *args);
int _search(const char *file_name);
int _create(directory &dir, const int has_path, char *args);
int _cat(directory &dir, const int has_path, char *args);
int _delete(directory &dir, const int has_path, char *args);
int _write(directory &dir, const int has_path, char *args);
int _reposition(directory &dir, const int has_path, char *args);
int _truncate(directory &dir, const int has_path, char *args);
void _ls(directory &dir, const int has_path, char *args);
int _open(directory &dir, const int has_path, char *args, char *path);
int _close(directory &dir, const int has_path, char *args);
int _regusr(char *args);
int _delusr(const char *args);
void _disusr();
int _login(const char *user_name);
void _lsfile();
int _chmod(directory &dir, const int has_path, char *args);
void _cd(const char *dir_name, const int has_path, char *args);

//some useful functions
directory init_directory(const char *dir_name, const char *creator);
file init_file(const char *file_name, const int mode, const char *creator);
directory search_dir_by_path(char *path);
vector<file>::iterator search_file_by_name(const char *file_name, directory &dir);
vector<directory>::iterator search_dir_by_name(const char *dir_name, directory &dir);
users_dict::iterator search_user_by_name(const char *user_name, users_dict &users);
void update_workspace(const char *dir_name, const int has_pre_path, const char* pre_path);
void set_modes(file &f, const int mode);


int main(int argc, char *argv[])
{
	init_fs();
	user_iter = search_user_by_name(ROOT_NAME, users);
	if(user_iter != users.end())
	{
		char cmd[MAX_STRING_LENGTH];
		char *cmd_name, *args;
		current_user = *user_iter;
		user_info tmp = NULL;
		current_dir = root_dir;
		while(wait_for_cmd(current_user))
		{
			cin.getline(cmd, MAX_STRING_LENGTH);
			cmd_name = strtok_r(cmd, " ", &args); 
			if(strcmp(cmd_name, "man") == 0)
				_man();
			else if(strcmp(cmd_name, "pwd") == 0)
				_pwd();
			else if(strcmp(cmd_name, "mkdir") == 0)
				_mkdir(current_dir, NO, args);
			else if(strcmp(cmd_name, "mkdirp") == 0)
				_mkdir(current_dir, YES, args);
			else if(strcmp(cmd_name, "dedir") == 0)
				_dedir(current_dir, NO, args);
			else if(strcmp(cmd_name, "dedirp") == 0)
				_dedir(current_dir, YES, args);
			else if(strcmp(cmd_name, "search") == 0)
				_search(args);
			else if(strcmp(cmd_name, "create") == 0)
				_create(current_dir, NO, args);
			else if(strcmp(cmd_name, "createp") == 0)
				_create(current_dir, YES, args);
			else if(strcmp(cmd_name, "cat") == 0)
				_cat(current_dir, NO, args);
			else if(strcmp(cmd_name, "catp") == 0)
				_cat(current_dir, YES, args);
			else if(strcmp(cmd_name, "delete") == 0)
				_delete(current_dir, NO, args);
			else if(strcmp(cmd_name, "deletep") == 0)
				_delete(current_dir, YES, args);
			else if(strcmp(cmd_name, "write") == 0)
				_write(current_dir, NO, args);
			else if(strcmp(cmd_name, "writep") == 0)
				_write(current_dir, YES, args);
			else if(strcmp(cmd_name, "regusr") == 0)
				_regusr(args);
			else if(strcmp(cmd_name, "delusr") == 0)
				_delusr(args);
			else if(strcmp(cmd_name, "disusr") == 0)
				_disusr();
			else if(strcmp(cmd_name, "login") == 0)
				_login(args);
			else if(strcmp(cmd_name, "ls") == 0)
				_ls(current_dir, NO, NULL);
			else if(strcmp(cmd_name, "lsp") == 0)
				_ls(current_dir, YES, args);
			else if(strcmp(cmd_name, "open") == 0)
				_open(current_dir, NO, args, NULL);
			else if(strcmp(cmd_name, "openp") == 0)
				_open(current_dir, YES, args, NULL);
			else if(strcmp(cmd_name, "close") == 0)
				_close(current_dir, NO, args);
			else if(strcmp(cmd_name, "closep") == 0)
				_close(current_dir, YES, args);
			else if(strcmp(cmd_name, "cd") == 0)
				_cd(args, NO, NULL);
			else if(strcmp(cmd_name, "cdp") == 0)
				_cd(args, YES, args);
			else if(strcmp(cmd_name, "lsfile") == 0)
				_lsfile();
			else if(strcmp(cmd_name, "chmod") == 0)
				_chmod(current_dir, NO, args);
			else if(strcmp(cmd_name, "chmodp") == 0)
				_chmod(current_dir, YES, args);
			else if(strcmp(cmd_name, "reposition") == 0)
				_reposition(current_dir, NO, args);
			else if(strcmp(cmd_name, "repositionp") == 0)
				_reposition(current_dir, YES, args);
			else if(strcmp(cmd_name, "truncate") == 0)
				_truncate(current_dir, NO, args);
			else if(strcmp(cmd_name, "truncatep") == 0)
				_truncate(current_dir, YES, args);
			else if(strcmp(cmd_name, "exit") == 0)
				break;
			else
				cout << "command not found" << endl;
		}
	}
	return 0;
}

void init_fs()
{
	//create a root user
	user_info _root = (user_info)malloc(sizeof(struct user_profile));
	strcpy(_root->user_name, ROOT_NAME);
	_root->password = ROOT_PASSWD;
	//push into user list
	users.push_back(_root); 
	//init root directory
	root_dir = init_directory("/", _root->user_name);
	//set default workspace
	strcpy(workspace, DEFAULT_WORKSPACE);
	opened_records.clear();
}

int wait_for_cmd(const user_info &user)
{
	if(user)
	{	//prompt
		cout << user->user_name << HEAD << workspace << TAIL << ":";
		return SUCCEED;
	}
	else
		return FAILED;
}

void _man()
{
	cout << "Manual:" <<endl;
	cout << "0) pwd (current work directory)" <<endl;
	cout << "1) mkdir(p) (path) directory_name" << endl;
	cout << "2) dedir(p) (path) directory_name" << endl;
	cout << "3) search file_name" << endl;
	cout << "4) create(p) (path) file_name mode" << endl;
	cout << "5）cat(p) (path) file_name" << endl;
	cout << "6）delete(p) (path) file_name" << endl;
	cout << "7）write(p) (path) file_name buff wmode" << endl;
	cout << "8）reposition(p) (path) file_name pos" << endl;
	cout << "9）truncate(p) (path) file_name cnt" << endl;
	cout << "10）ls(p) (path)" << endl;
	cout << "11）open(p) (path) file_name" << endl;
	cout << "12）close(p) (path) file_name" << endl;
	cout << "13）regusr user_name password" << endl;
	cout << "14) delusr user_name [root only]" << endl;
	cout << "15）disusr file_name" << endl;
	cout << "16）login user_name" << endl;
	cout << "17）lsfile" << endl;
	cout << "18）chmod(p) (path) file_name mode" << endl;
	cout << "19) cd(p) (path) file_name" << endl;
	cout << "20）exit" << endl;
}

void _pwd()
{	//print working directory
	cout << workspace << endl;
}

int _mkdir(directory &dir, const int has_path, char *args)
{
	if(dir!=NULL)
	{
		if(!has_path)
		{
			if(args == NULL || strcmp(args, "\0") == 0) return FAILED;//directory name cannot be empty
			char *dir_name = args;
			dir_iter = search_dir_by_name(dir_name, dir);
			if(dir_iter == dir->dirs.end())
			{
				directory new_directory = init_directory(dir_name, current_user->user_name);
				dir->dirs.push_back(new_directory);
				++(dir->item_number); 
				return SUCCEED;
			}
			else
			{
				cout << "directory already exists" << endl;
				return FAILED;
			}
		}
		else
		{
			char *path = strtok_r(args, " ", &args); //extract path
			directory target_dir = search_dir_by_path(path);
			if(target_dir == NULL)
				{
					cout << "directory does not exist" << endl;
					return FAILED;
				}
			else
				return _mkdir(target_dir, NO, args);
		}
	}
	return FAILED;
}

int _dedir(directory &dir, const int has_path, char *args)
{
	if(dir!=NULL)
	{
		if(!has_path)
		{
			if(args == NULL || strcmp(args, "\0") == 0) return FAILED;
			char *dir_name = args;
			dir_iter = search_dir_by_name(dir_name, dir);
			if(dir_iter == dir->dirs.end())
			{
				cout << "directory does not exist" << endl;
				return FAILED;
			}
			else
			{	//before deleting, exam the permission
				if(strcmp((*dir_iter)->creator, current_user->user_name) == 0)
				{
					dir_iter = dir->dirs.erase(dir_iter);
					--(dir->item_number); 
					return SUCCEED;
				}
				else
				{
					cout << "permission denied" << endl;
					return FAILED;
				}
			}
		}
		else
		{
			char *path = strtok_r(args, " ", &args);
			directory target_dir = search_dir_by_path(path);
			if(target_dir == NULL)
			{
				cout << "directory does not exist" << endl;
				return FAILED;
			}
			else
				return _dedir(target_dir, NO, args);
		}
	}
	return FAILED;
}

int _search(const char *file_name)
{
	if(file_name == NULL || strcmp(file_name, "\0") == 0) return FAILED;
	//record the whole path
	list<const char *> all_paths;
	all_paths.clear();
	//record the directories waiting to be searched
	list<directory> search_queue;
	search_queue.clear();
	search_queue.push_back(root_dir);
	//record all prarents of the current directory
	list<const char *> all_pre_paths;
	all_pre_paths.clear();
	all_pre_paths.push_back("/"); //push root path to queue
	//current directory
	directory current_dir;
	while(!search_queue.empty() && !all_pre_paths.empty())
	{	
		//pop front frist
		current_dir = search_queue.front();
		search_queue.pop_front();
		//get parent path
		char *tmp = (char *)malloc(sizeof(char)*MAX_STRING_LENGTH);
		strcpy(tmp, all_pre_paths.front());
		all_pre_paths.pop_front();
		//searching
		file_iter = search_file_by_name(file_name, current_dir);
		if(file_iter != current_dir->files.end())
		{	//if found
			char *tmp2 = (char *)malloc(sizeof(char)*MAX_STRING_LENGTH);
			strcpy(tmp2, tmp);
			strcat(tmp2, (*file_iter)->file_name);
			all_paths.push_back(tmp2);
		}
		//use c++11 features for_each with lambda function
		for_each(current_dir->dirs.begin(), current_dir->dirs.end(), \
					[ &](const directory d)
					{ 
						//push directories
						search_queue.push_back(d);
						//push parent paths
						char *tmp3 = (char *)malloc(sizeof(char)*MAX_STRING_LENGTH);
						strcpy(tmp3, tmp);
						strcat(tmp3, d->directory_name);
						strcat(tmp3, "/");
						all_pre_paths.push_back(tmp3); //record parent paths
					}); 
	}
	if(all_paths.empty())
	{	//empty indicates nonexistence
		cout << "file does not found" << endl;
		return FAILED;
	}
	else
	{
		for_each(all_paths.begin(), all_paths.end(), \
					[](const char *path){ cout << path << endl;}); 
		return SUCCEED;
	}
}

int _create(directory &dir, const int has_path, char *args)
{
	if(dir!=NULL)
	{
		if(!has_path)
		{
			if(args == NULL || strcmp(args, "\0") == 0) return FAILED;
			char *file_name = strtok_r(args, " ", &args); //get the file name
			int mode = atoi(args); //obtain the mode
			file_iter = search_file_by_name(file_name, dir);
			if(file_iter == dir->files.end())
			{
				file new_file = init_file(file_name, mode, current_user->user_name);
				dir->files.push_back(new_file);
				++(dir->item_number); 
				return SUCCEED;
			}
			else
			{
				cout << "file already exists" << endl;
				return FAILED;
			}
		}
		else
		{
			char *path = strtok_r(args, " ", &args);
			directory target_dir = search_dir_by_path(path);
			if(target_dir == NULL)
			{
				cout << "directory does not exist" << endl;
				return FAILED;
			}
			else
				return _create(target_dir, NO, args);
		}
	}
	return FAILED;
}

int _cat(directory &dir, const int has_path, char *args)
{
	if(dir != NULL)
	{
		if(!has_path)
		{
			if(args == NULL || strcmp(args, "\0") == 0) return FAILED;
			char *file_name = args;
			file_iter = search_file_by_name(file_name, dir);
			if(file_iter != dir->files.end())
			{
				//before read, exam the status of file and the permission
				if((*file_iter)->modes[0] == READ && (*file_iter)->status == OPENED)
				{	
					cout << (*file_iter)->content << endl;
					return SUCCEED;
				}
				else if((*file_iter)->status == CLOSED)
				{
					cout << "file does not open" << endl;
					return FAILED;
				}
				else if((*file_iter)->modes[0] != READ)
				{
					cout << "permission denied" << endl;
					return FAILED;
				}
				else
					return FAILED;
			}
			else
			{
				cout << "file does not exist" << endl;
				return FAILED;
			}
		}
		else
		{
			char *path = strtok_r(args, " ", &args);
			directory target_dir = search_dir_by_path(path);
			if(target_dir == NULL)
			{
				cout << "directory does not exist" << endl;
				return FAILED;
			}
			else
				return _cat(target_dir, NO, args);
		}
	}
	return FAILED;
}

int _delete(directory &dir, const int has_path, char *args)
{
	if(dir!=NULL)
	{
		if(!has_path)
		{
			if(args == NULL || strcmp(args, "\0") == 0) return FAILED;
			char *file_name = args;
			file_iter = search_file_by_name(file_name, dir);
			if(file_iter == dir->files.end())
			{
				cout << "file dose not exist" << endl;
				return FAILED;
			}
			else
			{
				if(strcmp((*file_iter)->creator, current_user->user_name) == 0)
				{
					file_iter = dir->files.erase(file_iter);
					--(dir->item_number); 
					return SUCCEED;
				}
				else
				{
					cout << "permission denied" << endl;
					return FAILED;
				}
			}
		}
		else
		{
			char *path = strtok_r(args, " ", &args);
			directory target_dir = search_dir_by_path(path);
			if(target_dir == NULL)
			{
				cout << "directory does not exist" << endl;
				return FAILED;
			}
			else
				return _delete(target_dir, NO, args);
		}
	}
	return FAILED;
}

int _write(directory &dir, const int has_path, char *args)
{
	if(dir == NULL) return FAILED;
	if(!has_path)
	{
		if(args == NULL || strcmp(args, "\0") == 0) return FAILED;
		char *file_name = strtok_r(args, " ", &args); //get file_name
		file_iter = search_file_by_name(file_name, dir);
		if(file_iter != dir->files.end())
		{	//before writing, exam the status and the permission
			if((*file_iter)->modes[1] == WRITE && (*file_iter)->status == OPENED)
			{
				char *file_content = strtok_r(args, " ", &args); //get content
				int type = atoi(args); //get operation type
				int length = 0; //record the total length of content after modified
				switch(type)
				{
					case 0:
					{	//append to the end
						if((length = strlen(file_content) + strlen((*file_iter)->content) + 1) > MAX_STRING_LENGTH)
						{
							char *new_content = (char *)malloc(sizeof(char)*length);
							strcpy(new_content, (*file_iter)->content);
							strcat(new_content, file_content);
							free((*file_iter)->content);
							(*file_iter)->content = new_content;
							(*file_iter)->length = length;
						}
						else
							strcat((*file_iter)->content, file_content);
						(*file_iter)->position = length-1;
						break;						
					}
					case 1:
					{	//replace content from the position of the pointer
						if((length = strlen((*file_iter)->content) - ((*file_iter)->position+1) + strlen(file_content) + 1) \
							> MAX_LENGTH)
						{
							char *new_content = (char *)malloc(sizeof(char)*length);
							strcpy(new_content, (*file_iter)->content);
							strncpy(new_content+(*file_iter)->position, file_content, strlen(file_content));
							free((*file_iter)->content);
							(*file_iter)->content = new_content;
							(*file_iter)->length = length;
						}
						else
							strncpy((*file_iter)->content+(*file_iter)->position, file_content, strlen(file_content));
						(*file_iter)->position = length-1;
						break;
					}
					case 2:
					{	//insert before the content from the position of the pointer
						length = strlen(file_content) + strlen((*file_iter)->content) + 1;
						char *new_content = (char *)malloc(sizeof(char)*length);
						strncpy(new_content, (*file_iter)->content, (*file_iter)->position);
						strcat(new_content, file_content);
						strcat(new_content, (*file_iter)->content+(*file_iter)->position);
						(*file_iter)->content = new_content;
						(*file_iter)->position = length-1;
						break;
					}
					default: ;
				}
				cout << "write ok" << endl;
				return SUCCEED;
			}
			else if((*file_iter)->status == CLOSED)
			{
				cout << "file does not open" << endl;
				return FAILED;
			}
			else if((*file_iter)->modes[1] != WRITE)
			{
				cout << "permission denied" << endl;
				return FAILED;
			}
			else
				return FAILED;
		}
		else
		{
			cout << "file does not exist" << endl;
			return FAILED;
		}
	}
	else
	{
		char *path = strtok_r(args, " ", &args);
		directory target_dir = search_dir_by_path(path);
		if(target_dir == NULL)
		{
			cout << "directory does not exist" << endl;
			return FAILED;
		}
		else
			return _write(target_dir, NO, args);
	}
}

int _reposition(directory &dir, const int has_path, char *args)
{
	if(dir!=NULL)
	{
		if(!has_path)
		{
			if(args == NULL || strcmp(args, "\0") == 0) return FAILED;
			char *file_name = strtok_r(args, " ", &args);
			int position = atoi(args); //convert to get the position
			file_iter = search_file_by_name(file_name, dir);
			if(file_iter != dir->files.end())
			{	//position should not be longer than content length
				if(position > (*file_iter)->length)
				{
					cout << "reposition failed" << endl;
					return FAILED;
				}
				else
				{
					(*file_iter)->position = position; 
					cout << "reposition ok" << endl;
					return SUCCEED;
				}
			}
			else
			{
				cout << "file does not exist" << endl;
				return FAILED;
			}
		}
		else
		{
			char *path = strtok_r(args, " ", &args);
			directory target_dir = search_dir_by_path(path);
			if(target_dir == NULL)
			{
				cout << "directory does not exist" << endl;
				return FAILED;
			}
			else
				return _reposition(target_dir, NO, args);
		}
	}
	return FAILED;
}

int _truncate(directory &dir, const int has_path, char *args)
{
	if(dir!=NULL)
	{
		if(!has_path)
		{
			if(args == NULL || strcmp(args, "\0") == 0) return FAILED;
			char *file_name = strtok_r(args, " ", &args);
			int cnt = atoi(args); //get cnt
			file_iter = search_file_by_name(file_name, dir);
			if(file_iter != dir->files.end())
			{
				if(cnt < (*file_iter)->length)
				{
					char *trunc_content = (char *)malloc(sizeof(char)*(*file_iter)->length);
					strncpy(trunc_content, (*file_iter)->content, cnt);
					free((*file_iter)->content);
					(*file_iter)->content = trunc_content;
					(*file_iter)->position = cnt; //point to the end of file
				}
				return SUCCEED;
			}
			else
			{
				cout << "file does not exist" << endl;
				return FAILED;
			}
		}
		else
		{
			char *path = strtok_r(args, " ", &args);
			directory target_dir = search_dir_by_path(path);
			if(target_dir == NULL)
			{
				cout << "directory does not exist" << endl;
				return FAILED;
			}
			else
				return _truncate(target_dir, NO, args);
		}
	}
	return FAILED;
}

void _ls(directory &dir, const int has_path, char *args)
{
	if(dir!=NULL)
	{
		if(dir->item_number != 0)
		{
			if(!has_path)
			{	//print out directories then files
				if(!dir->dirs.empty())
					for_each(dir->dirs.begin(), dir->dirs.end(), \
						[ &](const directory &d){ cout << BOLD_BLUE << d->directory_name << RESET << endl; }); 
				if(!dir->files.empty())
					for_each(dir->files.begin(), dir->files.end(),
						[ &](const file &f){ cout << f->file_name << endl; }); 
			}
			else
			{
				if(args == NULL || strcmp(args, "\0") == 0) return;
				char *path = strtok_r(args, " ", &args);
				directory target_dir = search_dir_by_path(path);
				if(target_dir == NULL)
				{
					cout << "directory does not exist" << endl;
					return;
				}
				else
					_ls(target_dir, NO, NULL);
			}
		}
	}
	return;
}

int _open(directory &dir, const int has_path, char *args, char *path)
{
	if(dir!=NULL)
	{
		if(!has_path)
		{
			if(args == NULL || strcmp(args, "\0") == 0) return FAILED;
			char *file_name = strtok_r(args, " ", &args);
			file_iter = search_file_by_name(file_name, dir);
			if(file_iter != dir->files.end())
			{
				if((*file_iter)->status == OPENED)
				{
					cout << "file already open" << endl;
					return FAILED;
				}
				else
				{	//open the file and add its to opened file record
					(*file_iter)->status = OPENED;
					strcpy((*file_iter)->opener, current_user->user_name); //set opener
					if(path != NULL) 
						opened_records.push_back(make_pair(*file_iter, path));
					else
						opened_records.push_back(make_pair(*file_iter, "/")); //if root directory, add "/"
					return SUCCEED;
				}
			}
			else
			{
				cout << "file does not exist" << endl;
				return FAILED;
			}
		}
		else
		{
			char *path = strtok_r(args, " ", &args);
			//record the path
			char *tmp = (char *)malloc(sizeof(char)*MAX_STRING_LENGTH); 
			strcpy(tmp, path);
			directory target_dir = search_dir_by_path(path);
			if(target_dir == NULL)
			{
				cout << "directory does not exist" << endl;
				return FAILED;
			}
			else
				return _open(target_dir, NO, args, tmp); //pass the path
		}
	}
	return FAILED;
}

int _close(directory &dir, const int has_path, char *args)
{
	if(dir!=NULL)
	{
		if(!has_path)
		{
			if(args == NULL || strcmp(args, "\0") == 0) return FAILED;
			char *file_name = strtok_r(args, " ", &args);
			file_iter = search_file_by_name(file_name, dir);
			if(file_iter != dir->files.end())
			{	//close file and remove it from opened file record
				(*file_iter)->status = CLOSED; 
				opened_iter = find_if(opened_records.begin(), opened_records.end(), \
					[ &](const pair<file, const char *> &p){return p.first == *file_iter;});
				if(opened_iter != opened_records.end()) opened_iter = opened_records.erase(opened_iter);
				return SUCCEED;
			}
			else
			{
				cout << "file does not exist" << endl;
				return FAILED;
			}
		}
		else
		{
			char *path = strtok_r(args, " ", &args);
			directory target_dir = search_dir_by_path(path);
			if(target_dir == NULL)
			{
				cout << "directory does not exist" << endl;
				return FAILED;
			}
			else
				return _close(target_dir, NO, args);
		}
	}
	return FAILED;
}

int _regusr(char *args)
{
	if(args == NULL || strcmp(args, "\0") == 0) return FAILED;
	char *user_name = strtok_r(args, " ", &args);
	user_iter = search_user_by_name(user_name, users);
	if(user_iter == users.end())
	{
		int password;
		password = atoi(args);
		user_info new_user = (user_info)malloc(sizeof(struct user_profile));
		strcpy(new_user->user_name, user_name);
		new_user->password = password;
		users.push_back(new_user); 
		cout << "register ok" << endl;
		return SUCCEED;	
	}
	else
	{
		cout << "user already exists" << endl;
		return FAILED;
	}
}

int _delusr(const char *user_name)
{
	if(user_name == NULL || strcmp(user_name, "\0") == 0) return FAILED;
	//make sure this cmd is root only
	if(strcmp(current_user->user_name, ROOT_NAME) != 0)
	{
		cout << "permission denied" << endl;
		return FAILED;
	}
	user_iter = search_user_by_name(user_name, users);
	if(user_iter != users.end())
	{
		if(strcmp((*user_iter)->user_name, ROOT_NAME) == 0)
		{
			cout << "permission denied" << endl;
			return FAILED;
		}
		else
		{
			user_iter = users.erase(user_iter);
			return SUCCEED;
		}
	}
	else
		cout << "user does not exist" << endl;
	return FAILED;
}

void _disusr()
{
	if(strcmp(current_user->user_name, ROOT_NAME) != 0)
	{
		cout << "permission denied" << endl;
		return;
	}
	for(user_iter = users.begin(); user_iter != users.end(); ++user_iter)
		cout << (*user_iter)->user_name << endl; 
}

int _login(const char *user_name)
{
	if(user_name == NULL || strcmp(user_name, "\0") == 0) return FAILED;
	user_iter = search_user_by_name(user_name, users);
	if(user_iter != users.end())
	{
		int password;
		cout << "password:";
		cin >> password;
		cin.ignore(); //ignore the "\0" left in buffer
		if(password == (*user_iter)->password)
		{	//exam the password
			current_user = *user_iter;
			cout << "login ok" << endl;
		}
		else
		{
			cout << "password wrong" << endl;
			return FAILED;
		}
	}
	else
		cout << "user does not exist" << endl;
	return FAILED;
}

void _lsfile()
{	//print out all opened files and their related information
	for(opened_iter = opened_records.begin(); opened_iter != opened_records.end() ; ++opened_iter)
	{
		cout << (*opened_iter).second << " " << (*opened_iter).first->file_name << " " \
		<< (*opened_iter).first->creator << " " << (*opened_iter).first->opener << " " \
		<< (*opened_iter).first->modes_sum << endl;
	}
}

int _chmod(directory &dir, const int has_path, char *args)
{
	if(dir!=NULL)
	{
		if(!has_path)
		{
			if(args == NULL || strcmp(args, "\0") == 0) return FAILED;
			char *file_name = strtok_r(args, " ", &args);
			int mode = atoi(args);
			file_iter = search_file_by_name(file_name, dir);
			if(file_iter != dir->files.end())
			{	//change the mode
				(*file_iter)->modes_sum = mode;
				set_modes(*file_iter, mode); //call to set the r, w, x
				return SUCCEED;
			}
			else
			{
				cout << "file does not exist" << endl;
				return FAILED;
			}
		}
		else
		{
			char *path = strtok_r(args, " ", &args);
			directory target_dir = search_dir_by_path(path);
			if(target_dir == NULL)
			{
				cout << "directory does not exist" << endl;
				return FAILED;
			}
			else
				return _chmod(target_dir, NO, args);
		}
	}
	return FAILED;
}

void _cd(const char *dir_name, const int has_path, char *args)
{
	if(dir_name == NULL || strcmp(dir_name, "\0") == 0) return;
	if(!has_path)
	{
		dir_iter = search_dir_by_name(dir_name, current_dir);
		if(dir_iter == current_dir->dirs.end())
		{
			cout << "directory does not exist" << endl;
			return;
		}
		else
		{
			current_dir = *dir_iter; //change current working directory
			update_workspace(dir_name, NO, NULL); //update the workspace string
		}
	}
	else
	{
		char *path = args;
		char pre_path[MAX_STRING_LENGTH];
		strcpy(pre_path, path);
		directory target_dir = search_dir_by_path(path);
		if(target_dir == NULL)
		{
			cout << "directory does not exist" << endl;
			return;
		}
		else
		{
			current_dir = target_dir;
			update_workspace(path, YES, pre_path); //recursively
		}
	}
}

directory init_directory(const char *dir_name, const char *creator)
{	//create a new directory and initialize it
	if(dir_name == NULL || strcmp(dir_name, "\0") == 0) return NULL;
	if(creator == NULL || strcmp(creator, "\0") == 0) return NULL;
	directory new_directory = (directory)malloc(sizeof(struct directory_struct));
	strcpy(new_directory->directory_name, dir_name);
	new_directory->item_number = 0;
	new_directory->modes[0] = READ; //default permission of directory is r and w
	new_directory->modes[1] = WRITE;
	new_directory->modes[2] = NONE;
	strcpy(new_directory->creator, creator); 
	new_directory->dirs.clear();
	new_directory->files.clear();
	return new_directory;
}

file init_file(const char *file_name, const int mode, const char *creator)
{	//create a new file and initialize it
	if(file_name == NULL || strcmp(file_name, "\0") == 0) return NULL;
	if(creator == NULL || strcmp(creator, "\0") == 0) return NULL;
	file new_file = (file)malloc(sizeof(struct file_struct));
	strcpy(new_file->file_name, file_name);
	new_file->content = (char *)malloc(sizeof(char)*MAX_LENGTH);
	new_file->length = MAX_LENGTH;
	new_file->position = 0; //set pointer at the beginning
	set_modes(new_file, mode);
	new_file->modes_sum = mode;
	new_file->status = CLOSED; //default status is closed
	strcpy(new_file->creator, creator);
	return new_file;
}

void set_modes(file &f, const int mode)
{
	if(f == NULL) return;
	switch(mode)
	{
		case 0:
		{   //none of r, w, x
			f->modes[0] = NONE;
			f->modes[1] = NONE;
			f->modes[2] = NONE;
			break;
		}
		case 1:
		{ 	//x
			f->modes[0] = NONE;
			f->modes[1] = NONE;
			f->modes[2] = EXECUTABLE;
			break;
		}
		case 2:
		{ 	//w
			f->modes[0] = NONE;
			f->modes[1] = WRITE;
			f->modes[2] = NONE;
			break;
		}
		case 3:
		{ 	//x+w
			f->modes[0] = NONE;
			f->modes[1] = WRITE;
			f->modes[2] = EXECUTABLE;
			break;
		}
		case 4:
		{	//r
			f->modes[0] = READ;
			f->modes[1] = NONE;
			f->modes[2] = NONE;
			break;
		}
		case 5:
		{	//x+r
			f->modes[0] = READ;
			f->modes[1] = NONE;
			f->modes[2] = EXECUTABLE;
			break;
		}
		case 6:
		{	//w+r
			f->modes[0] = READ;
			f->modes[1] = WRITE;
			f->modes[2] = NONE;
			break;
		}
		case 7:
		{
			f->modes[0] = READ;
			f->modes[1] = WRITE;
			f->modes[2] = EXECUTABLE;
			break;
		}
		default: ; 
	}
}

directory search_dir_by_path(char *path)
{
	if(path == NULL || strcmp(path, "\0") == 0) return NULL;
	//search from root_directory
	directory current_dir = root_dir;
	char *subpath = strtok_r(path, "/", &path); //subpath
	//if root path, then rest is NULL
	if(strcmp(path, " ") == 0) return current_dir;
	else
	{
		while(subpath != NULL)
		{
			dir_iter = search_dir_by_name(subpath, current_dir);
			if(dir_iter == current_dir->dirs.end())
				return NULL;
			else
			{
				current_dir = *dir_iter;
				subpath = strtok_r(path, "/", &path);
			}
		}
	}
	return current_dir;
}


vector<file>::iterator search_file_by_name(const char *file_name, directory &dir)
{	//use lambda function
	if(file_name == NULL || strcmp(file_name, "\0") == 0) return dir->files.end();
	return find_if(dir->files.begin(), dir->files.end(), \
		[file_name](const file &f){ return strcmp(f->file_name, file_name) == 0; }); //lambda function
}

vector<directory>::iterator search_dir_by_name(const char *dir_name, directory &dir)
{	//use lambda function
	if(dir_name == NULL || strcmp(dir_name, "\0") == 0) return dir->dirs.end();
	return find_if(dir->dirs.begin(), dir->dirs.end(), \
		[dir_name](const directory &d){return strcmp(d->directory_name, dir_name) == 0;});
}

users_dict::iterator search_user_by_name(const char *user_name, users_dict &users)
{	//use lambda function
	if(user_name == NULL || strcmp(user_name, "\0") == 0) return users.end();
	return find_if(users.begin(), users.end(), \
		[user_name](user_info const& user)->bool{ return strcmp(user->user_name, user_name) == 0;});
}

void update_workspace(const char *dir_name, const int has_pre_path, const char* pre_path)
{	//update the string of workspace
	if(current_dir == NULL || strcmp(dir_name, "\0") == 0) return;
	if(!has_pre_path)
	{
		strcat(workspace, dir_name);
		strcat(workspace, "/");
	}
	else
	{	
		char new_workspace[MAX_STRING_LENGTH];
		strcpy(new_workspace, pre_path);
		strcat(workspace, dir_name);
		strcat(workspace, "/");
		strcpy(workspace, new_workspace);
	}
}