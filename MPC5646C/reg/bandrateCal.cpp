#include<iostream>
#include<string>
#include<stdlib.h>
using namespace std;

char Hex2Char(char data);

int main()
{
	int bandrate = 0;
	string configuration;
	long int configuration_number;
	int reg_binary[32];
	int function = 0;
	int f_CANCLK = 0;
	
	struct {
	int PRESDIV;
	int RJW;
	int PSEG1;
	int PSEG2;
	int PROPSEG;
	}regconf;
 
	cout << "which do you want:\n" 
	<< "1.bandrate\n" 
	<< "2.configuration\n"
	<< "Please input the number:";
	cin >> function;
	switch(function)
	{
		case 1:
		{
			cout << "f_CANCLK(Hz):";
			cin >> f_CANCLK;
			cout << "configuration(Hex):";
			cin >> configuration;
			configuration_number = strtol(configuration.c_str(),NULL,16);
			cout << configuration_number;
			for(int i=0;i<32;i++)
			{
				reg_binary[31-i] = configuration_number%2;
				configuration_number = configuration_number >> 1;
			}
			cout <<'\n';
			for(int i=0;i<32;i++)
			{
				cout << char(reg_binary[i]+48);
			}
			
			cout <<'\n';
			regconf.PRESDIV = 0;
			for(int i=7;i>=0;i--)
			{
				regconf.PRESDIV = regconf.PRESDIV + reg_binary[i]*(1<<(7-i));
				cout << char(reg_binary[i]+48);
			}
			cout <<'\n'<< "PRESDIV:"
			<< regconf.PRESDIV;

			regconf.RJW = 0;
			for(int i=9;i>=8;i--)
			{
				regconf.RJW += reg_binary[i]*(1<<(9-i));
				cout << char(reg_binary[i]+48);
			}
			cout <<'\n'<< "RJW:"
			<< regconf.RJW;

			
			regconf.PSEG1 = 0;
			for(int i=12;i>=10;i--)
			{
				regconf.PSEG1 += reg_binary[i]*(1<<(12-i));
				cout << char(reg_binary[i]+48);
			}
			cout <<'\n'<< "PSEG1:"
			<< regconf.PSEG1;
			
			
			regconf.PSEG2 = 0;
			for(int i=15;i>=13;i--)
			{
				regconf.PSEG2 += reg_binary[i]*(1<<(15-i));
				cout << char(reg_binary[i]+48);
			}
			cout <<'\n'<< "PSEG2:"
			<< regconf.PSEG2;
		

			regconf.PROPSEG = 0;
			for(int i=31;i>=29;i--)
			{
				regconf.PROPSEG += reg_binary[i]*(1<<(31-i));
				cout << char(reg_binary[i]+48);
			}
			cout <<'\n'<< "PROPSEG:"
			<< regconf.PROPSEG;
			bandrate = f_CANCLK/((4+regconf.PROPSEG+regconf.PSEG1+regconf.PSEG2)*(regconf.PRESDIV+1));
			cout <<'\n'<< "bit rate is :"
			<< bandrate;			
	
			break;
		}
		case 2:
		{
			cout << "f_CANCLK(Hz):";
			cin >> f_CANCLK;
			cout << "bit rate(Hz):";
			cin >> bandrate;
			if(f_CANCLK%bandrate != 0){
				cout << "error 1:can't get configuration";
				break;
			}
			int temp = f_CANCLK/bandrate;
			cout << "temp:" << temp;
			regconf.RJW = 0x03;
			for(regconf.PRESDIV=0;regconf.PRESDIV < 516;regconf.PRESDIV++){
				for(regconf.PROPSEG=0;regconf.PROPSEG < 8;regconf.PROPSEG++){
					for(regconf.PSEG1=0;regconf.PSEG1 < 8;regconf.PSEG1++){
						for(regconf.PSEG2=0;regconf.PSEG2 < 8;regconf.PSEG2++){
							if(temp == ((4 + regconf.PROPSEG + regconf.PSEG1 + regconf.PSEG2)*(regconf.PRESDIV + 1)))
							{
								configuration[0] = Hex2Char(char((regconf.PRESDIV >> 4) & 0x0F));
								configuration[1] = Hex2Char(char(regconf.PRESDIV & 0x0F));
								configuration[2] = Hex2Char(char(((regconf.RJW << 2) & 0x0C) | ((regconf.PSEG1 >> 1) & 0x03 ) ));
								configuration[3] = Hex2Char(char((((regconf.PSEG1&0x01)<<3)&0x08) | regconf.PSEG2));
								configuration[4] = '0';
								configuration[5] = '0';
								configuration[6] = '0';
								configuration[7] = Hex2Char(char(regconf.PROPSEG));
								
								cout <<"configuration:";
								for(int i=0;i<8;i++)
								{
									cout << configuration[i];
								}
								cout << '\n';
							}
							
						}
					}
				}
			}
			
			break;
		}
	}
}



char Hex2Char(char data)
{
	if(data > 10)
	{
		return data + 0x37;
	}
	else
	{
		return data + 0x30;
	}
}
