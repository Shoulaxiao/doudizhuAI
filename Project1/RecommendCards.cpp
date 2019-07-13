//���Ľ� 2018.7.27
#include "RecommendCards.h"

//Test
void printCards(int*temp);

//������������ʼ��
//�Ż��˵��������жϣ������˳�ը�������
//���רΪģ�����ʹ�õĳ��ƺ���
int RecommendCards::InitPlaGenerate(fDdz* pDdz)
{
	int i, j;
	for (i = 0; i<PlaMax; i++)		//����Ƽ���������
		for (j = 0; j<21; j++)
			pDdz->iPlaArr[i][j] = -1;

	iPlaCount = 0;
	isSim = false;

	iHandNum = MinHandNum(pDdz->iOnHand);
	iUpHandNum = MinHandNum(iUpCards);     
	iDownHandNum = MinHandNum(iDownCards);
	iLastMaxPoint = CInfo->AnalyzeMainMaxPoint(pDdz->iLastOnTable);//-1��Ϊ����PASS 
	playerType = GetPlayerType(pDdz);

	return 0;
}


//����AI���ݵ�ǰ�ƾ����ɵ�ǰ�Ƽ�����
//����޶���:���Ľ�,����޶�ʱ��:2018-07-26
int RecommendCards::HelpPla(struct fDdz * pDdz, bool IsSim)
{
	int iPass[3] = { -2, -1, -1 };
	InitPlaGenerate(pDdz);
	isSim = IsSim;             
	//iFShowInConsole=false
	if (iFShowInConsole)               
	{
		if (!IsSim) 
		{
			//Test
			//cout << "split card result" << endl;

			int **result = pSplitCards->startSplitCard(pDdz->iOnHand);
			//cout << " Rocket ";
			for (int j = 0; j < 8; j++)
			{
				int flag = 0;
				if (result[j][0] != -1)
				{
					if (j == 0)cout << " Rocket ";
					if (j == 1)cout << " Bomb ";
					if (j == 2)cout << " Three ";
					if (j == 3)cout << " LinkThree ";
					if (j == 4)cout << " LinkSingle ";
					if (j == 5)cout << " LinkDouble ";
					if (j == 6)cout << " Double ";
					if (j == 7)cout << " Single ";
				}
				
				for (int i = 0; result[j][i] != -1; i++)
				{
					if (result[j][i] != -2)
					{
						cout << result[j][i] / 4 + 3 << " ";
					}
					else
					{
						cout << " , ";
					}
					flag++;
				}
				if(flag)
				cout << endl;
			}
			cout << endl;
		}

		if (!IsSim)
		{
			cout << endl << "��ǰAIʣ������ " << iHandNum << endl;
		}

		//Test
		if (!isSim&&judgeFriend(pDdz)) {
			cout << "It is my friend's cards" << endl;
		}
	}


	//���Ʊ�����������
	CInfo->SortById(pDdz->iOnHand);

	//����AI������
	int iCardcount = CInfo->CountCards(pDdz->iOnHand);
	
	//�ҵ�������ҵ�����Ƶ�
	int iMaxInother = 0;
	for (int i = 0; pRealDdz->iOnOtherHand[i] != -1; i++)
	{
		if (pRealDdz->iOnOtherHand[i]/4 > iMaxInother)
		{
			iMaxInother = pRealDdz->iOnOtherHand[i]/4;
		}
	}

	int iMayOnOtherBomb = 0;  //������ҿ��ܵ�ը���ͻ����
	int iMayOnOtherTwo = 0;   //�������2��������δ����
	int iRepeat[15] = { 0 };  //�����ظ���
	iRepeat[14] = -1;

	//�����������������Ƹ����Ƶĵ���������
	for (int i = 0; pRealDdz->iOnOtherHand[i] != -1; i++)
	{
		iRepeat[pRealDdz->iOnOtherHand[i] / 4]++;
	}

	//������ҿ��ܴ���ը����
	for (int i = 0; i < 14; i++)
	{
		if (iRepeat[i] == 4)
			iMayOnOtherBomb++;
	}
	//������ҿ��ܴ��ڻ��
	if (iRepeat[13] == 2)
	{
		iMayOnOtherBomb++;
	}

	int iTmp[20];                    //��һ�֣�Ҫ������
	memset(iTmp, -1, sizeof(iTmp));  //��ʼ��
									 //֮���������洢ʣ�µ����Ƶģ�-2��β��֮��Ϊ-1
									 //��һ�׷ֺ����͵���
	int iKing[3] = { -1,-1,-1 };      //��
	int iKingIndex = 0;
	int iTwo[5] = { -1,-1,-1,-1,-1 };      //2
	int iTwoIndex = 0;

	//�����ơ������͡�2����������������
	for (int i = 0; pDdz->iOnHand[i] != -1; i++)
	{
		if (pDdz->iOnHand[i] / 4 == 13)
		{
			iKing[iKingIndex++] = pDdz->iOnHand[i];
		}
		else if (pDdz->iOnHand[i] / 4 == 12)
		{
			iTwo[iTwoIndex++] = pDdz->iOnHand[i];
		}
	}

	//�Ƿ�ǰ������pass              //����pass������true,���򷵻�false
	if (IfPassTwice(pDdz))
	{
		if (iHandNum == 1)          //�����������������������𣬱���ʣ��һ�� ������һ�������Ƕ�
		{
			int i;
			for (i = 0; pDdz->iOnHand[i]!=-1; i++)
			{
				iTmp[i] = pDdz->iOnHand[i] != -1;   //������-1����1������-1����0
			}
			iTmp[i++] = -2;
			iTmp[i++] = -1;
			if(GetLegalCards(pDdz, iTmp))			//���뵥��������Ϣ���ж��Ƿ�Ϸ�������Ϸ�������iplaarr
				return iPlaCount;
		}

		
		//ֻʣ��һ������ʱ,ֱ�ӳ�
		if (iCardcount == 1)
		{
			iTmp[0] = pDdz->iOnHand[0];
			iTmp[1] = -2;
			iTmp[2] = -1;
			AddInPlaArr(pDdz, iTmp);	
			return iPlaCount;             
		}
		//ֻʣ����������ʱ
		else if (iCardcount == 2)
		{
			if (pDdz->iOnHand[0]/4 == pDdz->iOnHand[1]/4)//����Ƕ��ӣ�������ը��
			{
				iTmp[0] = pDdz->iOnHand[0];
				iTmp[1] = pDdz->iOnHand[1];
				iTmp[2] = -2;
				iTmp[3] = -1;
				AddInPlaArr(pDdz, iTmp);
				return iPlaCount;
			}
			if (pDdz->iOnHand[1] / 4 >= iMaxInother &&iMayOnOtherBomb == 0)//���������ը�������Լ����Ƶ���ڵ����������
			{
				iTmp[0] = pDdz->iOnHand[1];
				iTmp[1] = -2;
				iTmp[2] = -1;
				AddInPlaArr(pDdz, iTmp);
				return iPlaCount;
			}

			if (playerType == 2 && pDdz->iDownPlayerCards == 1)//����¼ҵ��������������
			{
				iTmp[0] = pDdz->iOnHand[1];
				iTmp[1] = -2;
				iTmp[2] = -1;
				AddInPlaArr(pDdz, iTmp);
				return iPlaCount;
			}

			//�����С��
			iTmp[0] = pDdz->iOnHand[0];
			iTmp[1] = -2;
			iTmp[2] = -1;
			AddInPlaArr(pDdz, iTmp);
			return iPlaCount;

		}
		//ֻʣ��������
		else if (iCardcount == 3)
		{
			if (pDdz->iOnHand[0] == pDdz->iOnHand[1] == pDdz->iOnHand[2])//��һ������AAA�ͣ�ֱ�ӳ�
			{
				iTmp[0] = pDdz->iOnHand[0];
				iTmp[1] = pDdz->iOnHand[1];
				iTmp[2] = pDdz->iOnHand[2];
				iTmp[3] = -2;
				iTmp[4] = -1;
				AddInPlaArr(pDdz, iTmp);
				return iPlaCount;
			}
			if (iKingIndex == 2)//����ը���ȳ���ը
			{
				iTmp[0] = iKing[0];
				iTmp[1] = iKing[1];
				iTmp[2] = -2;
				iTmp[3] = -1;
				AddInPlaArr(pDdz, iTmp);
				return iPlaCount;
			}
			if (pDdz->iOnHand[0] == pDdz->iOnHand[1] && iMayOnOtherBomb == 0)//��һ������AAB�ͣ���������û��ը��
			{
				if (pDdz->iOnHand[0]/4 >= iMaxInother)
				{
					iTmp[0] = pDdz->iOnHand[0];
					iTmp[1] = pDdz->iOnHand[1];
					iTmp[2] = -2;
					iTmp[3] = -1;
					AddInPlaArr(pDdz, iTmp);
					return iPlaCount;
				}
				if (pDdz->iOnHand[2]/4 >= iMaxInother)
				{
					iTmp[0] = pDdz->iOnHand[2];
					iTmp[1] = -2;
					iTmp[2] = -1;
					AddInPlaArr(pDdz, iTmp);
					return iPlaCount;
				}
			}
			if (pDdz->iOnHand[2] == pDdz->iOnHand[1] && iMayOnOtherBomb == 0)//��һ������ABB�ͣ���������û��ը��
			{
				if (pDdz->iOnHand[1]/4 >= iMaxInother)//���Ӵ�
				{
					iTmp[0] = pDdz->iOnHand[0];
					iTmp[1] = pDdz->iOnHand[1];
					iTmp[2] = -2;
					iTmp[3] = -1;
					AddInPlaArr(pDdz, iTmp);
					return iPlaCount;
				}
				if (pDdz->iOnHand[0]/4 >= iMaxInother)//���ƴ�
				{
					iTmp[0] = pDdz->iOnHand[0];
					iTmp[1] = -2;
					iTmp[2] = -1;
					AddInPlaArr(pDdz, iTmp);
					return iPlaCount;
				}
			}
			//ABC�͵��ƣ�����Ϊ3��Ӧ�ò��������￼�ǵ�
		}
		//����������2ʱ
		if (iHandNum == 2)
		{
			//����л��
			int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 0);//�������ը
			if (iCards[0] !=-1)
			{
				if (GetLegalCards(pDdz, iCards))
					return iPlaCount;
			}

			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 6);//����� 0 1 -2 3 4 -2 5 6 
			if (iCards[0] != -1 && iCards[3] == -1)//ֻ��һ������
			{
				if (iCards[0]/4 >= iMaxInother && iMayOnOtherBomb==0)//AI�Ķ������
				{
					if (GetLegalCards(pDdz, iCards))
						return iPlaCount;
				}
			} 

			iCards= pSplitCards->startSplitCard(pDdz->iOnHand, 2);//������
			int iThreeCount = 0;         //��������
			int iSingleCount = 0;        //��������

			for (int i = 0; iCards[i]!=-1; i++)
			{
				if (iCards[4] == -1)//0 1 2 -2 4 5 6 -2 8   //ֻ��һ������
				{
					if (iCards[0]/4 >= iMaxInother && iMayOnOtherBomb == 0)//AI���������  
					{
						iTmp[0] = iCards[0];
						iTmp[1] = iCards[1];
						iTmp[2] = iCards[2];

						//�ҵ����ŵ��ƻ��߶��ӡ�����
						iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 7);//���� 0 -2 2 -2 4
						if (iCards[0] != -1)
						{
							iTmp[3] = iCards[0];
							iTmp[4] = -2;
							iTmp[5] = -1;
							if (GetLegalCards(pDdz, iTmp))				   //���뵥��������Ϣ���ж��Ƿ�Ϸ�������Ϸ�������iplaarr
								return iPlaCount;
						}
						iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 6);//����� 0 1 -2 3 4 -2 6 7
						if (iCards[0] != -1&& iCards[3] ==-1 )
						{
							iTmp[3] = iCards[0];
							iTmp[4] = iCards[1];
							iTmp[5] = -2;
							iTmp[6] = -1;
							if (GetLegalCards(pDdz, iTmp))
								return iPlaCount;
						}

						if (GetLegalCards(pDdz, iTmp))		
							return iPlaCount;
					}
				}
				else if((iCards[4] != -1))	//��ֹһ������
				{  
					//0 1 2 -2 4 5 6 -2 7 8 9 -2
					if (iCards[7] == -1)//��������˳
					{
						iTmp[0] = iCards[0];
						iTmp[1] = iCards[1];
						iTmp[2] = iCards[2];
						iTmp[3] = iCards[4];
						iTmp[4] = iCards[5];
						iTmp[5] = iCards[6];

						//�ҵ������ŵ��ƻ��߶��ӡ�����
						iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 7);//����

						if (iCards[2] != -1)//0 -2 2 -2 3 -2 -1
						{
							iTmp[6] = iCards[0];
							iTmp[7] = iCards[2];
							iTmp[8] = -2;
							iTmp[9] = -1;
							if (GetLegalCards(pDdz, iTmp))     
								return iPlaCount;
						}

						iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 6);//�����
						if (iCards[3] != -1)//0 1 -2 3 4 -2 4 5 -2
						{
							iTmp[6] = iCards[0];
							iTmp[7] = iCards[1];
							iTmp[8] = iCards[3];
							iTmp[9] = iCards[4];
							iTmp[10] = -2;
							iTmp[11] = -1;
							if (GetLegalCards(pDdz, iTmp))
								return iPlaCount;
						}

						if (GetLegalCards(pDdz, iTmp))
							return iPlaCount;
					}
				}
			
				if (iCards[i] == -2)iThreeCount++;
			}

			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 7);//����  0 -2 2 -2 4
			for (int i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)iSingleCount++;
			}
			
			if (iThreeCount == 0)//û���������������ǵ�����һ����
			{
				if (iCards[0] != -1 && iCards[2] == -1 ) 
				{
					if (iCards[0]/4 >= iMaxInother && iMayOnOtherBomb == 0)//AI�ĵ������  
					{
						if (GetLegalCards(pDdz, iCards))
							return iPlaCount;
					}
				}
			}

			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 4);//��˳
			for (int i = 0; iCards[i]!=-1; i++)
			{
				if (iCards[i]==-2)
				{
					if (iCards[i - 1] / 4 >= iMaxInother)
					{
						if (GetLegalCards(pDdz, iCards))
							return iPlaCount;
					}
				}
			}
		}
		
		//�����¼ұ���������С����,��֮ǰû���������ѣ���ֹ���Ѵ������ǵ���С���ƣ�
		if (playerType == 3 && pDdz->iDownPlayerCards == 1&& !pDdz->haveHelpFriend)
		{
			CInfo->SortById(pDdz->iOnHand);
			iTmp[0] = pDdz->iOnHand[0];				//�ҳ���С����
			iTmp[1] = -2;
			iTmp[2] = -1;
			AddInPlaArr(pDdz, iTmp);
			pDdz->haveHelpFriend = true;		//Ҫ��֮ǰ���˶��ѣ��������϶������ƣ�˵�����Ѵ������ǵ���С����
			return iPlaCount;
		}
		else 
		{
			//����Ҫ��������
			Help0Pass(pDdz);
		}
	}
	else//����
	{
		if (iHandNum == 2)//ʣ�����������ʱ
		{
			//����л��
			int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 0);
			if (iCards[0] != 1)
			{
				if (GetLegalCards(pDdz, iCards))
					return iPlaCount;
			}
		}
		else if (iHandNum == 3)
		{
			//����л��
			int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 0);
			if (iCards[0] != 1)
			{
				int iTwo=0;
				for (int i = 0; pDdz->iOnHand[i]!=-1;i++)
				{
					if(pDdz->iOnHand[i]/4==12)
						iTwo++;
				}
				if (iTwo != 0)   //��������С�2��������������
				{
					if (GetLegalCards(pDdz, iCards))
						return iPlaCount;
				}
			}
		}
		int iTypeCount = pDdz->iLastTypeCount;
		/*if (301 == iTypeCount)//����
		{
			if (IsSim||(playerType==2&&pDdz->iDownPlayerCards<=3)|| (playerType == 3 && pDdz->iUpPlayerCards <= 3)|| (playerType == 1 && (pDdz->iUpPlayerCards <= 3|| pDdz->iDownPlayerCards <= 3)))
				Help3SingleSim(pDdz);
			else
				Help3Single(pDdz);
		}
		else if (402 == iTypeCount)
		{
			if (IsSim || (playerType == 2 && pDdz->iDownPlayerCards <=3) || (playerType == 3 && pDdz->iUpPlayerCards <=3)|| (playerType == 1 && (pDdz->iUpPlayerCards <= 3 || pDdz->iDownPlayerCards <= 3)))
				Help4DoubleSim(pDdz);
			else
				Help4Double(pDdz);
		}
		else if (503 == iTypeCount)
			Help5Three(pDdz);
		else if (604 == iTypeCount)
		{
			if (isSim || (playerType == 2 && pDdz->iDownPlayerCards <= 3) || (playerType == 3 && pDdz->iUpPlayerCards <= 3) || (playerType == 1 && (pDdz->iUpPlayerCards <= 3 || pDdz->iDownPlayerCards <= 3)))
				Help6ThreeOneSim(pDdz);
			else
				Help6ThreeOne(pDdz);
		}
		else if (605 == iTypeCount)
		{
			if (isSim || (playerType == 2 && pDdz->iDownPlayerCards <= 3) || (playerType == 3 && pDdz->iUpPlayerCards <= 3) || (playerType == 1 && (pDdz->iUpPlayerCards <= 3 || pDdz->iDownPlayerCards <= 3)))
				Help6ThreeDoubleSim(pDdz);
			else
				Help6ThreeDouble(pDdz);
		}
		*/

		int iTemp = 0;
		//1**Ϊ�����2**Ϊը����3**Ϊ���ƣ�4**Ϊ���ƣ�5**Ϊ������6**Ϊ����һ����һ��
		//11**Ϊ�Ĵ�һ����һ�ԣ�7**��8**��9**�ֱ�Ϊ��˳��˫˳����˳��10**Ϊ�ɻ���
		//*Ϊ0-9����������������Ϣ��CardsInfo.cpp�ĺ���AnalyzeTypeCount����
		if (301 == iTypeCount)//����
		{
			if (IsSim)
				Help3SingleSim(pDdz);
			else
			{
				iTemp = iPlaCount;
				Help3Single(pDdz);
				if (iTemp == iPlaCount)
				{
					if ((playerType == 2 && pDdz->iDownPlayerCards <= 3) 
						|| (playerType == 3 && pDdz->iUpPlayerCards <= 3) 
						|| (playerType == 1 && (pDdz->iUpPlayerCards <= 3 || pDdz->iDownPlayerCards <= 3)))
					{
						Help3SingleSim(pDdz);
					}
				}
			}
				
		}
		else if (402 == iTypeCount)//����
		{
			if (IsSim)
				Help4DoubleSim(pDdz);
			else
			{
				iTemp = iPlaCount;
				Help4Double(pDdz);
				if (iTemp == iPlaCount)
				{
					if ((playerType == 2 && pDdz->iDownPlayerCards <= 3) || (playerType == 3 && pDdz->iUpPlayerCards <= 3) || (playerType == 1 && (pDdz->iUpPlayerCards <= 3 || pDdz->iDownPlayerCards <= 3)))
					{
						Help4DoubleSim(pDdz);
					}
				}
			}
			/*else
				Help4Double(pDdz);*/
		}
		else if (503 == iTypeCount)
			Help5Three(pDdz);
		else if (604 == iTypeCount)
		{
			if (isSim )
				Help6ThreeOneSim(pDdz);
			else
			{
				iTemp = iPlaCount;
				Help6ThreeOne(pDdz);
				if (iTemp == iPlaCount)
				{
					if ((playerType == 2 && pDdz->iDownPlayerCards <= 3) || (playerType == 3 && pDdz->iUpPlayerCards <= 3) || (playerType == 1 && (pDdz->iUpPlayerCards <= 3 || pDdz->iDownPlayerCards <= 3)))
					{
						Help6ThreeOneSim(pDdz);
					}
				}
			}
			/*else
				Help6ThreeOne(pDdz);*/
		}
		else if (605 == iTypeCount)
		{
			if (isSim)
				Help6ThreeDoubleSim(pDdz);
			else
			{
				iTemp = iPlaCount;
				Help6ThreeDouble(pDdz);
				if (iTemp == iPlaCount)
				{
					if ((playerType == 2 && pDdz->iDownPlayerCards <= 3) || (playerType == 3 && pDdz->iUpPlayerCards <= 3) || (playerType == 1 && (pDdz->iUpPlayerCards <= 3 || pDdz->iDownPlayerCards <= 3)))
					{
						Help6ThreeDoubleSim(pDdz);
					}
				}
			}
			/*else
				Help6ThreeDouble(pDdz);*/
		}
		else if (iTypeCount > 700 && iTypeCount < 800)
			Help7LinkSingle(pDdz);
		else if (iTypeCount > 800 && iTypeCount < 900)
			Help8LinkDouble(pDdz);
		else if (iTypeCount > 900 && iTypeCount < 1000)
			Help9LinkThree(pDdz);
		else if (iTypeCount > 1000 && iTypeCount < 1100)
		{
			if ((iTypeCount - 1000) % 4 == 0)
				Help10LinkThreeSingle(pDdz);
			else
				Help10LinkThreeDouble(pDdz);
		}
		else if (1106 == iTypeCount)
			Help11FourSingle(pDdz);
		else if (1108 == iTypeCount)
			Help11FourDouble(pDdz);


		//������ƿɳ�����ը��
		if (iPlaCount == 0)
		{
			if (iHandNum <= 4)
				Help2Bomb(pDdz);
		}

		//������ƿɳ��������
		if (iPlaCount == 0)
		{
			Help1Rocket(pDdz);
		}
			
		if (iPlaCount == 0)		
		{
			if (pDdz->iPlaArr[iPlaCount][0] != -2)
			{
				AddInPlaArr(pDdz, iPass);
				return iPlaCount;
			}
		}
	}
	return iPlaCount;
}

RecommendCards::RecommendCards() : CInfo(CardsInfo::Instance())
{
	pSplitCards = new SplitCard();
	pMoldel = new PokerModelAnalysis();
}

RecommendCards::~RecommendCards()
{
	delete(pSplitCards);
	delete(pMoldel);
}

RecommendCards* RecommendCards::Instance()
{
	static RecommendCards instance;

	return &instance;
}

//���³��ƣ���iOnHand�в�ѯ���͵�iPlaArr
//����޶���:���Ľ�,����޶�ʱ��:2017-07-23
//playerType 1AIΪ���� 2�¼�Ϊ���� 3�ϼ�Ϊ����
int RecommendCards::Help0Pass(fDdz * pDdz)
{
	////�ж��Ƿ��׳�//   //�״γ���
	int cardsCount = CInfo->CountCards(pDdz->iOnHand);		//����������
	int iTmp;
	//����׳�
	if ((cardsCount == 17 && playerType != 1) || cardsCount == 20)
	{
		if (isSim)       //isSim = 1�޹���Ӧ��
		{
			Help3SingleSim(pDdz);//��ѯ���в�ͬ�����������͵�iOnHand����3 4 5 5 -1 -1|3 -2 4 -2 5 -2 -1
			Help4DoubleSim(pDdz);//�޹��������3 3 4 4 4 -1 -1|3 3 -2 4 4 -2 -1
			Help6ThreeOneSim(pDdz);//�޹�������һ��
			Help6ThreeDoubleSim(pDdz);//�޹���������
			Help5Three(pDdz);//H05-START�Ƽ�����Ӧ������
		}
		else if (!isSim)	//isSim = 0
		{
			if (CInfo->CountCards(pSplitCards->startSplitCard(pDdz->iOnHand, 7)) > 4)//�׳����������ŵ��Ʋų���
			{
				Help3Single(pDdz);
			}

			Help4Double(pDdz);

			iTmp= iPlaCount;
			Help6ThreeOne(pDdz);
			Help6ThreeDouble(pDdz);

			if (iTmp == iPlaCount)//û������һ�������������ų�����
			{
				Help5Three(pDdz);
			}
		}

		Help7LinkSingle(pDdz);
		Help8LinkDouble(pDdz);

		iTmp = iPlaCount;
		Help10LinkThreeSingle(pDdz);
		Help10LinkThreeDouble(pDdz);

		if (iTmp == iPlaCount)//û����˳�������߶�
		{
			 Help9LinkThree(pDdz);
		}
	}
	else//���׳�
	{
		if (isSim)
		{
			Help6ThreeOneSim(pDdz);
			Help6ThreeDoubleSim(pDdz);
			Help5Three(pDdz);
		}
		else if (!isSim)
		{
			iTmp = iPlaCount;
			Help6ThreeOne(pDdz);
			Help6ThreeDouble(pDdz);
			if (iTmp == iPlaCount)
			{
				Help5Three(pDdz);
			}
		}
		
		Help7LinkSingle(pDdz);
		Help8LinkDouble(pDdz);

		iTmp = iPlaCount;
		Help10LinkThreeSingle(pDdz);
		Help10LinkThreeDouble(pDdz);
		if (iTmp == iPlaCount)
		{
			Help9LinkThree(pDdz);
		}


		Help11FourSingle(pDdz);
		Help11FourDouble(pDdz);

		//AI�ǵ�����û�˱���ʱ�ų����ƣ�û�˱�˫���ų�����
		if (playerType == 1)
		{
			if (pDdz->iUpPlayerCards >1&& pDdz->iDownPlayerCards>1)
			{
				if (isSim)
					Help3SingleSim(pDdz);
				else
					Help3Single(pDdz);
			}
			if (pDdz->iUpPlayerCards != 2&& pDdz->iDownPlayerCards!=2)
			{
				if (isSim)
					Help4DoubleSim(pDdz);
				else
					Help4Double(pDdz);
			}
		}
		else//AI���ǵ���     
		{
			if (playerType == 2)//�¼��ǵ���
			{
				if (pDdz->iDownPlayerCards > 1)
				{
					if (isSim)
						Help3SingleSim(pDdz);
					else
						Help3Single(pDdz);
				}
				if (pDdz->iDownPlayerCards != 2)
				{
					if (isSim)
						Help4DoubleSim(pDdz);
					else
						Help4Double(pDdz);
				}
			}
			else if (playerType == 3)//�ϼ��ǵ���
			{
				if (pDdz->iUpPlayerCards > 1)
				{
					if (isSim)
						Help3SingleSim(pDdz);
					else
						Help3Single(pDdz);
				}
				if (pDdz->iUpPlayerCards != 2)
				{
					if (isSim)
						Help4DoubleSim(pDdz);
					else
						Help4Double(pDdz);
				}
			}
		}

		if (iPlaCount == 0 && cardsCount==4)
			Help2Bomb(pDdz);
		if (iPlaCount == 0&&cardsCount==2)
			Help1Rocket(pDdz);
	}
	//�������û�Ƴ���
	if (iPlaCount == 0)
	{
		AddInPlaArr(pDdz, pSplitCards->startSplitCard(pDdz->iOnHand, 6));//����

		Help1Rocket(pDdz);//���

		AddInPlaArr(pDdz, pSplitCards->startSplitCard(pDdz->iOnHand,7));//����

		return iPlaCount;
	}
	if (iPlaCount == 0)//��ֹ�������ƿɳ�
	{
		int iTmp[3] = { -2,-2,-1 };
		iTmp[0] = pDdz->iOnHand[0];
		GetLegalCards(pDdz, iTmp);
	}

	return iPlaCount;
}
//H00-END


//H01-START��iOnHand�в�ѯ��ϻ�����͵�iPlaArr
//����޶���:���Ľܣ�����޶�ʱ��:2017-07-19
//��ӳ��������
int RecommendCards::Help1Rocket(struct fDdz * pDdz)
{
	int iCount = iPlaCount;//���ƽ�
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 0);
	if (iCards[0] > -1)
	{
		int Cards = CInfo->CountCards(pDdz->iOnHand);
		if (Cards == 2 || iHandNum <= 2)//�Լ���Ӯʱ������
			AddInPlaArr(pDdz, iCards);

		//����Լ����ǵ��������������ٵĻ��������Ҳ��Ƕ��ѳ����ƵĻ���ը��
		else if (!judgeFriend(pDdz) && ((playerType == 2 && iDownHandNum <= 3) || (playerType == 3 && iUpHandNum <= 3)))
		{
			AddInPlaArr(pDdz, iCards);
		}
	}
	return iPlaCount-iCount;
}
//H01-END


//�ж��Ƿ��Ƕ��ѳ�����  ����������ϼ��ǵ��� ��iLastPassCount==1���� �¼��ǵ��� ��iLastPassCount==0 ;
//������������Ƕ��ѳ�����
//����޶���:���Ľܣ�����޶�ʱ��:2017-07-19
//���ͨ��
bool RecommendCards::judgeFriend(fDdz* pDdz)
{
	//Ϊ���������ؿ��������ԣ����ﲻ������ģ���º���
	if (!isSim && ((playerType == 3 && pDdz->iLastPassCount == 1) || (playerType == 2 && pDdz->iLastPassCount == 0))) 
		return 1;
	else
		return 0;
}


//H02-START�Ƽ�����Ӧ��ը��
//��ը����
//����޶���:���Ľܣ�����޶�ʱ��:2016-07-26
int RecommendCards::Help2Bomb(struct fDdz * pDdz)
{
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);

	int iCount = iPlaCount;//���ƽ�

	int Cards = CInfo->CountCards(pDdz->iOnHand);

	if (Cards == 4 && iCards[0] > -1)
		if (GetLegalCards(pDdz, iCards))
			return iPlaCount-iCount;   

	if (deleteTypeLeftSingleCount(pDdz->iOnHand, iCards) >= 4)   
		return 0; //����ը������4�ż����ϵĵ��ƣ������������ը����

	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);

	if (judgeFriend(pDdz) && iHandNum > 2)return 0;	//����Ƕ���,�����ǵ��ƴ������֣���ը

	//������������л�����ҵ���Ӯ�ļ��ʺܴ�ʱ������ը��
	int iKingCount = 0;
	for (int i = 0; pDdz->iOnHand[i] != -1; i++)
	{
		if (pDdz->iOnHand[i] == 52 || pDdz->iOnHand[i] == 53)iKingCount++;
	}

	if (iKingCount == 2)
	{
		//����ϼ�Ϊ�������������������С��4
		if (playerType == 3 && pDdz->iUpPlayerCards < 4)
			return 0;
		//����¼�Ϊ�������������������С��4
		if (playerType == 2 && pDdz->iDownPlayerCards <4)
			return 0;
	}

	if (!isSim)
	{
		if (pDdz->cDir == pDdz->cLandlord)//���Լ��ǵ���ʱ
		{
			/*if(Cards>=17|| pDdz->iUpPlayerCards > 10 || pDdz->iDownPlayerCards > 10)
			return 0;	*/
			if (Cards >= 10 || (pDdz->iUpPlayerCards > 10 && pDdz->iDownPlayerCards > 10))
				return 0;
		}
		else if (playerType == 3 && pDdz->iUpPlayerCards > 10)//����������ǵ���ʱ��Ҫ�����������С�ڵ���10�ų�ը��
			return 0;
		else if (playerType == 2 && pDdz->iDownPlayerCards > 10)
			return 0;
	}

	int iSave[21];
	memset(iSave, -1, sizeof(iSave));
	if (GetLegalCards(pDdz, iCards, iSave))
	{
		AddInPlaArr(pDdz, iSave);
		return iPlaCount-iCount;
	}

	//����Ǹ���,�Ҳ��Ƕ��ѵ���,��ը��������Ƿ��л��
	if (!IfPassTwice(pDdz)&&!judgeFriend(pDdz))
	{
		Help1Rocket(pDdz);
	}
	return 0;
}
//H02-END

/*
//H03-START��iOnHand�в�ѯ�������͵�iPlaArr
//����޶���:���Ľ�,����޶�ʱ��:17-07-20
int RecommendCards::Help3Single(struct fDdz * pDdz)
{
	int iCopyOnHand[21];
	int iCount = iPlaCount;//���ƽ�
	int iTmpCards[3];
	int iHandCards = CInfo->CountCards(pDdz->iOnHand);
	
	//���ֻʣһ���ƣ��ҺϷ���ֱ�ӳ�  
	if (pDdz->iOnHand[0] != -1 && pDdz->iOnHand[1] == -1)
	{
		iTmpCards[0] = pDdz->iOnHand[0];
		iTmpCards[1] = -2;
		iTmpCards[2] = -1;
		if (GetLegalCards(pDdz, iTmpCards))
			return iPlaCount - iCount;
	}

	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(iCopyOnHand));

	int*iCards= pSplitCards->startSplitCard(pDdz->iOnHand, 7);

	int iSingleCount = 0;
	int iThreeCount = 0;
	for (int i = 0; iCards[i]!=-1; i++)
	{
		if (iCards[i] == -2)iSingleCount++;
	}

	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
	for (int i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] == -2)iThreeCount++;
	}
	if (iLastMaxPoint <= -1 && iSingleCount <= iThreeCount) //�Լ����������������ڵ�������
		return 0;

	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 7);
	if ((playerType == 2 && pDdz->iDownPlayerCards <=2)|| (playerType == 3 && pDdz->iUpPlayerCards <= 2))
	{//����¼һ����ϼҵ����������߱�˫�������ĵ���
		int i;
		for (i = 0; iCards[i] != -1; i++);
		//0 1  2  3 4 5  6
		//1 -2 4 -2 5 -2 -1  
		int iTmp[3]= { -1,-2,-1 };
		if (i >= 2)
		{
			//if ((iCards[i - 2] == 52 || iCards[i - 2] == 53 )&& )i = i - 2;
			iTmp[0] = iCards[i - 2];
			if (GetLegalCards(pDdz, iTmp))
			{
				return iPlaCount - iCount;
			}
		}
	}
	
	if (playerType == 1)//AI�ǵ��������ƻ��߳��� ������С����
	{
		int iTmp[3] = { -1,-2,-1 };
		if (iCards[0] >= 0)
		{
			iTmp[0] = iCards[0];
			if (GetLegalCards(pDdz, iTmp))
			{
				return iPlaCount - iCount;
			}
		}
	}

	int iTwo[5] = { -1,-1,-1,-1,-1 };//�±�0 ��Ÿ���
	int iKing[3] = { -1,-1,-1 };//�±�0 ��Ÿ���
	int iTwoIndex = 1;
	int iKingIndex = 1;
	for (int i = 0; iCopyOnHand[i] != -1; i++)
	{
		if (iCopyOnHand[i] / 4 == 12)
		{
			iTwo[iTwoIndex++] = iCopyOnHand[i];
		}
		else if (iCopyOnHand[i] == 52 || iCopyOnHand[i] == 53)
		{
			iKing[iKingIndex++] = iCopyOnHand[i];
		}
	}

	iTwo[0] = iTwoIndex - 1;
	iKing[0] = iKingIndex - 1;

	if (!judgeFriend(pDdz) && pDdz->cDir != pDdz->cLandlord)//����������
	{
		if (iLastMaxPoint < 12)//�����������С��2�������ǳ���
		{
			for (int i = 0; iCopyOnHand[i] != -1; i++)
			{
				if (iCopyOnHand[i] == 52)
				{
					DeleteElementInCards(iCopyOnHand, 21, 52);
				}
				else if (iCopyOnHand[i] == 53)
				{
					DeleteElementInCards(iCopyOnHand, 21, 53);
				}
			}
		}
		else if (iLastMaxPoint == 12)//������2��������������࣬�������ô�����
		{
			if ((playerType == 2 && iDownHandNum > 5) || (playerType == 3 && iUpHandNum > 5))
			{
				for (int i = 0; iCopyOnHand[i] != -1; i++)
				{
					if (iCopyOnHand[i] == 53)
					{
						DeleteElementInCards(iCopyOnHand, 21, 53);
					}
				}
			}
		}
	}


	iCards = pSplitCards->startSplitCard(iCopyOnHand, 7);
	bool haveTwo = false;
	bool haveKing = false;
	for (int i = 0; iCards[i] != -1; i++)//�ж������Ƿ��ж�����
	{
		if (iCards[i] != -2)
		{
			if (iCards[i] / 4 == 12)
			{
				haveTwo = true;
				break;
			}
			else if (iCards[i] / 4 == 13)
			{
				haveKing = true;
				break;
			}
		}
	}

	if (iLastMaxPoint > -1)//����
	{
		if (!haveTwo)
		{
			if (iHandNum < 6|| (playerType == 2 && pDdz->iDownPlayerCards<5))
			{//û�ж��� ����С��6 �����¼��ǵ������¼�������С��5  ��ѯ�Ƿ������������ϵĶ������
				if (iTwo[0] != 0)
				{
					int iTmp[2] = { -1,-1};
					iTmp[0] = iTwo[1];    
					AppendCards(iTmp, iCards);
					haveTwo = true;
				}
			}
		}
	}

	if (GetLegalCards(pDdz, iCards)) 
	{
		return iPlaCount - iCount;
	}

	if (iPlaCount == 0)
	{
		if (iLastMaxPoint < 0)//û�е���ʱ���Լ����ƣ���������
			return 0;
		else//����
		{
			int iTmp[3] = { -2,-2,-1 };

			if (judgeFriend(pDdz))return 0;//�����ѵ���

			//����������
			else if (pDdz->cDir != pDdz->cLandlord)
			{
				int iSingle[3] = { -1,-2,-1 };
				//���Ҵ���5�ŵĵ�˳
				iCards = pSplitCards->startSplitCard(iCopyOnHand, 4);
				int iLinkSingleCount = 0;
				int i;
				int maxSingle = 0;

				//û�е���ʱ��ȡ��˳��������5�������һ�ŵ���
				for (i = 0; iCards[i] != -1; i++)
				{
					if (iCards[i] == -2)// 0 1 2 3 4 5 6(-2)   7 8 9 10 11 12  13(-2)
					{
						if (iLinkSingleCount > 5)
						{
							if (iCards[i - 1] > maxSingle)
							{
								maxSingle = iCards[i - 1];
							}
						}
						iLinkSingleCount = 0;
					}
					else
						iLinkSingleCount++;
				}

				if (maxSingle != 0)
				{
					iSingle[0] = maxSingle;  
				}

				if (GetLegalCards(pDdz, iSingle))
				{
					return iPlaCount - iCount;
				}

				//�����˳�ĵ��ƻ��ǲ��У�����û��
				
				//���Ҷ�
				if (haveTwo)return 0;
				else
				{
					if (iTwo[0] != 0)
					{
						iTmp[0] = iTwo[1];
						if (GetLegalCards(pDdz, iTmp))
						{
							return iPlaCount - iCount;
						}
						haveTwo = true;
					}
				}

				if ((playerType == 2 && iDownHandNum < 4) || (playerType == 3 && iUpHandNum < 4))
				{
					if (!haveTwo)
					{
						if (iTwo[0] != 0)
						{
							iTmp[0] = iTwo[1];
							if (GetLegalCards(pDdz, iTmp))
							{
								return iPlaCount - iCount;
							}
							haveTwo = true;
						}
					}
					else if (haveTwo && !haveKing&&iKing[0] != 0)
					{
						if (iKing[1] != -1 )
						{
							iTmp[0] = iKing[1];

							if (GetLegalCards(pDdz, iTmp))
							{
								return iPlaCount - iCount;
							}
							else
								return 0;
							haveKing = true;
						}
						else if (iKing[2] != -1)
						{
							iTmp[0] = iKing[2];
							if (GetLegalCards(pDdz, iTmp))
							{
								return iPlaCount - iCount;
							}
							else
								return 0;
						}
					}
				}
			}

			//AI�ǵ���
			else if (pDdz->cDir == pDdz->cLandlord)
			{
				if (!haveTwo)
				{
					if (iTwo[0] != 0)
					{
						iTmp[0] = iTwo[1];
						if (GetLegalCards(pDdz, iTmp))
						{
							return iPlaCount - iCount;
						}
						haveTwo = true;
					}
				}
				if (!haveKing&&iKing[0] != -1)
				{
					if (iKing[1] != -1)
					{
						iTmp[0] = iKing[1];
						if (GetLegalCards(pDdz, iTmp))
						{
							return iPlaCount - iCount;
						}
						else
							return 0;
					}
					else if (iKing[2] != -1)
					{
						iTmp[0] = iKing[2];
						if (GetLegalCards(pDdz, iTmp))
						{
							return iPlaCount - iCount;
						}
						else
							return 0;
					}
				}
			}
		}
	}
	return 0;
}
//H03-END
*/

//H03-START��iOnHand�в�ѯ�������͵�iPlaArr
//����޶���:���Ľ�,����޶�ʱ��:2018-07-26
//�޸���AI���������¼ұ�����˫��AI����С���Ƶ����⣻�߼��ṹ�ϵ��Ż���
int RecommendCards::Help3Single(struct fDdz * pDdz)
{
	int iCopyOnHand[21];
	int iCount = iPlaCount;//���ƽ�
	int iTmpCards[3];
	int iHandCards = CInfo->CountCards(pDdz->iOnHand);

	//���ֻʣһ���ƣ��ҺϷ���ֱ�ӳ�  
	if (pDdz->iOnHand[0] != -1 && pDdz->iOnHand[1] == -1)
	{
		iTmpCards[0] = pDdz->iOnHand[0];
		iTmpCards[1] = -2;
		iTmpCards[2] = -1;
		if (GetLegalCards(pDdz, iTmpCards))
			return iPlaCount - iCount;
	}

	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(iCopyOnHand));

	int * iCards = pSplitCards->startSplitCard(iCopyOnHand, 7);
	bool haveTwo = false;
	bool haveKing = false;
	for (int i = 0; iCards[i] != -1; i++)//�ж������Ƿ��е���������
	{
		if (iCards[i] != -2)
		{
			if (iCards[i] / 4 == 12)
			{
				haveTwo = true;
				break;
			}
			else if (iCards[i] / 4 == 13)
			{
				haveKing = true;
				break;
			}
		}
	}

	int iTwo[5] = { -1,-1,-1,-1,-1 };//�±�0 ��Ÿ���
	int iKing[3] = { -1,-1,-1 };//�±�0 ��Ÿ���
	int iTwoIndex = 1;
	int iKingIndex = 1;
	for (int i = 0; iCopyOnHand[i] != -1; i++)
	{
		if (iCopyOnHand[i] / 4 == 12)
		{
			iTwo[iTwoIndex++] = iCopyOnHand[i];
		}
		else if (iCopyOnHand[i] == 52 || iCopyOnHand[i] == 53)
		{
			iKing[iKingIndex++] = iCopyOnHand[i];
		}
	}

	iTwo[0] = iTwoIndex - 1;
	iKing[0] = iKingIndex - 1;
		

	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 7);

	//�¼һ����ϼҵ����������߱�˫�� AI�ǵ������¼һ��ϼұ�����˫
	if ((playerType == 2 && pDdz->iDownPlayerCards <= 2)
		|| (playerType == 3 && pDdz->iUpPlayerCards <= 2)
		|| (playerType == 1 && (pDdz->iDownPlayerCards <= 2 || pDdz->iUpPlayerCards <= 2)))
	{
		if (!isSim)
		{
			//������2
			if (iKing[0] != 0)
			{
				int iTmp[3] = { -1,-2,-1 };
				iTmp[0] = iKing[1];
				if (GetLegalCards(pDdz, iTmp))
					return iPlaCount - iCount;
			}
			if (iTwo[3] != 0)
			{
				int iTmp[3] = { -1,-2,-1 };
				iTmp[0] = iTwo[1];
				if (GetLegalCards(pDdz, iTmp))
					return iPlaCount - iCount;
			}

			//��������У������ĵ���
			int i;
			for (i = 0; iCards[i] != -1; i++);
			//0 1  2  3 4 5  6
			//1 -2 4 -2 5 -2 -1  
			int iTmp[3] = { -1,-2,-1 };
			if (i >= 2)
			{
				//if ((iCards[i - 2] == 52 || iCards[i - 2] == 53 )&& )i = i - 2;
				iTmp[0] = iCards[i - 2];
				if (GetLegalCards(pDdz, iTmp))
					return iPlaCount - iCount;
			}
		}
		else
		{
			//��������
			CInfo->SortById(iCopyOnHand); //iCopyOnHand[]��С��������

			int i;
			for (i = 0; iCopyOnHand[i] != -1; i++);

			int iTmp[3] = { -1,-2,-1 };
			iTmp[0] = iCopyOnHand[i - 1];

			if (GetLegalCards(pDdz, iTmp))
				return iPlaCount - iCount;
		}
	}

	//û�е���������С��6 �����¼��ǵ������¼�������С��5�������������ϵĶ������
	if (iLastMaxPoint > -1)//����
	{
		if (!haveTwo)
		{
			if (iHandNum < 6 || (playerType == 2 && pDdz->iDownPlayerCards<5))
			{
				if (iTwo[0] != 0)
				{
					int iTmp[2] = { -1,-1 };
					iTmp[0] = iTwo[1];
					AppendCards(iTmp, iCards);
					//haveTwo = true;
				}
			}
		}
	}

	//����������
	if (!judgeFriend(pDdz) && pDdz->cDir != pDdz->cLandlord)
	{
		//�����������С��2�������ǳ���
		if (iLastMaxPoint < 12)
		{
			for (int i = 0; iCopyOnHand[i] != -1; i++)
			{
				if (iCopyOnHand[i] == 52)
				{
					DeleteElementInCards(iCopyOnHand, 21, 52);
				}
				else if (iCopyOnHand[i] == 53)
				{
					DeleteElementInCards(iCopyOnHand, 21, 53);
				}
			}
		}
		//������2��������������࣬�������ô�����
		else if (iLastMaxPoint == 12)
		{
			if ((playerType == 2 && iDownHandNum > 5) || (playerType == 3 && iUpHandNum > 5))
			{
				for (int i = 0; iCopyOnHand[i] != -1; i++)
				{
					if (iCopyOnHand[i] == 53)
					{
						DeleteElementInCards(iCopyOnHand, 21, 53);
					}
				}
			}
		}
		if (GetLegalCards(pDdz, iCards))
		{
			return iPlaCount - iCount;
		}
	}


	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 7);

	int iSingleCount = 0;
	int iThreeCount = 0;
	for (int i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] == -2)iSingleCount++;
	}

	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
	for (int i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] == -2)iThreeCount++;
	}
	if (iLastMaxPoint <= -1 && iSingleCount <= iThreeCount) //�Լ����������������ڵ�������
		return 0;

	/* //û�ã���ɾ
	int iTmp[3] = { -1,-2,-1 };
	if (iCards[0] >= 0)
	{
		for (int i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
			{
				iTmp[0] = iCards[i];
				if (GetLegalCards(pDdz, iTmp))
				{
					return iPlaCount - iCount;
				}
			}
		}
	}*/

	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 7);

	//һ����������ƻ��߳��� ������С����
	if (GetLegalCards(pDdz, iCards))
	{
		return iPlaCount - iCount;
	}


	//����ģ����������������Ķ��ֳ��ƿ��н�����Ϊ0
	if (iPlaCount == 0)
	{
		if (iLastMaxPoint < 0)//û�е���ʱ���Լ����ƣ���������
			return 0;
		else//����
		{
			int iTmp[3] = { -2,-2,-1 };

			if (judgeFriend(pDdz))return 0;//�����ѵ���

										   //����������
			else if (pDdz->cDir != pDdz->cLandlord)
			{
				int iSingle[3] = { -1,-2,-1 };
				//���Ҵ���5�ŵĵ�˳
				iCards = pSplitCards->startSplitCard(iCopyOnHand, 4);
				int iLinkSingleCount = 0;
				int i;
				int maxSingle = 0;

				//û�е���ʱ��ȡ��˳��������5�������һ�ŵ���
				for (i = 0; iCards[i] != -1; i++)
				{
					if (iCards[i] == -2)// 0 1 2 3 4 5 6(-2)   7 8 9 10 11 12  13(-2)
					{
						if (iLinkSingleCount > 5)
						{
							if (iCards[i - 1] > maxSingle)
							{
								maxSingle = iCards[i - 1];
							}
						}
						iLinkSingleCount = 0;
					}
					else
						iLinkSingleCount++;
				}

				if (maxSingle != 0)
				{
					iSingle[0] = maxSingle;
				}

				if (GetLegalCards(pDdz, iSingle))
				{
					return iPlaCount - iCount;
				}

				//�����˳�ĵ��ƻ��ǲ��У�����û��

				//���Ҷ�
				if (haveTwo)return 0;
				else
				{
					if (iTwo[0] != 0)
					{
						iTmp[0] = iTwo[1];
						if (GetLegalCards(pDdz, iTmp))
						{
							return iPlaCount - iCount;
						}
						haveTwo = true;
					}
				}

				if ((playerType == 2 && iDownHandNum < 4) || (playerType == 3 && iUpHandNum < 4))
				{
					if (!haveTwo)
					{
						if (iTwo[0] != 0)
						{
							iTmp[0] = iTwo[1];
							if (GetLegalCards(pDdz, iTmp))
							{
								return iPlaCount - iCount;
							}
							haveTwo = true;
						}
					}
					else if (haveTwo && !haveKing&&iKing[0] != 0)
					{
						if (iKing[1] != -1)
						{
							iTmp[0] = iKing[1];

							if (GetLegalCards(pDdz, iTmp))
							{
								return iPlaCount - iCount;
							}
							else
								return 0;
							haveKing = true;
						}
						else if (iKing[2] != -1)
						{
							iTmp[0] = iKing[2];
							if (GetLegalCards(pDdz, iTmp))
							{
								return iPlaCount - iCount;
							}
							else
								return 0;
						}
					}
				}
			}

			//AI�ǵ���
			else if (pDdz->cDir == pDdz->cLandlord)
			{
				if (!haveTwo)
				{
					if (iTwo[0] != 0)
					{
						iTmp[0] = iTwo[1];
						if (GetLegalCards(pDdz, iTmp))
						{
							return iPlaCount - iCount;
						}
						haveTwo = true;
					}
				}
				if (!haveKing&&iKing[0] != -1)
				{
					if (iKing[1] != -1)
					{
						iTmp[0] = iKing[1];
						if (GetLegalCards(pDdz, iTmp))
						{
							return iPlaCount - iCount;
						}
						else
							return 0;
					}
					else if (iKing[2] != -1)
					{
						iTmp[0] = iKing[2];
						if (GetLegalCards(pDdz, iTmp))
						{
							return iPlaCount - iCount;
						}
						else
							return 0;
					}
				}
			}
		}
	}
	return 0;
}
//H03-END


//��ѯ���в�ͬ�����������͵�iOnHand����          //�޹���  3 4 5 5 -1 -1 // 3 -2 4 -2 5 -2 -1
int RecommendCards::Help3SingleSim(fDdz* pDdz)
{
	int i;
	int iTmpCards[60] = { -1 };

	int count = iPlaCount;
	int point = -1;
	int t = 0;
	for (i = 0; pDdz->iOnHand[i] >= 0; i++)
	{
		if (t > 0)
		{
			if (pDdz->iOnHand[i] / 4 == point)
				continue;
		}
		iTmpCards[t] = pDdz->iOnHand[i];
		point = iTmpCards[t] / 4;
		t++;
		iTmpCards[t] = -2;
		t++;
	}                               
	iTmpCards[t] = -1;
	if (GetLegalCards(pDdz, iTmpCards))
	{
		return iPlaCount - count;
	}
	return 0;
}

///////PASS//////////
//H04-START�Ƽ�����Ӧ�Զ���
//����޶���:���Ľ�����޶�ʱ��:2018-07-21
int RecommendCards::Help4Double(struct fDdz * pDdz)
{
	//����˫˳�����Ķ�����
	//˫˳�������ڼ������Ƿ���˫˳�ڵ����
	int i,  t;
	int iCopyOnHand[21];
	int iCount = iPlaCount;
	int * iCards;

	//�����е��Ƹ��ƽ�iCopyOnHand
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));

	//�Ƿ����ֱ�ӳ�
	i = CInfo->CountCards(pDdz->iOnHand);
	if (i == 2)  
	{
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 6);
		if (iCards[0] >= 0)
		{
			if (GetLegalCards(pDdz, iCards))
				return iPlaCount - iCount;
		}
	}
	
	//�ų�2����
	if (pDdz->iLastMainPoint<11)
	{
		for (i = 48; i <= 53; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21, i);
		}
	}

	//�ж�ը��,���ų�
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	//�ж�����,���ų�
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	} 

	//��˫˳
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 5);
	//�ų���˫˳����Ķ���
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	//�ų��ڵ�˳����Ķ���
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 4);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	//��������
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
	if (GetLegalCards(pDdz, iCards))
	{
		return iPlaCount - iCount;
	}

	//������ƣ��˳�
	if (pDdz->iLastMainPoint < 0)
		return 0;

	//�������û���ӣ����¸�ֵ����ɸѡ
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));

	//����������С��A���ų���
	if (pDdz->iLastMainPoint<11&&iHandNum>2)
	{
		for (i = 48; i <= 53; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21, i);
		}
	}

	//�ж�ը��,���ų�
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	//�ж�����,���ų�
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	//�ж϶��� ������Ӧ�ÿ��Ƕ����Ƿ����һ��˳��
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);

	if (!isSim)
	{
		if (judgeTypeAndAddInArr(pDdz, iCards))
			return iPlaCount - iCount;
	}
	else
	{
		if (GetLegalCards(pDdz,iCards))
			return iPlaCount - iCount;
	}
	
	//�ж��Ƿ��Ƕ��ѳ��ƣ����ǣ��򷵻�
	if (judgeFriend(pDdz))return 0;

	//�����Ȼû���ƣ�����ը��2
	int dui2[4] = { -1, -1, -1, -1 };
	t = 0;
	for (i = 0; pDdz->iOnHand[i] != -1; i++)
	{
		if (pDdz->iOnHand[i] / 4 == 12)
		{
			if (t == 3)
			{
				dui2[0] = -1;
				break;
			}
			dui2[t] = pDdz->iOnHand[i];
			t++;
		}
	}

	i = CInfo->CountCards(pDdz->iOnHand);
	//����ж�2,�����ڵ�һ�ֳ�
	if (dui2[1] != -1)
	{
		if (pDdz->cDir == pDdz->cLandlord&&i == 20)	//pDdz->cDir != pDdz->cLandlord
			return 0;
		//����
		dui2[2] = -2;
		dui2[3] = -1;
		if (GetLegalCards(pDdz, dui2))
		{
			return iPlaCount - iCount;
		}
	}

	return iPlaCount - iCount;
}


//�޹��������        //3 3 4 4 4 -1 -1//3 3 -2 4 4 -2 -1
int RecommendCards::Help4DoubleSim(struct fDdz * pDdz)
{
	int iTmpCards[60] = { -1 };
	int t = 0;
	int i;
	int point = -1;
	int iCount = iPlaCount;

	//Ѱ�Ҷ��ӣ��Ž�iTmpCards������
	for (i = 0; pDdz->iOnHand[i + 1] >= 0; i++)
	{
		if (pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4)
		{
			if (pDdz->iOnHand[i] / 4 != point)//��ֹ���������������ζ���
			{
				iTmpCards[t] = pDdz->iOnHand[i];
				point = pDdz->iOnHand[i] / 4;
				t++;
				iTmpCards[t] = pDdz->iOnHand[i + 1];
				t++;
				iTmpCards[t] = -2;
				t++;

			}
		}
	}
	iTmpCards[t] = -1;
	if (GetLegalCards(pDdz, iTmpCards))
	{
		return iPlaCount - iCount;
	}
	return iPlaCount - iCount;
}


//H05-START�Ƽ�����Ӧ������
//����Լ��г���Ȩ��������˳�����û�У�����˳������������С�ڵ���3ʱ���������� �����Լ�ֻ��������ʱ)
//����޶���:���Ľ�����޶�ʱ��:2018-07-18
int RecommendCards::Help5Three(struct fDdz * pDdz)
{
	int iCopyOnHand[21];
	int i;
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));

	int iCount = iPlaCount;

	int *iCards;

	//�Ƿ����ֱ�ӳ�
	i = CInfo->CountCards(pDdz->iOnHand);
	if (i == 3)
	{
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
		if (iCards[0] >= 0)
		{
			if (GetLegalCards(pDdz, iCards))
				return iPlaCount - iCount;
		}
	}

	//�ж�ը��,���ų�
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand,21, iCards[i]);
		}
	}

	//ȥ���������ϴ�����������������A���������������ڵ���3�����ߵ������ƴ�������ʱ�������ǳ�������  
	if ((playerType == 2 && pDdz->iDownPlayerCards > 3) || (playerType == 3 && pDdz->iUpPlayerCards > 3)||playerType==1)
	{
		if (pDdz->iLastMainPoint < 11 && iHandNum >= 3)
		{
			for (i = 48; i < 52; i++)
			{
				DeleteElementInCards(iCopyOnHand, 21, i);
			}
		}
	}

	//ȥ����˳ 
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 3);
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	//�ж�����,����Ӧ�ÿ��������Ƿ����һ��˳��
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);

	if (iCards[0] <= -1)return 0;

	if (deleteTypeLeftSingleCount(iCopyOnHand, iCards) >= 2)return 0;
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);

	/*if (!isSim)
		if (judgeTypeAndAddInArr(pDdz, iCards))
			return iPlaCount - iCount;
	else
		if (GetLegalCards(pDdz, iCards))
			return iPlaCount - iCount;
	*/
	
	//���û�У����ҷ��г���Ȩ���˳�
	if (pDdz->iLastMainPoint < 0)
	{
		if(iCards[0]/4>10&& CInfo->CountCards(pDdz->iOnHand)>10)//�г���Ȩ ����������ڵ���A��������������10���������
			return 0;
		else
		{
			int iTmp[7] = {-1,-1,-1,-2,-1,-1,-1}; 
			iTmp[0] = iCards[0];
			iTmp[1] = iCards[1];
			iTmp[2] = iCards[2];
			iTmp[3] = -2;
			iTmp[4] = -1;
			iCards= pSplitCards->startSplitCard(iCopyOnHand, 7);
			if (iCards[0] < 48)
			{
				iTmp[3] = iCards[0];
				iTmp[4] = -2;
				iTmp[5] = -1;
			}

			if (judgeTypeAndAddInArr(pDdz, iTmp))   
				return iPlaCount - iCount;

			iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
			if (iTmp[3]==-2&&iCards[0] < 48)
			{
				iTmp[3] = iCards[0];
				iTmp[4] = iCards[1];
				iTmp[5] = -2;
				iTmp[6] = -1;
			}
			
			if (judgeTypeAndAddInArr(pDdz, iTmp))
				return iPlaCount - iCount;
		}
	}
	else//����
	{
		memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));

		//�ж�ը��,���ų�
		int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
		if (iCards[0] > -1)
		{
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] != -2)
					DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
			}
		}

		//ȥ��2  
		if ((pDdz->iLastMainPoint < 11&&iHandNum>=3)||(playerType==2&&pDdz->iDownPlayerCards>3)|| (playerType == 3 && pDdz->iUpPlayerCards > 3))
		{
			for (i = 48; i < 52; i++)
			{
				DeleteElementInCards(iCopyOnHand, 21, i);
			}
		}

		iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
		
		if (!isSim)
		{
			if (judgeTypeAndAddInArr(pDdz, iCards))
				return iPlaCount - iCount;
		}
		else
		{
			if (GetLegalCards(pDdz, iCards))
				return iPlaCount - iCount;
		}
	}
	return 0;
}
//H05-END

//SplitCards����Ҫ�󣺴��ĵ��Ʋ�����2����
//H0601-START�Ƽ�����Ӧ������һ��
//����޶���:���Ľܣ�����޶�ʱ��:2017-07-16
int RecommendCards::Help6ThreeOne(struct fDdz * pDdz)
{
	int iTmpCards[60] = { -1 };
	int iCount = iPlaCount;

	//�ж����޺Ϸ�����,�����浽iSave1��
	int t = 0;
	int i = 0;
	int *iCards;
	t = CInfo->CountCards(pDdz->iOnHand);
	int j;

	//�������ֱ�ӳ�����ֱ�ӳ���ͨ����
	if (t == 4)
	{
		j = 0;
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
		if (iCards[0] > -1)
		{
			for (i = 0; iCards[i] != -1; i++)//����������ֵ��iTempCards
			{
				if (iCards[i] != -2)
				{
					iTmpCards[j] = iCards[i];
					j++;
				}
			}

			for (i = 0; pDdz->iOnHand[i] != -1; i++)//������һ�����ҳ���
			{
				if (pDdz->iOnHand[i] / 4 != iTmpCards[0] / 4)
				{
					iTmpCards[j] = pDdz->iOnHand[i];
					j++;
					break;
				}
			}
			iTmpCards[j] = -2;
			j++;
			iTmpCards[j] = -1;
			if (GetLegalCards(pDdz, iTmpCards))
			{
				return iPlaCount - iCount;
			}
		}
	}

	int iCopyOnHand[21];
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));

	if (iLastMaxPoint < 0 && iHandNum > 4)//�г���Ȩ������������4 ȥ��A����
	{
		for (i = 44; i < 48; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21, i);
		}
	}

	//�ж�ը��,���ų�
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);

	//�ų�ը��
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] != -2)
		{
			DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	//ȥ���������ϴ�����������������A���������������ڵ���3�����ߵ������ƴ�������ʱ�������ǳ�������  
	if ((playerType == 2 && pDdz->iDownPlayerCards > 3) || (playerType == 3 && pDdz->iUpPlayerCards > 3) || (playerType == 1&& pDdz->iDownPlayerCards > 3 && pDdz->iUpPlayerCards > 3))
	{
		if (pDdz->iLastMainPoint < 11 && iHandNum >= 3)
		{
			for (i = 48; i < 52; i++)
			{
				DeleteElementInCards(iCopyOnHand, 21, i);
			}
		}
	}
	
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);

	//���û������
	if (iCards[0] < 0)
	{
		return 0;
	}

	//���������,����������iTmpCards(ͨ��)
	t = 0;
	int santiaoNum = 0;
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[0] == -2)
			break;
		if (iCards[i] == -2)
		{
			iTmpCards[t] = iCards[i - 3];
			t++;
			iTmpCards[t] = iCards[i - 2];
			t++;
			iTmpCards[t] = iCards[i - 1];
			t++;
			iTmpCards[t] = -2;
			t++;
			santiaoNum++;
		}
	}
	iTmpCards[t] = -1;

	if (santiaoNum)
	{
		int santiao[6];
		int danwang = -1;
		int er[4] = { -1, -1, -1, -1 };
		t = 0;
		j = 0;
		//�����ϵĵ����ͷ�ը��2����������(ͨ��)
		int iHandCards = CInfo->CountCards(pDdz->iOnHand);
		for (i = iHandCards; i > iHandCards - 7; i--)
		{
			if (52 <= pDdz->iOnHand[i] && 53 >= pDdz->iOnHand[i])
			{
				j++;
				danwang = pDdz->iOnHand[i];

			}
			if (48 <= pDdz->iOnHand[i] && 51 >= pDdz->iOnHand[i])
			{
				er[t] = pDdz->iOnHand[i];
				t++;
			}
		}

		//���2��ը��,������
		if (er[3] >= 48)
			er[0] = -1;
		if (j > 1)//�������ը��������
			danwang = -1;


		//���ҵ���

		//ȥ����
		for (i = 52; i <=53; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21,i);
		}

		iCards = pSplitCards->startSplitCard(iCopyOnHand, 7);

		int dan[21] = { -1, -1, -1, -1 };

		//����е�
		if (iCards[0] > -1)
		{
			dan[0] = iCards[0];
			dan[1] = -1;
		}

		//���û�����ǳ��ƣ��뿪
		if (iCards[0] <= -1 && pDdz->iLastMainPoint < 0)
		{
			return 0;
		}

		if (dan[0] <=-1)
		{
			//���Ҵ���5�ŵĵ�˳
			iCards = pSplitCards->startSplitCard(iCopyOnHand, 4);
			int iLinkSingleCount = 0;
			int i;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)// 0 1 2 3 4 5(-2) 6 7 8 9 10 11 12(-2)
				{
					if (iLinkSingleCount > 5)
					{
						dan[0] = iCards[i - iLinkSingleCount];
						dan[1] = -1;
						break;
					}
					iLinkSingleCount = 0;
				}
				else
				{
					iLinkSingleCount++;
				}
			}
		}
	

		if (judgeFriend(pDdz))//����Ǹ����ѵ��ƣ�����
		{
			return 0;
		}

		//���û�����Ǹ���,�Ҳ��Ǹ����ѵ��ƣ�������������1�����Ҷ�����ĵ���
		if (dan[0] <= -1&& santiaoNum>1)
		{
			iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);//���Ҷ���
			if (iCards[0] > -1)
			{
				dan[0] = iCards[0];
				dan[1] = -1;
			}
		}

		
		//���û����Ч����
		if (dan[0] <= -1)
		{
			bool condition = AnalyzeCondition(pDdz);//�������Ʒ���false,�ǲ�������true
			if (!condition || iHandNum <= 2)
			{
				/*
				iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);//���Ҷ���
				if (iCards[0] > -1)
				{
					dan[0] = iCards[0];
					dan[1] = -1;
				}*/
				if (dan[0] <= -1)
				{
					if (er[0] != -1)
					{
						dan[0] = er[0];
						dan[1] = -1;
					}
				}
				/*if (danwang != -1)
				{
					dan[t] = danwang;
					t++;
				}*/
			}	
		}


		if (dan[0] == -1)//�����Ȼû�е��ƣ�����
		{
			return 0;
		}

		//����ϲ����ƺ�����
		for (i = 0; iTmpCards[i] != -1; i++)
		{
			if (iTmpCards[i] == -2)
			{
				santiao[2] = iTmpCards[i - 1];
				santiao[1] = iTmpCards[i - 2];
				santiao[0] = iTmpCards[i - 3];
				santiao[3] = -1;
				int santiaopoint = CInfo->AnalyzeMainMaxPoint(santiao);
				for (int j = 0; dan[j] != -1; j++)
				{
					if (dan[j] / 4 != santiaopoint)
					{
						santiao[3] = dan[j];
						santiao[4] = -2;
						santiao[5] = -1;
						if (!isSim)
						{
							if(judgeTypeAndAddInArr(pDdz, santiao))
								return iPlaCount - iCount;
						}
						else
						{
							if (GetLegalCards(pDdz, santiao))
								return iPlaCount - iCount;
						}
					}
				}
			}
		}
	}
	return iPlaCount - iCount;
}
//H0601-END

//�޹�������һ��
int RecommendCards::Help6ThreeOneSim(struct fDdz * pDdz)
{
	int iCount = iPlaCount;
	int iTmpCards[21] = { -1 };

	int i, iTemp, j;
	int point = -1;
	int danNum = 0;

	int santiaoPoint = 0;
	int t = 0;
	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 7);
	//����е�
	for (i = 0; pDdz->iOnHand[i + 2] >= 0; i++)
	{
		if (pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 2] / 4)
		{
			iTemp = i;

			for (j = 0; pDdz->iOnHand[j] >= 0; j++)
			{
				t = 0;
				//�õ��Ʋ����������е������ҽ�ը���ų�
				if (j == iTemp || j == iTemp + 1 || j == iTemp + 2 || pDdz->iOnHand[j] / 4 == pDdz->iOnHand[i] / 4)
					continue;
				//�õ���Ҳ���ܸ���һ��ͬһ�������ĵ��Ƶ�����ͬ
				if (pDdz->iOnHand[j] / 4 == point)
					continue;
				iTmpCards[t] = pDdz->iOnHand[i];
				t++;
				iTmpCards[t] = pDdz->iOnHand[i + 1];
				t++;
				iTmpCards[t] = pDdz->iOnHand[i + 2];
				t++;
				iTmpCards[t] = pDdz->iOnHand[j];
				point = pDdz->iOnHand[j] / 4;
				t++;
				iTmpCards[t] = -1;
				santiaoPoint = CInfo->AnalyzeMainMaxPoint(iTmpCards);   // ��������������
				iTmpCards[t] = -2;
				t++;
				iTmpCards[t] = -1;

				if (iCards[0] != -1&& iCards[0]/4<12&& iCards[0]!= iTmpCards[0])	
				{
					iTmpCards[3] = iCards[0];
				}

				GetLegalCards(pDdz,iTmpCards);
			}

		}
	}

	return iPlaCount - iCount;
}


//H0602-START�Ƽ�����Ӧ������һ��
//����޶���:���Ľܣ�����޶�ʱ��:2017-07-17
int RecommendCards::Help6ThreeDouble(struct fDdz * pDdz)
{
	int iTmpCards[60] = { -1 };
	int iCount = iPlaCount;
	int t = 0;
	int i = 0;
	int *iCards;
	t = CInfo->CountCards(pDdz->iOnHand);
	//�������ֱ�ӳ�����ֱ�ӳ�
	if (t == 5)
	{
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 2);
		if (iCards[0] > -1 && iCards[0] / 4 > iLastMaxPoint)
		{
			iTmpCards[0] = iCards[0];
			iTmpCards[1] = iCards[1];
			iTmpCards[2] = iCards[2];
			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 6);
			if (iCards[0] > -1)
			{
				iTmpCards[3] = iCards[0];
				iTmpCards[4] = iCards[1];
				iTmpCards[5] = -2;
				iTmpCards[6] = -1;

				if (GetLegalCards(pDdz, iTmpCards))
				{
					return iPlaCount - iCount;
				}
			}
		}
	}

	//////////////////////////
	int iCopyOnHand[21];
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));

	//�ж�ը��,���ų�
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 1);

	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
				DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}


	if (iLastMaxPoint < 0 && iHandNum > 4)//�г���Ȩ������������4 ȥ��A����
	{
		for (i = 44; i < 48; i++)
		{
			DeleteElementInCards(iCopyOnHand, 21, i);
		}
	}

	i = CInfo->CountCards(pDdz->iOnHand);

	if (IfPassTwice(pDdz))//����г���Ȩ
	{
		iCards = pSplitCards->startSplitCard(iCopyOnHand, 7);
		if (iCards[0] > -1 && iCards[0] < 28)//�������10���µĵ��ƣ����ȳ�����һ
			return 0;
	}

	/*//ȥ��2  
	if ((playerType == 2 && pDdz->iDownPlayerCards > 3) 
		|| (playerType == 3 && pDdz->iUpPlayerCards > 3)
		|| (playerType == 1 && pDdz->iDownPlayerCards > 3 && pDdz->iUpPlayerCards > 3))
	{
		if (pDdz->iLastMainPoint < 11 && iHandNum >= 3)
		{
			for (i = 48; i < 52; i++)
			{
				DeleteElementInCards(iCopyOnHand, 21, i);
			}
		}
	}*/
	
	//�ж���������
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);

	//���û������
	if (iCards[0] < 0)
		return 0;

	//���������,����������iTmpCards
	t = 0;
	int santiaoNum = 0;
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[0] == -2)
			break;
		if (iCards[i] == -2)
		{
			iTmpCards[t] = iCards[i - 3];
			t++;
			iTmpCards[t] = iCards[i - 2];
			t++;
			iTmpCards[t] = iCards[i - 1];
			t++;
			iTmpCards[t] = -2;
			t++;
			santiaoNum++;
		}
	}
	iTmpCards[t] = -1;

	if (santiaoNum)
	{
		bool condition = AnalyzeCondition(pDdz);
		if (condition||iHandNum>2)//�ų�2����
		{
			for (i = 48; i < 54; i++)
			{
				DeleteElementInCards(iCopyOnHand, 21, i);
			}
		}

		////����˫˳����Ķ���////////
		int santiao[8];
		iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
		int duizi[60];
		memset(duizi, -1, sizeof(duizi));

		for ( i = 0; iCards[i]!=-1; i++)
		{
			duizi[i] = iCards[i];
		}
		duizi[i] = -1;

		if (duizi[0] <= -1&& santiaoNum>1)
		{
			//���û�������ǳ��ƣ��뿪
			if (pDdz->iLastMainPoint < 0)
				return 0;

			//����Ƕ��ѵ��ƣ�����
			if (judgeFriend(pDdz))
				return 0;

			//����з����ƻ��Ƚ϶࣬��������
			if ((playerType == 2 && pDdz->iDownPlayerCards > 10) || (playerType == 3 && pDdz->iUpPlayerCards > 10))
				return 0;

			//���������������Ķ���,��������iCard��
			int  t = 0;
			for (int i = 0; iTmpCards[i] != -1; i++)
			{
				if (iTmpCards[i] == -2)
				{
					duizi[t] = iTmpCards[i - 2];
					++t;
					duizi[t] = iTmpCards[i - 1];
					++t;
					duizi[t] = -2;
					++t;
					break;//ֻ��һ������
				}
			}
			duizi[t] = -1;
		}

		//�����Ȼû����
		if (duizi[0] == -1)
			return 0;

		for (i = 0; iTmpCards[i] != -1; i++)
		{
			int santiaoPoint;
			if (iTmpCards[i] == -2)
			{
				santiao[0] = iTmpCards[i - 3];
				santiao[1] = iTmpCards[i - 2];
				santiao[2] = iTmpCards[i - 1];
				santiao[3] = -1;
				santiaoPoint = CInfo->AnalyzeMainMaxPoint(santiao);
				if (duizi[0] / 4 != santiaoPoint)
				{
					santiao[3] = duizi[0];
					santiao[4] = duizi[1];
					santiao[5] = -2;
					santiao[6] = -1;

					if (!isSim)
					{
						if (judgeTypeAndAddInArr(pDdz, santiao))
						{
							return  iPlaCount - iCount;
						}
					}
					else
					{
						if (GetLegalCards(pDdz, santiao))
						{
							return  iPlaCount - iCount;
						}
					}
				}					
			}
		}
	}
	return iPlaCount - iCount;
}
//H0602-END

//�޹�������һ��
int RecommendCards::Help6ThreeDoubleSim(struct fDdz * pDdz)
{
	int num = 0;
	int iCount = iPlaCount;
	int i, iTemp, j;
	int iTmpCards[500] = { -1 };
	int point = -1;
	int t = 0;
	int santiaoPoint = -1;
	for (i = 0; pDdz->iOnHand[i + 2] >= 0; i++)
	{
		if (t + 5 + 1 > 500)
			break;
		//�ҵ�һ�����ʵ�����
		if (num + 1<kPlaMax
			&& pDdz->iOnHand[i] / 4 > iLastMaxPoint
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 1] / 4
			&& pDdz->iOnHand[i] / 4 == pDdz->iOnHand[i + 2] / 4)
		{
			iTemp = i;
			for (j = 0; pDdz->iOnHand[j + 1] >= 0; j++)
			{
				t = 0;
				//�������������е����ų�
				if (j >= iTemp - 1 && j <= iTemp + 2)
					continue;
				//�ö���Ҳ���ܸ���һ��ͬһ�������Ķ��ӵ�����ͬ
				if (pDdz->iOnHand[j] / 4 == point)
					continue;
				if (pDdz->iOnHand[j]>=52)
					continue;
				if (pDdz->iOnHand[j] / 4 == pDdz->iOnHand[j + 1] / 4)
				{
					iTmpCards[t] = pDdz->iOnHand[i];
					t++;
					iTmpCards[t] = pDdz->iOnHand[i + 1];
					t++;
					iTmpCards[t] = pDdz->iOnHand[i + 2];
					t++;
					iTmpCards[t] = pDdz->iOnHand[j];
					point = pDdz->iOnHand[j] / 4;
					t++;
					iTmpCards[t] = pDdz->iOnHand[j + 1];
					t++;
					iTmpCards[t] = -1;
					santiaoPoint = CInfo->AnalyzeMainMaxPoint(iTmpCards);
					iTmpCards[t] = -2;
					t++;
					iTmpCards[t] = -1;

					if (GetLegalCards(pDdz, iTmpCards))
						num++;
				}
			}
		}
	}
	return iPlaCount - iCount;
}


//TODO
//H07-START�Ƽ�����Ӧ�Ե�˳
//����޶���:���Ľܣ�����޶�ʱ��:2017-07-17
int RecommendCards::Help7LinkSingle(struct fDdz * pDdz)
{
	int i, iLength,   t;
	int iCopyCards[21];
	int iCount = iPlaCount;
	CInfo->SortById(pDdz->iOnHand);//���Ʊ�����������(jwj)
	int shunziCount = 0;
	int *iCards;

	memcpy(iCopyCards, pDdz->iOnHand, sizeof(pDdz->iOnHand));
	t = CInfo->CountCards(pDdz->iOnHand);

	//���ը������˳���м�Ļ����ų�ը��
	if (deleteTypeLeftSingleCount(iCopyCards, pSplitCards->startSplitCard(iCopyCards, 1)) < 4)
	{
		iCards = pSplitCards->startSplitCard(iCopyCards, 1);

		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
			{
				DeleteElementInCards(iCopyCards, 21, iCards[i]);
			}
		}
	}
	

	//ȥ������2
	for (i = 48; i <= 53; i++)
	{
		DeleteElementInCards(iCopyCards, 21, i);	
	}

	int iTmpShunzi[21];
	memset(iTmpShunzi, -1, sizeof(iTmpShunzi));

	int NodesCount = 0;
	
	int iDanShunNum = 0;
	int iDanShunNoBombNum = 0;
	int MaxNumNoBomb = 0;


	//����ǳ���
	if (pDdz->iLastMainPoint < 0)
	{
		iCards = pSplitCards->startSplitCard(iCopyCards, 4);
		if (!isSim)
		{
			if (judgeTypeAndAddInArr(pDdz, iCards))
			{
				return iPlaCount - iCount;
			}
		}
		else
		{
			if (GetLegalCards(pDdz, iCards))
			{
				return iPlaCount - iCount;
			}
		}
	}

	//������ƣ��������ܳ����ƽڵ�  //����û���ж��Ƿ��Ƕ��Ѵ��
	else
	{
		iLength = pDdz->iLastTypeCount - 700;  //��ǰ����˳�ӳ���
		int point = pDdz->iLastMainPoint;
		while(point + iLength - 1 <= 11)   //��˳��󳤶�Ϊ12
		{
			int iTmp = 0;
			iTmp = iTmpShunzi[0];

			iDanShunNum = AnalyzeSingleLink(iLength, point, pDdz->iOnHand, iTmpShunzi);
			point=point+1;
			if (iDanShunNum == iLength && iTmp != iTmpShunzi[0])//��ֹ����һ����˳�ӽ��
			{
				if (!isSim)
				{
					judgeTypeAndAddInArr(pDdz, iTmpShunzi);
				}
				else
				{
					GetLegalCards(pDdz, iTmpShunzi);
				}
			}
		}
	}
	return iPlaCount - iCount;
}
//H07-END

//H08-START�Ƽ�����Ӧ��˫˳
//����޶���:���Ľܣ�����޶�ʱ��:2017-07-16
//�޶�����ʱ��˫˳�����
int RecommendCards::Help8LinkDouble(struct fDdz * pDdz)
{
	int i, j, k, iLength;
	int iCopyOnHand[21];
	int n = 1;
	int iFlag = 0;
	int iCardsFlag = 0;
	int iCard = 0;
	int iTmpShuangshun[21];
	
	int iCount = iPlaCount;

	memset(iTmpShuangshun, -1, sizeof(iTmpShuangshun));
	CInfo->SortById(pDdz->iOnHand);
	
	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));//��������

	//�ų�ը��
	int *iCards = pSplitCards->startSplitCard(iCopyOnHand, 1);
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] != -2)
		{
			DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	//�ų�2��С��
	for (int i = 0;iCopyOnHand[i]!=-1; i++)
	{
		if (iCopyOnHand[i] >= 48)
		{
			iCopyOnHand[i] = -1;
		}
	}

	//�ų���˳����(jwj)
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 3);
	{
		if (iCards[i] != -2)
		{
			DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}

	//�������˫˳�����µĵ��ƺܶࡣ�������
	if (deleteTypeLeftSingleCount(iCopyOnHand, pSplitCards->startSplitCard(iCopyOnHand, 5)) >= 2)
		return 0;

	//������λ����д�ĺ�������ʾ���˰���auto���÷�Ҳû�������� =[&] ��
	//Ӧ����һ����������Ϊint�ĺ���,�о�����͵�һ����ͨ�ĺ������Ҳ����
	auto GetShuangShun = [&](int *iCards, int iLength,int iLastMainPoint)->int
	{
		int shuangshunNum = 0;
		int iCopyCards[21];
		for (i = 0; iCards[i] != -1; i++)
		{
			iCopyCards[i] = iCards[i];
		}
		iCopyCards[i] = -1;

		//���������� ��������� ���ӱ䵥�� ���ű�û��
		for (i = 0; i < CInfo->CountCards(iCopyCards); i++)
		{
			if (iCopyCards[i] / 4 != iCopyCards[i + 1] / 4)
			{
				for (j = i; j < CInfo->CountCards(iCopyCards); j++)
					iCopyCards[j] = iCopyCards[j + 1];
				i--;
			}
		}

		//�����Ͷ��ӱ�ɵ���
		for (i = 0; i < CInfo->CountCards(iCopyCards); i++)
		{
			if (iCopyCards[i] / 4 == iCopyCards[i + 1] / 4)
			{
				for (j = i; j < CInfo->CountCards(iCopyCards); j++)
					iCopyCards[j] = iCopyCards[j + 1];
				i--;
			}
		}

		//���Ͻ�����iCopyCards[21]���������ɶ��ӵĵ������������ΪA

		for (i = 0; iCopyCards[i] >= 0; i++)
		{
			if (iCopyCards[i] / 4 == iCopyCards[i + 1] / 4 - 1)
				n++;    //n�ĳ�ֵΪ1
			else
			{
				n = 1;
				i = iFlag;    //û����
				iFlag++;
			}
			if (n == iLength)
			{
				if (shuangshunNum + 1 < kPlaMax && iCopyCards[i + 2 - iLength] / 4 > iLastMainPoint)
				{
					for (j = i + 2 - iLength; j <= i + 1; j++)
					{
						for (k = 0; k < CInfo->CountCards(pDdz->iOnHand); k++)
						{
							if (iCopyCards[j] / 4 == pDdz->iOnHand[k] / 4 && iCardsFlag < 2)
							{
								iTmpShuangshun[iCard] = pDdz->iOnHand[k];
								iCard++;
								iCardsFlag++;
							}
						}
						iCardsFlag = 0;
					}
					iTmpShuangshun[iCard] = -2;
					iTmpShuangshun[iCard + 1] = -1;
					if (GetLegalCards(pDdz, iTmpShuangshun))
						shuangshunNum++;
					iCard = 0;
				}
				n = 1;
				i = iFlag;
				iFlag++;
			}
		}
		return shuangshunNum;
	};


	if (pDdz->iLastTypeCount > 0)//����
	{
		iLength = (pDdz->iLastTypeCount - 800) / 2;
		GetShuangShun(iCopyOnHand,iLength,pDdz->iLastMainPoint);
	}
	else//������Ǹ���
	{
		for (iLength = 3; iLength <=11; iLength++)
		{
			GetShuangShun(iCopyOnHand, iLength,pDdz->iLastMainPoint);
		}
	}
	return iPlaCount-iCount;
}
//H08-END


//����޶���:���Ľ�����޶�ʱ��:2018-07-21
//����Ӧ�Գ��Ƴ���˳�����
//�޸��˴�����˳�Ĵ���
//�ѿ�
int RecommendCards::Help9LinkThree(struct fDdz * pDdz)
{
	int iTmpSanshun[21];
	memset(iTmpSanshun, -1, sizeof(iTmpSanshun));
	int sanshunNum = 0;
	int i, iTemp, j, k, iLength = pDdz->iLastTypeCount - 900;
	int iTempArr[12][5];

	if (CInfo->CountCards(pDdz->iOnHand) < 6)
		return 0;

	int iTypeCount = 0;
	int iCount = iPlaCount;
	if (pDdz->iLastMainPoint>= 0)//����
	{
		// iTempArr[i][0]����װ����Ϊ i �ж������ƣ�
		// iTempArr[i][1],iTempArr[i][2],iTempArr[i][3],iTempArr[i][4]����װ����Ϊ i ���Ƶı��룬
		memset(iTempArr, -1, 12 * 5 * 4);
		CInfo->SortById(pDdz->iOnHand);    //�Ƚ������������

		/*for (i = 0; pDdz->iOnHand[i] > -1; i++)
		{
			if (pDdz->iOnHand[i] < 48)
			{
				//������Ϊ iCards[i]/4 �����ж����ţ��� j ����ʾ
				j = 1;
				while (-1 != iTempArr[pDdz->iOnHand[i] / 4][j])
					j++;

				//���������iTempArr[iCards[i]/4][j]���棬��Ϊ�ñ���ĵ���ΪiCards[i]/4
				iTempArr[pDdz->iOnHand[i] / 4][j] = pDdz->iOnHand[i];

				//������Ϊ iCards[i]/4 ���Ƶ����� ��Ϊ j;
				iTempArr[pDdz->iOnHand[i] / 4][0] = j;
			}
		}*/

		int new_point;
		j = 0;
		for (i = 0; pDdz->iOnHand[i] > -1; i++)
		{
			if (pDdz->iOnHand[i] < 48)
			{
				if (j == 0)
					new_point = pDdz->iOnHand[i] / 4;

				//������Ϊ iOnHand[i]/4 �����ж����ţ��� j ����ʾ
				if (new_point = pDdz->iOnHand[i] / 4)
				{
					j++;
					//���������iTempArr[iOnHand[i]/4][j]���棬��Ϊ�ñ���ĵ���ΪiOnHand[i]/4
					iTempArr[pDdz->iOnHand[i] / 4][j] = pDdz->iOnHand[i];
				}
				if (new_point != pDdz->iOnHand[i + 1] / 4)
				{
					//������Ϊ iOnHand[i]/4 ���Ƶ����� ��Ϊ j;
					iTempArr[pDdz->iOnHand[i] / 4][0] = j;
					j = 0;
				}
			}
		}

		for (i = pDdz->iLastMainPoint + 1; i < 12; i++)
		{
			iTemp = iLength - 3;
			if (iTempArr[i][0] >= 3)
			{
				j = i;
				while (iTemp)
				{
					if (iTempArr[j][0] >= 3)
					{
						iTemp -= 3;
						j++;
					}
					else break;
				}
				if (sanshunNum + 1 < kPlaMax && iTemp == 0)
				{
					k = 0;
					while (k < iLength)
					{
						iTmpSanshun[k] = iTempArr[j][1];
						k++;
						iTmpSanshun[k] = iTempArr[j][2];
						k++;
						iTmpSanshun[k] = iTempArr[j][3];
						k++;
						j--;
					}
					iTmpSanshun[k] = -2;
					k++;
					iTmpSanshun[k] = -1;

					//GetLegalCards(pDdz,iTmpSanshun);
					if (GetLegalCards(pDdz, iTmpSanshun))//(jwj)
					{
						return iPlaCount - iCount;
					}
				}
			}
		}
	}

	//����ʱ,ֱ�ӳ����ƺ���˳
	if (pDdz->iLastMainPoint < 0)//����
	{
		int iCopyCards[21];
		memcpy(iCopyCards, pDdz->iOnHand, sizeof(pDdz->iOnHand));

		//�ų�ը��
		i = CInfo->CountCards(pDdz->iOnHand);
		int *iCards;
		if (!isSim)
		{
			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] != -2)
				{
					DeleteElementInCards(iCopyCards, 21, iCards[i]);
				}
			}
		}

		iCards = pSplitCards->startSplitCard(iCopyCards, 3);

		//�������˳��ֱ�ӳ�
		if (iCards[0] >= 0)
		{
			j = 0;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)
				{
					iTmpSanshun[j] = -2;
					j++;
					iTmpSanshun[j] = -1;
					GetLegalCards(pDdz, iTmpSanshun);
					j = 0;  
				}
				else{
					iTmpSanshun[j] = iCards[i];
					j++;
				}
			}
		}
	}
	return iPlaCount-iCount;
}
//H09-END


//H1001-START�Ƽ�����Ӧ����˳����
//����޶���:���Ľ�����޶�ʱ��:2018-07-21
int RecommendCards::Help10LinkThreeSingle(struct fDdz * pDdz)
{
	int iCount = iPlaCount;

	int iTmpSanshundan[40];
	int sanshundanNum = 0;
	int i, iTemp, j, k;
	int iTempArr[12][5];
	int nFlag = 0;
	int iProtect = 0;
	int iLength = (pDdz->iLastTypeCount - 1000);
	memset(iTempArr, -1, 12 * 5 * 4);
	CInfo->SortById(pDdz->iOnHand);    //�Ƚ������������
	int iCopyOnHand[21] = { -1 };

	int *iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 7); //����
	int iSingle = 0;
	for (int i = 0; iCards[i]!=-1; i++)
	{
		if (iCards[i] != -2)iSingle++;
	}
	if (iSingle < 2 && playerType == 1)return 0;//�Լ��ǵ����Ҳ�������

	int iCardCount = CInfo->CountCards(pDdz->iOnHand);

	if (iCardCount == 8)
	{
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 3);
		if (iCards[0] != -1)
		{
			int iTmp[10];
			memset(iTmp, -1, sizeof(iTmp));
			int i;
			for (i = 0; pDdz->iOnHand[i] != -1; i++)
			{
				iTmp[i] = pDdz->iOnHand[i];
			}
			iTmp[i++] = -2;
			iTmp[i] = -1;
			if (GetLegalCards(pDdz, iTmp))
			{
				return iPlaCount - iCount;
			}
		}	
	}

	memcpy(iCopyOnHand, pDdz->iOnHand, sizeof(pDdz->iOnHand));//��������

	//�ж�ը��,���ų�
	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);

	//�ų�ը��
	for (i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] != -2)
		{
			DeleteElementInCards(iCopyOnHand, 21, iCards[i]);
		}
	}
	int *iOnHand = iCopyOnHand;

	//�Լ�����
	if (iLength < 0)
	{
		iLength = 8;
		i = CInfo->CountCards(pDdz->iOnHand);

		iCards = pSplitCards->startSplitCard(iOnHand, 3); //�ж���˳
		if (iCards[0] >= 0)
		{
			int tmax = 0;//�����˳��
			j = 0;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)
				{
					if (j > tmax)
					{
						tmax = j;
					}
					j = 0;
				}
				else
					j++;//������
			}
			iLength = tmax + tmax / 3;
		}
	}

	int new_point;
	j = 0;
	for (i = 0; pDdz->iOnHand[i] > -1; i++)
	{
		if (pDdz->iOnHand[i] < 48)//С�ڶ�
		{
			if (j == 0)
				new_point = pDdz->iOnHand[i] / 4;

			//������Ϊ iOnHand[i]/4 �����ж����ţ��� j ����ʾ
			if (new_point = pDdz->iOnHand[i] / 4)
			{
				j++;
				//���������iTempArr[iOnHand[i]/4][j]���棬��Ϊ�ñ���ĵ���ΪiOnHand[i]/4
				iTempArr[pDdz->iOnHand[i] / 4][j] = pDdz->iOnHand[i];
			}
			if (new_point != pDdz->iOnHand[i + 1] / 4)
			{
				//������Ϊ iOnHand[i]/4 ���Ƶ����� ��Ϊ j;
				iTempArr[pDdz->iOnHand[i] / 4][0] = j;
				j = 0;
			}
		}
	}

	CInfo->SortById(iOnHand);    //�Ƚ������������
	for (i = pDdz->iLastMainPoint + 1; i < 12; i++)
	{
		iTemp = iLength;
		if (iTempArr[i][0] >= 3)
		{
			j = i;
			while (iTemp)
			{
				if (iTempArr[j][0] >= 3)
				{
					iTemp -= 4;
					j++;
				}
				else break;
			}
			if (iTemp == 0)
			{
				k = 0;
				j--;
				while (k < iLength / 4 * 3)
				{
					iTmpSanshundan[k] = iTempArr[j][1];
					k++;
					iTmpSanshundan[k] = iTempArr[j][2];
					k++;
					iTmpSanshundan[k] = iTempArr[j][3];
					k++;
					j--;
				}

			
				iCards = pSplitCards->startSplitCard(iOnHand, 7);
				for (int e = 0; iCards[e]!=-1 ; e++)
				{
					if (iCards[e] != -2&&iCards[e]<48)//С�ڶ��ĵ���
					{
						iTmpSanshundan[k] = iCards[e];//6 7  7 8
						k++;
						if (k == iLength)
						{
							iTmpSanshundan[k] = -2;
							k++;
							iTmpSanshundan[k] = -1;
							if (GetLegalCards(pDdz, iTmpSanshundan))
							{
								sanshundanNum++;
							}
							k = 0;
							break;
						}
					}
				}
				//���û�е��ƣ������    
				iCards = pSplitCards->startSplitCard(iOnHand, 6);
				for (int e = 0; iCards[e] != -1; e++)
				{
					if (iCards[e] == -2)
					{
						iTmpSanshundan[k] = iCards[e-1];
						k++;
						iTmpSanshundan[k] = iCards[e-2];
						k++;
						if (k == iLength)
						{
							iTmpSanshundan[k] = -2;
							k++;
							iTmpSanshundan[k] = -1;
							if (GetLegalCards(pDdz, iTmpSanshundan))
							{
								sanshundanNum++;
							}
							k = 0;
							break;
						}
					}
				}
			}
		}
	}
	return iPlaCount-iCount;
}
//H1001-END

//TODO ���һ���Ƶ�ʱ����Դ���
//H1002-START�Ƽ�����Ӧ����˳��˫
//����޶���:���Ľ�����޶�ʱ��:2018-07-21
int RecommendCards::Help10LinkThreeDouble(struct fDdz * pDdz)
{
	int iCount = iPlaCount;
	int iCardsNum = CInfo->CountCards(pDdz->iOnHand);
	int *iCards;

	if (iCardsNum == 10)
	{
		iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 3);//�ж���˳
		if (iCards[0] != -1)
		{
			iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 6);//�ж϶���
			if (iCards[3] != -1)
			{
				int iTmp[12];
				memset(iTmp, -1, sizeof(iTmp));
				int i;
				for (i = 0; pDdz->iOnHand[i]!=-1; i++)
				{
					iTmp[i] = pDdz->iOnHand[i];
				}
				iTmp[i++] = -2;
				iTmp[i++] = -1;
				if (GetLegalCards(pDdz, iTmp))
				{
					return iPlaCount - iCount;
				}
			}
		}
	}


	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 7);
	int iSingle = 0;

	for (int i = 0; iCards[i]!=-1; i++)
	{
		if (iCards[i] != -2&& iSingle<48)iSingle++;//����С�ڶ��ĵ�����
	}
	if (iSingle >= 2)return 0;//����˳������   


	//���˶��Ӻ����ԭ������һ�ż����ϵ��ƣ������������ֹ3445567���������˳��
	if(deleteTypeLeftSingleCount(pDdz->iOnHand, pSplitCards->startSplitCard(pDdz->iOnHand, 6)) >= 1)
		return 0;

	int iTmpSanshunshuang[70];
	int sanshunshuangNum = 0;
	int i, iTemp, j, k, m, iLength = (pDdz->iLastTypeCount - 1000);
	int iTempArr[12][5];
	int nFlag = 0;
	int iCopyOnHand[21] = { -1 };
	int bombPoint[4] = { -1, -1, -1, -1 };//����ը������
	int t = 0;//ը����
	int point = -1;//���

	iCards = pSplitCards->startSplitCard(pDdz->iOnHand, 1);
	j = 0;
	if (iCards[0] > -1)
	{
		for (i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] == -2)
			{
				bombPoint[j] = iCards[i - 1] / 4;
				j++;
			}
		}
	}
	//�ų�ը������
	j = 0;
	for (i = 0; pDdz->iOnHand[i] != -1; i++)
	{
		if (pDdz->iOnHand[i] / 4 != bombPoint[0] 
			&& pDdz->iOnHand[i] / 4 != bombPoint[1] 
			&& pDdz->iOnHand[i] / 4 != bombPoint[2]
			&& pDdz->iOnHand[i] / 4 != bombPoint[3])
		{
			if (pDdz->iOnHand[i] < 52)
			{
				iCopyOnHand[j] = pDdz->iOnHand[i];
				j++;
			}
		}
	}

	iCopyOnHand[j] = -1;

	int *iOnHand = iCopyOnHand;

	auto GetSanShunDouble = [&](int Length)
	{
		for (i = pDdz->iLastMainPoint + 1; i <12; i++)
		{
			iTemp = iLength;
			if (iTempArr[i][0] >= 3)
			{
				j = i;
				while (iTemp)
				{
					if (iTempArr[j][0] >= 3)
					{

						iTemp -= 5;
						j++;
					}
					else break;
				}
				if (iTemp == 0)
				{
					k = 0;
					j--;
					while (k < iLength / 5 * 3)
					{
						iTmpSanshunshuang[k] = iTempArr[j][1];
						k++;
						iTmpSanshunshuang[k] = iTempArr[j][2];
						k++;
						iTmpSanshunshuang[k] = iTempArr[j][3];
						k++;
						j--;
					}

					for (j = 0; pDdz->iOnHand[j] >= 0; j++)
					{
						nFlag = 1;
						for (m = 0; m < k; m++)
						{
							if (iOnHand[j] / 4 == iTmpSanshunshuang[m] / 4)
							{
								nFlag = 0;
								break;
							}
							if (iOnHand[j] / 4 == 12 && iHandNum > 2)//��������4ʱ�������Ǵ��Զ�
							{
								nFlag = 0;
								break;
							}
							if (iOnHand[j] / 4 >= 13)//���ܴ���
							{
								nFlag = 0;
								break;
							}
							if (iOnHand[j] / 4 == bombPoint[0] || iOnHand[j] / 4 == bombPoint[1])//���ܴ�ը��
							{
								nFlag = 0;
								break;
							}
						}
						if (sanshunshuangNum + 1<kPlaMax && nFlag)
						{
							if (iOnHand[j] / 4 == point)     //���(jwj)
								continue;
							if (iOnHand[j] / 4 != iOnHand[j + 1] / 4)
								continue;
							point = iOnHand[j] / 4;
							iTmpSanshunshuang[k] = iOnHand[j];
							k++;
							iTmpSanshunshuang[k] = iOnHand[j + 1];
							k++;
							if (k == iLength)
							{
								iTmpSanshunshuang[k] = -2;
								k++;
								iTmpSanshunshuang[k] = -1;
								GetLegalCards(pDdz, iTmpSanshunshuang);
								//break;
							}
						}
					}
				}
			}
		}
	};
	
	memset(iTempArr, -1, 12 * 5 * 4);

	CInfo->SortById(iOnHand);    //�Ƚ������������

	int new_point;
	j = 0;
	for (i = 0; pDdz->iOnHand[i] > -1; i++)
	{
		if (pDdz->iOnHand[i] < 48)
		{
			if (j == 0)
				new_point = pDdz->iOnHand[i] / 4;

			//������Ϊ iOnHand[i]/4 �����ж����ţ��� j ����ʾ
			if (new_point = pDdz->iOnHand[i] / 4)
			{
				j++;
				//���������iTempArr[iOnHand[i]/4][j]���棬��Ϊ�ñ���ĵ���ΪiOnHand[i]/4
				iTempArr[pDdz->iOnHand[i] / 4][j] = pDdz->iOnHand[i];
			}
			if (new_point != pDdz->iOnHand[i + 1] / 4)
			{
				//������Ϊ iOnHand[i]/4 ���Ƶ����� ��Ϊ j;
				iTempArr[pDdz->iOnHand[i] / 4][0] = j;
				j = 0;
			}
		}
	}

	if (pDdz->iLastMainPoint < 0)
	{
		iLength = 10;
		int i = CInfo->CountCards(pDdz->iOnHand);
		
		iCards = pSplitCards->startSplitCard(iOnHand, 3);
		//�������˳��ֱ�ӳ��ų�ը�������������˳
		int maxSanshun = 6;//�����˳��
		if (iCards[0] >= 0)
		{

			j = 0;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)
				{
					if (j > maxSanshun)
					{
						maxSanshun = j;
					}
					j = 0;
				}
				else
					j++;//������
			}

			iLength = maxSanshun + 2 * (maxSanshun / 3);
		}

		while (iLength >= 10) 
		{
			GetSanShunDouble(iLength);
			maxSanshun -= 3;
			if (maxSanshun>0)
			iLength = maxSanshun + 2  * (maxSanshun / 3);
		}
	}
	else
	{
		GetSanShunDouble(iLength);
	}

	//int lastMinPoint = CInfo->AnalyzeMainPoint(pDdz->iLastOnTable);

	return iPlaCount-iCount;
}
//H1002-END*/1

//H1101-START�Ƽ�����Ӧ���Ĵ�����
//����޶���:���Ľܣ�����޶�ʱ��:2017-07-17
int RecommendCards::Help11FourSingle(struct fDdz * pDdz)
	{
		int Cards = CInfo->CountCards(pDdz->iOnHand);

		if (Cards<6)
			return 0;

		if (pDdz->iLastMainPoint < 0)//�Լ�����
		{
			//����ǳ��ĵ�һ���ƻ�����������4�������������ڵ���17ʱ,�˳�
			if (Cards >= 17 || iHandNum > 4)
				return 0;
		}
		else//����
		{
			if (judgeFriend(pDdz))
				return 0;
		}

		int iCount = iPlaCount;
		int iTmpBomb[38];
		int iFourSingle[38];

		memset(iTmpBomb, -1, sizeof(iTmpBomb));
		memset(iFourSingle, -1, sizeof(iFourSingle));

		CInfo->SortById(pDdz->iOnHand);

		int iCopyCards[21];

		memcpy(iCopyCards, pDdz->iOnHand, sizeof(pDdz->iOnHand));

		int*iCards = pSplitCards->startSplitCard(iCopyCards, 1);

		if (iCards[0] == -1)//û��ը��
			return 0;

		if (deleteTypeLeftSingleCount(iCopyCards, iCards) >= 4)//����ը����������ż����ϵĵ��ƣ�����
			return 0;

		iCards = pSplitCards->startSplitCard(iCopyCards, 1);
		//�洢ը�� ���ң�ɾ��ը��
		int i;
		for (i = 0; iCards[i] != -1; i++)
		{
			iTmpBomb[i] = iCards[i];
			if (iCards[i] != -2)
			{
				DeleteElementInCards(iCopyCards, 21, iCards[i]);
			}
		}
		iTmpBomb[i] = -1;

		if (pDdz->iLastMainPoint < 0)//�Լ�����
		{
			if (iHandNum <= 4)
			{
				int count = 0;
				for (i = 0; iCopyCards[i] != -1; i++)
				{
					if (iCopyCards[i] >= 48)
						count++;
				}
				if (count >= 2)return 0;//���ʣ�µ��������ż������Ƶ��ʱ�������ǳ��Ĵ�����
			}
		}

		iCards = pSplitCards->startSplitCard(iCopyCards, 7);
		if (CInfo->CountCards(iCards)<2)//ֻ��һ�����ƻ���û��
			return 0;

		int index = 0;
		for (int i = 0; iTmpBomb[i] != -1; i++)//ֻȡ��С����������
		{
			if (iTmpBomb[i] == -2)
			{
				iFourSingle[index++] = iTmpBomb[i - 4];
				iFourSingle[index++] = iTmpBomb[i - 3];
				iFourSingle[index++] = iTmpBomb[i - 2];
				iFourSingle[index++] = iTmpBomb[i - 1];
				iFourSingle[index++] = iCards[0];
				iFourSingle[index++] = iCards[2];
				iFourSingle[index++] = -2;
				iFourSingle[index++] = -1;
			}
		}

		if (GetLegalCards(pDdz, iFourSingle))
			return iPlaCount - iCount;

		return iPlaCount - iCount;
	}
//H1101-END

//H1102-START�Ƽ�����Ӧ���Ĵ�����
//����޶���:���Ľܣ�����޶�ʱ��:2017-07-17
int RecommendCards::Help11FourDouble(struct fDdz * pDdz)
{
	int Cards = CInfo->CountCards(pDdz->iOnHand);

	if (Cards < 8)
		return 0;

	if (pDdz->iLastMainPoint < 0)//�Լ�����
	{
		if (Cards >= 17 || iHandNum > 4)//����ǳ��ĵ�һ���ƻ������Ʋ���6��,������������4ʱ,�˳�
			return 0;
	}
	else//����
	{
		if (judgeFriend(pDdz))return 0;
	}
	
	int iCount = iPlaCount;
	int iTmpBomb[38];
	int iTmpDouble[38];
	int iFourDouble[38];

	memset(iTmpBomb, -1, sizeof(iTmpBomb));
	memset(iTmpDouble, -1, sizeof(iTmpDouble));
	memset(iFourDouble, -1, sizeof(iFourDouble));

	CInfo->SortById(pDdz->iOnHand);

	int iCopyCards[21];

	memcpy(iCopyCards, pDdz->iOnHand, sizeof(pDdz->iOnHand));

	int*iCards = pSplitCards->startSplitCard(iCopyCards, 1);

	if (deleteTypeLeftSingleCount(iCopyCards, iCards)>=4)//����ը����������ż����ϵĵ��ƣ�����
		return 0;

	iCards = pSplitCards->startSplitCard(iCopyCards, 1);
	if (iCards[0] == -1)//û������
		return 0;

	//�洢ը�����ң�ɾ��ը��
	int i;
	for (i = 0; iCards[i]!=-1; i++)
	{
		iTmpBomb[i] = iCards[i];
		if (iCards[i] != -2)
		{
			DeleteElementInCards(iCopyCards, 21, iCards[i]);
		}
	}
	iTmpBomb[i] = -1;

	if (pDdz->iLastMainPoint < 0)//�Լ�����
	{
		if (iHandNum <= 4)
		{
			int count = 0;
			for (i = 0; iCopyCards[i] != -1; i++)//������ڵ���2������
			{
				if (iCopyCards[i] >= 48)
					count++;
			}
			if (count >= 2)return 0;//���ʣ�µ��������ż������Ƶ��ʱ�������ǳ��Ĵ�����
		}
	}


	//���������ǲ��˶��Ӻ����Ƿ�����࣬���Ƿ���˳���еĶ���
	iCards = pSplitCards->startSplitCard(iCopyCards, 6);

	if (CInfo->CountCards(iCards) < 4)//ֻ��һ�����ӣ���û�ж���
		return 0;

	if (deleteTypeLeftSingleCount(iCopyCards, iCards) >= 4)//���˶��Ӻ�������ż����ϵĵ��ƣ�����
		return 0;

	iCards = pSplitCards->startSplitCard(iCopyCards, 6);
	//�Ѷ��Ӵ������
	for ( i = 0; iCards[i] != -1; i++)
	{
		iTmpDouble[i] = iCards[i];
	}
	iTmpDouble[i] = -1;

	int index = 0;
	for (int i = 0; iTmpBomb[i]!=-1; i++)//ֻȡ��С����������
	{
		if (iTmpBomb[i] == -2)
		{
			iFourDouble[index++] = iTmpBomb[i - 4];
			iFourDouble[index++] = iTmpBomb[i - 3];
			iFourDouble[index++] = iTmpBomb[i - 2];
			iFourDouble[index++] = iTmpBomb[i - 1];
			iFourDouble[index++] = iTmpDouble[0];
			iFourDouble[index++] = iTmpDouble[1];
			iFourDouble[index++] = iTmpDouble[3];
			iFourDouble[index++] = iTmpDouble[4];
			iFourDouble[index++] = -2;
			iFourDouble[index++] = -1;
		}
	}
	if (GetLegalCards(pDdz, iFourDouble))
		return iPlaCount - iCount;
	return iPlaCount - iCount;
}
//H1102-END


//�������еĵ����� �ڲ��ĳһ����ǰ��Ĳ�ֵ�����ƺ�ʣ�µĵ��� �� ����ǰ�ĵ��ƣ�
//���������type��ͨ��int*type=startSplitCards(Ddz*pDdz,int type) 
//(���������Initdata())�õ��ģ��������type�ᱻ��գ�����
//����޶���:���Ľܣ�����޶�ʱ��:2017-07-17
int RecommendCards::deleteTypeLeftSingleCount(int*iCopyCard, int*type)
{
	int iCopyCards[21] = {-1};
	int iCopyType[38];
	for (int i = 0; i <38; i++)
	{
		iCopyType[i] = type[i];
	}
	for (int i = 0; i <21; i++)
	{
		iCopyCards[i] = iCopyCard[i];
	}
	int iBeforeSingle = 0;//����ǰ�ĵ�����
	int iAfterSingle = 0;//���ƺ�ʣ�µĵ�����

	iBeforeSingle=CInfo->CountCards(pSplitCards->startSplitCard(iCopyCards, 7));

	for (int i = 0; iCopyType[i] != -1; i++)
	{
		if (iCopyType[i] != -2)
		{
			DeleteElementInCards(iCopyCards, 21, iCopyType[i]);
		}
	}
	iAfterSingle= CInfo->CountCards(pSplitCards->startSplitCard(iCopyCards, 7));

	return iAfterSingle - iBeforeSingle;
}


//�жϲ�������У�ʣ�µ����Ƿ������ܶ൥�ƣ�������ᣬ�ж��Ƿ�Ϸ�������Ϸ�������н�����
//�ú��������deleteTypeLeftSingleCount���� ��˲��ƺ������ص�����ᱻ���
//����޶���:���Ľܣ�����޶�ʱ��:2017-07-21
int RecommendCards::judgeTypeAndAddInArr(fDdz *pDdz, int*iCards)
{
	int iCount = iPlaCount;
	int iTmp[38];
	memset(iTmp, -1, sizeof(iTmp));
	int iTmpIndex = 0;
	int iCopyType[38];
	memset(iCopyType, -1, sizeof(iCopyType));

	int i;
	for (i = 0; iCards[i] != -1; i++)
	{
		iCopyType[i] = iCards[i];
	}
	iCopyType[i] = -1;

	for ( i = 0; iCopyType[i] != -1; i++)
	{
		if (iCopyType[i] == -2)
		{
			iTmp[iTmpIndex++] = -2;
			iTmp[iTmpIndex++] = -1;
			iTmpIndex = 0;
			if (deleteTypeLeftSingleCount(pDdz->iOnHand, iTmp)< 2)//ɸѡ������˳�ӵģ����ж��Ƿ�Ϸ�
			{
				GetLegalCards(pDdz, iTmp);
			}
			memset(iTmp, -1, sizeof(iTmp));
		}
		else
		{
			iTmp[iTmpIndex++] = iCopyType[i];
		}
	}
	return iPlaCount - iCount;
}

//��ȡ������ͣ�1ΪAI�ǵ�����2Ϊ�¼��ǵ���,3Ϊ�ϼ��ǵ���
//����޶���:���Ľܣ�����޶�ʱ��:2017-7-13
//��˺ϸ�
int RecommendCards::GetPlayerType(fDdz *pDdz)
{
	if (pDdz->cDir == pDdz->cLandlord)
		return 1;
	else if (pDdz->cLastPlay == pDdz->cLandlord)
		return 3;
	else
		return 2;
}

//���������������ʣ����
//��δ�����ã�
int RecommendCards::OtherPlayersCards(fDdz *pDdz, Ddz *rDdz)
{
	int i;
	//�����AI
	if (pDdz->cDir == rDdz->cDir)
	{
		pDdz->iUpPlayerCards = rDdz->iUpPlayerCards;//�������������
		pDdz->iDownPlayerCards = rDdz->iDownPlayerCards;//�������������
	}
	//�����AI�������
	else if (pDdz->cDir == rDdz->cLastPlay)
	{

		for (i = 0; rDdz->iOnHand; i++);
		pDdz->iUpPlayerCards = rDdz->iDownPlayerCards;//�������������
		pDdz->iDownPlayerCards = i;
	}
	//�����AI�������
	else
	{
		for (i = 0; rDdz->iOnHand; i++);
		pDdz->iUpPlayerCards = i;//�������������
		pDdz->iDownPlayerCards = rDdz->iUpPlayerCards;
	}
	return 0;
}

//�ж�ǰ�������Ƿ�PASS����
bool RecommendCards::IfPassTwice(fDdz *pDdz)
{
	if (pDdz->iLastTypeCount <= 0)//����0ʱΪ����Pass
	{
		return true;
	}
	return false;
}


//DEBUG:������������ж϶����ܼ򵥽���״γ���ʱ�������������
//BUG
// ���SpitCard�ķ��ظ�ʽ�������
// ��iCards���������ܴ���ϼҳ��ƵĽ�,���浽����iSave����,iSave�����ʼ��Ϊ-1
//����޶���:�����Σ�����޶�ʱ��:16-07-28 23:50
//�޶����ݣ������жϳ���ʱ��Ե����
//2016/8/8:�����ж�ը����������
//2016/8/10 :ȡ������������ƿ����ж����Ƶ���䣬�������
int RecommendCards::GetLegalCards(fDdz *pDdz, int *iCards, int *iSave)
{
	//���ơ���������˳��˫˳����˳��������űȽϴ�С������һ����˳���ơ��Ĵ�������������������˳�����������űȽϴ�С������ƴ�С�޹ء�
	//�������ը���������ը���������������ͣ�ը��֮�䰴���Ŵ�С���бȽϡ�
	int iCardsTmp[50];
	int t = 0;//����ָ��iCards�±�
	int start;//ÿ�����Ϳ�ʼ���±�
	int iNodesCount = 0;

	//������ĳɡ�xxx -2 xxxx -2 -1���ṹ
	int num;
	for (num = 0; iCards[num] != -1; num++);
	if (iCards[num - 1] != -2)
	{
		iCards[num] = -2;
		iCards[num + 1] = -1;
	}

	for (int i = 0; iCards[i] != -1; i++)
	{
		start = 0;
		if (iCards[i] == -2)
		{
			iCardsTmp[t] = -1;
			CInfo->SortById(iCardsTmp);
			int iTypeCount = CInfo->AnalyzeTypeCount(iCardsTmp);
			int iType = iTypeCount / 100;						//�õ�����	
			int iCardsCount = iTypeCount % 100;					//�õ�����
			int myPoint = 0;

			if (pDdz->iLastMainPoint == -1 || pDdz->iLastTypeCount / 100 == iType)//���Ҷ�pass�����������µ����ͺ��ҵ����
			{
				if (pDdz->iLastTypeCount % 100 == iCardsCount || pDdz->iLastMainPoint == -1)
				{
					iCardsTmp[iCardsCount] = -1;
					//����Ϸ�,��ӵ�iSave
					myPoint = CInfo->AnalyzeMainMaxPoint(iCardsTmp);		//����iCard��������
					if (iLastMaxPoint<myPoint)
					{
						iCardsTmp[t] = -1;
						AppendCards(iCardsTmp, iSave);
						iNodesCount++;
					}
				}
			}

			//�����ը�����ϼҲ���ը��Ҳ���ǻ��
			if (iType == 2 && pDdz->iLastTypeCount / 100>2)
			{
				iCardsTmp[t] = -1;
				AppendCards(iCardsTmp, iSave);
				iNodesCount++;
			}

			//������
			if (iType == 1)
			{
				iCardsTmp[t] = -1;
				AppendCards(iCardsTmp, iSave);
				iNodesCount++;
			}
			t = 0;
		}
		else
		{
			iCardsTmp[t] = iCards[i];
			++t;
		}
	}
	return iNodesCount;
}

//���뵥��������Ϣ���ж��Ƿ�Ϸ�������Ϸ�������iplaarr
int RecommendCards::GetLegalCards(fDdz *pDdz, int *iCards)
{
	//���ơ���������˳��˫˳����˳��������űȽϴ�С������һ����˳���ơ��Ĵ�������������������˳�����������űȽϴ�С������ƴ�С�޹ء�
	//�������ը���������ը���������������ͣ�ը��֮�䰴���Ŵ�С���бȽϡ�

	int iCount = iPlaCount;				//����ģ����������������Ķ��ֳ��ƿ��н�����(jwj)
	int iCardsTmp[30];
	int t = 0;				//����ָ��iCards�±�
	int iTypeCount;
	int iType;
	int iCardsCount;
	int myPoint;
	for (int i = 0; iCards[i] != -1; i++)
	{
		/////////��������//////////////////
		if (iCards[i] == -2)// ����-2ʱ��
		{
			iCardsTmp[t] = -1;
			CInfo->SortById(iCardsTmp);						//���Ʊ�����������
			iTypeCount = CInfo->AnalyzeTypeCount(iCardsTmp);//����
				iType = iTypeCount / 100;
			iCardsCount = iTypeCount % 100;//����
			myPoint = CInfo->AnalyzeMainMaxPoint(iCardsTmp);//�������͵�������
			iCardsTmp[t] = -2;
			t++;
			iCardsTmp[t] = -1;
			t = 0;
			////////////�ж�////////////////////
			if (pDdz->iLastTypeCount / 100 == iType)
			{
				if (pDdz->iLastTypeCount % 100 == iCardsCount)//���Ʊ����������
				{
					if (iLastMaxPoint < myPoint)
					{
						AddInPlaArr(pDdz, iCardsTmp);
					}
				}
			}
			if (pDdz->iLastMainPoint == -1)//�������PASS
			{
				if (iTypeCount > 0)
				{
					AddInPlaArr(pDdz, iCardsTmp);
				}
			}

			//�����ը�����ϼҲ���ը��Ҳ���ǻ��
			if (iType == 2 && pDdz->iLastTypeCount / 100 > 2)
			{
				AddInPlaArr(pDdz, iCardsTmp);
			}
			//������
			if (iType == 1)
			{
				AddInPlaArr(pDdz, iCardsTmp);
			}
		}
		else
		{
			iCardsTmp[t] = iCards[i];
			++t;
		}
	}

	return iPlaCount - iCount;
}


//��һ�׷ֺ����͵��ƽ������ƴ洢��IPLARR�������� 
//TODO:�޸ľ�����Ϣ
//����޶���:���Ľܣ�����޶�ʱ��:17-07-16 
//�޶����ݣ����ӽڵ�����
int RecommendCards::AddInPlaArr(fDdz *pDdz, int *iCards)
{
	for (int i = 0; iCards[i] != -1; i++)   //�ж����Ƿ�������
	{
		if (iCards[i] != -2)
		{
			bool isIn = false;
			for (int j = 0; pDdz->iOnHand[j] != -1; j++)
			{
				if (iCards[i] == pDdz->iOnHand[j])//�ж��Ƿ��������� 
				{
					isIn = true;
					break;
				}
			}
			if (!isIn)                      //�����û���������У����ؽ��
				return iPlaCount;
		}
	}

	int num = iPlaCount;
	int t = 0;//�ڶ�ά�����±�
	if (iCards[0] != -2)                //������ƣ�����true
	{
		for (int i = 0; iCards[i] != -1; i++)
		{
			if (iCards[i] != -2)
			{
				//����ڵ����Ѿ��������ޣ�ֱ�ӷ���    //�����ﵽ����
				if (num + 2 > PlaMax)
					return num;
				pDdz->iPlaArr[num][t] = iCards[i];
				t++;
			}
			else
			{
				pDdz->iPlaArr[num][t] = -1;
				t = 0;
				num++;         //numΪ��������
			}
		}
	}
	else//PASS
	{
		//����ڵ����Ѿ��������ޣ�ֱ�ӷ���
		if (num + 2 > PlaMax)
			return num;
		pDdz->iPlaArr[num][t] = -2;
		t++;
		pDdz->iPlaArr[num][t] = -1;
		num++;
	}
	iPlaCount = num;
	return num;
}

// ����������������Ʋ�����iSave����
//��δ�����ã�
int RecommendCards::AnalyzeSplitedCards(int* iCards, int *iSave)
{
	int t = 0;//����ָ��iCards�±�
	for (int i = 0; iCards[i] != -1; i++)
	{

		if (iCards[i] != -2)
		{
			iSave[t] = iCards[i];
			++t;
		}
	}
	iSave[t] = -1;
	return t;
}


//��from����ӵ�to������棬to����ָ���Ϊ-2��������Ϊ-1
//�ӽ�������ʼд�����ݣ�����-2��-1��β
void RecommendCards::AppendCards(int* from, int* to)
{
	int i;
	//����to����-1����
	for (i = 0; to[i] != -1; i++);
	int j = i;

	for (i = 0; from[i] != -1; i++, j++)
	{
		to[j] = from[i];
	}

	//�ı��׺
	if (to[j - 1] != -2)
	{
		to[j] = -2;
		to[j + 1] = -1;
	}
}

//����������С����
int RecommendCards::MinHandNum(int* iOnHand)
{
	int iCopyOnHand[21];
	for (int i = 0; i < 21; i++)
	{
		iCopyOnHand[i] = iOnHand[i];
	}
	int num = 0;//����
	int t = 0;
	int j = 0;
	int bestStart = 0;
	int i = 0;
	int iTmpCards[21];

	memset(iTmpCards, -1, sizeof(iTmpCards));

	//�Ӳ��������У���Ѱ���������������,����icopyHand���ų�,����������
	auto findMaxNumCards = [&](int *iSplitedCards)
	{
		if (iSplitedCards[0] > -1)
		{
			t = 0;//���������
			j = 0;
			bestStart = 0;//���������ʼλ��
			for (i = 0; iSplitedCards[i] != -1; i++)
			{
				if (iSplitedCards[i] == -2)
				{
					if (j > t)
					{
						t = j;
						bestStart = i - t;
					}
					j = 0;
				}
				else
					j++;//����������
			}
			t = 0;
			for (i = bestStart; iSplitedCards[i] != -2; i++)
			{
				iTmpCards[t] = iSplitedCards[i];
				t++;
			}
			iTmpCards[t] = -1;
			num++;

			//��iCopyOnHand���ų�
			if (iTmpCards[0] > -1)
			{
				for (i = 0; iTmpCards[i] != -1; i++)
				{
					DeleteElementInCards(iCopyOnHand,21, iTmpCards[i]);
				}
			}
		}
	};

	//�ж����������ų�,����������  
	auto DeleteTypeCards = [&](int *iCards)
	{
		if (iCards[0] > -1)
		{
			t = 0;
			for (i = 0; iCards[i] != -1; i++)
			{
				if (iCards[i] == -2)
				{
					iTmpCards[t] = -1;
					for (j = 0; iTmpCards[j] != -1; j++)
					{
						DeleteElementInCards(iCopyOnHand, 21,iTmpCards[j]);//�Ӻ���ų�˳�������������ų������
					}
					num++;
					t = 0;
					iTmpCards[0] = -1;
				}
				else
				{
					iTmpCards[t] = iCards[i];
					t++;
				}
			}
		}
	};

	//�����жϻ��
	int *iCards = pSplitCards->startSplitCard(iOnHand, 0);
	if (iCards[0] > -1)
	{
		DeleteElementInCards(iCopyOnHand, 21, 52);
		DeleteElementInCards(iCopyOnHand, 21, 53);	
		num++;
	}

	int iSingleNum = 0;
	int iThreeNum = 0;
	int iDoubleNum = 0;
	
	//�жϵ��Ʋ�ɾ��
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 7);
	for (int i = 0;iCards[i]!=-1; i++)
	{
		if (iCards[i] != -2)
		{
			iSingleNum++;
		}
	}
	DeleteTypeCards(iCards);

	//�жϵ�˳,�ҵ����е�˳,���ų�
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 4);
	DeleteTypeCards(iCards);

	//�ж�ը��
	iCards = pSplitCards->startSplitCard(iOnHand, 1);
	DeleteTypeCards(iCards);

	//��¼����������
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
	for (int i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] == -2)
		{
			iThreeNum++;
		}
	}

	//�ҵ�������˳,���ų�
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 3);
	DeleteTypeCards(iCards);
	//�ж���������ɾ��
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 2);
	DeleteTypeCards(iCards);

	//�ж϶���
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
	for (int i = 0; iCards[i] != -1; i++)
	{
		if (iCards[i] != -2)
		{
			iDoubleNum++;
		}
	}

	//�ж�˫˳,�ҵ�����˫˳,���ų�
	iCards = pSplitCards->startSplitCard(iCopyOnHand, 5);
	DeleteTypeCards(iCards);

	iCards = pSplitCards->startSplitCard(iCopyOnHand, 6);
	DeleteTypeCards(iCards);

	int iTmpNum = 0;
	if (iThreeNum != 0)
	{
		if (iSingleNum>=iThreeNum)
		{
			num -= iThreeNum;
		}
		else if (iSingleNum < iThreeNum)
		{
			num -= iSingleNum;
			iTmpNum = iThreeNum - iSingleNum;
		}
		if (iDoubleNum >= iTmpNum)
		{
			num -= iTmpNum;
		}
		else if (iDoubleNum < iTmpNum)
		{
			num -= iDoubleNum;
		}
	}

	//�ж���(ʣ���Ʊ�Ϊ����2�����Ʋ����)
	for (i = 0; iCopyOnHand[i] != -1; i++)
	{
		if (iCopyOnHand[i] >= 48)
			num++;
	}
	return num;
}

//���ģ��
bool RecommendCards::GetModel(Ddz * realDdz)
{
	pRealDdz = realDdz;
	pMoldel->GetOtherPokersCards(iUpCards, iDownCards, pRealDdz);

	if (iFShowInConsole) 
	{
		//�ϼҵ���
		cout << "�²��ϼҵ���Ϊ " << endl;
		printCards(iUpCards);
	
		//�¼ҵ���
		cout << "�²��¼ҵ���Ϊ " << endl;
		printCards(iDownCards);

		cout << "�������ҵ���Ϊ ";
		printCards(pRealDdz->iOnOtherHand);
	}
	
	//����������
	int i = CInfo->CountCards(iUpCards);
	int j = CInfo->CountCards(iDownCards);
	if(pRealDdz->iUpPlayerCards != i || pRealDdz->iDownPlayerCards != j)
		return false;
	CInfo->SortById(iUpCards);
	CInfo->SortById(iDownCards);
	return true;
}

//���ƴ�ӡ����
void printCards(int*temp) {

	int j;
	int a = 0;
	for (int i = 0; temp[i] != -1; i++)
		a++;
	cout << "  ";
	//cout <<"����"<< a<<"���� ";
	for (int pass = a; pass >0; pass--)    //��С��������
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
			cout << " J ";    //J
		else if (temp[i] / 4 == 9)
			cout << " Q ";    //Q
		else if (temp[i] / 4 == 10)
			cout << " K ";    //K
		else if (temp[i] / 4 == 11)
			cout << " A ";    //A
		else if (temp[i] / 4 == 12)
			cout << 2 << " ";    //2
		else if (temp[i] == 52)
			cout << " С�� ";    //С��
		else if (temp[i] == 53)
			cout << " ���� ";    //����
	}
	cout << endl << endl;
}


//�������Ʒ���false,�ǲ�������true
bool RecommendCards::AnalyzeCondition(fDdz* pDdz)
{
	int sort = 0;//����
	//������������
	if (pDdz->cLandlord == pDdz->cDir)
	{
		if (pDdz->iUpPlayerCards <= 6 || pDdz->iDownPlayerCards <= 6)
			sort++;
	}
	else//AI���ǵ���
	{
		//�ϼ��ǵ���������С�ڵ���6
		if (pDdz->cLastPlay == pDdz->cLandlord&&pDdz->iUpPlayerCards <= 6)
			sort++;
		//�ϼҲ��ǵ��������¼�Ϊ���������¼�����С�ڵ���6
		if (pDdz->cLastPlay != pDdz->cLandlord&&pDdz->iDownPlayerCards <= 6)
			sort++;
	}

	//����������
	if (pDdz->cLandlord == pDdz->cDir)
	{
		if (iUpHandNum <= 3 || iDownHandNum <= 3)
			sort++;
		if (iUpHandNum < iHandNum&&iDownHandNum < iHandNum)
			sort++;
	}
	else//AI���ǵ���
	{
		//�ϼ��ǵ��������ϼҵ�����С�ڵ���3
		if (pDdz->cLastPlay == pDdz->cLandlord&&iUpHandNum <= 3)
			sort++;
		if (pDdz->cLastPlay != pDdz->cLandlord&&iDownHandNum <= 3)
			sort++;
	}
	//���Ƽ�ֵ����
	int MyValue = CardsValue(pDdz->iOnHand);
	int UpValue = CardsValue(iUpCards);
	int DownValue = CardsValue(iDownCards);
	if (pDdz->cLandlord == pDdz->cDir)
	{
		if (MyValue <UpValue&& MyValue <DownValue)
			sort++;
	}
	else
	{
		if (pDdz->cLastPlay == pDdz->cLandlord&&UpValue >MyValue&&UpValue>DownValue)
			sort++;
		if (pDdz->cLastPlay != pDdz->cLandlord&&DownValue >MyValue&&DownValue >UpValue)
			sort++;
	}
	if (sort >= 2)
		return false;
	else
		return true;
}

//��iCards[]���޳���ֵΪiElement����
void RecommendCards::DeleteElementInCards(int* iCards, int iCardsCount, int iElement)
{
	int i, j;
	i = j = 0;
	for (j = 0; j < iCardsCount; j++)
	{
		if (iCards[j] == iElement)
			i++;
		else
			iCards[j - i] = iCards[j];
	}
}

//�õ�����˳�ӳ��ȣ��������Ƿ���Բ���
//����޶���:���Ľ�����޶�ʱ��:2018-07-21
int RecommendCards::AnalyzeSingleLink(int Length, int iLastMainPoint, int *Cards, int* iTmpShunzi)
{
	if (Length < 5||Length>12) //���Ϸ�
		return 0;

	int i, j, k;
	int iCards[21];
	for (i = 0; i < 21; i++)
	{
		iCards[i] = Cards[i];
	}

	for (int i = 0; iCards[i] != -1; i++) //ȥ��2��С��   
	{
		if (iCards[i] >= 48)
		{
			iCards[i] = -1;
		}
	}

	//ȥ�����Ʋ���С��������
	CInfo->GetUnReaptedCards(iCards);

	int iBiaoji, iFlag, num ; 
	num = 0;

	for (i = 0; iCards[i + Length - 1] >= 0; i++)
	{
		num = 0;//������
		iBiaoji = 0;
		iFlag = 0;

		//�����������һ���Ƶ���С����
		if (iCards[i] / 4 > iLastMainPoint)//����Ҫȫ���ƶ�������һ����ҵ���С����
		{
			for (j = i; j < Length - 1 + i; j++)
			{
				//����������Ƶ㲻���򣬱�ǲ��˳�ѭ��
				if (iCards[j] / 4 != iCards[j + 1] / 4 - 1)
				{
					iBiaoji = 1;
					break;
				}
			}
		}
		else
			continue;

		if (iBiaoji == 0)
		{
			for (k = i; k < Length + i; k++)
			{
				if (iCards[k] < 0)
					break;

				iTmpShunzi[iFlag] = iCards[k];
				iFlag++;

				if (k == Length - 1 + i)
				{
					num = iFlag;
					iTmpShunzi[iFlag++] = -2;
					iTmpShunzi[iFlag++] = -1;
					return num;
				}
			}
		}
	}

	return num;
}


int RecommendCards::CardsValue(int* iCards)
{
	int i;
	int dSum = 100;			//��ֵ
	for (i = 0; iCards[i] >= 0; i++)
	{
		dSum = dSum - 5;			//����Խ��Խ��ÿ��һ�������Ƽ�5��
		if (i >= 1 && iCards[i - 1] / 4 == iCards[i] / 4)
			dSum = dSum + 2;	//���Ӽ�2��
		if (i >= 4 && iCards[i - 4] / 4 <= 7
			&& iCards[i - 4] / 4 + 1 == iCards[i - 3] / 4
			&& iCards[i - 3] / 4 + 1 == iCards[i - 2] / 4
			&& iCards[i - 2] / 4 + 1 == iCards[i - 1] / 4
			&& iCards[i - 1] / 4 + 1 == iCards[i] / 4)
			dSum = dSum + 6;	//2�����������Ƶ�˳��6��
	}
	return dSum;
}
