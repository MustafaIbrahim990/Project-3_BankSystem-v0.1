#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
using namespace std;

const string ClientsFileName = "ClientsData.txt";

enum enMainMenuOptions { eShowClientList = 1, eAddNewClient = 2, eDeleteClient = 3, eUpDateClientInfo = 4, eFindClient = 5, eExit = 6 };

struct stClientData
{
	string AccountNumber = "";
	string PINCode = "";
	string Name = "";
	string Phone = "";
	double AccountBalance = 0;
	bool MarkForDelete = false;
	bool MarkForUpDate = false;
};

//Declaration.

void ShowMainMenuOptions(); 

//Show Client List.

string AnderScore(short NumberOfAndScore)
{
	string AndScore = "";

	for (int i = 0; i < NumberOfAndScore; i++)
	{
		AndScore = AndScore + "_";
	}
	return AndScore;
}

vector <string> SplitString(string str, string Sperator = "#//#")
{
	vector <string> vString;
	short Position = 0;
	string sWord = "";

	while ((Position = str.find(Sperator)) != std::string::npos)
	{
		sWord = str.substr(0, Position);

		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		str.erase(0, Position + Sperator.length());
	}

	if (str != "")
	{
		vString.push_back(str);
	}
	return vString;
}

stClientData ConvertRecordClientInOneLineFromFileTostClientData(string strClientData)
{
	stClientData ClientData;
	vector <string> vClientData;

	vClientData = SplitString(strClientData);

	ClientData.AccountNumber = vClientData[0];
	ClientData.PINCode = vClientData[1];
	ClientData.Name = vClientData[2];
	ClientData.Phone = vClientData[3];
	ClientData.AccountBalance = stod(vClientData[4]);

	return ClientData;
}

vector <stClientData> LoadRecordClientInOneLineFromFileTovClientData(string FileName)
{
	vector <stClientData> vClientData;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line = "";
		stClientData ClientData;

		while (getline(MyFile, Line))
		{
			ClientData = ConvertRecordClientInOneLineFromFileTostClientData(Line);
			vClientData.push_back(ClientData);
		}
		MyFile.close();
	}
	return vClientData;
}

void PrintClientDataCardAtTabel(stClientData ClientData)
{
	cout << "| " << setw(18) << left << ClientData.AccountNumber;
	cout << "| " << setw(14) << left << ClientData.PINCode;
	cout << "| " << setw(40) << left << ClientData.Name;
	cout << "| " << setw(14) << left << ClientData.Phone;
	cout << "| " << setw(14) << left << ClientData.AccountBalance;
}

void PrintAllClientDataAtTable(vector <stClientData> vClientData)
{
	cout << "\n\n\t\t\t\t\t Client List (" << vClientData.size() << ") Client(s).\n";
	cout << AnderScore(113) << endl << endl;
	cout << "| " << setw(18) << left << "Account Number";
	cout << "| " << setw(14) << left << "PIN Code";
	cout << "| " << setw(40) << left << "Client Name";
	cout << "| " << setw(14) << left << "Phone";
	cout << "| " << setw(14) << left << "Account Balance" << endl;
	cout << AnderScore(113) << endl << endl;

	if (vClientData.size() == 0)
	{
		cout << "\n\t\t\t\t\tNo Clients Availble In The System.\n";
	}
	else
	{
		for (stClientData& ClData : vClientData)
		{
			PrintClientDataCardAtTabel(ClData);
			cout << endl;
		}
		cout << AnderScore(113) << endl << endl;
	}
}

void ShowClientListScreen()
{
	vector <stClientData> vClientData = LoadRecordClientInOneLineFromFileTovClientData(ClientsFileName);
	PrintAllClientDataAtTable(vClientData);
}

//Add New Client.

bool ClientExistsByAccountNumber(string FileName, string AccountNumber)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{
		string Line = "";
		stClientData ClientData;

		while (getline(MyFile, Line))
		{
			ClientData = ConvertRecordClientInOneLineFromFileTostClientData(Line);

			if (ClientData.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
		}
		MyFile.close();
	}
	return false;
}

stClientData ReadNewClient()
{
	stClientData ClientData;

	cout << "Enter Account Number? ";
	getline(cin >> ws, ClientData.AccountNumber);

	while (ClientExistsByAccountNumber(ClientsFileName, ClientData.AccountNumber))
	{
		cout << "\nClient With [" << ClientData.AccountNumber << "] Already Exists, Enter Another Account Number? ";
		getline(cin >> ws, ClientData.AccountNumber);
	}

	cout << "Enter PIN Code? ";
	getline(cin,ClientData.PINCode);

	cout << "Enter Your Name? ";
	getline(cin, ClientData.Name);

	cout << "Enter Your Phone? ";
	getline(cin, ClientData.Phone);

	cout << "Enter Account Balance? ";
	cin >> ClientData.AccountBalance;

	return ClientData;
}

string ConvertRecordClientToOneLine(stClientData ClientData, string Sperator = "#//#")
{
	string strClientData = "";

	strClientData += ClientData.AccountNumber + Sperator;
	strClientData += ClientData.PINCode + Sperator;
	strClientData += ClientData.Name + Sperator;
	strClientData += ClientData.Phone + Sperator;
	strClientData += to_string(ClientData.AccountBalance);

	return strClientData;
}

void SaveRecordClientInOneLineToFile(string FileName, string strClientData)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << strClientData << endl;
		MyFile.close();
	}
}

void AddNewClient()
{
	vector <stClientData> vClientData = LoadRecordClientInOneLineFromFileTovClientData(ClientsFileName);
	stClientData ClientData = ReadNewClient();
	SaveRecordClientInOneLineToFile(ClientsFileName, ConvertRecordClientToOneLine(ClientData));
}

void AddNewClients()
{
	char AddMore = 'n';

	do
	{
		cout << "Adding New Client : \n\n";

		//system("Cls");
		AddNewClient();

		cout << "\n\nClient Added Successfully, Do You Want to Add More Clients? [Y/N]? ";
		cin >> AddMore;

		cout << "\n\n";

	} while (toupper(AddMore) == 'Y');
}

void ShowAddNewClientsScreen()
{
	cout << "--------------------------------------------------\n";
	cout << "\t\tAdd New Clients Screen\n";
	cout << "--------------------------------------------------\n";

	AddNewClients();
}

//Delete Client.

string ReadClientAccountNumber()
{
	string AccountNumber = "";

	cout << "\nPlease Enter Account Number? ";
	getline(cin >> ws, AccountNumber);
	return AccountNumber;
}

bool FindClientByAccountNumber(string AccountNumber, stClientData& ClientData, vector <stClientData> vClientData)
{
	for (stClientData& ClData : vClientData)
	{
		if (ClData.AccountNumber == AccountNumber)
		{
			ClientData = ClData;
			return true;
		}
	}
	return false;
}

void PrintClientDataCard(stClientData ClientData)
{
	cout << "The Following Are The Client Details : \n";

	cout << "--------------------------------------------------\n";
	cout << "Account Number    : " << ClientData.AccountNumber;
	cout << "\nPIN Code          : " << ClientData.PINCode;
	cout << "\nName              : " << ClientData.Name;
	cout << "\nPhone             : " << ClientData.Phone;
	cout << "\nAccount Balance   : " << ClientData.AccountBalance << " $ ";
	cout << "\n--------------------------------------------------\n";
}

bool MarkClientForDeleteByAccountNummber(string AccountNumber, vector <stClientData>& vClientData)
{
	for (stClientData& ClData : vClientData)
	{
		if (ClData.AccountNumber == AccountNumber)
		{
			ClData.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector <stClientData> SaveRecordClientInOneLineToFileAfterDeleteOrUpDate(string FileName, vector <stClientData> vClientData)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string strClientData = "";

	if (MyFile.is_open())
	{
		for (stClientData& ClData : vClientData)
		{
			if (ClData.MarkForDelete == false)
			{
				strClientData = ConvertRecordClientToOneLine(ClData);
				MyFile << strClientData << endl;
			}
		}
		MyFile.close();
	}
	return vClientData;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector <stClientData>& vClientData)
{
	stClientData ClientData;
	char ClientAnswer = 'n';

	cout << "\n\n";

	if (FindClientByAccountNumber(AccountNumber, ClientData, vClientData))
	{
		PrintClientDataCard(ClientData);

		cout << "\n\nAre You Sure You Want to Delete This Client? [Y/N]? ";
		cin >> ClientAnswer;

		if (toupper(ClientAnswer) == 'Y')
		{
			MarkClientForDeleteByAccountNummber(AccountNumber, vClientData);
			SaveRecordClientInOneLineToFileAfterDeleteOrUpDate(ClientsFileName, vClientData);
			vClientData = LoadRecordClientInOneLineFromFileTovClientData(ClientsFileName);

			cout << "\nClient Delete Successfully.";
			return true;
		}
	}
	else
	{
		cout << "Client With [" << AccountNumber << "] Not Found!";
	}
	return false;
}

void ShowDeleteClientScreen()
{
	cout << "--------------------------------------------------\n";
	cout << "\t\tDelete Client Screen\n";
	cout << "--------------------------------------------------\n";

	vector <stClientData> vClientData = LoadRecordClientInOneLineFromFileTovClientData(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClientData);
}

//UpDate Client Info.

stClientData UpDateClientInfo(string AccountNumber)
{
	stClientData ClientData;

	ClientData.AccountNumber = AccountNumber;

	cout << "Enter PIN Code? ";
	getline(cin >> ws, ClientData.PINCode);

	cout << "Enter Your Name? ";
	getline(cin, ClientData.Name);

	cout << "Enter Your Phone? ";
	getline(cin, ClientData.Phone);

	cout << "Enter Account Balance? ";
	cin >> ClientData.AccountBalance;

	return ClientData;
}

bool UpDateClientByAccountNumber(string AccountNumber, vector <stClientData>& vClientData)
{
	stClientData ClientData;
	char ClientAnswer = 'n';

	cout << "\n\n";

	if (FindClientByAccountNumber(AccountNumber, ClientData, vClientData))
	{
		PrintClientDataCard(ClientData);

		cout << "\n\nAre You Sure You Want to UpDate This Client? [Y/N]? ";
		cin >> ClientAnswer;

		if (toupper(ClientAnswer) == 'Y')
		{
			cout << "\n\n";

			for (stClientData& ClData : vClientData)
			{
				if (ClData.AccountNumber == AccountNumber)
				{
					ClData = UpDateClientInfo(AccountNumber);
					break;
				}
			}
			SaveRecordClientInOneLineToFileAfterDeleteOrUpDate(ClientsFileName, vClientData);

			cout << "\nClient Delete Successfully.";
			return true;
		}
	}
	else
	{
		cout << "Client With [" << AccountNumber << "] Not Found!";
	}
	return false;
}

void ShowUpDateClientInfoScreen()
{
	cout << "--------------------------------------------------\n";
	cout << "\t\tUpDate Client Screen\n";
	cout << "--------------------------------------------------\n";

	vector <stClientData> vClientData = LoadRecordClientInOneLineFromFileTovClientData(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpDateClientByAccountNumber(AccountNumber, vClientData);
}

//Find Client.

void ShowFindClientScreen()
{
	cout << "--------------------------------------------------\n";
	cout << "\t\tFind Client Screen\n";
	cout << "--------------------------------------------------\n";

	stClientData ClientData;
	vector <stClientData> vClientData = LoadRecordClientInOneLineFromFileTovClientData(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	
	cout << "\n\n";

	if (FindClientByAccountNumber(AccountNumber, ClientData, vClientData))
	{
		PrintClientDataCard(ClientData);
	}
	else
	{
		cout << "Client With [" << AccountNumber << "] Not Found!";
	}
}

//Exit.

void ShowEndScreen()
{
	cout << "\n--------------------------------------------------\n";
	cout << "\t\tEnds Program : -)\n";
	cout << "--------------------------------------------------\n";
}

//Show Main Menu.

void GoBackToMainMenu()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenuOptions();
}

short ReadMainMenuOptions()
{
	short ClientChoose = 0;

	cout << "Choose What Do You Want to Do? [1 to 6]? ";
	cin >> ClientChoose;
	return ClientChoose;
}

void PerformMainMenuOptions(enMainMenuOptions MainMenuOptions)
{
	switch (MainMenuOptions)
	{
	case enMainMenuOptions::eShowClientList:
	{
		system("Cls");
		ShowClientListScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eAddNewClient:
	{
		system("Cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eDeleteClient:
	{
		system("Cls");
		ShowDeleteClientScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eUpDateClientInfo:
	{
		system("Cls");
		ShowUpDateClientInfoScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eFindClient:
	{
		system("Cls");
		ShowFindClientScreen();
		GoBackToMainMenu();
		break;
	}
	case enMainMenuOptions::eExit:
	{
		system("Cls");
		ShowEndScreen();
		break;
	}
	}
}

void ShowMainMenuOptions()
{
	system("Cls");
	cout << "==================================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "==================================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] UpDate Client.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Exit.\n";
	cout << "==================================================\n";

	PerformMainMenuOptions((enMainMenuOptions)ReadMainMenuOptions());
}

int main()
{
	ShowMainMenuOptions();
	system("pause>0");
	return 0;
}