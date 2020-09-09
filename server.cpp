#include <bits/stdc++.h>
#include <WinSock2.h>

using namespace std;

#define SIZE 1024
#define IP "127.0.0.1"
#define PORT 8080

WSADATA Winsockdata;
bool flag;
int checker;

SOCKET S_Socket;
SOCKET A_Socket;

struct sockaddr_in S_Add;
struct sockaddr_in C_Add;

char SenderBuffer[SIZE];
char RecvBuffer[SIZE];	

bool no_error(string message){
	if(checker == SOCKET_ERROR){
		cout<<"[-]"<<message<<" Failed. Please try again later...\n";
		cout<<"Error: "<<WSAGetLastError();
		cout<<'\n';
		flag = false;
		return false;
	}
	cout<<"[+]"<<message<<" Succesful.\n";
	return true;
}

void startup(){
	checker = WSAStartup(MAKEWORD(2, 2), &Winsockdata);
	if(checker != 0){
		cout<<"[-]WSAStartup failed"<<endl;
		flag = false;
		return;
	}
	cout<<"[+]WSAStartup connected"<<endl;
}

void display(){
	cout<<"======================SERVER SIDE======================\n";
	cout<<"==================CSE232 Assignment 1==================\n\n";
}

void allocate(){
	S_Add.sin_family = AF_INET;
	S_Add.sin_addr.s_addr = inet_addr(IP);
	S_Add.sin_port = htons(PORT);
}

void socket_create(){
	S_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(S_Socket == INVALID_SOCKET){
		cout<<"[-]TCP Server socket connection failed: "<<WSAGetLastError()<<'\n';
		flag = false;
		return;
	}
	cout<<"[+]TCP Server socket connected"<<endl;
}

void binding(){
	checker = bind(S_Socket, (SOCKADDR*)&S_Add, sizeof(S_Add));
	if(!no_error("Binding")) return;
}

void listening(){
	checker = listen(S_Socket, 2);
	if(!no_error("Listening")) return;
}

void accepting(){
	int val = sizeof(C_Add);
	A_Socket = accept(S_Socket, (SOCKADDR*)&C_Add, &val);
	if(A_Socket == INVALID_SOCKET){
		cout<<"[-]Accepting Failed. Please try again later...\n";
		WSAGetLastError();
		flag = false;
		return;
	}
	cout<<"[+] Accepting Succesful.\n";
}

void socket_close(){
	checker = closesocket(S_Socket);
	if(!no_error("Socket closing")) return;
}

void cleanup(){
	checker = WSACleanup();
	if(!no_error("Cleanup")) return;
}

void send_data(){
	checker = send(A_Socket, SenderBuffer, SIZE, 0);
	if(!no_error("Sending data")) return;
	cout<<"Data sent: "<<SenderBuffer<<endl;
}

void receive_data(){
	checker = recv(A_Socket, RecvBuffer, SIZE, 0);
	if(!no_error("Receiving data")) return;
	cout<<"Data received: "<<RecvBuffer<<endl;
}

void process_file(){
	string filename = "";
	for(int i = 0; i < checker; i++){
		filename += RecvBuffer[i];
	}
	cout<<filename.size()<<'\n';
	fstream file;
	file.open(filename.c_str(), ios::in);
	if(!file){
		cout<<"[-]No file with filename: "<<filename<<'\n';
		flag = false;
		return;
	}
	string word, ans = "";
	while(file >> word){
		ans += word;
		ans += " ";
	}
	strcpy(SenderBuffer, ans.c_str());
	file.close();
}

int main(){
	display();
	flag = true;
	startup();
	if(!flag) return 0;
	allocate();
	socket_create();
	if(!flag) return 0;
	binding();
	if(!flag) return 0;
	listening();
	if(!flag) return 0;
	accepting();
	if(!flag) return 0;
	receive_data();
	if(!flag) return 0;
	process_file();
	if(!flag) return 0;
	send_data();
	if(!flag) return 0;
	socket_close();
	if(!flag) return 0;
	cleanup();
	if(!flag) return 0;
	system("PAUSE");
	return 0;
}