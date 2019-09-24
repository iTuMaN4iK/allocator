#include <iostream>
#include <map>
#include <vector>
#include "factor.h"
#include "lib.h"

#define UNUSED(variable) (void)variable


template <typename T>
struct my_allocator{
    using value_type = T;

    using pointer = T*;
    using const_pointer = const T*;
    using reference =T&;
    using const_reference =T&;
    T * memory;
    template<typename U>
    struct rebind {
        using other = my_allocator<U>;
    };
    
    my_allocator(){
        memory=reinterpret_cast<T *>(std::malloc(10*sizeof(T)));
    };
    ~my_allocator()=default;

    template<typename U> 
    my_allocator(const my_allocator<U>&) {

    }

    T *allocate(std::size_t n){
#ifdef USE_PRETTY
        std::cout << "allocate: [n = " << n << "]" << std::endl;
#endif 
        UNUSED(n);
        return memory++;
    }
    void deallocate(T *p,std::size_t n){
#ifdef USE_PRETTY
        std::cout << "deallocate: [n  = " << n << "] " << std::endl;
#endif
        UNUSED(p);
        UNUSED(n);
    }
    template<typename U, typename ...Args>
    void construct(U *p, Args &&...args){
#ifdef USE_PRETTY
        std::cout << "construct" << std::endl;
#endif
        new(p) U(std::forward<Args>(args)...);
    }
    void destroy(T *p){
#ifdef USE_PRETTY
        std::cout << "destroy" << std::endl;
#endif
        p->~T();
    }
};

template <typename T, typename A = std::allocator<T>>
class MyContener{

public:
    struct MyNode{
        T value;
        MyNode* next;
        MyNode()=default;
        MyNode(const T& _value):value(_value),next(nullptr){};
    };
    struct MyIterator{
        public:
        //MyIterator()=default;
        MyIterator(MyNode* node):node_ptr(node){}
        T& operator*(){
            return node_ptr->value;
        }
        MyIterator& operator++(){
            if (node_ptr){
                node_ptr=node_ptr->next;
            }
            return *this;
        }
        bool operator!=(const MyIterator & rhs){
            return node_ptr!=rhs.node_ptr;
        }
        private:
        MyNode* node_ptr;
    };
    MyIterator begin(){
        return MyIterator(head);
    }
    MyIterator end(){
        return MyIterator(nullptr);
    }
    MyContener():head(nullptr),allocator(NodeAllocatorType()){};
    ~MyContener(){
        while(head){
            MyNode* nextNode=head->next;
            std::allocator_traits<NodeAllocatorType>::destroy(allocator,head);
            std::allocator_traits<NodeAllocatorType>::deallocate(allocator,head,1);
            head=nextNode;
        }
    };
    void add(const T& value){
        MyNode* newNode= std::allocator_traits<NodeAllocatorType>::allocate(allocator,1);
        std::allocator_traits<NodeAllocatorType>::construct(allocator,newNode,value);
        if(head==nullptr){
            head=newNode;
        }else{
            MyNode* mn=head;
            while (mn->next)
            {
                mn=mn->next;
            }
            mn->next=newNode;
        }
    }
private:
    MyNode* head;
    using NodeAllocatorType=typename std::allocator_traits<A>::template rebind_alloc<MyNode>;
    NodeAllocatorType allocator;
};
template <typename T, typename A = std::allocator<T>>
void fillMyContener(MyContener<T,A> & m){
    for (int i=0;i<10;i++){
        m.add(i);
    }
}
template <typename T,typename U,typename X,typename Y>
void fillMap(std::map<T,U,X,Y>&m){
    for (int i{};i<10;i++){
        m[i]=factor(i);
    }
}

template <typename T,typename U,typename X,typename Y>
void printMap(const std::map<T,U,X,Y> & m){
    for (const auto & i:m){
        std::cout<<i.first<<" "<<i.second<<std::endl;
    }
}

int main(int, char const *[])
{
    std::map<int,int>stdAllocMap;
    fillMap(stdAllocMap);
    auto MyAllocMap=std::map<int,int,std::less<int>,my_allocator<std::pair<const int,int>>>{};
    fillMap(MyAllocMap);
    printMap(MyAllocMap);
    auto myC=MyContener<int>{};
    auto myC2=MyContener<int,my_allocator<int>>{};
    fillMyContener(myC);
    fillMyContener(myC2);
    for (const auto & i:myC2){
        std::cout<<i<<" ";
    }
    std::cout<<std::endl;
    return 0;
}
