#pragma once

namespace Tools
{
    template<typename Class_T>
    struct Messenger
    {
        template<Stringy S>
        static OutString MethodString( S && tag )
        {
            OutString str;
            str << Class_T::ClassName();
            str << "::";
            str << std::forward<S>(tag);
            return str;
        }
        
        template<Size_T N>
        static consteval auto MethodName( const ct_string<N> & tag )
        {
            return Class_T::ClassName() + "::" + tag;
        }
        
        template<Size_T N>
        static consteval auto MethodName( const char(&tag)[N] )
        {
            return Class_T::ClassName() + "::" + tag;
        }
        
        template<Stringy S>
        static std::string MethodName( const S & tag )
        {
            return MethodString(tag);
        }

//        template<Size_T N, typename... Args>
//        static OutString Message( const ct_string<N> & tag, Args &&... args )
//        {
//            return OutString::FromMisc( Class_T::ClassName() + "::" + tag, args...);
//        }
//        
//        template<typename S, typename... Args>
//        static OutString Message( const S & tag, const Args &... args )
//        {
//            return OutString::FromMisc( Class_T::ClassName(), "::", tag, args...);
//        }

        template<Stringy S, typename... Args>
        static void print( const S & tag, const Args &... args )
        {
            Tools::print(Class_T::ClassName(), "::", tag, ": ", args...);
        }
        
        template<Stringy S, typename... Args>
        static void logprint( const S & tag, const Args &... args )
        {
            Tools::logprint(Class_T::ClassName(), "::", tag, ": ", args...);
        }
        
        template<Stringy S, typename... Args>
        static void eprint( const S & tag, const Args &... args )
        {
            Tools::eprint(Class_T::ClassName(), "::", tag, ": ", args...);
        }
        
        template<Stringy S, typename... Args>
        static void error( const S & tag, const Args &... args )
        {
            Tools::error(Class_T::ClassName(), "::", tag, ": ", args...);
        }
        
        template<Stringy S, typename... Args>
        static void wprint( const S & tag, const Args &... args )
        {
            Tools::wprint(Class_T::ClassName(), "::", tag, ": ", args...);
        }
        
        static constexpr std::string ClassName()
        {
            return "Messenger";
        }
        
    }; // class Messenger
    
} // namespace Tools
