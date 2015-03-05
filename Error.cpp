#include "Error.h"

Error::Error(const std::string& message) : errMessage_(message){

};

Error::~Error(){

};

std::string Error::getMessage(){
	return errMessage_;
};
