/*
===================================================================
Project Name  : コンピュータネットワーク 期末レポート (2)発展課題 誤り符号検出
File Name     : CRC.hpp
Encoding      : UTF-8
Creation Date : 30/7/2021
Copyright (c) 2021 Yuma Horaguchi All rights reserved.
===================================================================
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Shift_Register{
	public:
		vector<bool> parity_bit;
		vector<bool> judge_xor;
		int box_size;
		bool error;

		// コンストラクタ
		Shift_Register(){
			box_size = 0;
			error = false;
		}

		// g_xからシフトレジスタを初期化する関数
		void make_parity_bit(vector<bool> g_x, vector<bool> input){
			// 箱の個数を決定
			for(int i = 0; i < g_x.size(); i++){
				if(g_x[i] == 1){
					box_size += 1;
					// 箱間でXORの計算が必要かを記録
					if(g_x[i + 1] == 1){
						judge_xor.push_back(1);
					}
					else{
						if(i == g_x.size() - 1){
							judge_xor.push_back(1);
						}
						else{
							judge_xor.push_back(0);
						}
					}
				}
			}
			// 箱を初期化(x^n u(x)の代入)
			for(int i = 0; i < box_size; i++){
					parity_bit.push_back(input[i]);
			}
		}

		// シフトレジスタの状態を出力する関数
		void print_parity_bit(){
			for(int i = 0; i < parity_bit.size(); i++){
				cout << parity_bit[parity_bit.size() - 1 - i] << " ";
			}
			cout << endl;
		}

		// 割り算
		void CRC(vector<bool>input, vector<bool> g_x){
			for(int j = 0; j < input.size() - box_size; j++){
				vector<bool> prev_parity_bit = parity_bit;
				// 先頭の箱
				// XORの計算ありなら
				if(judge_xor[judge_xor.size() - 1] == 1){
					// 最後の箱とXOR
					parity_bit[box_size - 1] = input[box_size + j] ^ prev_parity_bit[0];
				}
				// XORの計算なしなら
				else{
					// 入力待ちをそのまま代入
					parity_bit[box_size - 1] = input[box_size + j];
				}
				// // 2つ目以降の箱
				for(int i = 1; i < box_size; i++){
					// XORの計算ありなら
					if(judge_xor[judge_xor.size() - 1 - i] == 1){
						// 1つ前の箱とXOR
						parity_bit[box_size - 1 - i] = prev_parity_bit[box_size - 1 - i + 1] ^ prev_parity_bit[0];
					}
					// XORの計算なしなら
					else{
						// 前の箱をそのまま代入
						parity_bit[box_size - 1 - i] = prev_parity_bit[box_size - 1 - i + 1];
					}
				}
				cout << "更新" << j + 1 << "回目" << endl;
				print_parity_bit();
			}
			check_error();
		}

		// 誤り検出
		void check_error(){
			for(int i = 0; i < parity_bit.size() ;i++){
				if(parity_bit[i] == 1){
					error = true;
					break;
				}
			}
		}
};

// 多項式を生成する関数
vector<bool> make_bit_list(string name){
	int length = 0;
	bool append;
	vector<bool> bit_list;
	cout << name << "の次数:";
	cin >> length;
	for(int i  = length; i >= 0; i--){
		cout << "x^" << i << "の係数:";
		cin >> append;
		bit_list.push_back(append);
	}
	return bit_list;
}

// c_xにu_xを繋げて新しいデータ系列を作成する関数
vector<bool> make_new_data(vector<bool> c_x, vector<bool> s_x){
	for(int i = 0; i < c_x.size(); i++){
		s_x.push_back(c_x[i]);
	}
	return s_x;
}

// 多項式を出力する関数
void print_polynomial(vector<bool> polynomial, string name){
	cout << name << " = ";
	int n = 0;
	while(1){
		if(polynomial[n] == 1){
			if(polynomial.size() - 1 - n == 0){
				cout << "1" << endl;
				break;
			}
			else{
				cout << "x^" << polynomial.size() - 1 - n;
				break;
			}
		}
		n++;
	}
	for(int i = n + 1; i < polynomial.size(); i++){
		if(i != polynomial.size() - 1){
			if(polynomial[i] == 1){
				if(polynomial.size() - i - 1 == 1){
					cout <<" + x";
				}
				else{
					cout <<" + x^" << polynomial.size() - i - 1;
				}
			}
		}
		else{
			if(polynomial[i] == 1){
				cout << " + 1" << endl;
			}
			else{
				cout << endl;
			}
		}
	}
}

// 入力x^n u(x)を作成する関数
vector<bool> make_input(vector<bool> u_x, vector<bool> g_x){
	vector<bool> input = u_x;
	for(int i = 0; i < g_x.size() - 1; i++){
		input.push_back(0);
	}
	return input;
}
