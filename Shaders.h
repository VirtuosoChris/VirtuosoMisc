//
//  Shaders.h
//  
//  Should put common / standard shaders or C functions to generate shader source here
//
//  Created by Chris Pugh on 1/12/16.
//
//

#ifndef Shaders_h
#define Shaders_h

#include <sstream>

namespace Virtuoso
{
    const char* vectorSuffix[4] = {".x",".y",".z",".w"};
    const char* splitVariables[4] = {"split.x", "split.y", "split.z", "split.w"};
    
    namespace GLSL
    {
        /// create magic number for bit splitting (eg, 1111 -> 01010101, needed for morton code generation)
        std::size_t magicNumber(std::size_t bitsSetInPattern,std::size_t numRepeats, std::size_t paddingBits, std::size_t maskIn, std::size_t loopCounter)
        {
            if (loopCounter == numRepeats )return maskIn;
            
            const std::size_t mask = maskIn << (bitsSetInPattern + bitsSetInPattern * paddingBits) | (1<<bitsSetInPattern)-1;
            
            return magicNumber(bitsSetInPattern, numRepeats, paddingBits, mask, loopCounter+1);
        }

        std::string mortonFunction(const std::string& type, const std::string& typeOut, int components, int bitdepth)
        {
            std::stringstream sstr;
            
            sstr<<typeOut<<" mortonCode("<<type<<" vecIn)\n{\n";
            
            for(int shift = bitdepth>>1 , repeats=2; shift > 0;shift>>=1, repeats<<=1)
            {
                std::string var = "split";
                
                sstr << "\t";
                if (repeats == 2)
                {
                    sstr << type<< " ";
                    var = std::string("vecIn");
                }
                
                std::size_t magicN = magicNumber(shift, repeats, components-1u, 0u,0u);
                sstr<< "split" <<" = ("<< var <<" | ("<< var<<" << "<<shift * (components-1) << ")) & "<<magicN<<";\n";
            }
            
            sstr << "\treturn ";
            
            for (int i =0; i < components; i++)
            {
                sstr<< splitVariables[i];
                
                if(i)
                {
                    sstr<< " << " << i;
                }
                
                if (i != components-1)
                {
                    sstr << " | ";
                }
                else
                {
                    sstr<<";";
                }
            }
            
            sstr<<"\n}\n";
            return sstr.str();
        }
        
        std::string mortonFunction_ivec4(int bits)
        {
            return mortonFunction("ivec4", "int", 4, bits);
        }
        
        std::string mortonFunction_ivec3(int bits)
        {
            return mortonFunction("ivec3", "int", 3, bits);
        }
        
        std::string mortonFunction_ivec2( int bits)
        {
            return mortonFunction("ivec2", "int", 2, bits);
        }
    }
}
#endif /* Shaders_h */
