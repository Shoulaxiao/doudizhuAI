/* ����������*/
/*
* ����޶���: ���Ľ�  2018.7.20
* ���ļ����ܣ� ����������
*/
#include "CardsInfo.h"
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "SplitCard.h"
#include "PokerModelAnalysis.h"

//�ڵ�������
#define PlaMax 100

//�洢���Լ�������Ϣ
struct fDdz
{
	int iOnHand[21];		//�����ƣ�����ֵ��ʼ��Ϊ-1��
	int *iLastOnTable;		//���һ�η�Pass�������飬��-1��β��Pass��Ϊ-1
	int iUpPlayerCards;		//�������������
	int iDownPlayerCards;	//�������������
	char cDir;				//��ҷ�λ���
	char cLandlord;			//������ҷ�λ���
	char cLastPlay;         //��λ������ҷ�λ���
	int iPlaCount;			//���ֳ��ƿ��н�������ֵ��[0��kPlaMax-1]��
	int iPlaArr[PlaMax][21];//���ֳ��ƿ��н⼯�������ƽ����Ʊ���������-1���,-2��β��//PASS����Ϊ-3��//passΪ-2-1;
	int iLastPassCount;		//��ǰ��������PASS����ֵ��[0,2],��ֵ2����������ȡ0��һ��PASSȡ1������PASSȡ2��
	int iLastMainPoint;		//��ǰ�������Ƶ�����ֵ��[0,15],��ֵ-1��iLastPassCount=0ʱ����ֵ����=1ʱ����ԭֵ��=2ʱֵΪ-1��
	int iLastTypeCount;		//��ǰ��������������ֵ��[0,1108],��ֵ0��iLastPassCount=0ʱ����ֵ��=1ʱ����ԭֵ��=2ʱֵΪ0��//���������Լ�����
	bool haveHelpFriend;	//��¼�Ƿ�������ѣ�������ֻ��һ����ʱ���õ�����Ȩ������С���ƣ��ж϶����Ƿ������Լ�����С���ƣ��� ��ʼ��Ϊfalse
	int iOnHandNum;			//AIʣ������   //��������������һ��
};
class RecommendCards
{
public:
	RecommendCards();
	~RecommendCards();
	static RecommendCards* Instance();
	int HelpPla(struct fDdz * pDdz, bool IsSim);    //�������͵���������Ƽ����ƺ���

	int Help0Pass(fDdz * pDdz);						//�Ƽ�����Ӧ������Pass

	int Help1Rocket(struct fDdz * pDdz);			//H01 �Ƽ�����Ӧ�Ի��

	int Help2Bomb(struct fDdz * pDdz);				//H02 �Ƽ�����Ӧ��ը��

	int Help3Single(struct fDdz * pDdz);			//H03 �Ƽ�����Ӧ�Ե���

	int Help4Double(struct fDdz * pDdz);			//H04 �Ƽ�����Ӧ�Զ���

	int Help4DoubleSim(struct fDdz * pDdz);			//�޹���Ӧ�Զ���

	int Help5Three(struct fDdz * pDdz);				//H05 �Ƽ�����Ӧ������

	int Help6ThreeOne(struct fDdz * pDdz);			//H0601 �Ƽ�����Ӧ������һ��

	int Help6ThreeOneSim(struct fDdz * pDdz);		//�޹���Ӧ������һ

	int Help6ThreeDouble(struct fDdz * pDdz);		//H0602 �Ƽ�����Ӧ������һ��

	int Help6ThreeDoubleSim(struct fDdz * pDdz);	//�޹���Ӧ��������
		
	int Help7LinkSingle(struct fDdz * pDdz);		//H07 �Ƽ�����Ӧ�Ե�˳

	int Help8LinkDouble(struct fDdz * pDdz);		//H08 �Ƽ�����Ӧ��˫˳

	int Help9LinkThree(struct fDdz * pDdz);			//H09 �Ƽ�����Ӧ����˳

	int Help10LinkThreeSingle(struct fDdz * pDdz);	//H1001 �Ƽ�����Ӧ����˳����

	int Help10LinkThreeDouble(struct fDdz * pDdz);	//H1002 �Ƽ�����Ӧ����˳����

	int Help11FourSingle(struct fDdz * pDdz);		//H1101 �Ƽ�����Ӧ���Ĵ�����

	int Help11FourDouble(fDdz * pDdz);				//H1102 �Ƽ�����Ӧ���Ĵ�����

	int GetPlayerType(fDdz *pDdz);					//�õ�������ҵ�����

	CardsInfo* CInfo;								

	int iPlaCount;									//����ģ����������������Ķ��ֳ��ƿ��н�����

	SplitCard *pSplitCards;							//���������

	PokerModelAnalysis *pMoldel;

	int OtherPlayersCards(fDdz* pDdz, Ddz *Ddz);	//���������������ʣ������δ�����ã�

	int* pCardsOnHand;

	char cAI;//��¼AI��λ

	int InitPlaGenerate(fDdz* pDdz);				//������������ʼ��

	int iTmpSave[100];

	int iLastMaxPoint;//�ϼ�������Ƶ���

	
	bool IfPassTwice(fDdz *pDdz);   // �ж�ǰ�������Ƿ�PASS����

	
	int GetLegalCards(fDdz *pDdz, int *iCards, int *iSave);// ��iCards���������ܴ���ϼҳ��Ƶ����ʹ浽iplaArr����

	int GetLegalCards(fDdz *pDdz, int *iCards);
	int AddInPlaArr(fDdz *pDdz, int *iCards);    //���Ϸ����ͷŽ�iPlaArr
	
	int AnalyzeSplitedCards(int* iCards, int *iSave); // �������������������

	void AppendCards(int* from, int* to);
	int Help3SingleSim(fDdz* pDdz);


	int MinHandNum(int* iOnHand);	// ����������С����
	int iHandNum;  //�ҵ�����
	int iUpHandNum;//�ϼҵ�����
	int iDownHandNum;//�¼ҵ�����
	int playerType; //�������  1ΪAI�ǵ���,2Ϊ�¼��ǵ���,3Ϊ�ϼ��ǵ���
	bool ifNeedPass;
	Ddz *pRealDdz;
	bool isSim;		   //isSim=1�޹���Ӧ��,isSim=0�й���Ӧ��
	int iUpCards[21];  //�ϼҵ���
	int iDownCards[21];//�¼ҵ���
	bool GetModel(Ddz * prealDdz);
	bool AnalyzeCondition(fDdz* pDdz);
	void DeleteElementInCards(int* iCards, int iCardsCount, int iPoint);
	int AnalyzeSingleLink(int Length, int iLastMainPoint,int *iCards, int* iTmpShunzi);
	int CardsValue(int* iCards); //��������ֵ


	bool judgeFriend(fDdz* pDdz);
	int deleteTypeLeftSingleCount(int*iCopyCard, int*type);//�������еĵ����� �ڲ��ĳһ����ǰ��Ĳ�ֵ
	int judgeTypeAndAddInArr(fDdz *pDdz, int*iCards);//�жϲ�������У�ʣ�µ����Ƿ������ܶ൥�ƣ�������ᣬ�ж��Ƿ�Ϸ�������Ϸ�������н�����
	int iLastTwoHand[21];//�洢���������
};

