#include<iostream>          
#include<string>
#include<vector>
#include<unistd.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>

using namespace std;

/// file_descriptor:
//  0-> read
//  1-> write
//  2-> error


vector<string> tokenizeCommand(string str, char delim)
{
    vector<string> strTokens;
    string tmp = "";
    bool bgFlag = false;

    for (int i = 0; i < str.length(); i++) {

        if (str[i] == delim)
        {
            strTokens.push_back(tmp);
            tmp = "";
        }
        else if (str[i] == '&')
        {
            strTokens.push_back(tmp);
            strTokens.push_back("bgRunTriggered"); //as a reference for toggling bgRunFlag in main()
            return strTokens;  //return from func because last token already pushed so no need to execute on more push_back at last
        }
        else if (str[i] == '\'')
        {
            //do nothing!
        }
        else {
            tmp += str[i];
        }
    }
    strTokens.push_back(tmp);   //for pushing last token because it doesn't has space after it
    return strTokens;
}


vector<string> separateCommands(string str, char delim)
{
    vector<string> strTokens;
    string tmp = "";

    bool foundPipe=false;

    for (int i = 0; i < str.length(); i++) {

        if(foundPipe == true)   //basically avoiding the space just after |
        {
            //no nothing
            foundPipe=false;
        }
        else if (str[i] == delim)
        {
            strTokens.push_back(tmp);
            tmp = "";
            foundPipe=true;
        }
        else if (str[i] == ' ' && str[i+1]==delim)
        {
            //do nothing!
        }
        else {
            tmp += str[i];
        }
    }
    strTokens.push_back(tmp);   //for pushing last token because it doesn't has space after it
    return strTokens;
}


void shiftAndPop(vector<string>& vec) {
    if (vec.empty()) {
        return;
    }

    // Shift elements to lower indices
    for (size_t i = 0; i < vec.size() - 1; ++i) {
        vec[i] = vec[i + 1];
    }

    vec.pop_back();
}

int main()
{
    string cmd;
    pid_t ret_Val;
    int childCount=0;
    bool pipesFlg=0;
    vector<string> history;
    int historyCommandsCounter=0;


    while (true)
    {
        cout << "\n( Enter Command ) : ";
        getline(cin, cmd);
        if (cmd == "exit")
            break;

		if(cmd=="history")
		{
		int srNo=history.size();
		cout<<"\n( Total Commands entered: "<< historyCommandsCounter << " )\n";
		cout<<"( Total history size: 10" << " )\n";
		cout<<"( Current history size: "<< history.size() << " )\n";
		
		cout<<"\nShowing HISTORY...\n";
		
		for(int i=history.size()-1;i>=0;i--)
		{
		cout << srNo-- << "-> " << history[i]<<endl;		
		}
		continue;  //leave the rest of the loop
		}
		if(cmd=="!!")
		{
			if(history.size()==0){
				cout<<"history is empty\n";
				continue;
			}
			cmd=history.back();
			cout<<"most recent command was " <<" "<<cmd<<endl<<endl;
		}
		
		if(cmd[0]=='!' && cmd[1]!='!' )
		{
		
			string numStr="";
			int index=0;
			for(int i=1;i<cmd.length();i++)
			{
			numStr[index++]=cmd[i];
			}
			int no=stoi(numStr);
			//no = static_cast<int>(cmd[1]) - '0'; // it can work on only one digit so used different approach above..			
			
			if(no > history.size() || no == 0)  
			{
			cout<<" command - "<<no<<" not found is history! (//ERROR_404)\n";
			continue;
			}
			
			cout<<"no "<<no<<" command will be going to execute...\n\n";
			cmd=history[no-1];
		}
		
	historyCommandsCounter++;
	if(historyCommandsCounter>10)   
	{
        shiftAndPop(history);
        history.push_back(cmd); 
        }
        else
        {
        history.push_back(cmd);
        }
        
        

        vector<string> commands = separateCommands(cmd, '|');
        //for testing the separation of chunks of related commands
        // cout<<"\n\nseparating each chunk of related commands:\n\n";
        // for (size_t i = 0; i < commands.size(); i++) 
        // {
        //     cout<<commands[i]<<"(finished)"<<endl;
        // }


        // //for tokenizing the individual commands   
        // cout<<"\n\nnow tokenizing each chunk of related commands:\n\n";
        // for (size_t i = 0; i < commands.size(); i++) 
        // {
        //     vector<string> tokens = tokenizeCommand(commands[i], ' ');
        //     for (size_t i = 0; i < tokens.size(); i++) 
        //     {
        //     cout<<tokens[i]<<"(finished)"<<endl;
        //     }

        //     cout<<endl;
        // }

		if(commands.size()>1)
		{
		pipesFlg=true;
		}
		int prev_pipe[2];
		int next_pipe[2];   //can also say as curr_pipe 
		

    	for (size_t i = 0; i < commands.size(); i++) 
    	{

        vector<string> tokens = tokenizeCommand(commands[i], ' ');

        
        bool bgRunFlag = false;     
        if (tokens.back() == "bgRunTriggered") 
        {
            bgRunFlag = true;
            tokens.pop_back();
        }   

		
        if (tokens[0] == "cd")
        {
                if (tokens.size() < 2)
                {
                     cout << "Usage: cd <directory>" << endl;
                }
                else
                {
                    if (chdir(tokens[1].c_str()) != 0)
                    {
                        perror("chdir failed");
                    }
                }
                 continue; // Skip the rest of the loop and start over
        }
                
        if(tokens[0]=="mkfifo")
        {
            if(tokens.size()<2)
            {
                cout << "Usage: mkfifo <filename> " << endl;
            }
            else
            {
                if(mkfifo(tokens[1].c_str(), 0666) != 0)  //if it does not return successful creation status (0)
                {
                    perror("mkfifo failed!\n");
                }
                else{
                    cout << "named pipe : " << tokens[1] << " created successfully!\n";
                }
            }
            continue;
        }
            
		if(i<commands.size()-1 && pipesFlg && pipe(next_pipe)==-1)  //pipe creation failed in any command other than last command //in last command we donot make pipe
		{
			perror("pipe creation failed!\n");
			exit(EXIT_FAILURE);
		}

            
        childCount++;
        ret_Val = fork();

        if (ret_Val < 0) {
            cerr << "\nFork() failed!\n";
        }
        else if (ret_Val == 0)
        {//child
            cout << "\nchild: "<<getpid()<<"\n";

        //the erase method in c++ having range flavor of paramters  deletes the starting range element
        //but doesn't deletes the ending range element and obviously it deletes the mid elements also 
        
        for(int i=0;i<tokens.size();i++)

        {
            if(tokens[i]=="<") //we can also implement 0< here because they are exactly the same thing

            {
                int fd_read=open(tokens[i+1].c_str(), O_RDONLY);
                dup2(fd_read, 0);
                close(fd_read);
                tokens.erase(tokens.begin()+i, tokens.begin()+i+2);  //removing < and filename
                // it will delete token[i] and token[i+1] but not token[i+2]
                break;
            }
        }

        for(int i=0;i<tokens.size();i++)
        {
            if(tokens[i]==">"){  //we can also implement 1> here because they are exactly the same thing
                int fd_write=open(tokens[i+1].c_str(), O_WRONLY | O_CREAT | O_TRUNC);  //we can also do O_APPEND
                dup2(fd_write, 1);
                close(fd_write);
                tokens.erase(tokens.begin()+i, tokens.begin()+i+2);  //removing > and filename
                // it will delete token[i] and token[i+1] but not token[i+2]
                break;
            }
        }

        for(int i=0;i<tokens.size();i++)
        {
            if(tokens[i]=="2>"){   //this has no alternative..
                int fd_error=open(tokens[i+1].c_str(), O_WRONLY | O_CREAT | O_APPEND);  //we can also do O_APPEND
                dup2(fd_error, 2);
                close(fd_error);
                tokens.erase(tokens.begin()+i, tokens.begin()+i+2);  //removing > and filename
                // it will delete token[i] and token[i+1] but not token[i+2]
                break;
            }
        }

		

        ////for testing..
        //cout<<"\n\nprinting command tokens for testing after redirection(removal of < > etc):\n";
        //for (string s : tokens)    
        //  cout << s << endl;

		if(pipesFlg)
		{

		if(i<commands.size()-1)  //if current command is not the last command
		{

		dup2(next_pipe[1], STDOUT_FILENO);   // Redirect stdout to write end of pipe
		}
		if(i>0)  //if current command is not the first command
		{
			dup2(prev_pipe[0], STDIN_FILENO);   // Redirect stdin to read end of previous pipe
		}

		close(prev_pipe[0]);     // Close read end of previous pipe
		close(prev_pipe[1]);	 // Close write end of previous pipe
		close(next_pipe[0]);  // Close read end of next pipe
		close(next_pipe[1]);  // Close write end of next pipe
		
		}


        char* args[tokens.size() + 1];
        for (size_t i = 0; i < tokens.size(); i++)
            args[i] = const_cast<char*>(tokens[i].c_str());
        args[tokens.size()] = NULL;


        if(execvp(args[0], args)==-1)
        {
            cout<<"\nCOMMAND NOT IDENTIFIED BY SHELL!\n";
			exit(EXIT_FAILURE);
			//OR exit(1);
        }

        cout<<endl;
        exit(0);
        }
        else
        {//parent
            cout<<"\n_____________________________________\n";
            cout << "\nparent: "<<getpid()<<"\n";
	
			if(pipesFlg)
			{
				if(i<commands.size()-1)
				{	
					close(next_pipe[1]); //close read end of prev pipe
				}
				if(i>0)
				{
					close(prev_pipe[0]); //close read end of prev pipe
				}

				// updating the prev_pipe array with the file descriptors of the next pipe.
				// this is done to prepare for the next iteration of the loop, where
				// the current pipe will become the previous pipe for the next command.
				// basically we are shifting the prev_pipe ptr's for visual understanding..
				prev_pipe[0]=next_pipe[0];
				prev_pipe[1]=next_pipe[1];

			}



            if(!bgRunFlag)
            {
                while(childCount>0)
                {
                int status;
                pid_t c_pid = wait(&status);    //wait for child to terminate...
                if(status==0)
                cout<<"child with pid "<<c_pid<<" terminated!\n";
                childCount--;
                }
            }
            else
            {
                cout<<"\nchild running in background!\n";
            }

        }

      }
    }

    cout << "\n***** shell terminated! *****\n";
    cout << "\n";
    //system("pause");
    return 0;
}





