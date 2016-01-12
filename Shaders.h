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

// how easily can we facilitate packing data  ? like 5 5 5 in a 16 bit int
// we SHOULD do this for our test case.  we really only need 8 bit int for 4, 4.

namespace Virtuoso
{
    const char* vectorSuffix[4] = {".x",".y",".z",".w"};
    const char* splitVariables[4] = {"splitX", "splitY", "splitZ", "splitW"};
    
    namespace GLSL
    {
        /// create magic number for
        std::size_t magicNumber(std::size_t bitsSetInPattern,std::size_t numRepeats, std::size_t paddingBits, std::size_t maskIn, std::size_t loopCounter)
        {
            if (loopCounter == numRepeats )return maskIn;
            
            const std::size_t mask = maskIn << (bitsSetInPattern + bitsSetInPattern * paddingBits) | (1<<bitsSetInPattern)-1;
            
            return magicNumber(bitsSetInPattern, numRepeats, paddingBits, mask, loopCounter+1);
        }
        
        const std::string mortonFunctionSmall = R"STRING(
        
        // This function assumes the input contains numbers that fit in a byte, and that the implementation has int64 available
        // Source: Sean Eron Anderson's "Bit Twiddling Hacks" webpage
        int mortonCodeSmallValues(ivec2 vecIn)
        {
            return ((int64_t(vecIn.x) * 0x0101010101010101ULL & 0x8040201008040201ULL) * 0x0102040810204081ULL >> 49) & 0x5555 |
            ((int64_t(vecIn.y) * 0x0101010101010101ULL & 0x8040201008040201ULL) * 0x0102040810204081ULL >> 48) & 0xAAAA;
            
        }
        )STRING";
        
        std::string mortonFunction(const std::string& type, const std::string& typeOut, int components, int bitdepth)
        {
            std::stringstream sstr;
            
            sstr<<typeOut<<" mortonCode("<<type<<" vecIn)\n{\n";
            
            for (int i =0; i < components; i++)
            {
                sstr <<"\t"<< typeOut <<" "<<splitVariables[i]<<" = vecIn"<<vectorSuffix[i]<<";\n";
                
                for(int shift = bitdepth>>1 , repeats=2; shift > 0;shift>>=1, repeats<<=1)
                {
                    
                    //int magicN = 55;
                    std::size_t magicN = magicNumber(shift, repeats, components-1u, 0u,0u);
                    sstr<<"\t"<< splitVariables[i]<<" = ("<<splitVariables[i]<<" | ("<<splitVariables[i]<<" << "<<shift * (components-1) << ")) & "<<magicN<<";\n";
                }
            }
            
            sstr << "\t"<<typeOut <<" rval = ";
            
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
            return mortonFunction("ivec3", "int", 3, 4, bits);
        }
        
        std::string mortonFunction_ivec2( int bits)
        {
            return mortonFunction("ivec2", "int", 2, 4, bits);
        }
    }
}
#endif /* Shaders_h */
