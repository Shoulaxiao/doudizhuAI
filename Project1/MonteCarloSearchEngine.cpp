#include "MonteCarloSearchEngine.h" 
#include <random> 
#include <iostream>
void printt(int*temp);
int iTest = 0;//����

/**
���Һ� �޸ģ�2018.7.29�û���λ��Ϣ���´���AI��Ӯ����������
**/
MonteCarloSearchEngine::MonteCarloSearchEngine()
{
	//��ʼ��������ӣ�����
	srand((unsigned)time(NULL));  

	pRecomCards = new RecommendCards(); 
	pMyDdzBoard = new fDdz(); 		// �Ƽ�����������  �ҷ�
	pUpDdzBoard = new fDdz();		//�ϼ�	
	pDownDdzBoard = new fDdz();		//�¼�
	pCinfo = new CardsInfo();    //�Ƶ���Ϣ
	//��ʼ����������
	AIWin = 0;
	UpWin = 0;
	DownWin = 0;
	nodeSeed = 0;
}
MonteCarloSearchEngine::~MonteCarloSearchEngine()
{
	delete(pRecomCards);
	delete(pMyDdzBoard);
	delete(pUpDdzBoard);
	delete(pDownDdzBoard);
	delete(pCinfo);
	//����ڵ�
	delete(nodesList);
}
//��ʼ����������
void MonteCarloSearchEngine::Init()
{
	pMyDdzBoard->haveHelpFriend = pRealDdz->haveHelpFriend;
	pUpDdzBoard->haveHelpFriend = pRealDdz->haveHelpFriend;
	pDownDdzBoard->haveHelpFriend = pRealDdz->haveHelpFriend;

	//��ʼ���̶�������Ϣ
	GetOtherPlayerCards(pUpDdzBoard->iOnHand,pDownDdzBoard->iOnHand);  
	pMyDdzBoard->cDir = pRealDdz->cDir;
	pMyDdzBoard->cLandlord = pRealDdz->cLandlord;
	pMyDdzBoard->cLastPlay = pRealDdz->cLastPlay;

	//��ʼ����λ��Ϣ  ///ȷ�����ҵķ�λ
	switch (pMyDdzBoard->cDir)
	{
	case 'A':
		pUpDdzBoard->cDir = 'C';
		pDownDdzBoard->cDir = 'B';
		break; 
	case 'B':
		pUpDdzBoard->cDir = 'A';
		pDownDdzBoard->cDir = 'C';
		break;
	case 'C':
		pUpDdzBoard->cDir = 'B';
		pDownDdzBoard->cDir = 'A';
		break;
	}



	pUpDdzBoard->cLastPlay = pDownDdzBoard->cDir;	//�ϼҵ��ϼҵ������ǵ��¼ҵķ�λ
	pUpDdzBoard->cLandlord = pRealDdz->cLandlord;	//�ϼҵĵ���
	pDownDdzBoard->cLastPlay = pMyDdzBoard->cDir;   //�¼ҵ��ϼҵ����ҵķ�λ
	pDownDdzBoard->cLandlord = pRealDdz->cLandlord;

	//��ʼ��ģ���ƾ���Ϣ
	pMyDdzBoard->iLastPassCount = pRealDdz->iLastPassCount;  //��ǰ��������PASS��
	pMyDdzBoard->iLastTypeCount = pRealDdz->iLastTypeCount;	 //��ǰ������������
	pMyDdzBoard->iLastMainPoint = pRealDdz->iLastMainPoint;	//��ǰ�������Ƶ���
	pMyDdzBoard->iUpPlayerCards = pRealDdz->iUpPlayerCards;	 //�ϼҵ�������
	pMyDdzBoard->iDownPlayerCards = pRealDdz->iDownPlayerCards;	//�¼ҵ�������

	
	//��¼AI���һ����Ч������Ϣ
	memcpy(pMyDdzBoard->iOnHand, pRealDdz->iOnHand, sizeof(pRealDdz->iOnHand));
	//�����Ƹ��ƽ�pMyDdzBoard

	int i;
	for (i = 0; pRealDdz->iOnTable[i][0] != -2; i++);   
														
	if (pRealDdz->iLastPassCount == 1)
	{	
		//��¼���һ��Pass�������飬��-1��β��Pass��Ϊ-1  
		pMyDdzBoard->iLastOnTable = pRealDdz->iOnTable[i - 2];
	}													

	else
	{
		//�����һ�γ���
		if (i==0)
			pMyDdzBoard->iLastOnTable = pRealDdz->iOnTable[0];

		else
			pMyDdzBoard->iLastOnTable = pRealDdz->iOnTable[i - 1];
	}


	//��ȡ��������
	for (i = 0; pMyDdzBoard->iLastOnTable[i] > 0; i++)
	{
		iLastValidCards[i] = pMyDdzBoard->iLastOnTable[i];
	}

	//���һ��Pass�������飬��-1��β��Pass��Ϊ-1
	iLastValidCards[i] = -1;


	//����AI������
	int myCardsCount = pCinfo->CountCards(pRealDdz->iOnHand);


	//��ʼ���ϼ�  
	pUpDdzBoard->iLastPassCount = pRealDdz->iLastPassCount;  //��ǰ��������PASS��
	pUpDdzBoard->iLastTypeCount = pRealDdz->iLastTypeCount;	 //��ǰ������������
	pUpDdzBoard->iUpPlayerCards = pRealDdz->iDownPlayerCards; //�ϼ�������
	pUpDdzBoard->iLastMainPoint = pRealDdz->iLastMainPoint;		//��ǰ�������Ƶ���
	pUpDdzBoard->iDownPlayerCards = myCardsCount;				//�¼�������
	pUpDdzBoard->iLastOnTable = iLastValidCards;				//���һ�η�Pass��������

	//��ʼ���¼�
	pDownDdzBoard->iLastPassCount = pRealDdz->iLastPassCount;
	pDownDdzBoard->iLastTypeCount = pRealDdz->iLastTypeCount;
	pDownDdzBoard->iUpPlayerCards = myCardsCount;
	pDownDdzBoard->iLastMainPoint = pRealDdz->iLastMainPoint;
	pDownDdzBoard->iDownPlayerCards = pRealDdz->iUpPlayerCards;
	pDownDdzBoard->iLastOnTable = iLastValidCards;


}

void printt(int*temp) {

	if (temp[0] == -2)
	{
		cout << "PASS ";
		return;
	}
	int j;
	int a = 0;
	for (int i = 0; temp[i] != -1; i++)
	{
		a++;
	}
	cout << "  ";
	//cout <<"����"<< a<<"���� ";
	for (int pass = a; pass >0; pass--)
	{
		for (int i = 0; i < pass - 1; i++)
		{
			if (temp[i] > temp[i + 1]) {
				j = temp[i];
				temp[i] = temp[i + 1];
				temp[i + 1] = j;
			}
		}
	}

	for (int i = 0; i<a; i++)
	{
		if (temp[i] / 4 == 0)
			cout << 3 << " ";    //3
		else if (temp[i] / 4 == 1)
			cout << 4 << " ";   //4
		else if (temp[i] / 4 == 2)
			cout << 5 << " ";   //5
		else if (temp[i] / 4 == 3)
			cout << 6 << " ";    //6
		else if (temp[i] / 4 == 4)
			cout << 7 << " ";    //7
		else if (temp[i] / 4 == 5)
			cout << 8 << " ";    //8
		else if (temp[i] / 4 == 6)
			cout << 9 << " ";   //9
		else if (temp[i] / 4 == 7)
			cout << 10 << " ";    //10
		else if (temp[i] / 4 == 8)
			cout << " J ";    //J
		else if (temp[i] / 4 == 9)
			cout << " Q ";    //Q
		else if (temp[i] / 4 == 10)
			cout << " K ";   //K
		else if (temp[i] / 4 == 11)
		{
			cout << " A ";    //A
		}
		else if (temp[i] / 4 == 12)
		{
			cout << 2 << " ";   //2
		}
		else if (temp[i] == 52)
		{
			cout << " С�� ";    //С��
		}
		else if (temp[i] == 53)
		{
			cout << " ���� ";   //����
		}
	}
	cout << ",";
}
//��ʼ����,��϶���
void MonteCarloSearchEngine::SearchAGoodMove(Ddz *pDdz)
{
	iTest = 0;
	pRealDdz = pDdz;
	//��ʼ������ֵ

	Init();

	bool model = pRecomCards->GetModel(pRealDdz);

	//���(pRealDdz->iUpPlayerCards != �ϼ�����|| pRealDdz->iDownPlayerCards != �¼�����)
	if (!model)
	{
		GetOtherPlayerCards(pRecomCards->iUpCards, pRecomCards->iDownCards);
	}

	//����AI���ݵ�ǰ�ƾ����ɵ�ǰ�Ƽ�����  iNodesCount �������ɿ��еĳ��Ʒ����Ľڵ������
	iNodesCount = pRecomCards->HelpPla(pMyDdzBoard, 0);

	int iPass[3] = { -2, -1, -1 };

	//��ֹ����������
	if (iNodesCount == 0)
	{
		int iTmp[4] = { -2,-1,-1,-1 };
		if (pDdz->iLastMainPoint <= -1)
		{
			if (pDdz->iOnHand[0] / 4 == pDdz->iOnHand[1] / 4)
			{
				iTmp[0] = pDdz->iOnHand[0];
				iTmp[1] = pDdz->iOnHand[1];
				iTmp[2] = -2;
				memcpy(ibestMove, iTmp, sizeof(ibestMove));
				return;
			}
			else
			{
				iTmp[0] = pDdz->iOnHand[0];
				iTmp[1] = -2;
				memcpy(ibestMove, iTmp, sizeof(ibestMove));
				return;
			}
		}
		else
		{
			memcpy(ibestMove, iPass, sizeof(ibestMove));
			return;
		}
	}

	//ֻ��һ�ֳ��Ʒ�����ʱ��
	if (iNodesCount == 1)
	{
		if (iFShowInConsole)
		{
			if (iTest == 0)
			{
				for (int i = 0; pMyDdzBoard->iPlaArr[i][0] != -1; i++)
				{
					cout << "ֻ��һ�����н� ";
					printt(pMyDdzBoard->iPlaArr[i]);
				}
				iTest++;
			}
		}


		//int iCountsPlaArr = Cinfo->CountCards(pMyDdzBoard->iPlaArr[0]);
		//int iCountsInHand= Cinfo->CountCards(pMyDdzBoard->iOnHand);

		//if (iCountsPlaArr == iCountsInHand || iCountsInHand - iCountsPlaArr <= 2)//����Ӯ���߳�����������ʣ��С�ڵ���2��ֱ�ӳ�
		if (pRecomCards->MinHandNum(pMyDdzBoard->iOnHand) <= 2)//����Ӯ���߳���������ʣ��С�ڵ���2��ֱ�ӳ�
		{
			memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
			return;
		}

		if (pMyDdzBoard->cDir == pMyDdzBoard->cLandlord) //���AI�ǵ�����ֱ�ӳ�
		{
			memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
			return;
		}
		else//AI���ǵ���
		{
			//����
			if (pMyDdzBoard->iLastMainPoint != -1)
			{
				//����ϼ��ǵ���
				if (pUpDdzBoard->cDir == pMyDdzBoard->cLandlord)
				{
					if (pMyDdzBoard->iLastPassCount == 1)//��������ѵ���
					{
						if (300 < pMyDdzBoard->iLastTypeCount&&pMyDdzBoard->iLastTypeCount < 403)//�����ǵ��ƺͶ���
						{
							if (pMyDdzBoard->iPlaArr[0][0] < 44)//���Ӻ͵��Ʋ�������K��
							{
								memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
								return;
							}
							else
							{
								memcpy(ibestMove, iPass, sizeof(ibestMove));
								return;
							}
						}
						else if (500 < pMyDdzBoard->iLastTypeCount&& pMyDdzBoard->iLastTypeCount < 606)//����������һ��һ�Ե�����
						{
							if (Cinfo->AnalyzeMainMaxPoint(pMyDdzBoard->iPlaArr[0]) < 10)//��������������10��
							{
								memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
								return;
							}
							else
							{
								memcpy(ibestMove, iPass, sizeof(ibestMove));
								return;
							}

						}
						else
						{
							memcpy(ibestMove, iPass, sizeof(ibestMove));
							return;
						}
					}
					else//����������
					{
						memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
						return;
					}
				}


				//����¼��ǵ���
				if (pDownDdzBoard->cDir == pMyDdzBoard->cLandlord)
				{
					//�����ѵ���
					if (pMyDdzBoard->iLastPassCount == 0)
					{
						if (300 < pMyDdzBoard->iLastTypeCount&&pMyDdzBoard->iLastTypeCount < 403)//�����ǵ��ƺͶ���
						{

							if (pMyDdzBoard->iDownPlayerCards >= 10)//����������ƴ��ڵ���10��
							{
								if (pMyDdzBoard->iPlaArr[0][0] < 44)//���Ӻ͵��Ʋ�������K��
								{
									memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
									return;
								}
								else {
									memcpy(ibestMove, iPass, sizeof(ibestMove));
									return;
								}
							}
							else
							{
								memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
								return;
							}
						}

						else if (500 < pMyDdzBoard->iLastTypeCount&& pMyDdzBoard->iLastTypeCount < 606)//���������������һ��һ��
						{
							if (Cinfo->AnalyzeMainMaxPoint(pMyDdzBoard->iPlaArr[0]) < 10)//��������������10��
							{
								memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
								return;
							}
							else {
								memcpy(ibestMove, iPass, sizeof(ibestMove));
								return;
							}

						}
						else
						{
							memcpy(ibestMove, iPass, sizeof(ibestMove));
							return;
						}
					}
					else//����������
					{
						memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
						return;
					}
				}
			}
			/*else//�Լ����ƣ�ֱ�ӳ�
			{
				memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
				return;
			}*/
		}

		memcpy(ibestMove, iPass, sizeof(ibestMove));
		return;
	}

	if (iFShowInConsole)
	{
		//Test
		if (iTest == 0) {
			cout << " ���н�Ϊ";
			for (int i = 0; pMyDdzBoard->iPlaArr[i][0] != -1; i++)
			{

				printt(pMyDdzBoard->iPlaArr[i]);
			}
			iTest++;
		}
	}

	if (iNodesCount > 1) {//�ж�����н�


		//���AI���ǵ���
		if (pMyDdzBoard->cDir != pMyDdzBoard->cLandlord)
		{
			//����
			if (pMyDdzBoard->iLastMainPoint != -1)
			{
				//����ϼ��ǵ���
				if (pUpDdzBoard->cDir == pMyDdzBoard->cLandlord)
				{
					if (pMyDdzBoard->iLastPassCount == 0)// ������������ƣ���ô����С��
					{
						memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
						return;
					}
					else if (pMyDdzBoard->iLastPassCount == 1)//��������ѵ���
					{
						if (300 < pMyDdzBoard->iLastTypeCount&&pMyDdzBoard->iLastTypeCount < 403)//�����ǵ��ƺͶ���
						{
							if (pMyDdzBoard->iPlaArr[0][0] >= 44)//���Ӻ͵��Ʋ�������K��
							{
								memcpy(ibestMove, iPass, sizeof(ibestMove));
								return;
							}
							else //   <44
							{
								memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
								return;
							}
						}
						else if (500 < pMyDdzBoard->iLastTypeCount&& pMyDdzBoard->iLastTypeCount < 606)////���������������һ��һ��
						{
							if (Cinfo->AnalyzeMainMaxPoint(pMyDdzBoard->iPlaArr[0]) < 8)//��������������10��
							{
								memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
								return;
							}
							else
							{
								memcpy(ibestMove, iPass, sizeof(ibestMove));
								return;
							}

						}
						else {
							memcpy(ibestMove, iPass, sizeof(ibestMove));
							return;
						}
					}
				}

				//����¼��ǵ���
				if (pDownDdzBoard->cDir == pMyDdzBoard->cLandlord)
				{
					//�����ѵ���
					if (pMyDdzBoard->iLastPassCount == 0)
					{
						if (300 < pMyDdzBoard->iLastTypeCount&&pMyDdzBoard->iLastTypeCount < 403)//�����ǵ��ƺͶ���
						{
							if (pMyDdzBoard->iDownPlayerCards >= 10)//����������ƴ��ڵ���10��
							{
								if (pMyDdzBoard->iPlaArr[0][0] < 44)//���Ӻ͵��Ʋ�������K�ģ�
								{

									memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
									return;
								}
								else
								{
									memcpy(ibestMove, iPass, sizeof(ibestMove));
									return;
								}
							}
							else {

								memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
								return;
							}

						}
						else if (500 < pMyDdzBoard->iLastTypeCount&&pMyDdzBoard->iLastTypeCount < 606)////���������������һ��һ��
						{
							if (Cinfo->AnalyzeMainMaxPoint(pMyDdzBoard->iPlaArr[0]) < 8)//��������������10��
							{
								memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
								return;
							}
							else
							{
								memcpy(ibestMove, iPass, sizeof(ibestMove));
								return;
							}

						}
						else {
							memcpy(ibestMove, iPass, sizeof(ibestMove));
							return;
						}
					}
					else {//����������
						memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
						return;
					}
				}
			}

		}
		if (pMyDdzBoard->cDir == pMyDdzBoard->cLandlord&&pMyDdzBoard->iDownPlayerCards == 17 && pMyDdzBoard->iUpPlayerCards == 17) {
			memcpy(ibestMove, pMyDdzBoard->iPlaArr[0], sizeof(ibestMove));
			return;
		}
	}

		nodesList = new MTCNode[iNodesCount]; //�ڵ�����

											  //��ʼ���ڵ�
		for (int i = 0; pMyDdzBoard->iPlaArr[i][0] != -1; i++)
			//���ֳ��ƿ��н⼯�������ƽ����Ʊ���������-1���,-2��β��//PASS����Ϊ-3��//passΪ-2-1;
		{
			//��ÿһ�ֿ��н����ڵ���
			memcpy(nodesList[i].iCards, pMyDdzBoard->iPlaArr[i], sizeof(pMyDdzBoard->iPlaArr[i]));

			nodesList[i].node = i;
			nodesList[i].iPlay = 0;
			nodesList[i].fUCB1 = 0;
			nodesList[i].fAverage = 0;
			nodesList[i].iWin = 0;
		}

		//ʱ��PASS
		DWORD k = GetTickCount(); //��ȡ���뼶��Ŀ  ///GetTickCount() window �ĺ���

		int iStartTime = k / 1000; // ��ʼʱ������Ϊ��λ

								   //cout << iStartTime << endl;
		int iRunTime = 0;

		//int BestUCb1 = 0;
		int bestNode = -1;

		iTotalPlay = 0;

		unsigned seed;
		seed = (unsigned)time(NULL);

		if (iFShowInConsole)
		{
			cout << endl << "MCTS Running Time(s) ";
		}

		int printRunTime = 0;

		while (iRunTime <= MaxRunTime)
		{
			if (iFShowInConsole)
			{
				//Test
				if (iRunTime != printRunTime)
				{
					cout << iRunTime << "  ";
					printRunTime = iRunTime;
				}
			}


			seed += 1;
			int i = 0;
			//�ж��Ƿ�ÿ���ڵ㶼�����ʹ�
			for (i = 0; i < iNodesCount; i++)
			{
				if (nodesList[i].iPlay == 0)
				{
					//TODEBUG
					bestNode = i;
					break;
				}
			}

			//ȡ����������нڵ㶼�����ʹ���ѡ��UCB1��ߵĽڵ�
			//���ѡ��ڵ�
			if (i == iNodesCount)
			{
				iRunTime = GetTickCount() / 1000 - iStartTime;
				if (iRunTime <= MaxRunTime / 3)

					bestNode = GetRandomNumFromRange(0, iNodesCount, 0); //���ѡȡ�ڵ�

				else {
					//�Ѿ�����ʱ������������ʱ�������֮һʱ���������ѡȡ�ڵ㣬����ѡȡfUCB1ֵ���Ľڵ�
					bestNode = 0;
					for (i = 0; i < iNodesCount; i++)
					{
						if (nodesList[i].fUCB1 >= nodesList[bestNode].fUCB1)
						{
							bestNode = i;
						}
					}
				}
			}

			int winner = RunMonteCarloSimulation(bestNode);

			iTotalPlay++;

			UpdateUCB1(nodesList, bestNode, winner, iTotalPlay);

			iRunTime = GetTickCount() / 1000 - iStartTime;

			if (iRunTime == 12) { break; }
		}

		if (iFShowInConsole)
		{
			cout << endl;
			cout << "ģ�����������" << iRunTime << "��" << endl;
			cout << "��ģ��" << iTotalPlay << "��" << endl;
			cout << "AIWIN:" << AIWin << " ,UPWIN:" << UpWin << " DownWIN:" << DownWin << endl;
		}

		bestNode = 0;
		for (int j = 0; j < iNodesCount; j++)
		{
			if (iFShowInConsole) 
			{
				cout << "node " << j << "average=" << nodesList[j].fAverage <<
					"= " << nodesList[j].iWin << "/" << nodesList[j].iPlay <<
					" UCB1=" << nodesList[j].fUCB1 << endl;
			}


			//�ҳ�ʤ�����Ľڵ�
			if (nodesList[j].fAverage >= nodesList[bestNode].fAverage)
				bestNode = j;
			if (iFShowInConsole)
			{
				cout << " I choose the:" << bestNode << endl;
			}
		}
		memcpy(ibestMove, nodesList[bestNode].iCards, sizeof(ibestMove));

		delete(nodesList);

	
	
	
	
	//ofstream SaveFile("data.txt", ios::app);
	//int i, j;
	//j = 0;
	//SaveFile << "�������ƣ� " ;
	//for (j = 0; pDdz->iOnHand[j] != -1; j++)
	//	SaveFile << pDdz->iOnHand[j] << ',';
	//SaveFile << endl;

	//SaveFile << "���γ��ƽڵ㣺" << endl;
	//for (i = 0; i<iNodesCount; i++)
	//{
	//	for (j = 0; nodesList[i].iCards[j] != -1; j++)
	//		SaveFile << nodesList[i].iCards[j] << ',';
	//	SaveFile << endl;
	//}
	//SaveFile << "���γ��ƣ�" ;
	//for (j = 0; ibestMove[j] != -1; j++)
	//	SaveFile << ibestMove[j] << ',';
	//SaveFile << endl;
	//SaveFile << "ģ�����������" << iRunTime << "��" << endl;
	//SaveFile << "��ģ��" << iTotalPlay << "��" << endl;
	//SaveFile << "AIWIN:" << AIWin << " ,UPWIN:" << UpWin << " DownWIN:" << DownWin << endl;
	//SaveFile << endl;
	//SaveFile.close();
	
	//ibestMove = nodesList[bestNode].iCards;

}




//���AI�ڵ�������ؿ���ģ��
//AIӮ����1�����򷵻�0
int MonteCarloSearchEngine::RunMonteCarloSimulation(int node)
{
	if (iTotalPlay>0)
		//��ʼ��ģ�����
		Init();
	//cout << " node:" << node << "in " << iNodesCount << endl;

	Update(node, 0);

	int iDepth = 1;

	//if (iTotalPlay>=1)
	//GetOtherPlayerCards(pUpDdzBoard->iOnHand, pDownDdzBoard->iOnHand);
	//��ʼ���ϼ��¼������Ϣ
	//pUpDdzBoard->iLastOnTable = nodesList[node].iCards;
	//pDownDdzBoard->iLastOnTable = nodesList[node].iCards;

	int iIsGameOver = -1;

	unsigned seed;
	seed = (unsigned)time(NULL);

	while (iIsGameOver==-1)
	{	
		seed += 10000;//�ı�seed

		//�����ڵ�
		int thisNode;
		int nodesCount = MakeMove(iDepth);

		//ѡ������ڵ����
		if (nodesCount > 1)
		{
			thisNode = GetRandomNumFromRange(0, nodesCount, iDepth);
		}
		else
		{
			thisNode = 0;
		}//���¾�����Ϣ
		Update(thisNode, iDepth);

		iDepth = iDepth + 1; //�������1

		iIsGameOver = IsGameOver();  //AIӮreturn 1  ��return 0,û����return -1;
	}
	return iIsGameOver;
}

// ���㲢���±��ڵ����ģ��ֵ
void MonteCarloSearchEngine::Update(int node, int iDepth)
{


	int iCardsCount;

	fDdz *thisBoard = pMyDdzBoard;

	switch (iDepth % 3)
	{
	case 1:thisBoard = pDownDdzBoard;
		break;
	case 2:thisBoard = pUpDdzBoard;
		break;
	}
	//ofstream SaveFile("SimData.txt", ios::app);
	//int i, j;
	//j = 0;

	/*SaveFile << "��" << iTotalPlay << "��ģ��"  ;
	SaveFile << "��:" << iDepth << " ";
	SaveFile << "����:";
	for (int i = 0; thisBoard->iOnHand[i] != -1; i++)
	{
		SaveFile << thisBoard->iOnHand[i] << ",";
	}
	SaveFile << endl;
	SaveFile << "�ڵ�:";*/
	//������//

	if (iDepth == 0 && iTotalPlay>0)
	{
		memcpy(thisBoard->iPlaArr[node], nodesList[node].iCards, sizeof(nodesList[node].iCards));
	}

	//for (int i = 0; thisBoard->iPlaArr[node][i] != -1; i++)
	//{
	//	SaveFile << thisBoard->iPlaArr[node][i] << ",";
	//}
	//SaveFile << endl;
	//SaveFile.close();

	//�����pass
	
	if (thisBoard->iPlaArr[node][0] <= -1)
	{
		thisBoard->iLastPassCount++;
		iCardsCount = 0;
		if (thisBoard->iLastPassCount >= 2)	//��������PASS
		{
			thisBoard->iLastPassCount = 0;
			thisBoard->iLastTypeCount = 0;
			thisBoard->iLastMainPoint = -1;

			memset(iLastValidCards, -1, sizeof(iLastValidCards));

			thisBoard->iLastOnTable = iLastValidCards;
		}
		//elseʲôҲ���ı�
	}
	else
	{
		//����������
		//for (iCardsCount = 0; thisBoard->iPlaArr[node][iCardsCount] != -1; iCardsCount++);
		//����thisBoard����
		//����iplaArr�������
		//TODO�����Դ���
		auto my_lambda_func = [](int *iOnHand, int elem)
		{
			int i = 0, j = 0;
			for (j = 0; j<21; j++)
			{

				if (iOnHand[j] == elem)
					i++;
				else
				{
					iOnHand[j - i] = iOnHand[j];
				}
			}
		};

		for (int n = 0; thisBoard->iPlaArr[node][n] != -1; n++)
		{
			my_lambda_func(thisBoard->iOnHand, thisBoard->iPlaArr[node][n]);
		}
		thisBoard->iLastPassCount = 0;
		thisBoard->iLastMainPoint = pCinfo->AnalyzeMainPoint(thisBoard->iPlaArr[node]);
		thisBoard->iLastTypeCount = pCinfo->AnalyzeTypeCount(thisBoard->iPlaArr[node]);
		memcpy(iLastValidCards, thisBoard->iPlaArr[node], sizeof(thisBoard->iPlaArr[node]));
		thisBoard->iLastOnTable = iLastValidCards;
	}
	if (thisBoard == pMyDdzBoard)
	{
		pUpDdzBoard->iLastPassCount = thisBoard->iLastPassCount;
		pUpDdzBoard->iLastOnTable = iLastValidCards;
		pUpDdzBoard->iLastTypeCount = thisBoard->iLastTypeCount;
		pUpDdzBoard->iLastMainPoint = thisBoard->iLastMainPoint;
		pDownDdzBoard->iLastPassCount = thisBoard->iLastPassCount;
		pDownDdzBoard->iLastOnTable = iLastValidCards;
		pDownDdzBoard->iLastTypeCount = thisBoard->iLastTypeCount;
		pDownDdzBoard->iLastMainPoint = thisBoard->iLastMainPoint;
	}
	if (thisBoard == pUpDdzBoard)
	{
		pMyDdzBoard->iLastPassCount = thisBoard->iLastPassCount;
		pMyDdzBoard->iLastOnTable = iLastValidCards;
		pMyDdzBoard->iLastTypeCount = thisBoard->iLastTypeCount;
		pMyDdzBoard->iLastMainPoint = thisBoard->iLastMainPoint;
		pDownDdzBoard->iLastPassCount = thisBoard->iLastPassCount;
		pDownDdzBoard->iLastOnTable = iLastValidCards;
		pDownDdzBoard->iLastTypeCount = thisBoard->iLastTypeCount;
		pDownDdzBoard->iLastMainPoint = thisBoard->iLastMainPoint;
	}

	if (thisBoard == pDownDdzBoard)
	{
		pUpDdzBoard->iLastPassCount = thisBoard->iLastPassCount;
		pUpDdzBoard->iLastOnTable = iLastValidCards;
		pUpDdzBoard->iLastTypeCount = thisBoard->iLastTypeCount;
		pUpDdzBoard->iLastMainPoint = thisBoard->iLastMainPoint;
		pMyDdzBoard->iLastPassCount = thisBoard->iLastPassCount;
		pMyDdzBoard->iLastOnTable = iLastValidCards;
		pMyDdzBoard->iLastTypeCount = thisBoard->iLastTypeCount;
		pMyDdzBoard->iLastMainPoint = thisBoard->iLastMainPoint;
	}
	iCardsCount = pCinfo->CountCards(thisBoard->iOnHand);
	if (thisBoard == pMyDdzBoard)
	{
		pUpDdzBoard->iDownPlayerCards = iCardsCount;
		pDownDdzBoard->iUpPlayerCards = iCardsCount;
	}
	if (thisBoard == pUpDdzBoard)
	{
		pDownDdzBoard->iDownPlayerCards = iCardsCount;
		pMyDdzBoard->iUpPlayerCards = iCardsCount;
	}
	if (thisBoard == pDownDdzBoard)
	{
		pUpDdzBoard->iUpPlayerCards = iCardsCount;
		pMyDdzBoard->iDownPlayerCards = iCardsCount;
	}

}




void MonteCarloSearchEngine::UpdateUCB1(MTCNode nodes[], int node, int Winner, int iTotalPlay)
{

	nodes[node].iPlay++;
	if (Winner == 1)
		nodes[node].iWin++;
	if (iTotalPlay == 0)
	{
		nodes[node].fUCB1 = 0;
		return;
	}

	nodes[node].fAverage = (float)nodes[node].iWin / (float)nodes[node].iPlay;

	//�������нڵ��FUCB1
	for (int i = 0; i < iNodesCount; i++)
	{
		if (nodes[i].iPlay == 0)
			break;
		float fUCB1 = 2 * log(iTotalPlay);
		fUCB1 = fUCB1 / nodes[i].iPlay;
		fUCB1 = sqrtf(fUCB1);
		nodes[i].fUCB1 = nodes[i].fAverage + fUCB1;
		///w/n+   ���� (2* log(iTotalPlay))/iplay
	}
}



int MonteCarloSearchEngine::MakeMove(int iDepth)
{
	int iCount = 0;
	fDdz *thisBoard = pMyDdzBoard;
	switch (iDepth % 3)
	{
	case 1:thisBoard = pDownDdzBoard;
		break;
	case 2:thisBoard = pUpDdzBoard;
		break;
	}
	
	//pRecomCards->InitPlaGenerate(thisBoard);

	iCount = pRecomCards->HelpPla(thisBoard,1);

	//iDepthPos[iDepth + 1] = iDepthPos[iDepth] + iCount;
	//AddDepthNodesInList(iDepth);
	return iCount;

}



//UNDONE
void MonteCarloSearchEngine::GetOtherPlayerCards(int* iUpPlayerCards, int* iDownPlayerCards)
{
	//int iOnUpHand[21] = { 1, 2, 3, 9, 11, 12, 14, 16, 19, 20, 24, 25, 32, 41, 44, 52, 53, -1 };
	//memcpy(pUpDdzBoard->iOnHand, iOnUpHand, sizeof(iOnUpHand));
	//int iOnDownHand[21] = { 6, 7, 10, 15, 21, 27, 31, 33, 34, 35, 37, 38, 47, 48, 49, 50, 51, -1 };
	//memcpy(pDownDdzBoard->iOnHand, iOnDownHand, sizeof(iOnDownHand));
	
	for (int i = 0; i < 21; i++)
	{
		iUpPlayerCards[i] = -1;
		iDownPlayerCards[i] = -1;
	}

	int count = pRealDdz->iUpPlayerCards + pRealDdz->iDownPlayerCards;

	std::vector<int> vi(count); // ����һ����СΪN��vector
	//��������������Ʒ���һ������

	for (int i = 0; i<count; ++i)
	{
		vi[i] = pRealDdz->iOnOtherHand[i];
	}

	//�������
	//srand((unsigned)time(NULL));
	//�����������������
	std::random_shuffle(vi.begin(), vi.end());  ////STL�еĺ���random_shuffle()������һ��Ԫ�����н��������������

	int i = 0;

	//���ɼ�����һ����ҵ������
	for (vector<int>::iterator it = vi.begin(); it != vi.begin() + pRealDdz->iUpPlayerCards; it++)
	{

		iUpPlayerCards[i] = *it;
		i++;
	}

	iUpPlayerCards[i] = -1;

	i = 0;
	pCinfo->SortById(iUpPlayerCards);

	//������һ���Ǽ�����ҵ������
	i = 0;
	for (vector<int>::iterator it = vi.begin() + pRealDdz->iUpPlayerCards; it != vi.end(); it++)
	{
		iDownPlayerCards[i] = *it;
		i++;
	}

	iDownPlayerCards[i] = -1;
	pCinfo->SortById(iDownPlayerCards);
}

//�ж��ƾ��Ƿ����,�����ؼ�ֵ
int MonteCarloSearchEngine::IsGameOver()
{
	//�ҷ��Ƴ���
	if (pMyDdzBoard->iOnHand[0] == -1)
	{
		AIWin++;
		return 1;
	}

	else if (pUpDdzBoard->iOnHand[0] == -1)
	{
		UpWin++;
		//����ǵ���������Ӯ��AIũ���䣩
		if (pUpDdzBoard->cDir == pUpDdzBoard->cLandlord)
			return 0;
		//������ǵ�������AI�ǵ�����AI�����䣩
		else if (pMyDdzBoard->cDir == pMyDdzBoard->cLandlord)
			return 0;
		//������ǵ���,��AI���ǵ�����AIũ��Ӯ��
		else if (pMyDdzBoard->cDir != pMyDdzBoard->cLandlord)
			return 1;
	}
	else if (pDownDdzBoard->iOnHand[0] == -1)
	{
		DownWin ++;
		//����ǵ���������Ӯ
		if (pDownDdzBoard->cDir == pDownDdzBoard->cLandlord)
			return 0;
		//������ǵ�������AI�ǵ���
		else if (pMyDdzBoard->cDir == pMyDdzBoard->cLandlord)
			return 0;
		//������ǵ���,��AI���ǵ���
		else if (pMyDdzBoard->cDir != pMyDdzBoard->cLandlord)
			return 1;
	}

	return -1;
}



void MonteCarloSearchEngine::InitBoard()
{
}


// ���ұ�
int MonteCarloSearchEngine::GetRandomNumFromRange(int iStart, int iEnd,int iDepth)
{
	std::vector<int> nodes(iEnd); // ����һ����СΪN��vector
	for (int i = 0; i < iEnd; i++)
	{
		nodes[i] = iStart;
		iStart++;
	}

	std::random_shuffle(nodes.begin(), nodes.end());//������

	int i;
	if (iDepth==0)
	{
		nodeSeed++;
		if (nodeSeed >= iEnd)
			nodeSeed = 0;
			i = nodeSeed;
	}
	else
	{
		 i = nodes[iEnd - 1];
	}

	int thisNode = nodes[i];
	return thisNode;
}
