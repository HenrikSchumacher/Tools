#pragma once

namespace Tools
{
    
    class RoundingModeBarrier
    {
    protected:
        
        int prev_state;
        int state;
        
        
    public:
        
        RoundingModeBarrier() = delete;
        
        explicit RoundingModeBarrier( int state_ )
        :   prev_state( std::fegetround() )
        ,   state ( state_ )
        {
            std::fesetround( state );
        }
        
        int CurrentState() const
        {
            return state;
        }
        
        int PreviousState() const
        {
            return prev_state;
        }
        
        ~RoundingModeBarrier()
        {
            std::fesetround( prev_state );
        }
        
    }; // class RoundingModeBarrier
    
}
