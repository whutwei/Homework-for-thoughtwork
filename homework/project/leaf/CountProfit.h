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
	int a[6];//依次为year;month;day;begin_hour;end_hour;person_num;

	int site_num;	//需要定的场地数
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
	msg_data *msg;										//存储每一行的数据信息
	int msg_num;										//输入信息数，行数
	int all_income;
	int all_payment;
	int all_profit;

private:
	int CountWeek(int year, int month,int day);			//计算星期几
	int CountSiteNum(int person_num);					//根据人数计算场地数
	void CountOneMsg(msg_data *msg);
	int CountMsgNum(string ss);							//计算输入的信息的条数
	void TransformData();								//input字符串转换为数字
	void TransformOutput();

public:
	string generatedSummary(string input);
};


#endif
