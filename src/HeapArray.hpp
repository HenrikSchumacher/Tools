#pragma once

namespace Tools
{

    template <
        typename Scal_, typename Int_,
        Size_T alignment = DefaultAlignment,
        typename Alloc_T_ = AlignedAllocator<Scal_>
    >
    class alignas(ObjectAlignment) HeapArray
    {
        static_assert(IntQ<Int_>,"");
        
    public:
        
        using Scal    = Scal_;
        using Real    = typename Scalar::Real<Scal_>;
        using Int     = Int_;
        using Alloc_T = Alloc_T_;

        static constexpr Size_T Alignment = alignment;
        
    private:
        
        Int buffer_size = 0;

        Scal * restrict buffer = nullptr ;

        
    public:
        
        explicit HeapArray( const Int n )
        :   buffer_size    { Tools::Max(0,n) }
        {
//            logprint("Constuctor of "+ClassName()+" of size "+ToString(d0) );
            Alloc_T::Alloc(buffer,ToSize_T(buffer_size),Alignment);
        }
        
        HeapArray( const Int n, cref<Scal> init )
        :   HeapArray( n )
        {
            Fill( init );
        }
        
        template<typename S>
        HeapArray( cptr<S> a, const Int n )
        :   HeapArray( n )
        {
            Read(a);
        }
        

        // The big four and a half:

        HeapArray() = default;

        // Destructor
        ~HeapArray()
        {
    //        logprint("Destuctor of "+ClassName()+" of size "+ToString(Size()) );
            Alloc_T::Free(buffer);
        }

        // Copy constructor
        HeapArray( const HeapArray & other )
        :   HeapArray( other.buffer_size  )
        {
    //        logprint("Copy of "+ClassName()+" of size "+ToString(other.Size()) );
            Read(other.buffer);
        }

        // Copy-cast constructor
        template<typename S, typename J, Size_T alignment_, typename A_T>
        explicit HeapArray( const HeapArray<S,J,alignment_,A_T> & other )
        :   HeapArray ( other.buffer_size    )
        {
            static_assert(IntQ<J>,"");
    //        logprint("Copy-cast of "+ClassName()+" of size "+ToString(other.Size()) );
            
            Read(other.buffer);
        }

        inline friend void swap( HeapArray & A, HeapArray & B) noexcept
        {
    //        logprint(A.ClassName()+": swap");
            // see https://stackoverflow.com/questions/5695548/public-friend-swap-member-function for details
            using std::swap;
            
            if( &A == &B )
            {
                wprint( std::string("An object of type ") + ClassName() + " has been swapped to itself.");
            }
            else
            {
                swap( A.buffer_size, B.buffer_size );
                swap( A.buffer,      B.buffer      );
            }
        }

        // Move constructor
        HeapArray( HeapArray && other ) noexcept
        :   HeapArray()
        {
    //        logprint(other.ClassName()+": Move-constructor");
            swap(*this, other);
        }


        /* Move-assignment operator */
        mref<HeapArray> operator=( HeapArray && other ) noexcept
        {
    //        logprint(other.ClassName()+": Move-assign");
            if( this == &other )
            {
                wprint("An object of type " + ClassName() + " has been move-assigned to itself.");
            }
            else
            {
                swap( *this, other );
            }
            return *this;
        }

        /* Copy-assignment operator */
        mref<HeapArray> operator=( const HeapArray & other )
        {
            if( this != &other )
            {
    //            logprint(other.ClassName()+": Copy-assignment of size "+ToString( other.buffer_size ));
                
                if( buffer_size != other.buffer_size )
                {
                    buffer_size = other.buffer_size;
                    
    //                logprint(other.ClassName()+": Reallocation of size "+ToString( buffer_size ) );
                    
                    Alloc_T::Free(buffer);
                    Alloc_T::Alloc(buffer,ToSize_T(buffer_size),Alignment);
                }
                
                Read( other.buffer );
            }
            return *this;
        }
        
        
    public:

        Int Size() const
        {
            return buffer_size;
        }

        template<typename S>
        void Read( cptr<S> a )
        {
            copy_buffer( a, buffer, buffer_size );
        }

        //// Parallelized version.
        template<typename S>
        void ReadParallel( cptr<S> a, const Int thread_count )
        {
            copy_buffer<VarSize,Parallel>( a, buffer, buffer_size, thread_count );
        }

        template<typename R>
        std::enable_if_t<Scalar::ComplexQ<Scal> && !Scalar::ComplexQ<R>,void>
        Read( cptr<R> re, cptr<R> im )
        {
            for( Int i = 0; i < buffer_size; ++i )
            {
                buffer[i].real( re[i] );
                buffer[i].imag( im[i] );
            }
        }

        template<typename S>
        void Write( mptr<S> a ) const
        {
            copy_buffer( buffer, a, buffer_size );
        }

        template<typename S>
        void WriteParallel( mptr<S> a, const Int thread_count ) const
        {
            copy_buffer<VarSize,Parallel>( buffer, a, buffer_size, thread_count );
        }

        template<typename R>
        std::enable_if_t<Scalar::ComplexQ<Scal> && !Scalar::ComplexQ<R>,void>
        Write( mptr<R> re, mptr<R> im ) const
        {
            for( Int i = 0; i < buffer_size; ++i )
            {
                re[i] = real(buffer[i]);
                im[i] = imag(buffer[i]);
            }
        }

        void Fill( cref<Scal> init )
        {
            fill_buffer( buffer, init, buffer_size );
        }

        void Fill( cref<Scal> init, const Int thread_count )
        {
            fill_buffer<VarSize,Parallel>( buffer, init, buffer_size, thread_count );
        }

        void SetZero()
        {
            zerofy_buffer( buffer, buffer_size );
        }

        void SetZero( const Int thread_count )
        {
            zerofy_buffer<VarSize,Parallel>( buffer, buffer_size, thread_count );
        }
        
    private:
        
        void BoundCheck( const Int i ) const
        {
#ifdef TOOLS_DEBUG
            if( (i < 0) || (i > buffer_size) )
            {
                eprint(ClassName()+": first index " + ToString(i) + " is out of bounds [ 0, " + ToString(dims[0]) +" [.");
            }
#else
            (void)i;
#endif
        }
        
    public:
        
        force_inline mptr<Scal> data()
        {
            return buffer;
        }

        force_inline cptr<Scal> data() const
        {
            return buffer;
        }
        
        force_inline mptr<Scal> data( const Int i )
        {
            BoundCheck(i);
            
            return &buffer[i];
        }
        
        force_inline cptr<Scal> data( const Int i ) const
        {
            BoundCheck(i);
            
            return &buffer[i];
        }
        
        
        force_inline mptr<Scal> begin()
        {
            return buffer;
        }

        force_inline cptr<Scal> begin() const
        {
            return buffer;
        }

        force_inline mptr<Scal> end()
        {
            return &buffer[buffer_size];
        }

        force_inline cptr<Scal> end() const
        {
            return &buffer[buffer_size];
        }
        
        
        force_inline mref<Scal> operator()(const Int i)
        {
            BoundCheck(i);
            
            return buffer[i];
        }
        
        force_inline cref<Scal> operator()(const Int i) const
        {
            BoundCheck(i);
            
            return buffer[i];
        }
        
        force_inline mref<Scal> operator[](const Int i)
        {
            BoundCheck(i);
            
            return buffer[i];
        }
        
        force_inline cref<Scal> operator[](const Int i) const
        {
            BoundCheck(i);
            
            return buffer[i];
        }
        
        
        force_inline mref<Scal> First()
        {
            return buffer[0];
        }
        
        force_inline cref<Scal> First() const
        {
            return buffer[0];
        }

        force_inline mref<Scal> Last()
        {
            return (buffer_size > 0 ? buffer[buffer_size-1] : buffer[0]);
        }
        
        force_inline cref<Scal> Last() const
        {
            return (buffer_size > 0 ? buffer[buffer_size-1] : buffer[0]);
        }
        
        template<bool copy>
        void Resize( const Int m_, const Int thread_count = 1 )
        {
            const Int m = Ramp(m_);
            
            HeapArray B (m);
            
            if constexpr ( copy )
            {
                if( m <= buffer_size )
                {
                    B.ReadParallel(buffer,thread_count);
                }
                else
                {
                    WriteParallel(B.data(),thread_count);
                }
            }
            
            swap( *this, B );
        }
        
        template<bool copy>
        void RequireSize( const Int m, const Int thread_count = 1 )
        {
            if( m > buffer_size )
            {
                Resize<copy>( m, thread_count );
            }
        }
        
    public:
        
        [[nodiscard]] inline friend std::string to_string(
            cref<HeapArray> A, const int prec = 17
        )
        {
            return ArrayToString( A.buffer, &A.buffer_size, Int(1), prec );
        }

        [[nodiscard]] inline friend std::string ToString( cref<HeapArray> A,
            const int prec = 17,
            std::string line_prefix = std::string("")
        )
        {
            return ArrayToString( A.buffer, &A.buffer_size, Int(1), prec, line_prefix );
        }
        
        [[nodiscard]] std::string friend ToString(
            cref<HeapArray> A, const Int i_begin, const Int i_end
        )
        {
            if( (i_begin >= 0) && ( i_end <= A.buffer_size) )
            {
                return ArrayToString(
                    &A.buffer[i_begin],
                    { Tools::Max(Int(0), i_end-i_begin) }
                );
            }
            else
            {
                return ArrayToString(A.buffer,0);
            }
        }

        inline friend std::ostream & operator<<( std::ostream & s, cref<HeapArray> array )
        {
            s << ToString(array);
            return s;
        }

    public:
        
        static std::string ClassName()
        {
            return std::string("HeapArray")
                + "<" + TypeName<Scal>
                + "," + TypeName<Int>
                + "," + Alloc_T::ClassName()
                + ">";
        }
        
    }; // HeapArray
    
} // namespace Tensors

