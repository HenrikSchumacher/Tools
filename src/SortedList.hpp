
namespace Tools
{
    template<typename T, typename I>
    class SortedList
    {
        std::vector<T> list;
        
    public:
        
        using iterator       = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;
        
        iterator begin()
        {
            return list.begin();
        }
        
        const_iterator begin() const
        {
            return list.cbegin();
        }
        
        iterator end()
        {
            return list.end();
        }
        
        const_iterator end() const
        {
            return list.cend();
        }
        
        SortedList() = default;
        
        ~SortedList() = default;
        
        explicit SortedList( const I n )
        {
            Reserve(n);
        }
        
        void Clear()
        {
            list.clear();
        }
        
        void Reserve( const std::size_t n )
        {
            list.reserve(n);
        }
        
        
//        I FindPosition( const T & X ) const
//        {
//            //Find first position whose element is not smaller than X.
//
//            I i = 0;
//            while( i < Size() && X < list[i] )
//            {
//                ++i;
//            }
//            return i;
//        }

        I FindPosition( const T & X ) const
        {
            //Finds first position whose element is not smaller than X.

            return static_cast<I>(
                std::distance(
                    list.begin(),
                    std::lower_bound(list.begin(), list.end(), X)
                )
            );
            
//            I i = 0;
//            const I n = list.size();
//
//            while( i < n && list[i] < X )
//            {
//                ++i;
//            }
//
//            return i;
        }
//        I FindPosition( const T & X ) const
//        {
//            //Finds largest position whose element is not smaller than X.
//
//            if( Size() == 0 )
//            {
//                print("a");
//                return 0;
//            }
//
//            if( X <= Min() )
//            {
//                print("b");
//                return 0;
//            }
//
//            if( X > Max() )
//            {
//                print("c");
//                return Size();
//            }
//
//            if( X == Max() )
//            {
//                print("d");
//                return Size()-1;
//            }
//
//            I a = 0;
//            I b = Size()-1;
//
//            //list[a] <= X < list[b]
//
//            // binary search
//            I c;
//            I delta = b - a;
//            while( delta > 0 )
//            {
//                c = b - delta/2;
//                if( list[c] > X )
//                {
//                    b = c;
//                }
//                else
//                {
//                    a = c;
//                }
//            }
//            // list[a] <= X < list[b];
//            print("e");
//            print("X = "+Tools::ToString(X));
//            print("a = "+Tools::ToString(a)+", list[a] = "+Tools::ToString(list[a]));
//            print("b = "+Tools::ToString(b)+", list[b] = "+Tools::ToString(list[b]));
//            return a;
//        }
        
        
        I Find( const T & X ) const
        {
            if( Size() == 0 )
            {
                return -1;
            }
            
            I i = FindPosition(X);

            return (list[i] == X) ? i : -1;
        }
        
        bool Insert( const T & X )
        {
//            tic("Insert");
            I i = FindPosition(X);
            
//            print("X    = " + Tools::ToString(X));
//            print("i    = " + Tools::ToString(i));
//            print("list = " + ToString());
            
            if( i == Size() )
            {
                list.push_back(X);
                return true;
            }
            
            if( list[i] != X )
            {
                list.insert( list.begin()+i, X );
//                print("list = " + ToString());
//                toc("Insert");
                return true;
            }
            else
            {
//                toc("Insert");
                return false;
            }
            
        }
        
        bool Drop( const T & X )
        {
            I i = Find(X);
            if( i >= 0 )
            {
                list.erase(list.begin()+i);
                return true;
            }
            else
            {
                return false;
            }
        }
        
        I Size() const
        {
            return static_cast<I>(list.size());
        }
        
        T Min() const
        {
            return list.front();
        }
        
        T Max() const
        {
            return list.back();
        }
        
        const T & operator[]( const I i ) const
        {
            return list[i];
        }
        
        friend bool IntersectingQ( const SortedList<T,I> & a, const SortedList<T,I> & b )
        {
            if( (a.Max() < b.Min()) || (b.Max() < a.Min()) )
            {
                return false;
            }
            else
            {
                //https://www.geeksforgeeks.org/union-and-intersection-of-two-sorted-arrays-2/
                I i = 0;
                I j = 0;
                const I m = a.Size();
                const I n = b.Size();
                
                
                while (i < m && j < n)
                {
                    if (a[i] < b[j])
                    {
                        i++;
                    }
                    else
                    {
                        if (b[j] < a[i])
                        {
                            j++;
                        }
                        else /* if a[i] == b[j] */
                        {
                            return true;
                        }
                    }
                }
                return false;
            }
        }
        
        friend void Intersection(
            const SortedList<T,I> & a,
            const SortedList<T,I> & b,
                  SortedList<T,I> & c
        )
        {
            c.Clear();
            
            //https://www.geeksforgeeks.org/union-and-intersection-of-two-sorted-arrays-2/

            const I m = a.Size();
            const I n = b.Size();
            
            I i = 0;
            I j = 0;
            
            while (i < m && j < n)
            {
                const T a_i = a[i];
                const T b_j = b[j];
                
                if (a_i < b_j)
                {
                    ++i;
                }
                else
                {
                    if (b_j < a_i)
                    {
                        j++;
                    }
                    else /* if a[i] == b[j] */
                    {
                        c.Insert(a_i);
                        ++i;
                        ++j;
                        
                    }
                }
            }
        }
        
        friend bool operator==(
            const SortedList<T,I> & a,
            const SortedList<T,I> & b
        )
        {
            if( a.Size() != b.Size() )
            {
                return false;
            }
            
            const I n = a.Size();
            for( I i = 0; i < n; ++i )
            {
                if( a[i] != b[i] )
                {
                    return false;
                }
            }
            
            return true;
        }
        
        friend bool operator!=(
            const SortedList<T,I> & a,
            const SortedList<T,I> & b
        )
        {
            if( a.Size() != b.Size() )
            {
                return true;
            }
            
            const I n = a.Size();
            for( I i = 0; i < n; ++i )
            {
                if( a[i] != b[i] )
                {
                    return true;
                }
            }
            
            return false;
        }
        
        std::string ToString() const
        {
            std::stringstream s;
            
            const std::size_t n = list.size();
            
            s << "{ ";
            if( n > 0 )
            {
                s<< Tools::ToString(list[0]);

                for( std::size_t i = 1; i < n; ++i )
                {
                    s <<", " << Tools::ToString(list[i]);
                }
            }
            s<< " }";
            
            return s.str();
        }
        
    };

} //namespace Tools
