#include "BoardMessageControl.h"
#include<string>
#include "windows.h"
#include<iomanip>
using namespace std;
void printNum(int *temp);

BoardMessageControl::BoardMessageControl()
{
	Pma = new PokerModelAnalysis();
}

BoardMessageControl::~BoardMessageControl()
{
	delete(engine);
}

//��ʼ���־���Ϣ
void BoardMessageControl::InitTurn(struct Ddz * pDdz)
{
	engine = new MonteCarloSearchEngine();
	CInfo = CardsInfo::Instance();
	pDdz->iTurnTotal =1;				//��ʼ��������
	pDdz->iTurnNow = 1;					//��ʼ����ǰ�ִ�
	pDdz->iStatus = 1;       //��ʼ����������״̬�����Ӹ���ֵ��ʹ��ʼ����һ���ܽ���������Ϣ��ѭ�����棩
	//Test
	if(!iFShowInConsole)
		pDdz->iRoundTotal = 100;			//��ʼ���ܾ���
	else
		pDdz->iRoundTotal = 1;			//��ʼ���ܾ���

	pDdz->dWinCount = 0;              //��ʼ��AIӮ�ľ���

	pDdz->iRoundNow = 1;				//��ʼ����ǰ�ִ�

	InitRound(pDdz);					//��ʼ����������
}
//P01-END

//M04-START���ñ������ݳ�ʼֵ
//��һ���޶���:������ ��һ���޶�ʱ��:16-07-31
//�޸�Ŀ��:otherPlayerCards��ʼ��
//����޶��ߣ���Ѷ�� ����޶�ʱ�䣺18-7-19
//�޸����ݼ�Ŀ�ģ���Ӱ汾�ŵĳ�ʼ��������iRoundNow=1��������汾�ű���γ�ʼ��
void BoardMessageControl::InitRound(struct Ddz * pDdz)
{
	int i, j;
	pDdz->iStatus = 1;					//��ʼ����������״̬
	strcpy_s(pDdz->sCommandIn, "");		//��ʼ������ͨ����������
	strcpy_s(pDdz->sCommandOut, "");		//��ʼ������ͨ���������
	for (i = 0; i < 21; i++)
	{		//��ʼ����������
		pDdz->iOnHand[i] = -1;
	}
	//��ʼ��otherPlayerCards
	for (i = 0; i < 54; i++)
	{
		pDdz->iOnOtherHand[i] = i;
	}
	pDdz->iOnOtherHand[54] = -1;
	for (i = 0; i<162; i++)				//��ʼ������������
		for (j = 0; j<21; j++)
			pDdz->iOnTable[i][j] = -2;
	for (i = 0; i < 21; i++)			//��ʼ�����ֳ���
		pDdz->iToTable[i] = -1;
	if (pDdz->iRoundNow==1)
	strcpy_s(pDdz->sVer, "");				//��ʼ������Э��汾��
	//strcpy_s(pDdz->sVer, kPlayerName);		//��ʼ�����ֲ���ѡ�ֳƺ�

	strcpy_s(pDdz->sName, kPlayerName);		//��ʼ�����ֲ���ѡ�ֳƺ�

	pDdz->cDir = 'B';						//��ʼ��������ҷ�λ���
	pDdz->cLandlord = 'B';				//��ʼ�����ֵ�����λ���
	pDdz->cWinner = 'B';					//��ʼ������ʤ�߷�λ���
	for (i = 0; i < 3; i++)				//��ʼ�����ֽ���
		pDdz->iBid[i] = -1;
	pDdz->iLastPassCount = 2;		//��ǰ��������PASS����ֵ��[0,2],��ֵ2����������ȡ0��һ��PASSȡ1������PASSȡ2��
	pDdz->iLastTypeCount = 0;		//��ǰ��������������ֵ��[0,1108],��ֵ0��iLastPassCount=0ʱ����ֵ��=1ʱ����ԭֵ��=2ʱֵΪ0��
	pDdz->iLastMainPoint = -1;		//��ǰ�������Ƶ�����ֵ��[0,15],��ֵ-1��iLastPassCount=0ʱ����ֵ����=1ʱ����ԭֵ��=2ʱֵΪ-1��
	pDdz->iBmax = 0;					//��ʼ�����ֽ��Ʒ���
	pDdz->iOTmax = 0;					//��ʼ����������������

	pDdz->haveHelpFriend = false;   //��¼�Ƿ���������ѣ�������ֻ��һ����ʱ���õ�����Ȩ������С���ƣ��ж϶����Ƿ������Լ�����С���ƣ��� ��ʼ��Ϊfalse
}
//MO4-END

//P02-START������Ϣ
//��һ���޶��ߣ�÷�գ���һ���޶�ʱ��:15-02-08
//����޶��ߣ���Ѷ�� ������޶�ʱ�䣺18-07-21
//�޶����ݣ������ڿ���̨�Ͻ��ж����Ϸ�Լ����ԵĹ���
//�Ż��߽���ƣ�������쳣�������
void BoardMessageControl::InputMsg(struct Ddz * pDdz)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	if (!iFShowInConsole)
	{
		//�˺�����һ�ζ�ȡ��������Ķ���ַ�(�����հ��ַ�����
		//����ָ���ĵ�ַΪ��ŵ�һ����ȡ���ַ���λ�ã���������Ŷ�ȡ���ַ���ֱ������N - 1��
		//��������ָ���Ľ�����Ϊֹ������ָ������������Ĭ�Ͻ�����Ϊ'\n'��	
		cin.getline(pDdz->sCommandIn, 80);//��ȡƽ̨�������Ϣ	
	}
	else
	{
	  //����־���Ϣ
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	  if (pDdz->iRoundNow > 0)
	  {
		  cout << endl << "�ܾ���Ϊ " << pDdz->iRoundTotal << " �֣���ǰΪ�� " << pDdz->iRoundNow << " ��\n"
		  << "A I Ӯ�� " << setiosflags(ios::fixed) << setprecision(0) << pDdz->dWinCount << " �֣�ʤ��Ϊ��";
	  if(pDdz->iRoundNow>1)
	      cout<< setiosflags(ios::fixed) <<setprecision(2)<< 100*((double)(pDdz->dWinCount) / (pDdz->iRoundNow-1)) << "%" << endl;
	  else 
		  cout << setiosflags(ios::fixed) << setprecision(2) << "0 %" << endl;
	  }
	  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	  //18-07-21
	
	  cout << endl << "��ʼ������ ������������ÿո������ֻ����0ΪPASS��J Q K A 2 С�� �����ֱ�Ϊ11 12 13 1 2 14 15�������0������" << endl << endl;
 {
		  
		//������
		int out[55];//�ƶ���������������
		int temp[55];//�ƶѣ�����һֱ�ڱ䣩
		int us[21];//���ǵ���
		int latter[21];//�¼ҵ���
		int former[21];//�ϼҵ���

		srand((unsigned)time(NULL));//��ʱ����Ϊ����

		//��ʼ����������
		for (int i = 0; i < 21; i++)
		{
			us[i] = -1;
			latter[i] = -1;
			former[i] = -1;
		}

		//���������
		if (isRandom){
		for (int i = 0; i < 54; i++)
		{
			temp[i] = i;//��������Ž��Ʊ��
		}
		for (int left = 53, i = 0; left>0; left--)//leftΪ�ƶ�ʣ������ 
		{
			int b = rand() % left;//���ѡȡһ����  ��0 <= b <= 52��
			out[i++] = temp[b];//out[0-52]
			//ȷ��ʣ������һֱ�ڼ��ٶ���ѡȡ���Ʋ��ظ�
			if (b != left) {
				temp[b] = temp[left];
			}
		}
		out[53] = temp[0];

		//��������ɵ�����ȡ�����ҵ���
		for (int i = 0; i < 51; i++)
		{
			if (i < 17) 
			{
				us[i] = out[i];
			}
			else if(i<34)
			{
				latter[i - 17] = out[i];
			}
			else 
			{
				former[i - 34] = out[i];
			}
		}
		}//isRandom

		//�̶������ƣ�������������Ҫ�ĳ�ʼ�ƣ�
		else{
			int fixation[52] = { 29, 19, 39, 38, 33, 18, 3, 44, 46, 22, 4, 53, 11, 49, 24, 28, 17,      //�ϼҵ���
				               43, 31, 36, 10, 35, 32, 30, 51, 13, 25, 47, 6, 1, 8, 26, 0, 37,    //AI����
							   41, 14, 21, 52, 48, 42, 20, 34, 9, 40, 2, 23, 16, 5, 27, 45, 7 };//�¼ҵ���
			for (int i = 0; i < 51; i++)
				{
					if (i < 17)
					{
						former[i] = fixation[i];
					}
					else if (i<34)
					{
						us[i - 17] = fixation[i];
					}
					else
					{
						latter[i - 34] = fixation[i];
					}
					out[i] = fixation[i];
				}
			//����
			out[51] = 15;
			out[52] = 12;
			out[53] = 50; 
		}
	
		//��ð����������ҵ����Ÿ���
		for (int pass = 17,j; pass >0; pass--)
		{
			for (int i = 0; i < pass-1; i++)
			{
				if (us[i] > us[i + 1]&& us[i+1]!=-1) {
					j = us[i];
					us[i] = us[i + 1];
					us[i + 1] = j;
				}
				if (latter[i] > latter[i + 1] && latter[i + 1] != -1) {
					j = latter[i];
					latter[i] = latter[i + 1];
					latter[i + 1] = j;
				}
				if (former[i] > former[i + 1] && former[i + 1] != -1) {
					j = former[i];
					former[i] = former[i + 1];
					former[i + 1] = j;
				}
			}
		}

	
		//������ת�����ַ���
		char sCard[4] = "";
		char sCardString[90] = "";
		strcpy_s(sCardString, "DEAL B");//����cat�޸�Ϊcpy��
		for (int i = 0; i < 17; i++)
		{
			if (us[i] >= 10) {
				sCard[0] = us[i] / 10 + '0';
				sCard[1] = us[i] % 10 + '0';
				sCard[2] = ',';
				sCard[3] = '\0';
			}
			else {
				sCard[0] = us[i] % 10 + '0';
				sCard[1] = ',';
				sCard[2] = '\0';
			}
			strcat_s(sCardString, sCard);
		}
		sCardString[strlen(sCardString) - 1] = '\0';//ȥ�������β������
		strcpy(pDdz->sCommandIn, sCardString);//�������AI�õ����Ƶı��루��cat�޸�Ϊcpy��
	

		//������ҵ�����
		if(iFShowOtherCard)
		{
			cout << "�ϼҵ�����";
			printCard(former);
			printNum(former);

			cout << "\nAI������  ";
			printCard(us);
			printNum(us);

			cout << "\n�¼ҵ�����";
			printCard(latter);
			printNum(latter);
		}
		
		
		//AI�õ��ƺ�������洢���ش�ƽ̨
		AnalyzeMsg(pDdz);
		OutputMsg(pDdz);
		//OK DEAL
		cout << endl;
		
		//�з�
		//�ϼҽз�
		if (!iFShowOtherCard) {
			cout << "\n�ϼҵ�����";
			printCard(former);
		}
		
		cout << "�ϼҽз֣� ";
		int formerbid;
		cin >> formerbid;

		char a[2];
		a[0]= formerbid % 10 + '0';//����ת��
		a[1] = '\0';
		strcpy(pDdz->sCommandIn,"BID A");//����
		strcat(pDdz->sCommandIn, a);//���������ַ���BID Aa 
		pDdz->sCommandIn[6] = '\0';
		AnalyzeMsg(pDdz);
		cout << endl;
		OutputMsg(pDdz);


		//AI�з�
		cout << "\nAI�з� ";
		strcpy(pDdz->sCommandIn, "BID WHAT");
		AnalyzeMsg(pDdz);
		cout << endl;
		OutputMsg(pDdz);
		//BID B_
		cout << endl;


		//�¼ҽз�
		if (!iFShowOtherCard) 
		{
			cout << "\n�¼ҵ�����";
			printCard(latter);
		}

		cout << "�¼ҽз֣� ";
		int latterbid;
		cin >> latterbid;

		char aa[2];
		aa[1] = '\0';
		aa[0] = latterbid % 10 + '0';
		strcpy(pDdz->sCommandIn, "BID C");
		strcat(pDdz->sCommandIn, aa);
		pDdz->sCommandIn[6] = '\0';
		AnalyzeMsg(pDdz);
		cout << endl;
		OutputMsg(pDdz);


		//��ʾ���ƣ��Լ��ж�˭�ǵ��� A B C
		int left[4];
		left[0] =out[51];
		left[1] = out[52];
		left[2] = out[53];
		left[3] = -1;

		//�ѵ���ת�����ַ���
		char info[90] = "";
		for (int i = 51; i < 54; i++)
		{
			if (out[i] >= 10) {
				sCard[0] = out[i] / 10 + '0';
				sCard[1] = out[i] % 10 + '0';
				sCard[2] = ',';
				sCard[3] = '\0';
			}
			else {
				sCard[0] = out[i] % 10 + '0';
				sCard[1] = ',';
				sCard[2] = '\0';
			}
			strcat_s(info, sCard);
		}

		if(strlen(info)>=1)
			info[strlen(info) - 1] = '\0';//ȥ�������β������
	
		//�ѵ��Ʒָ�����������˽зֵ�Լ����1����Ϊ0û�˻��Ϊ������2�����߽з�Ҫ��ǰ�߸߻�Ϊ0��
		
		//AIΪ����
		if (aiBid > latterbid && aiBid >= formerbid && aiBid !=0) {

			strcpy(pDdz->sCommandIn, "LEFTOVER B");
			strcat_s(pDdz->sCommandIn, info);
			
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << endl;
			cout << "                                                       AIΪ����������Ϊ";	
			printCard(left);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			AnalyzeMsg(pDdz);

			OutputMsg(pDdz);

			who = 1;
			who2 = 1;
			Pma->Gailvbiao(out, 2);	
		}

		//�¼�Ϊ����
		else if (latterbid >= aiBid && latterbid > formerbid && latterbid!=0) {
			strcpy(pDdz->sCommandIn, "LEFTOVER C");
			strcat_s(pDdz->sCommandIn, info);

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << endl;
			cout << "                                                       �¼�Ϊ����������Ϊ";
			
			printCard(left);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			AnalyzeMsg(pDdz);
			OutputMsg(pDdz);

			latter[17] = out[51];
			latter[18] = out[52];
			latter[19] = out[53];
			who = 2;
			who2 = 2;
			Pma->Gailvbiao(out, 3);
		}

		//�ϼ�Ϊ����
		else if (formerbid > aiBid && formerbid > latterbid && formerbid !=0) {
			strcpy(pDdz->sCommandIn, "LEFTOVER A");
			strcat_s(pDdz->sCommandIn, info);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << endl;
			cout << "                                                       �ϼ�Ϊ����������Ϊ";
			
			printCard(left);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			AnalyzeMsg(pDdz);
			OutputMsg(pDdz);

			former[17] = out[51];
			former[18] = out[52];
			former[19] = out[53];
			who = 0;
			who2 = 0;
			Pma->Gailvbiao(out, 1);
		}

		//������ƺ��������Ϣ
		if (iFShowOtherCard)
		{
			cout << endl;
			cout << "�ϼҵ�����";
			printCard(former);
			cout << "A I ������";
			printCard(pDdz->iOnHand);
			cout << "�¼ҵ�����";
			printCard(latter);
			
		}

		SplitCard *pSplitCards = new SplitCard();
		int **result = pSplitCards->startSplitCard(pDdz->iOnHand);

		while (pDdz->iStatus != 0)
		{
			int outOfHand[21];//������

			//�����е���
		if ((aiBid == 0 || aiBid == '/') && formerbid == 0 && latterbid == 0){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			strcpy(pDdz->sCommandIn, "GAMEOVER D");
			break;
		}

			//��ʼ������
			for (int i = 0; i < 21; i++)
			{
				outOfHand[i] = -1;
			}

			//�ֵ��ϼҳ���
			if (who == 0) {
				if (!iFShowOtherCard)
				{
					cout << endl << "�ϼҵ����� ";
					printCard(former);
				}
			
				cout << "�����ϼҳ��ƣ� ";
				int a[21];  //�洢Ҫ������
				for (int i = 0; i < 20; i++)
				{
					a[i] = -1;
				}

				int i = 0, b=-1, k = 0; //�޸ģ�b����ʼֵΪ-1

				//�ϼҳ���
				while (cin >> b) {   //����Ҫ������
					if (b != 0 && b>0 && b<16) {
						a[i++] = b;  //i��ֵ���,���˼���i��Ϊ��
					}
					else {    //bΪ0����ѭ��
						break;
					}
				}

				if (i == 0) {   //������:����b=0,i������,��ʱiΪ0
					strcpy(pDdz->sCommandIn, "PLAY A-1");

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
					cout << "                                                       �ϼ�PASS  " << endl;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
					//cout << " MSGtoAI " << pDdz->sCommandIn << endl;
					AnalyzeMsg(pDdz);		//����������Ϣ
					OutputMsg(pDdz);		//�����Ϣ
					cout << endl;
					who = 1;
					continue;
				}
				
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				    cout << "                                                       �ϼҳ��� �� ";

				//��ʾ�ճ�����
				for (int i = 0; a[i] > -1; i++) {
					if (a[i] == 1)cout << "A ";
					else if(a[i]==11)cout <<"J ";
					else if (a[i] == 12)cout << "Q ";
					else if (a[i] == 13)cout << "K ";
					else if (a[i] == 14)cout << "С�� ";
					else if (a[i] == 15)cout << "���� ";
					else cout << a[i] << " ";
				}
				cout << endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				
				//ȥ���ճ�����
				for (int i = 0; a[i] != -1; i++) {   //�Ӹճ���������һ��������
					for (int j = 0; former[j] != -1; j++) {  //��������������
						if (former[j] / 4 == 0)//��ֵΪ3
						{                          //�����˳��??�ȼ������ƶ����ǳ��˵���?�������ĸ���,�����ڹ�����������,�ȱ����j
							if (a[i] == 3) {   //������������3
								outOfHand[k++] = former[j]; //�������Ƽ���������ֵΪ3����
								//	cout << " find 3";
								former[j] = -1000;      //��������������ȥ��,Ϊʲô�Ǹ�-1000?
								break;
								//cout << "catch 3";
							}
							//cout << 3 << " ";    //3
						}

						else if (former[j] / 4 == 1)//��ֵΪ4
						{
							if (a[i] == 4) {
								outOfHand[k++] = former[j];
								//	cout << " find 4";
								former[j] = -1000;
								//cout << "catch 4";
								break;
							}
						}
						//cout << 4 << " ";   //4

						else if (former[j] / 4 == 2) {
							if (a[i] == 5) {
								outOfHand[k++] = former[j];
								//	cout << " find 5";
								former[j] = -1000;
								break;
								//cout << "catch 5";
							}
							//cout << 5 << " ";   //5
						}
						else if (former[j] / 4 == 3)
						{
							if (a[i] == 6) {
								outOfHand[k++] = former[j];
								//cout << " find 6";
								former[j] = -1000;
								break;
							}
							//cout << 6 << " ";    //6
						}
						else if (former[j] / 4 == 4) {
							if (a[i] == 7) {
								outOfHand[k++] = former[j];
								//cout << " find 7";
								former[j] = -1000;
								break;
							}
							//cout << 7 << " ";    //7
						}
						else if (former[j] / 4 == 5) {
							if (a[i] == 8) {
								outOfHand[k++] = former[j];
								//	cout << " find 8";
								former[j] = -1000;
								break;
							}
							//cout << 8 << " ";    //8
						}
						else if (former[j] / 4 == 6) {
							if (a[i] == 9) {
								outOfHand[k++] = former[j];
								//	cout << " find 9";
								former[j] = -1000;
								break;
							}
							//cout << 9 << " ";   //9
						}
						else if (former[j] / 4 == 7)
						{
							if (a[i] == 10) {
								outOfHand[k++] = former[j];
								//			cout << " find 10";
								former[j] = -1000;
								break;
							}
							//cout << 10 << " ";    //10
						}
						else if (former[j] / 4 == 8)
						{
							if (a[i] == 11) {
								outOfHand[k++] = former[j];
								//		cout << " find J";
								former[j] = -1000;
								break;
							}
							//cout << " J ";    //J
						}
						else if (former[j] / 4 == 9) {
							if (a[i] == 12) {
								outOfHand[k++] = former[j];
								//	cout << " find Q";
								former[j] = -1000;

								break;
							}
							//cout << " Q ";    //Q
						}
						else if (former[j] / 4 == 10) {
							if (a[i] == 13) {
								outOfHand[k++] = former[j];
								//			cout << " find K";
								former[j] = -1000;
								break;
							}
							//cout << " K ";   //K
						}
						else if (former[j] / 4 == 11)
						{
							if (a[i] == 1) {
								outOfHand[k++] = former[j];
								//			cout << " find A";
								former[j] = -1000;
								break;
							}
							//cout << " A ";    //A
						}
						else if (former[j] / 4 == 12)
						{
							if (a[i] == 2) {
								outOfHand[k++] = former[j];
								//				cout << " find 2";
								former[j] = -1000;
								break;
							}
							//cout << 2 << " ";   //2
						}
						else if (former[j] == 52)
						{
							if (a[i] == 14) {
								outOfHand[k++] = former[j];
								//					cout << " find С��";
								former[j] = -1000;
								break;
							}
							//cout << " С�� ";    //С��
						}
						else if (former[j] == 53)
						{
							if (a[i] == 15) {
								outOfHand[k++] = former[j];
								//					cout << " find ����";
								former[j] = -1000;
								break;
							}
							//cout << " ���� ";   //����
						}
					}
				}

				//�ѳ�����ת��Ϊ�ַ���������ƽ̨��Ϣ
				char info[90] = "";
				for (int i = 0; i <k; i++)
				{
					if (outOfHand[i] >= 10) {
						sCard[0] = outOfHand[i] / 10 + '0';
						sCard[1] = outOfHand[i] % 10 + '0';
						sCard[2] = ',';
						sCard[3] = '\0';
					}
					else {
						sCard[0] = outOfHand[i] % 10 + '0';
						sCard[1] = ',';
						sCard[2] = '\0';
					}
					strcat_s(info, sCard);
				}

				if (strlen(info) >= 1)
				info[strlen(info) - 1] = '\0';//ȥ�������β������

				strcpy(pDdz->sCommandIn, "PLAY A");
				strcat_s(pDdz->sCommandIn, info);

				//cout << " MSGtoAI " << pDdz->sCommandIn << endl;
				AnalyzeMsg(pDdz);		//����������Ϣ
				OutputMsg(pDdz);		//�����Ϣ
					who = 1;//׼����AI����
					Pma->Change(outOfHand, 1);

			}//�ֵ��ϼҳ���
 
			//�ֵ�AI����
			else if (who == 1) {

			//	cout << pDdz->sCommandIn << endl;
				cout << "AI thinking..." << endl<<endl;
				
				strcpy(pDdz->sCommandIn, "PLAY WHAT");//ת��AI���ǳ����㷨

			//	cout << " MSGtoAI " << pDdz->sCommandIn << endl;
				AnalyzeMsg(pDdz);		//����������Ϣ
				OutputMsg(pDdz);		//�����Ϣ

				who = 2;//׼�����¼ҳ���
			}

			//�ֵ��¼ҳ��ƣ�ͬ�ϼҳ��ƴ���һ��
			else if (who == 2) {
				if (!iFShowOtherCard)
				{
					cout << endl << "�¼ҵ����� ";
					printCard(latter);
				}
				cout << "�����¼ҳ��ƣ� ";
					int a[21];
					for (int i = 0; i < 20; i++)
					{
						a[i] = -1;
					}

					int i = 0, b, k = 0;

					while (cin >> b) {
						if (b != 0 && b>0 && b<16) {
							a[i++] = b;
						}
						else {
							break;
						}
					}

					//������
					if (i == 0) {
						strcpy(pDdz->sCommandIn, "PLAY C-1");
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
						cout << "                                                       �¼�PASS " << endl;
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
						//cout << " MSGtoAI " << pDdz->sCommandIn << endl;
						AnalyzeMsg(pDdz);		//����������Ϣ
						OutputMsg(pDdz);		//�����Ϣ
						cout << endl;
						who = 0;
						continue;
					}

					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
					cout << "                                                       �¼ҳ��� �� ";

					//���ճ�����,����ʾ���˵���
					for (int i = 0; a[i] > -1; i++) {
						if (a[i] == 1)cout << "A ";
						else if (a[i] == 11)cout << "J ";
						else if (a[i] ==12)cout << "Q ";
						else if (a[i] ==13)cout << "K ";
						else if (a[i] == 14)cout << "С�� ";
						else if (a[i] == 15)cout << "���� ";
						else cout << a[i] << " ";
					}
					cout << endl;
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				
					//��ʾ�����е��Ʊ��
					/*cout << endl<< "�¼��Ʊ���" << endl;
					for (int i = 0; latter[i]!=-1; i++)
					{
						cout << latter[i] << " ";
					}
					*/

					//��������ȥ���ճ�������
					for (int i = 0; a[i] != -1; i++) {
						for (int j = 0; latter[j] != -1; j++) {
							if (latter[j] / 4 == 0)
							{
								if (a[i] == 3) {
									outOfHand[k++] = latter[j];
								//	cout << " find 3";
									latter[j] = -1000;
									break;
									//cout << "catch 3";
								}
								//cout << 3 << " ";    //3
							}

							else if (latter[j] / 4 == 1)
							{
								if (a[i] == 4) {
									outOfHand[k++] = latter[j];
								//	cout << " find 4";
									latter[j] = -1000;
									//cout << "catch 4";
									break;
								}
							}
							//cout << 4 << " ";   //4
							else if (latter[j] / 4 == 2) {
								if (a[i] == 5) {
									outOfHand[k++] = latter[j];
								//	cout << " find 5";
									latter[j] = -1000;
									break;
									//cout << "catch 5";
								}
								//cout << 5 << " ";   //5
							}
							else if (latter[j] / 4 == 3)
							{
								if (a[i] == 6) {
									outOfHand[k++] = latter[j];
									//cout << " find 6";
									latter[j] = -1000;
									break;
								}
								//cout << 6 << " ";    //6
							}
							else if (latter[j] / 4 == 4) {
								if (a[i] == 7) {
									outOfHand[k++] = latter[j];
									//cout << " find 7";
									latter[j] = -1000;
									break;
								}
								//cout << 7 << " ";    //7
							}
							else if (latter[j] / 4 == 5) {
								if (a[i] == 8) {
									outOfHand[k++] = latter[j];
								//	cout << " find 8";
									latter[j] = -1000;
									break;
								}
								//cout << 8 << " ";    //8
							}
							else if (latter[j] / 4 == 6) {
								if (a[i] == 9) {
									outOfHand[k++] = latter[j];
								//	cout << " find 9";
									latter[j] = -1000;
									break;
								}
								//cout << 9 << " ";   //9
							}
							else if (latter[j] / 4 == 7)
							{
								if (a[i] == 10) {
									outOfHand[k++] = latter[j];
						//			cout << " find 10";
									latter[j] = -1000;
									break;
								}
								//cout << 10 << " ";    //10
							}
							else if (latter[j] / 4 == 8)
							{
								if (a[i] == 11) {
									outOfHand[k++] = latter[j];
							//		cout << " find J";
									latter[j] = -1000;
									break;
								}
								//cout << " J ";    //J
							}
							else if (latter[j] / 4 == 9) {
								if (a[i] == 12) {
									outOfHand[k++] = latter[j];
								//	cout << " find Q";
									latter[j] = -1000;

									break;
								}
								//cout << " Q ";    //Q
							}
							else if (latter[j] / 4 == 10) {
								if (a[i] == 13) {
									outOfHand[k++] = latter[j];
						//			cout << " find K";
									latter[j] = -1000;
									break;
								}
								//cout << " K ";   //K
							}
							else if (latter[j] / 4 == 11)
							{
								if (a[i] == 1) {
									outOfHand[k++] = latter[j];
						//			cout << " find A";
									latter[j] = -1000;
									break;
								}
								//cout << " A ";    //A
							}
							else if (latter[j] / 4 == 12)
							{
								if (a[i] == 2) {
									outOfHand[k++] = latter[j];
					//				cout << " find 2";
									latter[j] = -1000;
									break;
								}
								//cout << 2 << " ";   //2
							}
							else if (latter[j] == 52)
							{
								if (a[i] == 14) {
									outOfHand[k++] = latter[j];
				//					cout << " find С��";
									latter[j] = -1000;
									break;
								}
								//cout << " С�� ";    //С��
							}
							else if (latter[j] == 53)
							{
								if (a[i] == 15) {
									outOfHand[k++] = latter[j];
				//					cout << " find ����";
									latter[j] = -1000;
									break;
								}
								//cout << " ���� ";   //����
							}
						}
					}

					//�ѳ�����ת��Ϊ�ַ���������ƽ̨��Ϣ
					char info[90] = "";

				/*	for (int i = 0; i < 4; i++)
					{
						sCard[i] = '\0';
					}
					*/
					for (int i = 0; i <k; i++)
					{
						if (outOfHand[i] >= 10) {
							sCard[0] = outOfHand[i] / 10 + '0';
							sCard[1] = outOfHand[i] % 10 + '0';
							sCard[2] = ',';
							sCard[3] = '\0';
						}
						else {
							sCard[0] = outOfHand[i] % 10 + '0';
							sCard[1] = ',';
							sCard[2] = '\0';
						}
						strcat_s(info, sCard);
					}

					if (strlen(info) >= 1)
					info[strlen(info) - 1] = '\0';//ȥ�������β������

					strcpy(pDdz->sCommandIn, "PLAY C");
					strcat_s(pDdz->sCommandIn, info);

					//cout << "final" << pDdz->sCommandIn << endl;
					//cout << " MSGtoAI " << pDdz->sCommandIn << endl;
					AnalyzeMsg(pDdz);		//����������Ϣ
					OutputMsg(pDdz);		//�����Ϣ
					Pma->Change(outOfHand, 2);
					who = 0;
				}//�¼ҳ���
			
			//���ƺ���ʾ������Ϣ
			if (iFShowOtherCard)
			{
				cout << endl;
				cout << "�ϼҵ����� ";
				printCard(former);
				cout << "AI������   ";
				printCard(pDdz->iOnHand);
				cout << "�¼ҵ����� ";
				printCard(latter);

			}

			cout << "�ϼҵ�����: " << pDdz->iUpPlayerCards << "   �¼ҵ�����: " << pDdz->iDownPlayerCards;
			

			//����AI���ϵ�����
			int usCardCount=0;
			for (int i = 0; pDdz->iOnHand[i]!=-1; i++)
			{
				usCardCount++;
			}

			cout << "    A I ������: " << usCardCount ;

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			if (who2 == 0)cout << "      �ϼ�Ϊ���� "<<endl<<endl; 
			else if (who2 == 1)cout << "      AIΪ���� " << endl << endl;
			else if (who2 == 2)cout << "      �¼�Ϊ���� " << endl << endl;
			
		    //�ж�ʤ��
			if (pDdz->iUpPlayerCards == 0) {
				cout << endl<<"                                                       �ϼһ�ʤ  " << endl;
			//	cout << "				�ܹ������� " << pDdz->iRoundNow << " ��, AIӮ�� " << pDdz->dWinCount << " �֣� ʤ��Ϊ��" << pDdz->dWinCount / pDdz->iRoundNow <<"%"<< endl;

				strcpy(pDdz->sCommandIn, "GAMEOVER A");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			}
			else if (pDdz->iDownPlayerCards == 0) {
				cout << endl << "                                                       �¼һ�ʤ  " << endl;

				//cout << "				�ܹ������� " << pDdz->iRoundNow << " ��, AIӮ�� " << pDdz->dWinCount << " �֣� ʤ��Ϊ��" << pDdz->dWinCount / pDdz->iRoundNow << "%" << endl;
				strcpy(pDdz->sCommandIn, "GAMEOVER C");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			}
			else if (usCardCount == 0) {
				cout << endl << "                                                       AI��ʤ  " << endl;
				
				strcpy(pDdz->sCommandIn, "GAMEOVER B");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			}
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		}
	}
}	
//P02-END

//����Ƶı���
void printNum(int *temp){
	cout << "�Ƶı���Ϊ  ";
	for (int i = 0; temp[i] != -1; i++)
	if (temp[i + 1] != -1)
		cout << temp[i] << ",";
	else
		cout << temp[i];
	cout << endl<<endl;
}

//�ѱ��ת�����ƵĴ�С�������
void BoardMessageControl::printCard(int*temp) {

	int j;
	int a=0;
	for (int i = 0; temp[i]!=-1; i++)   //���ܸĳ�>-1�����˵����Ǹĳ�-1000�Ķ����ǵ�������
	{
		a++;

	}
	cout << "  ";
	//cout <<"����"<< a<<"���� ";

	for (int pass = a; pass >0; pass--)  //a��ѭ��
	{
		for (int i = 0; i < pass - 1; i++)
		{
			//����Ŵ�С����
			if (temp[i] > temp[i + 1]) {
				j = temp[i];
				temp[i] = temp[i + 1];
				temp[i + 1] = j;
			}
		}
	}

	//����Ƶ�ʵ�ʴ�С
	for (int i = 0; i<a; i++)
	{
		if (temp[i] / 4 == 0)
			cout << 3 << " ";    //3
		else if (temp[i] / 4 == 1)
			cout << 4 << " ";    //4
		else if (temp[i] / 4 == 2)
			cout << 5 << " ";    //5
		else if (temp[i] / 4 == 3)
			cout << 6 << " ";    //6
		else if (temp[i] / 4 == 4)
			cout << 7 << " ";    //7
		else if (temp[i] / 4 == 5)
			cout << 8 << " ";    //8
		else if (temp[i] / 4 == 6)
			cout << 9 << " ";    //9
		else if (temp[i] / 4 == 7)
			cout << 10 << " ";   //10
		else if (temp[i] / 4 == 8)
			cout << "J ";      //J
		else if (temp[i] / 4 == 9)
			cout << "Q ";        //Q
		else if (temp[i] / 4 == 10)
			cout << "K ";        //K
		else if (temp[i] / 4 == 11)
		{
			cout << "A ";        //A
		}
		else if (temp[i] / 4 == 12)
		{
			cout << 2 << " ";    //2
		}
		else if (temp[i] == 52)
		{
			cout << "С�� ";    //С��
		}
		else if (temp[i] == 53)
		{
			cout << "���� ";   //����
		}
		/*else if (temp[i] == -1000)
		{
			cout << temp[i] << " ";
		}*/
	}
	cout << endl<<endl;
}


//P0301-START��ȡ������汾��ϢDOU
//sCommandIn==DOUDIZHUVER 1.0
//����޶���:�ź���&÷��,����޶�ʱ��:15-02-10 21:04
//�޶����ݼ�Ŀ��:�޶�kPlayerNmae
void BoardMessageControl::GetDou(struct Ddz * pDdz)
{
	int i;								//��ѭ������
	for (i = 0; pDdz->sCommandIn[i] != '\0'; i++)
		pDdz->sVer[i] = pDdz->sCommandIn[i]; //��ƽ̨��õĵ�һ�а汾��Ϣ��DOUUDIZHUVER 1.0
	pDdz->sVer[i] = '\0';
	strcpy_s(pDdz->sCommandOut, "NAME ");
	strcat_s(pDdz->sCommandOut, kPlayerName);
}
//P0301-END

//P0302-START��ȡ�������־���ϢINF
//sCommandIn==INFO 1,4,1,6,9,2100
//��һ���޶���:�ų�&÷��,��һ���޶�ʱ��:15-02-10
//����޶���:��Ѷ��,����޶�ʱ��:18-7-20
//�޶����ݼ�Ŀ�ģ�ȥ��InitRound�������ظ����ã�����������
void BoardMessageControl::GetInf(struct Ddz * pDdz)		//�ִ���Ϣ������(����������Ϣ����Ӧд������Ա������):����INFO 1/4 1/9 9 2450     ���OK INFO
{
	char c;					//�浱ǰ�ֽ���Ϣ
	int iCount = 0;			//��¼���ݸ���
	int iTemp = 0;			//�м����
	int iMessage[7] = { 0 };		//��¼��������
	int i;
	//����Ϊ�ָ��,��һ�����־��������һλ,���������Ϊ��λ
	for (i = 5; pDdz->sCommandIn[i] != '\0'; i++)      //��i=5��ʼ  info 1,4,1,9,9,3150
	{
		c = pDdz->sCommandIn[i];
		if (c >= '0' && c <= '9')											//��ǰ�ַ�Ϊ0-9
		{
			iTemp = iTemp * 10 + c - '0';                    
			iMessage[iCount] = iTemp;								//����ѡ����
		}
		if (c == ',')														//��ǰ�ַ�Ϊ����,�ָ�����
		{
			iCount++;
			iTemp = 0;
		}
	}
	
	
	pDdz->iTurnNow = iMessage[0];						//��ǰ�ִ�
	pDdz->iTurnTotal = iMessage[1];						//������
	pDdz->iRoundNow = iMessage[2];						//��ǰ�ִ�
	pDdz->iRoundTotal = iMessage[3];					//�ܾ���
	pDdz->iLevelUp = iMessage[4];						//��������
	pDdz->iScoreMax = iMessage[5];						//�÷ֽ���
	//InitRound(pDdz);                    //��ʼ���Ƶ�����
	strcpy_s(pDdz->sCommandOut, "OK INFO");
}
//P0302-END

//P0303-START��ȡ������������ϢDEA
//sCommandIn==DEAL B0,4,5,7,9,10,17,21,25,33,34,39,41,43,44,45,46
//����޶���:������ ����޶�ʱ��:16-07-31 17:03
//�޶����ݼ�Ŀ��:���ӳ�ʼ�����ֺ���������������ı���,�����������������Ϣ
void BoardMessageControl::GetDea(struct Ddz * pDdz)
{
	int i;			      //��ѭ������
	int iNow = 0;		  //��ǰ���������
	int iCardId = 0;	//��ǰ�����Ʊ���
	char c;			      //��ǰָ���ַ�
	pDdz->cDir = pDdz->sCommandIn[5];     //��ȡ����AI��λ���
	if (pDdz->cDir == 'B')
	{
		cUpPlayer = 'A';
		cDownPlayer = 'C';
	}
	if (pDdz->cDir == 'A')
	{
		cUpPlayer = 'C';
		cDownPlayer = 'B';
	}
	if (pDdz->cDir == 'C')
	{
		cUpPlayer = 'B';
		cDownPlayer = 'A';
	}
	pDdz->cLastPlay = cUpPlayer;
	pDdz->iUpPlayerCards = 0;      //��ʼ��������
	pDdz->iDownPlayerCards = 0;

	for (i = 0; i < 21; i++)//����iOnhand[]
	{
		pDdz->iOnHand[i] = -1;
	}

	for (i = 6; pDdz->sCommandIn[i] != '\0'; i++)	//���ζ�ȡ����ָ��,��sCommandIn[6]��ʼ
	{
		c = pDdz->sCommandIn[i];			      //cΪ��ǰָ���ַ�
		if (c >= '0' && c <= '9')				        //��ǰ�ַ�Ϊ0-9
		{
			iCardId = iCardId * 10 + c - '0';
			pDdz->iOnHand[iNow] = iCardId;
		}
		if (c == ',')							            //��ǰ�ַ�Ϊ����,�ָ�����
		{
			iCardId = 0;
			iNow++;
		}
	}
	//CInfo->SortById(pDdz->iOnHand);     //������ȥ����
	OtherPlayerCards(pDdz, pDdz->iOnHand);//���˵������ܺ;��ǳ�ȥ�Լ�����
	strcpy_s(pDdz->sCommandOut, "OK DEAL");  //�ظ���Ϣ
	CInfo->SortById(pDdz->iOnHand);  //iOnHand[]��С��������
}
//P0303-END

//I02-START���㼺�����Ʋ��Խӿ�
int BoardMessageControl::CalBid(struct Ddz * pDdz)
{
	CallNum *CNum = new CallNum();
	int iMyBid = CNum->CallCardAnalyze(pDdz);		//����
	aiBid = iMyBid;
	delete(CNum);
	return iMyBid;
}

//P0304-START��ȡ�����������ϢBID
//����޶���:��˼��&÷��,����޶�ʱ��:15-02-08
void BoardMessageControl::GetBid(struct Ddz * pDdz)
{
	//AI�з�,sCommandIn==BID WHAT
	if (pDdz->sCommandIn[4] == 'W')					
	{
		strcpy_s(pDdz->sCommandOut, "BID _0");
		pDdz->sCommandOut[4] = pDdz->cDir;  //�зֵ����
		pDdz->iBid[pDdz->cDir - 'A'] = CalBid(pDdz);//���ý��ƺ��� ��ȡֵΪ0-3
		pDdz->sCommandOut[5] = pDdz->iBid[pDdz->cDir - 'A'] + '0';  //�еķ������ڲ�Ϊcharת��Ϊint���ⲿΪintת��Ϊchar���ͣ�
		//�������һ�����⣬���е�ʱ���п��ܱ�� BID B/��������BID B0
		pDdz->sCommandOut[6] = '\0';
		return;
	}
	//�˹��з�,sCommandIn==BID A/B/C*
	if (pDdz->sCommandIn[4] >= 'A'&&pDdz->sCommandIn[4] <= 'C')  //������ϢΪBID A/B/C
	{
		pDdz->iBid[pDdz->sCommandIn[4] - 'A'] = pDdz->sCommandIn[5] - '0';
		strcpy_s(pDdz->sCommandOut, "OK BID");
		return;
	}
}
//P0304-END

//P0305-START��ȡ�����������ϢLEF
//sCommandIn==LEFTOVER B27,48,53
//�ϴ��޶���:����&÷��,�ϴ��޶�ʱ��:15-02-08
//����޶��ߣ���Ѷ��������޶�ʱ�䣺18-7-20
//�޶����ݼ�Ŀ�ģ������ú�����������д����λ�ã��Ӷ��ı�if��������
//1��ֻҪ�����õ����ƣ�AI�͸���Ӧ����Ϣ������AI�õ��Ÿ���
//2��ֻ��AI�õ����ƲŽ����Ƶ��������򣬼���������
void BoardMessageControl::GetLef(struct Ddz * pDdz)
{
	int i, iCount = 0;
	char c;
	pDdz->cLandlord = pDdz->sCommandIn[9];    //ȷ��������
	//��ʼ������
	pDdz->iLef[0] = 0;
	pDdz->iLef[1] = 0;
	pDdz->iLef[2] = 0;
	//sCommandIn[9]Ϊ��λ,[10]Ϊ����
	for (i = 10; pDdz->sCommandIn[i] != '\0'; i++)
	{
		c = pDdz->sCommandIn[i];
		if (c >= '0' && c <= '9')
			pDdz->iLef[iCount] = pDdz->iLef[iCount] * 10 + c - '0';
		else
			iCount++;
	}
	//AIΪ����,������
	if (pDdz->sCommandIn[9] == pDdz->cDir)
	{
		pDdz->iOnHand[17] = pDdz->iLef[0];
		pDdz->iOnHand[18] = pDdz->iLef[1];
		pDdz->iOnHand[19] = pDdz->iLef[2];
		pDdz->iOnHand[20] = -1;
		OtherPlayerCards(pDdz, pDdz->iLef);
		pDdz->iUpPlayerCards = 17;
		pDdz->iDownPlayerCards = 17;
	}
	//�ϼ�Ϊ����
	if (pDdz->sCommandIn[9] == cUpPlayer)
	{
		pDdz->iUpPlayerCards = 20;
		pDdz->iDownPlayerCards = 17;
	}
	//�¼�Ϊ����
	if (pDdz->sCommandIn[9] == cDownPlayer)
	{
		pDdz->iUpPlayerCards = 17;
		pDdz->iDownPlayerCards = 20;
	}
	//AI�ظ��յ�������Ϣ
	if (pDdz->sCommandIn[9] == pDdz->cDir)
		CInfo->SortById(pDdz->iOnHand);					//iOnHand[]��С��������
	
	strcpy_s(pDdz->sCommandOut, "OK LEFTOVER");
	
	
}
//P0305-END

//P0306-START �������
//����޶���:������,����޶�ʱ��:16-07-31
//�޶�����:����iOnOtherHand
void BoardMessageControl::CalPla(struct Ddz * pDdz)
{
	//int iMax = 0;
	//SearchEngine *engine = new SearchEngine();

	//memset()����ԭ����extern void *memset(void *buffer, int c, int count)
	//buffer��Ϊָ���������,c���Ǹ���buffer��ֵ,count����buffer�ĳ���.
	//���������socket�ж������������.��:ԭ����memset(buffer, 0, sizeof(buffer))
	//��仰����forѭ������һ��,�����Ӽ��

	///////���itoTable/////
	memset(pDdz->iToTable,-1,sizeof(pDdz->iToTable));
	///////////////
	//�õ�Ҫ������
	engine->SearchAGoodMove(pDdz);
	int i=0;
	//���¼���Ҫ������
	CInfo->SortById(engine->ibestMove);
	for (i = 0; engine->ibestMove[i] >= 0; i++)
		pDdz->iToTable[i] = engine->ibestMove[i];
	pDdz->iToTable[i] = -1;//��-1��Ϊ���.


}

//P030602-START���ݼ������Ƹ�������
//����޶���:÷��&�ĺ��н�,����޶�ʱ��:15-03-01
//�޶����ݼ�Ŀ��:�޸ļ���������
void BoardMessageControl::UpdateMyPla(struct Ddz * pDdz)
{
	int i, j, k;
	if (pDdz->iToTable[0] == -1)	//Pass
	{
		pDdz->iOnTable[pDdz->iOTmax][0] = -1;//�޳���
		pDdz->iLastPassCount++;
		if (pDdz->iLastPassCount >= 2)	//��������PASS
		{
			pDdz->iLastPassCount = 0;
			pDdz->iLastTypeCount = 0;
			pDdz->iLastMainPoint = -1;
		}
		pDdz->iOTmax++;
	}
	else						//����PASS
	{
		//����������
		for (i = 0; pDdz->iToTable[i] >= 0; i++)
			pDdz->iOnTable[pDdz->iOTmax][i] = pDdz->iToTable[i];
		pDdz->iOnTable[pDdz->iOTmax][i] = -1;
		pDdz->iOTmax++;    //��ǰΪ���ֵڼ��γ���
		//����������
		for (j = 0; pDdz->iToTable[j] >= 0; j++)
		{
			for (i = 0; pDdz->iOnHand[i] >= 0; i++)
				if (pDdz->iOnHand[i] == pDdz->iToTable[j])
				{
					for (k = i; pDdz->iOnHand[k] >= 0; k++)
						pDdz->iOnHand[k] = pDdz->iOnHand[k + 1];
					break;
				}
		}
		pDdz->iLastPassCount = 0;
		pDdz->iLastTypeCount = CInfo->AnalyzeTypeCount(pDdz->iToTable); //�������ͺ�����
		pDdz->iLastMainPoint = CInfo->AnalyzeMainPoint(pDdz->iToTable); //����������С����
		pDdz->cLastPlay = pDdz->cDir;
	}
}
//PO30602-END

//P030603-START�������˳��Ƹ�������
//����޶���:������,����޶�ʱ��:16-07-31
//�޶����ݣ���¼�Ƶ�iOnOtherHand
void BoardMessageControl::UpdateHisPla(struct Ddz * pDdz)
{
	int i;
	int iCardId;
	int iNow;
	char c;
	pDdz->cLastPlay = pDdz->sCommandIn[5];
	//����������
	if (pDdz->sCommandIn[6] == '-')// PLAY ?-1 ��PASS
	{
		pDdz->iOnTable[pDdz->iOTmax][0] = -1;
		pDdz->iLastPassCount++;
		if (pDdz->iLastPassCount >= 2)	//��������PASS
		{
			pDdz->iLastPassCount = 0;
			pDdz->iLastTypeCount = 0;
			pDdz->iLastMainPoint = -1;
		}
		pDdz->iOTmax++;
	}
	else						// PLAY ����
	{
		for (i = 0; i<21; i++)							//����iToTable[]
			pDdz->iToTable[i] = -1;
		iCardId = 0;
		iNow = 0;
		for (i = 6; pDdz->sCommandIn[i] != '\0'; i++)		//���ζ�ȡ����
		{
			c = pDdz->sCommandIn[i];					//cΪ��ǰָ���ַ�
			if (c >= '0' && c <= '9')				        //��ǰ�ַ�Ϊ0-9
			{
				iCardId = iCardId * 10 + c - '0';
				pDdz->iToTable[iNow] = iCardId;
			}
			if (c == ',')									//��ǰ�ַ�Ϊ����,
			{
				iCardId = 0;
				iNow++;
			}
		}
		//����������
		for (i = 0; pDdz->iToTable[i] >= 0; i++)
		{
			pDdz->iOnTable[pDdz->iOTmax][i] = pDdz->iToTable[i];
		}
		pDdz->iOnTable[pDdz->iOTmax][i] = -1;//�ָ�

		//����iOnOtherHand��
		//����AI,ֻ֪���Լ����ơ����¼����������ƺͳ������ƣ��ڴ˻����϶����¼ҵ��ƽ����Ʋ�
		if (pDdz->cLastPlay == cUpPlayer)
			pDdz->iUpPlayerCards -= i;
		else if (pDdz->cLastPlay == cDownPlayer)
			pDdz->iDownPlayerCards -= i;

		OtherPlayerCards(pDdz, pDdz->iToTable);//ɾ���������ƣ�����������λ������е�ʣ��

		pDdz->iLastPassCount = 0;
		pDdz->iLastTypeCount = CInfo->AnalyzeTypeCount(pDdz->iToTable);
		pDdz->iLastMainPoint = CInfo->AnalyzeMainPoint(pDdz->iToTable);
		pDdz->iOTmax++;
	}
}
//PO30603-END

//P0306-START��ȡ�����������ϢPLA
//sCommandIn==PLAY WHAT
//����޶���:÷��,����޶�ʱ��:15-02-08
void BoardMessageControl::GetPla(struct Ddz * pDdz)
{
	if (pDdz->sCommandIn[5] == 'W')					//������ϢΪPLAY WHAT��Ӧ���ó��Ƽ��㺯���������
	{
		//cout << "calPla" << endl;
		CalPla(pDdz);					//���ó��Ƽ��㺯���������
		strcpy_s(pDdz->sCommandOut, "PLAY _");
		pDdz->sCommandOut[5] = pDdz->cDir;		//��������Ԥ����Ϣ׼����sCommandOut����

		CInfo->AppendCardsToS(pDdz->iToTable, pDdz->sCommandOut);		//Ҫ��������iToTable[]�е�����ת��Ϊ�ַ������ӵ�sCommandOut��
		
			if (iFShowInConsole)
			{
				//Test
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				if (pDdz->iToTable[0] != -1){
				cout << "\n                                                       AI ���� �� ";
				printCard(pDdz->iToTable);
				}
				else
					cout << "\n                                                       AI PASS"<<endl;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}

		UpdateMyPla(pDdz);		//���ݼ������Ƹ�������
	}
	else										//�����յ���ϢΪ������ҳ���
	{
		if (pDdz->sCommandIn[5] == pDdz->cDir)//�������
			return;
		UpdateHisPla(pDdz);		//�������˳��Ƹ�������
		strcpy_s(pDdz->sCommandOut, "OK PLAY");//�ظ��յ�
	}
	//��ǰ������1
	//pDdz->iOTmax++;
}
//P0306-END

//P0307-START��ȡ������ʤ����ϢGAM
//sCommandIn==GAMEOVER C
//sCommandOut == OK GAMEOVER
//��һ���޶���:÷��,��һ���޶�ʱ��:15-02-08 00:13
//����޶��ߣ���Ѷ��������޶�ʱ�䣺18-7-22
//�޶����ݣ������ڿ���̨�е��Ե��쳣�����Լ������Ϸ�����������Ϣ
void BoardMessageControl::GetGam(struct Ddz * pDdz)
{
	pDdz->cWinner = pDdz->sCommandIn[9];			//ʤ���߷�λ���

	//���˽е���
	if (iFShowInConsole){
		if (pDdz->cWinner == 'D'){
			InitRound(pDdz);							//����״̬������Ϊ���¿�ʼ��һ��
			cout << endl;
			strcpy_s(pDdz->sCommandOut, "���˽е��������¿���");
			pDdz->iRoundNow--;   //��ȥһ��
			pDdz->dWinCount--;
			OutputMsg(pDdz);		//�����Ϣ
		}
	}
		//Test �ۼ�ʤ��������dWinCount����
		if (pDdz->cDir != pDdz->cLandlord )            //AI���ǵ���
		{
			if (pDdz->cLandlord != pDdz->cWinner)   //����û��Ӯ
			{
				pDdz->dWinCount++;    //AIӮ�Ĵ�����һ
			}
		}
		else                    //AI�ǵ���
		{
			if (pDdz->cDir == pDdz->cWinner)   //AIӮ��
			{
				pDdz->dWinCount++;
			}
		}


		if (pDdz->iRoundNow == pDdz->iRoundTotal)		//�����ǰ������ÿ�־����ʱ
		{
			pDdz->iStatus = 0;							//����״̬������Ϊ����
			if (iFShowInConsole){
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				if (pDdz->iRoundNow > 0)
				{
					cout << endl << "�ܾ���Ϊ " << pDdz->iRoundTotal << " �֣���ǰ��Ϸ�ѽ���\n"
						<< "A I Ӯ�� " << setiosflags(ios::fixed) << setprecision(0) << pDdz->dWinCount << " �֣�ʤ��Ϊ��";
					if (pDdz->iRoundNow>1)
						cout << setiosflags(ios::fixed) << setprecision(2) << 100 * ((double)(pDdz->dWinCount) / (pDdz->iRoundTotal )) << "%" << endl;
					else
						cout << setiosflags(ios::fixed) << setprecision(2) << "0 %" << endl;
				}
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
				cout << endl;
				system("pause");
			}
		}
		else											//����
		{
			pDdz->iRoundNow++;							//��ǰ�ִμ�1
			InitRound(pDdz);							//����״̬������Ϊ���¿�ʼ��һ��
		}
		strcpy_s(pDdz->sCommandOut, "OK GAMEOVER");
	}

//PO307-END


//P03-START����������Ϣ
//����޶���:÷��,����޶�ʱ��:15-02-08 00:13
void BoardMessageControl::AnalyzeMsg(struct Ddz * pDdz)
{
	char sShort[4];
	int i;
	for (i = 0; i<3; i++)
		sShort[i] = pDdz->sCommandIn[i]; //[0-2]
	sShort[3] = '\0';
	if (strcmp(sShort, "DOU") == 0)					//�汾��Ϣ
	{
		GetDou(pDdz);
		return;
	}
	if (strcmp(sShort, "INF") == 0)					//�־���Ϣ
	{
		GetInf(pDdz);
		return;
	}
	if (strcmp(sShort, "DEA") == 0)					//������Ϣ
	{
		GetDea(pDdz);
		return;
	}
	if (strcmp(sShort, "BID") == 0)					//���ƹ���
	{
		GetBid(pDdz);
		return;
	}
	if (strcmp(sShort, "LEF") == 0)					//������Ϣ
	{
		GetLef(pDdz);
		return;
	}
	if (strcmp(sShort, "PLA") == 0)					//���ƹ���
	{
		GetPla(pDdz);
		return;
	}
	if (strcmp(sShort, "GAM") == 0)					//ʤ����Ϣ
	{
		GetGam(pDdz);
		return;
	}
	if (strcmp(sShort, "EXI") == 0)					//ǿ���˳�
	{
		exit(0);
	}
	
	strcpy_s(pDdz->sCommandOut, "ERROR at module AnalyzeMsg,sCommandIn without match");
	return;
}
//P03-END



//P04-START�����Ϣ
//����޶���:÷��,����޶�ʱ��:15-02-08 00:13
void BoardMessageControl::OutputMsg(struct Ddz * pDdz)
{
	//Test
	/*cout <<endl<< "LastPassCount " << pDdz->iLastPassCount << endl;
	cout << "LastMainPoint " << pDdz->iLastMainPoint << endl;
	cout << "LastTypeCount " << pDdz->iLastTypeCount << endl << endl;*/
	//cout����char[]�����أ�����һ��������ֻ���ַ������������������飬�����int������߱�����飬�򲻻ᡣ
	cout << pDdz->sCommandOut << endl;
}
//P04-END

//P05-START������������
//����޶���:÷��,����޶�ʱ��:15-02-08
/*void BoardMessageControl::CalOthers(struct Ddz * pDdz)
{
	pDdz->iVoid = 0;
}
//P05-END*/


//����Ҫɾ�����ƣ�������λ�������ʣ�ƣ����洢��iOnOtherHand��
//����޶��ߣ�������
//����޸�ʱ�䣺2016/7/31
//������
void BoardMessageControl::OtherPlayerCards(Ddz *pDdz, int *iCards)
{
	//���ú��������Ѿ��ź��������µ���
	auto deleteElem = [](int *iOnHand, int elem){  //auto����������������ʱ����ݱ�����ʼֵ�������Զ�Ϊ�˱���ѡ��ƥ�������
		int i = 0, j = 0;
		for (j = 0; j < 55; j++)
		{

			if (iOnHand[j] == elem)  //ƥ��
				i++;     
			else                   //��ƥ��
			{
				iOnHand[j - i] = iOnHand[j];  //ɾȥi����
			}
		}
	};
	
	//start
	int j;

	//�������
	if (iCards == pDdz->iLef)
	{
		for (j = 0; j < 3; j++)
		{
			deleteElem(pDdz->iOnOtherHand, iCards[j]);
		}
	}
	else
	{
		for (j = 0; iCards[j] >= 0; j++)
		{
			deleteElem(pDdz->iOnOtherHand, iCards[j]);
		}
	}
}