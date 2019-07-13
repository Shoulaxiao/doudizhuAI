#pragma once
//////////////////////////////////
//////�ƾ���Ϣ����̨///////////////
//���ܣ����ڳ�ʼ���־֣������ƾ�////
//����޶��ߣ���Ѷ�� �޶�ʱ�䣺2018.7.22
//�޶����ݼ�Ŀ�ģ�
//1��ȥ��GetInf()�ж�InitRound()���ظ����ã�
//2�������ڿ���̨�ڶ����Ϸ�Լ����ԵĹ��ܣ��������Ű�
//3���޸ı߽��������������쳣�������
//4��������ϸ�����ע��,��ߴ���ɶ���
////////////////////////////////


#include "Ddz.h"
#include "CardsInfo.h"
#include "MonteCarloSearchEngine.h"
#include "CallNum.h"
#include<list>


class BoardMessageControl
{
public:	

	char cUpPlayer;    //�ϼұ��

	char cDownPlayer;  //�¼ұ��

	int  aiBid;       //AI�з���

	int  who = 0;     //0��ʾ�ֵ��ϼҳ��ƣ�1��ʾAI���ƣ�2��ʾ�¼ҳ���

	int  who2 = 0;    //0��ʾ����Ϊ�ϼң�1��ʾAI��2��ʾ�¼�
	
	CardsInfo* CInfo; //��InitTurn�б���ʼ��
	
	MonteCarloSearchEngine *engine;//��InitTurn�б���ʼ��
	
	PokerModelAnalysis*Pma; //��BoardMessageControl�б���ʼ��
	
	BoardMessageControl(); //��ʼ��Pma
	
	~BoardMessageControl();  //�ͷ�engine
	
	void InitTurn(struct Ddz * pDdz);				//P01 ��ʼ����������

	void InitRound(struct Ddz * pDdz);				//M04 ��ʼ����������
	
	void InputMsg(struct Ddz * pDdz);				//P02 ������Ϣ

	void printCard(int*);                             //�ڿ���̨�ж��ƽ������

	void AnalyzeMsg(struct Ddz * pDdz);				//P03 ����������Ϣ

	void GetDou(struct Ddz * pDdz);					//P0301 ��ȡ������汾��ϢDOU

	void GetInf(struct Ddz * pDdz);					//P0302 ��ȡ�������־���ϢINF

	void GetDea(struct Ddz * pDdz);					//P0303 ��ȡ������������ϢDEA

	void GetBid(struct Ddz * pDdz);					//P0304 ��ȡ�����������ϢBID
 
	void GetLef(struct Ddz * pDdz);					//P0305 ��ȡ�����������ϢLEF

	void GetPla(struct Ddz * pDdz);					//P0306 ��ȡ�����������ϢPLA

	void CalPla(struct Ddz * pDdz);					//P030601 ���㼺�����Ʋ���

	void UpdateMyPla(struct Ddz * pDdz);			//P030602 ���ݼ������Ƹ�������

	void UpdateHisPla(struct Ddz * pDdz);			//P030603 �������˳��Ƹ�������

	void GetGam(struct Ddz * pDdz);					//P0307 ��ȡ������ʤ����ϢGAM

	void OutputMsg(struct Ddz * pDdz);				//P04 �����Ϣ

	void CalOthers(struct Ddz * pDdz);				//P05 ������������

	int CalBid(struct Ddz * pDdz);					//I02 ���㼺�����Ʋ���

	void OtherPlayerCards(Ddz *pDdz, int *iCards);  //����������������ʣ��
	
	

};

