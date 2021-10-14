/*
===================================================================
Project Name  : コンピュータネットワーク 期末レポート (2)発展課題 誤り符号検出
File Name     : CRC.cpp
Encoding      : UTF-8
Creation Date : 30/7/2021
Copyright (c) 2021 Yuma Horaguchi All rights reserved.
===================================================================
*/

#include "CRC.hpp"

int main(){
	// CRCによる符号化
	cout << "===========CRCによる送信データ系列の生成===========" << endl;
	vector<bool> u_x = make_bit_list("データ系列の多項式u(x)");
	vector<bool> g_x = make_bit_list("生成多項式g(x)");
	vector<bool> input = make_input(u_x, g_x);
	Shift_Register crc;

	// 入力データ、初期状態の確認
	cout << "データ系列の多項式：";
	print_polynomial(u_x, "u(x)");
	cout << "生成多項式：";
	print_polynomial(g_x, "g(x)");
	cout << "初期状態" << endl;
	crc.make_parity_bit(g_x, input);
	crc.print_parity_bit();

	// 演算
	crc.CRC(input, g_x);

	// CRC多項式の演算結果
	cout << "CRC多項式：";
	print_polynomial(crc.parity_bit, "c(x)");

	// 新しいデータ系列の作成
	vector<bool> s_x = make_new_data(crc.parity_bit, u_x);
	cout << "新しいデータ系列の多項式：";
	print_polynomial(s_x, "s(x)");
	cout << endl;

	// 誤り検出
	cout << "===================誤り検出===================" << endl;
	vector<bool> z_x = make_bit_list("受信データ系列の多項式z(x)");
	Shift_Register error_checker;

	// 入力データ、初期状態の確認
	cout << "受信データ系列の多項式：";
	print_polynomial(z_x, "z(x)");
	cout << "生成多項式：";
	print_polynomial(g_x, "g(x)");
	cout << "初期状態" << endl;
	error_checker.make_parity_bit(g_x, z_x);
	error_checker.print_parity_bit();

	// 演算
	error_checker.CRC(z_x, g_x);

	// 演算結果
	if(error_checker.error == false){
		cout << "誤りがありませんでした" << endl;
	}
	else{
		cout << "誤りがありました" << endl;
	}
}
