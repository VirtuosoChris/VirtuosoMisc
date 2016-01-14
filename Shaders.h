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
        
        const std::string mortonFunctionSmall = R"STRING(

        // This function assumes the input contains numbers that fit in a byte, and that the implementation has int64 available
        // Source: Sean Eron Anderson's "Bit Twiddling Hacks" webpage
        int mortonCodeSmallValues(ivec2 vecIn)
        {
            const uint64_t a = 0x0101010101010101UL;
            const uint64_t b = 0x8040201008040201UL;
            const uint64_t c = 0x0102040810204081UL;

            const u64vec2 shift = u64vec2(49, 48);
            const u64vec2 mask = u64vec2(0x5555, 0xAAAA);

            u64vec2 firstStep = (vecIn * a & b) * c;
            u64vec2 result = firstStep >> shift & mask;

            return int(result.x | result.y);
        }
        )STRING";

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
