#include <fstream>
#include <string>
#include <json/json.h>
#include <iostream>
#include <string>
using namespace Json;
using namespace std;

class GetPara{
	public:
		GetPara(const char *);
		int getPreFilterSize();
		int getPreFilterCap();
		int getBlockSize();
		int getMinDisparity();
		int getNumDisparity();
		int getTextureThreshold();
		int getUniquenessRatio();
		int getSpeckleWindowSize();
		int getSpeckleRange();
		int getDisp12MaxDiff();
		bool getDefault();
		bool getSBMChoice();
		string getImageFileName();
		bool doubleImage();
		string getLeftImageFileName();
		string getRightImageFileName();
		double getBaselineFocus();
		string getOutput();
	private:
		const char *filename;
		Value root;
};

GetPara::GetPara(const char* name){
	filename = name;	
	ifstream in;
	in.open(filename);
	in>>root;
}

string GetPara::getImageFileName(){
	return root["ImageFileName"].asString();
}
int GetPara::getPreFilterSize(){
	return root["PreFilterSize"].asInt();
}
int GetPara::getPreFilterCap(){
	return root["PreFilterCap"].asInt();
}
int GetPara::getBlockSize(){
	return root["BlockSize"].asInt();
}
int GetPara::getMinDisparity(){
	return root["MinDisparity"].asInt();
}
int GetPara::getNumDisparity(){
	return root["NumDisparity"].asInt();
}
int GetPara::getTextureThreshold(){
	return root["TextureThreshold"].asInt();
}
int GetPara::getUniquenessRatio(){
	return root["UniquenessRatio"].asInt();
}
int GetPara::getSpeckleWindowSize(){
	return root["SpeckleWindowSize"].asInt();
}
int GetPara::getSpeckleRange(){
	return root["SpeckleRange"].asInt();
}
int GetPara::getDisp12MaxDiff(){
	return root["Disp12MaxDiff"].asInt();
}
bool GetPara::getDefault(){
	return root["Default"].asBool();
}
bool GetPara::getSBMChoice(){
	return root["SBMChoice"].asBool();
}
string GetPara::getLeftImageFileName(){
	return root["LeftImageFileName"].asString();
}
string GetPara::getRightImageFileName(){
	return root["RightImageFileName"].asString();
}
bool GetPara::doubleImage(){
	return root["DoubleImage"].asBool();
}
double GetPara::getBaselineFocus(){
	return root["BaseLineFocus"].asDouble();
}

string GetPara::getOutput(){
	return root["Output"].asString();
}
