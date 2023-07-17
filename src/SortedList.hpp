
namespace Tools
{
    template<typename T, typename Int>
    class SortedList
    {
        ASSERT_SIGNED_INT(Int)
        
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
        
        explicit SortedList( const Int n )
        {
            Reserve(n);
        }
        
        void Clear()
        {
            list.clear();
        }
        
        void Reserve( const Size_T n )
        {
            list.reserve(n);
        }


        Int FindPosition( cref<T> X ) const
        {
            //Finds first position whose element is not smaller than X.

            return static_cast<Int>(
                std::distance(
                    list.begin(),
                    std::lower_bound(list.begin(), list.end(), X)
                )
            );
        }

        
        Int Find( cref<T> X ) const
        {
            if( Size() == 0 )
            {
                return -1;
            }
            
            Int i = FindPosition(X);

            return (list[i] == X) ? i : -1;
        }
        
        bool Insert( cref<T> X )
        {
            Int i = FindPosition(X);
            
            if( i == Size() )
            {
                list.push_back(X);
                return true;
            }
            
            if( list[i] != X )
            {
                list.insert( list.begin()+i, X );
                return true;
            }
            else
            {
                return false;
            }
            
        }
        
        bool Drop( cref<T> X )
        {
            Int i = Find(X);
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
        
        Int Size() const
        {
            return static_cast<Int>(list.size());
        }
        
        T Min() const
        {
            return list.front();
        }
        
        T Max() const
        {
            return list.back();
        }
        
        const T & operator[]( const Int i ) const
        {
            return list[i];
        }
        
        friend bool IntersectingQ(
            const SortedList<T,Int> & restrict a,
            const SortedList<T,Int> & restrict b
        )
        {
            if( (a.Max() < b.Min()) || (b.Max() < a.Min()) )
            {
                return false;
            }
            else
            {
                //https://www.geeksforgeeks.org/union-and-intersection-of-two-sorted-arrays-2/
                Int i = 0;
                Int j = 0;
                const Int m = a.Size();
                const Int n = b.Size();
                
                
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
            const SortedList<T,Int> & restrict a,
            const SortedList<T,Int> & restrict b,
                  SortedList<T,Int> & restrict c
        )
        {
            c.Clear();
            
            //https://www.geeksforgeeks.org/union-and-intersection-of-two-sorted-arrays-2/

            const Int m = a.Size();
            const Int n = b.Size();
            
            Int i = 0;
            Int j = 0;
            
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
            const SortedList<T,Int> & restrict a,
            const SortedList<T,Int> & restrict b
        )
        {
            if( a.Size() != b.Size() )
            {
                return false;
            }
            
            const Int n = a.Size();
            for( Int i = 0; i < n; ++i )
            {
                if( a[i] != b[i] )
                {
                    return false;
                }
            }
            
            return true;
        }
        
        friend bool operator!=(
            const SortedList<T,Int> & restrict a,
            const SortedList<T,Int> & restrict b
        )
        {
            if( a.Size() != b.Size() )
            {
                return true;
            }
            
            const Int n = a.Size();
            for( Int i = 0; i < n; ++i )
            {
                if( a[i] != b[i] )
                {
                    return true;
                }
            }
            
            return false;
        }
        
        std::string ToString( const int p = 16) const
        {
            std::stringstream s;
            
            const Size_T n = list.size();
            
            s << "{ ";
            if( n > 0 )
            {
                s<< Tools::ToString(list[0],p);

                for( Size_T i = 1; i < n; ++i )
                {
                    s <<", " << Tools::ToString(list[i],p);
                }
            }
            s<< " }";
            
            return s.str();
        }
        
    };

} //namespace Tools
