#include "stdafx.h"
#include "CountProfit.h"

CountProfit::CountProfit()
{
	all_income = 0;
	all_payment = 0;
	all_profit = 0;

	string path_test;
	cout<<"please input the path of test.txt"<<endl;  //����test.txt��ŵ�·��
	getline(cin,path_test);
	ifstream infile;
	infile.open(path_test,ios::in);
	if(!infile)
	{
		cout<<"open file error"<<endl;				//·����������ļ�������
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
	msg_num = CountMsgNum(ss);		//�����������Ϣ
	msg = new msg_data[msg_num];
	TransformData();				//����ת��

	for(int i=0;i<msg_num;i++)
	{
		CountOneMsg(msg+i);
		all_income+=msg[i].income;
		all_payment+=msg[i].payment;
	}
	all_profit = all_income-all_payment;

	TransformOutput();				//ת���õ�����ַ���

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
	int T,X,t; //tΪ����
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
	while ((pos = ss.find_first_of("\r\n",pos)) != string::npos)		//���ַ����ĵ�posλ��������ң��ɹ��ҵ�\r\n,��
	{
		if((pos - old_pos)<=2)			//����txt�ĵ�β������س����з������
			break;		
		old_pos = pos;
		count++;		//������һ
		pos++;			//�������Һ����
	}
	return count;
}

void CountProfit::TransformData()
{
	//���ұ� ���ָ������ �������ʽ�йأ������ʽ�䶯����Ҫ���Ĵ˴���
	const int dis[6]={0,1,1,1,4,4};
	const string div[6] = {"-" , "-" , " " , ":" , ":" , "\r\n"};

	string temp_s;

	string::size_type end_pos = 0;
	string::size_type begin_pos = 0;

	int i,j;
	//����ÿһ������ת��
	for(i=0;i<msg_num;i++)
	{
		//ÿһ������������ʱת��
		for(j=0;j<6;j++)
		{
			begin_pos = end_pos + dis[j];
			end_pos = input.find_first_of(div[j],begin_pos);
			temp_s = input.substr(begin_pos,end_pos-begin_pos);
			msg[i].a[j] = stoi(temp_s);
		}
	}
}

//˵��������һ����������Ϣ�����ڵȣ����site_num income payment ��profit
void CountProfit::CountOneMsg(msg_data *msg)
{
	msg->site_num = CountSiteNum(msg->a[5]);			//���㳡����
	if(msg->site_num==0)								
	{
		msg->income = msg->payment = msg->profit = 0;
		return;
	}
	msg->income = 30 * msg->a[5];						//����income
	int a = CountWeek(msg->a[0],msg->a[1],msg->a[2]);	//�����ܼ�
	//��һ�����صĻ���
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
	//�������ܻ���
	msg->payment = one_payment*msg->site_num;
	//����
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
		//�ҵ��ڶ���" "�����ض�
		end_pos = input.find_first_of(" ",begin_pos);
		end_pos++;
		end_pos = input.find_first_of(" ",end_pos);
		end_pos++;
		temp_s1 = input.substr(begin_pos,end_pos-begin_pos);
		temp_s = temp_s + temp_s1 + "+";			//ƴ��
		//incomeת��Ϊ�ַ�������ƴ��
		ss<<msg[i].income;
		ss>>temp_s1;
		ss.clear();ss.str("");
		temp_s = temp_s + temp_s1 + " ";
		
		ss<<"-"<<msg[i].payment;
		ss>>temp_s1;
		ss.clear();ss.str("");
		temp_s = temp_s + temp_s1 + " ";

		if(msg[i].profit>0)  
			ss<<"+"<<msg[i].profit<<"\r\n";		//�������ϼӺ�
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