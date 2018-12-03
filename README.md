# casch
by Colin Hosking and Jared Asch

Features:
-Forks and executes commands!
-Executes multiple commands separated by ; 
-Redirects using >, >>, <, <<
-Handles single pipes (|)
-Ignores any additional whitespace the user inupts
-Changes working directory using cd
-Prints a funny joke when the user inputs "joke"
-Exits when the user inputs "exit"
  
Attempted:
	History
  Tab completion
Bugs:
	Putting two ;'s next to each other will break the parser
	Redirecting to a file that does not exist occasionally does not work.
	Hair regrowth function may result in a seg fault.
	Occasionally, the unicorn command will fail, opening a great chasm in the earth and summoning the demon Beelzebub, who will proceeded to unleash his wrath and begin his reign of terror.
	
Files & Function Headers:
