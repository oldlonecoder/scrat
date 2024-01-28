//
// Created by oldlonecoder on 24-01-26.
//

#include "scrat/rt/Components.scrat.h"
#include <AppBook/Util/StrBreak.h>

namespace scrat
{


std::map<scrat::Type::T,     std::string_view> Component::TypeEnums
{
    {Type::Null,         "Null"},
    {Type::Number,       "Number"},
    {Type::Text,         "Text"},
    {Type::VoidPtr,      "VoidPtr"},
    {Type::Any,          "Any"},
    {Type::Leaf,         "Leaf"},
    {Type::Sign,         "Signed"},
    {Type::I8,           "I8"},
    {Type::I16,          "I16"},
    {Type::I32,          "I32"},
    {Type::I64,          "I64"},
    {Type::U8,           "U8"},
    {Type::U16,          "U16"},
    {Type::U32,          "U32"},
    {Type::U64,          "U64"},
    {Type::Float,        "Float"},
    {Type::Var,          "Var"},
    {Type::Bloc,         "Bloc"},
    {Type::Pipe,         "Pipe"},
    {Type::Keyword,      "Keyword"},
    {Type::Unary,        "Unary"},
    {Type::Prefix,       "Prefix"},
    {Type::Postfix,      "Postfix"},
    {Type::Operator,     "Operator"},
    {Type::Binary,       "Binary"},
    {Type::Function,     "Function"},
    {Type::FunctionPtr,  "FunctionPtr"},
    {Type::Object,       "Object"},
    {Type::Pointer,      "Pointer"},
    {Type::Type,         "Type"},
    {Type::Id,           "Id"},
    {Type::Ref,          "Ref"},
    {Type::Punc,         "Punc"},
    {Type::Assign,       "Assign"},
    {Type::Bool,         "Bool"},
    {Type::Hex,          "Hex"},
    {Type::BinFormat,    "BinFormat"},
    {Type::OctalFormat,  "OctalFormat"},
    {Type::F32,          "F32"},
    {Type::F64,          "F64"},
    {Type::F128,         "F128"},
    {Type::OpenPair,     "OpenPair"},
    {Type::ClosePair,    "ClosePair"},
    {Type::Static,       "Static"},
    {Type::Const,        "Const"},
    {Type::Integer,      "Integer"},
    {Type::Unsigned,     "Unsigned"},
    {Type::LineComment,   "LineComment"},
    {Type::BlocComment,   "BlocComment"},
    {Type::Litteral,      "Litteral"}
};



std::map<scrat::Mnemonic, std::string_view> Component::MnemonicEnums
{
    {Mnemonic::Null,             Lexem::Null},
    {Mnemonic::LeftShift,        Lexem::LeftShift},
    {Mnemonic::Radical,          Lexem::Radical},
    {Mnemonic::Exponent,         Lexem::Exponent},
    {Mnemonic::RightShift,       Lexem::RightShift},
    {Mnemonic::Indirection,      Lexem::Indirection},
    {Mnemonic::Deref,            Lexem::Deref},
    {Mnemonic::Decr,             Lexem::Decr},
    {Mnemonic::Incr,             Lexem::Incr},
    {Mnemonic::AssignAdd,        Lexem::AssignAdd},
    {Mnemonic::AssignSub,        Lexem::AssignSub},
    {Mnemonic::AssignMul,        Lexem::AssignMul},
    {Mnemonic::AssignDiv,        Lexem::AssignDiv},
    {Mnemonic::AssignMod,        Lexem::AssignMod},
    {Mnemonic::AssignAnd,        Lexem::AssignAnd},
    {Mnemonic::AssignOr,         Lexem::AssignOr},
    {Mnemonic::AssignXor,        Lexem::AssignXor},
    {Mnemonic::AssignC1,         Lexem::AssignC1},
    {Mnemonic::AssignLeftShift,  Lexem::AssignLShift},
    {Mnemonic::AssignRightShift, Lexem::AssignRShift},
    {Mnemonic::LessEq,           Lexem::LessEqual},
    {Mnemonic::GreaterEq,        Lexem::GreaterEqual},
    {Mnemonic::Equal,            Lexem::Equal},
    {Mnemonic::NotEqual,         Lexem::NotEqual},
    {Mnemonic::Add,              Lexem::Addition},
    {Mnemonic::Sub,              Lexem::Sub},
    {Mnemonic::Mul,              Lexem::Multiply},
    {Mnemonic::Indirection,      Lexem::Indirection},
    {Mnemonic::CommentCpp,       Lexem::CommentCpp},
    {Mnemonic::Modulo,           Lexem::Modulo},
    {Mnemonic::LessThan,         Lexem::LessThan},
    {Mnemonic::GreaterThan,      Lexem::GreaterThan},
    {Mnemonic::Assign,           Lexem::Assign},
    {Mnemonic::BinaryAnd,        Lexem::BinaryAnd},
    {Mnemonic::BinaryOr,         Lexem::BinaryOr},
    {Mnemonic::BinaryXor,        Lexem::Xor},
    {Mnemonic::C1,               Lexem::C1},// complement a 1
    {Mnemonic::C2,               Lexem::C2},
    {Mnemonic::BinaryNot,        Lexem::Not},   //
    {Mnemonic::LogicalAnd,       Lexem::BoolAnd},
    {Mnemonic::LogicalOr,        Lexem::BoolOr},
    {Mnemonic::OpenAbsOp,        Lexem::AbsBegin},
    {Mnemonic::CloseAbsOp,       Lexem::AbsEnd},
    {Mnemonic::OpenPar,          Lexem::OpenPar},
    {Mnemonic::ClosePar,         Lexem::ClosePar},
    {Mnemonic::Openindex,        Lexem::OpenIndex},
    {Mnemonic::Closeindex,       Lexem::CloseIndex},
    {Mnemonic::Openbrace,        Lexem::BraceBegin},
    {Mnemonic::Closebrace,       Lexem::BraceEnd},
    {Mnemonic::BeginComment,     Lexem::CommentBegin},
    {Mnemonic::EndComment,       Lexem::CommentEnd},
    {Mnemonic::Div,              Lexem::Division},
    {Mnemonic::Comma,            Lexem::Comma},
    {Mnemonic::Scope,            Lexem::Scope},
    {Mnemonic::Semicolon,        Lexem::Semicolon},
    {Mnemonic::Colon,            Lexem::Colon},
    {Mnemonic::Range,            Lexem::Range},
    {Mnemonic::Factorial,        Lexem::Factorial},
    {Mnemonic::Positive,         Lexem::Positive},
    {Mnemonic::Negative,         Lexem::Negative},
    {Mnemonic::Squote,           Lexem::Squote},
    {Mnemonic::Dquote,           Lexem::Dquote},
    {Mnemonic::Ternary,          Lexem::Ternary},
    {Mnemonic::Hash,             Lexem::Hash},
    {Mnemonic::Eos,              Lexem::Eos},
    {Mnemonic::Dot,              Lexem::Dot},
    {Mnemonic::Return,           Lexem::Return},
    {Mnemonic::If,               Lexem::If},
    {Mnemonic::Pi,               Lexem::Pi},
    {Mnemonic::Number,           Lexem::Number},
    {Mnemonic::U8,               Lexem::U8},
    {Mnemonic::U16,              Lexem::U16},
    {Mnemonic::U32,              Lexem::U32},
    {Mnemonic::U64,              Lexem::U64},
    {Mnemonic::I8,               Lexem::I8},
    {Mnemonic::I16,              Lexem::I16},
    {Mnemonic::I32,              Lexem::I32},
    {Mnemonic::I64,              Lexem::I64},
    {Mnemonic::Float,            Lexem::Float}, // Important: F32 : float; F64: double; F128: long double;
    {Mnemonic::String,           Lexem::String},
    {Mnemonic::Then,             Lexem::Then},
    {Mnemonic::Else,             Lexem::Else},
    {Mnemonic::Const,            Lexem::Const},
    {Mnemonic::Include,          Lexem::Include},
    {Mnemonic::Amu,              Lexem::Amu},
    {Mnemonic::At,               Lexem::At},
    {Mnemonic::Prime,            Lexem::Prime},
    {Mnemonic::Do,               Lexem::Do},
    {Mnemonic::While,            Lexem::While},
    {Mnemonic::For,              Lexem::For},
    {Mnemonic::Until,            Lexem::Until},
    {Mnemonic::Repeat,           Lexem::Repeat},
    {Mnemonic::Switch,           Lexem::Switch},
    {Mnemonic::Case,             Lexem::Case},
    {Mnemonic::Type,             Lexem::Type},
    {Mnemonic::LowHex,           Lexem::LowHex},
    {Mnemonic::UpHex,            Lexem::UpHex},
    {Mnemonic::Cosinus,          Lexem::Cosinus},
    {Mnemonic::ArcCosinus,       Lexem::ArcCosinus},
    {Mnemonic::Tangent,          Lexem::Tangent},
    {Mnemonic::ArcTangent,       Lexem::ArcTangent},
    {Mnemonic::Sinus,            Lexem::Sinus},
    {Mnemonic::ArcSinus,         Lexem::ArcSinus},
    {Mnemonic::Object,           Lexem::Object},
    {Mnemonic::Static,           Lexem::Static},
    {Mnemonic::This,             Lexem::This},
    {Mnemonic::Unshadow,         Lexem::Unshadow},// Take the address of the local var instance identifier, from the first parent bloc the var is declared instead.
    {Mnemonic::Try,              Lexem::Try},
    {Mnemonic::Catch,            Lexem::Catch},
    {Mnemonic::Throw,            Lexem::Throw},
    {Mnemonic::LineComment,      Lexem::LineComment},
    {Mnemonic::BlocComment,      Lexem::BlocComment},
};




std::string Component::Name(Type::T Typ)
{
    std::string Text;
    Type::T TypeBits = Typ;
    //int                a, c, b;

    for(const auto &T: Component::TypeEnums)
    {
        if(!TypeBits)
            break;

        if(T.first & TypeBits)
        {
            Text += T.second;
            TypeBits &= ~T.first;  // Remove the Bit from the "Sem" field.
            if(TypeBits)
                Text += ':';
        }
    }
    if(Text.empty())
        Text += "Null";
    return Text;
}

Type::T Component::FromStr(const std::string &TName)
{
    StrBreak SB;
    StrBreak::Word::Array Words;
    Type::T TMap = 0;
    StrBreak::ConfigData Data = {
        .View = TName,
        .Delimiters = ":",
        .o = StrBreak::Discard,
        .Words = Words
    };

    auto n = SB(Data);
    if(!n) return Type::Null;
    for(auto S: Words)
    {
        for(auto const& [k,v] : Component::TypeEnums)
        {
            if(v == S()) TMap |= k;
        }
    }
    return TMap;
}

std::string Component::MnemonicName(Mnemonic M)
{
    return std::string(Component::MnemonicEnums[M]);
}


Mnemonic Component::MnemonicEnum(const std::string &MName)
{
    for(auto const& [M, v] : Component::MnemonicEnums)
    {
        if(MName == v) return M;
    }

    return Mnemonic::Noop;
}



} // scrat