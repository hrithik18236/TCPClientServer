#include<bits/stdc++.h>
#include<WinSock2.h>

using namespace std;

#define SIZE 1024
#define IP "127.0.0.1"
#define PORT 8080

WSADATA Winsockdata;
bool flag;
int checker;

SOCKET C_Socket;

struct sockaddr_in S_Add;

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
	cout<<"======================CLIENT SIDE======================\n";
	cout<<"==================CSE232 Assignment 1==================\n\n";
}

void allocate(){
	S_Add.sin_family = AF_INET;
	S_Add.sin_addr.s_addr = inet_addr(IP);
	S_Add.sin_port = htons(PORT);
}

void socket_create(){
	C_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(C_Socket == INVALID_SOCKET){
		cout<<"[-]TCP Client socket connection failed: "<<WSAGetLastError()<<endl;
		flag = false;
		return;
	}
	cout<<"[+]TCP Client socket connected"<<endl;
}

void connect(){
	checker = connect(C_Socket, (SOCKADDR*)&S_Add, sizeof(S_Add));
	if(!no_error("Connection")) return;
}

void socket_close(){
	checker = closesocket(C_Socket);
	if(!no_error("Socket closing")) return;
}

void cleanup(){
	checker = WSACleanup();
	if(!no_error("Cleanup")){
		flag = false;
		return;
	}
}

void send_data(){
	checker = send(C_Socket, SenderBuffer, SIZE, 0);
	if(!no_error("Sending data")) return;
	cout<<"Data sent: "<<SenderBuffer<<endl;
}

void receive_data(){
	checker = recv(C_Socket, RecvBuffer, SIZE, 0);
	if(!no_error("Receiving data")) return;
	cout<<"[+]Data received. \nFilename: "<<SenderBuffer<<'\n';
	// cout<<"Data: \n"<<RecvBuffer<<endl;
}

void save_file(){
	fstream file;
	string filename = "";
	int index = 0;
	for(int i = 0; i < SIZE; i++){
		if(SenderBuffer[i] == ' ' && SenderBuffer[i + 1] == ' '){
			index = i;
			break;
		}
		filename += SenderBuffer[i];
	}
	file.open(filename.c_str(), ios::out);
	for(int i = 0; i < SIZE; i++){
		file.put(RecvBuffer[i]);
	}
	file.close();
	cout<<"[+]File saved succesfully in current local directory.\n";
}

int main(){
	display();
	flag = true;
	startup();
	if(!flag) return 0;
	allocate();
	socket_create();
	if(!flag) return 0;
	connect();
	if(!flag) return 0;
	cout<<"Enter the name of the file you wish to send to server: \n";
	cin.getline(SenderBuffer, SIZE);
	send_data();
	if(!flag) return 0;
	receive_data();
	if(!flag) return 0;
	socket_close();
	if(!flag) return 0;
	save_file();
	cleanup();
	if(!flag) return 0;
	system("PAUSE");
	return 0;
}