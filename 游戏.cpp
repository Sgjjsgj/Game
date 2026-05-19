/*
报错:
[Error] 'mt19937' was not declared in this scope
[工具] -> [编译选项] -> [编译时加入以下命令:] -> 输入 -std=c++11 
[工具] -> [编译选项] -> [在连接器命令行加入以下命令:] -> 在最后加入 -lstdc++fs
*/
#ifndef _WIN32
#error "This project can only run on Windows systems!"
#endif
#include<windows.h>
#include<mmsystem.h>
#include<filesystem>
#include<algorithm> 
#include<iostream>
#include<fstream> 
#include<conio.h>
#include<cstdlib>
#include<cstring>
#include<string>
#include<limits>
#include<random> 
#include<chrono>
#include<cstdio> 
#include<thread>
#include<ctime>
#include<set>
#include "graphics.h"
#include "json.hpp"
#define C (__FILE__[0] == 'C' ? true : false)
#pragma comment(lib,"Winmm.lib") 
using json = nlohmann::json; 
using namespace std;
namespace fs = std::filesystem;
mt19937& rnd(){
	static mt19937 rng(time(nullptr));
	return rng;
}void clear(){
	system("cls");
}void flush(){
	while(_kbhit()){
		_getch();
	}
	cin.ignore(numeric_limits<streamsize>::max(),'\n');
}
struct{
	string cnm;
	string enm;
	short id;
} ach[] = {
	{"小试牛刀","Getting Started",1},
	{"游戏达人","Game Master",2},
	{"富豪","Tycoon",3},
	{"全能选手","All-Rounded",4},
	{"收藏家","Collector",5},
};
int main(){ 
	system("title 游戏 Games");
	long long sc = 0;
	if(C){
		system("mkdir D:/logs 2>nul");
		system("mkdir D:/users 2>nul");
	}else{
		system("mkdir logs 2>nul");
		system("mkdir users 2>nul");
	}
	string time(__TIME__);
	replace(time.begin(),time.end(),':','-');
	string path = (C ? "D:/logs/GameLog " + string(__DATE__) + " " + time + ".log" : "logs/GameLog " + string(__DATE__) + " " + time + ".log");
	string acc = "",psw = "";
	json data;
	char id;
	string bp = (__FILE__[0] == 'C' ? "D:/users" : "./users");
	while(true){
		cout << "请选择注册/登录账户名（只能有字母/数字/下划线）: " << endl << "Please choose register/login account name(only letters/numbers/underscore): " << endl
		 << "1.注册" << endl << "1.Register" << endl
		 << "2.登录" << endl << "2.Login" << endl;
		cin >> id;
		if(id == '1'){
			while(true){
				cout << "请输入账户名: " << endl << "Please enter the account name: " << endl;
				cin >> acc;
				bool vld = true;
				for(unsigned char c : acc){
					if(c > 0x80){
						cerr << "不能含有中文！" << endl << "Chinese characters are not allowed!" << endl;
						vld = false;
						break;
					}
				}if(!vld){
					continue;
				}if(fs::is_empty(bp)){
					cout << "请输入新密码: " << endl << "Please enter the new password: " << endl;
					cin >> psw;
					data["user"] = acc;
					for(char& c : psw){
						c ^= 0x5A;
					}
					data["password"] = psw;
					data["score"] = 0;
					ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4);
					break;
				}else{
					bool flag = false;
					for(const auto& i : fs::directory_iterator(bp)){
						ifstream fin(i.path());
						json tmp;
						fin >> tmp;
						if(tmp["user"] == acc){
							cerr << "该账户已被注册过了！" << endl << "This account has already been registered!" << endl;
							flag = true;
							break;
						}
					}if(flag){
						continue;
					}else{
						cout << "请输入新密码: " << endl << "Please enter the new password: " << endl;
						cin >> psw;
						data["user"] = acc;
						for(char& c : psw){
							c ^= 0x5A;
						}
						data["password"] = psw;
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4);
						break;
					}
				}
			}
			continue;
		}else if(id == '2'){
			while(true){
				cout << "请输入账户名: " << endl << "Please enter the account name: " << endl;
				cin >> acc;
				if(fs::is_empty(bp)){
					cerr << "账户名错误！" << endl << "Invalid account name!" << endl;
					continue;
				}else{
					cout << "请输入密码: " << endl << "Please enter the password: " << endl;
					cin >> psw;
					bool flag = false;
					for(const auto& i : fs::directory_iterator(bp)){
						ifstream fin(i.path());
						json tmp;
						fin >> tmp;
						string str = tmp["password"];
						for(char& c : str){
							c ^= 0x5A;
						}
						if(tmp["user"] == acc && str == psw){
							cout << "登录成功" << endl << "Login successful" << endl;
							sc = tmp["score"];
							data = tmp; 
							flag = true;
							this_thread::sleep_for(chrono::seconds(2));
							break;
						}
					}if(flag){
						break;
					}else{
						cerr << "账户名或密码错误！" << endl << "Invalid account name or password!" << endl;
						continue;
					}
				}
			}
			break;
		}else{
			cerr << "编号输入错误！" << endl << "Invalid ID input!" << endl;
			continue;
		}
	}if(!(data.contains("achievement"))){
		data["achievement"] = json::object();
		for(short i = 1;i <= 5;i++){
			data["achievement"][to_string(i)] = false;
		}
	}
	ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
	clear();
	bool f[6] = {false,false,false,false,false,false};
	while(true){
		if(sc >= 100 && sc < 1000){
			data["achievement"]["1"] = true;
		}if(sc >= 1000 && sc < 5000){
			data["achievement"]["2"] = true;
		}if(sc >= 5000){
			data["achievement"]["3"] = true;
		}
		bool flag = true;
		for(short i = 1;i <= 4;i++){
			if(data["achievement"][to_string(i)] == false){
				flag = false;
				break;
			}
		}if(flag){
			data["achievement"]["5"] = true;
		}else{
			data["achievement"]["5"] = false;
		}
//		char id = '\0';
		ofstream file(path,ios_base::app);
		file << "[0]";
		cout << "分数: " << sc << endl << "Score: " << sc << endl
			 << "游戏列表: " << endl << "Games list: " << endl
			 << "1.猜数字" << endl << "1.Guess the number" << endl
			 << "2.躲避障碍" << endl << "2.Dodge obstacles" << endl
			 << "3.石头剪刀布" << endl << "3.Rock paper scissors" << endl
			 << "4.计算挑战" << endl << "4.Computational challenge" << endl
			 << "5.打字游戏" << endl << "5.Typing game" << endl
			 << "6.贪吃蛇" << endl << "6.Snake game" << endl
			 << "D: 删除账号" << endl << "D: Delete account" << endl 
			 << "S: 商城" << endl << "S: Shop" << endl
			 << "R: 排行榜" << endl << "R: Ranking" << endl
			 << "A: 成就" << endl << "A: Achievement" << endl
			 << "E: 退出" << endl << "E: Exit" << endl
			 << "注：每个游戏（编号1~6）按Esc都可以退出" << endl << "Note: Pressing Esc can exit each game (ID 1~6)" << endl
			 << endl << "请输入游戏编号: " << endl << "Please enter the game ID: " << endl;
		if(!(cin >> id)){
			file << " [-1]";
			cerr << "游戏编号输入错误！" << endl << "Invalid game ID entered!" << endl;
			cin.clear();
			flush();
			continue;
		}if(id == '1'){
			if(!flag){
				f[0] = true;
			}
			bool again = true;
			bool escPressed = false;
			while(true){
				clear();
				long long min = 0;
				while(true){
					if(_kbhit() && _getch() == 27){
						escPressed = true;
						break;
					}
					cout << "请输入最小范围: " << endl << "Please enter the minimum range: " << endl;
					if(!(cin >> min)){
						file << " [" << id << " 1 -1]";
						cerr << "最小范围输入错误！" << endl << "Invalid minimum range input!" << endl;
						cin.clear();
                        flush();
						continue;
					}
					sc++;
					data["score"] = sc;
					ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
					break;
				}
				if(escPressed) break;
				long long max = 0;
				while(true){
					if(_kbhit() && _getch() == 27){
						escPressed = true;
						break;
					}
					cout << "请输入最大范围: " << endl << "Please enter the maximum range: " << endl;
					if(!(cin >> max)){
						file << " [" << id << " 2 -1]";
						cerr << "最大范围输入错误！" << endl << "Invalid maximum range input!" << endl;
						cin.clear();
                        flush();
						continue;
					}else{
						if(max < min){
							file << " [" << id << " 2 -1]";
							cerr << "最大范围必须大于或等于最小范围！" << endl << "Maximum range must be greater than minimum range!" << endl;
							cin.clear();
                        	flush();
							continue;
						}else{
							sc++;
							data["score"] = sc;
							ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
							break;
						}
					}
				}
				if(escPressed) break;
				uniform_real_distribution<double> dist(static_cast<double>(min),static_cast<double>(max));
				long long awr = dist(rnd());
				while(true){
					if(_kbhit() && _getch() == 27){
						escPressed = true;
						break;
					}
					long long ges = 0;
					cout << "请输入您猜的数: " << endl << "Please enter your guessed number: " << endl;
					if(!(cin >> ges)){
						file << " [" << id << " 3 -1]";
						cerr << "数字输入错误！" << endl << "Invalid number input!" << endl;
						continue;
					}else if(ges == awr){
						file << " [" << id << " 3 1]";
						cout << "恭喜您猜对了！" << endl << "Congratulations!You guessed it right!" << endl;
						sc += 5;
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						break;
					}else if(ges < awr){
						file << " [" << id << " 3 0]";
						cout << "猜小了！" << endl << "Too low!" << endl;
						sc++; 
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						continue;
					}else if(ges > awr){
						file << " [" << id << " 3 2]";
						cout << "猜大了！" << endl << "Too high!" << endl;
						sc++;
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						continue; 
					}
                    flush();
				}
				if(escPressed) break;
				while(true){
					if(_kbhit() && _getch() == 27){
						escPressed = true;
						break;
					}
					char ch = '\0';
					cout << "是否再玩一次？(Y/N): " << endl << "Play again? (Y/N): " << endl;
					cin >> ch;
					flush();
					clear();
					file << " [" << id << " 4 ";
					if(ch != 'n' && ch != 'N' && ch != 'y' && ch != 'Y'){
						file << "-1] [" << id << " 4 ";
						cerr << "操作输入错误！" << endl << "Invalid input operation!" << endl;
						continue;
					}else{
						if(ch == 'n' || ch == 'N'){
							again = true;
							break;
						}else if(ch == 'y' || ch == 'Y'){
							again = false;
							break;
						}
					}
				}
				if(escPressed) break;
				file << again << ']';
				if(again){
					break;
				}else{
					continue;
				}
			}
			if(escPressed) continue;
		}else if(id == '2'){
			if(!flag){
				f[1] = true;
			}
			clear();
			bool exit = false;
			bool escPressed = false;
			int sp = 1;
			while(true){
				uniform_int_distribution<short> dist(1,9);
				int num1 = dist(rnd()),n = 0,y = 1,num2 = dist(rnd()),num3 = dist(rnd());
				while(num2 == num1){
					num2 = dist(rnd());
				}while(num3 == num1 || num3 == num2){
					num3 = dist(rnd());
				}while(n < 5){
					cout << "按下[E]退出，按下[A]向左，按下[D]向右，|为自己" << endl << "Press [E] to exit,press [A] to move left,press [D] to move right,| is you" << endl;
					if(kbhit()){
						char key = getch();
						if(key == 'E' || key == 'e'){
							file << " [" << id << ' ' << int('E') << ']';
							exit = true;
						}if(key == 'A' || key == 'a'){
							file << " [" << id << ' ' << int('A') << ']';
							if(sp > 1){
								sp--;
							}
							sc += 5;
							data["score"] = sc;
							ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						}if(key == 'D' || key == 'd'){
							file << " [" << id << ' ' << int('D') << ']';
							if(sp < 9){
								sp++; 
							}
							sc += 5;
							data["score"] = sc;
							ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						}
						if(key == 27){ // ESC
							escPressed = true;
							exit = true;
							break;
						}
					}
					if(escPressed) break;
					if(((num1 == sp || num2 == sp || num3 == sp) && y >= 5) || exit){
						while(true){
							char ch = '\0';
							cout << "是否再玩一次？(Y/N): " << endl << "Play again? (Y/N): " << endl;
							cin >> ch;
							flush();
							clear();
							if(ch != 'n' && ch != 'N' && ch != 'y' && ch != 'Y'){
								file << " [" << id << " -1]";
								cerr << "操作输入错误！" << endl << "Invalid input operation!" << endl;
								continue;
							}else{
								if(ch == 'n' || ch == 'N'){
									exit = true;
									break;
								}else if(ch == 'y' || ch == 'Y'){
									exit = false;
									break;
								}
							}
						}
						break;
					}
					for(int a = n;a >= 0;a--){
						cout << endl;
					}for(int i = 1;i <= 15;i++){
						if(i == num1 || i == num2 || i == num3){
							cout << 'v';
						}else{
							cout << ' ';
						}
					}for(int l = 0;l < 5-n;l++){
						cout << endl;
					}for(int i = 1;i <= 15;i++){
						if(i == sp){
							cout << '|';
						}else{
							cout << ' ';
						}
					}
					cout << endl;
					for(int m = 0;m <= 15;m++){
						cout << '_';
					}
					n++;
					y++;
					this_thread::sleep_for(chrono::milliseconds(250));
					clear();
				}
				if(escPressed) break;
				file << " [" << id << ' ' << exit << ']';
				if(exit){
					break;
				}else{
					continue;
				}
			}
			if(escPressed) continue;
		}else if(id == '3'){
			if(!flag){
				f[2] = true;
			}
			clear();
			string ce1[3] = {"石头","剪刀","布"},ce2[3] = {"Rock","Scissors","Paper"};
			bool escPressed = false;
			while(true){
				if(_kbhit() && _getch() == 27){
					escPressed = true;
					break;
				}
				cout << "石头剪刀布" << endl << "Rock paper scissors" << endl
					 << "0.退出" << endl << "0.Exit" << endl
					 << "1.石头" << endl << "1.Rock" << endl
					 << "2.剪刀" << endl << "2.Scissors" << endl
					 << "3.布" << endl << "3.Rock" << endl;
				short code,n = 0;
				string player,com;
				cout << "请输入您选择的编号: " << endl << "Please enter the number of your choice: " << endl;
				cin >> code;
				flush();
				if(!code){
					file << " [" << id << " 1 0]";
					clear();
					break;
				}else if(code == 1){
					player = "石头";
					sc += 5;
					data["score"] = sc;
					ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
					n = 0;
					file << " [" << id << " 1 1]";
				}else if(code == 2){
					player = "剪刀";
					sc += 5;
					data["score"] = sc;
					ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
					n = 1;
					file << " [" << id << " 1 2]";
				}else if(code == 3){
					player = "布";
					sc += 5;
					data["score"] = sc;
					ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
					n = 2;
					file << " [" << id << " 1 3]";
				}else{
					file << " [" << id << " 1 -1]";
					cerr << "编号输入错误！" << endl << "Invalid input ID!" << endl;
					sc++;
					data["score"] = sc;
					ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
					continue; 
				}
				uniform_int_distribution<short> dist(0,2);
				short choi = dist(rnd());
				com = ce1[choi];
				cout << "你选择了: " << ce1[n] << endl
					 << "Your choice: " << ce2[n] << endl
					 << "电脑选择了: " << com << endl
					 << "Computer's choice: " << ce2[choi] << endl;
				if(n == choi){
					cout << "平局" << endl
						 << "Tie" << endl;
					file << " [" << id << " 2 0]"; 
				}else if((n == 0 && choi == 1) || (n == 1 && choi == 2) || (n == 2 && choi == 0)){
					cout << "玩家赢了" << endl
						 << "Player won" << endl;
					file << " [" << id << " 2 2]";
				}else{
					cout << "电脑赢了" << endl
						 << "Computer won" << endl;
					file << " [" << id << " 2 1]";
				}
				sc += 5;
				data["score"] = sc;
				ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
				bool again;
				while(true){
					if(_kbhit() && _getch() == 27){
						escPressed = true;
						break;
					}
					char ch = '\0';
					cout << "是否再玩一次？(Y/N): " << endl << "Play again? (Y/N): " << endl;
					cin >> ch;
					flush();
					clear();
					if(ch != 'n' && ch != 'N' && ch != 'y' && ch != 'Y'){
						file << " [" << id << " 3 -1]";
						sc++;
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						cerr << "操作输入错误！" << endl << "Invalid input operation!" << endl;
						continue;
					}else{
						if(ch == 'n' || ch == 'N'){
							again = false;
							break;
						}else if(ch == 'y' || ch == 'Y'){
							again = true;
							break;
						}
						sc += 5; 
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						file << " [" << id << " 3 " << !again << ']';
					}
				}
				if(escPressed) break;
				clear();
				if(again){
					continue;
				}else{
					break;
				}
			}
			if(escPressed) continue;
		}else if(id == '4'){
			if(!flag){
				f[3] = true;
			}
			clear();
			bool escPressed = false;
			while(true){
				if(_kbhit() && _getch() == 27){
					escPressed = true;
					break;
				}
				uniform_int_distribution<short> dist1(20,30);
				uniform_int_distribution<short> dist2(0,20);
				uniform_int_distribution<short> d(1,4);
				int num1 = dist1(rnd()),num2 = dist2(rnd()),num3 = d(rnd());
				double awr = 0.0,player = 0.0;
				char sign;
				if(num3 == 1){
					sign = '+';
					awr = num1 + num2;
				}else if(num3 == 2){
					sign = '-';
					awr = num1 - num2;
				}else if(num3 == 3){
					sign = '*';
					awr = num1 * num2;
				}else{
					sign = '/';
					awr = double(num1) / num2;
				}while(true){
					if(_kbhit() && _getch() == 27){
						escPressed = true;
						break;
					}
					cout << num1 << sign << num2 << "=?" << endl;
					if(!(cin >> player)){
						cout << "回答输入错误！" << endl << "Invalid input!" << endl;
						file << " [" << id << " 1 -1]";
						continue; 
					}else if(awr == player){
						cout << "回答正确" << endl << "That's correct" << endl;
						file << " [" << id << " 1 1]";
						sc += 5; 
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						break;
					}else if(awr > player){
						cout << "小了" << endl << "Too low" << endl;
						file << " [" << id << " 1 0]";
						sc++;
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						continue;
					}else{
						cout << "大了" << endl << "Too high" << endl;
						file << " [" << id << " 1 2]";
						sc++;
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						continue;
					}
				}
				if(escPressed) break;
				bool again;
				while(true){
					if(_kbhit() && _getch() == 27){
						escPressed = true;
						break;
					}
					char ch = '\0';
					cout << "是否再玩一次？(Y/N): " << endl << "Play again? (Y/N): " << endl;
					cin >> ch;
					flush();
					clear();
					if(ch != 'n' && ch != 'N' && ch != 'y' && ch != 'Y'){
						file << " [" << id << " 3 -1]";
						sc++;
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						cerr << "操作输入错误！" << endl << "Invalid input operation!" << endl;
						continue;
					}else{
						if(ch == 'n' || ch == 'N'){
							again = false;
							break;
						}else if(ch == 'y' || ch == 'Y'){
							again = true;
							break;
						}
						sc += 5; 
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						file << " [" << id << " 3 " << !again << ']';
					}
				}
				if(escPressed) break;
				clear();
				if(again){
					continue;
				}else{
					break;
				}
			}
			if(escPressed) continue;
		}else if(id == '5'){
			if(!flag){
				f[4] = true;
			}
			clear();
			string word[10] = {
				"talk is cheap. show me the code",
			    "simplicity is the ultimate sophistication - computer science proverb",
			    "premature optimization is the root of all evil",
			    "the best time to learn programming was years ago, the second best is now",
			    "programming is not about typing, it is about thinking",
			    "good programmers write good code",
			    "testing shows the presence, not the absence of bugs",
			    "refactoring is key to keeping code healthy",
			    "code readability matters",
			    "programming is a craft that requires constant practice"
			};
			bool escPressed = false;
			while(true){
				if(_kbhit() && _getch() == 27){
					escPressed = true;
					break;
				}while(_kbhit()){
					_getch();
				}
				cout << "按下任意键开始……" << endl << "Press any key to start..." << endl;
				_getch();
				file << " [" << id << " 1 0]";
				uniform_int_distribution<short> dist(0,9);
				int num = dist(rnd()),crt = 0,tme = 0;
				cout << word[num] << endl;
				string str = "",s = "";
//				clear();
				auto sta = chrono::high_resolution_clock::now();
				for(int i = 0;i < word[num].size();){
					if(_kbhit()){
						char key = _getch();
						if(key == 27){
							escPressed = true;
							break;
						}else if(key == word[num][i]){
							crt++;
							sc += 5;
							data["score"] = sc;
							ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
							s.push_back(key);
							cout << key;
							i++;
						}else{
							crt--;
							tme++;
							cout << 'x';
							this_thread::sleep_for(chrono::seconds(1));
							while(_kbhit()){
								_getch();
							}
							clear();
							cout << word[num] << endl << s; 
						}
					}
				}
				if(escPressed) break;
				auto end = chrono::high_resolution_clock::now() - chrono::seconds(tme);
				auto ms = chrono::duration_cast<chrono::milliseconds>(end - sta);
				double nd = ms.count() / 1000.0;
				double ac = (crt * 100.0) / word[num].size();
				cout << endl << "用时: " << nd << "秒" << endl << "Time used: " << nd << 's' << endl
					 << "正确率: " << ac << '%' << endl << "Accuracy: " << ac << '%' << endl;
				file << " [" << id << " 1 " << nd << "s]"
					 << " [" << id << " 1 " << ac << "%]"; 
				bool again;
				while(true){
					if(_kbhit() && _getch() == 27){
						escPressed = true;
						break;
					}
					char ch = '\0';
					cout << "是否再玩一次？(Y/N): " << endl << "Play again? (Y/N): " << endl;
					cin >> ch;
					flush();
					clear();
					if(ch != 'n' && ch != 'N' && ch != 'y' && ch != 'Y'){
						file << " [" << id << " 2 -1]";
						sc++;
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						cerr << "操作输入错误！" << endl << "Invalid input operation!" << endl;
						continue;
					}else{
						if(ch == 'n' || ch == 'N'){
							again = false;
							break;
						}else if(ch == 'y' || ch == 'Y'){
							again = true;
							break;
						}
						sc += 5; 
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						file << " [" << id << " 2 " << !again << ']';
					}
				}
				if(escPressed) break;
				clear();
				if(again){
					continue;
				}else{
					break;
				}
			}
			if(escPressed) continue;
		}else if(id == '6'){
			if(!flag){
				f[5] = true;
			}
			file << " [" << id << " 0]";
			ofstream(C ? "D:/TmpSc.txt" : "TmpSc.txt") << sc;
			clear();
			system("贪吃蛇.exe 2>nul");
			string add;
			ifstream fin(C ? "D:/TmpSc.txt" : "TmpSc.txt");
			fin >> add;
			fin.close();
			sc += stoll(add);
			data["score"] = sc;
			ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
			system(("del " + string(C ? "D:/" : "") + "TmpSc.txt").c_str());
			file << " [" << id << " 1]";
		}else if(id == 'D' || id == 'd'){
			clear();
			while(true){
				string iac = "",ipw = "";
				cout << "请输入删除账户的用户名: " << endl << "Please enter the name for deleting the account: " << endl;
				cin >> iac;
				file << " [ 7 0 0]";
				bool flag = false;
				json iacc = json::object();
				for(const auto& i : fs::directory_iterator(bp)){
					ifstream fin(i.path());
					json tmp;
					fin >> tmp;
					if(tmp["user"] == iac){
						cout << "请输入删除账户的密码: " << endl << "Please enter the password for deleting the account: " << endl;
						iacc = tmp;
						flag = true;
						break;
					}
					fin.close();
				}if(flag){
					while(true){
						cin >> ipw;
						string ipsw = iacc["password"];
						for(char &c : ipsw){
							c ^= 0x5A;
						}if(ipsw == ipw){
							file << " [7 1 0]";
							if(remove((string(C ? "D:/" : "") + "users/" + iac + ".json").c_str()) == 0){
								file << " [7 2 0]";
								cout << "删除成功！" << endl << "Deleted successfully!" << endl;
							}else{
								file << " [7 2 1]";
								cerr << "删除失败！" << endl << "Deleted fail!" << endl;
							}
							break;
						}else{
							file << " [7 1 1]";
							cerr << "密码错误！" << endl << "Incorrect password!" << endl;
							continue;
						}
					}
					break;
				}else{
					cerr << "账户名错误！" << endl << "Incorrect account name!" << endl;
					continue;
				}
			}
			clear();
			cout << "感谢您的使用！" << endl << "Thank you for using!"; 
			file.close();
			return 0;
		}else if(id == 'S' || id == 's'){
			file << " [8";
			while(true){
				clear();
				cout << "积分: " << sc << endl << "Score: " << sc << endl
					 <<"1.设置页面颜色，需消耗10积分" << endl << "1.Set page color,consumes 10 credits" << endl
	//				 << "2."
					 << "请输入编号：" << endl << "Please enter the ID: " << endl;
				char sid;
				cin >> sid;
				if(sid == '1'){
					if(sc < 10){
						file << " 1 1]";
						cerr << "积分不足！" << endl << "Not enough credits!" << endl; 
					}else{
						file << " 1 0 ";
						sc -= 10;
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						cout << "兑换成功" << endl << "Redemption successful" << endl;
						while(true){
							cout << "请输入颜色: " << endl << "Please enter the color" << endl
								 << "1.红色(R)" << endl << "1.Red(R)" << endl
							 	 << "2.绿色(G)" << endl << "2.Green(G)" << endl
							 	 << "3.蓝色(B)" << endl << "3.Blue(B)" << endl
							 	 << "4.默认值(D)" << endl << "4.Default value(D)" << endl;
							char color;
							cin >> color;
							if(color == 'R'){
								file << int('R') << ']';
								system("color 4F");
								break;
							}else if(color == 'G'){
								file << int('G') << ']';
								system("color 20");
								break;
							}else if(color == 'B'){
								file << int('B') << ']';
								system("color 1F");
								break;
							}else if(color == 'D'){
								file << int('D') << ']';
								system("color 07");
								break;
							}else{
								file << "-1]";
								cerr << "颜色编号输入错误！" << endl << "Invalid color ID!" << endl;
							}
						}
					}
				}else{
					file << " 1 -1]"; 
					cerr << "编号输入错误！" << endl << "Invalid ID!" << endl;
				}
				bool again;
				while(true){
					char ch = '\0';
					cout << "是否继续兑换？(Y/N)" << endl << "Continue redeeming? (Y/N)" << endl;
					cin >> ch; 
					if(ch != 'n' && ch != 'N' && ch != 'y' && ch != 'Y'){
						file << " [8 2 -1]";
						sc++;
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						cerr << "操作输入错误！" << endl << "Invalid input operation!" << endl;
						continue;
					}else{
						if(ch == 'n' || ch == 'N'){
							again = false;
							break;
						}else if(ch == 'y' || ch == 'Y'){
							again = true;
							break;
						}
						sc += 5; 
						data["score"] = sc;
						ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
						file << " [8 2 " << !again << ']';
					}
				}
				clear();
				if(again){
					continue;
				}else{
					break;
				}
			}
		}else if(id == 'R' || id == 'r'){
			clear();
			set<pair<long long,string>,greater<pair<long long,string>>> rnk = {make_pair(data["score"],data["user"])};
			for(const auto& i : fs::directory_iterator(bp)){
				if(i.path().stem().string() != data["user"]){
					ifstream fin(i.path());
					json tmp;
					fin >> tmp;
					rnk.insert(make_pair(tmp["score"],tmp["user"]));
				}
			}
			cout << "排行榜: " << endl << "Ranking: " << endl;
			unsigned int usrn = 1;
			for(const auto& i : rnk){
				cout << usrn << ". " << i.second << " - " << i.first << "分/Point(s)" << endl;
				usrn++;
			}
			file << " [9]";
			while(true){
			    if(_kbhit() && _getch() == 27){
			    	clear();
			        break;
			    }
			}
		}else if(id == 'A' || id == 'a'){
			clear();
			cout << "成就: " << endl << "Achievements: " << endl
				 << "已解锁: " << endl << "Unlocked: " << endl;
			for(const auto& i : ach){
				bool unlocked = data["achievement"][to_string(i.id)];
				if(unlocked){
					cout << i.cnm << " - " << i.enm << endl; 
				}else{
					continue;
				}
			}
			cout << endl << "未解锁: " << endl << "Locked: " << endl;
			for(const auto& i : ach){
				bool unlocked = data["achievement"][to_string(i.id)];
				if(!unlocked){
					cout << i.cnm << " - " << i.enm << endl; 
				}else{
					continue;
				}
			}
			file << " [10]";
			while(true){
			    if(_kbhit() && _getch() == 27){
			    	clear();
			        break;
			    }
			}
		}else if(id == 'E' || id == 'e'){
			clear();
			file << " [11]";
			data["score"] = sc;
			ofstream((bp + "/" + acc + ".json").c_str()) << data.dump(4); 
			cout << "感谢您的使用！" << endl << "Thank you for using!"; 
			file.close();
			return 0;
		}else{
			file << " [-1]";
			cerr << "游戏编号或操作内容输入错误！" << endl << "Invalid game ID or operation input!" << endl;
			continue;
		}
		flag = true; 
		for(short i = 0;i < 6;i++){
			if(f[i] == false){
				flag = false;
				break;
			}
		}if(flag){
			data["achievement"]["4"] = true;
		}
		file.close();
	}
}
