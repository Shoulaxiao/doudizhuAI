//�޸��ˣ���Ѷ��
//��������Ҫ������Ϣ������
//���ڣ�2018-7-19
//���Ӷ�iStatus�ĳ�ʼ��ֵΪ1�����ӱ���isRandom���������̶������ƣ��޸�kPlayerNameֵ

#include <stdio.h>
#include <string.h>
#include <iostream>
#define kPlayerName "���������"
#define kPlaMax 1000
#pragma once

#define iFShowInConsole true
#define iFShowOtherCard true
#define isRandom true

using namespace std;

struct Ddz
{
	int  iStatus;			//����״̬-1����,0����,1��ʼ
	char sCommandIn[80];	//ͨ����������
	char sCommandOut[80];	//ͨ���������
	int iOnHand[21];		//�����ƣ�����ֵ��ʼ��Ϊ-1��
	int iOnTable[162][21];	//�ѳ������飨����ֵ��ʼ��Ϊ-2��ÿ����һ���ƣ���-1��β��Pass��Ϊ-1��-2������û�г��ƣ�-1����û�г��ƻ��߷ָ-2�����ý������ڸ��޹س�ֵ
	int iUpPlayerCards;		//�ϼ���������
	int iDownPlayerCards;	//�¼���������
	int iToTable[21];		//Ҫ������
	int iOnOtherHand[55];   //������������е���
	char sVer[80];			//Э��汾��
	char sName[80];			//����ѡ�ֳƺ�
	char cDir;				//��ҷ�λ���
	char cLandlord;			//������ҷ�λ���
	char cLastPlay;         //��ģ�����ʱ��λ������ҷ�λ���
	char cWinner;			//ʤ���߷�λ���
	int iBid[3];			//���Ʒ���AΪiBid[0],BΪ[1],CΪ[2]
	int iBmax;				//��ǰ��������ֵ��{0,1,2,3}����ֻ���˳�ʼ����
	int iOTmax;				//��ǰ��������
	int iRoundNow;			//��ǰ�ִ�
	int iRoundTotal;		//���ܾ���
	int iTurnNow;			//��ǰ�ִ�
	int iTurnTotal;			//������
	int iLevelUp;			//����ѡ����
	int iScoreMax;			//ת���÷ֽ���
	int iVoid;				//��Ͼ���м��������û���õ���
	int iLef[3];			//���ֵ���
	int iLastPassCount;		//��ǰ��������PASS����ֵ��[0,2],��ֵ2����������ȡ0��һ��PASSȡ1������PASSȡ2��
	int iLastTypeCount;		//��ǰ��������������ֵ��[0,1108],��ֵ0��iLastPassCount=0ʱ����ֵ��=1ʱ����ԭֵ��=2ʱֵΪ0��//���³���//ʮλ�Ժ��������Ƶ�����,��λ�����Ƶ�����                  
	int iLastMainPoint;		//��ǰ����������������ֵ��[0,14],��ֵ-1��iLastPassCount=0ʱ����ֵ����=1ʱ����ԭֵ��=2ʱֵΪ-1��
	bool haveHelpFriend;	//��¼�Ƿ���������ѣ�������ֻ��һ����ʱ���õ�����Ȩ������С���ƣ��ж϶����Ƿ������Լ�����С���ƣ��� ��ʼ��Ϊfalse

	//Test
	double dWinCount;    //AIӮ�ľ���
};

