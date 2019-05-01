#include <iostream>
#include <vector>
#include <string>

using namespace std;

void HexToBits(vector<char> &Text, vector<bool> &Bits)
{
	for (auto L : Text)
	{
		if (L < 58)
		{
			for (int i = 3; i >= 0; i--)
			{
				Bits.emplace_back((L >> i) & 1);
			}
		}
		else
		{
			Bits.emplace_back(1);
			L++;
			for (int i = 2; i >= 0; i--)
			{
				Bits.emplace_back((L >> i) & 1);
			}
		}
	}
	while(Bits.size() < 16)
		Bits.insert(Bits.end(), { 0, 0, 0, 0 });
}

void PrintBitsInHex(vector<bool> text)
{
	int temp = 0;
	cout << "0x";
	for (unsigned j = 0; j < text.size(); j += 4)
	{
		temp = text[0 + j] * 8 + text[1 + j] * 4 + text[2 + j] * 2 + text[3 + j] * 1;
		if (temp < 10)
		{
			cout << temp;
		}
		else
		{
			cout << static_cast<char>(87 + temp);
		}
	}
	cout << endl;
}

void PrintBitsInHex(vector<vector<bool>> text)
{
	int temp = 0;
	cout << "0x";
	for (int i = 0; i < text.size(); i++)
	{
		for (unsigned j = 0; j < text[i].size(); j += 4)
		{
			temp = text[i][0 + j] * 8 + text[i][1 + j] * 4 + text[i][2 + j] * 2 + text[i][3 + j] * 1;
			if (temp < 10)
			{
				cout << temp;
			}
			else
			{
				cout << static_cast<char>(87 + temp);
			}
		}
	}
	cout << endl;
}

void PrintVecBool(vector<bool> text)
{
	for (auto l : text) cout << l;
	cout << endl;
}

void InputThenHexToBits(vector<bool> &input, const int size)
{
	vector<char> temp;
	bool check = 1;
	string prompt;
	temp.reserve(size);
	while (cin >> prompt)
	{
		if (prompt.length() <= size)
		{
			check = 1;
			for (auto b : prompt)
			{
				if (b < '0' || b > 'f')
				{
					cout << "Invalid input \"" << b << "\"!\n";
					"Please give me input in hex (0 - f)\n";
					check = 0;
					break;
				}
			}
			if (check)
			{
				for (auto b : prompt)
				{
					temp.emplace_back(b);
				}
			}
		}
		else
		{
			cout << "Invalid input \"" << prompt << "\"!\n";
			cout << "Please give me input hex by hex or to " << 4 - temp.size() << " hexs at once!\n";
		}
		if (temp.size() > 0) break;
	}
	input.reserve(temp.size() * 4);
	HexToBits(temp, input);
}

vector<bool> operator<< (vector<bool> lhs, unsigned int rhs)
{
	if (rhs > lhs.size())
	{
		rhs = lhs.size();
	}
	if (rhs != 0)
	{
		lhs.erase(lhs.begin(), lhs.begin() + rhs);
		lhs.insert(lhs.end(), rhs, 0);
	}

	return lhs;
}

vector<bool> operator>> (vector<bool> lhs, unsigned int rhs)
{
	if (rhs > lhs.size())
	{
		rhs = lhs.size();
	}
	if (rhs != 0)
	{
		lhs.erase(lhs.end() - rhs, lhs.end());
		lhs.insert(lhs.begin(), rhs, 0);
	}
	return lhs;
}

// XOR dla vectora<bool>, działa tylko dla równych co do wielkości ciągów
vector<bool> operator^ (vector<bool> lhs, vector<bool> rhs)
{
	if (lhs.size() != rhs.size())
		return { 0 };
	for (unsigned int i = 0; i < lhs.size(); i++)
	{
		lhs[i] = lhs[i] ^ rhs[i];
	}
	return lhs;
}

// XOR dla vectora<vectora<bool>>,działa tylko dla równych co do wielkości ciągów
vector<vector<bool>> operator^ (vector<vector<bool>> lhs, vector<vector<bool>> rhs)
{
	for (int i = 0; i < lhs.size(); i++)
	{
		lhs[i] = lhs[i] ^ rhs[i];
	}
	return lhs;
}

vector<bool> operator* (vector<bool> lhs, vector<bool> rhs) 
{
	if (lhs.size() != rhs.size())
		return { 0 };
	vector<bool> temp;
	temp.insert(temp.begin(), lhs.size() + rhs.size() , 0);
	lhs.insert(lhs.begin(), rhs.size(), 0);
	for (int i = rhs.size(); i > 0; i--)
	{
		if (rhs[rhs.size()-1] == 1)
		{
			temp = temp ^ lhs;
		}
		lhs = lhs << 1;
		rhs = rhs >> 1;
	}
	for(int i = 0; temp[i] != 1;)
	{
		if (temp.size() == 4)
			break;
		temp.erase(temp.begin());
	}
	return temp;
} 

vector<bool> operator% (vector<bool> lhs, vector<bool> rhs)
{
	unsigned int initial_rhs = rhs.size();
	if (lhs.size() >= initial_rhs)
	{
		rhs.insert(rhs.end(), lhs.size() - rhs.size(), 0);
		while (true)
		{
			if (lhs.size() >= initial_rhs)
			{
				if (lhs[0] == 1)
				{
					lhs = lhs ^ rhs;
				}
				rhs = rhs >> 1;
				lhs.erase(lhs.begin());
				rhs.erase(rhs.begin());
			}
			else break;
		}
	}
	return lhs;
}

void operator+=(vector<bool> &lhs, vector<bool> rhs)
{
	lhs.insert(lhs.end(), rhs.begin(), rhs.end());
}

vector<bool> SBoxE(vector<bool> input)
{
	switch (input[0] * 8 + input[1] * 4 + input[2] * 2 + input[3])
	{
	case 0: return { 1,1,1,0 };
	case 1: return { 0,1,0,0 };
	case 2: return { 1,1,0,1 };
	case 3: return { 0,0,0,1 };
	case 4: return { 0,0,1,0 };
	case 5: return { 1,1,1,1 };
	case 6: return { 1,0,1,1 };
	case 7: return { 1,0,0,0 };
	case 8: return { 0,0,1,1 };
	case 9: return { 1,0,1,0 };
	case 10: return { 0,1,1,0 };
	case 11: return { 1,1,0,0 };
	case 12: return { 0,1,0,1 };
	case 13: return { 1,0,0,1 };
	case 14: return { 0,0,0,0 };
	case 15: return { 0,1,1,1 };
	default: return { 0 };
	}
}

vector<bool> KeyGeneration(vector<bool> base, vector<bool> seed)
{
	vector<bool> temp, temp2;
	temp.reserve(16);
	temp += vector<bool>{ base.begin(), base.begin() + 4 } ^ seed;
	temp2 += vector<bool>{temp.begin(), temp.begin() + 4} ^ vector<bool>{base.begin() + 8, base.begin() + 12};
	temp += vector<bool>{temp2.begin(), temp2.begin() + 4} ^ vector<bool>{base.begin() + 4, base.begin() + 8};
	temp += temp2;
	temp += vector<bool>{temp.begin() + 4, temp.begin() + 8} ^ vector<bool>{base.begin() + 12, base.begin() + 16};

	return temp;
}

int main()
{
	vector<bool> BitsOfText, Keyp, Key1, Key2;
	vector<bool> R = { 1,0,0,1,1 };

	vector<vector<bool>> m = { vector<bool>{ 0,0,1,1 },vector<bool>{ 0,0,1,0 },vector<bool>{ 0,0,1,0 },vector<bool>{ 0,0,1,1 } };
	cout << "Please give 4 hex of key: ";
	InputThenHexToBits(Keyp, 4);
	PrintBitsInHex(Keyp);
	vector<vector<bool>> Keyp_By_4Bits = { vector<bool>{Keyp.begin(), Keyp.begin() + 4},vector<bool>{Keyp.begin() + 4, Keyp.begin() + 8},vector<bool>{Keyp.begin() + 8, Keyp.begin() + 12},vector<bool>{Keyp.begin() + 12, Keyp.begin() + 16} };

	cout << "Please give 4 hex of text: ";
	InputThenHexToBits(BitsOfText, 4);
	PrintBitsInHex(BitsOfText);
	vector<vector<bool>> BitsOfText_By_4Bits = { vector<bool>{BitsOfText.begin(), BitsOfText.begin() + 4},vector<bool>{BitsOfText.begin() + 4, BitsOfText.begin() + 8},vector<bool>{BitsOfText.begin() + 8, BitsOfText.begin() + 12},vector<bool>{BitsOfText.begin() + 12, BitsOfText.begin() + 16} };

	Key1 = KeyGeneration(Keyp, SBoxE(vector<bool>{Keyp.end() - 4, Keyp.end()}) ^ vector<bool>{ 0, 0, 0, 1 });
	vector<vector<bool>> Key1_By_4Bits = { vector<bool>{Key1.begin(), Key1.begin() + 4},vector<bool>{Key1.begin() + 4, Key1.begin() + 8},vector<bool>{Key1.begin() + 8, Key1.begin() + 12},vector<bool>{Key1.begin() + 12, Key1.begin() + 16} };
	cout << "Key1: ";
	PrintBitsInHex(Key1);

	Key2 = KeyGeneration(Key1, SBoxE(vector<bool>{Key1.end() - 4, Key1.end()}) ^ vector<bool>{ 0, 0, 1, 0 });
	vector<vector<bool>> Key2_By_4Bits = { vector<bool>{Key2.begin(), Key2.begin() + 4},vector<bool>{Key2.begin() + 4, Key2.begin() + 8},vector<bool>{Key2.begin() + 8, Key2.begin() + 12},vector<bool>{Key2.begin() + 12, Key2.begin() + 16} };
	cout << "Key2: ";
	PrintBitsInHex(Key2);

	BitsOfText_By_4Bits = BitsOfText_By_4Bits ^ Keyp_By_4Bits;
	cout << "Step 1: ";
	PrintBitsInHex(BitsOfText_By_4Bits);

	BitsOfText_By_4Bits = vector<vector<bool>>{ SBoxE(BitsOfText_By_4Bits[0]), SBoxE(BitsOfText_By_4Bits[1]), SBoxE(BitsOfText_By_4Bits[2]), SBoxE(BitsOfText_By_4Bits[3]) };
	cout << "Step 2: ";
	PrintBitsInHex(BitsOfText_By_4Bits);

	BitsOfText_By_4Bits = vector<vector<bool>>{ BitsOfText_By_4Bits[0], BitsOfText_By_4Bits[1], BitsOfText_By_4Bits[3], BitsOfText_By_4Bits[2] };
	cout << "Step 3: ";
	PrintBitsInHex(BitsOfText_By_4Bits);

	BitsOfText_By_4Bits = vector<vector<bool>>{
		((BitsOfText_By_4Bits[0] * m[0]) % R) ^ ((BitsOfText_By_4Bits[2] * m[1]) % R),	((BitsOfText_By_4Bits[1] * m[0]) % R) ^ ((BitsOfText_By_4Bits[3] * m[1]) % R),
		((BitsOfText_By_4Bits[0] * m[2]) % R) ^ ((BitsOfText_By_4Bits[2] * m[3]) % R),	((BitsOfText_By_4Bits[1] * m[2]) % R) ^ ((BitsOfText_By_4Bits[3] * m[3]) % R) };

	cout << "Step 4: ";
	PrintBitsInHex(BitsOfText_By_4Bits);

	BitsOfText_By_4Bits = BitsOfText_By_4Bits ^ Key1_By_4Bits;
	cout << "Step 5: ";
	PrintBitsInHex(BitsOfText_By_4Bits);

	BitsOfText_By_4Bits = vector<vector<bool>>{ SBoxE(BitsOfText_By_4Bits[0]), SBoxE(BitsOfText_By_4Bits[1]), SBoxE(BitsOfText_By_4Bits[2]), SBoxE(BitsOfText_By_4Bits[3]) };
	cout << "Step 6: ";
	PrintBitsInHex(BitsOfText_By_4Bits);

	BitsOfText_By_4Bits = vector<vector<bool>>{ BitsOfText_By_4Bits[0], BitsOfText_By_4Bits[1], BitsOfText_By_4Bits[3], BitsOfText_By_4Bits[2] };
	cout << "Step 7: ";
	PrintBitsInHex(BitsOfText_By_4Bits);
	
	BitsOfText_By_4Bits = BitsOfText_By_4Bits ^ Key2_By_4Bits;
	cout << "Final: ";
	PrintBitsInHex(BitsOfText_By_4Bits);

	return 0;
}
