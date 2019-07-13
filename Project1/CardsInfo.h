#ifndef _CardsInfo
#define _CardsInfo

#include "Ddz.h"
//////////////////////////////////
//////������Ϣ����///////////////
//���ܣ����ڳ�ʼ���־֣������ƾ�////
////////////////////////////////

class CardsInfo
{
public:
	CardsInfo();
	~CardsInfo();
	static CardsInfo* Instance();

	///�ж�����////
	int IsType0Pass(int iCs[]);						//B00 �ж�Pass������1�ǣ�0����

	int IsType1Rocket(int iCs[]);					//B01 �жϻ��������1�ǣ�0����

	int IsType2Bomb(int iCs[]);						//B02 �ж�ը��������1�ǣ�0����

	int	IsType3Single(int iCs[]);					//B03 �жϵ��ƣ�����1�ǣ�0����

	int	IsType4Double(int iCs[]);					//B04 �ж϶��ƣ�����1�ǣ�0����

	int	IsType5Three(int iCs[]);					//B05 �ж�����������1�ǣ�0����

	int IsType6ThreeOne(int iCs[]);					//B0601 �ж�����һ��������1�ǣ�0����

	int IsType6ThreeDouble(int iCs[]);				//B0602 �ж�����һ�ԣ�����1�ǣ�0����

	int IsType7LinkSingle(int iCs[]);				//B07 �жϵ�˳������1�ǣ�0����

	int IsType8LinkDouble(int iCs[]);				//B08 �ж�˫˳������1�ǣ�0����

	int	IsType9LinkThree(int iCs[]);				//B09 �ж���˳������1�ǣ�0����

	int	IsType10LinkThreeSingle(int iCs[]);			//B1001 �ж���˳����������1�ǣ�0����

	int	IsType10LinkThreeDouble(int	iCs[]);			//B1002 �ж���˳���ԣ�����1�ǣ�0����

	int IsType11FourSingle(int iCs[]);				//B1101 �ж��Ĵ�����������1�ǣ�0����

	int IsType11FourDouble(int iCs[]);				//B1102 �ж��Ĵ����ԣ�����1�ǣ�0����


	///�������ͼ�����//
	int	AnalyzeTypeCount(int iCards[]);				//I03  �������ͺ�����

	int	AnalyzeMainPoint(int iCards[]);				//I04 ����������С����


	///////���������㡢����/////
	int CountCards(int iCards[]);					//I01 ����������

	void SortById(int iCards[]);					//M01 ���Ʊ�����������

	void SortByMuch(int iCards[]);					//M02 ��ͬ���������Ƚ�������

	void SortByMain(int iCards[]);					//M03 ������������������


	//����//
	void AppendCardsToS(int iCards[], char sMsg[]);	//M05 ����������ת��Ϊ�ַ���׷��

	// ��������������
	int AnalyzeMainMaxPoint(int iCards[]);
	
	int LastPlayerCardsCount(Ddz * pDdz);
	//ȥ���ظ����Ʋ���С��������
	int GetUnReaptedCards(int iCards[]);
};


#endif _CardsInfo