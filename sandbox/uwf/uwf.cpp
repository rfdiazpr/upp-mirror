#include "uwf.h"
#include <wincon.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <uwf/test.iml>
#include <Draw/iml_header.h>

#define IMAGECLASS TestImg
#define IMAGEFILE <uwf/test.iml>
#include <Draw/iml_source.h>

int number;


struct CtrlCExc : Exc {};

bool Exit;

int Port = 8001;

BOOL WINAPI CtrlCHandlerRoutine(__in  DWORD dwCtrlType)
{
	LOG("Ctrl+C handler");
	Exit = true;
	Cout() << "Ctrl + C\n";
	Socket h;
	ClientSocket(h, "127.0.0.1", Port);
	h.Write("quit");
	return TRUE;
}

CONSOLE_APP_MAIN
{
	LOG("About to start");
	SetConsoleCtrlHandler(CtrlCHandlerRoutine, true);
	Socket server;
	if(!ServerSocket(server, Port, true, 5)) {
		Cout() << "Cannot open server socket!\n";
		return;
	}
	Cout() << "Server socket opened\n";
	while(!Exit) {
		Cout() << "Exit: " << Exit << "\n";
		Socket request;
		if(server.Accept(request)) {
			if(Exit)
				break;
			Cout() << " request accepted\n";
			Dispatch(request);
		}
		else
			Cout() << " ...tick...\n";
	}
	LOG("EXIT");
	Cout() << "Exit\n";
}
