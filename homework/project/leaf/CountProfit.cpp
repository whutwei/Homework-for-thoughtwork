#include "stdafx.h"
#include "CountProfit.h"

CountProfit::CountProfit()
{
	all_income = 0;
	all_payment = 0;
	all_profit = 0;

	string path_test;
	cout<<"please input the path of test.txt"<<endl;  //输入test.txt存放的路径
	getline(cin,path_test);
	ifstream infile;
	infile.open(path_test,ios::in);
	if(!infile)
	{
		cout<<"open file error"<<endl;				//路径错误或者文件不存在
		exit(1);
	}

	stringstream ss(input);
	ss<<infile.rdbuf();
	input=ss.str();
	infile.close();
	cout<<input<<endl;							
}

CountProfit::~CountProfit()
{

}

string CountProfit::generatedSummary(string ss)
{
	msg_num = CountMsgNum(ss);		//计算多少条信息
	msg = new msg_data[msg_num];
	TransformData();				//数据转换

	for(int i=0;i<msg_num;i++)
	{
		CountOneMsg(msg+i);
		all_income+=msg[i].income;
		all_payment+=msg[i].payment;
	}
	all_profit = all_income-all_payment;

	TransformOutput();				//转换得到输出字符串

	delete []msg;
	return this->output;
}


int CountProfit::CountWeek(int year, int month,int day)
{
	int week;
	if(month==1||month==2) {
        month+=12;
        year--;
    }
    week=(day+2*month+3*(month+1)/5+year+year/4-year/100+year/400)%7+1;

	return week;
}


int CountProfit::CountSiteNum(int person_num)
{
	int T,X,t; //t为场数
	T=person_num/6;
	X=person_num%6;
	if(T==0)
	{
		if(X<4)
		{
			t=0;
			person_num=0;
		}
		else
			t=1;
	}
	if(T==1)
		t = 2;
	if(T == 2 || T == 3)
	{
		if(X >= 4)
			t = T+1;
		else t =T;
	}
	if(T > 3)
		t = T;
	return t;

}

int CountProfit::CountMsgNum(string ss)
{
	string::size_type pos = 0;
	string::size_type old_pos = 0;
	int count=0;
	while ((pos = ss.find_first_of("\r\n",pos)) != string::npos)		//从字符串的第pos位置往后查找，成功找到\r\n,则
	{
		if((pos - old_pos)<=2)			//避免txt文档尾部多个回车换行符误计数
			break;		
		old_pos = pos;
		count++;		//计数加一
		pos++;			//继续查找后面的
	}
	return count;
}

void CountProfit::TransformData()
{
	//查找表 （分割符与间距 与输入格式有关，输入格式变动则需要更改此处）
	const int dis[6]={0,1,1,1,4,4};
	const string div[6] = {"-" , "-" , " " , ":" , ":" , "\r\n"};

	string temp_s;

	string::size_type end_pos = 0;
	string::size_type begin_pos = 0;

	int i,j;
	//依次每一行数据转换
	for(i=0;i<msg_num;i++)
	{
		//每一行依次年月日时转换
		for(j=0;j<6;j++)
		{
			begin_pos = end_pos + dis[j];
			end_pos = input.find_first_of(div[j],begin_pos);
			temp_s = input.substr(begin_pos,end_pos-begin_pos);
			msg[i].a[j] = stoi(temp_s);
		}
	}
}

//说明，传入一场比赛的信息，日期等，算出site_num income payment 和profit
void CountProfit::CountOneMsg(msg_data *msg)
{
	msg->site_num = CountSiteNum(msg->a[5]);			//计算场地数
	if(msg->site_num==0)								
	{
		msg->income = msg->payment = msg->profit = 0;
		return;
	}
	msg->income = 30 * msg->a[5];						//计算income
	int a = CountWeek(msg->a[0],msg->a[1],msg->a[2]);	//计算周几
	//定一个场地的花费
	int one_payment = 0;
	if((a>1||a==1)&&(a<5||a==5))
	{
		for(int i=msg->a[3]+1;i<msg->a[4]+1;i++)
		{
			if(i>9&&i<13)
				one_payment+=30;
			if(i>12&&i<19)
				one_payment+=50;
			if(i>18&&i<21)
				one_payment+=80;
			if(i>20&&i<23)
				one_payment+=60;
		}
	}
	if((a==6)||(a==7))
	{
		for(int i=msg->a[3]+1;i<msg->a[4]+1;i++)
		{
			if(i>9&&i<13)
				one_payment+=40;
			if(i>12&&i<19)
				one_payment+=50;
			if(i>18&&i<23)
				one_payment+=60;
		}
	}
	//定场地总花费
	msg->payment = one_payment*msg->site_num;
	//利润
	msg->profit = msg->income - msg->payment;
}

void CountProfit::TransformOutput()
{
	string temp_s("[summary]\r\n\r\n");
	string temp_s1;

	string::size_type end_pos = 0;
	string::size_type begin_pos = 0;

	stringstream ss;

	for(int i=0;i<msg_num;i++)
	{
		//找到第二个" "处并截断
		end_pos = input.find_first_of(" ",begin_pos);
		end_pos++;
		end_pos = input.find_first_of(" ",end_pos);
		end_pos++;
		temp_s1 = input.substr(begin_pos,end_pos-begin_pos);
		temp_s = temp_s + temp_s1 + "+";			//拼接
		//income转换为字符串，并拼接
		ss<<msg[i].income;
		ss>>temp_s1;
		ss.clear();ss.str("");
		temp_s = temp_s + temp_s1 + " ";
		
		ss<<"-"<<msg[i].payment;
		ss>>temp_s1;
		ss.clear();ss.str("");
		temp_s = temp_s + temp_s1 + " ";

		if(msg[i].profit>0)  
			ss<<"+"<<msg[i].profit<<"\r\n";		//正数则补上加号
		else
			ss<<msg[i].profit<<"\r\n";
		ss>>temp_s1;
		ss.clear();ss.str("");
		temp_s = temp_s + temp_s1;

		begin_pos = input.find_first_of("\r\n",end_pos);
	}

	temp_s+="\r\n\r\nTotal Income: ";
	ss<<all_income;
	temp_s+=ss.str();
	ss.clear();ss.str("");

	temp_s+="\r\nTotal Payment: ";
	ss<<all_payment;
	temp_s+=ss.str();
	ss.clear();ss.str("");

	temp_s+="\r\nProfit: ";
	ss<<all_profit;
	temp_s = temp_s+ss.str()+"\r\n";
	ss.clear();ss.str("");

	output = temp_s;

}