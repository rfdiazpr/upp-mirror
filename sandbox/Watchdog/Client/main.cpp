#ifdef flagMAIN

#include "Client.h"

void Usage(){
	String appname = GetExeTitle();
	Cerr() << "Usage:\n"
	<< "\t" << appname << " --help\n"
	<< "\t\t" << "Prints usage information (this tex)\n"
	<< "\t" << appname << " --get [<max_age>]\n"
	<< "\t\t" << "Returns a list of not yet built revisions, optionally\n"
	<< "\t\t" << "limited to <max_age> days in the past.\n"
	<< "\t" << appname << " --accept <revision> [<start_time>]\n"
	<< "\t\t" << "Notifies server about building <revision>.\n"
	<< "\t" << appname << " --submit <revision> <result> <duration> <output_file> [<start_time> [<end_time>]]\n"
	<< "\t\t" << "Sends results to the server. Optional start and end times can be supplied as int64.\n"
	<< "\t" << appname << " --command <command> [<max_age>]\n"
	<< "\t\t" << "Gets and accepts work automaticaly, then performs <command>\n"
	<< "\t\t" << "with '@revision' substituted by actual revision number\n"
	<< "\t\t" << "and then submits the results to server. Optional parameter\n"
	<< "\t\t" << "<max_age> can be used to restrict the execution to commits\n"
	<< "\t\t" << "<max_age> days old.\n"
	<< "The optional <start_time> and <end_time> parameters can be used to specify when the build took place. "
	   "The value is int64 representation of U++ Time. If not supplied, server will use time of the accept "
	   "request as start and time of submit request as end. \n"
	<< "It is also posible to use short form switches '-h', '-g', '-a', '-s' and '-c'.\n";
}

void Command(const Vector<String>& cmd){
	// ask for work
	WatchdogClient client;
	int max_age = cmd.GetCount()==3?StrInt(cmd[2]):-1;
	if(!client.GetWork(max_age))
		Exit(2);
	
	// select what to do
	if(client.todo.GetCount()==0){
		Cout() << "Nothing to do.\n";
		Exit(0);
	}
	int revision = StrInt(client.todo.Top());
	Cout() << "Revision " << revision << " selected for build\n";
	if(!client.AcceptWork(revision))
		Exit(3);
	
	// replace @revision
	String command = cmd[1];
	command.Replace("@revision", IntStr(revision));
	
	// do work
	Cout() << "Executing command '" << command << "'\n";
	String output;
	Time start = GetSysTime();
	int result = Sys(command, output);
	int duration = GetSysTime()-start;
	
	if(result != 0)
		result = WD_FAILED;
	else
		result = WD_DONE;
	
	// send the results
	Cout() << "Sending results (result='" << status(result) << "', duration=" << duration << ")\n";
	if(!client.SubmitWork(revision, result, duration, output))
		Exit(4);
}

void CheckParamCount(const Vector<String>& cmd, int expected_min, int expected_max=-1){
	if(expected_max < 0) expected_max = expected_min;
	if(cmd.GetCount() < expected_min || cmd.GetCount() > expected_max) {
		Cerr() << "ERROR: Wrong number of parameters for "<<cmd[0]<<"\n";
		Usage();
		Exit(1);
	}
}

CONSOLE_APP_MAIN{
	StdLogSetup(LOG_FILE | LOG_COUT);
	SetDateFormat("%1:4d/%2:02d/%3:02d");
	SetDateScan("ymd");
	
	String cfg = GetDataFile("Client.ini");
	if(!FileExists(cfg)){
		RLOG("Configuration file '" << cfg << "' not found");
		Exit(5);
	}
	SetIniFile(cfg);
	LOG(GetIniInfoFormatted());
	
	const Vector<String>& cmd = CommandLine();
	if(cmd.GetCount() < 1) {
		Usage();
		Exit(1);
	}
	if(cmd[0] == "-h" || cmd[0] == "--help"){
		Usage();
	} else if(cmd[0] == "-g" || cmd[0] == "--get") {
		CheckParamCount(cmd, 1, 2);
		WatchdogClient client;
		int max_age = cmd.GetCount()==2?StrInt(cmd[1]):-1;
		if(!client.GetWork(max_age))
			Exit(2);
		if(client.todo.IsEmpty()){
			RLOG("Nothing to do");
			Exit(0);
		}
		for(int i = 0; i < client.todo.GetCount(); i++){
			Cout() << client.todo[i] << "\n";
		}
	} else if(cmd[0] == "-a" || cmd[0] == "--accept") {
		CheckParamCount(cmd, 2, 3);
		int rev = StrInt(cmd[1]);
		if (rev <= 0){
			Cerr() << "ERROR: Wrong value of <revision>\n";
			Exit(1);
		}
		Time start;
		if(cmd.GetCount()==3){
			start = ScanTime(cmd[2]);
		}
		WatchdogClient client;
		if(!client.AcceptWork(rev, start))
			Exit(3);
	} else if(cmd[0] == "-s" || cmd[0] == "--submit") {
		CheckParamCount(cmd, 5, 7);
		int rev = StrInt(cmd[1]);
		if (rev <= 0){
			Cerr() << "ERROR: Wrong value of <revision>\n";
			Exit(1);
		}
		int result = StrInt(cmd[2]);
		if (result < WD_READY || result > WD_DONE){
			Cerr() << "ERROR: Wrong value of <result>\n";
			Exit(1);
		}
		int duration = StrInt(cmd[3]);
		if (duration < 0){
			Cerr() << "ERROR: Wrong value of <duration>\n";
			Exit(1);
		}
		String output = LoadFile(cmd[4]);
		if (output.IsVoid()){
			Cerr() << "ERROR: Can't load file '"<<cmd[4]<<"'\n";
			Exit(4);
		}
		WatchdogClient client;
		client.SubmitWork(rev, result, duration, output);
	} else if(cmd[0] == "-c" || cmd[0] == "--command") {
		CheckParamCount(cmd, 2, 3);
		Command(cmd);
	} else {
		Usage();
		Exit(1); 
	}
}

#endif
