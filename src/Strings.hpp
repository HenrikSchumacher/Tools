#pragma once

#include "Strings/ToChars.hpp"
#include "Strings/FromChars.hpp"
#include "Strings/CharCount.hpp"
#include "Strings/CharArray.hpp"

namespace Tools
{
    
    // Not a really good discriminator, but good enough to distinguish a char converter class from an integral type.
    template<typename C, typename T>
    concept CharConv = NonIntQ<C>;
    
    template<typename A, typename ...Int>
    concept ArrayFun = NonPointerQ<A>;
    
    template<class T>
    concept Stringy = std::is_convertible_v<T, std::string_view>;
    
    template<class T>
    concept NonStringy = !Stringy<T>;
    
    namespace Format
    {
        namespace Vector
        {
            struct Default
            {
                static constexpr char prefix_0 [3] = "{ ";
                static constexpr char infix_0  [3] = ", ";
                static constexpr char suffix_0 [3] = " }";
            };
            
            struct Space
            {
                static constexpr char prefix_0 [1] = "";
                static constexpr char infix_0  [2] = " ";
                static constexpr char suffix_0 [1] = "";
            };
            
            struct TSV
            {
                static constexpr char prefix_0 [1] = "";
                static constexpr char infix_0  [2] = "\t";
                static constexpr char suffix_0 [1] = "";
            };
        }
        
        namespace Matrix
        {
            struct Tall
            {
                static constexpr char prefix_0 [3] = "{\n";
                static constexpr char infix_0  [3] = ",\n";
                static constexpr char suffix_0 [3] = "\n}";
                static constexpr char prefix_1 [4] = " { ";
                static constexpr char infix_1  [3] = ", ";
                static constexpr char suffix_1 [3] = " }";
            };
            
            struct Wide
            {
                static constexpr char prefix_0 [3] = "{ ";
                static constexpr char infix_0  [3] = ", ";
                static constexpr char suffix_0 [3] = " }";
                static constexpr char prefix_1 [4] = "{ ";
                static constexpr char infix_1  [3] = ", ";
                static constexpr char suffix_1 [3] = " }";
            };
            
            struct TSV
            {
                static constexpr char prefix_0 [1] = "";
                static constexpr char infix_0  [2] = "\n";
                static constexpr char suffix_0 [1] = "";
                static constexpr char prefix_1 [1] = "";
                static constexpr char infix_1  [2] = "\t";
                static constexpr char suffix_1 [1] = "";
            };
            
            struct CSV
            {
                static constexpr char prefix_0 [1] = "";
                static constexpr char infix_0  [2] = "\n";
                static constexpr char suffix_0 [1] = "";
                static constexpr char prefix_1 [1] = "";
                static constexpr char infix_1  [2] = ",";
                static constexpr char suffix_1 [1] = "";
            };
        }
        
        
        namespace Cube
        {
            struct Tall
            {
                static constexpr char prefix_0 [3] = "{\n";
                static constexpr char infix_0  [3] = ",\n";
                static constexpr char suffix_0 [3] = "\n}";
                static constexpr char prefix_1 [4] = " {\n";
                static constexpr char infix_1  [3] = ",\n";
                static constexpr char suffix_1 [4] = "\n }";
                static constexpr char prefix_2 [5] = "  { ";
                static constexpr char infix_2  [3] = ", ";
                static constexpr char suffix_2 [3] = " }";
            };
            
            struct Wide
            {
                static constexpr char prefix_0 [3] = "{ ";
                static constexpr char infix_0  [3] = ", ";
                static constexpr char suffix_0 [3] = " }";
                static constexpr char prefix_1 [3] = "{ ";
                static constexpr char infix_1  [3] = ", ";
                static constexpr char suffix_1 [3] = " }";
                static constexpr char prefix_2 [3] = "{ ";
                static constexpr char infix_2  [3] = ", ";
                static constexpr char suffix_2 [3] = " }";
            };
            
            struct Medium
            {
                static constexpr char prefix_0 [3] = "{\n";
                static constexpr char infix_0  [3] = ",\n";
                static constexpr char suffix_0 [3] = "\n}";
                static constexpr char prefix_1 [4] = " { ";
                static constexpr char infix_1  [3] = ", ";
                static constexpr char suffix_1 [3] = " }";
                static constexpr char prefix_2 [3] = "{ ";
                static constexpr char infix_2  [3] = ", ";
                static constexpr char suffix_2 [3] = " }";
            };
        }
    }
    
} // namespace Tools
