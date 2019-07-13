#include "PokerModelAnalysis.h"



void PokerModelAnalysis::Init()//��ʼ��
{
	UpWrong = 0;
	DownWrong = 0;
	for (int i = 0; i < 8; i++)////һ����ά���飬�����ͣ��������ӣ�������˵���Ͳ�����
	{
		for (int j = 0; j < 40; j++)
		{
			iUtimate[i][j] = -1;////8��40�����飬����Ԫ�����ݸ���ֵΪ��1
		}
	}
	for (int i = 0; i < 21; i++)////���20�ţ�������ʼ���ϼ��¼ҵ��Ƶ�����
	{
		iOneUtimate[i] = -1;
	}
	for (int i = 0; i < 21; i++)////2���¼�
	{
		iTwoUtimate[i] = -1;
	}
	for (int i = 0; i < 38; i++)////�����Լ�����17�ţ�ʣ�µ��Ʒ���һ�����飬���ֵ������ƶ������������
	{
		Buffer[i] = pMyDdz->iOnOtherHand[i];////��������װ��һ������������.����û�п����������Ķ���
	}
}//��ʼ��������Ϣ

PokerModelAnalysis::PokerModelAnalysis()////���캯����ab�����Ķ���
{
	A = new SplitCard();
	B = new CardsInfo();
}
PokerModelAnalysis::~PokerModelAnalysis()////��������
{
}
void PokerModelAnalysis::GetOtherPokersCards(int *iUpCards, int *iDownCards, Ddz *pDdz)////���������ҵ���
{
	pMyDdz = pDdz;////ָ�룬�ҵ�ָ��
	Init();////���ó�ʼ���Ǹ����������Ľ������������ȫԪ��Ϊ��1�ĳ�ʼ��
	if (pMyDdz->iOTmax <= 2)//��ǰ��������С��2
	{
		GameStart();////��������������һ��������ģ�⣬һ������ʵ�Ķ���ģ��
		Gaming();
	}
	else
	{
		Gaming();////��������������2�󣬾͵�����һ��
	}
	for (int i = 0; i < 21; i++)////ѭ�������ϼҵ��������е�Ԫ�����η����ϼ�����
	{
		iUpCards[i] = iOneUtimate[i];////�ϼҵ��ƣ���recommend cards���涨���
	}
	for (int i = 0; i < 21; i++)////ѭ�������¼ҵ��������е�Ԫ�����η����¼�����
	{
		iDownCards[i] = iTwoUtimate[i];////�¼ҵ���
	}
}

void PokerModelAnalysis::GameStart()
{
	for (int i = 0; i < 21; i++)////����ѭ����ѭ�����ұ��е��ƣ�����ֵȫ����Ϊ��1
	{
		iOneDe[i] = -1;////�ϼұ��е���
	}
	for (int i = 0; i < 21; i++)
	{
		iTwoDe[i] = -1;////�¼ұ��е���
	}
	if (pMyDdz->cDir != pMyDdz->cLandlord)////���Ҳ��ǵ�����ʱ��
	{
		Fendipai();////���������
	}
}

void PokerModelAnalysis::Gaming()////ִ��������������ǵ�һ����������
{
	DeleteC();
	Analysis(2);
	Analysis(1);
	Random(Buffer);
}

void PokerModelAnalysis::Analysis(int t)
{
/*------------------------------------------------------
�����ͷ����������е�������ӽ�ȥ���ܹ�15�֣�����pass
����д�ߣ�����ӱ   2017��07��05
*///-----------------------------------------------------------
	int iCs[21];////���Ƶ����鰡�����20�ţ�����һ����ȥģ����Ƶ����飬����17�����Ƽ���3�ŵ��Ƶ����
	for (int i = 0; i < 21; i++)
	{
		iCs[i] = -1;////��������ѭ��Ȼ��ȫ����ֵΪ��1��
	}
	if (t == 2)////�����Ĳ���������2���ǵ��� Analysis(2);Ȼ������һ��������1��2ֻ���������벻ͬ��if��� ����2Ӧ���ǽ����ϼҵ����ݣ�����1�ǽ����¼ҵ�����
	{
		for (int i = 0; i < 21; i++)
		{
			iCs[i] = pMyDdz->iOnTable[pMyDdz->iOTmax - 2][i];////�ѳ������飨����ֵ��ʼ��Ϊ-2��ÿ����һ���ƣ���-1��β��Pass��Ϊ-1��162=54*3
		}
		DeleteB(iCs, 2);////Ȼ��������delete����

		if (B->IsType0Pass(iCs) == 1)////���pass�˾�ֱ�ӷ��أ������cardsinfo����ģ��ж�PASS������1�ǣ�0����
		{
			return;
		}
		if (B->IsType1Rocket(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type1(iCs, 2);
			return;
		}
		if (B->IsType2Bomb(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type2(iCs, 2);
			return;
		}
		if (B->IsType3Single(iCs) == 1)////�ǵ��� ��������ȥ���ú���ĺ���   �жϵ��ƣ�����1�ǣ�0����
		{
			DeleteB(iCs, 1);
			Type3(iCs, 2);
			return;
		}
		if (B->IsType4Double(iCs) == 1)////�Ƕ��� ����   �ж϶��ƣ�����1�ǣ�0����
		{
			DeleteB(iCs, 1);
			Type4(iCs, 2);
			return;
		}
		if (B->IsType5Three(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type5(iCs, 2);
			return;
		}
		if (B->IsType6ThreeOne(iCs) == 1)////������һ ����   �ж�����һ��������1�ǣ�0����
		{
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
		if (B->IsType6ThreeDouble(iCs) == 1) {
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
		if (B->IsType7LinkSingle(iCs) == 1)//�ǵ�˳ ����
		{
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
		if (B->IsType8LinkDouble(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
		if (B->IsType9LinkThree(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
		if (B->IsType10LinkThreeSingle(iCs) == 1)
	    {
			DeleteB(iCs,1);
			Type6_11(iCs,2);
			return;
		}
		if (B->IsType10LinkThreeDouble(iCs) == 1)
		{
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
		if (B->IsType11FourSingle(iCs) == 1)
		{
			DeleteB(iCs,1);
			Type6_11(iCs,2);
			return;
		}
		if (B->IsType11FourDouble(iCs) == 1) {
			DeleteB(iCs, 1);
			Type6_11(iCs, 2);
			return;
		}
	 }
	if (t == 1)//t��1���������if����е����ݣ��ǵ�ǰ����������1����һ�е�i��Ԫ�ص�ֵ���������cs������
	{
		for (int i = 0; i < 21; i++)
		{
			iCs[i] = pMyDdz->iOnTable[pMyDdz->iOTmax - 1][i];
		}
		DeleteB(iCs, 1);

		if (B->IsType0Pass(iCs) == 1)
			{
				return;
			}
		if (B->IsType1Rocket(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type1(iCs, 1);
				return;
			}
		if (B->IsType2Bomb(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type2(iCs, 1);
				return;
			}
		if (B->IsType3Single(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type3(iCs, 1);//���type3��������ֵ��
				return;
			}
		if (B->IsType4Double(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type4(iCs, 1);
				return;
			}
		if (B->IsType5Three(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type5(iCs, 1);
				return;
			}
		if (B->IsType6ThreeOne(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type6_11(iCs, 1);
				return;
			}
		if (B->IsType6ThreeDouble(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type6_11(iCs, 1);
				return;
			}
		if (B->IsType7LinkSingle(iCs) == 1)//�ǵ�˳ ����
			{
				DeleteB(iCs, 2);
				Type6_11(iCs, 1);
				return;
			}
		if (B->IsType8LinkDouble(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type6_11(iCs, 1);
				return;
			}
		if (B->IsType9LinkThree(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type6_11(iCs, 1);
				return;
			}
		if (B->IsType10LinkThreeSingle(iCs) == 1)
			{
				DeleteB(iCs,2);
				Type6_11(iCs,1);
				return;
			}
		if (B->IsType10LinkThreeDouble(iCs) == 1)
			{
				DeleteB(iCs,2);
				Type6_11(iCs, 1);
				return;
			}
		if (B->IsType11FourSingle(iCs) == 1)
			{
			    DeleteB(iCs,2);
			    Type6_11(iCs,1);
				return;
			}
		if (B->IsType11FourDouble(iCs) == 1)
			{
				DeleteB(iCs, 2);
				Type6_11(iCs, 1);
				return;
			}
		}
	}

void PokerModelAnalysis::Random(int *iBuffer)
{
	int i = 0;
	int j = 0;
	int one = 0;
	int two = 0;
	int k = 0;
	while (iOneDe[j] != -1)////ֻҪ�����ϼұ��е���ֵ�����ڣ�1����ѭ����ѭ����Ҳ������ֵ��ʱ��
	{
		j++;////��ʵ�������ϼұ��е��Ƶ�����
	}
	if (j > pMyDdz->iUpPlayerCards)////����ϼұ��е��ƴ����ϼ����е��������Ͱ�j��ֵ�ָ�Ϊ0,���൱��֪�����е���
	{
		j = 0;
	}
	one = pMyDdz->iUpPlayerCards - j;////����������ϼұ��е��ƴ����ϼ����ƣ��ͽ����ߵĲ���ϼҼ���ֵ

	j = 0;////������ϼҵĲ��ֺ󣬾������㣬��ʼ�����¼ҵļ���

	while (iTwoDe[j] != -1)////ͬ�������ݣ���������Ƕ����¼ҵ�
	{
		j++;////�����¼ұ��е��Ƶ�����
	}
	if (j > pMyDdz->iDownPlayerCards)////����¼ұ��е��ƴ����¼����е��������Ͱ�j��ֵ�ָ�Ϊ0,���൱��֪�����е���
		j = 0;
	two = pMyDdz->iDownPlayerCards - j;////��¼���˱��е��ƻ��ж�����Ҫ�ָ��¼ң��������Ƽ�ȥ���е��ƣ�����Ŀǰδ֪���Ƶ�����

	i = one + two;//��һ������i����������ܹ�δ֪��������

	std::vector<int> vi(i);//����һ������
	//srand(unsigned(time(NULL)));
	for (int n = 0; n < i; n++)
	{
		vi[n] = iBuffer[n];
	}
	random_shuffle(vi.begin(), vi.end());
	j = 0;
	vector<int>::iterator it;
	for (it = vi.begin(); it != vi.begin() + one; it++)//���ϼ��������
	{
		iOneUtimate[j] = *it;
		j++;
	}
	for (k = 0; iOneDe[k] != -1; k++)//���ϼұ��е����ӽ�ȥ
	{
		iOneUtimate[j] = iOneDe[k];
		j++;
	}
	j = 0;
	for (it = vi.begin() + one; it != vi.end(); it++)//���¼��������
	{
		iTwoUtimate[j] = *it;
		j++;
	}
	k = 0;
	while (iTwoDe[k] != -1)//���¼ұ��е����ӽ�ȥ
	{
		iTwoUtimate[j] = iTwoDe[k];
		j++;
		k++;
	}
}

/*-------------------------------------------------------------------------------------
*����ģ���ж�  �ܹ������͵����з�֧
**  ��д�ߣ�����ӱ    2017-07-07
** �˴�û��Type0,�ж�Ϊpass���ֱ�ӷ����ˡ�����Ҫ��ʹ�ú����Ի�����������޸�
*///------------------------------------------------------------------------------------
void PokerModelAnalysis::Type1(int *iCs, int k)//˫��
{
	int a = iCs[0] / 4;//��4ȡ�����жϵ�ǰ���棬���������ϳ���������
	for (int i = 0; Buffer[i] != -1; i++)//����ѭ��������������ֵ��Ԫ��
	{
		if (a == Buffer[i] / 4){//����ʱ�����ϵ���һ���ƺͻ��������е�һ�������ʱ
			if (k == 2)//ִ���ϼҲ���
			{
				int j = 0;//��ʼ��һ��j����
				while (iOneDe[j] != -1)//ֻҪ�ϼұ��е�����������Ԫ�أ���һֱѭ��
				{
					j++;//���������ϼұ��е��Ƶ�����������λ���Ƶ�û��ֵ�õط�ȥ���������ֱ�Ӵ���Ԫ��
				}
				iOneDe[j] = Buffer[i];//ѭ�����˾͵��˿յ� ���Դ���Ԫ�ص�λ����
				//Ȼ��ͽ������Ѿ�ȷ�����Ʒ����ϼұ��е������鵱��
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];//�������Ѿ����˵��ƴӻ���������ɾ����Ԫ��������ǰ��
				}
				i--;//�������鳤�ȼ�1
			}
			if (k == 1)//����һ�£�ֻ�ǲ������鲻ͬ���˴������¼ұ��е�������
			{
				int j = 0;
				while (iTwoDe[j] != -1)
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
			}
		}
	}
}
void PokerModelAnalysis::Type2(int *iCs, int k)//ը��
{
	int a = iCs[0] / 4;//ͬ���жϴ�ʱ�����Ƶ�����ֵ
	for (int i = 0; Buffer[i] != -1; i++)//����ѭ�����������е�Ԫ��
	{
		if (a == Buffer[i] / 4)//����ʱ�����ϵ��ƺͻ��������е�����ͬ
		{
			if (k == 2)
			{
				int j = 0;
				while (iOneDe[j] != -1)
				{
					j++;//����������ȡλ��
				}
				iOneDe[j] = Buffer[i];//�����������Ļ��������е�ֵ�����ϼұ��е���
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];//Ȼ�󽫸���ֵ��Ԫ�شӻ���������ɾ��������Ԫ��������ǰ��
				}
				i--;//���鳤�ȼ�һ
			}
			if (k == 1)//�¼Ҳ���
			{
				int j = 0;
				while (iTwoDe[j] != -1)
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
			}
		}
	}
}
void PokerModelAnalysis::Type3(int *iCs, int k)//����
{
	     int a = iCs[0] / 4;
	for (int i = 0; Buffer[i] != -1; i++)//ѭ���Ǹ�buffer���飬ֻҪ��ֵ��һֱѭ����ÿһ��Ԫ�ض�����һ��
	{
		if (a == Buffer[i] / 4)//��֮ǰ�жϵ��ƺͻ���������������Դ˱Ƚ�
		{
			if (k == 2)//����2��ʱ�����ϼ�
			{
				int j = 0;
				while (iOneDe[j] != -1)
				{
					j++;//�ҵ����Է��Ƶ�λ��
				}
				iOneDe[j] = Buffer[i];//��Ϊ��ʱ������ͬ�����Խ����������е��Ʒ����ϼұ��е���������
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];//Ԫ����ǰ��
				}
				i--;//����ȡ����������ɾ������Ȼ�����鳤�Ⱦͼ�1
				break;
			}
			if (k == 1)//����1��ʱ�����¼�
			{
				int j = 0;
				while (iTwoDe[j] != -1)//ֻҪ��Ϊ��1����һֱ�����������
				{
					j++;//�����¼ұ��е��Ƶ�����
				}
				iTwoDe[j] = Buffer[i];//�����������ֵ�����¼ұ��е��ƣ�����Ϊ��һ���Ѿ��ж�����ˣ����Էŵ��¼ұ��е���������
				for (int x = i; Buffer[x] != -1; x++)//����ͬ�����һ��
				{
					Buffer[x] = Buffer[x + 1];//������ǰ��
				}
				i--;//���鳤�ȼ�1
				break;
			}
		}
	}
	Random(Buffer);//��Ϊ�˰��������
}
void PokerModelAnalysis::Type4(int *iCs, int k){//���Ƶ����
	
		int a = iCs[0] / 4;
		int count = 0;
		for (int i = 0; Buffer[i] != -1; i++)
		{
			if (a == Buffer[i] / 4)
			{
				if (k == 2)
				{
					int j = 0;
					while (iOneDe[j] != -1)
					{
						j++;
					}
					iOneDe[j] = Buffer[i];
					for (int x = i; Buffer[x] != -1; x++)
					{
						Buffer[x] = Buffer[x + 1];
					}
					i--;
					count++;
				    if(count==2) break;					
				}
				if (k == 1)
				{
					int j = 0;
					while (iTwoDe[j] != -1)
					{
						j++;
					}
					iTwoDe[j] = Buffer[i];
					for (int x = i; Buffer[x] != -1; x++)
					{
						Buffer[x] = Buffer[x + 1];
					}
					i--;
					count++;
					if (count == 2) break;
				}
			}
		}
}
void PokerModelAnalysis::Type5(int *iCs, int k)//����
{
	int a = iCs[0] / 4;
	int count = 0;
	for (int i = 0; Buffer[i] != -1; i++)
	{
		if (a == Buffer[i] / 4)
		{
			if (k == 2)
			{
				int j = 0;
				while (iOneDe[j] != -1)
				{
					j++;
				}
				iOneDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
				count++;
				if (count == 3) break;
			}
			if (k == 1)
			{
				int j = 0;
				while (iTwoDe[j] != -1)
				{
					j++;
				}
				iTwoDe[j] = Buffer[i];
				for (int x = i; Buffer[x] != -1; x++)
				{
					Buffer[x] = Buffer[x + 1];
				}
				i--;
				count++;
				if (count == 3) break;
			}
		}
	}
}
void PokerModelAnalysis::Type6_11(int *iCs, int k)
{
	for (int i = 0;iCs[i] != -1;i++)
	{
		int a = iCs[i] / 4;
		for (int i = 0; Buffer[i] != -1; i++)
		{
			if (a == Buffer[i] / 4)
			{
				if (k == 2)
				{
					int j = 0;
					while (iOneDe[j] != -1)
					{
						j++;
					}
					iOneDe[j] = Buffer[i];
					for (int x = i; Buffer[x] != -1; x++)
					{
						Buffer[x] = Buffer[x + 1];
					}
					i--;
					break;
				}
				if (k == 1)
				{
					int j = 0;
					while (iTwoDe[j] != -1)
					{
						j++;
					}
					iTwoDe[j] = Buffer[i];
					for (int x = i; Buffer[x] != -1; x++)
					{
						Buffer[x] = Buffer[x + 1];
					}
					i--;
					break;
				}
			}
		}
	}
}

void PokerModelAnalysis::CardsValue(int *pCards)
{
	InitialSum = 0;//��ʼ����Ϊ0
	ConfirmCards(pCards);//����ȷ���ƺ���

	if (iUtimate[0][0] != -1)//��һ��λ������
	{
		InitialSum = InitialSum + 10;//��������ֵ��������10��
	}
	for (int i = 0; iUtimate[1][i] != -1; i++)//��û����ը�� ��ը����������5��ǰ���ĸ��ը����
	{
		if (iUtimate[1][i] == -2)//���ڣ�2��ʲô״̬��û��ը��
		{
			i++;
			continue;
		}
		else
		{
			InitialSum = InitialSum + 9;//��������ֵ��ը����9��
			i = i + 4;//�����ƣ���ը�����ĸ�
		}
	}
	for (int i = 0; iUtimate[2][i] != -1; i = i + 4)//��û������ ������������4�񣬾���������ͬ����
	{
		InitialSum = InitialSum + 8;//��������ֵ��ÿѭ��һ�ξ�֤����һ����������һ����8��
	}
	for (int i = 0; iUtimate[3][i] != -1; i++)//�ɻ�
	{
		int j;
		for (j = 0; iUtimate[3][j] != -2; j = j + 3)//���forѭ�����������û�����ݣ��ǲ���Ӧ�ý�����ļ�������ֵ�����Ž�ȥ
		{
		}
		InitialSum = InitialSum + 7;//����jֵ����ֵ�����������ֵ
		i = i + j;//��i������һ����˳������
	}
	for (int i = 0; iUtimate[4][i] != -1; i++)//��û��˳�� ��˳�Ӿͽ���ѭ��
	{
		int j;
		for (j = 0; iUtimate[4][j] != -2; j++)//��˳�� ��Jֵ�����˳�ӵĳ��ȣ�����5����
		{
			//�������Ӧ��дʲô��
		}
		InitialSum = InitialSum + 6;//����jֵ����ֵ�����������ֵ
		i = i + j;//ʹi������һ��˳�ӵ�����
	}
	for (int i = 0; iUtimate[5][i] != -1; i++)
	{
		int j;
		for (j = 0; iUtimate[5][j] != -2; j = j + 2)//�ж�����
		{
		}
		InitialSum = InitialSum + 5;//����jֵ����ֵ�����������ֵ
		i = i + j;//ʹi������һ�����Ե�����
	}
	for (int i = 0; iUtimate[6][i] != -1; i = i + 3)//��û�ж��� �ж��Ӿ�������3��
	{
		InitialSum = InitialSum + 4;//��������ֵ
	}
	for (int i = 0; iUtimate[7][i] != -1; i = i + 2)//��û�е��� �е��ƾ�������2��
	{
		InitialSum = InitialSum + 3;//��������ֵ
	}
}

void PokerModelAnalysis::LordBest()//������������õ���
{
	//ConfirmCards(pMyDdz->iOnOtherHand);
	//Undone
	int k = 0;
	if (k = 0)//k��0���¼ҵ�������
	{
		for (int i = 0; i < 37; i++)//���������ϵ��ƣ�ʣ37��
		{
			if (i < 17)//Ȼ��ǰ���17��
			{
				iOneUtimate[i] = pMyDdz->iOnOtherHand[i];//���������ϵ��Ƶ�ǰ��17�Ŷ������ϼҵ���
			}
			else
			{
				iTwoUtimate[k] = pMyDdz->iOnOtherHand[i];// ʣ�µ�20�Ŷ��������¼�
			}
		}
	}
	else//k������0��ʱ��

	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 20)
			{
				iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
			}
			else
			{
				iTwoUtimate[i] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
}

void PokerModelAnalysis::PartnerBest()//�����ѷ�����õ���
{
	int k = 0;
	if (k = 0)
	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 20)
			{
				iTwoUtimate[i] = pMyDdz->iOnOtherHand[i];
			}
			else
			{
				iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
	else
	{
		for (int i = 0; i < 37; i++)
		{
			if (i < 17)
			{
				iOneUtimate[i] = pMyDdz->iOnOtherHand[i];
			}
			else
			{
				iTwoUtimate[k] = pMyDdz->iOnOtherHand[i];
				k++;
			}
		}
	}
}

void PokerModelAnalysis::Equality()//�����ҷ��������
{
	int k = 0;
	int j = 0;
	for (int i = 0; i < 37; i++)
	{
		if (i % 2 == 0 && k < 17)
		{
			iOneUtimate[k] = pMyDdz->iOnOtherHand[i];
			k++;
		}
		else
		{
			iTwoUtimate[j] = pMyDdz->iOnOtherHand[i];
			j++;
		}
	}
}

void PokerModelAnalysis::SaveCards(int j, int *pCards)
{
	int *abc = A->startSplitCard(pCards, j);
	int i;
	for (i = 0; abc[i] != -1; i++)
	{
		iUtimate[j][i] = abc[i];
	}
	if (abc[0] != -1)
	{
		DeleteCards(abc);
	}
}

void PokerModelAnalysis::DeleteCards(int *iResult)
{
	int k = 0;
	for (int i = 0; iResult[i] != -1; i++)
	{
		if (iResult[i] != -2)
		{
			for (k = 0; k < 21; k++)
			{
				if (iResult[i] == Hc[k])
				{
					for (; k < 20; k++)
					{
						Hc[k] = Hc[k + 1];
					}
					break;
				}
			}
		}
	}
}

void PokerModelAnalysis::ConfirmCards(int *pCards)
{
	SaveCards(0, pCards);
	SaveCards(1, pCards);
	SaveCards(2, pCards);
	SaveCards(3, pCards);
	SaveCards(4, pCards);
	SaveCards(5, pCards);
	SaveCards(6, pCards);
	SaveCards(62, pCards);
	SaveCards(7, pCards);
	SaveCards(8, pCards);
	SaveCards(9, pCards);
	SaveCards(10,pCards);
	SaveCards(102, pCards);
	SaveCards(11,pCards);
	SaveCards(1102, pCards);
}

void PokerModelAnalysis::DeleteB(int *iCs, int a)
{
	if (a == 2)//Ҳ���ж���ȣ�Ȼ��Ԫ��ǰ�ƣ�ͬ�����ط�һ��������ʲô��
	{
		for (int i = 0; iTwoDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iTwoDe[i] == iCs[j])
				{
					for (int k = i; iTwoDe[k] != -1; k++)
					{
						iTwoDe[k] = iTwoDe[k + 1];
					}
					i--;
					j = 0;
				}
			}
		}
		for (int i = 0; iOneDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iOneDe[i] == iCs[j])
				{
					for (int k = i; iOneDe[k] != -1; k++)
					{
						iOneDe[k] = iOneDe[k + 1];
					}
					UpWrong++;
					i--;
					j = 0;
				}
			}
		}
	}
	if (a == 1)
	{
		for (int i = 0; iTwoDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iTwoDe[i] == iCs[j])
				{
					for (int k = i; iTwoDe[k] != -1; k++)
					{
						iTwoDe[k] = iTwoDe[k + 1];
					}
					DownWrong++;
					i--;
					j = 0;
				}
			}
		}
		for (int i = 0; iOneDe[i] != -1; i++)
		{
			for (int j = 0; iCs[j] != -1; j++)
			{
				if (iOneDe[i] == iCs[j])
				{
					for (int k = i; iOneDe[k] != -1; k++)
					{
						iOneDe[k] = iOneDe[k + 1];
					}
					i--;
					j = 0;
				}
			}
		}	
	}

}

void PokerModelAnalysis::Fendipai()
{
	if (pMyDdz->cLandlord + 1 == pMyDdz->cDir)////�������ҷ��ϼң����ǵ����ķ�λ��˳ʱ���ƶ�һ�������������ڵķ�λ�������ҵ��ϼҾ��ǵ���
	{
		for (int i = 0; i < 3; i++)
		{
			iOneDe[i] = pMyDdz->iLef[i];////���ν�3�ŵ��Ʒ����ϼұ��е���������
		}
		for (int i = 0; Buffer[i] != -1; i++)////ֻҪ�����������ֵ��Ϊ��1����һֱѭ������ά�����ѭ��,���ҵ���1��λ�ú����������ŵ���
		{
			for (int j = 0; j < 3; j++)////�˴���ѭ�����ŵ���
			{
				if (Buffer[i] == pMyDdz->iLef[j])////�����������е�Ԫ�ص��ڻ����������Ԫ��ʱ
				{
					for (int k = i; Buffer[k] != -1; k++)
					{
						Buffer[k] = Buffer[k + 1];////��ǰ��
					}
					i--;
				}
			}
		}
	}
	if (pMyDdz->cLandlord - 1 == pMyDdz->cDir)////ͬ�����ҵ��¼��ǵ��������
	{
		for (int i = 0; i < 3; i++)////���ν�3�ŵ��Ʒ����¼ұ��е���������
		{
			iTwoDe[i] = pMyDdz->iLef[i];
		}
		for (int i = 0; Buffer[i] != -1; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				if (Buffer[i] == pMyDdz->iLef[j])
				{
					for (int k = i; Buffer[k] != -1; k++)
					{
						Buffer[k] = Buffer[k + 1];
					}
					i--;
				}
			}
		}
	}
}

void PokerModelAnalysis::DeleteC()
{
	for (int i = 0; iOneDe[i] != -1; i++)
	{
		for (int j = 0; Buffer[j] != -1; j++)
		{
			if (Buffer[j] == iOneDe[i])//����ѭ������ϼұ��е��ƺͻ������������ʱ���ͽ���������Ԫ��������ǰ��
			{
				for (int k = j; Buffer[k] != -1; k++)
				{
					Buffer[k] = Buffer[k + 1];
				}
				i--;
				j = 0;
			}
		}
	}
	for (int i = 0; iTwoDe[i] != -1; i++)//����ͬ�ϣ��˴�������¼Ҷ��Ե�
	{
		for (int j = 0; Buffer[j] != -1; j++)
		{
			if (Buffer[j] == iTwoDe[i])
			{
				for (int k = j; Buffer[k] != -1; k++)
				{
					Buffer[k] = Buffer[k + 1];
				}
				i--;
				j = 0;
			}
		}
	}
}


/****************************
*���ߣ� ף���  ʱ�䣺2017-7-16*
*���ܣ� ��ʼ�����ʱ�           *
*****************************/
void PokerModelAnalysis::Gailvbiao(int *Out,int p)/*���ʱ�ĳ�ʼ��*/
{
	/*��15���ƣ�3,4,5,6,7,8,9,10,J,Q,K,A,2,С��,��������0,1,2,3,4���������*/

	int One[15] = { 0 };/*�洢��ȥ���ƺ�AI���ƺ����������������One[0]��һ����0�������Զ���0*/
	int Dipai[15] = { 0 };/*װ���ŵ���,�±��ǵ��ƣ�ֵΪ���Ƶ�����*/
	int n; /*������n=����+one*/

		   /*����������ÿ���Ʒֱ������*/
	for (int i1 = 17; i1 < 51; i1++)
	{
		/*��Ϊ����С������4��ֵ��ͬ�����Էֿ�����*/
		if (Out[i1] == 52)
		{
			One[13] += 1;
		}
		if (Out[i1] == 53)
		{
			One[14] += 1;
		}
		if (Out[i1] != 52 && Out[i1] != 53)
		{
			int j1;
			j1 = Out[i1] / 4;
			One[j1]++;
		}
	}

	/*������Ʒֱ������*/
	for (int j2 = 51; j2<54; j2++)
	{
		if (Out[j2] == 52)
		{
			Dipai[13]++;
		}
		if (Out[j2] == 53)
		{
			Dipai[14]++;
		}
		else if (Out[j2] != 52 && Out[j2] != 53)
		{
			int i2 = Out[j2] / 4;
			Dipai[i2]++;
		}
	}

	if (p == 1) /*�������ϼ�*/
	{
		/*���ʱ��ʼ��*/
		for (int i3 = 0; i3 < 15; i3++)
		{
			n = One[i3] + Dipai[i3];
			int d = Dipai[i3];
			for (int j3 = 0; j3 < 5; j3++)
			{
				if (One[i3] == 0) /*������Ϊ0*/
				{
					if (d == 0)/*���ƺ������ƾ�Ϊ0����AB���ʱ��д�������Ϊ0�ĸ�����Ϊ1����������Ϊ0*/
					{
						if (j3 == 0)
						{
							BiaoA[i3][j3] = 1;
							BiaoB[i3][j3] = 1;
						}
						else if (j3 != 0)
						{
							BiaoA[i3][j3] = 0;
							BiaoB[i3][j3] = 0;
						}
					}
					else if (d != 0) /*������Ϊ0�����Ʋ�Ϊ0���򽫱�A���е��������ĸ�����Ϊ1��������������B��Ϊ0*/
					{
						if (j3 == 0) {
							BiaoA[i3][j3] = 0;
							BiaoB[i3][j3] = 1;
						}
						if (j3 == d)
						{
							BiaoA[i3][j3] = 1;
							BiaoB[i3][j3] = 0;
						}
						else {
							BiaoA[i3][j3] = 0;
							BiaoB[i3][j3] = 0;
						}
					}
				}
				else if (One[i3] != 0)/*�����Ʋ�Ϊ0*/
				{
					if (d == 0)/*�����Ʋ�Ϊ0������Ϊ0�����ABС�ڴ�����������������������������Ϊ0.5��������Ϊ0*/
					{
						if (j3 <= One[i3])
						{
							BiaoA[i3][j3] = 0.5;
							BiaoB[i3][j3] = 0.5;
						}
						else
						{
							BiaoA[i3][j3] = 0;
							BiaoB[i3][j3] = 0;
						}
					}
					else if (d != 0) /*�����Ʋ�Ϊ0�ҵ��Ʋ�Ϊ0*/
					{
						if (j3 <= n)/*�ڴ�����³�ʼ����A�ĸ���*/
						{
							if (j3 == d) /*�ѵ��������ĸ�����Ϊ0.85*/
							{
								BiaoA[i3][j3] = 0.85;
							}
							if (j3 < d)/*��С�ڵ��������ĸ�����Ϊ0*/
							{
								BiaoA[i3][j3] = 0;
							}
							if (j3 > d) /*�Ѵ��ڵ���������С���������ĸ�����Ϊ0.5*/
							{
								BiaoA[i3][j3] = 0.5;
							}
						}
						if (j3 <= One[i3])/*�ڴ�����³�ʼ����B�ĸ��ʱ�*/
						{
							BiaoB[i3][j3] = 0.5;/*�ڱ�B�аѴ���С�ڵ��������Ƶĸ�����Ϊ0.5*/
						}
						if (j3>One[i3])
						{
							BiaoB[i3][j3] = 0;/*�ڱ�B�аѴ��д��������Ƶĸ�����Ϊ0*/
						}
					}
				}
			}
		}
	}

	if (p == 2)/*������AI*/
	{
		for (int i4 = 0; i4<15; i4++)
		{
			for (int j4 = 0; j4 < 5; j4++)
			{
				if (One[i4] == 0)
				{
					if (j4 == 0)
					{
						BiaoA[i4][j4] = 1;
						BiaoB[i4][j4] = 1;
					}
					else
					{
						BiaoA[i4][j4] = 0;
						BiaoB[i4][j4] = 0;
					}
				}
				else
					if (j4 <= One[i4])
					{
						BiaoA[i4][j4] = 0.5;
						BiaoB[i4][j4] = 0.5;
					}
					else
					{
						BiaoA[i4][j4] = 0;
						BiaoB[i4][j4] = 0;
					}
			}
		}
	}

	if (p == 3)/*�������¼�*/
	{
		for (int i5 = 0; i5 < 15; i5++)
		{
			n = One[i5] + Dipai[i5];
			int d = Dipai[i5];
			for (int j5 = 0; j5 < 5; j5++)
			{
				if (One[i5] == 0) /*������Ϊ0*/
				{
					if (d == 0)/*���ƺ������ƾ�Ϊ0����AB���ʱ��д�������Ϊ0�ĸ�����Ϊ1����������Ϊ0*/
					{
						if (j5 == 0)
						{
							BiaoA[i5][j5] = 1;
							BiaoB[i5][j5] = 1;
						}
						else if (j5 != 0)
						{
							BiaoA[i5][j5] = 0;
							BiaoB[i5][j5] = 0;
						}
					}
					else if (d != 0) /*������Ϊ0�����Ʋ�Ϊ0���򽫱�B���е��������ĸ�����Ϊ1��������������A��Ϊ0*/
					{
						if (j5 == 0)
						{
							BiaoA[i5][j5] = 1;
							BiaoB[i5][j5] = 0;
						}
						if (j5 == d)
						{
							BiaoA[i5][j5] = 0;
							BiaoB[i5][j5] = 1;
						}
						else {
							BiaoA[i5][j5] = 0;
							BiaoB[i5][j5] = 0;
						}
					}
				}
				else if (One[i5] != 0)/*�����Ʋ�Ϊ0*/
				{
					if (d == 0)/*�����Ʋ�Ϊ0������Ϊ0�����ABС�ڴ�����������������������������Ϊ0.5��������Ϊ0*/
					{
						if (j5 <= One[i5])
						{
							BiaoA[i5][j5] = 0.5;
							BiaoB[i5][j5] = 0.5;
						}
						else
						{
							BiaoA[i5][j5] = 0;
							BiaoB[i5][j5] = 0;
						}
					}
					else if (d != 0) /*�����Ʋ�Ϊ0�ҵ��Ʋ�Ϊ0*/
					{
						if (j5 <= n)/*�ڴ�����³�ʼ����A�ĸ���*/
						{
							if (j5 == d) /*�ѵ��������ĸ�����Ϊ0.85*/
							{
								BiaoB[i5][j5] = 0.85;
							}
							if (j5< d)/*��С�ڵ��������ĸ�����Ϊ0*/
							{
								BiaoB[i5][j5] = 0;
							}
							if (j5 > d) /*�Ѵ��ڵ���������С���������ĸ�����Ϊ0.5*/
							{
								BiaoB[i5][j5] = 0.5;
							}
						}
						if (j5 <= One[i5])/*�ڴ�����³�ʼ����B�ĸ��ʱ�*/
						{
							BiaoA[i5][j5] = 0.5;/*�ڱ�B�аѴ���С�ڵ��������Ƶĸ�����Ϊ0.5*/
						}
						if (j5>One[i5])
						{
							BiaoA[i5][j5] = 0;/*�ڱ�B�аѴ��д��������Ƶĸ�����Ϊ0*/
						}
					}
				}
			}
		}
	}

//Test
	/*int Card[16] = {3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
	
	cout << "�ϼҵĸ��ʱ�" << endl;
	cout << "����\t" << 0 << "\t" << 1 << "\t" << 2 << "\t" << 3 << "\t" << 4 << "\t"<<endl;
   for (int i6 = 0; i6 < 15;i6++) {
	   if (Card[i6] == 14)cout << "A" << "\t";
	   else if(Card[i6] == 15)cout << "2" << "\t";
	   else if (Card[i6] == 16)cout << "С��" << "\t";
	   else if (Card[i6] == 11)cout << "J" << "\t";
	   else if (Card[i6] == 12)cout << "Q" << "\t";
	   else if (Card[i6] == 13)cout << "K" << "\t";
	   else if (Card[i6] == 14)cout << "A" << "\t";
	   else if (Card[i6] == 17)cout << "����" << "\t";
	   else cout << Card[i6] << "\t";
		cout << BiaoA[i6][0] <<"\t"<< BiaoA[i6][1] << "\t" << BiaoA[i6][2] << "\t" << BiaoA[i6][3] << "\t" << BiaoA[i6][4] << endl;
	}
    cout <<endl<< "�¼ҵĸ��ʱ�" << endl;
	cout << "����\t" << 0 << "\t" << 1 << "\t" << 2 << "\t" << 3 << "\t" << 4 << "\t" << endl;
    for (int j6 = 0; j6 < 15;j6++) 
	{
		if (Card[j6] == 14)cout << "A" << "\t";
		else if (Card[j6] == 15)cout << "2" << "\t";
		else if (Card[j6] == 11)cout << "J" << "\t";
		else if (Card[j6] == 12)cout << "Q" << "\t";
		else if (Card[j6] == 13)cout << "K" << "\t";
		else if (Card[j6] == 14)cout << "A" << "\t";
		else if (Card[j6] == 16)cout << "С��" << "\t";
		else if (Card[j6] == 17)cout << "����" << "\t";
		else cout << Card[j6] << "\t";
		cout << BiaoB[j6][0] << "\t" << BiaoB[j6][1] << "\t" << BiaoB[j6][2] << "\t" << BiaoB[j6][3] << "\t" << BiaoB[j6][4] << endl;
	}*/
}

/****************************
*���ߣ� ף���  ʱ�䣺2017-7-16*
*���ܣ� ���ݳ��Ƹ��¸��ʱ�      *
*****************************/
void PokerModelAnalysis::Change(int *Count, int t)/*���ʱ�ı仯*/
{
	int i, j;
	int a[15] = { 0 };/*�洢���ֳ��Ƶ�����*/
	float g = 0.2;/*���ʵı仯ֵ*/

	for (int i1 = 0; Count[i1] != -1; i1++)
	{
		if (Count[i1] == 52)
		{
			a[13]++;
		}
		if (Count[i1] == 53)
		{
			a[14]++;
		}
		if (Count[i1] != 52 && Count[i1] != 53)
		{
			int j1 = Count[i1] / 4;
			a[j1]++;
		}
	}

	if (t == 1) /*�ϼҳ��Ƹ��ʱ�ı仯*/
	{
		for (int i2 = 0; i2 < 15; i2++)
		{
			if (a[i2] != 0)/*���δpass��������������0*/
			{
				for (int j2 = 0; (BiaoA[i2][j2] != 0 || BiaoB[i2][j2] != 0) && j2<5; j2++)/*�ֱ��ҵ����ʱ�AB�����һ����ֵ������*/
				{
					if (BiaoA[i2][j2] != 0)
					{
						i = j2;/*�ҳ����ʱ�A�д��е��������ֵ*/
					}
					if (BiaoB[i2][j2] != 0)
					{
						j = j2;/*�ҳ����ʱ�B�д��е��������ֵ*/
					}
				}

				if (i == a[i2]) /*���������ڸ��ʱ�A���������*/
				{
					if (i >= j)/*���������ڸ��ʱ�A�����������A�������������B�����������AB��������0�ĸ��ʸ�Ϊ1�����������޸�Ϊ0*/
					{
						for (; i >= 0; i--)
						{
							if (i == 0)
							{
								BiaoA[i2][i] = 1;
								BiaoB[i2][i] = 1;
							}
							else
							{
								BiaoA[i2][i] = 0;
								BiaoB[i2][i] = 0;
							}
						}
					}
					if (i < j)/*���������ڸ��ʱ�A�����������A���������С��B�����������A�������и����޸�Ϊ0��B��j-i������Ϊ1,������Ϊ0*/
					{
						int c = j - i;
						for (; j >= 0; j--)
						{
							if (j <= i)
							{
								BiaoA[i2][i] == 0;
							}
							if (j == c)
							{
								BiaoB[i2][c] = 1;
							}
							else
							{
								BiaoB[i2][c] = 0;
							}
						}
					}
				}

				if (a[i2] < i)/*A�ĳ�����С�ڸ��ʱ�A���������*/
				{
					if (i > j)/*A�ĳ�����С�ڸ��ʱ�A�����������A�������������B���������*/
					{
						int r = 0;
						for (int l = 0; l < 5; l++)
						{
							if (BiaoA[i2][l] > 0.8&&BiaoA[i2][l]<0.9)
								r = l;
						}
						/* if (r==0) //�������û�е�����A�ĳ�����С�ڸ��ʱ�A�����������A�������������B���������
						{
						��Ϊ�����ܳ������������������
						}*/
						if (BiaoA[i2][r] >0.8&&BiaoA[i2][r]<0.9&&r == a[i2])/*�����е����ҳ�����ǡ�õ��ڵ�����������ʣ�µ����н��͸��ʱ�A�ĸ������Ӹ��ʱ�B�ĸ���*/
						{
							int i3 = i - a[i2];
							for (int j3 = i; j3 >= 0; j3--)
							{
								if (j3 == 0)
								{
									BiaoA[i2][i3] += g;
									BiaoB[i2][i3] -= g;
								}
								if (j3 > 0 && j3 <= i3) {
									BiaoA[i2][i3] -= 2 * g;/*��2g����Ϊ���ܻ���if (j3 == a[i2])���������ʱҪȷ��Aʣ�������ĸ���ҪС��Bʣ�������ĸ���*/
									BiaoB[i2][i3] += g;
								}
								if (j3 > i3)
								{
									BiaoA[i2][j3] = 0;
									BiaoA[i2][j3] = 0;
								}
							}
						}

						if (BiaoA[i2][r] > 0.8&&BiaoA[i2][r]<0.9&& a[i2] < r)/*�����е����ҳ�����С�ڵ���*/
						{
							int i3 = i - a[i2];/*ʣ�µ�A�����ֵ*/
							int q = r - a[i2];/*ʣ�µĵ���*/
											  /*int p = i - a[i2] - q;//ʣ�µ�B�����ֵ,���������²��Ը��ʱ�B���޸�*/
							for (int j3 = i3; j3 > 0; j3--)
							{
								if (j3 == q)
								{
									BiaoA[i2][j3] = 0.85;
								}
								else {
									BiaoA[i2][j3] -= 2 * g;
								}
							}
							for (int j3 = i3 + 1; j3 < 5; j3++)
							{
								BiaoA[i2][j3] = 0;
							}
						}
						if (BiaoA[i2][r] >0.8&&BiaoA[i2][r] <0.9&& a[i2] > r) /*�����е����ҳ��������ڵ���*/
						{
							int i3 = i - a[i2];
							for (int j3 = j; j3 < 5; j3++)
							{
								if (j3 == 0)
								{
									BiaoA[i2][j3] += g;
									BiaoB[i2][j3] -= g;
								}
								if (j3 > i3)
								{
									BiaoA[i2][j3] = 0;
									BiaoB[i2][j3] = 0;
								}
								if (j3 > 0 && j3 <= i3)
								{
									BiaoA[i2][j3] -= 2 * g;
									BiaoB[i2][j3] += g;
								}
							}
						}
					}

					else	if (i == j) /*A�ĳ�����С�ڸ��ʱ�A�����������A�������������B���������*/
					{
						for (int i3 = 0; i3 < 5; i3++)
						{
							if (i3 == 0)
							{
								BiaoA[i2][i3] += g;
								BiaoB[i2][i3] -= g;
							}
							else if (i3 > 0 && i3 <= i - a[i2])
							{
								BiaoA[i2][i3] -= g;
								BiaoB[i2][i3] += g;
							}
							else if (i3 > i - a[i2])
							{
								BiaoA[i2][i3] = 0;
								BiaoB[i2][i3] = 0;
							}
						}
					}

					else	if (i < j)/*A�ĳ�����С�ڸ��ʱ�A�����������A���������С��B���������*/
					{
						int r = 0;
						for (int l = 0; l < 5; l++)
						{
							if (BiaoB[i2][l] >0.8&&BiaoB[i2][l]<0.9)
								r = l;
						}
						/*	if (r==0) {}//�����ܳ��ִ���������Բ�����*/
						if (r != 0)
						{
							if (j - a[i2] == r)/*��ȥA�ĳ�������ǡ����Bӵ�еĵ�����������B�ĵ���������Ϊ1������AB���ʶ���Ϊ0*/
							{
								for (int i4 = 0; i4 < 5; i4++)
								{
									if (i4 == 0)
									{
										BiaoA[i2][i4] = 1;
									}
									else
									{
										BiaoA[i2][i4] = 0;
									}
									if (i4 == r)
									{
										BiaoB[i2][i4] = 1;
									}
									else {
										BiaoB[i2][i4] = 0;
									}
								}
							}
							if (j - a[i2] > r)
							{
								int i5 = i - a[i2];/*A���ƺ�Aʣ�µ���������Ϊj-a[i2]>r����i5������0*/
								int q = j - a[i2];/*A���ƺ�Bʣ�µ�����*/
								for (int j5 = 0; j5 < 5; j5++)
								{
									if (j5 == 0)
									{
										BiaoA[i2][j5] += g;
										BiaoB[i2][j5] -= g;
									}
									if (j5 <= q&&j5 >0)
									{
										BiaoB[i2][j5] += g;
										if (j5 <= i5)
										{
											BiaoA[i2][j5] -= g;
										}
										if (j5>i5)
										{
											BiaoA[i2][j5] = 0;
										}
									}
									if (j5>q)
									{
										BiaoA[i2][j5] = 0;
										BiaoB[i2][j5] = 0;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	if (t == 2) /*�¼ҳ��Ƹ��ʱ�ı仯,ͬ�ϼҳ������*/
	{
		for (int i2 = 0; i2 < 15; i2++)
		{
			if (a[i2] != 0)/*���δpass��������������0*/
			{
				for (int j2 = 0; (BiaoA[i2][j2] != 0 || BiaoB[i2][j2] != 0) && j2<5; j2++)/*�ֱ��ҵ����ʱ�AB�����һ����ֵ������*/
				{
					if (BiaoB[i2][j2] != 0)
					{
						j = j2;/*�ҳ����ʱ�B�д��е��������ֵ*/
					}
					if (BiaoA[i2][j2] != 0)
					{
						i = j2;/*�ҳ����ʱ�A�д��е��������ֵ*/
					}
				}

				if (j == a[i2]) /*���������ڸ��ʱ�B���������*/
				{
					if (j >= i)/*���������ڸ��ʱ�B�����������B�������������A�����������AB��������0�ĸ��ʸ�Ϊ1�����������޸�Ϊ0*/
					{
						for (int j3 = j; j3 >= 0; j3--)
						{
							if (j3 == 0)
							{
								BiaoA[i2][j3] = 1;
								BiaoB[i2][j3] = 1;
							}
							else
							{
								BiaoA[i2][j3] = 0;
								BiaoB[i2][j3] = 0;
							}
						}
					}
					if (j < i)/*���������ڸ��ʱ�B�����������B���������С��A�����������B�������и����޸�Ϊ0��A��j-i������Ϊ1,������Ϊ0*/
					{
						int c = i - j;
						for (int j3 = i; j3 >= 0; j3--)
						{
							if (j3 == 0)
							{
								BiaoB[i2][j3] = 1;
							}
							else
							{
								BiaoB[i2][j3] = 0;
							}
							if (j3 == c)
							{
								BiaoA[i2][j3] = 1;
							}
							else
							{
								BiaoA[i2][j3] = 0;
							}
						}
					}
				}


				if (a[i2] < j)/*B�ĳ�����С�ڸ��ʱ�B���������*/
				{

					if (i == j) /*B�ĳ�����С�ڸ��ʱ�B�����������B�������������A���������*/
					{
						for (int j3 = 0; j3 <5; j3++)
						{
							if (j3 == 0)
							{
								BiaoB[i2][j3] += g;
								BiaoA[i2][j3] -= g;
							}
							else  if (j3>0 && j3 <= j - a[i2])
							{
								BiaoA[i2][j3] += g;
								BiaoB[i2][j3] -= g;
							}
							else  if (j3> j - a[i2])
							{
								BiaoA[i2][j3] = 0;
								BiaoB[i2][j3] = 0;
							}
						}
					}

					if (j> i)/*B�ĳ�����С�ڸ��ʱ�B�����������B�������������A���������*/
					{
						int r = 0;

						for (int l = 0; l<5; l++)
						{
							if (BiaoB[i2][l] <0.9&&BiaoB[i2][l] >0.8)
							{
								r = l;
							}
						}
						/* if (r==0) //�������û�е�����B�ĳ�����С�ڸ��ʱ�B�����������B�������������A���������
						��Ϊ�����ܳ������������������*/
						if (BiaoB[i2][r] >0.8&&BiaoB[i2][r] <0.9&&r == a[i2])/*�����е����ҳ�����ǡ�õ��ڵ�����������ʣ�µ����н��͸��ʱ�B�ĸ������Ӹ��ʱ�A�ĸ���*/
						{
							int i3 = j - a[i2];
							for (int j3 = j; j3 >= 0; j3--)
							{
								if (j3 == 0)
								{
									BiaoA[i2][j3] -= g;
									BiaoB[i2][j3] += g;
								}
								if (j3>0 && j3 <= i3)
								{
									BiaoA[i2][j3] += g;
									BiaoB[i2][j3] -= 2 * g;	/*��2g����Ϊ���ܻ���if (j3 == a[i2])���������ʱҪȷ��Bʣ�������ĸ���ҪС��Aʣ�������ĸ���*/
								}
								if (j3>i3)
								{
									BiaoA[i2][j3] = 0;
									BiaoB[i2][j3] = 0;
								}
							}
						}
						if (BiaoB[i2][r] >0.8&&BiaoB[i2][r] <0.9&& a[i2] < r)/*�����е����ҳ�����С�ڵ���*/
						{
							int i3 = j - a[i2];/*ʣ�µ�B�����ֵ*/
							int q = r - a[i2];/*ʣ�µĵ���*/
											  /*int p = i - a[i2] - q;//ʣ�µ�A�����ֵ,���������²��Ը��ʱ�A���޸�*/
							for (int j3 = i3; j3 > 0; j3--)
							{
								if (j3 == q)
								{
									BiaoB[i2][j3] = 0.85;
								}
								else {
									BiaoB[i2][j3] -= 2 * g;
								}
							}
							for (int j3 = i3 + 1; j3 < 5; j3++)
							{
								BiaoB[i2][j3] = 0;
							}
						}
						if (BiaoB[i2][r] >0.8&&BiaoB[i2][r]<0.9&& a[i2] > r) /*�����е����ҳ��������ڵ���*/
						{
							int i3 = j - a[i2];
							for (int j3 = j; j3 < 5; j3++)
							{
								if (j3 == 0)
								{
									BiaoA[i2][j3] -= g;
									BiaoB[i2][j3] += g;
								}
								if (j3 > i3)
								{
									BiaoA[i2][j3] = 0;
									BiaoB[i2][j3] = 0;
								}
								if (j3>0 && j3 <= i3)
								{
									BiaoA[i2][j3] += g;
									BiaoB[i2][j3] -= 2 * g;
								}
							}
						}
					}

					if (j< i)/*B�ĳ�����С�ڸ��ʱ�B�����������B���������С��A���������*/
					{
						int r = 0;
						for (int l = 0; l < 5; l++)
						{
							if (BiaoA[i2][l] > 0.8&&BiaoA[i2][l]<0.9)
								r = l;
						}
						/*	if (r==0) {}//�����ܳ��ִ���������Բ�����*/
						if (r != 0)
						{
							if (i - a[i2] == r)/*��ȥB�ĳ�������ǡ����Aӵ�еĵ�����������A�ĵ���������Ϊ1������AB���ʶ���Ϊ0*/
							{
								for (int i4 = 0; i4 < 5; i4++)
								{
									if (i4 == 0)
									{
										BiaoB[i2][i4] = 1;
									}
									else
									{
										BiaoB[i2][i4] = 0;
									}
									if (i4 == r)
									{
										BiaoA[i2][i4] = 1;
									}
									else {
										BiaoA[i2][i4] = 0;
									}
								}
							}
							else if (i - a[i2]>r)
							{
								int i5 = j - a[i2];/*B���ƺ�Bʣ�µ���������Ϊi-a[i2]>r����i5������0*/
								int q = i - a[i2];/*B���ƺ�Aʣ�µ�����*/
								for (int j5 = 0; j5 <5; j5++)
								{
									if (j5 == 0)
									{
										BiaoB[i2][j5] += g;
										BiaoA[i2][j5] -= g;
									}
									if (j5 <= q&&j5 >0)
									{
										BiaoA[i2][j5] += g;
										if (j5 <= i5)
										{
											BiaoB[i2][j5] -= g;
										}
										if (j5>i5)
										{
											BiaoB[i2][j5] = 0;
										}
									}
									if (j5>q) {
										BiaoA[i2][j5] = 0;
										BiaoB[i2][j5] = 0;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	for (int i6 = 0; i6<15; i6++)
	{
		for (int j6 = 0; j6 < 5; j6++)
		{
			if (BiaoA[i6][j6] <= 0)
			{
				BiaoA[i6][j6] = 0;
			}
			if (BiaoA[i6][j6] >= 1)
			{
				BiaoA[i6][j6] = 1;
			}
			if (BiaoB[i6][j6] <= 0)
			{
				BiaoB[i6][j6] = 0;
			}
			if (BiaoB[i6][j6] >= 1)
			{
				BiaoB[i6][j6] = 1;
			}
		}
	}

	//Test
	/*int Card[16] = { 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17 };
	cout << "�ϼҵĸ��ʱ�" << endl;
	cout << "����\t" << 0 << "\t" << 1 << "\t" << 2 << "\t" << 3 << "\t" << 4 << "\t" << endl;
	for (int i6 = 0; i6 < 15; i6++) 
	{
			if (Card[i6] == 14)cout << "A" << "\t";
			else if (Card[i6] == 15)cout << "2" << "\t";
			else if (Card[i6] == 16)cout << "С��" << "\t";
			else if (Card[i6] == 11)cout << "J" << "\t";
			else if (Card[i6] == 12)cout << "Q" << "\t";
			else if (Card[i6] == 13)cout << "K" << "\t";
			else if (Card[i6] == 14)cout << "A" << "\t";
			else if (Card[i6] == 17)cout << "����" << "\t";
			else cout << Card[i6] << "\t";
		cout << BiaoA[i6][0] << "\t" << BiaoA[i6][1] << "\t" 
			 << BiaoA[i6][2] << "\t" << BiaoA[i6][3] << "\t" << BiaoA[i6][4] << endl;
	}
	cout <<endl<<"�¼ҵĸ��ʱ�" << endl;
	cout << "����\t" << 0 << "\t" << 1 << "\t" << 2 << "\t" << 3 << "\t" << 4 << "\t" << endl;
	for (int j6 = 0; j6 < 15; j6++) 
	{
		if (Card[j6] == 14)cout << "A" << "\t";
		else if (Card[j6] == 15)cout << "2" << "\t";
		else if (Card[j6] == 11)cout << "J" << "\t";
		else if (Card[j6] == 12)cout << "Q" << "\t";
		else if (Card[j6] == 13)cout << "K" << "\t";
		else if (Card[j6] == 14)cout << "A" << "\t";
		else if (Card[j6] == 16)cout << "С��" << "\t";
		else if (Card[j6] == 17)cout << "����" << "\t";
		else cout << Card[j6] << "\t";
		cout << BiaoB[j6][0] << "\t" << BiaoB[j6][1] << "\t" 
			 << BiaoB[j6][2] << "\t" << BiaoB[j6][3] << "\t" << BiaoB[j6][4] << endl;
	}*/
}