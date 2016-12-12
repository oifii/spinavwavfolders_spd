#include <string>
#include <fstream>
#include <vector>

#include <iostream>
#include <sstream>
using namespace std;
//#include <stdlib.h>

#include <ctime>
#include <windows.h>

int main(int argc, char *argv[])
{
	///////////////////
	//read in arguments
	///////////////////
	//char pathname[2048] = {"D:\\oifii-org\\httpdocs\\ns-org\\nsd\\ar\\cp\\audio_spi\\spimidisampler"};
	char pathname[2048] = {"."};
	char maxdetectionduration[64] = {""};
	double maxdetectionduration_s = 1.0f;
	if(argc>1)
	{
		strcpy(pathname, argv[1]);
	}
	if(argc>2)
	{
		maxdetectionduration_s = atof(argv[2]);
	}
	sprintf(maxdetectionduration, "%f", maxdetectionduration_s);

	///////////////////////////////////////////////////////
	//execute cmd line to get all wavfolder_*.txt filenames
	///////////////////////////////////////////////////////
	string path = pathname;
	string cmdline = "DIR " + path + "\\" + "wavfolder_*.txt /B /O:N > txtfilenames.txt";
	system(cmdline.c_str());


	///////////////////////////////////////
	//load filenames.txt into string vector
	///////////////////////////////////////
	vector<string> txtfilenames;
	ifstream ifs("txtfilenames.txt");
	string temp;
	while(getline(ifs,temp))
	{
		temp = path + "\\" + temp;
		txtfilenames.push_back(temp);
	}

	vector<string>::iterator it;	
	for ( it=txtfilenames.begin() ; it < txtfilenames.end(); it++ )
	{
		vector<string> wavfilenames;
		ifstream ifs(*it);
		string temp;
		while(getline(ifs,temp))
		{
			wavfilenames.push_back(temp);
		}

		//////////////////////////////////////////////////////////////
		//browse throught wav filenames and call spipitchdetection.exe
		//////////////////////////////////////////////////////////////
		string quote;
		quote = "\"";
		string cmdbuffer;

		vector<string>::iterator iterator;	
		for ( iterator=wavfilenames.begin() ; iterator < wavfilenames.end(); iterator++ )
		{
			if((*iterator).rfind(".txt")==string::npos)
			{
				system("del spd_ok.txt");
				cout << *iterator << endl; //*it is a .wav filename
				cmdbuffer = "spipitchdetection.exe " + quote + *iterator + quote + " " + maxdetectionduration;
				cout << cmdbuffer << endl;
				system(cmdbuffer.c_str());
				bool found = false;
				do
				{
					FILE* pFILE = fopen("spd_ok.txt", "r");
					if(pFILE)
					{
						found=true;
						fclose(pFILE);
					}
					else
					{
						Sleep(500);
					}
				} while(!found);
			}
		}
	}
	return 0;
}