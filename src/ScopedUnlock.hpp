#pragma once

namespace Tools
{

/*!@brief Use this class to temporarily unlock an instance of class `T`.
 *
 * The class `T` needs to provide public methods `Lock()`, `Unlock()`, and `bool LockedQ()`. What these functions do is in the  discretion of class `T`, but the intended behavior is that `Lock()` sets an internal Boolean to `true`, that `Unlock()` sets it to `false`, and that `LockedQ()` returns its current value.
 */
    template<class T, bool thorough_checksQ = true>
    class ScopedUnlock
    {
    protected:
        
        T & object;
        bool prev_lockedQ;
        
    public:
        
        ScopedUnlock() = delete;
        
        /*!@brief This is the class's constructor. It needs access to the object it references to.*/
        explicit ScopedUnlock(  T & object_to_unlock  )
        :   object       { object_to_unlock }
        ,   prev_lockedQ { object.LockedQ() }
        {
            if( prev_lockedQ ) { object.Unlock(); }
        }
        
        ~ScopedUnlock()
        {
            bool actually_lockedQ = object.LockedQ();
            
            if constexpr ( thorough_checksQ )
            {
                if( actually_lockedQ )
                {
                    eprint("ScopedUnlock(): object is not currently locked, but should be.");
                }
            }
            
            if( prev_lockedQ )
            {
                if( !actually_lockedQ ) { object.Lock(); }
            }
            else
            {
                if( actually_lockedQ ) { object.Unlock(); }
            }
        }

        /*!@brief Whether the associated object is locked.*/
        bool LockedQ() const
        {
            return object.LockedQ();
        }
        
        /*!@brief Whether the associated object was previously locked.*/
        bool PreviouslyLockedQ() const
        {
            return prev_lockedQ;
        }
        
        /*!@brief Check whether the locking state of the associated object is what it is supposed to be.*/
        bool BehavesAsIntendedQ() const
        {
            return !object.LockedQ();
        }
        
    }; // class ScopedUnlock
}
