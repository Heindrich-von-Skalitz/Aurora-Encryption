//Copyright 2022 Yang Y., All rights reserved.
//mixColumn为字节代换，写错了不能改了，列混淆将被命名为columnMix
#pragma once
#include<random>
#include<fstream>
#include"base64.h"
#include<string>
#include<conio.h>
#include<Windows.h>

std::vector<std::string>localization,
chinese = { "无法打开sbox.txt，将重新生成一个sbox.txt。" ,"无法打开key.txt，将重新生成一个key.txt。" ,"key.txt出错！将重新生成key.txt" ,"更新密钥或者S盒后务必重新启动！","请输入工作模式(1)加密(2)解密(3)生成密钥(4)生成S盒(5)输入密钥(其他数字)退出：","请输入明文：" ,"加密结果是：" ,"请输入密文（没有写检测代码，所以一定不要输入任何非加密结果）：","解密结果是：","请输512位16进制数，此模式仅用于复制别人发送的密钥，不适用于重新生成密钥：","密钥长度错误！","输入了非法字符！","替换成功" },
english = { "Can't open sbox.txt, generating a new sbox.txt. " ,"Can't open key.txt，will generating a new key.txt." ,"key.txt has error! Generating a new key.txt" ,"After key or S-box update/generation, you shall restart the program!","Please choose program mode: (1)Encryption (2)Decryption (3)New key generation (4)Sbox generation (5)Input new key (Other Number)Exit: ","Please input plain text: " ,"Encryption result:" ,"Please input cypher text:","Decryption result: ","Please input 512 hexadecimal numbers:","Key length ERROR!","INVALID CHAR detected!","Success!" },
russian = { "Не удается открыть sbox.txt, генерируется новый sbox.txt. " ,"Не удается открыть key.txt, будет создан новый key.txt." ,"key.txt ошибка! Генерируем новый key.txt" ,"После обновления/генерации ключа или S-box вам необходимо перезапустить программу!", "Пожалуйста, выберите режим работы программы: (1) Шифрование (2)Дешифрование (3)Генерация нового ключа (4)Генерация Sbox (5)Введите новый ключ (другой номер)Выход: ", "Пожалуйста, введите обычный текст: ", "Результат шифрования:", "Пожалуйста, введите текст шифра:", "Результат расшифровки:", "Пожалуйста, введите 512 шестнадцатеричных чисел:", "Ошибка длины ключа!", "Обнаружен НЕДОПУСТИМЫЙ символ!", "Успех!" };

std::wstring u16_to_wstring(std::u16string inp) {
	return std::wstring(inp.begin(), inp.end());
}
std::u16string wstring_to_u16(std::wstring inp) {
	return std::u16string(inp.begin(), inp.end());
}
std::string wstr_to_str(std::wstring inp) {
	return std::string(inp.begin(), inp.end());
}
std::u16string str_to_u16(std::string inp) {
	return std::u16string(inp.begin(), inp.end());
}

std::vector<std::vector<int>> sbox(16, std::vector<int>(16));
std::vector<std::vector<int>> invsbox(16, std::vector<int>(16));
std::string base16 = "0123456789ABCDEF";
std::vector<std::vector<int>> mixColumnMatrix = { {2,3,1,1}, {1,2,3,1}, {1,1,2,3}, {3,1,1,2} };
std::vector<std::vector<int>> invMixColumnMatrix = { {14,11,13,9}, {9,14,11,13}, {13,9,14,11}, {11,13,9,14} };
std::string key;
std::string str_mat1 = { 2,3,1,1,1,2,3,1,1,1,2,3,3,1,1,2 },
str_mat2 = { 14,11,13,9,9,14,11,13,13,9,14,11,11,13,9,14 };//互为逆的字符串,生成列混淆用的矩阵


std::ofstream write_key;

void gotoxy(int x, int y) {
	int xx = 0x0b;
	HANDLE hOutput;
	COORD loc;
	loc.X = x;
	loc.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, loc);
}

std::string stringToBase16(std::string inptmp) {
	std::string anstmp = "";
	for (int i = 0; i < inptmp.size(); i++)
	{
		anstmp.push_back(base16[(uint8_t)inptmp[i] / 16]);
		anstmp.push_back(base16[(uint8_t)inptmp[i] % 16]);
	}
	return anstmp;
}

std::string base16ToString(std::string inptmp)
{
	std::string tmpans = "";
	for (int i = 0; i < inptmp.size() / 2; i++) {
		tmpans.push_back(base16.find(inptmp[2 * i]) * 16 + base16.find(inptmp[2 * i + 1]));
	}
	return tmpans;
}

int randint(int min, int max)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(min, max);
	return distrib(gen);
}

int xorr(int a_, int b_)
{
	return (!a_ && b_) || (a_ && !b_);
}

void summonSbox()
{
	system("python .\\ASE\\summonSbox.py");
}

void summonKey()
{
	int tmp = 0;
	write_key.open(".\\ASE\\key.txt");
	for (int i = 0; i < 256; i++)
	{
		tmp = randint(0, 255);
		write_key << base16[tmp / 16] << base16[tmp % 16];
	}
	write_key.close();
}

void cursor(bool bl)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = bl; //隐藏控制台光标
	SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
}

void init()
{
	cursor(false);
	std::cout << "Use A/D to select language. Press E/ENTER to continue. 使用 A/D 选择语言，使用E/ENTER继续。";
	int language = 0, keyval = 0;
	while (true) {
		keyval = 0;
		if (_kbhit()) {
			keyval = _getch();
		}
		if (keyval == 13 || keyval == 101) {
			break;
		}
		if (keyval == 97) {
			language--;
		}
		if (keyval == 100) {
			language++;
		}
		if (language < 0) {
			language = 2;
		}
		if (language > 2) {
			language = 0;
		}
		//std::cout << keyval;
		gotoxy(0, 1);
		std::cout << "                       ";
		gotoxy(0, 1);
		switch (language) {
		case 0: std::cout<<"中文"; break;
		case 1: std::cout << "English"; break;
		case 2: std::cout << "Русский"; break;
		}
		Sleep(50);
	}
	switch (language) {
	case 0: localization = chinese; break;
	case 1: localization = english; break;
	case 2: localization = russian; break;
	}
	system("cls");
	gotoxy(0, 0);
	//调用sbox中第a个元素:sbox[a/16][a%16]，sbox的值作为逆sbox的索引时返回sbox的索引
	std::string inp_key;
initializeSbox:
	std::ifstream ifs_sbox(".\\ASE\\sbox.txt");
	if (!ifs_sbox.is_open()) {
		std::cout << localization[0] << std::endl;
		summonSbox();
		goto initializeSbox;
	}
initializeKey:
	std::ifstream ifs_key(".\\ASE\\key.txt");
	if (!ifs_key.is_open()) {
		std::cout << localization[1] << std::endl;
		if (false) {
		key_err:
			std::cout << localization[2] << std::endl;
		}
		summonKey();
		goto initializeKey;
	}
	ifs_key >> inp_key;
	if (inp_key.size() != 512)
	{
		goto key_err;
	}
	for (int i = 0; i < 256; i++)
	{
		key.push_back(base16.find(inp_key[2 * i]) * 16 + base16.find(inp_key[2 * i + 1]));
	}
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			ifs_sbox >> sbox[i][j];
		}
	}
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			invsbox[sbox[i][j] / 16][sbox[i][j] % 16] = i * 16 + j;
		}
	}
	ifs_sbox.close();
	ifs_key.close();
}

int getSbox(uint8_t index)//一个八位二进制正数/16相当于右移四位，%16相当于返回被右移消去的四位整数0xA3/16=A,0xA3%16=3
{
	return sbox[index / 16][index % 16];
}

int getInvSbox(uint8_t index)
{
	return invsbox[index / 16][index % 16];
}

std::vector<std::vector<int>> GF28mul16Matrix(std::vector<std::vector<int>> tmp1, std::vector<std::vector<int>> tmp2)
{
	std::vector<std::vector<int>> ans(4, std::vector<int>(4));
	uint8_t tmp_ans, tmp_L, tmp_R;//直接让操作系统截掉超出部分，玩图灵完备玩的
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				tmp_ans = 0;
				tmp_L = tmp1[i][k];
				tmp_R = tmp2[k][j];
				for (int ii = 0; ii < 8; ii++)
				{/*//知乎上抄的算法
					if (tmp_L & 0x01)//如果tmp_L最低位是1就异或tmp_R，相当于加上tmp_R * 1
					{
						tmp_ans ^= tmp_R;
					}
					tmp_L >>= 1; //tmp_L右移一位，相当于除以2
					//这段代码相当于把tmp_R限定在八位指定区间
					if (tmp_R & 0x80)//如果tmp_R最高位为1
					{
						//左移一位相当于乘二
						tmp_R = tmp_R << 1; //注：这里会丢失最高位，但是不用担心

						tmp_R ^= 0x1B; //计算伽罗瓦域内除法tmp_R = tmp_R / (x^8(刚好丢失最高位) + x^4 + x^3 + x^1 + 1),0x1B=00011011
					}
					else
					{
						//左移一位相当于乘二
						tmp_R = tmp_R << 1;
					}*///自己写的算法
					if (tmp_L & 0x01)
					{
						tmp_ans ^= tmp_R;
					}
					tmp_L >>= 1;
					tmp_R <<= 1;
				}
				ans[i][j] ^= tmp_ans;
			}
		}
	}
	return ans;
}

std::string mixColumn(std::string mCinp)
{
	std::string mCtmp = "";
	for (int i = 0; i < 16; i++)
	{
		mCtmp.push_back(getSbox((uint8_t)mCinp[i]));
	}
	return mCtmp;
}

std::string invMixColumn(std::string iMCinp)
{
	std::string iMCtmp = "";
	for (int i = 0; i < 16; i++)
	{
		iMCtmp.push_back(getInvSbox((uint8_t)iMCinp[i]));
	}
	return iMCtmp;
}

std::string XOR(std::string inp1, std::string inp2) {
	std::string xortmp = "0000000000000000";
	for (int i = 0; i < 16; i++)
	{
		xortmp[i] = ((int)inp1[i]) ^ ((int)inp2[i]);
	}
	return xortmp;
}

std::string shiftRows(std::string sRinp) {
	std::string sR_ans = sRinp;
	sR_ans[4] = sRinp[5];
	sR_ans[5] = sRinp[6];
	sR_ans[6] = sRinp[7];
	sR_ans[7] = sRinp[4];

	sR_ans[8] = sRinp[10];
	sR_ans[9] = sRinp[11];
	sR_ans[10] = sRinp[8];
	sR_ans[11] = sRinp[9];

	sR_ans[12] = sRinp[15];
	sR_ans[13] = sRinp[12];
	sR_ans[14] = sRinp[13];
	sR_ans[15] = sRinp[14];
	return sR_ans;
}

std::string invShiftRows(std::string sRinp) {
	std::string sR_ans = sRinp;
	sR_ans[4] = sRinp[7];
	sR_ans[5] = sRinp[4];
	sR_ans[6] = sRinp[5];
	sR_ans[7] = sRinp[6];

	sR_ans[8] = sRinp[10];
	sR_ans[9] = sRinp[11];
	sR_ans[10] = sRinp[8];
	sR_ans[11] = sRinp[9];

	sR_ans[12] = sRinp[13];
	sR_ans[13] = sRinp[14];
	sR_ans[14] = sRinp[15];
	sR_ans[15] = sRinp[12];
	return sR_ans;
}

std::string stringMultiply(std::string sMinp1, std::string sMinp2) {
	std::vector<std::vector<int>>sMtmp1(4, std::vector<int>(4));
	std::vector<std::vector<int>>sMtmp2(4, std::vector<int>(4));
	std::string sMans;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) {
			sMtmp1[i][j] = sMinp1[i * 4 + j];
			sMtmp2[i][j] = sMinp2[i * 4 + j];
		}
	}
	sMtmp1 = GF28mul16Matrix(sMtmp1, sMtmp2);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++) {
			sMans.push_back(sMtmp1[i][j]);
		}
	}
	return sMans;
}

std::string encryptVec(std::string inpEV) {
	std::string ktmp, tmp1 = inpEV;
	for (int i = 0; i < 16; i++)//生成首轮密钥
	{
		ktmp.push_back(key[i]);
	}
	tmp1 = XOR(tmp1, ktmp);//首轮轮密钥加
	for (int i = 0; i < 15; i++)
	{
		tmp1 = mixColumn(tmp1);//字节代换
		tmp1 = shiftRows(tmp1);//行位移
		tmp1 = stringMultiply(tmp1, str_mat1);
		ktmp = "";
		for (int j = 0; j < 16; j++)//生成每轮的密钥
		{
			ktmp.push_back(key[j + (i + 1) * 16]);
		}
		tmp1 = XOR(tmp1, ktmp);//每轮轮密钥加
		//std::cout << stringToBase16(key_tmp) << std::endl;
	}
	return tmp1;
}

std::string decryptVec(std::string inpDV) {
	std::string ktmp, tmp1 = inpDV;
	//std::cout << inpDV.size() << std::endl;
	for (int i = 14; i >= 0; i--)
	{
		ktmp = "";
		for (int j = 0; j < 16; j++)//生成每轮的密钥
		{
			ktmp.push_back(key[j + (i + 1) * 16]);
		}
		//std::cout << stringToBase16(key_tmp) << std::endl;
		tmp1 = XOR(tmp1, ktmp);//每轮轮密钥加
		tmp1 = stringMultiply(tmp1, str_mat2);//逆列混淆
		tmp1 = invShiftRows(tmp1);//逆行位移
		tmp1 = invMixColumn(tmp1);//逆字节代换
	}
	ktmp = "";
	for (int i = 0; i < 16; i++)//生成首轮密钥
	{
		ktmp.push_back(key[i]);
	}
	//std::cout << ktmp.size() << std::endl;
	tmp1 = XOR(tmp1, ktmp);//首轮轮密钥加
	return tmp1;
}

void AES()
{
	init();
	cursor(true);
	std::cout << localization[3] << std::endl;
	int AES_mode = 1;
	std::string tmp_inpString = "";
	while (AES_mode) {
		std::string inp = "";//输入，将被分块
		std::string tmp1 = "";//分块后的字符将被储存在这个变量里
		std::string vec4 = "";//保存异或向量
		std::string tmp_vec4;
		std::string answ = "";
		std::string oup = "";
		std::string key_tmp = "";//保存轮密钥
		std::wstring winp;//输入UTF-16字符串
		int tmp3 = 0;
		int tmpoup = 0;
		std::cout << localization[4] << std::endl;
		std::cin >> AES_mode;
		switch (AES_mode) {
		case 1://加密
			std::cout << localization[5] << std::endl;
			std::wcin.ignore();
			std::getline(std::wcin, winp);
			inp = wstr_to_str(winp);
			/*for (int i = 0; i < inp.size() - 1; i++) {
				std::cout << (unsigned short)inp[i] << std::endl;
			}*/
			for (int i = 0; i < 16; i++)//生成初始向量
			{
				vec4.push_back(randint(0, 255));
			}
			//std::cout << stringToBase16(vec4) << std::endl;
			oup += encryptVec(vec4);
			//oup += vec4;
			while (!inp.empty()) {
				key_tmp = "";
				for (int i = 0; i < 16; i++)//生成首轮密钥
				{
					key_tmp.push_back(key[i]);
				}
				tmp1 = "";//切割
				if (inp.size() >= 16)
				{
					for (int i = 0; i < 16; i++)
					{
						tmp1.push_back(inp[i]);
					}
					inp.erase(0, 16);
				}
				else {
					tmp3 = inp.size();
					for (int i = 0; i < 16; i++) {
						if (i < tmp3) {
							tmp1.push_back(inp[i]);
						}
						else {
							tmp1.push_back(0);
						}
					}
					inp.erase(0, inp.size());
				}
				tmp_vec4 = tmp1;//保存明文
				tmp1 = XOR(tmp1, vec4);//初始向量和输入内容异或
				tmp1 = XOR(tmp1, key_tmp);//首轮轮密钥加
				for (int i = 0; i < 15; i++)
				{
					tmp1 = mixColumn(tmp1);//字节代换
					tmp1 = shiftRows(tmp1);//行位移
					tmp1 = stringMultiply(tmp1, str_mat1);
					key_tmp = "";
					for (int j = 0; j < 16; j++)//生成每轮的密钥
					{
						key_tmp.push_back(key[j + (i + 1) * 16]);
					}
					tmp1 = XOR(tmp1, key_tmp);//每轮轮密钥加
					//std::cout << stringToBase16(key_tmp) << std::endl;
				}
				vec4 = XOR(tmp1,tmp_vec4);//获取下一轮的向量
				oup += tmp1;
			}
			std::cout << localization[6] << std::endl;
			std::cout << stringToBase16(oup) << std::endl;
			break;
		case 2://解密
			std::cout << localization[7] << std::endl;
			std::cin.ignore();
			std::getline(std::cin, inp);
			vec4 = "";
			for (int i = 0; i < 32; i++) {
				vec4.push_back(inp[i]);
			}
			vec4 = decryptVec(base16ToString(vec4));
			//vec4 = base16ToString(vec4);
			inp.erase(0, 32);
			while (!inp.empty()) {
				tmp1 = "";
				for (int i = 0; i < 32; i++) {
					tmp1.push_back(inp[i]);
				}
				tmp1 = base16ToString(tmp1);
				inp.erase(0, 32);
				tmp_vec4 = tmp1;
				for (int i = 14; i >= 0; i--)
				{
					key_tmp = "";
					for (int j = 0; j < 16; j++)//生成每轮的密钥
					{
						key_tmp.push_back(key[j + (i + 1) * 16]);
					}
					//std::cout << stringToBase16(key_tmp) << std::endl;
					tmp1 = XOR(tmp1, key_tmp);//每轮轮密钥加
					tmp1 = stringMultiply(tmp1, str_mat2);//逆列混淆
					tmp1 = invShiftRows(tmp1);//逆行位移
					tmp1 = invMixColumn(tmp1);//逆字节代换
				}
				key_tmp = "";
				for (int i = 0; i < 16; i++)//生成首轮密钥
				{
					key_tmp.push_back(key[i]);
				}
				tmp1 = XOR(tmp1, key_tmp);//首轮轮密钥加
				tmp1 = XOR(tmp1, vec4);//和初始向量异或
				//std::cout << tmp1 << std::endl;
				vec4 = XOR(tmp1, tmp_vec4);
				oup += tmp1;
			}
			std::cout << localization[8] << std::endl;
			//std::wcout << u16_to_wstring(str_to_u16(oup)) << std::endl;
			std::cout << oup << std::endl;
			break;
		case 3://生成密钥
			summonKey();
			init();
			break;
		case 4://更新S盒
			summonSbox();
			init();
			break;
		case 5://输入密钥更新
			write_key.open(".\\ASE\\key.txt");
			std::cout << localization[9] << std::endl;
			std::cin >> tmp_inpString;
			int finded_index;
			if (tmp_inpString.size() != 512) {
				std::cout << localization[10] << std::endl;
				break;
			}
			for (int i = 0; i < 512; i++)
			{
				finded_index = base16.find(tmp_inpString[i]);
				if (finded_index == std::string::npos)
				{
					std::cout << localization[11] << std::endl;
					break;
				}
			}
			write_key << tmp_inpString;
			write_key.close();
			system("cls");
			std::cout << localization[12] << std::endl;
			init();
			break;
		default:
			AES_mode = 0;
			break;
		}
	}
}