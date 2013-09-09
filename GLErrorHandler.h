#ifndef ERROR_HANDLER_H_INCLUDED
#define ERROR_HANDLER_H_INCLUDED

#include <iostream>
#include <GL/gl.h>

int logGLErrors(std::ostream& log){
	
	int numErrors = 0;

	log<<"\nOpenGL Error Check\n=============================\n";

	while(true){
			
		switch( glGetError() ){

			case GL_NO_ERROR:
				{
					if(!numErrors){
						log<<"No errors found\n";		
					}
					log<<std::endl;
					return numErrors;
				}
			
			case GL_INVALID_ENUM:
				{
					error++;
					log<<"GL_INVALID_ENUM: Enum argument is out of range\n";
				
				}
			case GL_INVALID_VALUE:
				{
					error++;
					log<<"GL_INVALID_VALUE: Numeric value argument out of range\n";
							
				}

			case GL_INVALID_OPERATION:
				{
					error++;
					log<<"GL_INVALID_OPERATION: Operation is invalid in current state\n";
				}

			case GL_OUT_OF_MEMORY:
				{
					
					error++;
					log<<"GL_OUT_OF_MEMORY error\n";
				
				}


		}
	
	}





}
	


#endif