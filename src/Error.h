#ifndef ERROR_MSG
#define ERROR_MSG

#include <string>

/** \brief This class is just an error message. 
	It should be thrown wherever the exception has occurred. 
*/
class Error {
	public:
		Error(const std::string& message);
		~Error();
		
		std::string getMessage();
	
	private:
		const std::string errMessage_;
};

#endif
