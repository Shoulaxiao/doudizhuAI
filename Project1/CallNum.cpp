
#include "CallNum.h"

CallNum::CallNum()//�����޸�
{
	CInfo = CardsInfo::Instance();
	pSplitCards = new SplitCard();
}

CallNum::~CallNum()//�����޸�
{
}

CallNum* CallNum::Instance()//�����޸�
{
	static CallNum instance;

	return &instance;
}
/*
����޶��������� 18-07-22
*/

int CallNum::CardForce(int iCards[])
{
	int iMyCards[18] = { 0 };//��������

	int CopyiMyCards[18] = { 0 };//��������

	//�������ƣ�ż��Ϊ�ƵĴ�С������Ϊ���Ƶ�����
	int iRepeat[27] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };//������ֻ�����Ƽ������� 

	int iBRepeat[27] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };//���Q���ϵ�ը������������

	int iOrder[18] = { 0 };//���� 

	int iStraight[9] = { 0 };//˳�ӣ�ÿ��λ��ʾһ��˳�ӣ���һλΪ��ʼ���ڶ�λΪ����������λΪ��ʼ�ͽ����Ĳ�ֵ

	int iSingle = 0;//���ţ�����������

	int iForce = 0;//����

	bool bLock = true;//������


	int*single = pSplitCards->startSplitCard(iCards, 7);//��Ӧ�����еĽӿں����еĵ�������
	//С��A�ĵ��Ƶ�Ȩֵ
	for (int i = 0; single[i] != -1; i++)
	{
		if (single[i] != -2 && single[i] < 48)
		{
			iSingle++;
			iForce = single[i] - 10;
		}
	}

	//�������ͣ�ת��Ϊʵ�ʵ���
	for (int i = 0; i < 17; i++)
	{

		if (iCards[i] / 4 == 0)
		{
			iMyCards[i] = 3;    //3
		}
		if (iCards[i] / 4 == 1)
		{
			iMyCards[i] = 4;    //4
		}
		if (iCards[i] / 4 == 2)
		{
			iMyCards[i] = 5;    //5
		}
		if (iCards[i] / 4 == 3)
		{
			iMyCards[i] = 6;    //6
		}
		if (iCards[i] / 4 == 4)
		{
			iMyCards[i] = 7;    //7
		}
		if (iCards[i] / 4 == 5)
		{
			iMyCards[i] = 8;    //8
		}
		if (iCards[i] / 4 == 6)
		{
			iMyCards[i] = 9;    //9
		}
		if (iCards[i] / 4 == 7)
		{
			iMyCards[i] = 10;    //10
		}
		if (iCards[i] / 4 == 8)
		{
			iMyCards[i] = 11;    //J
		}
		if (iCards[i] / 4 == 9)
		{
			iMyCards[i] = 12;    //Q
		}
		if (iCards[i] / 4 == 10)
		{
			iMyCards[i] = 13;    //K
		}
		if (iCards[i] / 4 == 11)
		{
			iMyCards[i] = 14;    //A
		}
		if (iCards[i] / 4 == 12)
		{
			iMyCards[i] = 22;    //2
		}
		if (iCards[i] == 52)
		{
			iMyCards[i] = 66;    //С��
		}
		if (iCards[i] == 53)
		{
			iMyCards[i] = 99;    //����
		}
	}

	//���¶���һ�����ƺ���������Q����ը���ļ�¼

	for (int i = 0, j = 0; i < 17; i++)//ͳ���������������С
	{
		if (iMyCards[i] == iMyCards[i + 1])
		{
			iBRepeat[j] = iMyCards[i + 1];
			iBRepeat[j + 1]++;
		}
		else
		{
			j += 2;//��iRepeat���ƶ���¼
		}
	}

	//�Ƚ�Q���ϵ�ը��ֱ�Ӽ���Ȩֵ
	for (int i = 1; i < 26; i += 2)
	{
		//Q����ը����Ȩֵ���㣬��iBRepeat���ж�
		if (iBRepeat[i] == 4 && iBRepeat[i - 1] > 11)
		{
			iForce = iBRepeat[i - 1] - 4;
		}
	}
	//˳�ӵĵ�һ��ɸѡ
	for (int i = 0, j = 0; i < 17; i++)
	{
		if (iMyCards[i] + 1 == iMyCards[i + 1])//�ж��Ƿ�����
		{
			iOrder[j] = iMyCards[i];    //����������ڵ��Ƶ���������ֻ��¼��ǰһ��
			j++;
		}
		//�������ڵ��ƣ����ߴ���ǰ��2�㼰���ϣ�����Ϊ���һ����
		else if (iMyCards[i] + 1 < iMyCards[i + 1] || i == 16)
		{

			if (iOrder[j - 1] + 1 == iMyCards[i])
			{
				iOrder[j] = iMyCards[i];//�ѱ�Ӧ�����������һ��������iOrder��
				j += 2;//��iOrder������һ���������洢��һ��˳��
			}
		}
		else
		{
			continue;//����������ȵ��Ʋ���
		}
	}
	//˳�ӵĵڶ���ɸѡ
	for (int i = 0, j = 0; i < 17; i++)
	{
		if (iOrder[i] != 0)
		{
			if (bLock)
			{
				iStraight[j] = iOrder[i];//˳�ӿ�ʼ�ĵ�һ�ŷ���iStraight
				j++;
				bLock = false;
			}
		}
		else
		{
			if (iOrder[i - 1] == 0 && iOrder[i + 1] == 0)//ǰ���Ϊ0��ʾiOreder����˳�ӣ�������
			{
				break;
			}
			else
			{
				if (iOrder[i - 1] - iStraight[j - 1] >= 4)
				{
					iStraight[j] = iOrder[i - 1];//˳�ӵĵڶ�λΪ���һ����
					j++;
					iStraight[j] = iStraight[j - 1] - iStraight[j - 2];//����λ��ʾ�ӿ�ʼ��������Ҫ�����μӷ�
					j++;
					bLock = true;
				}
				else//������˳�ӵ�����������ɸѡ
				{
					iStraight[j - 1] = 0;
					j--;
					bLock = true;
				}
			}
		}
	}

	//˳�ӵ�Ȩֵ����,�����ı�˳�������ֵ
	for (int i = 1; i < 8; i += 3)
	{
		if (iStraight[i + 1] >= 7)
		{
			iForce = iStraight[i] - 7;//iStraight[i] - 10 + 3
		}
		else
		{
			iForce = iStraight[i] - 8;//iStraight[i] - 10 + 2
		}
	}

	//���ӡ�������ը���ĵ�һ��ɸѡ��֮��Ĵ��ƶ�������Щ�Ļ����Ͻ�����ϣ�
	for (int i = 0, j = 0; i < 17; i++)//ͳ���������������С
	{
		if (iMyCards[i] == iMyCards[i + 1])
		{
			iRepeat[j] = iMyCards[i + 1];
			iRepeat[j + 1]++;
		}
		else
		{
			j += 2;//��iRepeat���ƶ���¼
		}
	}

	//��˳�ӵ�ǰ�������¶����ƽ���ɸѡ(�����������֮���ʹ˳�����鷢���ı䣬����Ӧ��ǰ�����˳��Ȩֵ�ļ���)
	for (int i = 0; i < 26; i += 2)
	{
		for (int x = 0, y = 1; x < 8; x += 3)
		{
			for (int z = 2; iStraight[z] > 0; iStraight[z]--)
			{
				if (iStraight[x] == iRepeat[i])
				{
					iRepeat[i + 1]--;
				}
				else if (iStraight[x] < iStraight[y])
				{
					iStraight[x] = iStraight[x] + 1;
				}
			}
		}
	}

	//�ڶ���ɸѡ������Ȩֵ�ļ���
	for (int i = 1; i < 26; i += 2)
	{
		//ը����С��Q�ģ�
		if (iRepeat[i] == 4 && iRepeat[i - 1] < 11)
		{
			iForce = iRepeat[i - 1] - 4;
		}
		//���ӵ�Ȩֵ
		if (iRepeat[i] == 2 && iRepeat[i - 1] < 48)//ʣ����ӣ�С��2����Ȩֵ����
		{
			iForce = iRepeat[i - 1] - 10;
		}
		//��������˳��Ȩֵ
		if (iRepeat[i] == 3 && iRepeat[i - 1] < 48)//ʣ����������˳��С��2����Ȩֵ����
		{
			iForce = iRepeat[i - 1] - 8;
			/*���Է�ֹ�ظ�����һ��������ֵ
			eg:444555999
			����444��Ȩֵ��Ȼ��������˳����������+1���ٽ���555�ļ���
			����444��Ȩֵ+555��Ȩֵ+1==444555��Ȩֵ
			*/
			if (iRepeat[i - 1] + 1 == iRepeat[i + 1] && iRepeat[i + 2] == 3)
			{
				iForce++;
			}
		}
	}


	//�����жϺ���
	/*
	��������˼�룺��������
	1.ÿ���ƶ������и�����������ֵ����10Ϊ�м�ֵ0��С��10�����εݼ�������10�����ε�����A��2��6��С����7��������8����
	2.���ƾ���ÿ���ƵĻ���ֵ
	3.�����ڻ���Ȩֵ�Ļ�����+1
	4.�������������ʹ��ԣ�һ����Ϊ���Ǵ�10�����µ��ƣ�����ȨֵΪ����Ȩֵ+2
	5.��˳��˫˳��Ȩֵ��Ϊ���һ���ƵĻ���Ȩֵ+2��3���ݶ�Ϊ��˳7�ż�����+3��˫˳4�Լ�����+3��
	6.��˳����˳���ƣ���Ϊ�����ƾ�Ϊ10���µ��ƣ���ȨֵΪ����Ȩֵ+5����ֹ��ֺ�������ĵ�Ȩֵһ��������С��
	7.ը�����Ĵ�������Ϊը���ķ����ʹ��Ƶ�Ч��һ������Ȩֵ��Ϊ����Ȩֵ+6
	8.���Ȩֵ����Ϊ16
	PS����˳Ϊ����������Ǻܶ࣬��ʱ��������Ȩֵ
	*/


	//�����ļ���(���Ӽ��޸�Ȩֵ)
	for (int i = 0; i < 17; i++)
	{
		//���������С��3����������ֵ(���Կ���ɾ��)
		/*if (iSingle < 3)
		{
			iForce += 3 - iSingle;
		}*/
		if (iFShowInConsole)
			cout << " SingleNumber " << iSingle << endl;

		return iForce;
	}
}


/*
����޶��������� 18-07-22
*/
//�жϽ��Ƶ���ipip��ipnum��Ϊfloat
int CallNum::CallCardAnalyze(Ddz* pDdz)
{
	int iInHand = 0;//������
	int iBig = 0;//ͳ������A���ϵĴ����ж���
	float iPip = 0.0;//����Ȩֵ
	int bomb = 0;//�ж��Ƿ���˫����˫��ֻ��ѭ�����㵥���еļ�ֵ���ֲ�������
	int iCallPoint = -1;//���ս�����
	int iHeadCallPointSecond;//�ϼҽ����ܵ���
	int iHeadCallPointFirst;//���ϼҽ��Ƶ���

	/*//��ȡ�ϼҽ�����
	if (pDdz->iBid[0] == -1)
	{
		iHeadCallPointFirst = -1;//���ϼҽ��Ƶ���
		iHeadCallPointSecond = -1;//�ϼҽ��Ƶ���
	}
	else if (pDdz->iBid[1] == -1)
	{
		iHeadCallPointFirst = -1;//���ϼҽ��Ƶ���
		iHeadCallPointSecond = pDdz->iBid[0];//�ϼҽ��Ƶ���
	}
		else if (pDdz->iBid[2] == -1)
		{
			iHeadCallPointFirst = pDdz->iBid[0];//���ϼҽ��Ƶ���
			iHeadCallPointSecond = pDdz->iBid[1];//�ϼҽ��Ƶ���
		}
		*/
		//������ҵı�Ų�׼����¼����Ƶ���
	if (pDdz->cDir == 'A')//0 1 2 
	{
		iHeadCallPointFirst = pDdz->iBid[1];
		iHeadCallPointSecond = pDdz->iBid[2];
	}
	else if (pDdz->cDir == 'B')
	{
		iHeadCallPointFirst = pDdz->iBid[0];//iBid[2]ԭ
		iHeadCallPointSecond = pDdz->iBid[2];//iBid[0]ԭ
	}
	else if (pDdz->cDir == 'C')
	{
		iHeadCallPointFirst = pDdz->iBid[0];
		iHeadCallPointSecond = pDdz->iBid[1];
	}


	//�Ƶ��жϣ���A���ϵĵ��Ƶļ�ֵ����
	for (int i = 0; i < 17; i++)
	{
		iInHand = pDdz->iOnHand[i];
		if (iInHand >= 53)		//���� iPip += 8;
		{
			bomb += 2;
			iBig += 3;
		}
		else if (iInHand >= 52)	//С�� iPip +=7;
		{
			bomb += 1;
			iBig += 3;
		}
		else if (iInHand >= 48)	//2
		{
			iPip += 6.0;
			iBig += 2;
		}
		else if (iInHand >= 44) //A
		{
			iBig++;
		}
	}

	if (bomb == 3)//˫��
		iPip += 16.0;//iPip += 16;
	if (bomb == 1)//С��
		iPip += 7.0;//iPip += 7;
	if (bomb == 2)//����
		iPip += 8.0;//iPip += 8;

	//���Ʒ���
	//���Ƶ�һ������Ļ�������������Ҫ�м��Ŵ���
	if (iBig >= 5)
	{
		float iPipNum = CardForce(pDdz->iOnHand) + iPip;//һ�����Ƶ����ռ�ֵ

		if (iFShowInConsole)
		{
			//Test
			cout << "���͵�Ȩֵ�������� 2 ��С����:  " << iPipNum - iPip << endl;
			cout << "���Ƶ�Ȩֵ��2 ��С����:  " << iPip << endl;
			cout << "�ܵ�Ȩֵ:  " << iPipNum << endl;
			cout << "���Ƶ���Ŀ��ʾ:  " << iBig << endl;
		}

		//��ʾ�Լ���A��ʱ��
		if (iHeadCallPointFirst == -1 && iHeadCallPointSecond == -1)//����ֱ�ӽ��ƣ����ÿ���������
		{
			if (iPipNum >= 18.0)
			{
				if (3 / 2 * iPip >= iPipNum)
				{
					iCallPoint = 3;
				}
				else if (3 / 2 * iPip >= 3 / 4 * iPipNum)
				{
					iCallPoint = 2;
				}
				else
				{
					iCallPoint = 1;
				}
			}
			else if (iPipNum >= 10.0)
			{
				if (iPip == iPipNum)
				{
					iCallPoint = 0;
				}
				else if (iPip >= 1 / 2 * iPipNum)
				{
					iCallPoint = 1;
				}
				else
				{
					iCallPoint = 0;
				}
			}
			else
			{
				iCallPoint = 0;
			}
		}
		//��ʾ�Լ�ΪB��ʱ��,�ҵ�׼���з���Aһ��ʱ��ѡ�񲻽�
		if (iHeadCallPointFirst != -1 && iHeadCallPointSecond == -1)//���ϼҽ����ƣ��ֵ�������
		{
			if (iPipNum >= 18.0)
			{
				if (iPipNum >= 18.0)
				{
					if (3 / 2 * iPip >= iPipNum && iHeadCallPointFirst < 3)
					{
						iCallPoint = 3;
					}
					else if (3 / 2 * iPip >= 3 / 4 * iPipNum && iHeadCallPointFirst < 2)
					{
						iCallPoint = 2;
					}
					else if (iHeadCallPointFirst < 1)
					{
						iCallPoint = 1;
					}
					else
					{
						iCallPoint = 0;
					}
				}
				else if (iPipNum >= 10.0)
				{
					if (iPip == iPipNum)
					{
						iCallPoint = 0;
					}
					else if (iPip >= 1 / 2 * iPipNum && iHeadCallPointFirst < 1)
					{
						iCallPoint = 1;
					}
					else
					{
						iCallPoint = 0;
					}
				}
				else
				{
					iCallPoint = 0;
				}
			}
		}
		//��ʾ�Լ�ΪC��ʱ��,�ҵ�׼���з���Bһ��ʱ��ѡ�񲻽�
		if (iHeadCallPointFirst != -1 && iHeadCallPointSecond != -1)
		{
			if (iPipNum >= 18.0)
			{
				if (3 / 2 * iPip >= iPipNum && iHeadCallPointFirst < 3 && iHeadCallPointSecond < 3)
				{
					iCallPoint = 3;
				}
				else if (3 / 2 * iPip >= 3 / 4 * iPipNum && iHeadCallPointFirst < 2 && iHeadCallPointSecond < 2)
				{
					iCallPoint = 2;
				}
				else if (iHeadCallPointFirst < 1 && iHeadCallPointSecond < 1)
				{
					iCallPoint = 1;
				}
				else
				{
					iCallPoint = 0;
				}
			}
			else if (iPipNum >= 10.0)
			{
				if (iPip == iPipNum)
				{
					iCallPoint = 0;
				}
				else if (iPip >= 1 / 2 * iPipNum && iHeadCallPointFirst < 1 && iHeadCallPointSecond < 1)
				{
					iCallPoint = 1;
				}
				else
				{
					iCallPoint = 0;
				}
			}
			else
			{
				iCallPoint = 0;
			}
		}
	}
		else //��iBig̫С��ʾû�ж��ٴ��ƣ�ĳЩ����������Կ��ǽ�1��
		{
			int iPipNum = CardForce(pDdz->iOnHand) + iPip;//һ�����Ƶ����ռ�ֵ
			if (iFShowInConsole)
			{
				//Test
				cout << "���͵�Ȩֵ��������2�ʹ�С����Ȩֵ��:  " << iPipNum - iPip << endl;
				cout << "���Ƶ�Ȩֵ��A 2 ��С����:  " << iPip << endl;
				cout << "�ܵ�Ȩֵ:  " << iPipNum << endl;
				cout << "���Ƶ���Ŀ��ʾ:  " << iBig << endl;
			}

			//��ʾ�Լ���A��ʱ��(��Ҫ����Ū)
			if (iHeadCallPointFirst == -1 && iHeadCallPointSecond == -1)
			{
				if (iPipNum >= 10 && iPip <= 1 / 2 * iPipNum)
				{
					iCallPoint = 1;
				}
				else
				{
					iCallPoint = 0;
				}
			}

			//��ʾ�Լ�ΪB��ʱ��,�ҵ�׼���з���Aһ��ʱ��ѡ�񲻽�
			if (iHeadCallPointFirst != -1 && iHeadCallPointSecond == -1)
			{
				if (iPipNum >= 10 && iPip <= 1 / 2 * iPipNum && iHeadCallPointFirst < 1)
				{
					iCallPoint = 1;
				}
				else
				{
					iCallPoint = 0;
				}
			}

			//��ʾ�Լ�ΪC��ʱ��,�ҵ�׼���з���Bһ��ʱ��ѡ�񲻽�
			if (iHeadCallPointFirst != -1 && iHeadCallPointSecond != -1)
			{
				if (iPipNum >= 10 && iPip <= 1 / 2 * iPipNum && iHeadCallPointFirst < 1 && iHeadCallPointSecond < 1)
				{
					iCallPoint = 1;
				}
				else
				{
					iCallPoint = 0;
				}
			}

		}



		//�ж��Ƿ���2�����������ֵ�ǰ�ᣬ����Ҫ��1������2���� ����������һ����
		/*int j = 0,k=0;
		if (iCallPoint == 3)
		{
		for (int i = 0; i < 17; i++)
		{
		if (pDdz->iOnHand[i]/4==12)
		{
		j++;//2
		}
		else if (pDdz->iOnHand[i] / 4 == 13)
		{
		k++;//��
		}
		}
		//û������������û����
		if (j < 2 || k < 1)
		{
		if (iHeadCallPointSecond != 2 && iHeadCallPointFirst != 2)
		iCallPoint = 2;
		else
		iCallPoint = 0;
		}
		//����������һ�������ϵĶ�
		if (j >= 1 && k == 2)
		{
		iCallPoint = 3;
		}
		}*/

		//���ؽ��Ƶ���
		return iCallPoint;
	}