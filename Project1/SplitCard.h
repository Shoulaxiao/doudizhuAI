#ifndef _SplitCard
#define _SplitCard
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


/**
* 
*  ����޶���:������&&��С�� 2017.7.13
*  SplitCard ʵ�ֶ����͵ķ���������
*/

class SplitCard
{
public:
	SplitCard();//���������С��ϣ�
	~SplitCard();
	static SplitCard* Instance();

	int** startSplitCard(int iCards[]);//����ӿ�
	int* startSplitCard(int iCards[], int type);//����ӿ�

private:
	void initData();//��ʼ������
	void transcode();//�������ͣ��������С��ϣ�
	void judgeRepeat();//�ж�����
	void judgeOrder();//�ж�����
	void judgeMinLink(int iMinLink);//ѡȡ��С����������
	void judgeLink(int iMinLink);//�ж�˳��

	void judgeRocket();//�жϻ��
	void judgeBomb();//�ж�ը��
	void judgeSingle();//�жϵ���
	void judgeDouble();//�ж϶���
	void judgeThree();//�ж�����
	void judgeLinkSingle();//�жϵ�˳���������С��ϣ�
	void judgeLinkDouble();//�ж�˫˳
	void judgeLinkThree();//�ж���˳

	void OptimizeLinkSingle();//�޸ĵ�˳��BUG

private:
	int* iCards;//��׼����
	int iMyCards[38];//�ڲ�����

	int iRepeat[26];//���Ƽ�����
	int iSingleInRepeatIndex[38];//������ɸѡ��ĵ��Ƶ�����

	int iWaitOrder[38];//�ȴ��������Ƶ��ƣ������Ϊ���˳�Ӻ�ĵ���
	int iOrder[38];//����
	int iMinLinkIndex[38];//��С���������Ƶ�����
	int iSingleInLinkIndex[38];//��˳��ɸѡ��ĵ��Ƶ�����
	int iLinkIndex[38];//˳������

	int iRocket[4];//���
	int iBomb[38];//ը��  
	int iSingle[38];//����
	int iDouble[38];//����
	int iThree[38];//����
	int iLinkSingle[38];//��˳
	int iLinkDouble[38];//˫˳
	int iLinkThree[38];//��˳

	int* iResult[8];//���ؽ��
};

#endif
