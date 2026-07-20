#pragma once

#include <cfenv>

namespace Tools
{
/*!@brief Attempts to figure out the actual rounding mode on the current floating-point environment by performing a few tests.
 *
 * We do not trust std::fegetround. Under Apple clang version 21.0.0.1.1.101 it did not work reliably without explicitly using `#pragma STDC FENV_ACCESS ON`. This function tests the actual behavior of the FPU instead of relying on compiler support.
 *
 * Also, Daniel Lemire says that a call to `std::fegetround` could be relatively expensive. A call to this function is now for free, but it requires only two casts from `double` to `float` and two casts from `float` to `double`.
 */
    template<bool verboseQ = false>
    int CurrentRoundingMode()
    {
        // I learnt the trick to use `static volatile` from  Daniel Lemire's blogpost: https://lemire.me/blog/2022/11/16/a-fast-function-to-check-your-floating-point-rounding-mode/
        
        // The choice of number x_0_ is not fully arbitrary. In order to distinguish the modes FE_TONEAREST and FE_TOWARDZERO, it is important that this number is rounds _up_ in rounding mode FE_TONEAREST.
        static volatile double x_0_ =  0.9999999999999998;  // 1 - eps
        static volatile double y_0_ = -0.9999999999999998;  // eps - 1
        
        // This prevents multiple loading of volatile variables.
        double x_0 = x_0_;
        double y_0 = y_0_;
        
        // We exploit that the cast operator to lower lower precision has to use the rounding mode.
        float x_1 = static_cast<float>(x_0);
        float y_1 = static_cast<float>(y_0);
        
        // Now we cast back to double for the comparisons (making implicit casts explicit.) Casting to higher precision is exact.
        double x_2 = static_cast<double>(x_1);
        double y_2 = static_cast<double>(y_1);
        
        bool positive_rounded_upQ = (x_2 > x_0);
        bool negative_rounded_upQ = (y_2 > y_0);
        
        int mode;
        
        if( positive_rounded_upQ )
        {
            if( negative_rounded_upQ )
            {
                mode = FE_UPWARD;
            }
            else
            {
                mode = FE_TONEAREST;
            }
        }
        else
        {
            if( negative_rounded_upQ )
            {
                mode = FE_TOWARDZERO;
            }
            else
            {
                mode = FE_DOWNWARD;
            }
        }
        
        if constexpr ( verboseQ )
        {
            print("Tools::CurrentRoundingMode()");
            valprint("x_0", std::format("{: .17g}",x_0) );
            valprint("x_1", std::format("{: .17g}",x_1) );
            valprint("x_2", std::format("{: .17g}",x_2) );
            valprint("y_0", std::format("{: .17g}",y_0) );
            valprint("y_1", std::format("{: .17g}",y_1) );
            valprint("y_2", std::format("{: .17g}",y_2) );
            valprint("positive rounded up", BoolString(positive_rounded_upQ) );
            valprint("negative rounded up", BoolString(negative_rounded_upQ) );
            valprint("Detected rounding mode",mode);
        }
        
        return mode;
    }
    
    
    
/*!@brief This class provides RAII-style way to temporarily change the rounding mode of the floating-point environment (FE). The constructor uses std::fesetround to set the rounding mode to the desired value (if different from the current value); the constructor uses std::fesetround to reset the rounding mode to the previous value.
 *
 * The effect last only during the life-span of instance of this class. Several instances of ScopedRoundingMode can be alive at once. Then the actual rounding mode should be the one specified by the one that was instantiated last.
 *
 * Beware that the rounding mode can also be altered by different means. This is why the  destructor checks for mismatch between expected and actually found rounding mode.
 *
 * @tparam thorough_checksQ If set to true (default), then the constructor will check whether its expected mode corresponds to the actual floating point mode. In case of a mismatch, it issues an error.
 *
 */
    template<bool thorough_checksQ = true>
    class ScopedRoundingMode
    {
    protected:
        
        int prev_mode_;
        int intended_mode_;
        
    public:
        
        ScopedRoundingMode() = delete;
        
        /*!@brief This is the class's constructor. It is _not_ a function to change the rounding mode! CAUTION: Similar to other RAII constructs like `std::lock_guard`, you really, really have name the class for proper working. A line like `ScopedRoundingMode(mode);` will create an instance and immediately destruct it, so that it won't have any lasting effect. The correct way is to give the instance a name like this: `ScopedRoundingMode barrier (mode);`.
         *
         * @param mode Desired rounding mode. The only possible values are FE_TONEAREST, FE_DOWNWARD, FE_UPWARD, and FE_TOWARDZERO.
         */
        
        explicit ScopedRoundingMode( const int mode )
        :   prev_mode_     { CurrentRoundingMode() }
        ,   intended_mode_ { mode         }
        {
//            #pragma STDC FENV_ACCESS ON
            // The C++ documentation on https://en.cppreference.com/cpp/numeric/fenv/feround insinuates that this pragma were not needed, but Apple Clang 21.0.0 does not set the rounding mode without it.
            
            if( !ModeValidQ(intended_mode_) )
            {
                eprint("ScopedRoundingMode(): Requested rounding mode " + ToString(intended_mode_) + " does not coincide with any of FE_TONEAREST (=" + ToString(FE_TONEAREST) + "), FE_DOWNWARD (=" + ToString(FE_DOWNWARD) + "), FE_UPWARD  (=" + ToString(FE_UPWARD) + "), or FE_TOWARDZERO (=" + ToString(FE_TOWARDZERO) + "). Using the previous rounding mode.");
                intended_mode_ = prev_mode_;
            }
            
            if( intended_mode_ != prev_mode_ )
            {
                SetMode(intended_mode_);
            }
        }
        
        ~ScopedRoundingMode()
        {
            int actual_mode = CurrentRoundingMode();
            
            if constexpr ( thorough_checksQ )
            {
                if( intended_mode_ != actual_mode )
                {
                    eprint("ScopedRoundingMode(): intendend mode (= " + ToString(intended_mode_) + ") does not match actual mode (= " + ToString(actual_mode) + "). Somebody or something must have messed with your floating-point environment.");
                }
            }
            
            if( prev_mode_ != actual_mode )
            {
                SetMode(prev_mode_);
            }
        }
        
        static bool ModeValidQ( const int mode )
        {
            return (mode == (FE_TONEAREST)) || (mode == (FE_DOWNWARD)) || (mode == (FE_UPWARD)) || (mode == (FE_TOWARDZERO));
        }
        
        int IntendedMode() const
        {
            return intended_mode_;
        }

        int PreviousMode() const
        {
            return prev_mode_;
        }
        
        template<bool verboseQ = false>
        bool BehavesAsIntendedQ() const
        {
            return CurrentRoundingMode<verboseQ>() == IntendedMode();
        }
        
    private:
        
        void SetMode( const int intended_mode )
        {
            std::fesetround( intended_mode );
            
            if( CurrentRoundingMode() != intended_mode )
            {
                eprint("ScopedRoundingMode::SetMode(): Call to std::fesetround did not have the desired effect.");
            }
        }
        
    }; // class ScopedRoundingMode
}
