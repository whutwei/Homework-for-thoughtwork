#ifndef _COUNTPROFIT_H
#define _COUNTPROFIT_H

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <sstream>

using namespace std;
using std::string;

typedef struct
{
	int a[6];//����Ϊyear;month;day;begin_hour;end_hour;person_num;

	int site_num;	//��Ҫ���ĳ�����
	int income;
	int payment;
	int profit;
}msg_data;


class CountProfit
{
public:
	CountProfit();
	~CountProfit();

public:
	string input;
	string output;

private:
	msg_data *msg;										//�洢ÿһ�е�������Ϣ
	int msg_num;										//������Ϣ��������
	int all_income;
	int all_payment;
	int all_profit;

private:
	int CountWeek(int year, int month,int day);			//�������ڼ�
	int CountSiteNum(int person_num);					//�����������㳡����
	void CountOneMsg(msg_data *msg);
	int CountMsgNum(string ss);							//�����������Ϣ������
	void TransformData();								//input�ַ���ת��Ϊ����
	void TransformOutput();

public:
	string generatedSummary(string input);
};


#endif
