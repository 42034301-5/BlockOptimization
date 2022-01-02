#ifndef __CONVERT_HPP__
#define __CONVERT_HPP__
#include "global.hpp"

QuadExp convertSET(const std::cmatch& m)
{
    QuadExp result;
    result.op = "SET";
    result.a1 = m[1].str();
    result.a2 = m[2].str();
    result.a3 = "-";

    return result;
}

QuadExp convertART(const std::cmatch& m)
{
    static std::map<std::string, std::string> opt = {
        {"+", "ADD"}, {"-", "SUB"},
        {"*", "MUL"}, {"/", "DIV"},
        {"%", "MOD"}
    };

    QuadExp result;
    result.op = opt[m[3]];
    result.a1 = m[1].str();
    result.a2 = m[2].str();
    result.a3 = m[4].str();

    return result;
}

QuadExp convertFAR(const std::cmatch& m)
{
    QuadExp result;
    result.op = "FAR";
    result.a1 = m[1].str();
    result.a2 = m[2].str();
    result.a3 = m[3].str();

    return result;
}

QuadExp convertTAR(const std::cmatch& m)
{
    QuadExp result;
    result.op = "TAR";
    result.a1 = m[1].str();
    result.a2 = m[2].str();
    result.a3 = m[3].str();

    return result;
}

QuadExp convertJMP(const std::cmatch& m)
{
    QuadExp result;
    result.op = "JMP";
    result.a1 = m[1].str();
    result.a2 = "-";
    result.a3 = "-";

    return result;
}

QuadExp convertJOP(const std::cmatch& m)
{
    static std::map<std::string, std::string> opt = {
        {">", "JGT"}, {">=", "JGE"},
        {"<", "JLT"}, {"<=", "JLE"},
        {"==", "JEQ"}, {"!=", "JNE"}
    };

    QuadExp result;
    result.op = opt[m[2]];
    result.a1 = m[4].str();
    result.a2 = m[1].str();
    result.a3 = m[3].str();

    return result;
}

static std::vector<std::pair<std::regex, std::function<QuadExp(const std::cmatch&)>>> expRules = 
{
    {std::regex("(\\w*)\\s[=]\\s(\\w*)"), std::function(convertSET)},
    {std::regex("(\\w*)\\s[=]\\s(\\w*)\\s([\\+\\-\\*\\/\\%])\\s(\\w*)"), std::function(convertART)},
    {std::regex("(\\w*)\\s[=]\\s(\\w*)\\s\\[\\s(\\w*)\\s\\]"), std::function(convertFAR)},
    {std::regex("(\\w*)\\s\\[\\s(\\w*)\\s\\]\\s[=]\\s(\\w*)"), std::function(convertTAR)},
    {std::regex("!:\\s(\\w*)"), std::function(convertJMP)},
    {std::regex("\\?\\s(\\w*)\\s(.*)\\s(\\w*)\\s:\\s(\\w*)"), std::function(convertJOP)}
};

QuadExp convert(const std::string& tri)
{
    QuadExp e;
    if(tri == "HALT")
    {
        e.op = "HALT";
        e.a1 = "-", e.a2 = "-", e.a3 = "-";
        return e;
    }
    std::cmatch m;
    for(auto&& rule : expRules)
    {
        if(std::regex_match(tri.c_str(), m, rule.first))
        {
            e = rule.second(m);
            break;
        }
    }
    return e;
}


std::string convert2tri(const QuadExp& e)
{
    std::string result;
    if(e.op == "HALT")
        return "HALT";
    std::map<std::string, std::string> opt = 
    {
        {"ADD", "+"}, {"SUB", "-"},
        {"MUL", "*"}, {"DIV", "/"},
        {"MOD", "%"}
    };
    std::map<std::string, std::string> rop = 
    {
        {"JGT", ">"}, {"JGE", ">="},
        {"JLT", "<"}, {"JLE", "<="},
        {"JEQ", "=="}, {"JNE", "!="}
    };

    //(SET, A, X, -) => A = X
    if(e.op == "SET")
        result = e.a1 + " = " + e.a2;
    
    //(ADD, A, B, C) => A = B + C
    else if(
        e.op == "ADD" || e.op == "SUB" ||
        e.op == "MUL" || e.op == "DIV" ||
        e.op == "MOD"
    )
        result = e.a1 + " = " + e.a2 + " " + opt[e.op] + " " + e.a3;
    
    //(FAR, X, A, I) => X = A [ I ]
    else if(e.op == "FAR")
        result = e.a1 + " = " + e.a2 + " [ " + e.a3 + " ]";
    
    //(TAR, A, I, X) => A [ I ] = X
    else if(e.op == "TAR")
        result = e.a1 + " [ " + e.a2 + " ] = " + e.a3;
    
    //(JMP, T, -, -) => !: T
    else if(e.op == "JMP")
        result = "!: " + e.a1;

    //(JGT, T, X, Y) => ? X > Y : T
    else if(e.op[0] == 'J' && e.op != "JMP")
        result = "? " + e.a2 + " " + rop[e.op] + " " + e.a3 + " : " + e.a1;

    return result;
}

#endif