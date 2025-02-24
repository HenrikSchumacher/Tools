#pragma once

namespace Tools
{
    // TODO: Make the job pointers respect the size of the cache line somehow.
    
    template<typename Int_>
    class JobPointers
    {
        static_assert(IntQ<Int_>,"");
        
    public:
        
        using Int = Int_;
        using Container_T = HeapArray<Int,Int>;
        
//        using Alloc_T = AlignedAllocator<Int>;
        
        JobPointers() = default;
        
    private:
        
        explicit JobPointers( Int thread_count_ )
        :   thread_count( Max( Int(1), thread_count_ ) )
        ,   job_ptr     ( thread_count + 1 )
        {
            print("JobPointers allocates " + ToString(thread_count + 1) + ".");
        }
        
    public:
        
        
        ~JobPointers() = default;
        
        
//        // Copy constructor
//        JobPointers( const JobPointers & other )
//        :   JobPointers( other.thread_count  )
//        {
//            job_ptr.Read(other.job_ptr.data());
//        }
//
//        // Copy-cast constructor
//        template<typename J>
//        explicit JobPointers( const JobPointers<J> & other )
//        :   JobPointers ( other.thread_count    )
//        {
//            static_assert(IntQ<J>,"");
//            
//            Read(other.job_ptr.data());
//        }
//
//        inline friend void swap( JobPointers & A, JobPointers & B) noexcept
//        {
//    //        logprint(A.ClassName()+": swap");
//            // see https://stackoverflow.com/questions/5695548/public-friend-swap-member-function for details
//            using std::swap;
//            
//            if( &A == &B )
//            {
//                wprint( std::string("An object of type ") + ClassName() + " has been swapped to itself.");
//            }
//            else
//            {
//                swap( A.thread_count, B.thread_count );
//                swap( A.job_ptr,      B.job_ptr      );
//            }
//        }
//
//        // Move constructor
//        JobPointers( JobPointers && other ) noexcept
//        :   JobPointers()
//        {
//    //        logprint(other.ClassName()+": Move-constructor");
//            swap(*this, other);
//        }
//
//
//        /* Move-assignment operator */
//        mref<JobPointers> operator=( JobPointers && other ) noexcept
//        {
//    //        logprint(other.ClassName()+": Move-assign");
//            if( this == &other )
//            {
//                wprint("An object of type " + ClassName() + " has been move-assigned to itself.");
//            }
//            else
//            {
//                swap( *this, other );
//            }
//            return *this;
//        }
//
//        /* Copy-assignment operator */
//        mref<JobPointers> operator=( const JobPointers & other )
//        {
//            if( this != &other )
//            {
//    //            logprint(other.ClassName()+": Copy-assignment of size "+ToString( other.buffer_size ));
//                
//                if( thread_count != other.thread_count )
//                {
//                    thread_count = other.thread_count;
//                    
//    //                logprint(other.ClassName()+": Reallocation of size "+ToString( buffer_size ) );
//
//                    job_ptr = Container_T(thread_count+1);
//                }
//                
//                Read( other.job_ptr );
//            }
//            return *this;
//        }
        
        
        
        template<typename I>
        JobPointers( const Int job_count, const I thread_count_ )
        :   JobPointers( static_cast<Int>(thread_count_) )
        {
            print("A");
            BalanceWorkLoad( job_count );
            
            print( ToString(job_ptr) );
        }
        
        template<typename T, typename I>
        JobPointers( const Int job_count, cptr<T> costs, const I thread_count_, bool accumulate = true )
        :   JobPointers( static_cast<Int>(thread_count_) )
        {
            if( accumulate )
            {
                print("B");
                BalanceWorkLoad_Accumulated( job_count, costs );
            }
            else
            {
                print("C");
                BalanceWorkLoad( job_count, costs );
            }
            
            print( ToString(this) );
        }
        
        template<typename I>
        const Int & operator[]( const I i ) const
        {
            return job_ptr[i];
        }
        
        template<typename I>
        const Int & operator()( const I i ) const
        {
            return job_ptr[i];
        }
        
        Int Size() const
        {
            return job_ptr.Size();
        }
        
        Int ThreadCount() const
        {
            return thread_count;
        }
        
        Int JobCount() const
        {
            return job_ptr.Last();
        }
        
        const Int * data() const
        {
            return job_ptr.data();
        }

        cref<Container_T> Container() const
        {
            return job_ptr;
        }
        
        template<typename I>
        void Read( cptr<I> buffer )
        {
            std::copy( buffer, &buffer[thread_count + 1], job_ptr );
        }
        
        template<typename I>
        void Write( mptr<I> buffer ) const
        {
            std::copy( job_ptr, &job_ptr[thread_count + 1], buffer );
        }

    public:
        
        cptr<Int> begin() const
        {
            return job_ptr.begin();
        }
        
        cptr<Int> end() const
        {
            return job_ptr.end();
        }
        
        
        void BalanceWorkLoad( const Int job_count )
        {
            for( Int k = 0; k < thread_count+1; ++k )
            {
                job_ptr[k] = JobPointer( job_count, thread_count, k );
            }
        }
        
        template<typename T>
        void BalanceWorkLoad_Accumulated( const Int job_count, cptr<T> costs )
        {
            // This function reads in a list job_acc_costs of accumulated costs, then allocates job_ptr as a vector of size thread_count + 1, and writes the work distribution to it.
            // Assigns threads to consecutive chunks jobs, ..., job_ptr[k+1]-1 of jobs.
            // Uses a binary search to find the chunk boundaries.
            // The cost of the i-th job is job_acc_costs[i+1] - job_acc_costs[i].
            // The cost of the k-th thread goes from job no job_ptr[k] to job no job_ptr[k+1] (as always in C/C++, job_ptr[k+1] points _after_ the last job.
        
            
            TOOLS_PTIC("BalanceWorkLoad_Accumulated");
            
            HeapArray<T,Int> acc_costs( job_count + 1 );
            
            acc_costs[0] = Int(0);
            
            // Better not parallelize this.
//            parallel_accumulate( &costs[0], &acc_costs[1], job_count, thread_count );
            
            parallel_accumulate( costs, &acc_costs[1], job_count, Int(1) );

            BalanceWorkLoad( job_count, acc_costs.data() );
            
            TOOLS_PTOC("BalanceWorkLoad_Accumulated");
        }
        
        template<typename T>
        void BalanceWorkLoad( const Int job_count, cptr<T> acc_costs )
        {
            static_assert(IntQ<T>,"");
            
            // This function reads in a list job_acc_costs of accumulated costs, then allocates job_ptr as a vector of size thread_count + 1, and writes the work distribution to it.
            // Assigns threads to consecutive chunks jobs, ..., job_ptr[k+1]-1 of jobs.
            // Uses a binary search to find the chunk boundaries.
            // The cost of the i-th job is job_acc_costs[i+1] - job_acc_costs[i].
            // The cost of the k-th thread goes from job no job_ptr[k] to job no job_ptr[k+1] (as always in C/C++, job_ptr[k+1] points _after_ the last job.
        
            
            TOOLS_PTIC("BalanceWorkLoad");
            
            if( job_count <=0 )
            {
                eprint(ClassName()+"::BalanceWorkLoad: job_count cost is <= 0. Aborting with invalid job pointers.");
                
                
                job_ptr.Fill( Int(0) );
                
                return;
            }

            job_ptr[thread_count] = job_count;
            
            const Int naive_chunk_size = CeilDivide(job_count,thread_count);
            
            const T total_cost = acc_costs[job_count];
            
            
            if( total_cost <=0 )
            {
                eprint(ClassName()+"::BalanceWorkLoad: Total cost is 0. Aborting with invalid job pointers");
                
                TOOLS_LOGDUMP(job_count);
                TOOLS_LOGDUMP(thread_count);
                
                logvalprint( "acc_costs", ArrayToString( acc_costs, {job_count + 1} ) );
                
                std::fill( &job_ptr[0], &job_ptr[thread_count+1], Int(0) );
                            
                TOOLS_PTOC("BalanceWorkLoad");
                            
                return;
            }
            
            const T per_thread_cost = CeilDivide(total_cost,static_cast<T>(thread_count));
            
            // binary search for best work load distribution
            
            //  There is quite a lot false sharing in this loop... so better not parallelize it.
            for( Int thread = 0; thread < thread_count - 1; ++thread )
            {
                // each thread (other than the last one) is required to have at least this accumulated cost
                T target = Min( total_cost, per_thread_cost * static_cast<T>(thread + 1) );
                
                // find an index a such that b_row_acc_costs[ a ] < target;
                // taking naive_chunk_size * thread as initial guess, because that might be nearly correct for costs that are evenly distributed over the block rows
                Int pos = thread + 1;
                Int a = Min(job_count, naive_chunk_size * pos );
                while( acc_costs[a] >= target )
                {
                    --pos;
                    a = Min(job_count, naive_chunk_size * pos );
                };
                
                // find an index  b such that b_row_acc_costs[ b ] >= target;
                // taking naive_chunk_size * (thread + 1) as initial guess, because that might be nearly correct for costs that are evenly distributed over the block rows
                pos = thread + 1;
                Int b = Min(job_count, naive_chunk_size * pos );
                while( (b < job_count) && (acc_costs[b] < target) )
                {
                    ++pos;
                    b = Min(job_count, naive_chunk_size * pos );
                };
                
                // binary search
                while( b > a + 1 )
                {
                    const Int c = a + (b-a)/2;
                    if( acc_costs[c] > target )
                    {
                        b = c;
                    }
                    else
                    {
                        a = c;
                    }
                }

                job_ptr[thread + 1] = b;
            }
            
            TOOLS_PTOC("BalanceWorkLoad");
        }

        [[nodiscard]] std::string friend ToString( const JobPointers & J )
        {
            return ToString(J.job_ptr);
        }
        
        
        
    private:
        
        Int thread_count = 0;
        Container_T job_ptr;
        
        
    public:
        
        static std::string ClassName()
        {
            return std::string("JobPointers") + "<" + TypeName<Int> + ">";
        }
        
        
    }; // JobPointers

} // namespace Tools
